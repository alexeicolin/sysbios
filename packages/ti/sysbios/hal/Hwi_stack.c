/*
 * Copyright (c) 2013, Texas Instruments Incorporated
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
 *  ======== Hwi_stack.c ========
 */

#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include "package/internal/Hwi.xdc.h"

#ifdef __ti__
#pragma CODE_SECTION(ti_sysbios_hal_Hwi_initStack, ".text:ti_sysbios_hal_Hwi_initStack")
#pragma CODE_SECTION(ti_sysbios_hal_Hwi_checkStack, ".text:ti_sysbios_hal_Hwi_checkStack")
#endif

#if defined(xdc_target__isaCompatible_430)    \
    || defined(xdc_target__isaCompatible_430X)

/*
 *  ======== Hwi_initStack ========
 *  Initialize the Common Interrupt Stack
 */
Void ti_sysbios_hal_Hwi_initStack(Void)
{
    Hwi_StackInfo stkInfo;
    UInt *curStack;

    /* Get stack base and size */
    Hwi_getStackInfo(&stkInfo, FALSE);

    curStack = (UInt *)(stkInfo.hwiStackBase);

    while (curStack < (UInt *)(&curStack)) {
        *((UInt *)curStack) = 0xbebe;
        curStack++;
    }
}

#else

/*
 *  ======== Hwi_initStack ========
 *  Initialize the Common Interrupt Stack
 */
Void ti_sysbios_hal_Hwi_initStack(Void)
{
    Hwi_StackInfo stkInfo;
    UArg curStack;

    /* Get stack base and size */
    Hwi_getStackInfo(&stkInfo, FALSE);

#ifdef xdc_target__isaCompatible_28
    curStack = (UArg)(stkInfo.hwiStackBase) + (SizeT)(stkInfo.hwiStackSize);

    while (--curStack > ((UArg)(&curStack) + 1)) {
        *((UInt8 *)curStack) = 0xbebe;
    }
#else
    curStack = (UArg)(stkInfo.hwiStackBase);

    while (curStack < (UArg)(&curStack)) {
        *((UInt8 *)curStack) = 0xbe;
        curStack++;
    }
#endif
}

#endif

/*
 *  ======== Hwi_checkStack ========
 *  Check the Common Interrupt Stack for overflow.
 */
Void ti_sysbios_hal_Hwi_checkStack(Void)
{
    Hwi_StackInfo stkInfo;

    if (Hwi_getStackInfo(&stkInfo, FALSE)) {
        Error_raise(NULL, Hwi_E_stackOverflow, 0, 0);
    }
}
