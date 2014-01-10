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

#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Swi.h>
#include <ti/sysbios/interfaces/IHwi.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/family/arm/HwiCommon.h>

#include "package/internal/Hwi.xdc.h"

extern Void ti_sysbios_family_arm_sim1030_Hwi_init(Void);
extern Char *ti_sysbios_family_xxx_Hwi_switchToIsrStack();
extern Void ti_sysbios_family_xxx_Hwi_switchToTaskStack(Char *oldTaskSP);

#define Hwi_switchToIsrStack ti_sysbios_family_xxx_Hwi_switchToIsrStack
#define Hwi_switchToTaskStack ti_sysbios_family_xxx_Hwi_switchToTaskStack
#define Hwi_init ti_sysbios_family_arm_sim1030_Hwi_init

static Int postInit(Hwi_Object *hwi, Error_Block *eb);

volatile Hwi_L1Intc Hwi_l1Intc;
volatile Hwi_L2Intc Hwi_l2Intc;

#define L1_GLOBAL_MASK 0x4
#define L1_NEW_FIQ_AGR 0x2
#define L1_NEW_IRQ_AGR 0x1

#define L2_GLOBAL_MASK 0x4
#define L2_NEW_FIQ_AGR 0x2
#define L2_NEW_IRQ_AGR 0x1

#ifdef ti_sysbios_family_arm_sim1030_Hwi_dispatcherTaskSupport__D
/* disable unused local variable warning during optimized compile */
#pragma diag_suppress=179
#define TASK_DISABLE Task_disable
#define TASK_RESTORE Task_restoreHwi
#else
#define TASK_DISABLE Hwi_taskDisable
#define TASK_RESTORE Hwi_taskRestoreHwi
#endif

#ifdef ti_sysbios_family_arm_sim1030_Hwi_dispatcherSwiSupport__D
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
 */
Int Hwi_Module_startup (Int phase)
{
    int i;
    Hwi_Object *hwi;

    /* must wait for these modules to initialize first */
    if (!Startup_rtsDone()) {
        return Startup_NOTDONE;
    }

    /* okay to proceed with initialization */

#ifndef ti_sysbios_hal_Hwi_DISABLE_ALL_HOOKS
    for (i = 0; i < Hwi_hooks.length; i++) {
        if (Hwi_hooks.elem[i].registerFxn != NULL) {
            Hwi_hooks.elem[i].registerFxn(i);
        }
    }
#endif
    Hwi_init();                 // sets up FIQ/IRQ stackpointers, etc

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

    Hwi_module->taskSP = (Char *)-1;/* signal that we're executing on the */
                                        /* ISR stack */

    Hwi_enableIER(Hwi_module->ierMask); /* IER per static Hwi settings */

    Hwi_l1Intc.ITR = 0;
    Hwi_l2Intc.ITR1 = 0;
    Hwi_l2Intc.ITR2 = 0;

    for (i = 0; i < Hwi_NUM_INTERRUPTS; i++) {
        hwi = Hwi_module->dispatchTable[i];
        if (hwi !=  NULL) {
            postInit(hwi, NULL);
        }
    }
    return (Startup_DONE);
}

/*
 *  ======== Hwi_Instance_init ========
 */
Int Hwi_Instance_init(Hwi_Object *hwi, Int intNum, Hwi_FuncPtr fxn, const Hwi_Params *params, Error_Block *eb)
{
    Int status;

    if (Hwi_module->dispatchTable[intNum] != NULL) {
        Error_raise(eb, Hwi_E_alreadyDefined, intNum, 0);
        return (1);
    }

    Hwi_module->dispatchTable[intNum] = hwi;
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

    status = postInit(hwi, eb);

    if (Error_check(eb)) {
        return (2 + status);
    }

    return (0);
}

/*
 *  ======== postInit ========
 *  Function to be called during module startup to complete the
 *  initialization of any statically created or constructed Hwi.
 *  returns (0) and clean 'eb' on success
 *  returns 'eb' *and* 'n' for number of successful createFxn() calls iff
 *      one of the createFxn() calls fails
 */
static Int postInit (Hwi_Object *hwi, Error_Block *eb)
{
    Int i;

#ifndef ti_sysbios_hal_Hwi_DISABLE_ALL_HOOKS
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
#pragma CODE_SECTION(ti_sysbios_family_arm_sim1030_Hwi_disable__E, ".text:ti_sysbios_family_arm_sim1030_Hwi_disable__E")
#pragma CODE_SECTION(ti_sysbios_family_arm_sim1030_Hwi_enable__E, ".text:ti_sysbios_family_arm_sim1030_Hwi_enable__E")
#pragma CODE_SECTION(ti_sysbios_family_arm_sim1030_Hwi_restore__E, ".text:ti_sysbios_family_arm_sim1030_Hwi_restore__E")
#pragma FUNC_EXT_CALLED(ti_sysbios_family_arm_sim1030_Hwi_disable__E);
#pragma FUNC_EXT_CALLED(ti_sysbios_family_arm_sim1030_Hwi_enable__E);
#pragma FUNC_EXT_CALLED(ti_sysbios_family_arm_sim1030_Hwi_restore__E);
#endif

/*
 *  ======== Hwi_disable ========
 *  disables only IRQ interrupts
 */

UInt ti_sysbios_family_arm_sim1030_Hwi_disable__E()
{
    return HwiCommon_disable();
}

/*
 *  ======== Hwi_enable ========
 *  enables only IRQ interrupts
 */
UInt ti_sysbios_family_arm_sim1030_Hwi_enable__E()
{
    return HwiCommon_enable();
}

/*
 *  ======== Hwi_restore ========
 *  restores only IRQ interrupts
 */
Void ti_sysbios_family_arm_sim1030_Hwi_restore__E(UInt key)
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
 *  ======== Hwi_disableIER ========
 *  Disable certain maskable interrupts.
 *
 *  Disables specific interrupts by clearing the bits specified by
 *  mask in the Interrupt Enable Register (IER).
 *
 *  @param(mask)    bitmask of interrupts to disable
 *  @a(returns)     previous IER settings bitmask
 */
Bits16 Hwi_disableIER(Bits16 mask)
{
    UInt key;
    Bits16 oldIER;

    key = Hwi_disable();
    oldIER = Hwi_module->IER;
    Hwi_module->IER &= ~mask;
    Hwi_restore(key);

    return oldIER;
}

/*
 *  ======== Hwi_enableIER ========
 *  Enable certain maskable interrupts.
 *
 *  Enables specific interrupts by setting the bits specified by
 *  mask in the Interrupt Enable Register (IER).
 *
 *  @param(mask)    bitmask of interrupts to enable
 *  @a(returns)     previous IER settings bitmask
 */
Bits16 Hwi_enableIER(Bits16 mask)
{
    UInt key;
    Bits16 oldIER;

    key = Hwi_disable();
    oldIER = Hwi_module->IER;
    Hwi_module->IER |= mask;
    Hwi_restore(key);

    return oldIER;
}

/*
 *  ======== Hwi_restoreIER ========
 *  Restore maskable interrupts to the state they were in
 *  when either disableIER() or enableIER() was called.
 *
 *  Simply writes mask to IER.
 *
 *  @param(mask)    bitmask of interrupts to restore
 *  @a(returns)     previous IER settings bitmask
 */
Bits16 Hwi_restoreIER(Bits16 mask)
{
    UInt key;
    Bits16 oldIER;

    key = Hwi_disable();
    oldIER = Hwi_module->IER;
    Hwi_module->IER = mask;
    Hwi_restore(key);

    return oldIER;
}

/*
 *  ======== Hwi_disableInterrupt ========
 */
UInt Hwi_disableInterrupt(UInt intNum)
{
    return (Hwi_disableIER(1 << intNum)) ;
}

/*
 *  ======== Hwi_enableInterrupt ========
 */
UInt Hwi_enableInterrupt(UInt intNum)
{
    return (Hwi_enableIER(1 << intNum));
}

/*
 *  ======== Hwi_RestoreInterrupt ========
 */
Void Hwi_restoreInterrupt(UInt intNum, UInt key)
{
    if (key & (1 << (intNum))) {
        Hwi_enableInterrupt(intNum);
    }
    else {
        Hwi_disableInterrupt(intNum);
    }
}

/*
 *  ======== Hwi_clearInterrupt ========
 */
Void Hwi_clearInterrupt(UInt intNum)
{
}

/*
 *  ======== Hwi_getHandle ========
 */
Hwi_Handle Hwi_getHandle(UInt intNum)
{
    return (Hwi_module->dispatchTable[intNum]);
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

extern Void ti_sysbios_family_arm_sim1030_Hwi_IRQ(Void);

/*
 *  ======== Hwi_post ========
 */
Void Hwi_post(UInt intNum)
{
    Hwi_l1Intc.SIR_IRQ = intNum;
    ti_sysbios_family_arm_sim1030_Hwi_IRQ();
}

/*
 *  ======== Hwi_getTaskSP ========
 */
Char *Hwi_getTaskSP()
{
    return (HwiCommon_getTaskSP());
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

    switch (params->maskSetting) {
        case Hwi_MaskingOption_NONE:
            hwi->disableMask = 0;
            hwi->restoreMask = 0;
            break;
        case Hwi_MaskingOption_ALL:
            hwi->disableMask = 0xffff;
            hwi->restoreMask = 0xffff;
            break;
        default:
        case Hwi_MaskingOption_SELF:
            hwi->disableMask = 1 << intNum;
            hwi->restoreMask = 1 << intNum;
            break;
        case Hwi_MaskingOption_BITMASK:
            hwi->disableMask = params->disableMask;
            hwi->restoreMask = params->restoreMask;
            break;
    }

    if (params->enableInt) {
        Hwi_enableInterrupt(intNum);
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
 *  ======== Hwi_dispatchFIQC ========
 *  FIQ interrupt dispatcher
 */
#pragma CLINK (ti_sysbios_family_arm_sim1030_Hwi_dispatchFIQ)
interrupt Void ti_sysbios_family_arm_sim1030_Hwi_dispatchFIQ()
{
    Hwi_Object *hwi;

    hwi = Hwi_module->dispatchTable[Hwi_l1Intc.SIR_FIQ];

    (hwi->fxn)(hwi->arg);
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
    Int swiKey;
    Int i;

    /* save irp for ROV call stack view */
    Hwi_module->irp = irp;

    if (Hwi_dispatcherSwiSupport) {
        swiKey = SWI_DISABLE();
    }

    /* set thread type to Hwi */
    prevThreadType = BIOS_setThreadType(BIOS_ThreadType_Hwi);

    /* Porcess ALL pending and enabled interrupts */
    do {
        intNum = Hwi_l1Intc.SIR_IRQ;        /* get current L1 int num */

        if (intNum == 0) {                  /* is from L2? */
            intNum = Hwi_l2Intc.SIR_IRQ;    /* get current L2 int num */
            intNum += 32;                   /* force to linear index */
        }

        hwi = Hwi_module->dispatchTable[intNum];

        hwi->irp = Hwi_module->irp;

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

        (hwi->fxn)(hwi->arg);

        Hwi_disable();

        Log_write1(Hwi_LD_end, (IArg)hwi);

#ifndef ti_sysbios_hal_Hwi_DISABLE_ALL_HOOKS
        /* call the end hooks */
        for (i = 0; i < Hwi_hooks.length; i++) {
            if (Hwi_hooks.elem[i].endFxn != NULL) {
                Hwi_hooks.elem[i].endFxn((IHwi_Handle)hwi);
            }
        }
#endif

        if (intNum > 31) {                      /* is from L2? */
            Hwi_l2Intc.CONTROL = L2_NEW_IRQ_AGR;/* force NEW_IRQ_AGR */
        }
        Hwi_l1Intc.CONTROL = L1_NEW_IRQ_AGR;    /* force NEW_IRQ_AGR */
    } 
          /* loop thru all active and enabled IRQ ints */
    while (Hwi_l1Intc.ITR & ~Hwi_l1Intc.MIR & Hwi_module->irq0Mask);

    /* Run Swi scheduler */
    if (Hwi_dispatcherSwiSupport) {
        SWI_RESTORE(swiKey);
    }

    /* restore thread type */
    BIOS_setThreadType(prevThreadType);
}


