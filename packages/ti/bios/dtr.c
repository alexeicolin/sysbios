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
 *  ======== dtr.c ========
 *  Stacking Transformer Driver
 *
 *  This driver may be stacked on top of any streaming device and will
 *  transform data to or from the device from fix point to floating point
 *  or vice versa.  A user supplied function (devid = DTR_USER) may be 
 *  specified in params to allow arbitrary "in place" transformation/
 *  conditioning of streaming data.
 */

#include <ti/bios/include/std.h>

#include <ti/bios/include/dev.h>
#include <ti/bios/include/que.h>
#include <ti/bios/include/sys.h>
#include <ti/bios/include/fxn.h>
#include <ti/bios/include/mem.h>
#include <ti/bios/include/sem.h>

#include <ti/bios/include/dtr.h>

typedef struct DTR_TransObj {
    DEV_Obj     dobj;           /* MUST be first field */

    Fxn         fxn;            /* fix2flt, flt2fix, multiply, or user */
    Arg         arg;            /* arg for fxn (specified in params) */
} DTR_TransObj;

/*
 *  Default values for this transformer.
 */
__FAR__ DTR_Params DTR_PARAMS = {
    { 1 },              /* scale.value */
    { (Arg)NULL,        /* user.arg */
      (Fxn)NULL },      /* user.fxn */
};

static Int      DTR_close(DEV_Handle device);
static Int      DTR_ctrl(DEV_Handle device, Uns cmd, Arg arg);
static Int      DTR_idle(DEV_Handle device, Bool flush);
static Int      DTR_issue(DEV_Handle device);
static Int      DTR_open(DEV_Handle device, String name);
static Bool     DTR_ready(DEV_Handle device, SEM_Handle sem);
static Int      DTR_reclaim(DEV_Handle device);

/*
 *  Driver function table.
 */
__FAR__ DEV_Fxns DTR_FXNS = {
    DTR_close,          /* close */
    DTR_ctrl,           /* ctrl */
    DTR_idle,           /* idle */
    DTR_issue,          /* issue */
    DTR_open,           /* open */
    DTR_ready,          /* ready */
    DTR_reclaim,        /* reclaim */
};

/*
 *  ======== DTR_close ========
 */
Int DTR_close(DEV_Handle device)
{
    DTR_TransObj        *trans = (DTR_TransObj *)device->object;
    QUE_Handle          queue;

    /* close underlying device(s) */
    DEV_close(&trans->dobj);

    if (device->mode == DEV_INPUT) {
        queue = trans->dobj.fromdevice;
    }
    else {
        queue = trans->dobj.todevice;
    }
    QUE_delete(queue);

    /* recycle trans object */ 
    MEM_free(0, trans, sizeof(DTR_TransObj));   /* free object */
    
    return (SYS_OK);
}

/*
 *  ======== DTR_ctrl ========
 */
Int DTR_ctrl(DEV_Handle device, Uns cmd, Arg arg)
{
    DTR_TransObj        *trans = (DTR_TransObj *)device->object;

    return (DEV_ctrl(&trans->dobj, cmd, arg));
}

/*
 *  ======== DTR_idle ========
 */
Int DTR_idle(DEV_Handle device, Bool flush)
{
    DTR_TransObj        *trans = (DTR_TransObj *)device->object;

    return (DEV_idle(&trans->dobj, flush));
}

/*
 *  ======== DTR_issue ========
 */
Int DTR_issue(DEV_Handle device)
{
    DTR_TransObj        *trans = (DTR_TransObj *)device->object;
    DEV_Frame           *frame;
    Int                 status;
    
    if (device->mode != DEV_INPUT) {    /* DEV_OUTPUT */
        frame = QUE_get(device->todevice);

        (*trans->fxn)(trans->arg, frame->addr, frame->size);

        QUE_put(trans->dobj.todevice, frame);
    }

    status = DEV_issue(&trans->dobj);

    return (status);
}

/*
 *  ======== DTR_open ========
 */
Int DTR_open(DEV_Handle device, String name)
{
    DTR_TransObj        *trans;
    DEV_Device          *entry;
    DTR_Params          *params;
    QUE_Handle          queue;
    Int                 status;

    name = DEV_match(name, &entry);
    if (entry == NULL) {
        SYS_error("DTR", SYS_ENODEV);
        return (SYS_ENODEV);
    }

    /* allocate transformer object */
    if ((trans = MEM_alloc(0, sizeof(DTR_TransObj), 0)) == MEM_ILLEGAL) {
        SYS_error("DTR MEM_alloc", SYS_EALLOC);
        return (SYS_EALLOC);
    }

    trans->dobj = *device;      /* copy descriptor fields */
    trans->dobj.fxns = *(DEV_Fxns *)(entry->fxns);

    /*
     * create 'data' queues.  the 'free' queues (i.e. todevice for
     * input and fromdevice for output) are shared since there
     * is no reason to copy an empty frame from one queue to the
     * next.
     *
     * INPUT:
     *    create new 'fromdevice' in downstream device object.
     * OUTPUT:
     *    create new 'todevice' in downstream device object.
     */
    queue = QUE_create(NULL);
    if (queue == NULL) {
        MEM_free(0, trans, sizeof(DTR_TransObj));       /* free object */
        SYS_error("DTR QUE_create", SYS_EALLOC);

        return (SYS_EALLOC);
    }

    if (device->mode == DEV_INPUT) {
        trans->dobj.fromdevice = queue;
    }
    else {
        trans->dobj.todevice = queue;
    }

    trans->dobj.devid = entry->devid;
    trans->dobj.params = entry->params;

    /* open underlying device */
    if ((status = DEV_open(&trans->dobj, name)) != SYS_OK) {
        QUE_delete(queue);
        MEM_free(0, trans, sizeof(DTR_TransObj));

        return (status);
    }

    params = (device->params) ? (DTR_Params *)device->params : &DTR_PARAMS;

    if (device->devid == NULL) {
        /* DTR_USER */
        if ((trans->fxn = params->user.fxn) == NULL) {
            trans->fxn = (Fxn)FXN_F_nop;
        }
        trans->arg = params->user.arg;
    }
    else {
        trans->fxn = (Fxn)device->devid;
        trans->arg = (Arg)&params->scale.value;
    }

    device->object = (Ptr)trans;

    return (SYS_OK);
}

/*
 *  ======== DTR_ready ========
 */
Bool DTR_ready(DEV_Handle device, SEM_Handle sem)
{
    DTR_TransObj        *trans = (DTR_TransObj *)device->object;

    return (DEV_ready(&trans->dobj, sem));
}

/*
 *  ======== DTR_reclaim ========
 */
Int DTR_reclaim(DEV_Handle device)
{
    DTR_TransObj        *trans = (DTR_TransObj *)device->object;
    DEV_Frame           *frame;
    Int                 status;

    status = DEV_reclaim(&trans->dobj);

    if (device->mode == DEV_INPUT && status == SYS_OK) {
        frame = QUE_get(trans->dobj.fromdevice);

        (*trans->fxn)(trans->arg, frame->addr, frame->size);

        QUE_put(device->fromdevice, frame);
    }

    return (status);
}
/*
 *  ======== DTR_multiply ========
 *
 *  This transformation simply scales the data by an amount
 *  specified when the device was opened.
 *
 *  *addr = *addr * scale
 */
Void DTR_multiply(Arg arg, DTR_Elem *addr, size_t size)
{
    register size_t     i;
    register DTR_Scale  scale;

    scale = *(DTR_Scale *)arg;

    for (i = (size / sizeof(DTR_Elem)); i > 0; i--, addr++) {
        *addr = *addr * scale;
    }
}

#ifndef _FLOAT_
/*
 *  ======== DTR_multiplyInt16 ========
 *
 *  This transformation simply scales the data by an amount
 *  specified when the device was opened.
 *
 *  *addr = *addr * scale
 */
Void DTR_multiplyInt16(Arg arg, DTR_ElemInt16 *addr, size_t size)
{
    register size_t     i;
    register DTR_Scale  scale;

    scale = *(DTR_Scale *)arg;

    for (i = (size / sizeof(DTR_ElemInt16)); i > 0; i--, addr++) {
        *addr = *addr * scale;
    }
}

#endif

/*
 *  ======== DTR_fix2flt ========
 *
 *  This transformation converts integer data to floating point data.
 *
 *  *addr = (Float)(*addr * scale)
 */
Void DTR_fix2flt(Arg arg, DTR_Elem *addr, size_t size)
{
    register size_t     i;              
    register Int        *iptr;
    register Float      *fptr;
    register DTR_Scale  scale;
    
    scale = *(DTR_Scale *)arg;
    fptr = (Float *)addr;
    iptr = (Int *)addr;

    for (i = (size / sizeof(Float)); i > 0; i--) {
        *fptr++ = ((Float)*iptr++) * scale;
    }
}


/*
 *  ======== DTR_flt2fix ========
 *
 *  This transformation converts floating point data to integer data.
 *
 *  *addr = (Int)(*addr * scale)
 */
Void DTR_flt2fix(Arg arg, DTR_Elem *addr, size_t size)
{
    register size_t     i;
    register Int        *iptr;
    register Float      *fptr;
    register DTR_Scale  scale;

    scale = *(DTR_Scale *)arg;
    fptr = (Float *)addr;
    iptr = (Int *)addr;

    for (i = (size / sizeof(Float)); i > 0; i--) {
        *iptr++ = (Int)(*fptr++ * scale);
    }
}


