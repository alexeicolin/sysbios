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
 *  ======== gbl.h ========
 *
 */

#ifndef GBL_
#define GBL_

#ifdef __cplusplus
extern "C" {
#endif

#define ti_sysbios_BIOS__nolocalnames
#define xdc_runtime_Types__nolocalnames

#include <ti/bios/include/std.h>
#include <ti/sysbios/BIOS.h>
#include <xdc/runtime/Types.h>

/* external references */
extern __FAR__ Uint16 GBL_procId;
extern __FAR__ Uint32 GBL_clkIn;
extern __FAR__ Uint32 GBL_freq;

/*
 * ======== GBL_getProcId ========
 *
 * Returns the Uint16 static configured processor ID required by MSGQ
 *
 */
static inline Uint16 GBL_getProcId(Void)
{
    return (GBL_procId);
}

/*
 * ======== GBL_setProcId ========
 *
 * Sets the Uint16 static configured processor ID required by MSGQ
 * Must only be called in GBL User Init Function (bios.GBL.USERINITFXN).
 *
 */
static inline Void GBL_setProcId(Uint16 newProcId)
{
    GBL_procId = newProcId;
}

/*
 * ======== GBL_getClkIn ========
 *
 * Returns the static configured value of the input clock to the board.
 * This value needs to be specified in KHz as an Uint32
 */
static inline Uint32 GBL_getClkIn(Void)
{
    return (GBL_clkIn);
}

/*
 * ======== GBL_getFrequency ========
 *
 * Returns the current value of cpu frequency in KHz as an Uint32.
 *
 */
static inline Uint32 GBL_getFrequency(Void)
{
    xdc_runtime_Types_FreqHz freq1;

    ti_sysbios_BIOS_getCpuFreq(&freq1);
    return (freq1.lo / 1000);
}

/*
 * ======== GBL_setFrequency ========
 *
 * Sets the current frequency of CPU frequency.Frequency needs to be specified
 * as an Uint32 value in Khz. This API can only be called from main.
 * This API does NOT configure the PLL.
 * It only updates an internal BIOS variable.
 */
static inline Bool GBL_setFrequency(Uint32 frequency)
{
    xdc_runtime_Types_FreqHz freq1;

    freq1.lo = frequency * 1000;
    freq1.hi = 0;
    ti_sysbios_BIOS_setCpuFreq(&freq1);

    return (TRUE);
}

// TODO: Uint16  GBL_getVersion(Void)

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  /* GBL_ */

