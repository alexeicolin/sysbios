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
 *  ======== swi.h ========
 *
 */

#ifndef SWI_
#define SWI_

#ifdef __cplusplus
extern "C" {
#endif

#define ti_sysbios_knl_Swi__nolocalnames
#define ti_sysbios_BIOS__nolocalnames
#include <ti/sysbios/knl/Swi.h>
#include <ti/sysbios/BIOS.h>

#include <ti/bios/include/fxn.h>
#include <ti/bios/include/sts.h>

typedef ti_sysbios_knl_Swi_Struct          SWI_Obj, *SWI_Handle;

typedef Void (*SWI_Fxn) (Arg,Arg); 

#if defined(_28_)
typedef struct SWI_Args {
    UArg        arg0;
    UArg        arg1;
} SWI_Args;
#endif

typedef struct SWI_Attrs {
    SWI_Fxn     fxn;
    Arg         arg0;
    Arg         arg1;
    Int         priority;
    Uns         mailbox;
} SWI_Attrs;

extern __FAR__ SWI_Attrs SWI_ATTRS;

/*
 *  ======== SWI_isSWI ========
 *  Returns true if current thread is SWI.
 */
static inline Bool SWI_isSWI()
{
    if (ti_sysbios_BIOS_getThreadType() == ti_sysbios_BIOS_ThreadType_Swi) {
        return (TRUE);
    }

    return (FALSE);
}

static inline Void SWI_andn(SWI_Handle swi, Uns key)
{
    ti_sysbios_knl_Swi_andn((ti_sysbios_knl_Swi_Handle)swi, key);
}

static inline Void SWI_dec(SWI_Handle swi)
{
    ti_sysbios_knl_Swi_dec((ti_sysbios_knl_Swi_Handle)swi);
}

static inline Void SWI_disable(Void)
{
    ti_sysbios_knl_Swi_disable();
}

static inline Void SWI_enable(Void)
{
    ti_sysbios_knl_Swi_enable();
}

static inline Void SWI_inc(SWI_Handle swi)
{
    ti_sysbios_knl_Swi_inc((ti_sysbios_knl_Swi_Handle)swi);
}

static inline Void SWI_or(SWI_Handle swi, Uns key)
{
    ti_sysbios_knl_Swi_or((ti_sysbios_knl_Swi_Handle)swi, key);
}

static inline Void SWI_post(SWI_Handle swi)
{
    ti_sysbios_knl_Swi_post((ti_sysbios_knl_Swi_Handle)swi);
}

static inline Uns SWI_getmbox(Void)
{
    return ti_sysbios_knl_Swi_getTrigger();
}

static inline SWI_Handle SWI_self(Void)
{
    return ((SWI_Handle)ti_sysbios_knl_Swi_self());
}

static inline Void SWI_delete(SWI_Handle swi)
{
    ti_sysbios_knl_Swi_Handle handle = (ti_sysbios_knl_Swi_Handle)swi;
    
#if defined(_28_)
    xdc_runtime_Memory_free(NULL, (Ptr)swi->__f3, sizeof(SWI_Args));
#endif
    ti_sysbios_knl_Swi_delete(&handle);
}

extern SWI_Handle SWI_create(SWI_Attrs *attrs);
extern Uns SWI_raisepri(Uns mask);
extern Void SWI_restorepri(Uns key);
extern Uns SWI_getpri(SWI_Handle swi);
extern Void SWI_getattrs(SWI_Handle swi, SWI_Attrs *attrs);
extern Void SWI_setattrs(SWI_Handle swi, SWI_Attrs *attrs);
extern Void SWI_andnHook(Arg swi, Arg key);
extern Void SWI_orHook(Arg swi, Arg key);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* SWI_ */

