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
 *  ======== clk.h ========
 *
 */

#ifndef CLK_
#define CLK_

#ifdef __cplusplus
extern "C" {
#endif

#define ti_sysbios_BIOS__nolocalnames
#define ti_sysbios_knl_Clock__nolocalnames
#define xdc_runtime_Timestamp__nolocalnames
#define xdc_runtime_Types__nolocalnames

#include <xdc/runtime/Types.h>
#include <xdc/runtime/Timestamp.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>

static inline LgUns CLK_countspms()
{
    xdc_runtime_Types_FreqHz freq;

    xdc_runtime_Timestamp_getFreq(&freq);
    return (freq.lo / 1000);
}

static inline Float CLK_cpuCyclesPerHtime()
{
    xdc_runtime_Types_FreqHz freq1, freq2;
    
    ti_sysbios_BIOS_getCpuFreq(&freq1);
    xdc_runtime_Timestamp_getFreq(&freq2);

    return ((float)freq1.lo / (float)freq2.lo);
}

static inline Float CLK_cpuCyclesPerLtime()
{
    xdc_runtime_Types_FreqHz freq1;
    
    ti_sysbios_BIOS_getCpuFreq(&freq1);
    return ((((float)freq1.lo / 1000) * (float)ti_sysbios_knl_Clock_tickPeriod) / 1000);
}

static inline LgUns CLK_gethtime()
{
    return ((LgUns)xdc_runtime_Timestamp_get32());
}

static inline LgUns CLK_getltime()
{
    return (ti_sysbios_knl_Clock_getTicks());
}

static inline LgUns CLK_getprd()
{
    xdc_runtime_Types_FreqHz freq1;
    
    xdc_runtime_Timestamp_getFreq(&freq1);
    return (((freq1.lo / 1000) * ti_sysbios_knl_Clock_tickPeriod) / 1000);
}

static inline Bool CLK_reconfig()
{
    return (ti_sysbios_knl_Clock_tickReconfig());
}

static inline Void CLK_start()
{
    ti_sysbios_knl_Clock_tickStart();
}

static inline Void CLK_stop()
{
    ti_sysbios_knl_Clock_tickStop();
}

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* CLK_ */

