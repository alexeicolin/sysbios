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
#include <xdc/runtime/System.h>

#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Swi.h>
#include <ti/sysbios/interfaces/IHwi.h>
#include <ti/sysbios/BIOS.h>

#include "package/internal/Hwi.xdc.h"

extern Char *ti_sysbios_family_xxx_Hwi_switchToIsrStack();
extern Void ti_sysbios_family_xxx_Hwi_switchToTaskStack(Char *oldTaskSP);

#define Hwi_switchToIsrStack ti_sysbios_family_xxx_Hwi_switchToIsrStack
#define Hwi_switchToTaskStack ti_sysbios_family_xxx_Hwi_switchToTaskStack

#ifdef ti_sysbios_family_arm_gic_Hwi_dispatcherTaskSupport__D
#define TASK_DISABLE Task_disable
#define TASK_RESTORE Task_restoreHwi
#else
#define TASK_DISABLE Hwi_taskDisable
#define TASK_RESTORE Hwi_taskRestoreHwi
#endif

#ifdef ti_sysbios_family_arm_gic_Hwi_dispatcherSwiSupport__D
#define SWI_DISABLE Swi_disable
#define SWI_RESTORE Swi_restoreHwi
#else
#define SWI_DISABLE Hwi_swiDisable
#define SWI_RESTORE Hwi_swiRestoreHwi
#endif

#define NUM_GICD_ENABLE_REGS    32

/*
 *  ======== Hwi_Module_startup ========
 *  must initialize IRQ (and SWI?) SPs (R13s)
 */
Int Hwi_Module_startup (Int startupPhase)
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
    Hwi_module->isrStack =
        (Char *) (((UInt32) (Hwi_module->isrStackBase) & 0xfffffff8) +
                   (UInt32) (Hwi_module->isrStackSize) - (32 * sizeof(Int)));

    /*
     * Signal that we're executing on the ISR stack.
     */
    Hwi_module->taskSP = (Char *)-1;

    /*
     * Initialize the dispatchTable with default entries pointing
     * to nonPluggedHwi() handler.
     *
     * This will catch any triggered Interrupts that were not created
     * using the Hwi module.
     */
    for (i = 0; i < Hwi_NUM_INTERRUPTS; i++) {
        hwi = Hwi_module->dispatchTable[i];
        if (hwi !=  ti_sysbios_family_arm_gic_Hwi_Module_State_nonPluggedHwi()) {
            Hwi_postInit(hwi, NULL);
        }
    }
    return (Startup_DONE);
}

/*
 *  ======== Hwi_Instance_init  ========
 */
Int Hwi_Instance_init(Hwi_Object *hwi, Int intNum, Hwi_FuncPtr fxn, const Hwi_Params *params, Error_Block *eb)
{
    Int status;

    if (intNum >= Hwi_NUM_INTERRUPTS) {
        Error_raise(eb, Hwi_E_badIntNum, intNum, 0);
        return (1);
    }

    if (Hwi_module->dispatchTable[intNum] !=
        ti_sysbios_family_arm_gic_Hwi_Module_State_nonPluggedHwi()) {
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
 */
Void Hwi_Instance_finalize(Hwi_Object *hwi, Int status)
{
#ifndef ti_sysbios_hal_Hwi_DISABLE_ALL_HOOKS
    Int i, cnt;
#endif
    UInt intNum;

    for (intNum = 0; intNum < Hwi_NUM_INTERRUPTS; intNum++) {
        if (Hwi_module->dispatchTable[intNum] == hwi) {
            break;
        }
    }

    if (intNum == Hwi_NUM_INTERRUPTS) {
        Error_raise(NULL, Hwi_E_handleNotFound, hwi, 0);
        return;
    }

    Hwi_disableInterrupt(intNum);
    Hwi_module->dispatchTable[intNum] =
        ti_sysbios_family_arm_gic_Hwi_Module_State_nonPluggedHwi();

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
    UInt32 i, j;
    UInt32 intActiveReg;

    /*
     * Disable IRQs
     */
    Hwi_disable();

    /*
     * Disable forwarding of interrupts in GIC Distributer and CPU interface
     * Controller.
     */
    Hwi_gicd.CTLR = 0x0;
    Hwi_gicc.CTLR = 0x0;

    /*
     * Disable all interrupts at startup
     */
    for (i = 0; i < NUM_GICD_ENABLE_REGS; i++) {

        /*
         * Disable the forwarding of the corresponding interrupt from GIC
         * distributor to CPU interface.
         */
        Hwi_gicd.ICENABLER[i] = 0xFFFFFFFF;

        /* Track interrupts that cannot be disabled */
        Hwi_module->iser[i] |= Hwi_gicd.ISENABLER[i];
    }

    /*
     * Enable forwarding of interrupts in GIC Distributor and CPU interface
     * Controller.
     */
    Hwi_gicd.CTLR = 0x1;
    Hwi_gicc.CTLR = 0x1;

    /* Search for any previously active interrupts and acknowledge them */
    for (i = 0; i < NUM_GICD_ENABLE_REGS; i++) {
        intActiveReg = Hwi_gicd.ISACTIVER[i];
        if (intActiveReg) {
            for (j = 0; j < 32; j++) {
                if (intActiveReg & 0x1) {
                    Hwi_gicc.EOIR = i*32 + j;
                }
                intActiveReg = intActiveReg >> 1;
            }
        }
    }

    /*
     * Clear any currently pending enabled interrupts
     *
     * Note: SGIs are always enabled
     */
    for (i = 0; i < 4; i++) {
        Hwi_gicd.CPENDSGIR[i] = 0x01010101;
    }

    for (i = 0; i < NUM_GICD_ENABLE_REGS; i++) {
        Hwi_gicd.ICPENDR[i] = 0xFFFFFFFF;
    }

    /*
     * Clear all interrupt active status registers
     */
    for (i = 0; i < NUM_GICD_ENABLE_REGS; i++) {
        Hwi_gicd.ICACTIVER[i] = 0xFFFFFFFF;
    }


    /*
     * Enable any statically config'd interrupts that are enabled.
     */
    for (i = 0; i < NUM_GICD_ENABLE_REGS; i++) {
        Hwi_gicd.ISENABLER[i] = Hwi_module->iser[i];
    }

    /*
     * Set MPU0 as the target processor for all interrupts.
     *
     * ITARGETSR[0:7] are Read-Only
     */
    for (i = 8; i < (Hwi_NUM_INTERRUPTS / 4); i++) {
        Hwi_gicd.ITARGETSR[i] = Hwi_module->itargetsr[i];
    }

    /*
     * Initialize Binary Point Register
     */
    Hwi_gicc.BPR = Hwi_BPR;

    /*
     * Set configured interrupt Sense and Priority for each
     * interrupt
     */
    for (i=0; i < Hwi_NUM_INTERRUPTS; i++) {
            Hwi_setPriority(i, Hwi_module->dispatchTable[i]->priority);
    }

    /*
     * Initialize Interrupt Priority Mask Register
     *
     * Initialize PMR with the minimum possible interrupt priority.
     */
    Hwi_gicc.PMR = Hwi_MIN_INT_PRIORITY;

    /*
     * Set trigger sensitivity of interrupts
     *
     * On the Cortex-A15:
     *  -   For SGI: The ICFGR bits are read-only and a bit-pair always
     *               reads as b10 because SGIs are edge-triggered.
     *  -   For PPI: The ICFGR bits are read-only and a bit-pair always
     *               reads as b01 because PPIs are level-sensitive.
     *  -   For SPI: The LSB of the bit-pair is read only and is always
     *               1. MSB can be altered to change trigger sensitivity.
     *               b01    Interrupt is active-High level-sensitive
     *               b11    Interrupt is rising edge-sensitive
     */
    for (i = 2; i < (Hwi_NUM_INTERRUPTS / 16); i++) {
        Hwi_gicd.ICFGR[i] = Hwi_module->icfgr[i];
    }
}

/*
 *  ======== Hwi_startup ========
 */
Void Hwi_startup()
{
    Hwi_enable();
}

/*
 *  ======== ti_sysbios_family_xxx_Hwi_switchToIsrStack ========
 *  Char *ti_sysbios_family_xxx_Hwi_switchToIsrStack();
 *
 *  If not already on the isr stack, switch to it.
 *
 *  Checks the Hwi module object's taskSP.
 *  If 0, then set taskSP to current SP then switch to ISR stack.
 *
 *  Returns:
 *        old taskSP value for use with switchToTaskStack()
 */
#if defined(__GNUC__) && !defined(__ti__)
Char * __attribute__((naked)) ti_sysbios_family_xxx_Hwi_switchToIsrStack()
{
    __asm__ __volatile__ (
            "ldr r1, hwiModState\n\t"
            "ldr r0, [r1]\n\t"                /* Old taskSP */
            "cmp r0, #0\n\t"                  /* On ISR stack already ? */
            "bxne lr\n\t"                     /* Return if yes */
            "str r13, [r1]\n\t"               /* save SP into taskSP */
            "ldr r13, [r1, #4]\n\t"           /* switch to isr Stack */
            "bx lr\n\t"
            "hwiModState: .word ti_sysbios_family_arm_gic_Hwi_Module__state__V"
                 );
}
#endif

/*
 *  ======== ti_sysbios_family_xxx_Hwi_switchToTaskStack ========
 *  ti_sysbios_family_xxx_Hwi_switchToTaskStack(Char *oldTaskSP);
 *
 *  If at bottom of ISR stack, switch to Task stack.
 *
 *  If oldTaskSP is zero, then this is the thread that switched
 *  to the ISR stack first and therefore it is safe to switch
 *  back to the task stack address saved in the Hwi module
 *  object's taskSP variable.
 */
#if defined(__GNUC__) && !defined(__ti__)
Void __attribute__ ((naked)) ti_sysbios_family_xxx_Hwi_switchToTaskStack(Char *oldTaskSP)
{
    __asm__ __volatile__ (
            "cmp r0, #0\n\t"              /* Lowest order ISR? */
            "bxne lr\n\t"                 /* Return if not */
            "ldr r1, taskSP\n\t"          /* if yes, restore */
            "ldr r13, [r1]\n\t"           /* Interrupted task's SP */
            "str r0, [r1]\n\t"            /* Signal return to TaskStack */
            "bx lr\n\t"
            "taskSP: .word ti_sysbios_family_arm_gic_Hwi_Module__state__V"
                 );
}
#endif

/*
 *  ======== Hwi_getHandle ========
 */
Hwi_Handle Hwi_getHandle(UInt intNum)
{
    return (Hwi_module->dispatchTable[intNum]);
}

/*
 *  ======== Hwi_post ========
 */
Void Hwi_post(UInt intNum)
{
    if (intNum >= Hwi_NUM_INTERRUPTS) {
        Error_raise(0, Hwi_E_badIntNum, intNum, 0);
    }

    if (intNum < 16) {
        /*
         * SGIR Register format:
         *
         * Bits  [25:24]        [23:16]     [15]      [3:0]
         *   TargetListFilter CPUTargetList NSATT SGIInterruptId
         *
         * Selected Values for each field:
         * TargetListFilter: (0x2) Forward interrupt only to CPU
         *                   that posted the interrupt.
         * CPUTargetList   : Has No effect when TargetListFilter
         *                   set to 0x2.
         * NSATT           : Writable only from secure mode.
         * SGIInterruptId  : (intNum & 0xF) SGI Interrupt Number.
         */
        Hwi_gicd.SGIR =  (0x2 << 24) | (intNum & 0xf);
    }
    else {
        Hwi_gicd.ISPENDR[intNum/32] = 1 << (intNum & 0x1f);
    }

    /*
     * Add delay to insure posted interrupt are triggered before function
     * returns.
     */
    __asm__ __volatile__ (
            "dsb \n\t"
            "isb"
        );
}

/*
 *  ======== Hwi_getTaskSP ========
 */
Char *Hwi_getTaskSP()
{
    return (Hwi_module->taskSP);
}

/*
 *  ======== Hwi_disableInterrupt ========
 */
UInt Hwi_disableInterrupt(UInt intNum)
{
    UInt key, oldEnableState, index, mask;

    key = Hwi_disable();
    index = intNum / 32;
    mask = 1 << (intNum & 0x1f);
    oldEnableState = Hwi_gicd.ISENABLER[index] & mask;
    Hwi_gicd.ICENABLER[index] = mask;
    Hwi_restore(key);

    return oldEnableState;
}

/*
 *  ======== Hwi_enableInterrupt ========
 */
UInt Hwi_enableInterrupt(UInt intNum)
{
    UInt key, oldEnableState, index, mask;

    key = Hwi_disable();
    index = intNum / 32;
    mask = 1 << (intNum & 0x1f);
    oldEnableState = Hwi_gicd.ISENABLER[index] & mask;
    Hwi_gicd.ISENABLER[index] = mask;
    Hwi_restore(key);

    return oldEnableState;
}

/*
 *  ======== Hwi_RestoreInterrupt ========
 */
Void Hwi_restoreInterrupt(UInt intNum, UInt key)
{
    if (key) {
        Hwi_enableInterrupt(intNum);
    }
    else {
        Hwi_disableInterrupt(intNum);
    }
}

/*
 *  ======== Hwi_clearInterrupt ========
 *  Clear a specific interrupt's pending status.
 */
Void Hwi_clearInterrupt(UInt intNum)
{
    /*
     * For level-triggered SPIs and PPIs, interrupt remains pending
     * if interrupt signal remains asserted.
     */
    UInt index, shift, mask;

    if (intNum < 16) {
        index = intNum / 4;         /* Offset of required GICD_CPENSGIR */
        shift = (intNum & 0x3) << 3;
        mask = 1 << shift;          /* SGI Clear-pending field offset */
        Hwi_gicd.CPENDSGIR[index] = mask;
    }
    else {
        index = intNum / 32;
        mask = 1 << (intNum & 0x1F);
        Hwi_gicd.ICPENDR[index] = mask;
    }
}

/*
 *  ======== switchFromBootStack ========
 *  Indicate that we are leaving the boot stack and
 *  are about to switch to a task stack.
 */
Void Hwi_switchFromBootStack()
{
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
        stkInfo->hwiStackPeak = stkInfo->hwiStackSize -
                    (SizeT)(--isrSP - (Char *)stkInfo->hwiStackBase);
    }
    else {
        stkInfo->hwiStackPeak = 0;
    }

    return stackOverflow;
}

/*!
 *  ======== Hwi_setPriority ========
 *  Set an interrupt's relative priority.
 *
 *  @param(intNum)      ID of interrupt
 *  @param(priority)    priority
 */
Void Hwi_setPriority(UInt intNum, UInt priority)
{
    UInt key;
    UInt regIndex, oldPriorities, newPriorities, shift, mask;

    if (intNum < Hwi_NUM_INTERRUPTS) {
        regIndex = intNum >> 2;
        shift = (intNum & 0x3) << 3;
        mask = 0xFF << shift;
        key = Hwi_disable();
        oldPriorities = Hwi_gicd.IPRIORITYR[regIndex] & (~mask);
        newPriorities = oldPriorities | ((priority & 0xFF) << shift);
        Hwi_gicd.IPRIORITYR[regIndex] = newPriorities;
        Hwi_restore(key);
    }
}

/*
 *  ======== Hwi_reconfig ========
 *  Reconfigure a dispatched interrupt.
 */
Void Hwi_reconfig(Hwi_Object *hwi, Hwi_FuncPtr fxn, const Hwi_Params *params)
{
    UInt intNum;
    UInt32 shift, mask;

    for (intNum = 0; intNum < Hwi_NUM_INTERRUPTS; intNum++) {
        if (Hwi_module->dispatchTable[intNum] == hwi) {
            break;
        }
    }

    Hwi_disableInterrupt(intNum);

    hwi->fxn = fxn;
    hwi->arg = params->arg;
    hwi->type = params->type;
    hwi->priority = params->priority;
    hwi->triggerSensitivity = params->triggerSensitivity;
    hwi->targetProcList = params->targetProcList;

    /* Set trigger sensitivity */
    shift = (intNum & 0xF) << 1;
    mask  = 0x3 << shift;
    Hwi_gicd.ICFGR[intNum >> 4] = (Hwi_gicd.ICFGR[intNum >> 4] & (~mask)) |
        ((hwi->triggerSensitivity & 0x3) << shift);

    /* Set target processors */
    shift = (intNum & 0x3) << 3;
    mask  = 0xF << shift;
    Hwi_gicd.ITARGETSR[intNum >> 2] = (Hwi_gicd.ITARGETSR[intNum >> 2] & (~mask)) |
        ((hwi->targetProcList & 0xF) << shift);

    /*
     * the -1 sentinel priority is the default passed by hal Hwi_create().
     * Translate it to Hwi_DEFAULT_INT_PRIORITY, which is our default priority.
     */
    if (hwi->priority == ~0) {
        hwi->priority = Hwi_DEFAULT_INT_PRIORITY;
    }

    Hwi_setPriority(intNum, hwi->priority);

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
 *  ======== Hwi_nonPluggedHwiHandler ========
 */
Void Hwi_nonPluggedHwiHandler(UArg arg)
{
    /* Force MAIN threadtype So we can safely call Error_raise */

    BIOS_setThreadType(BIOS_ThreadType_Main);

    //Error_raise(0, Hwi_E_undefined, Hwi_intc.SIR_IRQ, 0);
}

/*
 *  ======== Hwi_dispatchIRQC ========
 *  Configurable IRQ interrupt dispatcher.
 */
Void Hwi_dispatchC(Hwi_Irp irp)
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
#ifndef ti_sysbios_hal_Hwi_DISABLE_ALL_HOOKS
    Int i;
#endif

    /* Acknowledge Interrupt */
    Hwi_module->curIntId = Hwi_gicc.IAR;

    /* Ignore spurious ints */
    if ((Hwi_module->curIntId == 1023) || (Hwi_module->curIntId == 1022)) {
        Hwi_module->spuriousInts++;
        Hwi_module->lastSpuriousInt = Hwi_module->curIntId;
        return;
    }

    /* save irp for ROV call stack view */
    Hwi_module->irp = irp;

    intNum = Hwi_module->curIntId;

    if (Hwi_dispatcherSwiSupport) {
        swiKey = SWI_DISABLE();
    }

    /* set thread type to Hwi */
    prevThreadType = BIOS_setThreadType(BIOS_ThreadType_Hwi);

    /* Process only this pending interrupt */
    hwi = Hwi_module->dispatchTable[Hwi_module->curIntId];

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
               (IArg)prevThreadType, (IArg)Hwi_module->curIntId, hwi->irp);

    if (Hwi_dispatcherAutoNestingSupport) {
        /*
         * IRQ Handler is disabled on exception entry
         * in secure state
         */
            Hwi_enable();
        // TODO should asynchronous abort be enabled ?
    }

    /* call user's ISR func */
    (hwi->fxn)(hwi->arg);

    Hwi_disable();

    /* Signal End of Interrupt */
    Hwi_gicc.EOIR = intNum;

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
