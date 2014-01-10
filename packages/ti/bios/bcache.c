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
 *  ======== bcache.c ========
 */

#include <xdc/std.h>

#include <ti/bios/include/bcache.h>
#ifdef xdc_target__isa_66

#include <ti/sysbios/family/c66/Cache.h>

Void BCACHE_wait(Void)
{
    ti_sysbios_family_c66_Cache_wait();
}

Void BCACHE_inv(Ptr blockPtr, size_t byteCnt, Bool wait)
{
    ti_sysbios_family_c66_Cache_inv(blockPtr, byteCnt,
        ti_sysbios_family_c66_Cache_Type_ALL, wait);
}

Void BCACHE_wb(Ptr blockPtr, size_t byteCnt, Bool wait)
{
    ti_sysbios_family_c66_Cache_wb(blockPtr, byteCnt,
        ti_sysbios_family_c66_Cache_Type_ALL, wait);
}

Void BCACHE_wbInv(Ptr blockPtr, size_t byteCnt, Bool wait)
{
    ti_sysbios_family_c66_Cache_wbInv(blockPtr, byteCnt,
        ti_sysbios_family_c66_Cache_Type_ALL, wait);
}

Void BCACHE_invL1pAll(Void)
{
    ti_sysbios_family_c66_Cache_invL1pAll();
}

Void BCACHE_wbAll(Void)
{
    ti_sysbios_family_c66_Cache_wbAll();
}

Void BCACHE_wbInvAll(Void)
{
    ti_sysbios_family_c66_Cache_wbInvAll();
}

#else           /* end of C66 definitions */

#include <ti/sysbios/family/c64p/Cache.h>

Void BCACHE_wait(Void)
{
    ti_sysbios_family_c64p_Cache_wait();
}

Void BCACHE_inv(Ptr blockPtr, size_t byteCnt, Bool wait)
{
    ti_sysbios_family_c64p_Cache_inv(blockPtr, byteCnt,
        ti_sysbios_family_c64p_Cache_Type_ALL, wait);
}

Void BCACHE_wb(Ptr blockPtr, size_t byteCnt, Bool wait)
{
    ti_sysbios_family_c64p_Cache_wb(blockPtr, byteCnt,
        ti_sysbios_family_c64p_Cache_Type_ALL, wait);
}

Void BCACHE_wbInv(Ptr blockPtr, size_t byteCnt, Bool wait)
{
    ti_sysbios_family_c64p_Cache_wbInv(blockPtr, byteCnt,
        ti_sysbios_family_c64p_Cache_Type_ALL, wait);
}

Void BCACHE_invL1pAll(Void)
{
    ti_sysbios_family_c64p_Cache_invL1pAll();
}

Void BCACHE_wbAll(Void)
{
    ti_sysbios_family_c64p_Cache_wbAll();
}

Void BCACHE_wbInvAll(Void)
{
    ti_sysbios_family_c64p_Cache_wbInvAll();
}

#endif          /* end of C64P definitions */


