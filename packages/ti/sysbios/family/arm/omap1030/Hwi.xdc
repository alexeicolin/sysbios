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
 *  ======== Hwi.xdc ========
 *
 */
package ti.sysbios.family.arm.omap1030;

import xdc.rov.ViewInfo;

import xdc.runtime.Diags;
import xdc.runtime.Log;
import xdc.runtime.Error;

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

    /*! OMAP1030 supports 96 interrupts. 32 L1 and 64 L2. */
    const Int NUM_INTERRUPTS = 96;

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
        String      fxn; 
        UArg        arg; 
        Ptr         irp; 
    };
    
    /*!
     *  ======== ModuleView ========
     *  @_nodoc
     */
    metaonly struct ModuleView {
        String      options[4];
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
                ['Module',   {type: ViewInfo.MODULE,   viewInitFxn: 'viewInitModule',   structName: 'ModuleView'}]
            ]
        });

    /*! Interrupt type. IRQ or FIQ */
    enum Type {
        Type_IRQ,               /*! IRQ interrupt. */
        Type_FIQ                /*! FIQ interrupt. */
    };

    /*! Interrupt sense. EDGE or LEVEL */
    enum Sense {
        Sense_EDGE,             /*! Edge sensitive interrupt. */
        Sense_LEVEL,            /*! Level sensitive interrupt. */
        Sense_TRM               /*! set per device TRM */
    };

    /*! Level 1 Interrupt Controller. Symbol "Hwi_l1Intc" is physical device */
    struct L1Intc {
        UInt32 ITR;             /*! 0x00 Interrupt Input Register */ 
        UInt32 MIR;             /*! 0x04 Mask Interrupt Register */ 
        UInt32 res[2];          /*! 0x08 - 0x0F */ 
        UInt32 SIR_IRQ;         /*! 0x10 Source IRQ Register */ 
        UInt32 SIR_FIQ;         /*! 0x14 Source FIQ Register */ 
        UInt32 CONTROL;         /*! 0x18 Control Register */ 
        UInt32 ILR[32];         /*! 0x1C thru 0x9B (32 defined) */ 
        UInt32 SISR;            /*! 0x9C Interrupt Set Register */ 
        UInt32 GMR;             /*! 0xA0 Global Mask Register */ 
    };

    /*! Level 2 Interrupt Controller. Symbol "Hwi_l2Intc" is physical device */
    struct L2Intc {
        UInt32 ITR1;            /*! 0x00 Interrupt Register */ 
        UInt32 MIR1;            /*! 0x04 Mask Interrupt Register */ 
        UInt32 res1[2];         /*! 0x08 - 0x0F */ 
        UInt32 SIR_IRQ;         /*! 0x10 Source IRQ Register */ 
        UInt32 SIR_FIQ;         /*! 0x14 Source FIQ Register */ 
        UInt32 CONTROL;         /*! 0x18 Control Register */ 
        UInt32 ILR1[32];        /*! 0x1C thru 0x9B (32 defined) */ 
        UInt32 SISR1;           /*! 0x9C Interrupt Set Register */ 
        UInt32 STATUS;          /*! 0xA0 Status Register */ 
        UInt32 OCP_CFG;         /*! 0xA4 OCP Config Register */ 
        UInt32 INTH_REV;        /*! 0xA8 Module Revision register */ 
        UInt32 res2[21];        /*! 0xAC - 0xFF */ 
        UInt32 ITR2;            /*! 0x100 Interrupt Register */ 
        UInt32 MIR2;            /*! 0x104 Mask Interrupt Register */ 
        UInt32 res3[5];         /*! 0x108 - 0x11B */ 
        UInt32 ILR2[32];        /*! 0x11C thru 0x19B (32 defined) */ 
        UInt32 SISR2;           /*! 0x19C Interrupt Set Register */ 
    };

    extern volatile L1Intc l1Intc;

    extern volatile L2Intc l2Intc;

    // -------- Module Parameters --------

    /*! Reset Handler. Default is c_int00 */
    metaonly config VectorFuncPtr resetFunc;

    /*! Undefined instruction exception handler. Default is self loop */
    metaonly config VectorFuncPtr undefinedInstFunc;

    /*! SWI Handler. Default is internal SWI handler */
    metaonly config VectorFuncPtr swiFunc;

    /*! Prefetch abort exception handler. Default is self loop */
    metaonly config VectorFuncPtr prefetchAbortFunc;

    /*! Data abort exception handler. Default is self loop */
    metaonly config VectorFuncPtr dataAbortFunc;

    /*! Reserved exception handler. Default is self loop */
    metaonly config VectorFuncPtr reservedFunc;

    /*! IRQ interrupt handler. Default is internal IRQ dispatcher */
    metaonly config VectorFuncPtr irqFunc;

    /*! FIQ interrupt handler. Default is internal FIQ dispatcher */
    metaonly config VectorFuncPtr fiqFunc;

    /*! 
     *  FIQ stack pointer. Default = null.
     *  (Indicates that stack is to be created using
     *  staticPlace()
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

    /*!
     *  Error raised when Hwi is already defined
     */
    config Error.Id E_alreadyDefined = {
        msg: "E_alreadyDefined: Hwi already defined: intr# %d"
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
     *  Disable specific L1 interrupts.
     *
     *  Disables specific L1 interrupts by setting the bits specified by
     *  mask in the level 1 Mask Interrupts Register (MIR).
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

    /*!
     *  ======== enableMIR0 ========
     *  Enable specific L1 interrupts.
     *
     *  Enables specific L1 interrupts by clearing the bits specified by
     *  mask in the level 1 Mask Interrupts Register (MIR).
     *
     *  @param(mask)    bitmask of interrupts to enable
     *  @b(returns)     previous MIR0 settings bitmask
     */
    @DirectCall
    Bits32 enableMIR0(Bits32 mask);
    @DirectCall
    Bits32 enableMIR1(Bits32 mask);
    @DirectCall
    Bits32 enableMIR2(Bits32 mask);

    /*!
     *  ======== restoreMIR0 ========
     *  Restore maskable interrupts to the state they were in 
     *  when either disableMIR0() or enableMIR0() was called.
     *
     *  Simply writes mask to the L1 MIR register.
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

    /*!
     *  ======== setPriority ========
     *  Set an interrupt's relative priority.
     *
     *  Not an instance function so that it can be used
     *  with non-dispatched interrupts.
     *
     *  @param(intNum)      ID of interrupt
     *  @param(priority)    priority
     */
    @DirectCall
    Void setPriority(UInt intNum, UInt priority);

    /*!
     *  ======== setSense ========
     *  Set an interrupt's sense (EDGE/LEVEL).
     *
     *  Not an instance function so that it can be used
     *  with non-dispatched interrupts.
     *
     *  @param(intNum)  ID of interrupt
     *  @param(sense)   sense = EDGE/LEVEL
     */
    @DirectCall
    Void setSense(UInt intNum, Sense sense);

    /*!
     *  ======== setType ========
     *  Set an interrupt's type (FIQ/IRQ).
     *
     *  Not an instance function so that it can be used
     *  with non-dispatched interrupts.
     *
     *  @param(intNum)      ID of interrupt
     *  @param(type)        type = FIQ/IRQ
     */
    @DirectCall
    Void setType(UInt intNum, Type type);

instance:

    /*! 
     *  Interrupt sense (EDGE, LEVEL, TRM. Default is TRM.
     *  TRM setting means that the interrupt sense is configured
     *  per the settings provided in the Technical Reference Manual.
     */
    config Sense sense = Sense_TRM;

    /*! Interrupt type (IRQ/FIQ. Default is IRQ. */
    config Type type = Type_IRQ;

    /*! Interrupt priority (0-63). Default is 0. */
    override config Int priority = 0;

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

    /* Interrupt Dispatcher */
    Void dispatchIRQC(Irp irp);

    /* default FIQ Interrupt Dispatcher */
    Void dispatchFIQC();

    /*!
     *  const array to hold all HookSet objects.
     */
    config HookSet hooks[length] = [];

    /*! Meta World Only Hwi Configuration Object. */
    metaonly struct InterruptObj {
        Bool used;              /* Interrupt already defined? */
        Bool useDispatcher;     /* Should dispatcher handle this Int? */
        FuncPtr fxn;            /* Dispatched ISR function */
    };

    /*!
     * Meta-only array of interrupt objects.
     * This meta-only array of Hwi config objects is initialized
     * in Hwi.xs:module$meta$init().
     */
    metaonly config InterruptObj interrupt[NUM_INTERRUPTS];

    struct Instance_State {
        Sense       sense;          // Interrupt Sense
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
        Bits32      irq0Mask;       // L1 IRQ interrupts
        UInt        irp;            // temporary irp storage for IRQ handler
        Char        *taskSP;        // temporary storage of interrupted
                                    // Task's SP during ISR execution
        /* ROM */
        Char        *isrStack;      // Points to isrStack address
        Ptr         isrStackBase;   // _stack
        Ptr         isrStackSize;   // _STACK_SIZE
        Char        fiqStack[];     // buffer used for FIQ stack
        SizeT       fiqStackSize;
        Handle      dispatchTable[NUM_INTERRUPTS]; 
                                    // dispatch table
    };
}

