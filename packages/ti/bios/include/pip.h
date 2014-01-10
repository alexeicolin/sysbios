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
 *  ======== pip.h ========
 *
 */

#ifndef PIP_
#define PIP_

#include <stddef.h>
#include <ti/bios/include/fxn.h>
#include <ti/bios/include/sts.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct PIP_Obj PIP_Obj;

__FAR__ struct PIP_Obj {
    Uns         threshold;              /* max size of frames in pip */

    PIP_Obj *   preaderTakeProbe;       /* take (get) probe */
    Ptr         readerAddr;             /* set by PIP_get() */
    Uns         readerSize;             /* ditto */
    Ptr         readerCurdesc;          /* pointer to list of pipe descs */
    FXN_Obj *   pnotifyReader;          /* ptr to notify reader function */
    Uns         readerNumFrames;        /* number of frames at reader end */
    PIP_Obj *   preaderGiveProbe;       /* give (free) probe */
    Uns *       pwriterNumFrames;       /* ptr to numframes on writer end */
    FXN_Obj     notifyWriter;           /* notify writer function */
    STS_Obj *   preaderSts;             /* statistics configured for reader */
    PIP_Obj *   pwriterTakeProbe;       /* take (alloc) probe */
    Ptr         writerAddr;             /* set by PIP_alloc() */
    Uns         writerSize;             /* ditto */
    Ptr         writerCurdesc;          /* pointer to list of pipe descs */
    FXN_Obj *   pnotifyWriter;          /* ptr to notify writer function */
    Uns         writerNumFrames;        /* number of numframes at writer end */
    PIP_Obj *   pwriterGiveProbe;       /* give (put) probe */
    Uns *       preaderNumFrames;       /* ptr to numframes at reader end */
    FXN_Obj     notifyReader;           /* notify reader function */
    STS_Obj *   pwriterSts;             /* statistics configured for writer */
};

typedef struct PIP_Obj *PIP_Handle;

extern Void     PIP_alloc(PIP_Handle pipe);
extern Void     PIP_free(PIP_Handle pipe);
extern Void     PIP_get(PIP_Handle pipe);
extern Void     PIP_put(PIP_Handle pipe);
extern Int      PIP_peek(PIP_Handle pipe, Ptr *addr, Uns rw);
extern Void     PIP_reset(PIP_Handle pipe);

#define PIP_READER 0
#define PIP_WRITER 1

#define PIP_getReaderAddr(pipe)         ((pipe)->readerAddr)
#define PIP_getReaderNumFrames(pipe)    ((pipe)->readerNumFrames)
#define PIP_getReaderSize(pipe)         ((pipe)->readerSize)
#define PIP_setReaderSize(pipe,size)    ((pipe)->readerSize = size)
#define PIP_getWriterAddr(pipe)         ((pipe)->writerAddr)
#define PIP_getWriterNumFrames(pipe)    ((pipe)->writerNumFrames)
#define PIP_getWriterSize(pipe)         ((pipe)->writerSize)
#define PIP_setWriterSize(pipe,size)    ((pipe)->writerSize = size)

typedef struct PIP_Curdesc {
    Ptr addr;
    size_t size;
    struct PIP_Curdesc *next;
} PIP_Curdesc;

#define PIP_getThreshold(pipe)          ((pipe)->threshold)
#define PIP_setReaderAddr(pipe,addr)    ((pipe)->readerAddr = (Ptr)addr)
#define PIP_getReaderCurdesc(pipe)      ((pipe)->readerCurdesc)
#define PIP_setReaderCurdesc(pipe,desc) ((pipe)->readerCurdesc = (Ptr)desc)
#define PIP_setReaderNumFrames(pipe,nf) ((pipe)->readerNumFrames = nf)
#define PIP_setWriterAddr(pipe,addr)    ((pipe)->writerAddr = addr)
#define PIP_getWriterCurdesc(pipe)      ((pipe)->writerCurdesc)
#define PIP_setWriterCurdesc(pipe,desc) ((pipe)->writerCurdesc = (Ptr)desc)
#define PIP_setWriterNumFrames(pipe,nf) ((pipe)->writerNumFrames = nf)

#define PIP_getAddrInDesc(desc)         ((desc)->addr)
#define PIP_getSizeInDesc(desc)         ((desc)->size)

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* PIP_ */


