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
 *  ======== mem.h ========
 *
 *  Open Issues
 *  - malloc/free
 *
 */

#ifndef MEM_
#define MEM_

#ifdef __cplusplus
extern "C" {
#endif

#define xdc_runtime_Memory__nolocalnames
#define xdc_runtime_IHeap__nolocalnames
#define ti_sysbios_heaps_HeapMem__nolocalnames

#include <xdc/runtime/Memory.h>
#include <xdc/runtime/IHeap.h>

#include <ti/sysbios/heaps/HeapMem.h>

#include <stddef.h>

#define MEM_ILLEGAL ((Ptr)NULL)         /* illegal memory address */

typedef xdc_runtime_Memory_Size         MEM_sizep;

#define MEM_headersize(space)   sizeof(ti_sysbios_heaps_HeapMem_Header)
#define MEM_HEADERSIZE          sizeof(ti_sysbios_heaps_HeapMem_Header)
#define MEM_headermask(space)   (sizeof(ti_sysbios_heaps_HeapMem_Header) - 1)
#define MEM_HEADERMASK          (sizeof(ti_sysbios_heaps_HeapMem_Header) - 1)

typedef struct MEM_Attrs {
    Int         space;          /* Not used, but here for compatibilty */
} MEM_Attrs;

typedef struct MEM_Stat {
    MEM_sizep   size;   /* orig size of heap */
    MEM_sizep   used;   /* madus used in heap */
    size_t      length; /* largest contiguous block */
    Int         space;  /* Indicate x, y or p memory */
    Ptr         base;   /* internal buffer */
} MEM_Stat;

extern __FAR__ MEM_Attrs                        MEM_ATTRS;

extern Void MEM_init();

extern Ptr MEM_alloc(Int id, size_t size, size_t align);
extern Ptr MEM_calloc(Int id, size_t size, size_t align);
extern Int MEM_define(Ptr base, MEM_sizep length, MEM_Attrs *attrs);
extern Bool MEM_free(Int id, Ptr addr, size_t size);
extern Int MEM_increaseTableSize(Uns numEntries);
extern Void MEM_redefine(Int id, Ptr base, MEM_sizep length);
extern Bool MEM_stat(Int id, MEM_Stat *statbuf);
extern Void MEM_undefine(Int id);
extern Ptr MEM_valloc(Int id, size_t size, size_t align, Char value);
extern Int MEM_getSegId(xdc_runtime_IHeap_Handle handle);
extern xdc_runtime_IHeap_Handle MEM_getHandle(Int id);
extern Ptr MEM_getBaseAddress(Int segid);
extern Int MEM_getNumSegments(Void);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* MEM_ */
