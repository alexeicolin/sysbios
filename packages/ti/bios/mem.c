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
 *  ======== mem.c ========
 */

#include <xdc/std.h>

#include <string.h> /* for memcpy() */

#include <xdc/runtime/Error.h>
#include <xdc/runtime/IHeap.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Memory.h>

#include <ti/sysbios/heaps/HeapMem.h>

#include <ti/bios/include/mem.h>
#include <ti/bios/include/sys.h>

extern __FAR__ IHeap_Handle *MEM_table;
extern __FAR__ Uns           MEM_tabSize;
extern __FAR__ MEM_Attrs     MEM_ATTRS;

static __FAR__ Bool staticTable = TRUE;

/*
 *  ======== MEM_alloc ========
 */
Ptr MEM_alloc(Int id, size_t size, size_t align)
{
    Error_Block eb;
    
    Error_init(&eb);
    
    if (id < 0) {
        SYS_error( "MEM", SYS_EALLOC, id, size, align );
        return (NULL);
    }

    /*
     *  Allow non-legacy apps to use modules that use MEM (e.g. drivers).
     *  Use the default heap in xdc.runtime.Memory.
     */
    if (MEM_tabSize == 0) {
        return (xdc_runtime_Memory_alloc(NULL, size, align, &eb));
    }
   
    if ((Uns)id >= MEM_tabSize) {
        SYS_error( "MEM", SYS_EALLOC, id, size, align );
        return (NULL);
    }
   
    if (MEM_table[id] == NULL) {
        SYS_error( "MEM", SYS_EALLOC, id, size, align );
        return(FALSE);
    }

    return (xdc_runtime_Memory_alloc(MEM_table[id], size, align, &eb));
}

/*
 *  ======== MEM_valloc ========
 */
Ptr MEM_valloc(Int id, size_t size, size_t align, Char value)
{
    Char *p;
    
    if ((p = MEM_alloc(id, size, align)) == NULL) {
        return (NULL);
    }

    memset(p, value, size);

    return (p);
}

/*
 *  ======== MEM_calloc ========
 */
Ptr MEM_calloc(Int id, size_t size, size_t align)
{
    return (MEM_valloc(id, size, align, 0));
}

/*
 *  ======== MEM_define ========
 */
extern Int MEM_define(Ptr base, MEM_sizep length, MEM_Attrs *attrs)
{
    Int                 id;
    IArg                key;
    HeapMem_Params      params;
    HeapMem_Handle      heap;
    IHeap_Handle       *tmpTable;
    Error_Block         eb;

    HeapMem_Params_init(&params);
    params.size = length;
    params.buf = base;
    Error_init(&eb);
    heap = HeapMem_create(&params, &eb);
    if (heap == NULL) {
        return(-1);
    }

    /* Use HeapMem's gate for thread-safety */
    key = HeapMem_enter();

    /* look for empty slot in memory table */
    for (id = 0; id < MEM_tabSize; id++) {
        if (MEM_table[id] == NULL) {
            break;
        }
    }

    /* increase table size if no empty slot found */
    if (id == MEM_tabSize) {
        tmpTable = Memory_calloc(MEM_table[0],
                 (MEM_tabSize + 1) * sizeof(IHeap_Handle), 0, &eb);
        if (tmpTable == NULL) {
            HeapMem_leave(key);
            return(-1);
        }

        memcpy(tmpTable, MEM_table, MEM_tabSize * sizeof(IHeap_Handle));

        if ((MEM_tabSize != 0) && (staticTable != TRUE)) {
            Memory_free(MEM_table[0], MEM_table, MEM_tabSize * sizeof(IHeap_Handle));
        }

        staticTable = FALSE;
        MEM_table = tmpTable;
        MEM_tabSize++;
    }

    MEM_table[id] = HeapMem_Handle_to_xdc_runtime_IHeap(heap);

    HeapMem_leave(key);

    return (id);
}

/*
 *  ======== MEM_free ========
 */
Bool MEM_free(Int id, Ptr addr, size_t size)
{
    if (id < 0) {
        SYS_error("MEM", SYS_EFREE, id, addr, size);
        return (FALSE);
    }

    /*
     *  Allow non-legacy apps to use modules that use MEM (e.g. drivers).
     *  Use the default heap in xdc.runtime.Memory.
     */
    if (MEM_tabSize == 0) {
        xdc_runtime_Memory_free(NULL, addr, size);
        return (TRUE);
    }
    
    if (((Uns)id < MEM_tabSize) && (MEM_table[id] != NULL)) {
        xdc_runtime_Memory_free(MEM_table[id], addr, size);
        return (TRUE);
    }
    else {
        SYS_error("MEM", SYS_EFREE, id, addr, size);
        return (FALSE);
    }
}

/*
 *  ======== MEM_increaseTableSize ========
 */
Int MEM_increaseTableSize(Uns numEntries)
{
    IArg                key;
    IHeap_Handle       *tmpTable;
    Error_Block         eb;
    
    Error_init(&eb);

    /* Use HeapMem's gate for thread-safety */
    key = HeapMem_enter();

    tmpTable = Memory_calloc(MEM_table[0],
        (MEM_tabSize + numEntries) * sizeof(IHeap_Handle), 0, NULL);
    
    if (tmpTable == NULL) {
        HeapMem_leave(key);
        return (SYS_EALLOC);
    }

    memcpy(tmpTable, MEM_table, MEM_tabSize * sizeof(IHeap_Handle));

    if ((MEM_tabSize != 0) && (staticTable != TRUE)) {
        Memory_free(MEM_table[0], MEM_table, MEM_tabSize * sizeof(IHeap_Handle));
    }

    staticTable = FALSE;
    MEM_table = tmpTable;
    MEM_tabSize += numEntries;

    HeapMem_leave(key);

    return (SYS_OK);
}

/*
 *  ======== MEM_redefine ========
 */
Void MEM_redefine(Int id, Ptr base, MEM_sizep length)
{
    IArg           key;
    HeapMem_Params params;
    HeapMem_Handle heap;

    /* Make sure the segid is valid. */    
    if ((id < 0) || (id == 0) || ((Uns)id >= MEM_tabSize)) {
        SYS_error("MEM", SYS_EINVAL);
        return;
    }
    
    key = HeapMem_enter();
    
    if (MEM_table[id] == NULL) {
        HeapMem_leave(key);
        SYS_error("MEM", SYS_EINVAL);        
        return;
    }
    
    heap = HeapMem_Handle_from_xdc_runtime_IHeap(MEM_table[id]);

    HeapMem_destruct(HeapMem_struct(heap));

    HeapMem_Params_init(&params);
    params.size = length;
    params.buf = base;

    HeapMem_construct(HeapMem_struct(heap), &params);

    HeapMem_leave(key);
}

/*
 *  ======== MEM_stat ========
 */
Bool MEM_stat(Int id, MEM_Stat *statbuf)
{
    IArg            key;
    HeapMem_Handle  heap;
    Memory_Stats    stat;
    HeapMem_ExtendedStats extendedStats;
    
    if ((id < 0) || ((Uns)id >= MEM_tabSize) || (MEM_table[id] == NULL)) {
        SYS_error("MEM", SYS_EINVAL);        
        return (FALSE);
    }

    heap = HeapMem_Handle_from_xdc_runtime_IHeap(MEM_table[id]);

    /* Use HeapMem's gate for thread-safety */
    key = HeapMem_enter();

    HeapMem_getExtendedStats(heap, &extendedStats);
    Memory_getStats(MEM_table[id], &stat);
    statbuf->size = stat.totalSize;
    statbuf->used = stat.totalSize - stat.totalFreeSize;
    statbuf->length = stat.largestFreeSize;
    statbuf->space = 1;
    statbuf->base = extendedStats.buf;

    HeapMem_leave(key);

    return (TRUE);
}

/*
 *  ======== MEM_undefine ========
 */
Void MEM_undefine(Int id)
{
    HeapMem_Handle      heap;
    
    /* Make sure the id is valid */
    if ( (id < 0) || ((Uns)id >= MEM_tabSize)  ||
        (MEM_table[id] == NULL)) {
        SYS_error("MEM", SYS_EINVAL);
        return;
    }

    heap = HeapMem_Handle_from_xdc_runtime_IHeap(MEM_table[id]);
    HeapMem_delete(&heap);

    MEM_table[id] = NULL;
}

/*
 *  ======== MEM_getSegId ========
 */
Int MEM_getSegId(IHeap_Handle handle)
{
    Int i;

    for (i=0 ; i < MEM_tabSize; i++) {
        if (MEM_table[i] == handle) {
            return (i);
        }
    }

    return (-1);
}

/*
 *  ======== MEM_getHandle ========
 */
IHeap_Handle MEM_getHandle(Int id)
{
    if ((id < 0) || ((Uns)id >= MEM_tabSize)  || 
        (MEM_table[id] == NULL)) {
        SYS_error("MEM", SYS_EINVAL);
        return (NULL);
    }
    return (MEM_table[id]);
}

/*
 *  ======== MEM_getBaseAddress ========
 */
Ptr MEM_getBaseAddress(Int id)
{
    HeapMem_ExtendedStats stats;
    
    if ((id < 0) || ((Uns)id >= MEM_tabSize)  || 
        (MEM_table[id] == NULL)) {
        SYS_error("MEM", SYS_EINVAL);
        return (NULL);
    }

    HeapMem_getExtendedStats(
        HeapMem_Handle_from_xdc_runtime_IHeap(MEM_table[id]), &stats);

    return (stats.buf);
}

/*
 *  ======== MEM_getNumSegments ========
 */
Int MEM_getNumSegments(Void)
{
    return (MEM_tabSize);
}


