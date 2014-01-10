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
package ti.sysbios.family.arm.m3;

import xdc.rov.ViewInfo;
import xdc.runtime.Diags;
import xdc.runtime.Log;
import xdc.runtime.Assert;
import xdc.runtime.Error;

import ti.sysbios.BIOS;
import ti.sysbios.interfaces.IHwi;

/*!
 *  ======== Hwi ========
 *  Cortex M3 Hardware Interrupt Manager
 *
 *  The Cortex M3's Nested Vectored Interrupt Controller (NVIC)
 *  supports up to 256 interrupts/exceptions. In practice, most
 *  devices support much fewer (ie the Stellaris family of devices
 *  have only 80 total interrupts defined).
 *
 *  SYS/BIOS Interrupt IDs or interrupt numbers correspond
 *  to an interrupt's position in the interrupt vector table.
 *
 *  ID 0 corresponds to vector 0 which is used by the NVIC
 *  to hold the initial (reset) stack pointer value.
 *
 *  ID 1 corresponds to vector 1 which is the reset vector (ie _c_int00)
 *
 *  IDs 2-14 are hardwired to exceptions.
 *
 *  ID 15 is the SysTick timer interrupt.
 *
 *  ID's 16-255 are mapped to the NVIC's user interrupts 0-239
 *  which are tied to platform specific interrupt sources.
 *
 *  The M3 Hwi module supports "zero latency" interrupts.
 *  Interrupts configured with priority greater (in priority,
 *  lower in number) than {@link #disablePriority}
 *  are NOT disabled by {@link #disable}.
 *
 *  Zero latency interrupts are not handled by the SYS/BIOS interrupt
 *  dispatcher. Instead, they are vectored to directly.
 *  As such, and because they are not masked by {@link #disable},
 *  these interrupt handlers are severely restricted in terms of the
 *  SYS/BIOS APIs they can invoke and THREAD SAFETY MUST BE CAREFULLY
 *  CONSIDERED!
 *
 *  Due to the M3's native automatic stacking of saved-by-caller C
 *  context on the way to an ISR, zero latency interrupt handlers
 *  are implemented using regular C functions (ie no 'interrupt'
 *  keyword is required).
 *
 *  Zero latency interrupts are distinguished from regular dispatched
 *  interrupts at create time by their priority being greater than
 *  {@link #disablePriority}.
 *
 *  Note that since zero latency interrupts don't use the dispatcher,
 *  the {@link ti.sysbios.interfaces.IHwi#arg arg} parameter is not
 *  functional.
 *
 *  @a(Interrupt Masking Options)
 *
 *  The NVIC interrupt controller is designed for priority based
 *  interrupts.
 *
 *  No support is provided for anything but {@link #MaskingOption_LOWER}.
 *
 *  @a(Interrupt Priorities)
 *
 *  In general, the NVIC supports priority values of 0 thru 255.
 *
 *  In practice valid priorities values are device dependent and their
 *  nesting behaviors depend on the {@link #priGroup} setting.
 *
 *  Priority 0 is the highest priority and by default is
 *  reserved for zero latency interrupts
 *  (see {@link #disablePriority}).
 *
 *  See the Cortex M3 architecture reference manual for details
 *  on the behavior of interrupt priorities and their relationship
 *  to the {@link #priGroup} setting.
 *
 *  @a(Interrupt Vector Tables)
 *  Stellaris devices:
 *
 *  By default, two vector tables are created for Stellaris devices:
 *
 *  A 16 entry boot vector table is placed at address 0x00000000 in
 *  FLASH.
 *
 *  An 80 entry vector table is placed at address 0x20000000 in RAM.
 *
 *  The FLASH boot vector table contains the reset vector and exception
 *  handler vectors used until the RAM based vector table is initialized.
 *
 *  The RAM vector table contains the 16 exception vectors as well as all
 *  the 64 user interrupt vectors.
 *
 *  During system startup, the NVIC Vector Table Offset Registor is
 *  intialized to point to this vector table after the table has been
 *  initialized.
 *
 *  @a( )
 *  Dual M3 Core ('Ducati') devices:
 *
 *  By default, Ducati core 0 places its runtime vector table at address
 *  0x00000400 and core 1 places its runtime vector table at address
 *  0x00000800.
 *
 *  Additionally, a boot vector table is placed at address
 *  0x00000000 which is shared by both cores.
 *
 *  The boot reset vector function determines which core it is being
 *  executed on and jumps to the reset vector contained in its corresponding
 *  runtime vector table.
 *
 *  The generation and placement of these vector tables is made
 *  automatically when the
 *  {@link ti.sysbios.family.arm.ducati.Core} module is used.
 *
 *  Although STRONGLY discouraged, this default behavior can be overridden
 *  by explicitly setting the
 *  {@link #resetVectorAddress Hwi.resetVectorAddress} and
 *  {@link #vectorTableAddress Hwi.vectorTableAddress} config parameters.
 *
 *  @a(Restrictions)
 *  When used within a dual M3 core (Ducati) arrangement, care must be
 *  taken when initializing this shared resource.
 *  The "Shared Resources" note provided
 *  in the {@link ti.sysbios.family.arm.ducati ducati} package discusses
 *  the management of the various hardware and software resources
 *  shared by the two M3 cores.
 *  @a
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
 *    <tr><td> {@link #disableInterrupt} </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #enable}           </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td><td>   N    </td></tr>
 *    <tr><td> {@link #enableInterrupt}  </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #Params_init}      </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #restore}          </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #restoreInterrupt} </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #construct}        </td><td>   N    </td><td>   N    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #delete}           </td><td>   N    </td><td>   N    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #destruct}         </td><td>   N    </td><td>   N    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #getHookContext}   </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
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
 *  </table>
 *  @p
 */


@Template("./Hwi.xdt")  /* generates the vector table and the dispatcher */
@ModuleStartup      /* generate a call to startup function */

module Hwi inherits ti.sysbios.interfaces.IHwi
{
    // -------- Module Constants --------

    /*!
     *  The Cortex M3 NVIC supports up to 256 interrupts/exceptions.
     *
     *  The actual number supported is device specific and provided by
     *  the catalog device specification.
     */
    config Int NUM_INTERRUPTS;

    /*!
     *  The Cortex M3 NVIC supports up to 256 interrupt priorities.
     *
     *  The actual number supported is device specific and provided by
     *  the catalog device specification.
     */
    config Int NUM_PRIORITIES;

    // -------- Module Types --------

    /*! Hwi vector function type definition. */
    typedef Void (*VectorFuncPtr)(void);

    /*! Exception hook function type definition. */
    typedef Void (*ExceptionHookFuncPtr)(ExcContext *);

    /*! NVIC Configuration Control Register (CCR). */
    struct CCR {
        Bits8 STKALIGN;         /*! Auto stack alignment in exception */
        Bits8 BFHFNMIGN;        /*! All faults ignore BUS Faults */
        Bits8 DIV_0_TRP;        /*! Trap on divide by zero */
        Bits8 UNALIGN_TRP;      /*! Trap on all unaligned accesses */
        Bits8 USERSETMPEND;     /*! Allow user to trigger interrupts */
        Bits8 NONEBASETHRDENA;  /*! Allow entering thread mode anytime */
    };

    /*! @_nodoc
     * Nested Vectored Interrupt Controller.
     */
    struct NVIC {
        UInt32 RES_00;       /*! 0xE000E000 reserved */
        UInt32 ICTR;         /*! 0xE000E004 Interrupt Control Type */
        UInt32 RES_08;       /*! 0xE000E008 reserved */
        UInt32 RES_0C;       /*! 0xE000E00C reserved */
        UInt32 STCSR;        /*! 0xE000E010 SysTick Control & Status Register */
        UInt32 STRVR;        /*! 0xE000E014 SysTick Reload Value Register */
        UInt32 STCVR;        /*! 0xE000E018 SysTick Current Value Register */
        UInt32 STCALIB;      /*! 0xE000E01C SysTick Calibration Value Register */
        UInt32 RES_20 [56];  /*! 0xE000E020-0xE000E0FC reserved */
        UInt32 ISER [8];     /*! 0xE000E100-0xE000E11C Interrupt Set Enable Registers */
        UInt32 RES_120 [24]; /*! 0xE000E120-0xE000E17C reserved */
        UInt32 ICER [8];     /*! 0xE000E180-0xE000E19C Interrupt Clear Enable Registers */
        UInt32 RES_1A0 [24]; /*! 0xE000E1A0-0xE000E1FC reserved */
        UInt32 ISPR [8];     /*! 0xE000E200-0xE000E21C Interrupt Set Pending Registers */
        UInt32 RES_220 [24]; /*! 0xE000E220-0xE000E7C reserved */
        UInt32 ICPR [8];     /*! 0xE000E280-0xE000E29C Interrupt Clear Pending Registers */
        UInt32 RES_2A0 [24]; /*! 0xE000E2A0-0xE000E2FC reserved */
        UInt32 IABR [8];     /*! 0xE000E300-0xE000E31C Interrupt Active Bit Registers */
        UInt32 RES_320 [56]; /*! 0xE000E320-0xE000E3FC reserved */
        UInt8  IPR [240];    /*! 0xE000E400-0xE000E4EF Interrupt Priority Registers */
        UInt32 RES_4F0 [516];/*! 0xE000E4F0-0xE000ECFC reserved */
        UInt32 CPUIDBR;      /*! 0xE000ED00 CPUID Base Register */
        UInt32 ICSR;         /*! 0xE000ED04 Interrupt Control State Register */
        UInt32 VTOR;         /*! 0xE000ED08 Vector Table Offset Register */
        UInt32 AIRCR;        /*! 0xE000ED0C Application Interrupt/Reset Control Register */
        UInt32 SCR;          /*! 0xE000ED10 System Control Register */
        UInt32 CCR;          /*! 0xE000ED14 Configuration Control Register */
        UInt8  SHPR[12];     /*! 0xE000ED18 System Handlers 4-15 Priority Registers */
        UInt32 SHCSR;        /*! 0xE000ED24 System Handler Control & State Register */
        UInt8  MMFSR;        /*! 0xE000ED28 Memory Manage Fault Status Register */
        UInt8  BFSR;         /*! 0xE000ED29 Bus Fault Status Register */
        UInt16 UFSR;         /*! 0xE000ED2A Usage Fault Status Register */
        UInt32 HFSR;         /*! 0xE000ED2C Hard Fault Status Register */
        UInt32 DFSR;         /*! 0xE000ED30 Debug Fault Status Register */
        UInt32 MMAR;         /*! 0xE000ED34 Memory Manager Address Register */
        UInt32 BFAR;         /*! 0xE000ED38 Bus Fault Address Register */
        UInt32 AFSR;         /*! 0xE000ED3C Auxiliary Fault Status Register */
        UInt32 PFR0;         /*! 0xE000ED40 Processor Feature Register */
        UInt32 PFR1;         /*! 0xE000ED44 Processor Feature Register */
        UInt32 DFR0;         /*! 0xE000ED48 Debug Feature Register */
        UInt32 AFR0;         /*! 0xE000ED4C Auxiliary Feature Register */
        UInt32 MMFR0;        /*! 0xE000ED50 Memory Model Fault Register0 */
        UInt32 MMFR1;        /*! 0xE000ED54 Memory Model Fault Register1 */
        UInt32 MMFR2;        /*! 0xE000ED58 Memory Model Fault Register2 */
        UInt32 MMFR3;        /*! 0xE000ED5C Memory Model Fault Register3 */
        UInt32 ISAR0;        /*! 0xE000ED60 ISA Feature Register0 */
        UInt32 ISAR1;        /*! 0xE000ED64 ISA Feature Register1 */
        UInt32 ISAR2;        /*! 0xE000ED68 ISA Feature Register2 */
        UInt32 ISAR3;        /*! 0xE000ED6C ISA Feature Register3 */
        UInt32 ISAR4;        /*! 0xE000ED70 ISA Feature Register4 */
        UInt32 RES_D74[5];   /*! 0xE000ED74-0xE000ED84 reserved */
        UInt32 CPACR;        /*! 0xE000ED88 Coprocessor Access Control Register */
        UInt32 RES_D8C[93];  /*! 0xE000ED8C-0xE000EEFC reserved */
        UInt32 STI;          /*! 0xE000EF00 Software Trigger Interrupt Register */
        UInt32 RES_F04[12];  /*! 0xE000EF04-0xE000EF30 reserved */
        UInt32 FPCCR;        /*! 0xE000EF34 FP Context Control Register */
        UInt32 FPCAR;        /*! 0xE000EF38 FP Context Address Register */
        UInt32 FPDSCR;       /*! 0xE000EF3C FP Default Status Control Register */
        UInt32 MVFR0;        /*! 0xE000EF40 Media & FP Feature Register0 */
        UInt32 MVFR1;        /*! 0xE000EF44 Media & FP Feature Register1 */
        UInt32 RES_F48[34];  /*! 0xE000EF48-0xE000EFCC reserved */
        UInt32 PID4;         /*! 0xE000EFD0 Peripheral ID Register4 */
        UInt32 PID5;         /*! 0xE000EFD4 Peripheral ID Register5 */
        UInt32 PID6;         /*! 0xE000EFD8 Peripheral ID Register6 */
        UInt32 PID7;         /*! 0xE000EFDC Peripheral ID Register7 */
        UInt32 PID0;         /*! 0xE000EFE0 Peripheral ID Register0 */
        UInt32 PID1;         /*! 0xE000EFE4 Peripheral ID Register1 */
        UInt32 PID2;         /*! 0xE000EFE8 Peripheral ID Register2 */
        UInt32 PID3;         /*! 0xE000EFEC Peripheral ID Register3 */
        UInt32 CID0;         /*! 0xE000EFF0 Component ID Register0 */
        UInt32 CID1;         /*! 0xE000EFF4 Component ID Register1 */
        UInt32 CID2;         /*! 0xE000EFF8 Component ID Register2 */
        UInt32 CID3;         /*! 0xE000EFFC Component ID Register3 */
    }

    /*!
     * Physical Nested Vectored Interrupt Controller Device.
     * Short name is "Hwi_nvic"
     * Long name is "ti_sysbios_family_arm_m3_Hwi_nvic"
     */
    extern volatile NVIC nvic;

    /*!
     * Virtual Nested Vectored Interrupt Controller structure
     * written to by both cores for SMP.
     * Short name is "Hwi_vnvic"
     * Long name is "ti_sysbios_family_arm_m3_Hwi_vnvic"
     */
    extern volatile NVIC vnvic;

    /*!
     *  Exception Context - Register contents at the time of an exception.
     */
    struct ExcContext {
        /* Thread Context */
        BIOS.ThreadType threadType; /* Type of thread executing at */
                                    /* the time the exception occurred */
        Ptr     threadHandle;       /* Handle to thread executing at */
                                    /* the time the exception occurred */
        Ptr     threadStack;        /* Address of stack contents of thread */
                                    /* executing at the time the exception */
                                    /* occurred */
        SizeT   threadStackSize;    /* size of thread stack */

        /* Internal Registers */
        Ptr     r0;
        Ptr     r1;
        Ptr     r2;
        Ptr     r3;
        Ptr     r4;
        Ptr     r5;
        Ptr     r6;
        Ptr     r7;
        Ptr     r8;
        Ptr     r9;
        Ptr     r10;
        Ptr     r11;
        Ptr     r12;
        Ptr     sp;
        Ptr     lr;
        Ptr     pc;
        Ptr     psr;

        /* NVIC registers */
        Ptr     ICSR;
        Ptr     MMFSR;
        Ptr     BFSR;
        Ptr     UFSR;
        Ptr     HFSR;
        Ptr     DFSR;
        Ptr     MMAR;
        Ptr     BFAR;
        Ptr     AFSR;
    }

    /*! @_nodoc */
    metaonly struct BasicView {
        Ptr         halHwiHandle;
        String      label;
        String      type;
        Int         intNum;
        Int         priority;
        Int         group;
        Int         subPriority;
        String      fxn;
        UArg        arg;
    };

    /*! @_nodoc */
    metaonly struct DetailedView {
        Ptr         halHwiHandle;
        String      label;
        String      type;
        Int         intNum;
        Int         priority;
        Int         group;
        Int         subPriority;
        String      fxn;
        UArg        arg;
        Ptr         irp;
        String      status;
        Int         coreId;
    };

    /*! @_nodoc */
    metaonly struct ModuleView {
        String      options[4];
        String      activeInterrupt;
        String      pendingInterrupt;
        String      exception;
        SizeT       hwiStackPeak;
        SizeT       hwiStackSize;
        Ptr         hwiStackBase;
    };

    /*! @_nodoc */
    @Facet
    metaonly config ViewInfo.Instance rovViewInfo =
        ViewInfo.create({
            viewMap: [
                ['Basic',
                    {
                        type: ViewInfo.INSTANCE,
                        viewInitFxn: 'viewInitBasic',
                        structName: 'BasicView'
                    }
                ],
                ['Detailed',
                    {
                        type: ViewInfo.INSTANCE,
                        viewInitFxn: 'viewInitDetailed',
                        structName: 'DetailedView'
                    }
                ],
                ['Module',
                    {
                        type: ViewInfo.MODULE,
                        viewInitFxn: 'viewInitModule',
                        structName: 'ModuleView'
                    }
                ],
                ['Exception',
                    {
                        type: ViewInfo.TREE,
                        viewInitFxn: 'viewInitException',
                        structName: 'ExcContext'
                    }
                ]
            ]
        });

    // -------- Module Parameters --------

    // Logs

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

    // Asserts

    /*! Assert when bad maskSetting parameter provided */
    config Assert.Id A_unsupportedMaskingOption = {
        msg: "A_unsupportedMaskingOption: unsupported maskSetting."
    };

    // Errors

    /*!
     *  Error raised when Hwi is already defined
     */
    config Error.Id E_alreadyDefined = {
        msg: "E_alreadyDefined: Hwi already defined: intr# %d"
    };

    /*!
     *  Error raised when an exception occurs
     */
    config Error.Id E_exception = {
        msg: "E_exception: id = %d, pc = %08x.\nTo see more exception detail, set ti.sysbios.family.arm.m3.Hwi.enableException = true or,\nexamine the Exception view for the ti.sysbios.family.arm.m3.Hwi module using ROV."
    };

    /*!
     *  Error raised when an uninitialized interrupt occurs
     */
    config Error.Id E_noIsr = {
        msg: "E_noIsr: id = %d, pc = %08x"
    };

    /*!
     *  Error raised when NMI exception occurs
     */
    config Error.Id E_NMI = {
        msg: "E_NMI: %s"
    };

    /*!
     *  Error raised when hard fault exception occurs
     */
    config Error.Id E_hardFault = {
        msg: "E_hardFault: %s"
    };

    /*!
     *  Error raised when memory fault exception occurs
     */
    config Error.Id E_memFault = {
        msg: "E_memFault: %s, address: %08x"
    };

    /*!
     *  Error raised when bus fault exception occurs
     */
    config Error.Id E_busFault = {
        msg: "E_busFault: %s, address: %08x"
    };

    /*!
     *  Error raised when usage fault exception occurs
     */
    config Error.Id E_usageFault = {
        msg: "E_usageFault: %s"
    };

    /*!
     *  Error raised when svCall exception occurs
     */
    config Error.Id E_svCall = {
        msg: "E_svCall: svNum = %d"
    };

    /*!
     *  Error raised when debugMon exception occurs
     */
    config Error.Id E_debugMon = {
        msg: "E_debugMon: %s"
    };

    /*!
     *  Error raised when reserved exception occurs
     */
    config Error.Id E_reserved = {
        msg: "E_reserved: %s %d"
    };

    // configs

    /*!
     *  Location of the Interrupt Vector Table
     *
     *  This parameter allows the user to override the default placement
     *  of the interrupt vector table. The NVIC's Vector Table Offset
     *  Register (VTOR) is also programmed to this value.
     *
     *  By default, the interrupt vector table will be placed at
     *  address 0x00000000.
     *
     *  Some systems require the vector table to be placed at an address
     *  other than 0 but still need a copy of the two M3 boot vectors
     *  (SP and reset PC), located there. To achieve this, a separate
     *  parameter {@link #resetVectorAdress} is provided. If the
     *  resetVectorAddress has a different value then the vectorTableAddress
     *  then a separate 2 vector table is generated and placed at that
     *  address.
     *
     *  The vector table must be placed at an address at or lower than
     *  0x3FFFFC00 and must be aligned on an even 64 word boundary.
     */
    metaonly config Ptr vectorTableAddress = 0x00000000;

    /*!
     *  Alternate reset vector address. Default is 0x00000000
     *
     *  if initialized by the user then an additional reset vector
     *  is created and placed in the ".resetVecs" section.
     *  To place the .resetVecs section into a specific memory section,
     *  add the following command to your config script:
     *  @p(code)
     *  Program.sectMap[".resetVecs"] = YourMemorySection;
     *  @p
     */
    metaonly config Ptr resetVectorAddress = 0x00000000;

    /*! Reset Handler. Default is c_int00 */
    metaonly config VectorFuncPtr resetFunc;

    /*! NMI Handler. Default is set to an internal exception handler */
    metaonly config VectorFuncPtr nmiFunc;

    /*! Hard Fault Handler. Default is set to an internal exception handler */
    metaonly config VectorFuncPtr hardFaultFunc;

    /*! Hard Mem Handler. Default is set to an internal exception handler */
    metaonly config VectorFuncPtr memFaultFunc;

    /*! Bus Fault Handler. Default is set to an internal exception handler */
    metaonly config VectorFuncPtr busFaultFunc;

    /*! Usage Fault Handler. Default is set to an internal exception handler */
    metaonly config VectorFuncPtr usageFaultFunc;

    /*! SVCall Handler. Default is set to an internal exception handler */
    metaonly config VectorFuncPtr svCallFunc;

    /*! Debug Mon Handler. Default is set to an internal exception handler */
    metaonly config VectorFuncPtr debugMonFunc;

    /*! Reserved Exception Handler. Default is set to an internal exception handler */
    metaonly config VectorFuncPtr reservedFunc;

    /*! Uninitialized ISR Handler. Default is set to an internal exception handler */
    metaonly config VectorFuncPtr nullIsrFunc;

    /*
     *  SMP Interrupt affinity mappings
     *
     *  In SMP mode, this array maps an interrupt number to the
     *  coreId it is to be tied to. By default, all ints are mapped to
     *  core 0.
     *
     *  For example, to make Timer 1 from the
     *  ti.sysbios.family.arm.ducati.Timer
     *  module interrupt on core 1 rather than core 0, add the following to
     *  your config file:
     *
     *  @p(code)
     *     var m3Hwi = xdc.useModule('ti.sysbios.family.arm.m3.Hwi');
     *     m3Hwi.intAffinity[22] = 1;
     *  @p
     *
     *  @a(constraints)
     *  Valid core Ids are 0 and 1 for Ducati/Benelli SMP applications.
     *
     *  Interrupt numbers below 16 are ignored.
     *  Only interrupt numbers greater than or equal to #16 can be routed to
     *  either Ducati/Benelli core.
     *
     *  Interrupt #19, the Ducati inter-core interrupt, is reserved for
     *  exclusive use within the SMP kernel.
     */
    metaonly config UInt8 intAffinity[];

    /*!
     *  Enable full exception decoding
     *
     *  When this is enabled, the exception handler will fully
     *  decode an exception and dump the registers to the
     *  system console.
     */
    metaonly config Bool enableException = true;

    /*!
     *  User Exception Context Buffer Address
     *
     *  By default, when an exception occurs, an {@link #ExcContext}
     *  structure is allocated on the ISR stack and filled in within the
     *  exception handler.
     *
     *  If {@link #excContextBuffer} is initialized by the user, the
     *  {@link #ExcContext} structure will be placed at that address instead.
     *
     *  The buffer must be large enough to contain an {@link #ExcContext}
     *  structure.
     */
    metaonly config Ptr excContextBuffer;
    metaonly config Ptr excContextBuffers[];

    /*!
     *  User Exception Stack Buffer Address
     *
     *  By default, when an exception occurs, a pointer to the base address
     *  of the stack being used by the thread causing the exception is placed
     *
     *  If {@link #excStackBuffer} is initialized by the user, the
     *  stack contents of the thread causing the exception will be
     *  copied to that address instead.
     *
     *  The buffer must be large enough to contain the largest task stack
     *  or ISR stack defined in the application.
     */
    metaonly config Ptr excStackBuffer;
    metaonly config Ptr excStackBuffers[];


    /*!
     *  User Exception hook function.
     *
     *  Called just after the exception context has been initialized.
     *
     *  This function will be run on the ISR stack.
     *
     *  This function must run to completion.
     *
     *  It is called without any Task or Swi scheduling protection
     *  and therefore can not call any functions that may cause a Swi or Task
     *  scheduling operation (Swi_post(), Semaphore_post(), Event_post(), etc).
     */
    config ExceptionHookFuncPtr excHookFunc = null;
    config ExceptionHookFuncPtr excHookFuncs[];

    /*!
     *  NVIC CCR register settings
     *
     *  These setting are written to Hwi_nvic.CCR at startup time.
     *
     *  See the Cortex M3 architecture reference manual for details
     *  on the meanings of these parameters.
     */
    metaonly config CCR nvicCCR = {
        STKALIGN: 1,
        BFHFNMIGN: 0,
        DIV_0_TRP: 0,
        UNALIGN_TRP: 0,
        USERSETMPEND: 0,
        NONEBASETHRDENA: 0
    };

    /*!
     *  The priority that BASEPRI is set to by Hwi_disable().
     *
     *  All interrupts configured with equal or less priority (equal or
     *  higher number) than disablePriority are disabled by Hwi_disable.
     *  Interrupts configured with higher priority (smaller number) than
     *  disablePriority are non-maskable (ie zero-latency).
     *
     *  The default setting is the second highest interrupt priority
     *  defined for the device. This results in priority 0 (and all
     *  other values in the same priority group) being
     *  the non-maskable interrupt priority. All other priorities
     *  are disabled with Hwi_disable().
     */
    config UInt disablePriority;

    /*!
     *  The PRIGROUP setting. Default is 0.
     *
     *  This value will be written to the PRIGROUP field
     *  within the NVIC's Application Interrupt and Reset Control
     *  Register (Hwi_nvic.AIRCR). It defines how the 8 bit priority
     *  values are interpreted by the hardware.
     *
     *  Valid settings are 0-7.
     *
     *  The default setting of 0 causes bits 7-1 of an interrupt's
     *  priority value to be used to as a pre-emption priority, and bit 0
     *  is used to determine which of two simultaneous interrupts with
     *  the same pre-emption priority will be serviced first.
     */
    config UInt priGroup = 0;

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
     *  ======== disableFxn ========
     *  function call implementation
     */
    @DirectCall
    UInt disableFxn();

    /*!
     *  @_nodoc
     *  ======== enableFxn ========
     *  function call implementation
     */
    @DirectCall
    UInt enableFxn();

    /*!
     *  @_nodoc
     *  ======== restoreFxn ========
     *  function call implementation
     */
    @DirectCall
    Void restoreFxn(UInt key);

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
     *  @_nodoc
     *  ======== plug ========
     *  Plug a non dispatched interrupt vector with an ISR address.
     *
     *  @param(intNum)  interrupt number
     *  @param(fxn)     pointer to ISR function
     */
    @DirectCall
    Void plug(UInt intNum, VectorFuncPtr fxn);

    /*!
     *  ======== getHandle ========
     *  Returns Hwi_handle associated with intNum
     *
     *  @param(intNum)  interrupt number
     */
    @DirectCall
    Handle getHandle(UInt intNum);

    /*!
     *  ======== setPriority ========
     *  Set an interrupt's relative priority.
     *
     *  Valid priorities are 0 - 255. 0 is highest priority.
     *
     *  @param(intNum)      ID of interrupt
     *  @param(priority)    priority
     */
    @DirectCall
    Void setPriority(UInt intNum, UInt priority);

    /*!
     *  ======== excSetBuffers ========
     *  Set the exception context and stack buffer pointers
     *
     *  @param(excContextBuffer)        Address to place ExcContext
     *  @param(excStackBuffer)          Address to place ExcStack
     */
    @DirectCall
    Void excSetBuffers(Ptr excContextBuffer, Ptr excStackBuffer);

    /*!
     *  @_nodoc
     *  ======== initNVIC ========
     *  initialize everything but leave ints disabled
     */
    @DirectCall
    Void initNVIC();

    /*!
     *  @_nodoc
     *  ======== initStacks ========
     * set up M3 split stacks
     */
    @DirectCall
    Void initStacks(Ptr hwiStack);

    /*!
     *  @_nodoc
     *  ======== flushVnvic ========
     *  Reconfigure a dispatched interrupt.
     *
     *  Called by the internal function "Hwi_updateNvic()".
     *
     *  This is a public API because it is also called by "Core_hwiFunc()".
     */
    @DirectCall
    Void flushVnvic();

instance:

    /*!
     *  Interrupt priority.
     *  The default is 255 which is the lowest priority.
     *
     *  Priority 0 is the highest priority and by default is
     *  reserved for zero latency interrupts
     *  (see {@link #disablePriority}).
     *
     *  Valid priorities values are device dependent and their
     *  nesting behaviors depend on the {@link #priGroup} setting.
     *
     *  See the Cortex M3 architecture reference manual for details
     *  on the meanings of these parameters.
     */
    override config Int priority = 255;

    /*!
     * Interrupt Masking Option. Only MaskingOption_LOWER is supported.
     *
     * The NVIC interrupt controller is designed for priority based
     * interrupts. No support is provided for anything but
     * Hwi.MaskingOption_LOWER.
     */
    override config IHwi.MaskingOption maskSetting = IHwi.MaskingOption_LOWER;

    /*!
     *  Use the interrupt dispatcher with this interrupt. Default is true.
     *
     *  If set to false, the interrupt dispatcher is NOT used. Instead,
     *  the configured Hwi function address is placed directly in the
     *  vector table, which results in the dispatcher being bypassed.
     *
     *  @a(Warning)
     *  Interrupts configured to bupass the dispatcher are not allowed
     *  to call ANY SYS/BIOS APIs that effect thread scheduling. Examples
     *  of API that should no be invoked are:
     *
     *  @p(dlist)
     *    - Swi_post(),
     *    - Semaphore_post(),
     *    - Event_post(),
     *    - Task_yield()
     *  @p
     *
     *  Additionally, although the signature for a non-dispatched interrupt
     *  function is the same as that for a dispatched interrupt
     *  (see {@link #FuncPtr}), no argument is actually passed
     *  to the non-dispatched ISR handler.
     */
    config Bool useDispatcher = true;

    /*!
     *  ======== reconfig ========
     *  Reconfigure a dispatched interrupt.
     */
    @DirectCall
    Void reconfig(FuncPtr fxn, const Params *params);

internal:   /* not for client use */

    /* const dispatchTable */
    config Handle dispatchTable[];

    /* const array of statically created Hwi priorities */
    config UInt8 priorities[];

    /*
     *  The omap4430 ES1 devices have a nasty bug in the unicache
     *  that locks the bus up when an interrupt occurs at a specific
     *  time during an internal cache operation.
     *  The flag below, when set to true, activates special
     *  code in the Hwi module to work around this bug.
     *  "WA1_1" comes from "WorkAround 1.1" from a list of potential
     *  solutions to the problem developed by the design team.
     */
    metaonly config Bool enableWA1_1 = false;

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

    /* initial Hwi_nvic.CCR value */
    config UInt32 ccr;

    /*!
     *  const array to hold all HookSet objects.
     */
    config HookSet hooks[length] = [];

    /*
     *  ======== postInit ========
     *  finish initializing static and dynamic Hwis
     */
    Int postInit(Object *hwi, Error.Block *eb);

    /*!
     *  ======== updateNvic ========
     *  Internal SMP function to cause the virtual NVIC to be flushed to the
     *  actual NVIC.
     *
     *  This function is called by the various user APIs that manipulate
     *  individual NVIC register bits
     *  (ie Hwi_enable/disable/restore/clearInterrupt())
     *
     *  If the current core is the owner of "intNum", flushVnvic() is called
     *  immediately.
     *
     *  Otherwise an intercore interrupt is generated to force the other core
     *  to perform the flushVnvic().
     *
     */
    Void updateNvic(UInt intNum);

    /*!
     *  ======== initEnables ========
     *  initialize the enables
     */
    Void initEnables();

    /*!
     *  ======== excHandlerAsm ========
     *  asm code exception handler
     */
    Void excHandlerAsm();

    /*!
     *  ======== excHandler ========
     *  exception Handler routes to
     *  either min or max exception handler
     */
    Void excHandler(UInt *excStack, UInt lr);

    /*!
     *  ======== excHandlerMin ========
     *  Minimal Exception Handler
     */
    Void excHandlerMin(UInt *excStack, UInt lr);

    /*!
     *  ======== excHandlerMax ========
     *  Full Featured Exception Handler
     */
    Void excHandlerMax(UInt *excStack, UInt lr);

    /*! Hwi exception handler function type definition. */
    typedef Void (*ExcHandlerFuncPtr)(UInt *, UInt);

    config ExcHandlerFuncPtr excHandlerFunc = excHandlerMin;

    /*!
     *  ======== excFillContext ========
     */
    Void excFillContext();

    /*!
     *  ======== excNmi ========
     */
    Void excNmi(UInt *excStack);

    /*!
     *  ======== excHardFault ========
     */
    Void excHardFault(UInt *excStack);

    /*!
     *  ======== excMemFault ========
     */
    Void excMemFault(UInt *excStack);

    /*!
     *  ======== excBusFault ========
     */
    Void excBusFault(UInt *excStack);

    /*!
     *  ======== excUsageFault ========
     */
    Void excUsageFault(UInt *excStack);

    /*!
     *  ======== excSvCall ========
     */
    Void excSvCall(UInt *excStack);

    /*!
     *  ======== excDebugMon ========
     */
    Void excDebugMon(UInt *excStack);

    /*!
     *  ======== excReserved ========
     */
    Void excReserved(UInt *excStack, UInt excNum);

    /*!
     *  ======== excNoIsr ========
     */
    Void excNoIsr(UInt *excStack, UInt excNum);

    /*!
     *  ======== excDumpRegs ========
     */
    Void excDumpRegs(UInt lr);

    /*!
     *  ======== pendSV ========
     * Used by dispatcher
     */
    Void pendSV();

    /*! Hwi vector function type definition. */
    typedef Void (*HandlerFuncPtr)(Handle, UInt);

    /* Low Level Interrupt Dispatcher Wrapper */
    Void dispatch();

    /*
     * "Top Half" of Interrupt Dispatcher
     *  Does not include Swi_restore() and Task_restore()
     */
    UInt dispatchC(Irp irp);

    /* "Bottom half", run swi scheduler */
    Void doSwiRestore(UInt tskKey);

    /* "Bottom half", run task scheduler */
    Void doTaskRestore(UInt tskKey);

    /*! Meta World Only Hwi Configuration Object. */
    metaonly struct InterruptObj {
        String name;            /* symbol used for vector table entry */
        Bool used;              /* Interrupt already defined? */
        Bool useDispatcher;     /* Should dispatcher handle this Int? */
        UInt priority;          /* priority */
        FuncPtr fxn;            /* Dispatched ISR function */
    };

    /*!
     * Meta-only array of interrupt objects.
     * This meta-only array of Hwi config objects is initialized
     * in Hwi.xs:module$meta$init().
     */
    metaonly config InterruptObj interrupt[];

    struct Instance_State {
        UArg            arg;            // Argument to Hwi function.
        FuncPtr         fxn;            // Hwi function.
        Int             intNum;         // Interrupt number
        Irp             irp;            // current IRP
        Ptr             hookEnv[];
    };

    struct Module_State {
        Bits16          enables;            // Bit 15 = SysTick IE,
                                            // Bit 0 = WA1.1
        Bits32          iser[8];            // Initial Interrupt Set Enable Reg values
        UInt8           priorities[];       // table of interrupt priorities
        Char            *taskSP;            // Temporary storage of interrupted
                                            // Task's SP during ISR execution
        Bool            excActive[];        // TRUE if an exception has occurred
        ExcContext      *excContext[];      // Exception context
        Ptr             excStack[];         // Exception thread stack
        Ptr             isrStack;           // Points to isrStack address
        Ptr             isrStackBase;       // = __TI_STACK_BASE
        SizeT           isrStackSize;       // = Program.stack
        Ptr             vectorTableBase;    // Points to base of vector table
        UInt            swiTaskKeys;        // dispatcher Swi and Task key storage
        volatile Bool   vnvicFlushRequired; // if TRUE, Hwi_vnvicFlush will copy
                                            // changed vnvic regs to nvic
        Handle          dispatchTable[];    // dispatch table
        UInt8           intAffinity[];      // smp int-to-coreId mappings
        UInt32          intAffinityMasks[][]; // smp per-core NVIC register masks
    };
}
