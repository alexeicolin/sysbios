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
 *  ======== Power.c ========
 */

#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/knl/Swi.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/hal/Hwi.h>
#include <ti/sysbios/hal/unicache/Cache.h>
#include <ti/sysbios/hal/ammu/AMMU.h>
#include "package/internal/Power.xdc.h"

#define _VERBOSE_ 0

#define REG(x) (*((volatile UInt32 *)(x)))

/* INTC register addresses */
#define EVTMASK0_REG    0x01800080
#define EVTMASK1_REG    0x01800084
#define EVTMASK2_REG    0x01800088
#define EVTMASK3_REG    0x0180008C
#define EXPMASK0_REG    0x018000C0
#define EXPMASK1_REG    0x018000C4
#define EXPMASK2_REG    0x018000C8
#define EXPMASK3_REG    0x018000CC
#define INTMUX1_REG     0x01800104
#define INTMUX2_REG     0x01800108
#define INTMUX3_REG     0x0180010C
#define AEGMUX0_REG     0x01800140
#define AEGMUX1_REG     0x01800144
#define INTDMASK_REG    0x01800188

/* power down controller address and state options */
#define PDCCMD_REG      0x01810000
#define PDCCMD_IDLE     0x00005555
#define PDCCMD_STANDBY  0x0001FFFF

/* register offsets from SYSC base */
#define SYSCONFIG_REG_OFFSET    0x8
#define VBUSM2OCP_REG_OFFSET    0x40
#define EDMA_REG_OFFSET         0x48
#define CORE_REG_OFFSET         0x4C
#define IVA_ICTRL_REG_OFFSET    0x50
#define IDLEDLY_REG_OFFSET      0xF08

/* register offsets from EDMA (TPCC) base */
#define CLKGDIS_REG_OFFSET      0xFC
#define DCHMAP0_REG_OFFSET      0x100
#define QCHMAP0_REG_OFFSET      0x200
#define DMAQNUM0_REG_OFFSET     0x240
#define QDMAQNUM_REG_OFFSET     0x260
#define QUETCMAP_REG_OFFSET     0x280
#define QUEPRI_REG_OFFSET       0x284
#define DRAEM0_REG_OFFSET       0x340
#define QWMTHRA_REG_OFFSET      0x620
#define AETCTL_REG_OFFSET       0x700
#define IER_REG_OFFSET          0x1050
#define IERH_REG_OFFSET         0x1054
#define IESR_REG_OFFSET         0x1060
#define IESRH_REG_OFFSET        0x1064
#define QEER_REG_OFFSET         0x1084
#define QEESR_REG_OFFSET        0x108C
#define OPT0_REG_OFFSET         0x4000

/* C register declarations for control registers */
extern cregister volatile unsigned int TSCL;
extern cregister volatile unsigned int AMR;
extern cregister volatile unsigned int CSR;
extern cregister volatile unsigned int IER;
extern cregister volatile unsigned int ISTP;
extern cregister volatile unsigned int IRP;
extern cregister volatile unsigned int SSR;
extern cregister volatile unsigned int GPLYB;
extern cregister volatile unsigned int GFPGFR;
extern cregister volatile unsigned int TSR;
extern cregister volatile unsigned int ITSR;
extern cregister volatile unsigned int IERR;

/* put subsystem context into Power-specific section */
#pragma DATA_SECTION(ssContext, ".data:ti_sysbios_family_c64p_tesla_Power_data");
Power_SubsystemContext ssContext;

/* declarations for globals in the Power-specific section */
extern far UInt32 ti_sysbios_family_c64p_tesla_Power_doResume;
extern far UInt32 ti_sysbios_family_c64p_tesla_Power_readyIdle;
extern far UInt32 ti_sysbios_family_c64p_tesla_Power_cpuRegs;

/*
 *  ======== Power_idleCPU ========
 */
Void Power_idleCPU()
{
    REG(PDCCMD_REG) = PDCCMD_IDLE;
    REG(PDCCMD_REG);
    asm(" idle");
}

/*
 *  ======== Power_suspend ========
 */
UInt Power_suspend(Power_Suspend level)
{
    Bool l1CacheEnabled;
    Bool l2CacheEnabled;
    UInt32 *wordPtr;
    UInt32 taskKey;
    UInt32 swiKey;
    UInt32 hwiKey;
    UInt32 reset;
    UInt32 tmp1;
    UInt32 tmp2;
    UInt32 i;

    /* disable interrupts */
    hwiKey = Hwi_disable();

    /* disable scheduling */
    taskKey = Task_disable();
    swiKey = Swi_disable();

    /* check Unicache state; set 'enabled' flags */
    l1CacheEnabled = Cache_cache.L1_CONFIG & 0x2;
    l2CacheEnabled = Cache_cache.L2_CONFIG & 0x2;

#if _VERBOSE_
    System_printf("Power_suspend\n");
    System_printf("    suspend level = 0x%x\n", level);
    System_printf("    subsystem context = 0x%x\n", &ssContext);
    System_printf("    CPU context = 0x%x\n", 
        &ti_sysbios_family_c64p_tesla_Power_cpuRegs);
    System_printf("    CPU sys regs = 0x%x\n", &ssContext.cpuSysRegs);
    System_printf("    INTC context = 0x%x\n", &ssContext.configINTC);
    System_printf("    SYSC context = 0x%x\n", &ssContext.configSYSC);
    System_printf("    AMMU context = 0x%x\n", &ssContext.configAMMU);
    System_printf("    EDMA context = 0x%x\n", &ssContext.configEDMA);
    System_printf("    TSC flag = 0x%x\n", &ssContext.tscRunning);
    System_printf("    L1 context = 0x%x\n", &ssContext.configL1);
    System_printf("    L1 enabled = 0x%x\n", l1CacheEnabled);
    System_printf("    L2 context = 0x%x\n", &ssContext.configL2);
    System_printf("    L2 enabled = 0x%x\n", l2CacheEnabled);
#endif

    /* = = = = = = = = */
    
    /* if HIBERNATE: save Tesla subsystem context ... */
    if (level == Power_Suspend_HIBERNATE) {

        /* save Unicache config context */
        ssContext.configL1.CONFIG = (UInt32) Cache_cache.L1_CONFIG;
        ssContext.configL1.OCP = (UInt32) Cache_cache.L1_OCP;
        ssContext.configL2.CONFIG = (UInt32) Cache_cache.L2_CONFIG;
        ssContext.configL2.OCP = (UInt32) Cache_cache.L2_OCP;

        /* = = = = = = = = */

        /* save AMMU context */
        for (i = 0; i < AMMU_numLargePages; i++) {
            ssContext.configAMMU.largeAddr[i] = 
                (UInt32) AMMU_mmu.LARGE_ADDR[i];
            ssContext.configAMMU.largePolicy[i] = 
                (UInt32) AMMU_mmu.LARGE_POLICY[i];
        }
        for (i = 0; i < AMMU_numMediumPages; i++) {
            ssContext.configAMMU.medAddr[i] = 
                (UInt32) AMMU_mmu.MEDIUM_ADDR[i];
            ssContext.configAMMU.medPolicy[i] = 
                (UInt32) AMMU_mmu.MEDIUM_POLICY[i];
        }
        for (i = 0; i < AMMU_numSmallPages; i++) {
            ssContext.configAMMU.smallAddr[i] = 
                (UInt32) AMMU_mmu.SMALL_ADDR[i];
            ssContext.configAMMU.smallPolicy[i] = 
                (UInt32) AMMU_mmu.SMALL_POLICY[i];
        }

        /* = = = = = = = = */

        /* save SYSC context */
        ssContext.configSYSC.SYSCONFIG = 
            REG((UInt32)Power_syscRegs + SYSCONFIG_REG_OFFSET);
        ssContext.configSYSC.VBUSM2OCP = 
            REG((UInt32)Power_syscRegs + VBUSM2OCP_REG_OFFSET);
        ssContext.configSYSC.EDMA = 
            REG((UInt32)Power_syscRegs + EDMA_REG_OFFSET);
        ssContext.configSYSC.CORE = 
            REG((UInt32)Power_syscRegs + CORE_REG_OFFSET);
        ssContext.configSYSC.IVA_ICTRL = 
            REG((UInt32)Power_syscRegs + IVA_ICTRL_REG_OFFSET);
        ssContext.configSYSC.IDLEDLY = 
            REG((UInt32)Power_syscRegs + IDLEDLY_REG_OFFSET);

        /* = = = = = = = = */

        /* save INTC context */
        ssContext.configINTC.EVTMASK0 = REG(EVTMASK0_REG);
        ssContext.configINTC.EVTMASK1 = REG(EVTMASK1_REG);
        ssContext.configINTC.EVTMASK2 = REG(EVTMASK2_REG);
        ssContext.configINTC.EVTMASK3 = REG(EVTMASK3_REG);
        ssContext.configINTC.EXPMASK0 = REG(EXPMASK0_REG);
        ssContext.configINTC.EXPMASK1 = REG(EXPMASK1_REG);
        ssContext.configINTC.EXPMASK2 = REG(EXPMASK2_REG);
        ssContext.configINTC.EXPMASK3 = REG(EXPMASK3_REG);
        ssContext.configINTC.INTMUX1 = REG(INTMUX1_REG);
        ssContext.configINTC.INTMUX2 = REG(INTMUX2_REG);
        ssContext.configINTC.INTMUX3 = REG(INTMUX3_REG);
        ssContext.configINTC.AEGMUX0 = REG(AEGMUX0_REG);
        ssContext.configINTC.AEGMUX1 = REG(AEGMUX1_REG);
        ssContext.configINTC.INTDMASK = REG(INTDMASK_REG);

        /* = = = = = = = = */

        /* save EDMA context */
 
        ssContext.configEDMA.CLKGDIS = 
            REG((UInt32)Power_tpccRegs + CLKGDIS_REG_OFFSET);

        /* save DMA chan to PARAM mapping registers */
        wordPtr = (UInt32 *)((UInt32)Power_tpccRegs + DCHMAP0_REG_OFFSET);
        for (i = 0; i < 64; i++) {      
            ssContext.configEDMA.DCHMAP[i] = *wordPtr++;
        }

        /* save QDMA chan to PARAM mapping registers */
        wordPtr = (UInt32 *)((UInt32)Power_tpccRegs + QCHMAP0_REG_OFFSET);
        for (i = 0; i < 8; i++) {       
            ssContext.configEDMA.QCHMAP[i] = *wordPtr++;
        }

        /* save DMA queue mapping registers */
        wordPtr = (UInt32 *)((UInt32)Power_tpccRegs + DMAQNUM0_REG_OFFSET);
        for (i = 0; i < 8; i++) {       
            ssContext.configEDMA.DMAQNUM[i] = *wordPtr++;
        }

        ssContext.configEDMA.QDMAQNUM = 
            REG((UInt32)Power_tpccRegs + QDMAQNUM_REG_OFFSET);
        ssContext.configEDMA.QUETCMAP = 
            REG((UInt32)Power_tpccRegs + QUETCMAP_REG_OFFSET);
        ssContext.configEDMA.QUEPRI = 
            REG((UInt32)Power_tpccRegs + QUEPRI_REG_OFFSET);

        /* save DMA and QDMA region access enable bits */
        wordPtr = (UInt32 *)((UInt32)Power_tpccRegs + DRAEM0_REG_OFFSET);
        for (i = 0; i < (8 * 3); i++) { 
            ssContext.configEDMA.regionAccessBits[i] = *wordPtr++;
        }

        ssContext.configEDMA.QWMTHRA = 
            REG((UInt32)Power_tpccRegs + QWMTHRA_REG_OFFSET);
        ssContext.configEDMA.AETCTL = 
            REG((UInt32)Power_tpccRegs + AETCTL_REG_OFFSET);
        ssContext.configEDMA.IER = 
            REG((UInt32)Power_tpccRegs + IER_REG_OFFSET);
        ssContext.configEDMA.IERH = 
            REG((UInt32)Power_tpccRegs + IERH_REG_OFFSET);
        ssContext.configEDMA.QEER = 
            REG((UInt32)Power_tpccRegs + QEER_REG_OFFSET);

        /* bulk save of all PaRAMs (8 regs * 128 PaRAMs */
        wordPtr = (UInt32 *)((UInt32)Power_tpccRegs + OPT0_REG_OFFSET);
        for (i = 0; i < (8 * 128); i++) {       
            ssContext.configEDMA.PaRAMs[i] = *wordPtr++;
        }

        /* = = = = = = = = */

        /* save CPU control registers */
        ssContext.cpuSysRegs.AMR = AMR;
        ssContext.cpuSysRegs.CSR = CSR;
        ssContext.cpuSysRegs.IER = IER;
        ssContext.cpuSysRegs.ISTP = ISTP;
        ssContext.cpuSysRegs.IRP = IRP;
        ssContext.cpuSysRegs.SSR = SSR;
        ssContext.cpuSysRegs.GPLYB = GPLYB;
        ssContext.cpuSysRegs.GFPGFR = GFPGFR;
        ssContext.cpuSysRegs.TSR = TSR;
        ssContext.cpuSysRegs.ITSR = ITSR;
        ssContext.cpuSysRegs.IERR = IERR;

    }

    /* sample and set the 'TSC is running' state flag */
    tmp1 = TSCL;
    tmp2 = TSCL;
    if (tmp1 == tmp2) {
        ssContext.tscRunning = 0;
    }
    else {
        ssContext.tscRunning = 1;
    }

    /* if Unicache enabled, prepare for standby ... */
    if (l1CacheEnabled || l2CacheEnabled) {

        /* if HIBERNATE: write back all for L1 and L2 */
        if (level == Power_Suspend_HIBERNATE) { 
            Cache_wbAll();
        }
        /* else, retention, just clean the write buffers */
        else {
            Cache_wb(0, 0, Cache_Type_ALL, TRUE);/* start=end=0 -> clean bufs */
        }

        /* now bypass the caches... */
        if (l1CacheEnabled) {
            Cache_disable(Cache_Type_L1);
        }
        if (l2CacheEnabled) {
            Cache_disable(Cache_Type_L2);
        }
    }

    /* set reset-function-sampled 'doing a resume' flag */
    ti_sysbios_family_c64p_tesla_Power_doResume = 1;

    /* set the ready-to-standby flag (an FYI for the MPU) */
    ti_sysbios_family_c64p_tesla_Power_readyIdle = 1;

    /* setup PDC to put GEM into standby when execute IDLE */
    REG(PDCCMD_REG) = PDCCMD_STANDBY;
    REG(PDCCMD_REG);

    /* make function call to save child-preserved CPU regs and do standby ... */
    reset = ti_sysbios_family_c64p_tesla_Power_standby(
        &ti_sysbios_family_c64p_tesla_Power_cpuRegs);

    /* = = = = = = = = */

    /* NOTE: return here both when woke from IDLE, or resumed after reset */

    /* = = = = = = = = */

    /* note: this symbol is not used, but is defined for debug purposes only */
    asm(" .global ti_sysbios_family_c64p_tesla_Power_suspend_RESUME");
    asm("ti_sysbios_family_c64p_tesla_Power_suspend_RESUME:");

    /* if HIBERNATE and *did* reset: restore all context ... */
    if ((reset != 0) && (level == Power_Suspend_HIBERNATE)) {

        /* restore CPU control registers */
        AMR = ssContext.cpuSysRegs.AMR;
        CSR = ssContext.cpuSysRegs.CSR;
        IER = ssContext.cpuSysRegs.IER;
        ISTP = ssContext.cpuSysRegs.ISTP;
        IRP = ssContext.cpuSysRegs.IRP;
        SSR = ssContext.cpuSysRegs.SSR;
        GPLYB = ssContext.cpuSysRegs.GPLYB;
        GFPGFR = ssContext.cpuSysRegs.GFPGFR;
        TSR = ssContext.cpuSysRegs.TSR;
        ITSR = ssContext.cpuSysRegs.ITSR;
        IERR = ssContext.cpuSysRegs.IERR;

        /* = = = = = = = = */

        /* restore AMMU configuration */
        for (i = 0; i < AMMU_numLargePages; i++) {
            AMMU_mmu.LARGE_ADDR[i] = 
                (Char *) ssContext.configAMMU.largeAddr[i];
            AMMU_mmu.LARGE_POLICY[i] = 
                ssContext.configAMMU.largePolicy[i];
        }
        for (i = 0; i < AMMU_numMediumPages; i++) {
            AMMU_mmu.MEDIUM_ADDR[i] =
                (Char *) ssContext.configAMMU.medAddr[i];
            AMMU_mmu.MEDIUM_POLICY[i] =
                ssContext.configAMMU.medPolicy[i];
        }
        for (i = 0; i < AMMU_numSmallPages; i++) {
            AMMU_mmu.SMALL_ADDR[i] =
                (Char *) ssContext.configAMMU.smallAddr[i];
            AMMU_mmu.SMALL_POLICY[i] =
                ssContext.configAMMU.smallPolicy[i];
        }

        /* = = = = = = = = */

        /* restore Unicache config */
        Cache_cache.L1_OCP = ssContext.configL1.OCP;
        tmp1 = Cache_cache.L1_OCP;      /* read to ensure posted write done */
        Cache_cache.L1_CONFIG = ssContext.configL1.CONFIG;
        tmp1 = Cache_cache.L1_CONFIG;   /* read to ensure posted write done */
        Cache_cache.L2_OCP = ssContext.configL2.OCP;
        tmp1 = Cache_cache.L2_OCP;      /* read to ensure posted write done */
        Cache_cache.L2_CONFIG = ssContext.configL2.CONFIG;
        tmp1 = Cache_cache.L2_CONFIG;   /* read to ensure posted write done */

        /* = = = = = = = = */

        /* restore SYSC context */
        REG((UInt32)Power_syscRegs + SYSCONFIG_REG_OFFSET) =
            ssContext.configSYSC.SYSCONFIG;
        REG((UInt32)Power_syscRegs + VBUSM2OCP_REG_OFFSET) =
            ssContext.configSYSC.VBUSM2OCP;
        REG((UInt32)Power_syscRegs + EDMA_REG_OFFSET) =
            ssContext.configSYSC.EDMA;
        REG((UInt32)Power_syscRegs + CORE_REG_OFFSET) =
            ssContext.configSYSC.CORE;
        REG((UInt32)Power_syscRegs + IVA_ICTRL_REG_OFFSET) =
            ssContext.configSYSC.IVA_ICTRL;
        REG((UInt32)Power_syscRegs + IDLEDLY_REG_OFFSET) =
            ssContext.configSYSC.IDLEDLY;

        /* = = = = = = = = */

        /* restore INTC context */
        REG(EVTMASK0_REG) = ssContext.configINTC.EVTMASK0;
        REG(EVTMASK1_REG) = ssContext.configINTC.EVTMASK1;
        REG(EVTMASK2_REG) = ssContext.configINTC.EVTMASK2;
        REG(EVTMASK3_REG) = ssContext.configINTC.EVTMASK3;
        REG(EXPMASK0_REG) = ssContext.configINTC.EXPMASK0;
        REG(EXPMASK1_REG) = ssContext.configINTC.EXPMASK1;
        REG(EXPMASK2_REG) = ssContext.configINTC.EXPMASK2;
        REG(EXPMASK3_REG) = ssContext.configINTC.EXPMASK3;
        REG(INTMUX1_REG) = ssContext.configINTC.INTMUX1;
        REG(INTMUX2_REG) = ssContext.configINTC.INTMUX2;
        REG(INTMUX3_REG) = ssContext.configINTC.INTMUX3;
        REG(AEGMUX0_REG) = ssContext.configINTC.AEGMUX0;
        REG(AEGMUX1_REG) = ssContext.configINTC.AEGMUX1;
        REG(INTDMASK_REG) = ssContext.configINTC.INTDMASK;

        /* = = = = = = = = */

        /* restore EDMA context */

        REG((UInt32)Power_tpccRegs + CLKGDIS_REG_OFFSET) =
            ssContext.configEDMA.CLKGDIS;

        /* restore DMA chan to PARAM mapping registers */
        wordPtr = (UInt32 *)((UInt32)Power_tpccRegs + DCHMAP0_REG_OFFSET);
        for (i = 0; i < 64; i++) {      
            *wordPtr++ = ssContext.configEDMA.DCHMAP[i];
        }

        /* restore QDMA chan to PARAM mapping registers */
        wordPtr = (UInt32 *)((UInt32)Power_tpccRegs + QCHMAP0_REG_OFFSET);
        for (i = 0; i < 8; i++) {       
            *wordPtr++ = ssContext.configEDMA.QCHMAP[i];
        }

        /* restore DMA queue mapping registers */
        wordPtr = (UInt32 *)((UInt32)Power_tpccRegs + DMAQNUM0_REG_OFFSET);
        for (i = 0; i < 8; i++) {       
            *wordPtr++ = ssContext.configEDMA.DMAQNUM[i];
        }

        REG((UInt32)Power_tpccRegs + QDMAQNUM_REG_OFFSET) =
            ssContext.configEDMA.QDMAQNUM;
        REG((UInt32)Power_tpccRegs + QUETCMAP_REG_OFFSET) =
            ssContext.configEDMA.QUETCMAP;
        REG((UInt32)Power_tpccRegs + QUEPRI_REG_OFFSET) =
            ssContext.configEDMA.QUEPRI;

        /* restore DMA and QDMA region access enable bits */
        wordPtr = (UInt32 *)((UInt32)Power_tpccRegs + DRAEM0_REG_OFFSET);
        for (i = 0; i < (8 * 3); i++) { 
            *wordPtr++ = ssContext.configEDMA.regionAccessBits[i];
        }

        REG((UInt32)Power_tpccRegs + QWMTHRA_REG_OFFSET) =
            ssContext.configEDMA.QWMTHRA;
        REG((UInt32)Power_tpccRegs + AETCTL_REG_OFFSET) =
            ssContext.configEDMA.AETCTL;

        /* restore interrupt enable registers (using IESR and IESRH) */
        REG((UInt32)Power_tpccRegs + IESR_REG_OFFSET) =
            ssContext.configEDMA.IER;
        REG((UInt32)Power_tpccRegs + IESRH_REG_OFFSET) =
            ssContext.configEDMA.IERH;

        /* restore QDMA event enable register (using QEESR) */
        REG((UInt32)Power_tpccRegs + QEESR_REG_OFFSET) = 
            ssContext.configEDMA.QEER;

        /* restore all PaRAMs (8 regs * 128 PaRAMs */
        wordPtr = (UInt32 *)((UInt32)Power_tpccRegs + OPT0_REG_OFFSET);
        for (i = 0; i < (8 * 128); i++) {       
            *wordPtr++ = ssContext.configEDMA.PaRAMs[i];
        }

#if _VERBOSE_
        System_printf("hibernate: restored context\n");
#endif

    }

    /* Else: Restore caches to their pre-standby enable state.
     * Note: When come out of retention reset caches will always be enabled,
     *       even if they weren't enabled before standby. So, need to disable
     *       them now, if they weren't enabled when suspend was invoked. 
     */
    else {
        /* restore the enabled state of the caches ... */
        if (l1CacheEnabled) {
            Cache_enable(Cache_Type_L1);
        }
        else {
            Cache_disable(Cache_Type_L1);
        }
        if (l2CacheEnabled) {
            Cache_enable(Cache_Type_L2);
        }
        else {
            Cache_disable(Cache_Type_L2);
        }
    }

#if _VERBOSE_
    System_printf("reset flag = %d\n", reset);
#endif

    /* if TSC was enabled on entry: start it again */
    if (ssContext.tscRunning == 1) {
        TSCL = 1;       /* write any value to TSC to kick start it */
    }

    /* clear the ready-to-standby flag */
    ti_sysbios_family_c64p_tesla_Power_readyIdle = 0;

    /* clear the reset-sampled 'do resume' flag */
    ti_sysbios_family_c64p_tesla_Power_doResume = 0;

    /* re-enable scheduling */
    Task_restore(taskKey);
    Swi_restore(swiKey);

    /* re-enable interrupts */
    Hwi_restore(hwiKey);

    return (reset);
}


