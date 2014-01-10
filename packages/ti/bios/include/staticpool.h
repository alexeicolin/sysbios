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
 *  ======== staticpool.h ========
 *
 */

#ifndef STATICPOOL_
#define STATICPOOL_

#include <stddef.h>
#include <ti/bios/include/pool.h>
#include <ti/bios/include/fxn.h>
#include <ti/bios/include/que.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Both the init and exit functions currently do nothing */
#define STATICPOOL_init    FXN_F_nop
#define STATICPOOL_exit    FXN_F_nop

/*
 *  Allocator's parameters.
 *     addr:       Start of the block of memory that contains the buffers.
 *     length:     Length of the block of memory that contains the buffers.
 *     bufferSize: Size of the individual buffers.
 */
typedef struct STATICPOOL_Params {
    Ptr         addr;
    size_t      length;
    size_t      bufferSize;
} STATICPOOL_Params;

typedef struct STATICPOOL_Obj {
    QUE_Obj   queue;        /* Queue to hold the messages */
    size_t    bufferSize;   /* Size of each buffer.      */
} STATICPOOL_Obj, *STATICPOOL_Handle;

/* This allocator's interface functions */
extern __FAR__ const POOL_Fxns STATICPOOL_FXNS;

extern Int  _STATICPOOL_open(Ptr *object, Ptr params);
extern Int  _STATICPOOL_alloc(Ptr object, Ptr *buf, size_t size);
extern Void _STATICPOOL_free(Ptr object, Ptr buf, size_t size);


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* STATICPOOL_ */
