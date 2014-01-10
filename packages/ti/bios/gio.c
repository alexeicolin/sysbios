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
 *  ======== gio.c ========
 */

#include <ti/bios/include/std.h>
#include <string.h>

#include <ti/bios/include/gio.h>
#include <ti/bios/include/_gio.h>
#include <ti/bios/include/iom.h>
#include <ti/bios/include/sys.h>
#include <ti/bios/include/sem.h>
#include <ti/bios/include/que.h>


__FAR__ GIO_Attrs GIO_ATTRS = {
    2,              /* nPackets */
    SYS_FOREVER,    /* timeout */
};

/*
 *  ======== GIO_init ========
 */
Void GIO_init()
{
}

/*
 *  ======== _GIO_iomCallback ========
 *  This function is called by the mini-driver when I/O completes.
 */
Void _GIO_iomCallback(Ptr cbArg, IOM_Packet *packet)
{
    GIO_Handle          gioChan = (GIO_Handle)cbArg;
    GIO_AppCallback     *appCallback = (GIO_AppCallback *)packet->misc;
    Int                 status;
    Ptr                 addr;
    size_t              size;
 
    if (appCallback == NULL) {
        /* this was a synchronous call -- post semaphore (or alternate sync) */
        GIO->SEMPOST(gioChan->syncObj);
    }
    else {
        status = packet->status;
        addr = packet->addr;
        size = packet->size;
        
        /* recycle packet back onto free list */
        QUE_put(&gioChan->freeList, packet);

        /* callback into application with status and size */
        (*appCallback->fxn)(appCallback->arg, status, addr, size);
    }
}

/*
 *  ======== GIO_control ========
 */
Int GIO_control(GIO_Handle gioChan, Uns cmd, Ptr args)
{
    /* call mini-driver control fxn */
    return (gioChan->fxns->mdControlChan(gioChan->mdChan, cmd, args));
}

/*
 *  ======== GIO_new ========
 */
GIO_Handle GIO_new(GIO_Handle gioChan, String name, Int mode, Int *status, Ptr optArgs,
        IOM_Packet packetBuf[], Ptr syncObject, GIO_Attrs *attrs)
{
    DEV_Device  *entry;
    Int         i;
    Int         tmpStat;

    if (attrs == NULL) {
        attrs = &GIO_ATTRS;
    }

    /*
     * status param is used to pass additional device status back to caller.
     */
    if (status == NULL) {
        status = &tmpStat;    /* no longer need to check if status valid ptr */
    }

    *status = IOM_COMPLETED;
    
    /*
     *  Find device structure in device table for device with name 'name'.
     *  DEV_match() returns the remaining name string for use by the
     *  mini-driver's create() function.
     */
    name = DEV_match(name, &entry);
    if (entry == NULL) {
        SYS_error(name, SYS_ENODEV); /* sys error - no device found */
        return (NULL);
    }
    
    if (entry->type != DEV_IOMTYPE) {
        SYS_error("IOM", SYS_EINVAL); /* sys error - invalid device parameter */
        return (NULL);
    }

    /* initialize queue structures */
    QUE_new(&gioChan->freeList);

    /* zero out the packet buffers */
    memset(packetBuf, 0, attrs->nPackets * sizeof(IOM_Packet));

    /* Put packets into freeList. */
    for (i=0; i < attrs->nPackets; i++) {
        QUE_put(&gioChan->freeList, &packetBuf[i]);
    }

    /*
     * Plug semaphore or other synchronization object.  'gioChan->syncObj' is
     * used to wait for I/O to complete when GIO_submit() is called with
     * NULL *appCallback parameter. 
     */
    gioChan->syncObj = syncObject;

    gioChan->fxns = (IOM_Fxns *)entry->fxns;
    gioChan->mode = mode;
    gioChan->timeout = attrs->timeout;

    *status = gioChan->fxns->mdCreateChan(&gioChan->mdChan, entry->devp,
            name, mode, optArgs, _GIO_iomCallback, gioChan);

    if (gioChan->mdChan == NULL) {
        return (NULL);
    }

    return (gioChan);
}

/*
 *  ======== GIO_create ========
 */
GIO_Handle GIO_create(String name, Int mode, Int *status, Ptr optArgs, \
        GIO_Attrs *attrs)
{
    GIO_Handle  gioChan;
    IOM_Packet  *packet;
    DEV_Device  *entry;
    Int         i;
    Int         tmpStat;

    if (attrs == NULL) {
        attrs = &GIO_ATTRS;
    }

    /*
     * status param is used to pass additional device status back to caller.
     */
    if (status == NULL) {
        status = &tmpStat;    /* no longer need to check if status valid ptr */
    }

    *status = IOM_COMPLETED;
    
    /*
     *  Find device structure in device table for device with name 'name'.
     *  DEV_match() returns the remaining name string for use by the
     *  mini-driver's create() function.
     */
    name = DEV_match(name, &entry);
    if (entry == NULL) {
        SYS_error(name, SYS_ENODEV); /* sys error - no device found */

        return (NULL);
    }
    
    if (entry->type != DEV_IOMTYPE) {
        SYS_error("IOM", SYS_EINVAL); /* sys error - invalid device parameter */

        return (NULL);
    }

    /*  allocate and 0-fill IOM object */
    gioChan = MEM_calloc(0, sizeof(GIO_Obj), 0);
    if (gioChan == NULL) {
        *status = IOM_EALLOC;  
       
        return (NULL);
    }

    /* initialize queue structures */
    QUE_new(&gioChan->freeList);

    /*
     * Allocate packets for asynch I/O.
     */
    for (i=0; i < attrs->nPackets; i++) {

        packet = _GIO_mkPacket();

        if (packet == NULL) {
           
            *status = IOM_EALLOC;

            GIO_delete(gioChan);
            return (NULL);
        }

        QUE_put(&gioChan->freeList, packet);
    }

    /*
     * Create semaphore or other synchronization object.  'gioChan->syncObj' is
     * used to wait for I/O to complete when GIO_submit() is called with
     * NULL *appCallback parameter. 
     */
    gioChan->syncObj = GIO->SEMCREATE(0, NULL);

    if (gioChan->syncObj == NULL) {

        *status = IOM_EALLOC;
 
        GIO_delete(gioChan);
        return (NULL);
    }

    gioChan->fxns = (IOM_Fxns *)entry->fxns;
    gioChan->mode = mode;
    gioChan->timeout = attrs->timeout;

    *status = gioChan->fxns->mdCreateChan(&gioChan->mdChan, entry->devp,
            name, mode, optArgs, _GIO_iomCallback, gioChan);

    if (gioChan->mdChan == NULL) {
        
        GIO_delete(gioChan);
        return (NULL);
    }

    return (gioChan);
}

/*
 *  ======== GIO_delete ========
 */
Int GIO_delete(GIO_Handle gioChan)
{
    IOM_Packet *packet;

    /* flush and delete low-level device ... */
    if (gioChan->fxns != NULL && gioChan->mdChan != NULL) {
        GIO_flush(gioChan);
        gioChan->fxns->mdDeleteChan(gioChan->mdChan);
    }

    /* delete semaphore or alternate sync object ... */
    if (gioChan->syncObj != NULL) {
        GIO->SEMDELETE(gioChan->syncObj);
    }

    /* free frames ... */
    packet = QUE_get(&gioChan->freeList);
    while (packet != (IOM_Packet *)(&gioChan->freeList)) {
        _GIO_rmPacket(packet);
        packet = QUE_get(&gioChan->freeList);
    }

    /* free GIO object. */
    (Void)MEM_free(0, gioChan, sizeof(GIO_Obj));

    return (IOM_COMPLETED);
}

/*
 *  ======== GIO_submit ========
 */
Int GIO_submit(GIO_Handle gioChan, Uns cmd, Ptr bufp, \
         size_t *psize, GIO_AppCallback *appCallback)
{
    Int         status;
    Bool        semStat;
    IOM_Packet  *packet;

    if (appCallback == NULL) {
        /* synchronous operation, use dedicated packet */
        packet = &gioChan->syncPacket;
    }
    else {
        /* asynchronous operation, get packet from freelist */
        packet = QUE_get(&gioChan->freeList);
        if (packet == (IOM_Packet *)(&gioChan->freeList)) {
            return (IOM_ENOPACKETS);
        }
    }

    /* initialize size variable if psize == NULL */
    if (psize == NULL) {
        packet->size = 0;
        psize = &packet->size;
    }

    packet->cmd = cmd;
    packet->addr = bufp;
    packet->size = *psize;
    packet->status = IOM_COMPLETED;
    /* 
     * 'appCallback' will be NULL for synchronous calls. 
     * 'packet->misc' is used in callback function to call callback (async)
     * or post semaphore (sync).
     */
    packet->misc = (Arg)appCallback;

    /* call down into mini-driver */
    status = gioChan->fxns->mdSubmitChan(gioChan->mdChan, packet);

    if (status == IOM_COMPLETED) {
        *psize = packet->size;
        status = packet->status;

        /* If async then place packet back on free list */    
        if (appCallback != NULL) {
            
            QUE_put(&gioChan->freeList, packet);
        }

        return (status);
    }

    /*
     * Call SEMPEND Fxn only if synchronous i/o and no error returned
     *   from mdSubmitChan().
     */
    if (appCallback == NULL) {

        if (status < 0) {    /* error occured */
            *psize = 0;
            return (status);
        }

        /* synchronous I/O -- call global blocking function */
        semStat = GIO->SEMPEND(gioChan->syncObj, gioChan->timeout);

        if (semStat) {
            *psize = packet->size;
            status = packet->status;
        }
        else {    /* timeout occurred */
            *psize = 0;
            
            /* 
             * NOTE: A channel timeout needs special handling. Timeouts are
             * usually due to some serious underlying device or system state
             * and may require the channel, or possibly the device,to be reset.
             * Because the mini-driver may still own the IOM_Packet here
             * driver's will need to perform timeout processing. We will call
             * the mini-driver's control fxn with the IOM_CHAN_TIMEDOUT command
             * code.
             */
             if ((status = gioChan->fxns->mdControlChan(gioChan->mdChan,
                     IOM_CHAN_TIMEDOUT, NULL)) != IOM_COMPLETED) { 
                 
                 return (IOM_ETIMEOUTUNREC); /* Fatal: may have lost IOP */
             }
             
             return (IOM_ETIMEOUT);
        }
    }

    return (status);
}
