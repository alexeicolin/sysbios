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
package ti.sysbios.family.arm.tms570;

import xdc.runtime.Diags;
import xdc.runtime.Error;
import xdc.runtime.Log;
import xdc.rov.ViewInfo;

/*!
 *  ======== Hwi ========
 *  Hardware Interrupt Support Module.
 *
 *  This Hwi module provides TMS570 (VIM) specific implementations of the
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
 *    <tr><td> {@link #disableREQMASKSET0}     </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #disableREQMASKSET1}     </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #disableFIQ}       </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #disableInterrupt} </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #disableIRQ}       </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #enable}           </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td><td>   N    </td></tr>
 *    <tr><td> {@link #enableREQMASKSET0}      </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #enableREQMASKSET1}      </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #enableFIQ}        </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td><td>   N    </td></tr>
 *    <tr><td> {@link #enableInterrupt}  </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #enableIRQ}        </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td><td>   N    </td></tr>
 *    <tr><td> {@link #getHandle}      </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #Params_init}      </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #restore}          </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #restoreREQMASKSET0}     </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #restoreREQMASKSET1}     </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
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

    /*! The TMS570 ARM Interrupt Controller supports 64 interrupts. */
    const Int NUM_INTERRUPTS = 64;

    // -------- Module Types --------

    /*! Hwi vector function type definition. */
    typedef Void (*VectorFuncPtr)(void);

    /*! @_nodoc Hwi plug function type definition. */
    typedef Void (*PlugFuncPtr)(void);

    /*! Interrupt type. IRQ or FIQ */
    enum Type {
        Type_IRQ,               /*! IRQ interrupt. */
        Type_FIQ                /*! FIQ interrupt. */
    };

    /*!
     * ARM Interrupt Controller.
     */
    struct VIM {
        UInt32 PARFLG;          /*! 0xFDEC VIM RAM Parity Flag Register */
        UInt32 PARCTL;          /*! 0xFDF0 VIM RAM Parity Control Register */
        UInt32 ADDERR;          /*! 0xFDF4 Address Parity Error Register */
        UInt32 FBPARERR;        /*! 0xFDF8 Fallback Parity Error Register */
        UInt32 RESFC;           /*! 0xFDFC reserved */

        UInt32 IRQINDEX;        /*! 0x0000 current IRQ id */
        UInt32 FIQINDEX;        /*! 0x0004 current FIQ id */
        UInt32 RES08[2];        /*! 0x0008 reserved */

        UInt32 FIRQPR[4];       /*! 0x0010-0x001C FIQ/IRQ Program Control Registers */
        UInt32 INTREQ[4];       /*! 0x0020-0x002C Pending Interrupt Request Registers */

        UInt32 REQENASET[4];    /*! 0x0030-0x003C Interrupt (Enable) Mask Set Registers */
        UInt32 REQENACLR[4];    /*! 0x0040-0x004C Interrupt (Enable) Mask Clear Registers */

        UInt32 WAKEENASET[4];   /*! 0x0050-0x005C Wake Mask Set Registers */
        UInt32 WAKEENACLR[4];   /*! 0x0060-0x006C Wake Mask Clear Registers */

        UInt32 IRQVECREG;       /*! 0x0070 IRQ vector */
        UInt32 FIQVECREG;       /*! 0x0074 FIQ vector */

        UInt32 CAPEVT;          /*! 0x0078 Capture Event Register */
        UInt32 RES7C[1];        /*! 0x007C reserved */

        UInt8 CHANMAP[128];     /*! 0x0080-0x00FF Channel Map (Priority) Registers */
    };

    /*!
     * Physical Vectored Interrupt Manager (VIM) Device.
     * Short name is "Hwi_vim"
     * Long name is "ti_sysbios_family_arm_tms570_Hwi_vim"
     */
    extern volatile VIM vim;

    /*! @_nodoc */
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

    /*! @_nodoc */
    metaonly struct ModuleView {
        String      options[4];
        SizeT       hwiStackPeak;
        SizeT       hwiStackSize;
        Ptr         hwiStackBase;
    };

    /*! @_nodoc */
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
     *  Returns pointer to Hwi instance object.
     *
     *  @param(intNum)  interrupt number
     */
    @DirectCall
    Object *getHandle(UInt intNum);

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
     *  ======== setPriority ========
     *  Set an interrupt's relative priority.
     *
     *  @param(intNum)      ID of interrupt
     *  @param(priority)    priority (0-63)
     */
    @DirectCall
    Void setPriority(UInt intNum, UInt priority);

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

    /*! disableMask0. default is derived from MaskingOption_SELF maskSetting. */
    config Bits32 disableMask0 = 0;

    /*! disableMask1. default is derived from MaskingOption_SELF maskSetting. */
    config Bits32 disableMask1 = 0;

    /*! restoreMask0. default is derived from MaskingOption_SELF maskSetting. */
    config Bits32 restoreMask0 = 0;

    /*! restoreMask1. default is derived from MaskingOption_SELF maskSetting. */
    config Bits32 restoreMask1 = 0;

    /*! wakeEnable. default is true. */
    config Bool wakeEnable = true;

    /*! Interrupt type (IRQ/FIQ. Default is IRQ. */
    config Type type = Type_IRQ;

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
    Void init();

    /* Interrupt Dispatcher assembly code wrapper */
    Void dispatchIRQ();

    /* IRQ Interrupt Dispatcher */
    Void dispatchIRQC(Irp irp);

    /* default FIQ Interrupt Dispatcher */
    Void dispatchFIQC();

    /*!
     *  ======== plug ========
     *  Plug an interrupt vector with an ISR address.
     *
     *  @param(intNum)  interrupt number
     *  @param(fxn)     pointer to ISR function
     */
    Void plug(UInt intNum, PlugFuncPtr fxn);

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
        Type        type;           // Interrupt Type
        UInt        priority;       // Interrupt Priority
        Bits32      disableMask0;   // REQENASET0 disable mask
        Bits32      disableMask1;   // REQENASET1 disable mask
        Bits32      restoreMask0;   // REQENASET0 restore mask
        Bits32      restoreMask1;   // REQENASET1 restore mask
        UArg        arg;            // Argument to Hwi function.
        FuncPtr     fxn;            // Hwi function.
        Int         intNum;         // Interrupt number
        Irp         irp;            // current IRP
        Ptr         hookEnv[];
    };

    struct Module_State {
        VIM         *vim;
        Bits32      req0Mask;       // Initial REQENASET0 mask
        Bits32      req1Mask;       // Initial REQENASET1 mask
        UInt        irp;            // temporary irp storage for IRQ handler
        Char        *taskSP;        // temporary storage of interrupted
                                    // Task's SP during ISR execution

        /* ROM */
        Char        *isrStack;      // Points to isrStack address
        Ptr         isrStackBase;   // _stack
        Ptr         isrStackSize;   // _STACK_SIZE
        Char        fiqStack[];     // buffer used for FIQ stack
        SizeT       fiqStackSize;
        UInt        *vimRam;        // VIM RAM
        Handle      dispatchTable[NUM_INTERRUPTS];
                                    // dispatch table
    };
}

