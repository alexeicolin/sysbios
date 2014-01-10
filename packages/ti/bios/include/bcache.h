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
 *  ======== bcache.h ========
 *
 */

#ifndef BCACHE_
#define BCACHE_

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_TMS320C6600)
#define ti_sysbios_family_c66_Cache__nolocalnames
#include <ti/sysbios/family/c66/Cache.h>

typedef ti_sysbios_family_c66_Cache_L1Size BCACHE_L1_Size;
typedef ti_sysbios_family_c66_Cache_L2Size BCACHE_L2_Size;
typedef ti_sysbios_family_c66_Cache_Mode BCACHE_Mode;
typedef ti_sysbios_family_c66_Cache_Type BCACHE_Level;
typedef ti_sysbios_family_c66_Cache_Mar BCACHE_Mar;

/*
 *  L1 and L2 Cache size structure
 */
typedef struct BCACHE_Size {
    BCACHE_L1_Size l1psize;
    BCACHE_L1_Size l1dsize;
    BCACHE_L2_Size l2size;
} BCACHE_Size;

#define BCACHE_L1_0K ti_sysbios_family_c66_Cache_L1Size_0K
#define BCACHE_L1_4K ti_sysbios_family_c66_Cache_L1Size_4K
#define BCACHE_L1_8K ti_sysbios_family_c66_Cache_L1Size_8K
#define BCACHE_L1_16K ti_sysbios_family_c66_Cache_L1Size_16K
#define BCACHE_L1_32K ti_sysbios_family_c66_Cache_L1Size_32K

#define BCACHE_L2_0K ti_sysbios_family_c66_Cache_L2Size_0K
#define BCACHE_L2_32K ti_sysbios_family_c66_Cache_L2Size_32K
#define BCACHE_L2_64K ti_sysbios_family_c66_Cache_L2Size_64K
#define BCACHE_L2_128K ti_sysbios_family_c66_Cache_L2Size_128K
#define BCACHE_L2_256K ti_sysbios_family_c66_Cache_L2Size_256K

#define BCACHE_NORMAL ti_sysbios_family_c66_Cache_Mode_NORMAL
#define BCACHE_FREEZE ti_sysbios_family_c66_Cache_Mode_FREEZE
#define BCACHE_BYPASS ti_sysbios_family_c66_Cache_Mode_BYPASS

#define BCACHE_L1D ti_sysbios_family_c66_Cache_Type_L1D
#define BCACHE_L1P ti_sysbios_family_c66_Cache_Type_L1P
#define BCACHE_L2 ti_sysbios_family_c66_Cache_Type_L2

#define BCACHE_MAR_DISABLE ti_sysbios_family_c66_Cache_Mar_DISABLE
#define BCACHE_MAR_ENABLE ti_sysbios_family_c66_Cache_Mar_ENABLE

static inline Void BCACHE_setSize(BCACHE_Size *size)
{
    ti_sysbios_family_c66_Cache_Size cacheSize;
    
    cacheSize.l1pSize = size->l1psize;
    cacheSize.l1dSize = size->l1dsize;
    cacheSize.l2Size = size->l2size;

    ti_sysbios_family_c66_Cache_setSize(&cacheSize);
}

static inline Void BCACHE_getSize(BCACHE_Size *size)
{
    ti_sysbios_family_c66_Cache_Size cacheSize;
    
    ti_sysbios_family_c66_Cache_getSize(&cacheSize);

    size->l1psize = cacheSize.l1pSize;
    size->l1dsize = cacheSize.l1dSize;
    size->l2size = cacheSize.l2Size;
}

static inline BCACHE_Mode BCACHE_setMode(BCACHE_Level level, BCACHE_Mode mode)
{
    return ti_sysbios_family_c66_Cache_setMode(level, mode);
}

static inline BCACHE_Mode BCACHE_getMode(BCACHE_Level level)
{
    return ti_sysbios_family_c66_Cache_getMode(level);
}

static inline Void BCACHE_setMar(Ptr baseAddr, size_t byteSize, UInt32 val)
{
    ti_sysbios_family_c66_Cache_setMar(baseAddr, byteSize, val);
}

static inline UInt32 BCACHE_getMar(Ptr baseAddr)
{
    return ti_sysbios_family_c66_Cache_getMar(baseAddr);
}

#else           /* end of 66 definitions */

#define ti_sysbios_family_c64p_Cache__nolocalnames
#include <ti/sysbios/family/c64p/Cache.h>

typedef ti_sysbios_family_c64p_Cache_L1Size BCACHE_L1_Size;
typedef ti_sysbios_family_c64p_Cache_L2Size BCACHE_L2_Size;
typedef ti_sysbios_family_c64p_Cache_Mode BCACHE_Mode;
typedef ti_sysbios_family_c64p_Cache_Type BCACHE_Level;
typedef ti_sysbios_family_c64p_Cache_Mar BCACHE_Mar;

/*
 *  L1 and L2 Cache size structure
 */
typedef struct BCACHE_Size {
    BCACHE_L1_Size l1psize;
    BCACHE_L1_Size l1dsize;
    BCACHE_L2_Size l2size;
} BCACHE_Size;

#define BCACHE_L1_0K ti_sysbios_family_c64p_Cache_L1Size_0K
#define BCACHE_L1_4K ti_sysbios_family_c64p_Cache_L1Size_4K
#define BCACHE_L1_8K ti_sysbios_family_c64p_Cache_L1Size_8K
#define BCACHE_L1_16K ti_sysbios_family_c64p_Cache_L1Size_16K
#define BCACHE_L1_32K ti_sysbios_family_c64p_Cache_L1Size_32K

#define BCACHE_L2_0K ti_sysbios_family_c64p_Cache_L2Size_0K
#define BCACHE_L2_32K ti_sysbios_family_c64p_Cache_L2Size_32K
#define BCACHE_L2_64K ti_sysbios_family_c64p_Cache_L2Size_64K
#define BCACHE_L2_128K ti_sysbios_family_c64p_Cache_L2Size_128K
#define BCACHE_L2_256K ti_sysbios_family_c64p_Cache_L2Size_256K

#define BCACHE_NORMAL ti_sysbios_family_c64p_Cache_Mode_NORMAL
#define BCACHE_FREEZE ti_sysbios_family_c64p_Cache_Mode_FREEZE
#define BCACHE_BYPASS ti_sysbios_family_c64p_Cache_Mode_BYPASS

#define BCACHE_L1D ti_sysbios_family_c64p_Cache_Type_L1D
#define BCACHE_L1P ti_sysbios_family_c64p_Cache_Type_L1P
#define BCACHE_L2 ti_sysbios_family_c64p_Cache_Type_L2

#define BCACHE_MAR_DISABLE ti_sysbios_family_c64p_Cache_Mar_DISABLE
#define BCACHE_MAR_ENABLE ti_sysbios_family_c64p_Cache_Mar_ENABLE

static inline Void BCACHE_setSize(BCACHE_Size *size)
{
    ti_sysbios_family_c64p_Cache_Size cacheSize;
    
    cacheSize.l1pSize = size->l1psize;
    cacheSize.l1dSize = size->l1dsize;
    cacheSize.l2Size = size->l2size;

    ti_sysbios_family_c64p_Cache_setSize(&cacheSize);
}

static inline Void BCACHE_getSize(BCACHE_Size *size)
{
    ti_sysbios_family_c64p_Cache_Size cacheSize;
    
    ti_sysbios_family_c64p_Cache_getSize(&cacheSize);

    size->l1psize = cacheSize.l1pSize;
    size->l1dsize = cacheSize.l1dSize;
    size->l2size = cacheSize.l2Size;
}

static inline BCACHE_Mode BCACHE_setMode(BCACHE_Level level, BCACHE_Mode mode)
{
    return ti_sysbios_family_c64p_Cache_setMode(level, mode);
}

static inline BCACHE_Mode BCACHE_getMode(BCACHE_Level level)
{
    return ti_sysbios_family_c64p_Cache_getMode(level);
}

static inline Void BCACHE_setMar(Ptr baseAddr, size_t byteSize, BCACHE_Mar val)
{
    ti_sysbios_family_c64p_Cache_setMar(baseAddr, byteSize, val);
}

static inline BCACHE_Mar BCACHE_getMar(Ptr baseAddr)
{
    return ti_sysbios_family_c64p_Cache_getMar(baseAddr);
}

#endif          /* end of 64P definitions */

extern Void BCACHE_wait(Void);

extern Void BCACHE_inv(Ptr blockPtr, size_t byteCnt, Bool wait);

extern Void BCACHE_wb(Ptr blockPtr, size_t byteCnt, Bool wait);

extern Void BCACHE_wbInv(Ptr blockPtr, size_t byteCnt, Bool wait);

extern Void BCACHE_invL1pAll(Void);

extern Void BCACHE_wbAll(Void);

extern Void BCACHE_wbInvAll(Void);

/*
 *  Support for older BIOS 5 legacy implementation
 */
#define BCACHE_invalidate(ptr, cnt)             BCACHE_inv(ptr, cnt, 1)

/* BCACHE_writeback() performs a writeback invalidate */
#define BCACHE_writeback(ptr, cnt)              BCACHE_wbInv(ptr, cnt, 1)

#define BCACHE_writebackInvalidate(ptr, cnt)    BCACHE_wbInv(ptr, cnt, 1)
#define BCACHE_writebackInvalidateAll()         BCACHE_wbInvAll()

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  /* BCACHE_ */
