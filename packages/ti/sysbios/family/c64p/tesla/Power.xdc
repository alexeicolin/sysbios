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
 *  ======== Power.xdc ========
 *
 *
 */

package ti.sysbios.family.c64p.tesla;

/*!
 *  ======== Power ========
 *  Power Module
 *
 *  This module is used to reduce power consumption of the Tesla subsystem.  
 *  Three reduction techniques are supported:
 *
 *  Suspend/Resume with Retention: The Tesla subsystem is put into standby, 
 *  upon which PRCM will transition the power domain to closed-switch retention
 *  (CSWR).  Upon release from retention reset, the CPU will start executing 
 *  from reset; a reset hook function will hijack the boot process, restore CPU
 *  state to that before retention, and then return back into the 
 *  Power_suspend() API, which will return to its caller, to resume the 
 *  application.
 *
 *  Suspend/Resume with Hibernation: The Tesla subsystem context is saved to 
 *  external RAM, and GEM is put into standby.  Upon the standby transition, 
 *  PRCM will transition the power domain to off.  Upon release from reset, the
 *  CPU will start executing from reset; a reset hook function will hijack the 
 *  boot process, restore CPU state, return back into the Power_suspend() API, 
 *  which will restore the remaining Tesla subsystem context, and then return 
 *  to its caller, to resume the application.
 *
 *  Idling in the Idle loop:  A function will be inserted into the Idle loop, 
 *  which will automatically idle the CPU while waiting for the next interrupt.
 * 
 *  The application must explicitly call to Power_suspend() for Suspend/Resume
 *  functionality.  For idling functionality, this enabled statically, in the 
 *  application configuration, and the idling in the idle loop will happen 
 *  routinely upon execution of the Idle loop.
 *
 *  Suspend/Resume can be invoked from Task, Swi, or Idle loop context.  
 *  It cannot be invoked from Hwi context.
 */
module Power
{
    /*! Suspend Level. */
    enum  Suspend {
        Suspend_RETENTION,      /*! Suspend to RETENTION state */
        Suspend_HIBERNATE       /*! Suspend to HIBERNATE state */
    };

    /*! Base address of EDMA TPCC registers */
    config Ptr tpccRegs = 0x01C00000;

    /*! Base address of SYSC registers */
    config Ptr syscRegs = 0x01C20000;

    /*! Idle the CPU when threads blocked waiting for an interrupt? */
    config Bool idle = false;

    /*! Name of segment to load Power's reset code and saved context */
    metaonly config String loadSegment = "EXT_RAM";

    /*!
     *  ======== idleCPU ========
     *  Function used to automatically idle the CPU in the Idle loop.
     *
     *  When the 'idle' configuration flag is set: this function will
     *  be added to the list of Idle loop functions.  When called from the
     *  Idle loop, it will invoke the IDLE instruction, to idle the CPU until 
     *  the next interrupt occurs.  This function will idle the CPU, but will
     *  not put the whole megamodule (GEM) into standby.
     */
    @DirectCall
    Void idleCPU();

    /*!
     *  ======== suspend ========
     *  Function used for suspend/resume of the Tesla subsystem.
     *
     *  This function will save Tesla subsystem context and then put GEM into
     *  standby, as the final steps for a transition to either retention
     *  (CSWR), or hibernate (also referred to as off-mode).
     *
     *  Prior to invoking this function the application software must prepare
     *  for suspend, which includes: configuring PRCM for the appropriate 
     *  transition once GEM goes to standby; disabling unintended wakeup 
     *  interrupts, and configuring intended wakeups; taking care of any 
     *  required notifications to software components; and 'quieting' of the
     *  DSP application (e.g., ensuring that all in-progress EDMA activity 
     *  completes).
     */
    @DirectCall
    UInt suspend(Suspend level);

internal:

    struct CpuSysRegs {
        UInt32  AMR;
        UInt32  CSR;
        UInt32  IER;
        UInt32  ISTP;
        UInt32  IRP;
        UInt32  SSR;
        UInt32  GPLYB;
        UInt32  GFPGFR;
        UInt32  TSR;
        UInt32  ITSR;
        UInt32  IERR;
    }

    struct IntcConfig {
        UInt32 EVTMASK0;
        UInt32 EVTMASK1;
        UInt32 EVTMASK2;
        UInt32 EVTMASK3;
        UInt32 EXPMASK0;
        UInt32 EXPMASK1;
        UInt32 EXPMASK2;
        UInt32 EXPMASK3;
        UInt32 INTMUX1;
        UInt32 INTMUX2;
        UInt32 INTMUX3;
        UInt32 AEGMUX0;
        UInt32 AEGMUX1;
        UInt32 INTDMASK;
    };

    struct SyscConfig {
        UInt32 SYSCONFIG;
        UInt32 VBUSM2OCP;
        UInt32 EDMA;
        UInt32 CORE;
        UInt32 IVA_ICTRL;
        UInt32 IDLEDLY;
    };

    struct UnicacheConfig {
        UInt32 CONFIG;
        UInt32 OCP;
    };

    struct AmmuConfig {
        UInt32 largeAddr[8];
        UInt32 largePolicy[8];
        UInt32 medAddr[7];
        UInt32 medPolicy[7];
        UInt32 smallAddr[3];
        UInt32 smallPolicy[3];
        UInt32 mmuConfig;
    };

    struct EdmaConfig {
        UInt32 CLKGDIS;
        UInt32 DCHMAP[64];
        UInt32 QCHMAP[8];
        UInt32 DMAQNUM[8];
        UInt32 QDMAQNUM;
        UInt32 QUETCMAP;
        UInt32 QUEPRI;
        UInt32 regionAccessBits[24];
        UInt32 QWMTHRA;
        UInt32 AETCTL;
        UInt32 IER;
        UInt32 IERH;
        UInt32 QEER;
        UInt32 PaRAMs[1024];
    };

    struct SubsystemContext {
        CpuSysRegs      cpuSysRegs;
        UnicacheConfig  configL1;
        UnicacheConfig  configL2;
        IntcConfig      configINTC;
        SyscConfig      configSYSC;
        AmmuConfig      configAMMU;
        EdmaConfig      configEDMA;
        Bool            tscRunning;
    };

    /*!
     *  ======== resetFxn ========
     *  Startup reset function that checks if coming out of a processor reset 
     *  due to Power_suspend().  If no, it will simply return.  If yes, it
     *  will restore CPU context, and return using a saved return pointer,
     *  warping back into the Power_suspend API.
     */
    Void resetFxn();

    /*!
     *  ======== standby ========
     *  Function used for final CPU register save and transition to standby.
     */
    UInt standby(UInt32 *cpuRegs);

}

