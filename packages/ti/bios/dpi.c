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
 *  ======== dpi.c ========
 *  New Pipe Driver
 *
 *  The pipe device is a software device used to stream data between
 *  SPOX tasks.  The old DPI driver accomplished this by creating a
 *  shared pair of queues distinct from the original queues created by
 *  SIO.  This allowed efficient I/O plus the ability to have multiple
 *  readers and writers. This method has some limitations, the most
 *  significant being that stacking drivers won't stack on top of the
 *  old DPI (idling is the other).
 *
 *  The new DPI is a pipe implementation which can have stacking drivers
 *  on top. Instead of criss-crossing device pointers to the shared
 *  queues, the orignal SIO queues are left intact and the input/output
 *  function handles the transferring between the device queues.  This
 *  method has the limitation of only one reader and one writer allowed.
 *  It does correctly handle device idling, with one exception: if the
 *  input side of the pipe is closed before the output side, the output
 *  side will throw away remaining output frames instead of waiting for
 *  them to drain if it's been created with attrs.flush TRUE.
 *
 *  The new DPI also fixes the simultaneous SIO_select problem per MR 2172
 *
 *  As usual, SEM semaphores are used to keep track of the number of
 *  available buffers on each of the fromdevice queues.
 */

#include <ti/bios/include/std.h>
#include <stddef.h>
#include <string.h>

#include <ti/bios/include/dev.h>
#include <ti/bios/include/mem.h>
#include <ti/bios/include/que.h>
#include <ti/bios/include/sem.h>
#include <ti/bios/include/sys.h>
#include <ti/bios/include/tsk.h>

#include <ti/bios/include/dpi.h>

#define COPYBUFS        /* Comment this line for a more efficient version
                           of the DPI driver when using SIO_ISSUERECLAIM. */

#include <stdlib.h>     /* for atoi() */

/*
 *  ======== SPipeObj ========
 * *ONE* SPipeObj is allocated for each pipe (e.g. "/pipe0").  readers
 * and writers adjust their semaphore pointers (in DPI_open()) to use
 * the common pair in SPipeObj.
 */
typedef struct SPipeObj {
    QUE_Elem        link;       /* MUST be first element of this structure */
    SEM_Handle      dataSem;
    SEM_Handle      freeSem;
    SEM_Handle      readySem[2];
    DEV_Handle      device[2];
    Int             id;
} SPipeObj;

/*
 * One PipeObj is allocated for each open device.
 */
typedef struct PipeObj {
    SEM_Handle      toSem;
    SEM_Handle      fromSem;
    SPipeObj        *sPipe;
} PipeObj;

#define Static static

/*
 *  Driver function table.
 */
Static Int DPI_close(DEV_Handle dev);
Static Int DPI_issue(DEV_Handle dev);
Static Int DPI_open(DEV_Handle dev, String name);
Static Bool DPI_ready(DEV_Handle dev, SEM_Handle sem);
Static Int DPI_reclaim(DEV_Handle dev);

__FAR__ DEV_Fxns DPI_FXNS = {
    DPI_close,          /* close */
    DEV_CTRL,           /* ctrl */
    DEV_IDLE,           /* idle */
    DPI_issue,          /* issue */
    DPI_open,           /* open */
    DPI_ready,          /* ready */
    DPI_reclaim,        /* reclaim */
};

Static __FAR__ SEM_Handle mutex;        /* for mutual exclusion in open and close */
Static __FAR__ QUE_Handle sPipeList;    /* list of all shared pipe objects */

Static __FAR__ SPipeObj *mkSPipe(DEV_Handle dev);
Static __FAR__ Void rmSPipe(SPipeObj *sPipe);


/*
 *  ======== DPI_close ========
 */
Static Int DPI_close(DEV_Handle dev)
{
    PipeObj         *pipe = (PipeObj *)dev->object;
    SPipeObj        *sPipe = pipe->sPipe;

    MEM_free(0, pipe, sizeof(PipeObj));
    
    SEM_pend(mutex, SYS_FOREVER);

    sPipe->device[dev->mode] = NULL;
    sPipe->readySem[dev->mode] = NULL;
             
    if (sPipe->device[DEV_INPUT] == NULL &&
                sPipe->device[DEV_OUTPUT] == NULL) {
        /* delete all shared pipe sub-objects */
        SEM_delete(sPipe->dataSem);
        SEM_delete(sPipe->freeSem);

        /* remove sPipe obj from sPipeList */
        QUE_remove(&sPipe->link);
        
        /* delete sPipe object itself */
        MEM_free(0, sPipe, sizeof (SPipeObj));
    }

    SEM_post(mutex);

    return (SYS_OK);
}

/*
 *  ======== DPI_init ========
 */
Void DPI_init(Void)
{
    /* Make sure the initialization happens only once for the DPI driver.*/
    static __FAR__ Bool curInit = FALSE;

    if ( curInit ) {
        return;
    }
    curInit = TRUE;

    mutex = SEM_create(1, NULL);
    sPipeList = QUE_create(NULL);

    if (mutex == NULL || sPipeList == NULL) {
        SYS_abort("DPI");
    }
}
    
/*
 *  ======== DPI_issue ========
 */
Static Int DPI_issue(DEV_Handle dev)
{
    PipeObj         *pipe = (PipeObj *)dev->object;
    SPipeObj        *sPipe = pipe->sPipe;
    DEV_Handle      otherdev = sPipe->device[dev->mode ^ 0x1];
    SEM_Handle      otherReady = sPipe->readySem[dev->mode ^ 0x1];
    DEV_Frame       *otherframe;
    DEV_Frame       *frame;
#ifdef COPYBUFS
    DEV_Frame       *srcframe;
    DEV_Frame       *dstframe;
#endif
    
    /*
     * Atomically check that each side has a frame so we can do an
     * exchange.  We can't be sure that a frame is on the
     * dev->todevice queue (just put there by SIO) since a task
     * switch to the task on the other side might intervene and
     * take the frame from this side.
     */
    TSK_disable();
    if (otherdev != NULL &&
        !QUE_empty(dev->todevice) && !QUE_empty(otherdev->todevice)) {

        otherframe = QUE_get(otherdev->todevice);
        frame = QUE_get(dev->todevice);

        /* done with atomic stuff */
        TSK_enable();

/*
 * #define COPYBUFS to cause buffers to be copied through the pipe
 * instead of being exchanged.  Doing so retains the semantics of
 * the ISSUERECLAIM model, but is slow.  If COPYBUFS is *not* defined,
 * then one side reclaims buffers issued by the other side, thereby
 * not strictly retaining buffer ordering.
 */
#ifdef COPYBUFS
        if (dev->mode == DEV_INPUT) {
            dstframe = frame;
            srcframe = otherframe;
        }
        else {
            dstframe = otherframe;
            srcframe = frame;
        }
        memcpy(dstframe->addr, srcframe->addr, srcframe->size);
        dstframe->size = srcframe->size;
        dstframe->arg = srcframe->arg;

        QUE_put(dev->fromdevice, frame);
        QUE_put(otherdev->fromdevice, otherframe);

        /*
         * frames reclaimed from an output device must have size 0.
         */
        if (dev->mode != DEV_INPUT) {
            frame->size = 0;
        }
        else {
            otherframe->size = 0;
        }
#else
        QUE_put(dev->fromdevice, otherframe);
        QUE_put(otherdev->fromdevice, frame);

        /*
         * frames reclaimed from an output device must have size 0.
         */
        if (dev->mode != DEV_INPUT) {
            otherframe->size = 0;
        }
        else {
            frame->size = 0;
        }
#endif
    }
    else {
        /* done with atomic stuff */
        TSK_enable();
    }

    SEM_post(pipe->toSem);

    if (otherReady != NULL) {
        SEM_post(otherReady);
    }

    return SYS_OK;
}

/*
 *  ======== DPI_open ========
 */
Static Int DPI_open(DEV_Handle dev, String name)
{
    PipeObj         *pipe;
    SPipeObj        *sPipe, *tmpPipe;

    /* decode and validate devid */
    if (dev->devid < 0) {
        dev->devid = atoi(name);
    }

    SEM_pend(mutex, SYS_FOREVER);

    /* search pipe list for previously opened pipe with same id */
    sPipe = MEM_ILLEGAL;
    if (!QUE_empty(sPipeList)) {
        tmpPipe = (SPipeObj *)QUE_head(sPipeList);
        do {
            if (tmpPipe->id == dev->devid) {
                sPipe = tmpPipe;
                break;
            }
            tmpPipe = (SPipeObj *)QUE_next((&tmpPipe->link));
        } while (tmpPipe != (SPipeObj *)sPipeList);
    }

    if (sPipe == MEM_ILLEGAL) {
        /*
         * Allocate and initialize SPipeObj on first open.
         */
        sPipe = mkSPipe(dev);
        if (sPipe == MEM_ILLEGAL) {
            SEM_post(mutex);
            return SYS_EALLOC;
        }
        QUE_put(sPipeList, &sPipe->link);
    }
    else {      /* sPipe found on list */
        if (sPipe->device[dev->mode] != NULL) {
            /*
             * Only one input and one output allowed
             */
            SEM_post(mutex);
            return SYS_EBUSY;
        }
    }
    sPipe->device[dev->mode] = dev;
    SEM_post(mutex);

    pipe = MEM_alloc(0, sizeof (PipeObj), 0);
    if (pipe == MEM_ILLEGAL) {
        /*
         * We need to undo work done by mkSPipe() if first open.
         * Also need to undo changes to sPipeList queue.
         */
        QUE_remove(&sPipe->link);
        rmSPipe(sPipe);
        return SYS_EALLOC;
    }

    /*
     * Criss-cross SEM handles so both sides are referencing
     * the same physical objects.
     */
    if (dev->mode == DEV_INPUT) {
        pipe->fromSem = sPipe->dataSem;
        pipe->toSem = sPipe->freeSem;
    }
    else {
        pipe->toSem = sPipe->dataSem;
        pipe->fromSem = sPipe->freeSem;
    }

    /*
     * Point things around.
     */
    pipe->sPipe = sPipe;
    dev->object = (Ptr)pipe;

    return (SYS_OK);
}

/*
 *  ======== DPI_ready ========
 *  called by SIO_select(), returns TRUE if device is ready.
 */
Static Bool DPI_ready(DEV_Handle dev, SEM_Handle sem)
{
    PipeObj         *pipe = (PipeObj *)dev->object;

    pipe->sPipe->readySem[dev->mode] = sem;
    
    return !(QUE_empty(dev->fromdevice));
}

/*
 *  ======== DPI_reclaim ========
 */
Static Int DPI_reclaim(DEV_Handle dev)
{
    PipeObj         *pipe = (PipeObj *)dev->object;

    if (SEM_pend(pipe->fromSem, dev->timeout)) {
        return (SYS_OK);
    }
    else {
        return (SYS_ETIMEOUT);
    }
}

/*
 *  ======== mkSPipe ========
 */
Static SPipeObj *mkSPipe(DEV_Handle dev)
{
    SPipeObj *sPipe = MEM_alloc(0, sizeof (SPipeObj), 0);
    
    if (sPipe != MEM_ILLEGAL) {
        sPipe->dataSem = SEM_create(0, NULL);
        if (sPipe->dataSem == NULL) {
            rmSPipe(sPipe);
            return (MEM_ILLEGAL);
        }

        sPipe->freeSem = SEM_create(0, NULL);
        if (sPipe->freeSem == NULL) {
            rmSPipe(sPipe);
            return(MEM_ILLEGAL);
        }

        sPipe->readySem[DEV_INPUT] = NULL;
        sPipe->readySem[DEV_OUTPUT] = NULL;
        sPipe->device[DEV_INPUT] = NULL;
        sPipe->device[DEV_OUTPUT] = NULL;
        sPipe->id = dev->devid;

        return (sPipe);
    }
    else {
        return (MEM_ILLEGAL);
    }
}

/*
 *  ======== rmSPipe ========
 */
Static Void rmSPipe(SPipeObj *sPipe)
{

    if (sPipe == MEM_ILLEGAL) {
        return;
    }
    else {
        if (sPipe->dataSem != NULL) {
            SEM_delete(sPipe->dataSem);
        }
        if (sPipe->freeSem != NULL) {
            SEM_delete(sPipe->freeSem);
        }
        MEM_free(0, sPipe, sizeof(SPipeObj));
    }
}


