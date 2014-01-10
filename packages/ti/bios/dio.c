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
 *  ======== dio.c ========
 *
 *  DSP/BIOS SIO device driver (DEV) which interfaces to IOM mini-
 *  drivers. See the DSP/BIOS Users Guide for more information on SIO
 */

#include <ti/bios/include/std.h>

#include <ti/bios/include/dev.h>
#include <ti/bios/include/sys.h>
#include <ti/bios/include/que.h>
#include <ti/bios/include/sem.h>
#include <ti/bios/include/mem.h>

#include <ti/bios/include/iom.h>
#include <ti/bios/include/dio.h>

Int     DIO_issue(DEV_Handle device);

/*
 *  ======== DIO_ctrl ========
 *  Simply call down to the mini-driver.
 */
Int DIO_ctrl(DEV_Handle device, Uns cmd, Arg arg)
{
    Int status;

    DIO_Handle dio = (DIO_Handle)device->object;

    status = dio->fxns->mdControlChan(dio->chanp, cmd, (Ptr)arg);

    if (status < 0 ) {
        return (SYS_EBADIO);
    }
    else {
        return (SYS_OK);
    }
}

/*  ======== DIO_init ========
 *
 */
Void DIO_init(Void)
{
    /*
     *  Module initialization
     */
}

Int     DIO_cbIdle(DEV_Handle device, Bool flush);
Int     DIO_cbIssue(DEV_Handle device);
Bool    DIO_cbReady(DEV_Handle device, SEM_Handle sem);
Int     DIO_cbReclaim(DEV_Handle device);
Void    DIO_cbCallback(Ptr devp, DEV_Frame *frame);

/*
 *  ======== DIO_cbCallback ========
 */
Void DIO_cbCallback(Ptr devp, DEV_Frame *frame)
{
    DEV_Handle  device = (DEV_Handle)devp;
    DIO_Handle  dio = (DIO_Handle)device->object;

    if (frame->cmd == IOM_READ || frame->cmd == IOM_WRITE) {
        QUE_put(device->fromdevice, frame); 
        dio->context.cb.fxn(dio->context.cb.arg0,
            dio->context.cb.arg1);
    }
}

/*
 *  ======== DIO_cbIdle ========
 *  DIO_cbIdle() simply return SYS_OK
 *  which means it really is a NOP.
 */
Int DIO_cbIdle(DEV_Handle device, Bool flush)
{
    return (SYS_OK);
}

/*
 *  ======== DIO_cbIssue ========
 */
Int DIO_cbIssue(DEV_Handle device)
{
    DIO_Handle dio = (DIO_Handle)device->object;
    DEV_Frame   *frame;
    Int         status;

    frame = QUE_get(device->todevice);

    frame->cmd = (device->mode == DEV_INPUT) ? IOM_READ : IOM_WRITE;
    frame->status = IOM_PENDING;

    status = dio->fxns->mdSubmitChan(dio->chanp, frame);

    if (status < 0) {
        return (SYS_EBADIO);
    }
    else {
        if (status == IOM_COMPLETED) {
            DIO_cbCallback(device, frame);
        }
    
        return (SYS_OK);
    }

}

/*
 *  ======== DIO_cbReady ========
 */
Bool DIO_cbReady(DEV_Handle device, SEM_Handle sem)
{
    return (!(QUE_empty(device->fromdevice)));
}

/*
 *  ======== DIO_cbReclaim ========
 *  This function is expecting at least one buffer ready to be
 *  processed from the fromdevice queue.
 *  If there are no buffers ready then it will return an error.
 *  If more than one buffer is ready, it will call the callback function.
 */
Int DIO_cbReclaim(DEV_Handle device)
{
    DIO_Handle  dio = (DIO_Handle)device->object;
    QUE_Handle  queElem;

    queElem = device->fromdevice->next;

    if (queElem == device->fromdevice) {
        return (SYS_EBADIO);
    }

    if (queElem->next != device->fromdevice) {
        dio->context.cb.fxn(dio->context.cb.arg0,
            dio->context.cb.arg1);
    }

    return (SYS_OK);
}


static DIO_Handle cbMkPort(DEV_Handle device, String name);
static Void cbRmPort(DIO_Handle dio);

/*
 *  ======== cbMkPort ========
 *  Creates a DIO object and binds the controller.
 */
static DIO_Handle cbMkPort(DEV_Handle device, String name)
{
    DIO_Params *params = (DIO_Params *)device->params;
    DEV_Callback  *callback = (DEV_Callback *)device->callback;
    DIO_Handle dio;
    DEV_Device  *entry;
    Uns         mode;
    Int         status;

    /* callback must not be NULL if using this version of DIO */
    if (callback == NULL) {
        return (NULL);
    }
    
    /* params must contain name of mini-driver */
    if (params == NULL) {
        return (NULL);
    }
    
    /*
     * check to see that name of mini-driver matches one in the device table
     * and its type is of DEV_IOMTYPE.
     */
    (void)DEV_match(params->name, &entry);
    if (entry == NULL || entry->type != DEV_IOMTYPE) {
        return (NULL);
    }

    /* allocate 0-initialized dio object */
    if ((dio = MEM_calloc(0, sizeof(DIO_Obj), 0)) == MEM_ILLEGAL) {
        return (NULL);
    }

    /* initialize the DIO callback values */
    dio->context.cb = *callback;

    dio->fxns = (IOM_Fxns *)entry->fxns;

    mode = (device->mode == DEV_INPUT) ? IOM_INPUT : IOM_OUTPUT;

    /* create a channel from the mini-driver */
    status = dio->fxns->mdCreateChan(&dio->chanp, entry->devp, name, mode,
                params->chanParams, DIO_cbCallback, device); 

    if (status != IOM_COMPLETED) {
        cbRmPort(dio);
        return (NULL);
    }

    return (dio);
}

/*
 *  ======== cbRmPort ========
 *  Remove a DIO object and cleans up
 */
static Void cbRmPort(DIO_Handle dio)
{
    /* if chanp not NULL, must delete mini-driver channel */
    if (dio->chanp != NULL) {
        dio->fxns->mdDeleteChan(dio->chanp);
    }

    MEM_free(0, dio, sizeof(DIO_Obj));
}

/*
 *  ======== DIO_cbDynamicClose ========
 *  DIO_cbDynamicClose() can only be called if the user knows that 
 *  all frames have been reclaimed and there are no pending frames.
 */
Int DIO_cbDynamicClose(DEV_Handle device)
{
    DIO_Handle dio = (DIO_Handle)device->object;

    cbRmPort(dio);

    return (SYS_OK);
}

/*
 *  ======== DIO_cbDynamicOpen ========
 */
Int DIO_cbDynamicOpen(DEV_Handle device, String name)
{
    /* allocates DIO_Obj and creates mini-driver channel */
    if ((device->object = (Ptr)cbMkPort(device, name)) != NULL) {
        return (SYS_OK);
    }
    else {
        return (SYS_EBADIO);
    }
}

/*
 *  ======== DIO_cbStaticClose ========
 *  DIO_cbStaticClose() should never be called.
 */
Int DIO_cbStaticClose(DEV_Handle device)
{
    return (SYS_EBADIO);
}

Int     DIO_tskIdle(DEV_Handle device, Bool flush);
Int     DIO_tskIssue(DEV_Handle device);
Bool    DIO_tskReady(DEV_Handle device, SEM_Handle sem);
Int     DIO_tskReclaim(DEV_Handle device);
Void    DIO_tskCallback(Ptr devp, DEV_Frame *frame);

/*
 *  ======== DIO_tskCallback ========
 */
Void DIO_tskCallback(Ptr devp, DEV_Frame *frame)
{
    DEV_Handle  device = (DEV_Handle)devp;
    DIO_Handle  dio = (DIO_Handle)device->object;

    if (frame->cmd == IOM_READ || frame->cmd == IOM_WRITE) {
        QUE_put(device->fromdevice, frame); 

        SEM_post(dio->context.sems.complete);

        /*
         * If semaphore was registered with DIO_ready(), dio->ready will
         * be non-NULL.  In this case, SIO_select() is probably waiting for
         * this semaphore to be posted by the first ready device.
         */
        if (dio->context.sems.ready) {
            SEM_post(dio->context.sems.ready);
        }
    }
    else {
        SEM_post(dio->context.sems.complete);
    }
}

/*
 *  ======== DIO_tskIdle ========
 *  DIO_tskIdle() puts the device back to the state it was in just after
 *  DIO_open() was called.
 */
Int DIO_tskIdle(DEV_Handle device, Bool flush)
{
    DIO_Handle dio = (DIO_Handle)device->object;
    Uns         pendCount = 0;
    DEV_Frame   localFrame;
    Int         status;

    localFrame.status = IOM_PENDING;

    if (device->mode == DEV_INPUT || flush) {
        localFrame.cmd = IOM_ABORT;
    }
    else {
        localFrame.cmd = IOM_FLUSH;
    }

    status = dio->fxns->mdSubmitChan(dio->chanp, &localFrame);

    if (status == IOM_PENDING) {
        for (;;) {
            if (SEM_pend(dio->context.sems.complete, device->timeout)) {
                /*
                 * Break out of the for loop when the local abort/flush
                 * frame status shows that it is complete.
                 */
                if (localFrame.status == IOM_COMPLETED) {
                    break;
                }
                else {
                    /* keep track of data frame completions */
                    pendCount++;
                }
            }
            else {
                return (SYS_ETIMEOUT);
            }
        }

        /* 
         * Update complete semaphore so it corresponds to number of frames
         * on the 'todevice' queue.  'for' loop above may have made the 
         * complete sem count inconsistent.
         */
        while (pendCount--) {
            SEM_post(dio->context.sems.complete);
        }
    }

    return (SYS_OK);
}

/*
 *  ======== DIO_tskIssue ========
 */
Int DIO_tskIssue(DEV_Handle device)
{
    DIO_Handle dio = (DIO_Handle)device->object;
    DEV_Frame   *frame;
    Int         status;

    frame = QUE_get(device->todevice);

    frame->cmd = (device->mode == DEV_INPUT) ? IOM_READ : IOM_WRITE;
    frame->status = IOM_PENDING;

    status = dio->fxns->mdSubmitChan(dio->chanp, frame);

    if (status < 0) {
        return (SYS_EBADIO);
    }
    else {
        if (status == IOM_COMPLETED) {
            QUE_put(device->fromdevice, frame);
            SEM_post(dio->context.sems.complete);
        }
        
        return (SYS_OK);
    }
}

/*
 *  ======== DIO_tskReady ========
 */
Bool DIO_tskReady(DEV_Handle device, SEM_Handle sem)
{
    DIO_Handle  dio = (DIO_Handle)device->object;

    dio->context.sems.ready = sem;

    return (!(QUE_empty(device->fromdevice)));
}

/*
 *  ======== DIO_tskReclaim ========
 */
Int DIO_tskReclaim(DEV_Handle device)
{
    DIO_Handle  dio = (DIO_Handle)device->object;

    /*
     * Wait here if there are no buffers on the device->fromdevice
     * queue.
     */
    if (SEM_pend(dio->context.sems.complete, device->timeout)) {
        return (SYS_OK);
    }
    else {
        return (SYS_ETIMEOUT);
    }
}


static DIO_Handle tskMkPort(DEV_Handle device, String name);
static Void tskRmPort(DIO_Handle dio);

/*
 *  ======== tskMkPort ========
 *  Creates a DIO object and binds the controller.
 */
static DIO_Handle tskMkPort(DEV_Handle device, String name)
{
    DIO_Params *params = (DIO_Params *)device->params;
    DIO_Handle dio;
    DEV_Device  *entry;
    Uns         mode;
    Int         status;

    /* params should contain name of mini-driver */
    if (params == NULL) {
        return (NULL);
    }
    
    /*
     * check to see that name of mini-driver matches one in the device table
     * and its type is of DEV_IOMTYPE.
     */
    (void)DEV_match(params->name, &entry);
    if (entry == NULL || entry->type != DEV_IOMTYPE) {
        return (NULL);
    }

    /* allocate 0-initialized dio object */
    if ((dio = MEM_calloc(0, sizeof(DIO_Obj), 0)) == MEM_ILLEGAL) {
        return (NULL);
    }

    /*
     * Tasks will pend on dio->complete if there are no available frames on
     * the fromdevice queue.
     */
    dio->context.sems.complete = SEM_create(0, NULL);

    /* make sure SEM_create() succeeded ... */
    if (dio->context.sems.complete == NULL) {
        MEM_free(0, dio, sizeof(DIO_Obj));     /* free dio object */
        return (NULL);
    }

    dio->fxns = (IOM_Fxns *)entry->fxns;

    mode = (device->mode == DEV_INPUT) ? IOM_INPUT : IOM_OUTPUT;

    /* create a channel from the mini-driver */
    status = dio->fxns->mdCreateChan(&dio->chanp, entry->devp, name, mode,
                params->chanParams, DIO_tskCallback, device); 

    if (status != IOM_COMPLETED) {
        tskRmPort(dio);
        return (NULL);
    }

    return (dio);
}

/*
 *  ======== tskRmPort ========
 *  Removes a DIO object and cleans up
 */
static Void tskRmPort(DIO_Handle dio)
{
    /* if chanp not NULL, must delete mini-driver channel */
    if (dio->chanp != NULL) {
        dio->fxns->mdDeleteChan(dio->chanp);
    }

    /* remove the semaphore ... */
    SEM_delete(dio->context.sems.complete);

    /* and finally the object */
    MEM_free(0, dio, sizeof(DIO_Obj));
}

/*
 *  ======== DIO_tskDynamicClose ========
 *  DIO_tskIdle() should be called before DIO_tskDynamicClose().
 */
Int DIO_tskDynamicClose(DEV_Handle device)
{
    DIO_Handle  dio = (DIO_Handle)device->object;

    /*
     * All frames should have been returned by mini-driver since SIO should
     * have already called DIO_idle().
     */
    tskRmPort(dio);

    return (SYS_OK);
}

/*
 *  ======== DIO_tskDynamicOpen ========
 */
Int DIO_tskDynamicOpen(DEV_Handle device, String name)
{
    /* allocates DIO_Obj and creates mini-driver channel */
    if ((device->object = (Ptr)tskMkPort(device, name)) != NULL) {
        return (SYS_OK);
    }
    else {
        return (SYS_EBADIO);
    }
}

/*
 *  ======== DIO_tskStaticClose ========
 *  DIO_tskStaticClose() should never be called.
 */
Int DIO_tskStaticClose(DEV_Handle device)
{
    return (SYS_EBADIO);
}



