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
 *  ======== Hwi.xdc ========
 *
 */
package ti.sysbios.family.arm.a8.intcps;

import xdc.rov.ViewInfo;

import xdc.runtime.Diags;
import xdc.runtime.Log;
import xdc.runtime.Error;

import ti.sysbios.BIOS;
import ti.sysbios.interfaces.IHwi;

/*!
 *  ======== Hwi ========
 *  Hardware Interrupt Support Module.
 *
 *  This Hwi module provides ARM family-specific implementations of the
 *  APIs defined in {@link ti.sysbios.interfaces.IHwi IHwi}.
 *
 *  Additional ARM device-specific APIs are also provided.
 *
 *  @a(NOTE)
 *  In this Hwi module implementation, the instance config parameter value
 *  {@link #MaskingOption_LOWER} is equivalent to {@link #MaskingOption_SELF}.
 *  Statically configuring a Hwi object's {@link #Params.maskSetting} to
 *  {@link #MaskingOption_LOWER} will result in the generation of a benign
 *  build warning. Dynamic usages of {@link #MaskingOption_LOWER} will be
 *  silently converted to {@link #MaskingOption_SELF}.
 *
 */

@Template("./Hwi.xdt")  /* generates the vector table and the dispatcher */
@ModuleStartup          /* generates call to Hwi_Module_startup at startup */

module Hwi inherits ti.sysbios.interfaces.IHwi
{

    // -------- Module Constants --------

    /*! intcps supports 128 interrupts. */
    const Int NUM_INTERRUPTS = 128;

    /*!
     *  The device-specific number of priorities supported.
     *
     *  The actual number supported is device specific and provided by
     *  the catalog device specification.
     */
    config Int NUM_PRIORITIES;

    // -------- Module Types --------

    /*! Hwi vector function type definition. */
    typedef Void (*VectorFuncPtr)(void);

    /*!
     *  ======== BasicView ========
     *  @_nodoc
     */
    metaonly struct BasicView {
        Ptr         halHwiHandle;
        String      label;
        Int         intNum;
        UInt        priority;
        String      fxn;
        UArg        arg;
    };

    /*!
     *  ======== DetailedView ========
     *  @_nodoc
     */
    metaonly struct DetailedView {
        Ptr         halHwiHandle;
        String      label;
        Int         intNum;
        UInt        priority;
        String      fxn;
        UArg        arg;
        Ptr         irp;
        Bool        enabled;
        Bool        pending;
    };

    /*!
     *  ======== ModuleView ========
     *  @_nodoc
     */
    metaonly struct ModuleView {
        String      options[4];
        UInt        spuriousInterrupts;
        UInt        lastSpuriousInterrupt;
        SizeT       hwiStackPeak;
        SizeT       hwiStackSize;
        Ptr         hwiStackBase;
    };

    /*!
     *  ======== rovViewInfo ========
     *  @_nodoc
     */
    @Facet
    metaonly config ViewInfo.Instance rovViewInfo =
        ViewInfo.create({
            viewMap: [
                ['Basic',    {type: ViewInfo.INSTANCE, viewInitFxn: 'viewInitBasic',    structName: 'BasicView'}],
                ['Detailed', {type: ViewInfo.INSTANCE, viewInitFxn: 'viewInitDetailed', structName: 'DetailedView'}],
                ['Module',   {type: ViewInfo.MODULE,   viewInitFxn: 'viewInitModule',   structName: 'ModuleView'}]
            ]
        });

    /*! Interrupt type. IRQ or FIQ */
    enum Type {
        Type_IRQ,               /*! IRQ interrupt. */
        Type_FIQ                /*! FIQ interrupt. */
    };

    /*! Interrupt Controller. Symbol "Hwi_intc" is physical device */
    struct Intc {
        UInt32 REVISION;        /*! 0x00 Config Register */

        UInt32 hole0[3];        /*! 0x04-0x0C */

        UInt32 SYSCONFIG;       /*! 0x10 Config Register */
        UInt32 SYSSTATUS;       /*! 0x14 Status Register */

        UInt32 hole1[10];       /*! 0x18-0x3c */

        UInt32 SIR_IRQ;         /*! 0x40 Source IRQ Register */
        UInt32 SIR_FIQ;         /*! 0x44 Source FIQ Register */
        UInt32 CONTROL;         /*! 0x48 Control Register */
        UInt32 PROTECTION;      /*! 0x4C Protection Register */
        UInt32 IDLE;            /*! 0x50 Idle Register */

        UInt32 hole2[3];        /*! 0x54-0x5C */

        UInt32 IRQ_PRIORITY;    /*! 0x60 IRQ Priority Register */
        UInt32 FIQ_PRIORITY;    /*! 0x64 IRQ Priority Register */
        UInt32 THRESHOLD;       /*! 0x68 Threshold Register */

        UInt32 hole3[5];        /*! 0x6C-0x7C */

        UInt32 ITR0;            /*! 0x80 Interrupt Input Register */
        UInt32 MIR0;            /*! 0x84 Mask Interrupt Register */
        UInt32 MIR_CLEAR0;      /*! 0x88 MIR Clear Register */
        UInt32 MIR_SET0;        /*! 0x8C MIR Set Register */
        UInt32 ISR_SET0;        /*! 0x90 Interrupt Set Register */
        UInt32 ISR_CLEAR0;      /*! 0x94 Interrupt Clear Register */
        UInt32 PENDING_IRQ0;    /*! 0x98 Pending IRQ Register */
        UInt32 PENDING_FIQ0;    /*! 0x9C Pending FIQ Register */

        UInt32 ITR1;            /*! 0xA0 Interrupt Input Register */
        UInt32 MIR1;            /*! 0xA4 Mask Interrupt Register */
        UInt32 MIR_CLEAR1;      /*! 0xA8 MIR Clear Register */
        UInt32 MIR_SET1;        /*! 0xAC MIR Set Register */
        UInt32 ISR_SET1;        /*! 0xB0 Interrupt Set Register */
        UInt32 ISR_CLEAR1;      /*! 0xB4 Interrupt Clear Register */
        UInt32 PENDING_IRQ1;    /*! 0xB8 Pending IRQ Register */
        UInt32 PENDING_FIQ1;    /*! 0xBC Pending FIQ Register */

        UInt32 ITR2;            /*! 0xC0 Interrupt Input Register */
        UInt32 MIR2;            /*! 0xC4 Mask Interrupt Register */
        UInt32 MIR_CLEAR2;      /*! 0xC8 MIR Clear Register */
        UInt32 MIR_SET2;        /*! 0xCC MIR Set Register */
        UInt32 ISR_SET2;        /*! 0xD0 Interrupt Set Register */
        UInt32 ISR_CLEAR2;      /*! 0xD4 Interrupt Clear Register */
        UInt32 PENDING_IRQ2;    /*! 0xD8 Pending IRQ Register */
        UInt32 PENDING_FIQ2;    /*! 0xDC Pending FIQ Register */

        UInt32 ITR3;            /*! 0xE0 Interrupt Input Register */
        UInt32 MIR3;            /*! 0xE4 Mask Interrupt Register */
        UInt32 MIR_CLEAR3;      /*! 0xE8 MIR Clear Register */
        UInt32 MIR_SET3;        /*! 0xEC MIR Set Register */
        UInt32 ISR_SET3;        /*! 0xF0 Interrupt Set Register */
        UInt32 ISR_CLEAR3;      /*! 0xF4 Interrupt Clear Register */
        UInt32 PENDING_IRQ3;    /*! 0xF8 Pending IRQ Register */
        UInt32 PENDING_FIQ3;    /*! 0xFC Pending FIQ Register */

        UInt32 ILR[128];        /*! 0x100 thru 0x2FC */
    };

    extern volatile Intc intc;

    // -------- Module Parameters --------

    /*! Reset Handler. Default is c_int00 */
    metaonly config VectorFuncPtr resetFunc;

    /*!
     * Undefined instruction exception handler.
     * Default is an internal exception handler.
     */
    metaonly config VectorFuncPtr undefinedInstFunc;

    /*!
     * SWI Handler. Default is internal SWI handler.
     * Default is an internal exception handler.
     */
    metaonly config VectorFuncPtr swiFunc;

    /*!
     * Prefetch abort exception handler.
     * Default is an internal exception handler.
     */
    metaonly config VectorFuncPtr prefetchAbortFunc;

    /*!
     * Data abort exception handler.
     * Default is an internal exception handler.
     */
    metaonly config VectorFuncPtr dataAbortFunc;

    /*!
     * Reserved exception handler.
     * Default is an internal exception handler.
     */
    metaonly config VectorFuncPtr reservedFunc;

    /*! IRQ interrupt handler. Default is internal IRQ dispatcher */
    metaonly config VectorFuncPtr irqFunc;

    /*! FIQ interrupt handler. Default is internal FIQ dispatcher */
    metaonly config VectorFuncPtr fiqFunc;

    /*!
     *  FIQ stack pointer. Default = null.
     *  (Indicates that stack is to be created internally)
     */
    config Ptr fiqStack = null;

    /*!
     * FIQ stack size in MAUs.
     * Default is 1024 bytes.
     */
    metaonly config SizeT fiqStackSize = 1024;

    /*!
     *  Memory section used for FIQ stack
     *  Default is null.
     */
    metaonly config String fiqStackSection = null;

    /*! Initial MIR0 Interrupt Mask. Default is 0xffffffff */
    config Bits32 mir0Mask = 0xffffffff;

    /*! Initial MIR1 Interrupt Mask. Default is 0xffffffff */
    config Bits32 mir1Mask = 0xffffffff;

    /*! Initial MIR2 Interrupt Mask. Default is 0xffffffff */
    config Bits32 mir2Mask = 0xffffffff;

    /*! Initial MIR3 Interrupt Mask. Default is 0xffffffff */
    config Bits32 mir3Mask = 0xffffffff;

    /*!
     *  Error raised when an attempt is made to create a Hwi
     *  that has already been created.
     */
    config Error.Id E_alreadyDefined = {
        msg: "E_alreadyDefined: Hwi already defined, intnum: %d"
    };

    /*!
     *  Error raised when Hwi handle referenced in Hwi_delete()
     *  is not found in the Hwi dispatch table
     */
    config Error.Id E_handleNotFound = {
        msg: "E_handleNotFound: Hwi handle not found: 0x%x"
    };

    /*!
     *  Error raised when an undefined interrupt has fired.
     */
    config Error.Id E_undefined = {
        msg: "E_undefined: Hwi undefined, intnum: %d"
    };

    /*!
     *  Error raised if an attempt is made to create a Hwi
     *  with an interrupt number greater than NUM_INTERRUPTS - 1.
     */
    config Error.Id E_badIntNum = {
        msg: "E_badIntNum, intnum: %d is out of range"
    };

    /*!
     *  Issued just prior to Hwi function invocation (with interrupts disabled)
     */
    config Log.Event LM_begin = {
        mask: Diags.USER1 | Diags.USER2,
        msg: "LM_begin: hwi: 0x%x, func: 0x%x, preThread: %d, intNum: %d, irp: 0x%x"
    };

    /*!
     *  Issued just after return from Hwi function (with interrupts disabled)
     */
    config Log.Event LD_end = {
        mask: Diags.USER2,
        msg: "LD_end: hwi: 0x%x"
    };


    // -------- Module Functions --------

    /*!
     *  ======== disable ========
     */
    @Macro
    override UInt disable();

    /*!
     *  ======== enable ========
     */
    @Macro
    override UInt enable();

    /*!
     *  ======== restore ========
     */
    @Macro
    override Void restore(UInt key);

    /*!
     *  ======== inUseMeta ========
     *  @_nodoc
     *  Check for Hwi already in use.
     *  For internal SYS/BIOS use only.
     *  Should be called prior to any internal Hwi.create().
     *
     *  @param(intNum)  interrupt number
     */
    metaonly Bool inUseMeta(UInt intNum);

    /*!
     *  ======== getHandle ========
     *  Returns Hwi_Handle associated with intNum
     *
     *  @param(intNum)  interrupt number
     */
    @DirectCall
    Handle getHandle(UInt intNum);

    /*!
     *  ======== enableFIQ ========
     *  Enable FIQ interrupts.
     *
     *  @b(returns)     previous FIQ interrupt enable/disable state
     */
    @DirectCall
    UInt enableFIQ();

    /*!
     *  ======== disableFIQ ========
     *  Disable FIQ interrupts.
     *
     *  @b(returns)     previous FIQ interrupt enable/disable state
     */
    @DirectCall
    UInt disableFIQ();

    /*!
     *  ======== restoreFIQ ========
     *  Restore FIQ interrupts.
     *
     *  @param(key)     enable/disable state to restore
     */
    @DirectCall
    Void restoreFIQ(UInt key);

    /*!
     *  ======== enableIRQ ========
     *  Enable IRQ interrupts.
     *
     *  @param(key)     enable/disable state to restore
     */
    @DirectCall
    UInt enableIRQ();

    /*!
     *  ======== disableIRQ ========
     *  Disable IRQ interrupts.
     *
     *  @b(returns)     previous IRQ interrupt enable/disable state
     */
    @DirectCall
    UInt disableIRQ();

    /*!
     *  ======== restoreIRQ ========
     *  Restore IRQ interrupts.
     *
     *  @param(key)     enable/disable state to restore
     */
    @DirectCall
    Void restoreIRQ(UInt key);

    /*!
     *  ======== disableMIR0 ========
     *  Disable specific interrupts.
     *
     *  Disables specific interrupts by setting the bits specified by
     *  mask in the Mask Interrupts Register (MIR).
     *
     *  @param(mask)    bitmask of interrupts to disable
     *  @b(returns)     previous MIR0 settings bitmask
     */
    @DirectCall
    Bits32 disableMIR0(Bits32 mask);
    @DirectCall
    Bits32 disableMIR1(Bits32 mask);
    @DirectCall
    Bits32 disableMIR2(Bits32 mask);
    @DirectCall
    Bits32 disableMIR3(Bits32 mask);

    /*!
     *  ======== enableMIR ========
     *  Enable specific interrupts.
     *
     *  Enables specific interrupts by clearing the bits specified by
     *  mask in the Mask Interrupts Register (MIR).
     *
     *  @param(mask)    bitmask of interrupts to enable
     *  @b(returns)     previous MIR settings bitmask
     */
    @DirectCall
    Bits32 enableMIR0(Bits32 mask);
    @DirectCall
    Bits32 enableMIR1(Bits32 mask);
    @DirectCall
    Bits32 enableMIR2(Bits32 mask);
    @DirectCall
    Bits32 enableMIR3(Bits32 mask);

    /*!
     *  ======== restoreMIR0 ========
     *  Restore maskable interrupts to the state they were in
     *  when either disableMIR0() or enableMIR0() was called.
     *
     *  Simply writes mask to the MIR register.
     *
     *  @param(mask)    bitmask of interrupts to restore
     *  @b(returns)     previous MIR0 settings bitmask
     */
    @DirectCall
    Bits32 restoreMIR0(Bits32 mask);
    @DirectCall
    Bits32 restoreMIR1(Bits32 mask);
    @DirectCall
    Bits32 restoreMIR2(Bits32 mask);
    @DirectCall
    Bits32 restoreMIR3(Bits32 mask);

    /*!
     *  ======== setPriority ========
     *  Set an interrupt's priority.
     *
     *  Valid priority values range from 0 to
     *  ({@link #NUM_PRIORITIES} - 1),
     *  0 is the highest priority.
     *
     *  @param(intNum)      ID of interrupt
     *  @param(priority)    priority
     */
    @DirectCall
    Void setPriority(UInt intNum, UInt priority);

    /*!
     *  ======== setType ========
     *  Set an interrupt's type (FIQ/IRQ).
     *
     *  @param(intNum)      ID of interrupt
     *  @param(type)        type = FIQ/IRQ
     */
    @DirectCall
    Void setType(UInt intNum, Type type);

instance:

    /*! Interrupt type (IRQ/FIQ). Default is IRQ. */
    config Type type = Type_IRQ;

    /*
     *  Interrupt priority.
     *
     *  Valid priorities are 0 - (Hwi_NUM_PRIORITIES - 1).
     *  0 is the highest priority.
     *
     *  The default value of -1 is used as a flag to indicate
     *  the lowest (logical) device-specific priority value.
     *  For instance, on TI81XX devices, this translates
     *  to a priority of 63.
     */
    override config Int priority = -1;

    /*!
     *  The maskSetting parameter is ignored.
     *  Only Hwi_MaskingOption_LOWER is supported.
     *
     *  The interrupt controller is designed for priority based interrupts
     */
    override config IHwi.MaskingOption maskSetting = IHwi.MaskingOption_LOWER;

    /*!
     *  ======== reconfig ========
     *  Reconfigure a dispatched interrupt.
     */
    @DirectCall
    Void reconfig(FuncPtr fxn, const Params *params);

internal:   /* not for client use */

    /*
     * Swi and Task module function pointers.
     * Used to decouple Hwi from Swi and Task when
     * dispatcherSwiSupport or
     * dispatcherTaskSupport is false.
     */
    config UInt (*swiDisable)();
    config Void (*swiRestoreHwi)(UInt);
    config UInt (*taskDisable)();
    config Void (*taskRestoreHwi)(UInt);

    /*
     *  ======== postInit ========
     *  finish initializing static and dynamic Hwis
     */
    Int postInit(Object *hwi, Error.Block *eb);

    /*
     *  ======== initIntController ========
     */
    Void initIntController();

    /* assembly code mode registers setup */
    Void init(Ptr fiqStack);

    /* assembly code to set vector table base address */
    Void initVbar();

    /* Interrupt Dispatcher assembly code wrapper */
    Void dispatchIRQ();

    /* Interrupt Dispatcher C code */
    Void dispatchIRQC(Irp irp);

    /* default FIQ Interrupt Dispatcher */
    Void dispatchFIQC();

    /* non plugged interrupt handler */
    Void nonPluggedHwiHandler(UArg arg);

    /*!
     *  const array to hold all HookSet objects.
     */
    config HookSet hooks[length] = [];

    /*! Meta World Only Hwi Configuration Object. */
    metaonly struct InterruptObj {
        Bool used;              /* Interrupt already defined? */
        FuncPtr fxn;            /* Dispatched ISR function */
    };

    /*!
     * Meta-only array of interrupt objects.
     * This meta-only array of Hwi config objects is initialized
     * in Hwi.xs:module$meta$init().
     */
    metaonly config InterruptObj interrupt[NUM_INTERRUPTS];

    struct Instance_State {
        Type        type;           // Interrupt Type
        UInt        priority;       // Interrupt Priority
        UArg        arg;            // Argument to Hwi function.
        FuncPtr     fxn;            // Hwi function.
        Irp         irp;            // current IRP
        Ptr         hookEnv[];
    };

    struct Module_State {
        Bits32      mir0Mask;       // Initial MIR0 mask
        Bits32      mir1Mask;       // Initial MIR1 mask
        Bits32      mir2Mask;       // Initial MIR2 mask
        Bits32      mir3Mask;       // Initial MIR2 mask
        UInt        spuriousInts;   // Count of spurious interrupts
        UInt        lastSpuriousInt;// Most recent spurious interrupt
        UInt        irp;            // temporary irp storage for IRQ handler
        Char        *taskSP;        // temporary storage of interrupted
                                    // Task's SP during ISR execution

        Char        *isrStack;      // Points to isrStack address
        Ptr         isrStackBase;   // _stack
        Ptr         isrStackSize;   // _STACK_SIZE
        Char        fiqStack[];     // buffer used for FIQ stack
        SizeT       fiqStackSize;
        Hwi.Object  nonPluggedHwi;  // default Hwi object
        Handle      dispatchTable[NUM_INTERRUPTS];
                                    // dispatch table
    };
}

