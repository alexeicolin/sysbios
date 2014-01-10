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
 *  ======== buf.h ========
 *
 */

#ifndef BUF_
#define BUF_

#define xdc_runtime_Error__nolocalnames
#define ti_sysbios_heaps_HeapBuf__nolocalnames

#include <ti/bios/include/std.h>

#include <xdc/runtime/Error.h>

#include <ti/sysbios/heaps/HeapBuf.h>
#include <ti/bios/include/mem.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef ti_sysbios_heaps_HeapBuf_Struct    BUF_Obj, *BUF_Handle;

typedef struct BUF_Stat {
    MEM_sizep postalignsize; /* Size of buffer after alignment*/
    MEM_sizep size;          /* Original size of buffer       */
    Uns totalbuffers;        /* Total buffers in pool         */    
    Uns freebuffers;         /* Number of free buffers in pool*/ 
} BUF_Stat;

typedef struct BUF_Attrs {
    Int         segid;          /* segment for element allocation*/
} BUF_Attrs;

extern __FAR__ BUF_Attrs BUF_ATTRS;  /* Default attributes */

extern BUF_Handle BUF_create(Uns numbuff, MEM_sizep size, Uns align,
    BUF_Attrs *attrs);

extern Uns BUF_maxbuff(BUF_Handle buf);

extern Void BUF_stat(BUF_Handle buf, BUF_Stat *statbuf);

static inline Uns BUF_delete(BUF_Handle buf)
{
    ti_sysbios_heaps_HeapBuf_Handle handle = (ti_sysbios_heaps_HeapBuf_Handle)buf;

    ti_sysbios_heaps_HeapBuf_delete(&handle);

    return ((Uns) 1);   /* always return success since no return status above */
}

static inline Ptr BUF_alloc(BUF_Handle buf)
{
    ti_sysbios_heaps_HeapBuf_Handle handle = (ti_sysbios_heaps_HeapBuf_Handle)buf;
    xdc_runtime_Error_Block eb;

    xdc_runtime_Error_init(&eb);

    return (ti_sysbios_heaps_HeapBuf_alloc(handle, ti_sysbios_heaps_HeapBuf_getBlockSize(handle), 0, &eb));
}

static inline Bool BUF_free(BUF_Handle buf, Ptr bufaddr)
{
    ti_sysbios_heaps_HeapBuf_Handle handle = (ti_sysbios_heaps_HeapBuf_Handle)buf;

    ti_sysbios_heaps_HeapBuf_free(handle, bufaddr, ti_sysbios_heaps_HeapBuf_getBlockSize(handle));

    return (TRUE);
}

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* BUF_ */
