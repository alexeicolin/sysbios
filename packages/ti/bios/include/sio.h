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
 *  ======== sio.h ========
 *  Stream I/O Manager
 *
 */

#ifndef SIO_
#define SIO_

#include <stddef.h>
#include <ti/bios/include/dev.h>
#include <ti/bios/include/que.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SIO_INPUT       DEV_INPUT               /* input stream */
#define SIO_OUTPUT      DEV_OUTPUT              /* output stream */

#define SIO_STANDARD            0
#define SIO_ISSUERECLAIM        1

typedef DEV_Callback SIO_Callback;      /* for callback mode */

typedef struct SIO_Attrs {      /* stream attributes */
    Int         nbufs;          /* number of buffers */
    Int         segid;          /* buffer segment */
    size_t      align;          /* buffer alignment */
    Bool        flush;          /* TRUE -> don't block in DEV_idle() */
    Uns         model;          /* SIO_STANDARD, SIO_ISSUERECLAIM */
    Uns         timeout;        /* passed to all calls to DEV_reclaim() */
    SIO_Callback *callback;     /* initializes callback in DEV_Obj */
} SIO_Attrs;

typedef struct SIO_Obj {        /* stream object */
    DEV_Obj     dobj;           /* device object */
    Bool        flush;          /* flush */
    QUE_Obj     framelist;      /* start of frame list */
    DEV_Frame   *standardFrame; /* frame for use in SIO_STANDARD model only */
    Uns         model;          /* SIO_STANDARD or SIO_ISSUERECLAIM */
    DEV_Fxns    *pfxns;         /* pointer to DEV_Fxns for static stream init */
} SIO_Obj, *SIO_Handle;

extern __FAR__ SIO_Attrs SIO_ATTRS;     /* default attributes */

extern Int _SIO_idle(SIO_Handle stream, Bool flush);
#define SIO_idle(stream)                _SIO_idle(stream, 0)
#define SIO_flush(stream)               _SIO_idle(stream, 1)

#define SIO_ctrl(stream, cmd, arg)      DEV_ctrl((DEV_Handle)stream, cmd, arg)
#define SIO_ready(stream)               DEV_ready((DEV_Handle)stream, NULL)

#define SIO_segid(stream)               ((stream)->dobj.segid)
#define SIO_bufsize(stream)             ((stream)->dobj.bufsize)
#define SIO_nbufs(stream)               ((stream)->dobj.nbufs)

extern SIO_Handle SIO_create(String name, Int mode, size_t size, SIO_Attrs *attrs);
extern Int SIO_delete(SIO_Handle stream);
extern Int SIO_get(SIO_Handle stream, Ptr *buf);
extern Int SIO_staticbuf(SIO_Handle stream, Ptr *buf);
extern Int SIO_put(SIO_Handle stream, Ptr *buf, size_t nbytes);
extern Uns SIO_select(SIO_Handle streamtab[], Int n, Uns timeout);
extern Int SIO_issue(SIO_Handle stream, Ptr pbuf, size_t nbytes, Arg arg);
extern Int SIO_reclaim(SIO_Handle stream, Ptr *ppbuf, Arg *parg);
extern Int SIO_reclaimx(SIO_Handle stream, Ptr *ppbuf, Arg *parg, Int *pfstatus);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* SIO_ */
