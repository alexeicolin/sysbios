/*
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 *  ======== dov.c ========
 *  Stackable "OVerlap" Driver
 *
 *  This stacking driver can be used to input buffers whose first
 *  n bytes are copies of the last n bytes of the previous buffer.
 *
 *  CONFIGURATION:
 *
 *  #include <dov.h>
 *
 *  driver DOV {
 *      "/overlap" :    devid = -1;
 *  };
 *
 *  USAGE:
 *
 *  An integer 'nbytes' is appended to the device name and decoded by
 *  DOV_open().  It is used to determine the number of the bytes to
 *  be retained from the end of each buffer and copied into the front
 *  of the next buffer.
 *
 *  For example:
 *
 *      stream = SIO_create("/overlap8/a2d", SIO_INPUT, 1024, NULL);
 *
 *  will open a stacking overlap device which will copy the last 8 bytes
 *  of the previous buffer into the first 8 bytes of the next buffer.
 *  The "/a2d" device will be the source of the data, since "/overlap8"
 *  is prepended to "/a2d".
 *
 *  CONSTRAINTS:
 *
 *  DOV devices cannot be opened for DEV_OUTPUT.
 *
 *  The overlap size in bytes (nbytes) must be greater than 0 and less
 *  than the bufsize parameter to SIO_create().
 *  
 *  This driver does not support any control calls.  All control calls
 *  are passed to the underlying device.
 *
 */

#include <ti/bios/include/std.h>
#include <stddef.h>

#include <ti/bios/include/dev.h>
#include <ti/bios/include/mem.h>
#include <ti/bios/include/que.h>
#include <ti/bios/include/sem.h>
#include <ti/bios/include/sys.h>

#include <ti/bios/include/dov.h>

#include <ctype.h>
#include <string.h>
#include <stdlib.h>

typedef struct DOV_CopyObj {
    DEV_Obj     dobj;           /* should be first field for efficiency */

    Char        *overlap;       /* buffer containing previous data */
    size_t      size;           /* size of overlap */
} DOV_CopyObj;

static Int      DOV_close(DEV_Handle device);
static Int      DOV_ctrl(DEV_Handle device, Uns cmd, Arg arg);
static Int      DOV_idle(DEV_Handle device, Bool flush);
static Int      DOV_open(DEV_Handle device, String name);
static Bool     DOV_ready(DEV_Handle device, SEM_Handle sem);
static Int      DOV_issue(DEV_Handle device);
static Int      DOV_reclaim(DEV_Handle device);

/*
 *  Driver function table.
 */
__FAR__ DEV_Fxns DOV_FXNS = {
    DOV_close,          /* close */
    DOV_ctrl,           /* ctrl */
    DOV_idle,           /* idle */
    DOV_issue,          /* issue */
    DOV_open,           /* open */
    DOV_ready,          /* ready */
    DOV_reclaim,        /* reclaim */
};

static Void setup_todevice(DEV_Handle device);
static Void unsetup_todevice(DEV_Handle device);
static Void setup_fromdevice(DEV_Handle device);

/*
 *  ======== DOV_close ========
 */
static Int DOV_close(DEV_Handle device)
{
    DOV_CopyObj *copy = (DOV_CopyObj *)device->object;
    DEV_Frame   *frame;

    /* close underlying device(s) */
    DEV_close(&(copy->dobj));

    /* move frames _up_ from downstream device */
    while (!QUE_empty(copy->dobj.todevice)) {
        frame = QUE_get(copy->dobj.todevice);

        frame->size = frame->size + copy->size;
        frame->addr = (Char *)frame->addr - copy->size;

        QUE_put(device->todevice, frame);
    }

    QUE_delete(copy->dobj.todevice);
    QUE_delete(copy->dobj.fromdevice);

    /* free overlap buffer */
    MEM_free(0, copy->overlap, copy->size);

    /* recycle copy object */ 
    MEM_free(0, copy, sizeof(DOV_CopyObj));

    return (SYS_OK);
}

/*
 *  ======== DOV_ctrl ========
 */
static Int DOV_ctrl(DEV_Handle  device, Uns cmd, Arg arg)
{
    DOV_CopyObj *copy = (DOV_CopyObj *)device->object;

    return (DEV_ctrl(&(copy->dobj), cmd, arg));
}

/*
 *  ======== DOV_idle ========
 */
static Int DOV_idle(DEV_Handle device, Bool flush)
{
    DOV_CopyObj *copy = (DOV_CopyObj *)device->object;

    return (DEV_idle(&(copy->dobj), flush));
}

/*
 *  ======== DOV_issue ========
 */
static Int DOV_issue(DEV_Handle device)
{
    DOV_CopyObj *copy = (DOV_CopyObj *)device->object;
    Int         status;

    setup_todevice(device);

    status = DEV_issue(&(copy->dobj));

    if (status != SYS_OK) {
        unsetup_todevice(device);
    }

    return (status);
}

/*
 *  ======== DOV_open ========
 */
static Int DOV_open(DEV_Handle device, String name)
{
    DOV_CopyObj *copy;
    DEV_Device  *entry;
    Int         status = SYS_EALLOC;
    DEV_Frame   *frame;
    size_t      size;

    if (device->mode != DEV_INPUT) {
        return (SYS_EINVAL);
    }

    /*
     * If devid is nonzero, it holds the 'size' of the overlap buffer.
     */
    if (device->devid > 0) {
        size = device->devid;
    }
    else {
        size = atoi(name);

        /*
         * Skip the numeric characters to get to the underlying
         * device's name.
         */
        while (isdigit(*name)) {
            name++;
        }
    }

    if (size <= 0 || size >= device->bufsize) {
        return (SYS_EINVAL);
    }

    /*
     * find underlying device in device table
     */
    name = DEV_match(name, &entry);
    if (entry == NULL) {
        return (SYS_ENODEV);
    }

    /* allocate copy object */
    if ((copy = MEM_alloc(0, sizeof(DOV_CopyObj), 0)) == MEM_ILLEGAL) {
        return (SYS_EALLOC);
    }

    copy->size = size;

    /* allocate and initialize overlap buffer */
    if ((copy->overlap = MEM_valloc(0, size, 0, DOV->INITIAL)) == MEM_ILLEGAL) {
        goto e1;
    }

    copy->dobj = *device;       /* copy descriptor fields */
    copy->dobj.fxns = *(DEV_Fxns *)(entry->fxns);
    copy->dobj.devid = entry->devid;
    copy->dobj.params = entry->params;

    /* size of underlying buffers */
    copy->dobj.bufsize = device->bufsize - size;

    /*
     * create queues and frames for underlying device.
     */
    if ((copy->dobj.todevice = QUE_create(NULL)) == NULL) {
        goto e2;
    }
    if ((copy->dobj.fromdevice = QUE_create(NULL)) == NULL) {
        goto e3;
    }

    /*
     * adjust frame size and address according to the overlap size before
     * copying frames to underlying device's 'todevice' queue
     */
    while (!QUE_empty(device->todevice)) {
        frame = QUE_get(device->todevice);

        frame->size = frame->size - size;
        frame->addr = (Char *)frame->addr + size;

        QUE_put(copy->dobj.todevice, frame);
    }

    /* open underlying device */
    if ((status = DEV_open((&copy->dobj), name)) != SYS_OK) {
        goto e4;
    }

    device->object = (Ptr)copy;

    return (SYS_OK);            /* all is well */


    /* free memory and return error code */
e4:
    QUE_delete(copy->dobj.fromdevice);
e3:
    QUE_delete(copy->dobj.todevice);
e2:
    MEM_free(0, copy->overlap, copy->size);
e1:
    MEM_free(0, copy, sizeof(DOV_CopyObj));

    return (status);
}

/*
 *  ======== DOV_ready ========
 */
static Bool DOV_ready(DEV_Handle device, SEM_Handle sem)
{
    DOV_CopyObj *copy = (DOV_CopyObj *)device->object;

    return (DEV_ready((&copy->dobj), sem));
}

/*
 *  ======== DOV_reclaim ========
 */
static Int DOV_reclaim(DEV_Handle device)
{
    DOV_CopyObj *copy = (DOV_CopyObj *)device->object;
    Int status;

    status = DEV_reclaim(&(copy->dobj));
    if (status == SYS_OK) {
        setup_fromdevice(device);
    }

    return (status);
}

/*
 *  ======== setup_todevice ========
 *  Setup the underlying device's todevice frame.
 *  Common code for DOV_input() before the DEV_input() call and for
 *  DOV_issue() before the DEV_issue() call.
 */
static Void setup_todevice(DEV_Handle device)
{
    DOV_CopyObj *copy = (DOV_CopyObj *)device->object;
    DEV_Frame   *frame;
    
    /* get empty frame from upstream queue */
    frame = QUE_get(device->todevice);

    frame->addr = (Char *)frame->addr + copy->size;
    frame->size = frame->size - copy->size;

    QUE_put(copy->dobj.todevice, frame);
}

/*
 *  ======== unsetup_todevice ========
 *  Undo the setup done for the underlying device's todevice frame.
 *  Called if the underlying device's DEV_issue() fails.
 */
static Void unsetup_todevice(DEV_Handle device)
{
    DOV_CopyObj *copy = (DOV_CopyObj *)device->object;
    DEV_Frame   *frame;
    
    /* get empty frame from downstream queue */
    frame = QUE_get(copy->dobj.todevice);

    frame->addr = (Char *)frame->addr - copy->size;
    frame->size = frame->size + copy->size;

    QUE_put(device->todevice, frame);
}

/*
 *  ======== setup_fromdevice ========
 *  Setup the fromdevice frame for the upper layer (probably SIO).
 *  Common code for DOV_input() after the DEV_input() call and for
 *  DOV_reclaim() after the DEV_reclaim() call.
 */
static Void setup_fromdevice(DEV_Handle device)
{
    DOV_CopyObj *copy = (DOV_CopyObj *)device->object;
    DEV_Frame   *frame;

    frame = QUE_get(copy->dobj.fromdevice);

    frame->addr = (Char *)frame->addr - copy->size;
    frame->size = frame->size + copy->size;

    /*
     * copy bytes from end of previous buffer into current buffer
     */
    memcpy(frame->addr, copy->overlap, copy->size);

    /*
     * copy bytes at end of current buffer into overlap buffer 
     */
    memcpy(copy->overlap,
        (Char *)frame->addr + frame->size - copy->size, copy->size);

    QUE_put(device->fromdevice, frame);
}
