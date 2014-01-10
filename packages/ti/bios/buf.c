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
 *  ======== buf.c ========
 */

#include <xdc/std.h>

#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>

#include <ti/sysbios/heaps/HeapBuf.h>
#include <ti/bios/include/mem.h>
#include <ti/bios/include/buf.h>

__FAR__ BUF_Attrs BUF_ATTRS = {
    0               /* Segment ID points to default segment */
};

/*
 *  ======== BUF_create ========
 */
BUF_Handle BUF_create(Uns numbuff, MEM_sizep size, Uns align, BUF_Attrs *attrs)
{
    HeapBuf_Params params;
    BUF_Handle buf = NULL;
    Error_Block eb;

    if (attrs == NULL) {
        attrs = &BUF_ATTRS;
    }    

    Error_init(&eb);

    HeapBuf_Params_init(&params);

    params.buf = MEM_alloc(attrs->segid, size * numbuff, align);
    params.numBlocks = numbuff;
    params.blockSize = size;
    params.align = align;
    params.bufSize = size * numbuff;
    
    if (params.buf != NULL) {
        buf = (BUF_Handle)HeapBuf_create(&params, &eb);
    }

    return (buf);
}

/*
 *  ======== BUF_maxbuff ========
 */
Uns BUF_maxbuff(BUF_Handle buf)
{
    HeapBuf_ExtendedStats stats;

    /*
     *  The total number of allocated buffers at any one point in time will be
     *  written to the stats struct.
     */
    HeapBuf_getExtendedStats((HeapBuf_Handle)buf, &stats);

    return (stats.maxAllocatedBlocks);
}

/*
 *  ======== BUF_stat ========
 */
Void BUF_stat(BUF_Handle buf, BUF_Stat *bufStats)
{    
    Memory_Stats        memStats;

    HeapBuf_getStats((HeapBuf_Handle)buf, &memStats);

    bufStats->postalignsize = HeapBuf_getBlockSize((HeapBuf_Handle)buf); 
    bufStats->size          = HeapBuf_getBlockSize((HeapBuf_Handle)buf);
    bufStats->totalbuffers  = memStats.totalSize / bufStats->postalignsize;
    bufStats->freebuffers   = memStats.totalFreeSize / bufStats->postalignsize;
}


