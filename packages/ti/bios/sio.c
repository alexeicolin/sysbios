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
 *  ======== sio.c ========
 *  Stream I/O Manager
 */

#include <ti/bios/include/std.h>
#include <stddef.h>

#include <ti/bios/include/dev.h>
#include <ti/bios/include/_dev.h>
#include <ti/bios/include/obj.h>
#include <ti/bios/include/que.h>
#include <ti/bios/include/sio.h>
#include <ti/bios/include/sys.h>
#include <ti/bios/include/mem.h>

#define _BOTH_

__FAR__ SIO_Attrs SIO_ATTRS = {
    2,                  /* nbufs */
    0,                  /* segid */
    0,                  /* align */
    FALSE,              /* flush */
    SIO_STANDARD,       /* model */
    SYS_FOREVER,        /* timeout */
    NULL                /* callback */
};

/*
 *  ======== SIO_create ========
 */
SIO_Handle SIO_create(name, mode, bufsize, attrs)
    String      name;
    Int         mode;           
    size_t      bufsize;                
    SIO_Attrs   *attrs;
{
    Int         i;
    Int         errno = SYS_EALLOC;     /* for use after "goto delete" */
    DEV_Device  *entry;
    DEV_Handle  device;                 /* == stream */
    DEV_Frame   *frame;
    SIO_Handle  stream;
    QUE_Handle  queue;

    if (attrs == NULL) {
        attrs = &SIO_ATTRS;
    }

#if !defined(_BOTH_) 
    if (attrs->model == SIO_STANDARD) {
        return (NULL);
    }
#endif

    name = DEV_match(name, &entry);
    if (entry == NULL) {
        /*
         * We print the passed name instead of the module name ("SIO")
         * for a more informative error message.
         *
         * If DEV_match() matches, name no longer points to the original
         * device name, but if entry is NULL, name is unchanged.
         */
        SYS_error(name, SYS_ENODEV);
        return (NULL);
    }

    if (entry->type != DEV_SIOTYPE) {
        SYS_error("SIO", SYS_EINVAL); /* sys error - invalid device parameter */
        return (NULL);
    }

    stream = (SIO_Handle)MEM_calloc(0, sizeof(SIO_Obj), 0);

    if (stream == NULL) {
        return (NULL);
    }

    stream->flush = attrs->flush;       /* save flush flag for closing */
    stream->model = attrs->model;
    stream->pfxns = NULL;               /* NULL for all dynamic SIO */

    /*
     * Do the QUE_new() now since SIO_delete() will check if the
     * framelist is empty, so the queue must be initialized.
     */
    QUE_new(&stream->framelist);

    /* first field in SIO_Obj is a DEV_Obj */
    device = (DEV_Handle)stream;

    /* fill in device object fields */
    device->mode = mode;                /* DEV_INPUT or DEV_OUTPUT */
    device->devid = entry->devid;       /* meaning depends on device */
    device->bufsize = bufsize;          /* size to be allocated per buffer */
    device->align = attrs->align;       /* buffer alignment */
    device->nbufs = attrs->nbufs;       /* max # of buffers allowed */
    device->segid = attrs->segid;       /* allocation memory segment */
    device->params = entry->params;     /* device parameters handle */
    device->timeout = attrs->timeout;   /* timeout used by DEV_reclaim() */
    device->callback = attrs->callback; /* callback parameters */

    /* assign pointer to driver function table */
    device->fxns = *(DEV_Fxns *)(entry->fxns);  

    /* 
     * create todevice and fromdevice queues;
     *  MEM_alloc() will call SYS_error() if allocation fails
     */
    if ((queue = QUE_create(NULL)) == NULL) {
        goto delete;
    };
    device->todevice = queue;
    
    if ((queue = QUE_create(NULL)) == NULL) {
        goto delete;
    };
    device->fromdevice = queue;

    /*
     * Do not allocate buffers in SIO_ISSUERECLAIM model since the application
     * will provide the buffers.
     */
    if (attrs->model == SIO_ISSUERECLAIM) {
        bufsize = 0;
    }

    /* allocate the frame objects, put on framelist queue */
    for (i = device->nbufs; i > 0; i--) {
        frame = DEV_mkframe(attrs->segid, bufsize, attrs->align);
        if (frame == NULL) {
            goto delete;
        }

        QUE_put(&stream->framelist, frame);
    }

#if defined(_BOTH_)
    if (attrs->model == SIO_STANDARD) {
        /* 
         * allocate an extra frame with no buffer.  This will be used
         * by SIO_get() and SIO_put().
         */
        stream->standardFrame = DEV_mkframe(0, 0, 0);
        if (stream->standardFrame == NULL) {
            goto delete;
        }

        /*
         * SIO_STANDARD devices have n+1 buffers.  Make sure that
         * the device driver knows this.  This is especially important
         * for stacking drivers that allocate their own frames since
         * they need to know the maximum number of frames.
         */
        device->nbufs++;
    }
#endif

    /* Call the device-specific open function using DEV_open macro */
    if ((errno = DEV_open(device, name)) != SYS_OK) {

        /* open function returned an error */
delete:
        /*
         * Since SIO_delete() calls idle and close before freeing
         * the device frames and buffers, we bind SYS_zero() to 
         * make these effectively nops.  This allows us to save space
         * by not having a special _SIO_rmstream() function.
         */
        device->fxns.idle = DEV_IDLE;
        device->fxns.close = DEV_CLOSE;

        /*
         * WARNING!  This is used by SIO_delete() so that to make
         * sure that it doesn't traverse the 'todevice' queue.  There's
         * a strong dependancy between SIO_create() and SIO_delete().
         */
        device->nbufs = 0;

        SIO_delete(stream);

        /*
         * errno could be SYS_EALLOC here, so we need to cover the most
         * restrictive case for the SYS_errors[] array.  Although 0s are
         * the wrong parameters for a SYS_EALLOC message, they're better
         * than random garbage, and it's OK to pass too many for others.
         */
        SYS_error("SIO", errno, 0, 0, 0 );

        return (NULL);
    }

    /* device-specific open function was successful */
    return (stream);
}

/*
 *  ======== SIO_delete ========
 */
Int SIO_delete(SIO_Handle stream)
{
    DEV_Handle  device = (DEV_Handle)stream;
    QUE_Handle  queue;
    DEV_Frame   *frame;
    Int         status;
#if defined(_BOTH_)
    Int         nbufs;
#endif

    status = DEV_idle(device, stream->flush);   /* idle device(s) */

    if (status != SYS_OK) {
        return (status);        /* idle failed */
    }

#if defined(_BOTH_)
    /*
     * Pull all frames back from the device level and free them.
     * In SIO_ISSUERECLAIM mode, the client must call SIO_reclaim()
     * to get back all frames before calling SIO_delete().
     */
    if (stream->model == SIO_STANDARD) {
        nbufs = device->nbufs - 1;

        /*
         * Handle the boundary case, where SIO_delete() is called
         * before nbufs I/O operations have occurred.
         */
        queue = &stream->framelist;
        while ((frame = QUE_get(queue)) != (DEV_Frame *)queue) {
            DEV_rmframe(frame, device->segid, device->bufsize);
            nbufs--;
        }

        /*
         * Get any outstanding frames back and free them.
         */
        for (; nbufs > 0; nbufs--) {
            status = DEV_reclaim(device);

            if (status != SYS_OK) {
                return (status);
            }

            frame = QUE_get(device->fromdevice);
            DEV_rmframe(frame, device->segid, device->bufsize);
        }

        /*
         * Free the standard frame object, but not the buffer.  The
         * application has the buffer.
         */
        if (stream->standardFrame) {
            DEV_rmframe(stream->standardFrame, 0, 0);
        }
    }
    else {
        queue = &stream->framelist;
        while ((frame = QUE_get(queue)) != (DEV_Frame *)queue) {
            DEV_rmframe(frame, device->segid, 0);
        }
    }
#else
    
    queue = &stream->framelist;
    while ((frame = QUE_get(queue)) != (DEV_Frame *)queue) {
        DEV_rmframe(frame, device->segid, 0);
    }
#endif
        
    status = DEV_close(device);         /* close device(s) */
    if (status != SYS_OK) {
        return (status);
    }

    if (device->todevice) {
        QUE_delete(device->todevice);
    }
    if (device->fromdevice) {
        QUE_delete(device->fromdevice);
    }

    /* free stream object */
    MEM_free(0, stream, sizeof(SIO_Obj));

    /* return status from idle and close (above) */
    return (status);
}

/*
 *  ======== SIO_get ========
 *
 *  Exchange an empty buffer with a non-empty buffer from "stream".
 *  Before: "ppbuf" points to empty buffer;
 *  After: "ppbuf" points to non-empty buffer.
 */
Int SIO_get(SIO_Handle stream, Ptr *ppbuf)
{
    DEV_Handle  dev;
    DEV_Frame   *frame;
    Int         status;

    dev = (DEV_Handle)stream;

    /*
     * This will only execute the first time SIO_get() is called.
     * Copy all frames from framelist to the device's todevice queue.
     * The device is issued all empty frames the first time SIO_get()
     * is called.
     */
    while ( (frame = (DEV_Frame *)QUE_get(&stream->framelist)) !=
                (DEV_Frame *)&stream->framelist ) {
        QUE_put(dev->todevice, frame);
        status = DEV_issue(dev);
        if (status != SYS_OK) {
            return (status * -1);
        }
    }

    frame = stream->standardFrame;

    /* load the frame with the buffer pointed to by ppbuf */
    frame->addr = *ppbuf;
    frame->size = dev->bufsize;

    /* put the frame on the todevice queue */
    QUE_put(dev->todevice, frame);

    /* send the frame to the device */
    status = DEV_issue(dev);
    if (status != SYS_OK) {
        return (status * -1);
    }

    /* wait for an input frame to be available */
    status = DEV_reclaim(dev);
    if (status != SYS_OK) {
        return (status * -1);
    }

    frame = (DEV_Frame *)QUE_get(dev->fromdevice);

    stream->standardFrame = frame;

    *ppbuf = frame->addr;

    return (frame->size);
}

/*
 *  ======== _SIO_idle ========
 *  _SIO_idle is called by SIO_idle() and SIO_flush() macros
 */
Int _SIO_idle(SIO_Handle stream, Bool flush)
{
    DEV_Handle  device = (DEV_Handle)stream;
    Int         status;
#if defined(_BOTH_)
    QUE_Handle  queue;
    DEV_Frame   *frame;
    Int         nbufs;
#endif

    status = DEV_idle(device, flush);   /* idle device(s) */

    if (status != SYS_OK) {
        return (status);
    }

#if defined(_BOTH_)
    /*
     * For standard model, pull all frames back from the device level and
     * put them on the stream's framelist.
     */
    if (stream->model == SIO_STANDARD) {
        nbufs = device->nbufs - 1;

        /*
         * Handle the boundary case, where SIO_idle() is called
         * before nbufs I/O operations have occurred.  Count the
         * number of frames on the framelist.
         */
        queue = &stream->framelist;
        frame = QUE_head(queue);
        while (frame != (DEV_Frame *)queue) {
            frame = QUE_next(frame);
            nbufs--;
        }

        /*
         * Get any outstanding frames back from the device and
         * put them onto the stream's framelist.
         */
        for (; nbufs > 0; nbufs--) {
            if ((status = DEV_reclaim(device)) != SYS_OK) {
                return (status);
            }
            frame = QUE_get(device->fromdevice);
            QUE_put(&stream->framelist, frame);
        }
    }
#endif

    return (SYS_OK);
}

/*
 *  ======== SIO_issue ========
 *
 *  Send buffer "pbuf" of size "nbytes" to "stream" with arguments "arg".
 *  Used for both input and output.  Returns to caller without blocking.
 *
 *  "stream" must have been created using attrs.mode = SIO_ISSUERECLAIM.
 */
Int SIO_issue(SIO_Handle stream, Ptr pbuf, size_t nbytes, Arg arg)
{
    DEV_Handle  dev;
    DEV_Frame   *frame;
    Int         status;

    dev = (DEV_Handle)stream;

    /* get a valid frame from the stream's framelist queue or return error */
    frame = (DEV_Frame *)QUE_get(&stream->framelist);
    if (frame == (DEV_Frame *)&stream->framelist) {
        return SYS_EBADIO;
    }

    /* load the frame with the buffer pointed to by pbuf */
    frame->addr = pbuf;
    frame->size = nbytes;
    frame->arg = arg;

    /* put the frame on the todevice queue */
    QUE_put(dev->todevice, frame);

    /* call the device's issue function to send the buffer to the stream */
    status = DEV_issue(dev);

    return (status);
}

/*
 *  ======== SIO_put ========
 * 
 *  Exchange a non-empty buffer "buf" of size "nbytes" with an empty buffer 
 *  from "stream".
 *  Before: "ppbuf" points to non-empty buffer;
 *  After: "ppbuf" points to empty buffer.
 *
 */
Int SIO_put(SIO_Handle stream, Ptr *ppbuf, size_t nbytes)
{
    DEV_Handle  dev;
    DEV_Frame   *frame;
    Int         status;

    dev = (DEV_Handle)stream;

    frame = stream->standardFrame;
    frame->size = nbytes;
    frame->addr = *ppbuf;

    QUE_put(dev->todevice, frame);

    DEV_issue(dev);

    if ( (frame = (DEV_Frame *)QUE_get(&stream->framelist)) !=
                (DEV_Frame *)&stream->framelist ) {

        /* free frame is available -- no need to call DEV_reclaim() */
        stream->standardFrame = frame;
    }
    else {

        /* reclaim frame from device */
        status = DEV_reclaim(dev);
        if (status != SYS_OK) {
            return (status * -1);
        }

        frame = (DEV_Frame *)QUE_get(dev->fromdevice);
        stream->standardFrame = frame;
    }

    *ppbuf = frame->addr;

    return (frame->size);
}

/*
 *  ======== SIO_reclaimx ========
 *
 *  Extended version of SIO_reclaim().  Identical to SIO_reclaim() but adds
 *  new parameter that will return the frame status field.  This allows the 
 *  IOM driver to pass frame-specific status information up to the application.
 *
 *  Applications still check return value of SIO_reclaimx() (< 0 for error),
 *  and >= 0 for success (frame size).
 *
 *  See SDSsq41555 for more.
 */
Int SIO_reclaimx(SIO_Handle stream, Ptr *ppbuf, Arg *parg, Int *pfstatus)
{
    DEV_Handle  dev;
    DEV_Frame   *frame;
    size_t      size;
    Int         status;

    dev = (DEV_Handle)stream;

    status = DEV_reclaim(dev);

    if (status == SYS_OK) {
        frame = (DEV_Frame *)QUE_get(dev->fromdevice);

        *ppbuf = frame->addr;

        if (parg != NULL) {
            *parg = frame->arg;
        }

        if (pfstatus != NULL) {
            *pfstatus = frame->status;
        }

        size = frame->size;

        /*
         * Return frame to free list (size saved above for return).
         */
        frame->size = 0;
        QUE_put(&stream->framelist, (QUE_Elem *)frame);

        return (size);
    }
    else {
        return (status * -1);
    }
}

/*
 * ======== SIO_reclaim ========
 */
Int SIO_reclaim(SIO_Handle stream, Ptr *ppbuf, Arg *parg)
{
    DEV_Handle  dev;
    DEV_Frame   *frame;
    size_t      size;
    Int         status;

    dev = (DEV_Handle)stream;

    status = DEV_reclaim(dev);

    if (status == SYS_OK) {
        frame = (DEV_Frame *)QUE_get(dev->fromdevice);

        *ppbuf = frame->addr;
        if (parg != NULL) {
            *parg = frame->arg;
        }
        size = frame->size;

        /*
         * Return frame to free list (size saved above for return).
         */
        frame->size = 0;
        QUE_put(&stream->framelist, (QUE_Elem *)frame);

        return (size);
    }
    else {
        return (status * -1);
    }
}

/*
 *  ======== SIO_select ========
 */
Uns SIO_select(SIO_Handle streamtab[], Int n, Uns timeout)
{
    Int         i;
    Uns         mask = 1;
    Uns         ready = 0;
    SEM_Obj     sem;            /* local variable on stack! */
    SIO_Handle  *stream;
#if defined(_BOTH_)
    DEV_Frame   *frame;
#endif

    SEM_new(&sem, 0);           /* initial semaphore */

    stream = streamtab;

    for (i = n; i > 0; i--, stream++) {

#if defined(_BOTH_)
        if ( (*stream)->model == SIO_STANDARD ) {
            if ((*stream)->dobj.mode == DEV_INPUT) {
                /* 
                 * Make sure input device has been primed and started.  This
                 * is typically done by SIO_get() but it is possible for
                 * SIO_select() to be called before SIO_get().  This code is
                 * identical to the code in SIO_get() -- it may make sense to
                 * make this loop into a separate function (_SIO_primeget()?)
                 * to save code space.
                 */
                while ( (frame = (DEV_Frame *)QUE_get(&(*stream)->framelist)) !=
                        (DEV_Frame *)&(*stream)->framelist ) {
                    QUE_put((*stream)->dobj.todevice, frame);
                    DEV_issue((DEV_Handle)*stream);
        
                    /* what can we do if DEV_issue fails? */
                }
            }
            else {
                /*
                 * Initial calls to SIO_put() pull frames from the stream's
                 * framelist instead of calling DEV_reclaim().  Frames are
                 * never put back onto framelist in SIO_STANDARD model.
                 */
                if (!QUE_empty(&(*stream)->framelist)) {
                    ready = 1;
                }
            }
        }
#endif

        /* see if device is ready and register call back */
        if (DEV_ready((DEV_Handle)*stream, &sem)) {
            ready = 1;
        }
    }

    if (!ready) {
        SEM_pend(&sem, timeout);         /* wait until device is ready */
    }

    ready = 0;

    stream = streamtab;

    for (i = n; i > 0; i--, stream++) {
        /* see if device is ready and un-register call back */
        if (DEV_ready((DEV_Handle)*stream, NULL)) {
            ready |= mask;
        }

#if defined(_BOTH_)
        else {
            /*
             * This will only be true for DEV_OUTPUT mode.
             * We don't need to check if mode == DEV_OUTPUT here since
             * framelist will have been emptied for DEV_INPUT mode (above).
             */
            if ( ((*stream)->model == SIO_STANDARD) &&
                        (!QUE_empty(&(*stream)->framelist)) ) {
                ready |= mask;
            }
        }
#endif

        mask = mask << 1;
    }

    return (ready);
}

/*
 *  ======== SIO_staticbuf ========
 *  Return static buffer created for gconf streams.
 *
 *  The SIO_Obj macro puts all static frames on 'stream->framelist'.
 *
 *  Returns 0 if no static buffer exists.
 *  SIO_staticbuf can only be called once for each static buffer.
 */
Int SIO_staticbuf(SIO_Handle stream, Ptr *ppbuf)
{
    DEV_Frame   *frame;
    Int         size;

#if defined(_BOTH_)
    if (stream->model == SIO_STANDARD) {
        frame = stream->standardFrame;

        size = frame->size;
        frame->size = 0;

        *ppbuf = frame->addr;
    }
    else {
        frame = QUE_get(&stream->framelist);

        size = frame->size;
        frame->size = 0;

        *ppbuf = frame->addr;

        QUE_put(&stream->framelist, frame);
    }
#else

    frame = QUE_get(&stream->framelist);

    size = frame->size;
    frame->size = 0;

    *ppbuf = frame->addr;

    QUE_put(&stream->framelist, frame);

#endif

    return (size);
}
