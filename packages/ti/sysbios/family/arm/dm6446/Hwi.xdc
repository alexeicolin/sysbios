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
package ti.sysbios.family.arm.dm6446;

import xdc.rov.ViewInfo;

import xdc.runtime.Diags;
import xdc.runtime.Error;
import xdc.runtime.Log;

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
 *  @p(html)
 *  <h3> Calling Context </h3>
 *  <table border="1" cellpadding="3">
 *    <colgroup span="1"></colgroup> <colgroup span="5" align="center"></colgroup>
 *
 *    <tr><th> Function                 </th><th>  Hwi   </th><th>  Swi   </th><th>  Task  </th><th>  Main  </th><th>  Startup  </th></tr>
 *    <!--                                                                                                                 -->
 *    <tr><td> {@link #clearInterrupt}   </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #create}           </td><td>   N    </td><td>   N    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #disable}          </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #disableEINT0}     </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #disableEINT1}     </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #disableFIQ}       </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #disableInterrupt} </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #disableIRQ}       </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #enable}           </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td><td>   N    </td></tr>
 *    <tr><td> {@link #enableEINT0}      </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #enableEINT1}      </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #enableFIQ}        </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td><td>   N    </td></tr>
 *    <tr><td> {@link #enableInterrupt}  </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #enableIRQ}        </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td><td>   N    </td></tr>
 *    <tr><td> {@link #getHandle}        </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #Params_init}      </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #restore}          </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #restoreEINT0}     </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #restoreEINT1}     </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #restoreFIQ}       </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #restoreInterrupt} </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #restoreIRQ}       </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #setPriority}      </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #construct}        </td><td>   N    </td><td>   N    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #delete}           </td><td>   N    </td><td>   N    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #destruct}         </td><td>   N    </td><td>   N    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #getHookContext}   </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #reconfig}         </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #setFunc}          </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #setHookContext}   </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td colspan="6"> Definitions: <br />
 *       <ul>
 *         <li> <b>Hwi</b>: API is callable from a Hwi thread. </li>
 *         <li> <b>Swi</b>: API is callable from a Swi thread. </li>
 *         <li> <b>Task</b>: API is callable from a Task thread. </li>
 *         <li> <b>Main</b>: API is callable during any of these phases: </li>
 *           <ul>
 *             <li> In your module startup after this module is started (e.g. Hwi_Module_startupDone() returns TRUE). </li>
 *             <li> During xdc.runtime.Startup.lastFxns. </li>
 *             <li> During main().</li>
 *             <li> During BIOS.startupFxns.</li>
 *           </ul>
 *         <li> <b>Startup</b>: API is callable during any of these phases:</li>
 *           <ul>
 *             <li> During xdc.runtime.Startup.firstFxns.</li>
 *             <li> In your module startup before this module is started (e.g. Hwi_Module_startupDone() returns FALSE).</li>
 *           </ul>
 *       </ul>
 *    </td></tr>
 *
 *
 *  </table>
 *  @p
 */

@Template("./Hwi.xdt")  /* generates the vector table and the dispatcher */
@ModuleStartup          /* generates call to Hwi_Module_startup at startup */

module Hwi inherits ti.sysbios.interfaces.IHwi
{

    // -------- Module Constants --------

    /*! The DM6446 ARM Interrupt Controller supports 64 interrupts. */
    const Int NUM_INTERRUPTS = 64;

    // -------- Module Types --------

    /*! Hwi vector function type definition. */
    typedef Void (*VectorFuncPtr)(void);

    /*!
     * ARM Interrupt Controller.
     */
    struct AINTC {
        UInt32 FIQ0;    /*! 0x00 Interrupt Status of INT [31:0] if mapped to FIQ */
        UInt32 FIQ1;    /*! 0x04 Interrupt Status of INT [63:32] if mapped to FIQ */
        UInt32 IRQ0;    /*! 0x08 Interrupt Status of INT [31:0] if mapped to IRQ */
        UInt32 IRQ1;    /*! 0x0C Interrupt Status of INT [63:32] if mapped to IRQ */
        UInt32 FIQENTRY;/*! 0x10 Entry Address [28:0] for valid FIQ interrupt */
        UInt32 IRQENTRY;/*! 0x14 Entry Address [28:0] for valid IRQ interrupt */
        UInt32 EINT0;   /*! 0x18 Interrupt Enable Register 0 */
        UInt32 EINT1;   /*! 0x1C Interrupt Enable Register 1 */
        UInt32 INTCTL;  /*! 0x20 Interrupt Operation Control Register */
        UInt32 EABASE;  /*! 0x24 Interrupt Entry Base Address */
        UInt32 RES[2];  /*! 0x28 reserved */
        UInt32 INTPR[8];/*! 0x30 Interrupt 0-7 Priority select */
//      UInt32 INTPR1;  /*! 0x34 Interrupt 8-15 Priority select */
//      UInt32 INTPR2;  /*! 0x38 Interrupt 16-23 Priority select */
//      UInt32 INTPR3;  /*! 0x3C Interrupt 24-31 Priority select */
//      UInt32 INTPR4;  /*! 0x40 Interrupt 32-39 Priority select */
//      UInt32 INTPR5;  /*! 0x44 Interrupt 40-47 Priority select */
//      UInt32 INTPR6;  /*! 0x48 Interrupt 48-55 Priority select */
//      UInt32 INTPR7;  /*! 0x4C Interrupt 56-63 Priority select */
    };

    /*!
     * Physical ARM Interrupt Controller Device.
     * Short name is "Hwi_aIntc"
     * Long name is "ti_sysbios_family_arm_dm6446_Hwi_aIntc"
     */
    extern volatile AINTC aIntc;

    /*!
     *  ======== BasicView ========
     *  @_nodoc
     */
    metaonly struct BasicView {
        Ptr         halHwiHandle;
        String      label;
        Int         intNum;
        Int         priority;
        String      fxn;
        UArg        arg;
        Ptr         irp;
        String      status;
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
                ['Basic', {type: ViewInfo.INSTANCE, viewInitFxn: 'viewInitBasic', structName: 'BasicView'}],
                ['Module', {type: ViewInfo.MODULE, viewInitFxn: 'viewInitModule', structName: 'ModuleView'}]
            ]
        });

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
     *  FIQ stack size in MAUs.
     *  Default is 1024 bytes.
     */
    metaonly config SizeT fiqStackSize = 1024;

    /*!
     *  Memory section used for FIQ stack
     *  Default is null.
     */
    metaonly config String fiqStackSection = null;

    /*!
     *  EABASE Size Setting. Default is 4 bytes per entry.
     *  Options are 4, 8, 16, 32 bytes per entry.
     */
    config Bits32 eabaseSize = 4;

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
     *  @_nodoc
     *  ======== inUseMeta ========
     *  Check for Hwi already in use.
     *  For internal SYS/BIOS use only.
     *  Should be called prior to any internal Hwi.create().
     *
     *  @param(intNum)  interrupt number
     */
    metaonly Bool inUseMeta(UInt intNum);

    /*!
     *  ======== getHandle ========
     *  Returns pointer to Hwi instance object.
     *
     *  @param(intNum)  interrupt number
     */
    @DirectCall
    Object *getHandle(UInt intNum);

    /*!
     *  @_nodoc
     *  ======== getInstance ========
     *  Returns pointer to Hwi instance object.
     *
     *  @param(intNum)  interrupt number
     */
    @DirectCall
    Object *getInstance(UInt intNum);

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
     *  ======== disableEINT0 ========
     *  Disable specific interrupts (0-31).
     *
     *  Disables specific interrupts by clearing the bits specified by
     *  'mask' in the EINT0.
     *
     *  @param(mask)    bitmask of interrupts to disable
     *  @b(returns)     previous EINT0 settings bitmask
     */
    @DirectCall
    Bits32 disableEINT0(Bits32 mask);

    /*!
     *  ======== disableEINT1 ========
     *  Disable specific interrupts (32-63).
     *
     *  Disables specific interrupts by clearing the bits specified by
     *  'mask' in the EINT1.
     *
     *  @param(mask)    bitmask of interrupts to disable
     *  @b(returns)     previous EINT1 settings bitmask
     */
    @DirectCall
    Bits32 disableEINT1(Bits32 mask);

    /*!
     *  ======== enableEINT0 ========
     *  Enable specific interrupts (0-31).
     *
     *  Enables specific interrupts by clearing the bits specified by
     *  'mask' in the EINT0.
     *
     *  @param(mask)    bitmask of interrupts to disable
     *  @b(returns)     previous EINT0 settings bitmask
     */
    @DirectCall
    Bits32 enableEINT0(Bits32 mask);

    /*!
     *  ======== enableEINT1 ========
     *  Enable specific interrupts (32-63).
     *
     *  Enables specific interrupts by clearing the bits specified by
     *  'mask' in the EINT0.
     *
     *  @param(mask)    bitmask of interrupts to disable
     *  @b(returns)     previous EINT0 settings bitmask
     */
    @DirectCall
    Bits32 enableEINT1(Bits32 mask);

    /*!
     *  ======== restoreEINT0 ========
     *  Restore maskable interrupts to the state they were in
     *  when either disableEINT0() or enableEINT0() was called.
     *
     *  Simply writes mask to the EINT0 register.
     *
     *  @param(mask)    bitmask of interrupts to restore
     *  @b(returns)     previous EINT0 settings bitmask
     */
    @DirectCall
    Bits32 restoreEINT0(Bits32 mask);

    /*!
     *  ======== restoreEINT1 ========
     *  Restore maskable interrupts to the state they were in
     *  when either disableEINT1() or enableEINT1() was called.
     *
     *  Simply writes mask to the EINT1 register.
     *
     *  @param(mask)    bitmask of interrupts to restore
     *  @b(returns)     previous EINT1 settings bitmask
     */
    @DirectCall
    Bits32 restoreEINT1(Bits32 mask);

    /*!
     *  ======== setPriority ========
     *  Set an interrupt's relative priority.
     *
     *  Priority 0-1 define the interrupt as an FIQ interrupt
     *  Priority 2-7 define the interrupt as an IRQ interrupt
     *
     *  @param(intNum)      ID of interrupt
     *  @param(priority)    priority
     */
    @DirectCall
    Void setPriority(UInt intNum, UInt priority);

instance:

    /*! disableMask0. default is derived from MaskingOption_SELF maskSetting. */
    config Bits32 disableMask0 = 0;

    /*! disableMask1. default is derived from MaskingOption_SELF maskSetting. */
    config Bits32 disableMask1 = 0;

    /*! restoreMask0. default is derived from MaskingOption_SELF maskSetting. */
    config Bits32 restoreMask0 = 0;

    /*! restoreMask1. default is derived from MaskingOption_SELF maskSetting. */
    config Bits32 restoreMask1 = 0;

    /*!
     *  Interrupt priority (0-7). Default is 7 which is the
     *  lowest priority IRQ interrupt.
     */
    override config Int priority = 7;

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

    /*
     *  ======== interruptType ========
     *  returns 0 if interrupt is IRQ
     *  returns 1 if interrupt is FIQ
     */
    UInt interruptType(UInt intNum);

    /* assembly code mode registers setup */
    Void init();

    /* Interrupt Dispatcher assembly code wrapper */
    Void dispatchIRQ();

    /* IRQ Interrupt Dispatcher */
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

    metaonly config Bool assignResetVector = true;

    struct Instance_State {
        UInt        priority;       // Interrupt Priority
        Bits32      disableMask0;   // EINT0 disable mask
        Bits32      disableMask1;   // EINT1 disable mask
        Bits32      restoreMask0;   // EINT0 restore mask
        Bits32      restoreMask1;   // EINT1 restore mask
        UArg        arg;            // Argument to Hwi function.
        FuncPtr     fxn;            // Hwi function.
        Int         intNum;         // Interrupt number
        Irp         irp;            // current IRP
        Ptr         hookEnv[];
    };

    struct Module_State {
        Bits32      eint0Mask;      // Initial EINT0 mask
        Bits32      eint1Mask;      // Initial EINT1 mask
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

