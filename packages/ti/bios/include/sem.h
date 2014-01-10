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
 *  ======== sem.h ========
 *
 *
 */

#ifndef SEM_
#define SEM_

#ifdef __cplusplus
extern "C" {
#endif

#define xdc_runtime_Error__nolocalnames
#define ti_sysbios_knl_Semaphore__nolocalnames
#define ti_sysbios_knl_Semaphore__internalaccess

#include <ti/bios/include/std.h>
#include <ti/bios/include/obj.h>
#include <ti/bios/include/que.h>
#include <ti/bios/include/sys.h>

#include <xdc/runtime/Error.h>
#include <ti/sysbios/knl/Semaphore.h>

typedef ti_sysbios_knl_Semaphore_Struct SEM_Obj, *SEM_Handle;

typedef struct SEM_Attrs {
    Uns         type;
    String      name;
} SEM_Attrs;

#define SEM_ipost       SEM_post

extern __FAR__ SEM_Attrs SEM_ATTRS;

extern SEM_Handle SEM_create(Int count, SEM_Attrs *attrs);

static inline Int SEM_count(SEM_Handle sem)
{
    return (ti_sysbios_knl_Semaphore_getCount((ti_sysbios_knl_Semaphore_Handle)sem));
}

static inline Void SEM_delete(SEM_Handle sem)
{
    ti_sysbios_knl_Semaphore_Handle handle = (ti_sysbios_knl_Semaphore_Handle)sem;

    ti_sysbios_knl_Semaphore_delete(&handle);
}

static inline Void SEM_new(SEM_Obj *obj, Int count)
{
    ti_sysbios_knl_Semaphore_construct(obj, count, NULL);
}

static inline Bool SEM_pend(SEM_Handle sem, Uns timeout)
{
    return (ti_sysbios_knl_Semaphore_pend((ti_sysbios_knl_Semaphore_Handle)sem, timeout));
}

static inline Bool SEM_pendBinary(SEM_Handle sem, Uns timeout)
{
    ((ti_sysbios_knl_Semaphore_Object *)sem)->mode = 
        ti_sysbios_knl_Semaphore_Mode_BINARY;

    return (ti_sysbios_knl_Semaphore_pend((ti_sysbios_knl_Semaphore_Handle)sem, timeout));
}

static inline Void SEM_post(SEM_Handle sem)
{
    ti_sysbios_knl_Semaphore_post((ti_sysbios_knl_Semaphore_Handle)sem);
}

static inline Void SEM_postBinary(SEM_Handle sem)
{
    ((ti_sysbios_knl_Semaphore_Object *)sem)->mode = 
        ti_sysbios_knl_Semaphore_Mode_BINARY;

    ti_sysbios_knl_Semaphore_post((ti_sysbios_knl_Semaphore_Handle)sem);
}

static inline Void SEM_reset(SEM_Handle sem, Int count)
{
    ti_sysbios_knl_Semaphore_reset((ti_sysbios_knl_Semaphore_Handle)sem, count);
}

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* SEM_ */
