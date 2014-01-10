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
 *  ======== swi.c ========
 */

#define ti_sysbios_knl_Swi__internalaccess

#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/knl/Swi.h>
#include <ti/sysbios/knl/Intrinsics.h>
#include <ti/sysbios/knl/Queue.h>
#include <ti/sysbios/hal/Hwi.h>

#include <ti/bios/include/std.h>
#include <ti/bios/include/mem.h>
#include <ti/bios/include/swi.h>

__FAR__ SWI_Attrs SWI_ATTRS = { 
    (SWI_Fxn)0,
    0,          /* arg0 */
    0,          /* arg1 */
    1,          /* priority */ /* priority 0 is for the SWI scheduler */
    0           /* mailbox */
};

#if defined(_28_)
/*
 *  ======== SWI_stub ========
 *  Used solely to cast UArgs to Args and then pass them to the user function
 */
Void SWI_stub(UArg fxn, UArg args) {
    SWI_Fxn legacyFxn;
    SWI_Args *legacyArgs;
    
    legacyFxn = (SWI_Fxn)fxn;
    legacyArgs = (SWI_Args *)args;
    
    legacyFxn((Arg)legacyArgs->arg0, (Arg)legacyArgs->arg1);
}
#endif

/*
 *  ======== SWI_create ========
 */
SWI_Handle SWI_create(SWI_Attrs *attrs)
{
#if defined(_28_)
    SWI_Args *args;
#endif
    Swi_Params parms;
    Swi_Handle swi;
    Error_Block eb;

    Swi_Params_init(&parms);

    Error_init(&eb);

    if (attrs == NULL) {
        attrs = &SWI_ATTRS;
    }

    parms.priority  = attrs->priority;
    parms.trigger = attrs->mailbox;

#if defined(_28_)
    args = (SWI_Args *)Memory_alloc(NULL, sizeof(SWI_Args), 0, &eb);
    args->arg0 = (UArg)(attrs->arg0);
    args->arg1 = (UArg)(attrs->arg1);
    parms.arg0 = (UArg)(attrs->fxn);
    parms.arg1 = (UArg)args;

    swi = Swi_create((Swi_FuncPtr)SWI_stub, &parms, &eb);
#else
    parms.arg0 = (UArg)(attrs->arg0);
    parms.arg1 = (UArg)(attrs->arg1);
    
    swi = Swi_create((Swi_FuncPtr)(attrs->fxn), &parms, &eb);
#endif

    return ((SWI_Handle)swi);
}


/*
 *  ======== SWI_getattrs ========
 *  SWI_getattrs retrieves attributes of an existing SWI object
 */
Void SWI_getattrs(SWI_Handle swi, SWI_Attrs *attrs)
{
    Swi_Handle handle = (Swi_Handle)swi;

    if (attrs != NULL) {
#if defined(_28_)
        attrs->fxn = (SWI_Fxn)handle->arg0;
        attrs->arg0 = (Arg)((SWI_Args *)(handle->arg1))->arg0;
        attrs->arg1 = (Arg)((SWI_Args *)(handle->arg1))->arg1;
#else
        attrs->fxn =  (SWI_Fxn)handle->fxn;
        attrs->arg0 = (Arg)(handle->arg0);
        attrs->arg1 = (Arg)(handle->arg1);
#endif
        attrs->priority = handle->priority;
        attrs->mailbox = handle->trigger;
    }
}

/*
 *  ======== SWI_setattrs ========
 *  SWI_setattrs sets attributes of an existing SWI object
 */
Void SWI_setattrs(SWI_Handle swi, SWI_Attrs *attrs)
{
#if defined(_28_)
    SWI_Args *args;
    Swi_Handle handle = (Swi_Handle)swi;
#endif
    Swi_Params swiParams;
    Uns key;

    Swi_Params_init(&swiParams);

    key = Hwi_disable();

    if (attrs == NULL) {
        attrs = &SWI_ATTRS;
    }

    swiParams.priority = attrs->priority;
    swiParams.trigger = attrs->mailbox;
    
#if defined(_28_)
    args = (SWI_Args *)(handle->arg1);
    args->arg0 = (UArg)(attrs->arg0);
    args->arg1 = (UArg)(attrs->arg1);
    swiParams.arg0 = (UArg)(attrs->fxn);
    swiParams.arg1 = (UArg)args;
    
    Swi_construct((Swi_Struct *)swi, (Swi_FuncPtr)SWI_stub, &swiParams, NULL);
#else
    swiParams.arg0 = (UArg)(attrs->arg0);
    swiParams.arg1 = (UArg)(attrs->arg1);
    
    Swi_construct((Swi_Struct *)swi, (Swi_FuncPtr)attrs->fxn, &swiParams, NULL);
#endif

    Hwi_restore(key);
}

Uns SWI_raisepri(Uns mask)
{
    return (1 << (Swi_raisePri(Intrinsics_maxbit(mask))+1));
}

Void SWI_restorepri(Uns key)
{
    Swi_restorePri(Intrinsics_maxbit(key)-1);
}


Uns SWI_getpri(SWI_Handle swi)
{
    return (1 << Swi_getPri((Swi_Handle)swi));
}

Void SWI_andnHook(Arg swi, Arg key)
{
    SWI_andn((SWI_Handle)swi, (Uns)(LgUns)key);
}

Void SWI_orHook(Arg swi, Arg key)
{
    SWI_or((SWI_Handle)swi, (Uns)(LgUns)key);
}
