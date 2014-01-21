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
 *  ======== Hwi.c ========
 */

#include <xdc/std.h>

#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>
#include <xdc/runtime/Startup.h>
#include <xdc/runtime/Log.h>

#include <ti/sysbios/interfaces/IHwi.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Swi.h>
#include <ti/sysbios/family/arm/HwiCommon.h>

#include "package/internal/Hwi.xdc.h"

extern Void ti_sysbios_family_arm_dm6446_Hwi_init(Void);
extern Char *ti_sysbios_family_xxx_Hwi_switchToIsrStack();
extern Void ti_sysbios_family_xxx_Hwi_switchToTaskStack(Char *oldTaskSP);
extern Void ti_sysbios_family_xxx_Hwi_switchAndRunFunc(Void (*func)());

#define Hwi_switchToIsrStack ti_sysbios_family_xxx_Hwi_switchToIsrStack
#define Hwi_switchToTaskStack ti_sysbios_family_xxx_Hwi_switchToTaskStack
#define Hwi_switchAndRunFunc ti_sysbios_family_xxx_Hwi_switchAndRunFunc

#ifdef ti_sysbios_family_arm_dm6446_Hwi_dispatcherTaskSupport__D
/* disable unused local variable warning during optimized compile */
#pragma diag_suppress=179
#define TASK_DISABLE Task_disable
#define TASK_RESTORE Task_restoreHwi
#else
#define TASK_DISABLE Hwi_taskDisable
#define TASK_RESTORE Hwi_taskRestoreHwi
#endif

#ifdef ti_sysbios_family_arm_dm6446_Hwi_dispatcherSwiSupport__D
/* disable unused local variable warning during optimized compile */
#pragma diag_suppress=179
#define SWI_DISABLE Swi_disable
#define SWI_RESTORE Swi_restoreHwi
#else
#define SWI_DISABLE Hwi_swiDisable
#define SWI_RESTORE Hwi_swiRestoreHwi
#endif

/*
 *  ======== Hwi_Module_startup ========
 *  must initialize IRQ, FIQ, (and SWI?) SPs (R13s)
 */
Int Hwi_Module_startup (Int startupPhase)
{
    int i;
    Hwi_Object *hwi;

    /* must wait for these modules to initialize first */
    if (!Startup_rtsDone()) {
        return (Startup_NOTDONE);
    }

    /* okay to proceed with initialization */

#ifndef ti_sysbios_hal_Hwi_DISABLE_ALL_HOOKS
    for (i = 0; i < Hwi_hooks.length; i++) {
        if (Hwi_hooks.elem[i].registerFxn != NULL) {
            Hwi_hooks.elem[i].registerFxn(i);
        }
    }
#endif

    /* set up FIQ stack pointer & switch to SYSTEM mode */
    Hwi_init();                 

    Hwi_initIntController();

    /* 
     * Initialize the pointer to the isrStack.
     *
     * The dispatcher's SP is decremented to accomodate its local variables 
     * BEFORE switching to the ISR stack. Consequently, the intial value of
     * the ISR stack SP must leave room for these variables.
     *
     * Leave room for up to 32 32 bit local variables.
     */
    Hwi_module->isrStack = (Char *) (((UInt32) (Hwi_module->isrStackBase) & 0xfffffff8) + 
                                     (UInt32) Hwi_module->isrStackSize - 32 * sizeof(Int));

    Hwi_module->taskSP = (Char *)-1;    /* signal that we're executing */
                                        /* on the ISR stack */

    for (i = 0; i < Hwi_NUM_INTERRUPTS; i++) {
        hwi = Hwi_module->dispatchTable[i];
        if (hwi !=  NULL) {
            Hwi_postInit(hwi, NULL);
        }
    }

    return Startup_DONE;        /* Max startup phase needed is 0 */
}

/*
 *  ======== Hwi_Instance_init  ========
 */
Int Hwi_Instance_init(Hwi_Object *hwi, Int intNum, 
                      Hwi_FuncPtr fxn, const Hwi_Params *params,
                      Error_Block *eb)
{
    Int status;

    if (Hwi_module->dispatchTable[intNum] != NULL) {
        Error_raise(eb, Hwi_E_alreadyDefined, intNum, 0);
        return (1);
    }

    Hwi_module->dispatchTable[intNum] = hwi;

    hwi->intNum = intNum;

    Hwi_reconfig(hwi, fxn, params);

#ifndef ti_sysbios_hal_Hwi_DISABLE_ALL_HOOKS
    if (Hwi_hooks.length > 0) {
        /* Allocate environment space for each hook instance. */
        hwi->hookEnv = Memory_calloc(Hwi_Object_heap(),
                Hwi_hooks.length * sizeof(Ptr), 0, eb);

        if (hwi->hookEnv == NULL) {
            return (1);
        }
    }
#endif

    hwi->irp = 0;

    status = Hwi_postInit(hwi, eb);

    if (Error_check(eb)) {
        return (2 + status);
    }

    return (0);
}

/*
 *  ======== Hwi_postInit ========
 *  Function to be called during module startup to complete the
 *  initialization of any statically created or constructed Hwi.
 *  returns (0) and clean 'eb' on success
 *  returns 'eb' *and* 'n' for number of successful createFxn() calls iff
 *      one of the createFxn() calls fails
 */
Int Hwi_postInit (Hwi_Object *hwi, Error_Block *eb)
{
#ifndef ti_sysbios_hal_Hwi_DISABLE_ALL_HOOKS
    Int i;

    for (i = 0; i < Hwi_hooks.length; i++) {
        hwi->hookEnv[i] = (Ptr)0;
        if (Hwi_hooks.elem[i].createFxn != NULL) {
            Hwi_hooks.elem[i].createFxn((IHwi_Handle)hwi, eb);

            if (Error_check(eb)) {
                return (i);
            }
        }
    }
#endif

    return (0);
}

/*
 *  ======== Hwi_Instance_finalize ========
 *  Here on Hwi_delete() or failed Hwi_create().
 */
Void Hwi_Instance_finalize(Hwi_Object *hwi, Int status)
{
    Int i, cnt;
    UInt intNum;

    for (intNum = 0; intNum < Hwi_NUM_INTERRUPTS; intNum++) {
        if (Hwi_module->dispatchTable[intNum] == hwi) {
            break;
        }
    }

    Hwi_disableInterrupt(intNum);
    Hwi_module->dispatchTable[intNum] = NULL;

    if (status == 1) {  /* failed Hwi_create */
        return;
    }

#ifndef ti_sysbios_hal_Hwi_DISABLE_ALL_HOOKS
    if (Hwi_hooks.length > 0) {
        if (status == 0) {
            cnt = Hwi_hooks.length;
        }
        else {
            cnt = status - 2;
        }

        for (i = 0; i < cnt; i++) {
            if (Hwi_hooks.elem[i].deleteFxn != NULL) {
                Hwi_hooks.elem[i].deleteFxn((IHwi_Handle)hwi);
            }
        }

        Memory_free(Hwi_Object_heap(), hwi->hookEnv,
                Hwi_hooks.length * sizeof(Ptr));
    }
#endif
}

/*
 *  ======== Hwi_initIntController ========
 */
Void Hwi_initIntController()
{
    UInt i, sizeCode;

    /*
     * The interrupt controller has no RESET feature and
     * none would be required after a hard reset or power up.
     * But the CCS reset/reload process doesn't do a hard reset.
     * The code below tries to "virtually" reset the intc.
     */

    /* Clear any currently pending interrupts */
    Hwi_aIntc.FIQ0 = 0xFFFFFFFF;
    Hwi_aIntc.FIQ1 = 0xFFFFFFFF;
    Hwi_aIntc.IRQ0 = 0xFFFFFFFF;
    Hwi_aIntc.IRQ1 = 0xFFFFFFFF;

    /*
     * INTCTL settings:
     *
     *  IDMODE (Bit 2) = 0 = disable immediately
     *  IERAW  (Bit 1) = 0 = masked ints in IRQENTRY
     *  FERAW  (Bit 0) = 0 = masked ints in FIQENTRY
     */

    Hwi_aIntc.INTCTL = 0;

    switch (Hwi_eabaseSize) {
        case 4:
            sizeCode = 0;
            break;      
        case 8:
            sizeCode = 1;
            break;      
        case 16:
            sizeCode = 2;
            break;      
        case 32:
            sizeCode = 3;
            break;      
    }

    Hwi_aIntc.EABASE = (Hwi_aIntc.EABASE & 0x0ffffff8) | sizeCode;

    /*
     * Configure the interrupt priority (and therefore type)
     */

    for (i=0; i<64; i++) {
        if (Hwi_module->dispatchTable[i] == NULL) {
            Hwi_setPriority(i, 7);      /* lowest priority IRQ */
        }
        else {
            Hwi_setPriority(i, Hwi_module->dispatchTable[i]->priority);
        }
    }
    Hwi_restoreEINT0(Hwi_module->eint0Mask); /* EINT0 per static Hwi settings */
    Hwi_restoreEINT1(Hwi_module->eint1Mask); /* EINT1 per static Hwi settings */
}

/*
 *  ======== Hwi_startup ========
 */
Void Hwi_startup()
{
    Hwi_enable();
}

/*
 * Below APIs provided for backward compatability.
 */

#ifdef __ti__
#pragma CODE_SECTION(ti_sysbios_family_arm_dm6446_Hwi_disable__E, ".text:ti_sysbios_family_arm_dm6446_Hwi_disable__E")
#pragma CODE_SECTION(ti_sysbios_family_arm_dm6446_Hwi_enable__E, ".text:ti_sysbios_family_arm_dm6446_Hwi_enable__E")
#pragma CODE_SECTION(ti_sysbios_family_arm_dm6446_Hwi_restore__E, ".text:ti_sysbios_family_arm_dm6446_Hwi_restore__E")
#pragma FUNC_EXT_CALLED(ti_sysbios_family_arm_dm6446_Hwi_disable__E);
#pragma FUNC_EXT_CALLED(ti_sysbios_family_arm_dm6446_Hwi_enable__E);
#pragma FUNC_EXT_CALLED(ti_sysbios_family_arm_dm6446_Hwi_restore__E);
#endif

/*
 *  ======== Hwi_disable ========
 *  disables only IRQ interrupts
 */

UInt ti_sysbios_family_arm_dm6446_Hwi_disable__E()
{
    return HwiCommon_disable();
}

/*
 *  ======== Hwi_enable ========
 *  enables only IRQ interrupts
 */
UInt ti_sysbios_family_arm_dm6446_Hwi_enable__E()
{
    return HwiCommon_enable();
}

/*
 *  ======== Hwi_restore ========
 *  restores only IRQ interrupts
 */
Void ti_sysbios_family_arm_dm6446_Hwi_restore__E(UInt key)
{
    HwiCommon_restore(key);
}

/*
 *  ======== Hwi_disableFIQ ========
 *  disables only FIQ interrupts
 */

UInt Hwi_disableFIQ()
{
    return HwiCommon_disableFIQ();
}

/*
 *  ======== Hwi_enableFIQ ========
 *  enables only FIQ interrupts
 */
UInt Hwi_enableFIQ()
{
    return HwiCommon_enableFIQ();
}

/*
 *  ======== Hwi_restoreFIQ ========
 *  restores only IRQ interrupts
 */
Void Hwi_restoreFIQ( UInt key )
{
    HwiCommon_restoreFIQ(key);
}

/*
 *  ======== Hwi_disableFIQ ========
 *  disables only FIQ interrupts
 */

UInt Hwi_disableIRQ()
{
    return HwiCommon_disableIRQ();
}

/*
 *  ======== Hwi_enableIRQ ========
 *  enables only IRQ interrupts
 */
UInt Hwi_enableIRQ()
{
    return HwiCommon_enableIRQ();
}

/*
 *  ======== Hwi_restoreIRQ ========
 *  restores only IRQ interrupts
 */
Void Hwi_restoreIRQ( UInt key )
{
    HwiCommon_restoreIRQ(key);
}

/*
 *  ======== Hwi_disableEINT0 ========
 */
Bits32 Hwi_disableEINT0(Bits32 mask)
{
    UInt key;
    Bits32 oldEINT0;

    key = Hwi_disable();
    oldEINT0 = Hwi_aIntc.EINT0;
    Hwi_aIntc.EINT0 &= ~mask;
    Hwi_restore(key);

    return (oldEINT0);
}

/*
 *  ======== Hwi_disableEINT1 ========
 */
Bits32 Hwi_disableEINT1(Bits32 mask)
{
    UInt key;
    Bits32 oldEINT1;

    key = Hwi_disable();
    oldEINT1 = Hwi_aIntc.EINT1;
    Hwi_aIntc.EINT1 &= ~mask;
    Hwi_restore(key);

    return (oldEINT1);
}

/*
 *  ======== Hwi_enableEINT0 ========
 */
Bits32 Hwi_enableEINT0(Bits32 mask)
{
    UInt key;
    Bits32 oldEINT0;

    key = Hwi_disable();
    oldEINT0 = Hwi_aIntc.EINT0;
    Hwi_aIntc.EINT0 |= mask;
    Hwi_restore(key);

    return (oldEINT0);
}

/*
 *  ======== Hwi_enableEINT1 ========
 */
Bits32 Hwi_enableEINT1(Bits32 mask)
{
    UInt key;
    Bits32 oldEINT1;

    key = Hwi_disable();
    oldEINT1 = Hwi_aIntc.EINT1;
    Hwi_aIntc.EINT1 |= mask;
    Hwi_restore(key);

    return (oldEINT1);
}

/*
 *  ======== Hwi_restoreEINT0 ========
 */
Bits32 Hwi_restoreEINT0(Bits32 mask)
{
    UInt key;
    Bits32 oldEINT0;

    key = Hwi_disable();
    oldEINT0 = Hwi_aIntc.EINT0;
    Hwi_aIntc.EINT0 = mask;
    Hwi_restore(key);

    return (oldEINT0);
}

/*
 *  ======== Hwi_restoreEINT1 ========
 */
Bits32 Hwi_restoreEINT1(Bits32 mask)
{
    UInt key;
    Bits32 oldEINT1;

    key = Hwi_disable();
    oldEINT1 = Hwi_aIntc.EINT1;
    Hwi_aIntc.EINT1 = mask;
    Hwi_restore(key);

    return (oldEINT1);
}

/*
 *  ======== Hwi_getHandle ========
 */
Hwi_Object *Hwi_getHandle(UInt intNum)
{
    return (Hwi_module->dispatchTable[intNum]);
}

/*
 *  ======== Hwi_getInstance ========
 */
Hwi_Object *Hwi_getInstance(UInt intNum)
{
    return (Hwi_module->dispatchTable[intNum]);
}

/*
 *  ======== Hwi_post ========
 */
Void Hwi_post(UInt intNum)
{
}

/*
 *  ======== Hwi_getTaskSP ========
 */
Char *Hwi_getTaskSP()
{
    return (HwiCommon_getTaskSP());
}

/*
 *  ======== Hwi_disableInterrupt ========
 */
UInt Hwi_disableInterrupt(UInt intNum)
{
    if ( intNum < 32 ) {
        return (Hwi_disableEINT0(1 << intNum)) ;
    }
    else {
        intNum %= 32;    // normalize to 0 - 31
        return (Hwi_disableEINT1(1 << intNum));
    }
}

/*
 *  ======== Hwi_enableInterrupt ========
 */
UInt Hwi_enableInterrupt(UInt intNum)
{
    if ( intNum < 32 ) {
        return (Hwi_enableEINT0 (1 << intNum));
    }
    else {
        intNum %= 32;    // normalize to 0 - 31
        return (Hwi_enableEINT1 (1 << intNum));
    }
}

/*
 *  ======== Hwi_RestoreInterrupt ========
 */
Void Hwi_restoreInterrupt(UInt intNum, UInt key)
{
    if (key & (1 << (intNum%32))) {
        Hwi_enableInterrupt(intNum);
    }
    else {
        Hwi_disableInterrupt(intNum);
    }
}

/*
 *  ======== Hwi_interruptType ========
 *  returns 0 if interrupt is IRQ
 *  returns 1 if interrupt is FIQ
 */
UInt Hwi_interruptType(UInt intNum)
{
    UInt intpr;

    intpr = Hwi_aIntc.INTPR[intNum/8];
    intpr = intpr >> (intNum % 8)*4;
    intpr &= 0xf;
    if (intpr < 2) {
        return (1);
    }
    else {
        return (0);
    }
}

/*
 *  ======== Hwi_clearInterrupt ========
 */
Void Hwi_clearInterrupt(UInt intNum)
{
    if (Hwi_interruptType(intNum) == 0) {
        if (intNum < 32) {
            Hwi_aIntc.IRQ0 = 1 << intNum;
        }
        else {
            intNum %= 32;        // normalize to 0 - 31
            Hwi_aIntc.IRQ1 = 1 << intNum;
        }
    }
    else {
        if (intNum < 32) {
            Hwi_aIntc.FIQ0 = 1 << intNum;
        }
        else {
            intNum %= 32;        // normalize to 0 - 31
            Hwi_aIntc.FIQ1 = 1 << intNum;
        }
    }
}

/*
 *  ======== switchFromBootStack ========
 *  Indicate that we are leaving the boot stack and
 *  are about to switch to a task stack.
 */
Void Hwi_switchFromBootStack()
{
    HwiCommon_switchFromBootStack();
    Hwi_module->taskSP = 0;
}

/*
 *  ======== Hwi_getStackInfo ========
 *  Used to get Hwi stack usage info.
 */
Bool Hwi_getStackInfo(Hwi_StackInfo *stkInfo, Bool computeStackDepth)
{
    Char *isrSP;
    Bool stackOverflow;

    /* Copy the stack base address and size */
    stkInfo->hwiStackSize = (SizeT)Hwi_module->isrStackSize;
    stkInfo->hwiStackBase = Hwi_module->isrStackBase;

    isrSP = stkInfo->hwiStackBase;

    /* Check for stack overflow */
    stackOverflow = (*isrSP != (Char)0xbe ? TRUE : FALSE);

    if (computeStackDepth && !stackOverflow) {
        /* Compute stack depth */
        do {
        } while(*isrSP++ == (Char)0xbe);
        stkInfo->hwiStackPeak = stkInfo->hwiStackSize - (SizeT)(--isrSP - (Char *)stkInfo->hwiStackBase);
    }
    else {
        stkInfo->hwiStackPeak = 0;
    }

    return stackOverflow;
}

/*
 *  ======== Hwi_setPriority ========
 *  Set an interrupt's relative priority.
 *
 *  @param(intNum)      ID of interrupt
 *  @param(priority)    priority
 */
Void Hwi_setPriority(UInt intNum, UInt priority)
{
    UInt intpr, intprMask;

    intpr = Hwi_aIntc.INTPR[intNum/8];
    intprMask = 0xF << (intNum % 8)*4;
    intprMask = ~intprMask;
    Hwi_aIntc.INTPR[intNum/8] = (intpr & intprMask) | (priority << (intNum % 8)*4);
}

/*
 *  ======== Hwi_reconfig ========
 *  Reconfigure a dispatched interrupt.
 */
Void Hwi_reconfig(Hwi_Object *hwi, Hwi_FuncPtr fxn, const Hwi_Params *params)
{
    UInt intNum;

    for (intNum = 0; intNum < Hwi_NUM_INTERRUPTS; intNum++) {
        if (Hwi_module->dispatchTable[intNum] == hwi) {
            break;
        }
    }

    Hwi_disableInterrupt(intNum);

    hwi->fxn = fxn;
    hwi->arg = params->arg;

    /* 
     * the -1 sentinel priority is the default passed by hal Hwi_create().
     * Translate it to 7, which is our default priority.
     */

    if (params->priority == -1) {
        hwi->priority = 7;
    }
    else {
        hwi->priority = params->priority;
    }

    Hwi_setPriority(intNum, hwi->priority);

    switch (params->maskSetting) {
        case Hwi_MaskingOption_NONE:
            hwi->disableMask0 = 0;
            hwi->disableMask1 = 0;
            hwi->restoreMask0 = 0;
            hwi->restoreMask1 = 0;
            break;
        case Hwi_MaskingOption_ALL:
            hwi->disableMask0 = 0xffffffff;
            hwi->disableMask1 = 0xffffffff;
            hwi->restoreMask0 = 0xffffffff;
            hwi->restoreMask1 = 0xffffffff;
            break;
        default:
        case Hwi_MaskingOption_SELF:
            if (intNum < 32) {
                hwi->disableMask0 = 1 << intNum;
                hwi->disableMask1 = 0;
                hwi->restoreMask0 = 1 << intNum;
                hwi->restoreMask1 = 0;
            }
            else {
                hwi->disableMask0 = 0;
                hwi->disableMask1 = 1 << intNum%32;
                hwi->restoreMask0 = 0;
                hwi->restoreMask1 = 1 << intNum%32;
            }
            break;
        case Hwi_MaskingOption_BITMASK:
            hwi->disableMask0 = params->disableMask0;
            hwi->disableMask1 = params->disableMask1;
            hwi->restoreMask0 = params->restoreMask0;
            hwi->restoreMask1 = params->restoreMask1;
            break;
    }

    if (params->enableInt) {
        Hwi_enableInterrupt(intNum);
    }
}

/*
 *  ======== Hwi_getFunc ========
 */
Hwi_FuncPtr Hwi_getFunc(Hwi_Object *hwi, UArg *arg)
{
    *arg = hwi->arg;

    return (hwi->fxn);
}

/*
 *  ======== setFunc ========
 */
Void Hwi_setFunc(Hwi_Object *hwi, Void (*fxn)(), UArg arg)
{
    hwi->fxn = fxn;
    hwi->arg = arg;
}

/*
 *  ======== Hwi_getIrp ========
 */
Hwi_Irp Hwi_getIrp(Hwi_Object *hwi)
{
    return (hwi->irp);
}

/*
 *  ======== Hwi_getHookContext ========
 *  Get hook instance's context for a hwi.
 *
 *  @param(id)            hook instance's ID
 *  @a(hookContextPtr)    hook instance's context for task
 */
Ptr Hwi_getHookContext(Hwi_Object *hwi, Int id)
{
    return (hwi->hookEnv[id]);
}

/*
 *  ======== Hwi_setHookContext ========
 *  Set hook instance's context for a hwi.
 *
 *  @param(id)            hook instance's ID
 *  @param(hookContext)   value to write to context
 */
Void Hwi_setHookContext(Hwi_Object *hwi, Int id, Ptr hookContext)
{
    hwi->hookEnv[id] = hookContext;
}

/*
 *  ======== Hwi_dispatchFIQC ========
 *  FIQ interrupt dispatcher
 */
#pragma CLINK (Hwi_dispatchFIQC)
Void interrupt Hwi_dispatchFIQC()
{
    Hwi_Object *entry;
    UInt intNum;

    /* determine current int num */
    switch (Hwi_eabaseSize) {
        case 4:
            intNum = ((Hwi_aIntc.FIQENTRY - (Hwi_aIntc.EABASE & 0x0ffffff8)) >> 2) - 1;
            break;      
        case 8:
            intNum = ((Hwi_aIntc.FIQENTRY - (Hwi_aIntc.EABASE & 0x0ffffff8)) >> 3) - 1;
            break;      
        case 16:
            intNum = ((Hwi_aIntc.FIQENTRY - (Hwi_aIntc.EABASE & 0x0ffffff8)) >> 4) - 1;
            break;      
        case 32:
            intNum = ((Hwi_aIntc.FIQENTRY - (Hwi_aIntc.EABASE & 0x0ffffff8)) >> 5) - 1;
            break;      
    }

    /* ack this interrupt */
    Hwi_clearInterrupt(intNum);

    entry = Hwi_module->dispatchTable[intNum];

    (entry->fxn)(entry->arg);
}

/*
 *  ======== Hwi_dispatchIRQC ========
 *  Configurable IRQ interrupt dispatcher.
 */
Void Hwi_dispatchIRQC(Hwi_Irp irp)
{
    /*
     * Enough room is reserved above the isr stack to handle
     * as many as 16 32-bit stack resident local variables.
     * This space is reserved for the Swi scheduler.
     *
     * If the swi scheduler requires more than this, you must
     * handle this in Hwi_Module_startup().
     */

    Hwi_Object *hwi;
    BIOS_ThreadType prevThreadType;
    UInt intNum;
    UInt32 oldEINT0, oldEINT1;
    Int swiKey;
    Int i;

    /* save irp for ROV call stack view */
    Hwi_module->irp = irp;

    /* determine current int num */
    switch (Hwi_eabaseSize) {
        case 4:
            intNum = ((Hwi_aIntc.IRQENTRY - (Hwi_aIntc.EABASE & 0x0ffffff8)) >> 2) - 1;
            break;      
        case 8:
            intNum = ((Hwi_aIntc.IRQENTRY - (Hwi_aIntc.EABASE & 0x0ffffff8)) >> 3) - 1;
            break;      
        case 16:
            intNum = ((Hwi_aIntc.IRQENTRY - (Hwi_aIntc.EABASE & 0x0ffffff8)) >> 4) - 1;
            break;      
        case 32:
            intNum = ((Hwi_aIntc.IRQENTRY - (Hwi_aIntc.EABASE & 0x0ffffff8)) >> 5) - 1;
            break;      
    }

    /* ack this interrupt */
    Hwi_clearInterrupt(intNum);

    hwi = Hwi_module->dispatchTable[intNum];

    hwi->irp = Hwi_module->irp;

    if (Hwi_dispatcherSwiSupport) {
        swiKey = SWI_DISABLE();
    }

    /* set thread type to Hwi */
    prevThreadType = BIOS_setThreadType(BIOS_ThreadType_Hwi);

#ifndef ti_sysbios_hal_Hwi_DISABLE_ALL_HOOKS
    /* call the begin hooks */
    for (i = 0; i < Hwi_hooks.length; i++) {
        if (Hwi_hooks.elem[i].beginFxn != NULL) {
            Hwi_hooks.elem[i].beginFxn((IHwi_Handle)hwi);
        }
    }
#endif

    Log_write5(Hwi_LM_begin, (IArg)hwi, (IArg)hwi->fxn, 
               (IArg)prevThreadType, (IArg)intNum, hwi->irp);

    /* call the user's isr */
    if (Hwi_dispatcherAutoNestingSupport) {
        oldEINT0 = Hwi_aIntc.EINT0;
        Hwi_aIntc.EINT0 &= ~hwi->disableMask0;
        oldEINT1 = Hwi_aIntc.EINT1;
        Hwi_aIntc.EINT1 &= ~hwi->disableMask1;
        Hwi_enable();

        (hwi->fxn)(hwi->arg);

        Hwi_disable();
        Hwi_aIntc.EINT0 |= (hwi->restoreMask0 & oldEINT0);
        Hwi_aIntc.EINT1 |= (hwi->restoreMask1 & oldEINT1);
    }
    else {
        (hwi->fxn)(hwi->arg);
    }

    Log_write1(Hwi_LD_end, (IArg)hwi);

#ifndef ti_sysbios_hal_Hwi_DISABLE_ALL_HOOKS
    /* call the end hooks */
    for (i = 0; i < Hwi_hooks.length; i++) {
        if (Hwi_hooks.elem[i].endFxn != NULL) {
            Hwi_hooks.elem[i].endFxn((IHwi_Handle)hwi);
        }
    }
#endif

    /* Run Swi scheduler */
    if (Hwi_dispatcherSwiSupport) {
        SWI_RESTORE(swiKey);
    }

    /* restore thread type */
    BIOS_setThreadType(prevThreadType);
}
