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
 *  ======== hwi_c64t.h ========
 *
 */

#ifndef HWI_
#define HWI_

#ifdef __cplusplus
extern "C" {
#endif

#define ti_sysbios_family_c64p_Hwi__nolocalnames
#define ti_sysbios_BIOS__nolocalnames

#include <ti/sysbios/family/c64p/Hwi.h>
#include <ti/sysbios/BIOS.h>

/* map legacy wugen APIs to tesla set */
#include <ti/sysbios/family/c64p/tesla/Wugen.h>

#define HWI_applyWugenMasks(mask) ti_sysbios_family_c64p_tesla_Wugen_applyMasks(mask)

#define HWI_disableWugen(eventid) ti_sysbios_family_c64p_tesla_Wugen_disableDma(eventid)

#define HWI_enableWugen(eventid) ti_sysbios_family_c64p_tesla_Wugen_enableEvent(eventid)

#define HWI_getWugenMasks(mask) ti_sysbios_family_c64p_tesla_Wugen_getMasks(mask)

#define HWI_ierToWugenMasks(mask) ti_sysbios_family_c64p_tesla_Wugen_ierToMasks(mask)

#define HWI_wugenDisableDma(dmaid) ti_sysbios_family_c64p_tesla_Wugen_disableDma(dmaid)

#define HWI_wugenEnableDma(dmaid) ti_sysbios_family_c64p_tesla_Wugen_enableDma(dmaid)

#define HWI_disableI()          _disable_interrupts()
#define HWI_enableI()           _enable_interrupts()
#define HWI_restoreI(old)       _restore_interrupts(old)

typedef struct HWI_Attrs {
    Uns         intrMask;       /* IER bitmask, 1="self" (default) */
    Uns         ccMask;         /* CSR CC bitmask, 1="leave alone" (default) */
    Arg         arg;            /* fxn arg (default=0)*/
} HWI_Attrs;

extern __FAR__ HWI_Attrs HWI_ATTRS;

extern Void HWI_dispatchPlug(Int vecid, Fxn fxn, Int dmachan, HWI_Attrs *attrs);

/*
 *  ======== HWI_disable ========
 *  Disable interrupts
 */
static inline Uns HWI_disable()
{
    return (ti_sysbios_family_c64p_Hwi_disable());
}

/*
 *  ======== HWI_enable ========
 *  Enable interrupts
 */
static inline Void HWI_enable()
{
    ti_sysbios_family_c64p_Hwi_enable();
}

/*
 *  ======== HWI_eventMap ========
 */
static inline Void HWI_eventMap(Int vecid, Int eventid)
{
    ti_sysbios_family_c64p_Hwi_eventMap(vecid, eventid);
}

/*
 *  ======== HWI_restore ========
 *  Restore interrupts state
 */
static inline Void HWI_restore(Uns mask)
{
    ti_sysbios_family_c64p_Hwi_restore(mask);
}

/*
 *  ======== HWI_isHWI ========
 *  Returns true if current thread is HWI.
 */
static inline Bool HWI_isHWI()
{
    if (ti_sysbios_BIOS_getThreadType() == ti_sysbios_BIOS_ThreadType_Hwi) {
        return (TRUE);
    }

    return (FALSE);
}

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* HWI_ */
