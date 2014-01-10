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
#include <ti/sysbios/family/arm/HwiCommon.h>

#include "package/internal/Hwi.xdc.h"

extern Void ti_sysbios_family_arm_omap1030_Hwi_init(Void);
extern Char *ti_sysbios_family_xxx_Hwi_switchToIsrStack();
extern Void ti_sysbios_family_xxx_Hwi_switchToTaskStack(Char *oldTaskSP);

#define Hwi_switchToIsrStack ti_sysbios_family_xxx_Hwi_switchToIsrStack
#define Hwi_switchToTaskStack ti_sysbios_family_xxx_Hwi_switchToTaskStack
#define Hwi_init ti_sysbios_family_arm_omap1030_Hwi_init

static Int postInit(Hwi_Object *hwi, Error_Block *eb);
static Void initIntControllers();

/*
 * the following bitmaps contain the default sense settings for
 * the L1 and L2 interrupts.
 */
static Char const l1DefaultSenseTable[4]  = {0xA7, 0xde, 0xfe, 0x93};

static Char const l2DefaultSenseTable[16] = {0xfd, 0xc1, 0xd3, 0xf5,
                                       0xfe, 0xf7, 0xfb, 0x7f,
                                       0x00, 0x00, 0x00, 0x38,
                                       0x00, 0x00, 0x00, 0x00};

#define L1_GLOBAL_MASK 0x4
#define L1_NEW_FIQ_AGR 0x2
#define L1_NEW_IRQ_AGR 0x1

#define L2_GLOBAL_MASK 0x4
#define L2_NEW_FIQ_AGR 0x2
#define L2_NEW_IRQ_AGR 0x1

#ifdef ti_sysbios_family_arm_omap1030_Hwi_dispatcherTaskSupport__D
/* disable unused local variable warning during optimized compile */
#pragma diag_suppress=179
#define TASK_DISABLE Task_disable
#define TASK_RESTORE Task_restoreHwi
#else
#define TASK_DISABLE Hwi_taskDisable
#define TASK_RESTORE Hwi_taskRestoreHwi
#endif

#ifdef ti_sysbios_family_arm_omap1030_Hwi_dispatcherSwiSupport__D
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

    initIntControllers();

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

    Hwi_restoreMIR0(Hwi_module->mir0Mask); /* MIR0 per static Hwi settings */
    Hwi_restoreMIR1(Hwi_module->mir1Mask); /* MIR1 per static Hwi settings */
    Hwi_restoreMIR2(Hwi_module->mir2Mask); /* MIR2 per static Hwi settings */

    for (i = 0; i < Hwi_NUM_INTERRUPTS; i++) {
        hwi = Hwi_module->dispatchTable[i];
        if (hwi !=  NULL) {
            postInit(hwi, NULL);
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

static Void initIntControllers()
{
    UInt i;

    Hwi_l2Intc.OCP_CFG |= 2;    /* reset L2 Interrupt Controller */

    /*
     * The L1 interrupt controller has no RESET feature and
     * none would be required after a hard reset or power up.
     * But the CCS reset/reload process doesn't do a hard reset.
     * The code below tries to "virtually" reset the L1 intc.
     */

    Hwi_l1Intc.MIR = 0xffffffff;/* mask all interrupts */
    Hwi_l1Intc.ITR = 0;         /* clear any pending interrupts */
    Hwi_l1Intc.CONTROL = 0x3;   /* latch any latent interrupts */
    Hwi_l1Intc.ITR = 0;         /* drown the ashes */
    Hwi_l1Intc.CONTROL = 0x3;   /* stir the ashes */
    Hwi_l1Intc.ITR = 0;         /* and drown them again */

    /*
     * Set configured interrupt Sense and Priority for each
     * L1 and L2 interrupt
     */
    for (i=0; i<96; i++) {
        if ((Hwi_module->dispatchTable[i] == NULL) ||
            (Hwi_module->dispatchTable[i]->sense == Hwi_Sense_TRM)) {
            if (i < 32 ) {
                Hwi_setSense(i, (l1DefaultSenseTable[i/8] &
                                (1 << (i%8))) ?
                                Hwi_Sense_LEVEL : Hwi_Sense_EDGE);
            }
            else {
                Hwi_setSense(i, (l2DefaultSenseTable[(i-32)/8] &
                                (1 << ((i-32)%8))) ?
                                Hwi_Sense_LEVEL : Hwi_Sense_EDGE);
            }
        }
        else {
            Hwi_setSense(i, Hwi_module->dispatchTable[i]->sense);
            Hwi_setPriority(i, Hwi_module->dispatchTable[i]->priority);
            Hwi_setType(i, Hwi_module->dispatchTable[i]->type);
        }
    }
    /* Route L2 FIQ to FIQ */
    Hwi_setType(2, Hwi_Type_FIQ);
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
#pragma CODE_SECTION(ti_sysbios_family_arm_omap1030_Hwi_disable__E, ".text:ti_sysbios_family_arm_omap1030_Hwi_disable__E")
#pragma CODE_SECTION(ti_sysbios_family_arm_omap1030_Hwi_enable__E, ".text:ti_sysbios_family_arm_omap1030_Hwi_enable__E")
#pragma CODE_SECTION(ti_sysbios_family_arm_omap1030_Hwi_restore__E, ".text:ti_sysbios_family_arm_omap1030_Hwi_restore__E")
#pragma FUNC_EXT_CALLED(ti_sysbios_family_arm_omap1030_Hwi_disable__E);
#pragma FUNC_EXT_CALLED(ti_sysbios_family_arm_omap1030_Hwi_enable__E);
#pragma FUNC_EXT_CALLED(ti_sysbios_family_arm_omap1030_Hwi_restore__E);
#endif

/*
 *  ======== Hwi_disable ========
 *  disables only IRQ interrupts
 */

UInt ti_sysbios_family_arm_omap1030_Hwi_disable__E()
{
    return HwiCommon_disable();
}

/*
 *  ======== Hwi_enable ========
 *  enables only IRQ interrupts
 */
UInt ti_sysbios_family_arm_omap1030_Hwi_enable__E()
{
    return HwiCommon_enable();
}

/*
 *  ======== Hwi_restore ========
 *  restores only IRQ interrupts
 */
Void ti_sysbios_family_arm_omap1030_Hwi_restore__E(UInt key)
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

/*!
 *  ======== Hwi_disableMIR0 ========
 */
Bits32 Hwi_disableMIR0(Bits32 mask)
{
    UInt key;
    Bits32 oldMIR0;

    key = Hwi_disable();
    oldMIR0 = Hwi_l1Intc.MIR;
    Hwi_l1Intc.MIR |= mask;
    Hwi_restore(key);

    return (oldMIR0);
}

/*!
 *  ======== Hwi_disableMIR1 ========
 */
Bits32 Hwi_disableMIR1(Bits32 mask)
{
    UInt key;
    Bits32 oldMIR1;

    key = Hwi_disable();
    oldMIR1 = Hwi_l2Intc.MIR1;
    Hwi_l2Intc.MIR1     |= mask;
    Hwi_restore(key);

    return (oldMIR1);
}

/*!
 *  ======== Hwi_disableMIR2 ========
 */
Bits32 Hwi_disableMIR2(Bits32 mask)
{
    UInt key;
    Bits32 oldMIR2;

    key = Hwi_disable();
    oldMIR2 = Hwi_l2Intc.MIR2;
    Hwi_l2Intc.MIR2     |= mask;
    Hwi_restore(key);

    return (oldMIR2);
}

/*!
 *  ======== Hwi_enableMIR0 ========
 */
Bits32 Hwi_enableMIR0(Bits32 mask)
{
    UInt key;
    Bits32 oldMIR0;

    key = Hwi_disable();
    oldMIR0 = Hwi_l1Intc.MIR;
    Hwi_l1Intc.MIR &= ~mask;
    Hwi_restore(key);

    return (oldMIR0);
}

/*!
 *  ======== Hwi_enableMIR1 ========
 */
Bits32 Hwi_enableMIR1(Bits32 mask)
{
    UInt key;
    Bits32 oldMIR1;

    key = Hwi_disable();
    oldMIR1 = Hwi_l2Intc.MIR1;
    Hwi_l2Intc.MIR1     &= ~mask;
    Hwi_restore(key);

    return (oldMIR1);
}

/*!
 *  ======== Hwi_enableMIR2 ========
 */
Bits32 Hwi_enableMIR2(Bits32 mask)
{
    UInt key;
    Bits32 oldMIR2;

    key = Hwi_disable();
    oldMIR2 = Hwi_l2Intc.MIR2;
    Hwi_l2Intc.MIR2     &= ~mask;
    Hwi_restore(key);

    return (oldMIR2);
}

/*!
 *  ======== Hwi_restoreMIR0 ========
 */
Bits32 Hwi_restoreMIR0(Bits32 mask)
{
    UInt key;
    Bits32 oldMIR0;

    key = Hwi_disable();
    oldMIR0 = Hwi_l1Intc.MIR;
    Hwi_l1Intc.MIR = mask;
    Hwi_restore(key);

    return (oldMIR0);
}

/*!
 *  ======== Hwi_restoreMIR1 ========
 */
Bits32 Hwi_restoreMIR1(Bits32 mask)
{
    UInt key;
    Bits32 oldMIR1;

    key = Hwi_disable();
    oldMIR1 = Hwi_l2Intc.MIR1;
    Hwi_l2Intc.MIR1 = mask;
    Hwi_restore(key);

    return (oldMIR1);
}

/*!
 *  ======== Hwi_restoreMIR2 ========
 */
Bits32 Hwi_restoreMIR2(Bits32 mask)
{
    UInt key;
    Bits32 oldMIR2;

    key = Hwi_disable();
    oldMIR2 = Hwi_l2Intc.MIR2;
    Hwi_l2Intc.MIR2 = mask;
    Hwi_restore(key);

    return (oldMIR2);
}

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
    if ( intNum < 32 ) {
        Hwi_l1Intc.SISR = (1 << intNum);
        Hwi_l1Intc.SISR = 0;
    }
    else if ( intNum < 64 ) {
        intNum %= 32;    // normalize to 0 - 31
        Hwi_l2Intc.SISR1 = (1 << intNum);
        Hwi_l2Intc.SISR1 = 0;
    }
    else if ( intNum < 96 ) {
        intNum %= 32;    // normalize to 0 - 31
        Hwi_l2Intc.SISR2 = (1 << intNum);
        Hwi_l2Intc.SISR2 = 0;
    }
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
        return (Hwi_disableMIR0 (1 << intNum)) ;
    }
    else if ( intNum < 64 ) {
        intNum %= 32;    // normalize to 0 - 31
        return (Hwi_disableMIR1 (1 << intNum));
    }
    else {
        intNum %= 32;    // normalize to 0 - 31
        return (Hwi_disableMIR2 (1 << intNum));
    }
}

/*
 *  ======== Hwi_enableInterrupt ========
 */
UInt Hwi_enableInterrupt(UInt intNum)
{
    if ( intNum < 32 ) {
        return (Hwi_enableMIR0 (1 << intNum));
    }
    else if ( intNum < 64 ) {
        intNum %= 32;    // normalize to 0 - 31
        return (Hwi_enableMIR1 (1 << intNum));
    }
    else {
        intNum %= 32;    // normalize to 0 - 31
        return (Hwi_enableMIR2 (1 << intNum));
    }
}

/*
 *  ======== Hwi_RestoreInterrupt ========
 */
Void Hwi_restoreInterrupt(UInt intNum, UInt key)
{
    if (key & (1 << (intNum%32))) {
        Hwi_disableInterrupt(intNum);
    }
    else {
        Hwi_enableInterrupt(intNum);
    }
}

/*
 *  ======== Hwi_clearInterrupt ========
 */
Void Hwi_clearInterrupt(UInt intNum)
{
    if ( intNum < 32 ) {
        Hwi_l1Intc.ITR &= ~(1 << intNum);
    }
    else if ( intNum < 64 ) {
        intNum %= 32;    // normalize to 0 - 31
        Hwi_l2Intc.ITR1 &= ~(1 << intNum);
    }
    else if ( intNum < 96 ) {
        intNum %= 32;    // normalize to 0 - 31
        Hwi_l2Intc.ITR2 &= ~(1 << intNum);
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

/*!
 *  ======== Hwi_setPriority ========
 *  Set an interrupt's relative priority.
 *
 *  @param(intNum)      ID of interrupt
 *  @param(priority)    priority
 */
Void Hwi_setPriority(UInt intNum, UInt priority)
{
    UInt ilr;

    if ( intNum < 32 ) {
        ilr = Hwi_l1Intc.ILR[intNum];
        Hwi_l1Intc.ILR[intNum] = (ilr & 0xffffff83) | priority << 2;
    }
    else if ( intNum < 64 ) {
        intNum %= 32;    // normalize to 0 - 31
        ilr = Hwi_l2Intc.ILR1[intNum];
        Hwi_l2Intc.ILR1[intNum] = (ilr & 0xffffff83) | priority << 2;
    }
    else if ( intNum < 96 ) {
        intNum %= 32;    // normalize to 0 - 31
        ilr = Hwi_l2Intc.ILR2[intNum];
        Hwi_l2Intc.ILR2[intNum] = (ilr & 0xffffff83) | priority << 2;
    }
}

/*!
 *  ======== Hwi_setSense ========
 *  Set an interrupt's sense (EDGE/LEVEL).
 *
 *  @param(intNum)      ID of interrupt
 *  @param(sense)       sense = Sense_EDGE/Sense_LEVEL
 */
Void Hwi_setSense(UInt intNum, Hwi_Sense sense)
{
    UInt ilr;

    if ( intNum < 32 ) {
        ilr = Hwi_l1Intc.ILR[intNum];
        Hwi_l1Intc.ILR[intNum] = (ilr & 0xfffffffd) | sense << 1;
    }
    else if ( intNum < 64 ) {
        intNum %= 32;    // normalize to 0 - 31
        ilr = Hwi_l2Intc.ILR1[intNum];
        Hwi_l2Intc.ILR1[intNum] = (ilr & 0xfffffffd) | sense << 1;
    }
    else if ( intNum < 96 ) {
        intNum %= 32;    // normalize to 0 - 31
        ilr = Hwi_l2Intc.ILR2[intNum];
        Hwi_l2Intc.ILR2[intNum] = (ilr & 0xfffffffd) | sense << 1;
    }
}

/*!
 *  ======== Hwi_setType ========
 *  Set an interrupt's type (FIQ/IRQ).
 *
 *  @param(intNum)      ID of interrupt
 *  @param(type)        type = FIQ/IRQ
 */
Void Hwi_setType(UInt intNum, Hwi_Type type)
{
    UInt ilr;

    if ( intNum < 32 ) {
        ilr = Hwi_l1Intc.ILR[intNum];
        Hwi_l1Intc.ILR[intNum] = (ilr & 0xfffffffe) | type;
        if (type == Hwi_Type_IRQ) {
            Hwi_module->irq0Mask |= (1 << intNum);
        }
        else {
            Hwi_module->irq0Mask &= ~(1 << intNum);
        }
    }
    else if ( intNum < 64 ) {
        intNum %= 32;    // normalize to 0 - 31
        ilr = Hwi_l2Intc.ILR1[intNum];
        Hwi_l2Intc.ILR1[intNum] = (ilr & 0xfffffffe) | type;
    }
    else if ( intNum < 96 ) {
        intNum %= 32;    // normalize to 0 - 31
        ilr = Hwi_l2Intc.ILR2[intNum];
        Hwi_l2Intc.ILR2[intNum] = (ilr & 0xfffffffe) | type;
    }
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
    hwi->sense = params->sense;
    hwi->type = params->type;
    hwi->priority = params->priority;

    Hwi_setSense(intNum, hwi->sense);
    Hwi_setType(intNum, hwi->type);
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
 *  ======== Hwi_dispatchFIQC ========
 *  FIQ interrupt dispatcher
 */
#pragma CLINK (Hwi_dispatchFIQC)
Void interrupt Hwi_dispatchFIQC()
{
    Hwi_Object *entry;
    UInt intNum;

    intNum = Hwi_l1Intc.SIR_FIQ;                /* get current L1 int num */

    if (intNum == 2) {                          /* is from L2 FIQ? */
        intNum = Hwi_l2Intc.SIR_FIQ;            /* get current L2 int num */
        intNum += 32;                           /* force to linear index */
    }

    entry = Hwi_module->dispatchTable[intNum];

    (entry->fxn)(entry->arg);

    if (intNum > 31) {                          /* was from L2? */
        Hwi_l2Intc.CONTROL = L2_NEW_FIQ_AGR;    /* force NEW_FIQ_AGR */
    }

    Hwi_l1Intc.CONTROL = L1_NEW_FIQ_AGR;        /* force NEW_FIQ_AGR */
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

        Hwi_enable();

        /* call user's ISR func */
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

    }   /* loop thru all active and enabled IRQ ints */
    while (Hwi_l1Intc.ITR & ~Hwi_l1Intc.MIR & Hwi_module->irq0Mask);

    /* Run Swi scheduler */
    if (Hwi_dispatcherSwiSupport) {
        SWI_RESTORE(swiKey);
    }

    /* restore thread type */
    BIOS_setThreadType(prevThreadType);
}
