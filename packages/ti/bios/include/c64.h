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
 *  ======== c64.h ========
 *  'C64xx Peripheral Register Definitions
 *
 */

#ifndef C64_
#define C64_

#ifdef __cplusplus
extern "C" {
#endif

#include <ti/bios/include/std.h>

#if defined(_64P_) || defined(_674_) || defined(_64T_) || defined(_66_)
#define ti_sysbios_family_c64p_Hwi__nolocalnames
#include <ti/sysbios/family/c64p/Hwi.h>

#elif defined(_64_)
#define ti_sysbios_family_c64_Hwi__nolocalnames
#include <ti/sysbios/family/c64/Hwi.h>

#else
#error "target not supported"
#endif

/*
 *  Interrupt vector id
 */
#define C64_NMI     1
#define C64_INT4    4
#define C64_INT5    5
#define C64_INT6    6
#define C64_INT7    7
#define C64_INT8    8
#define C64_INT9    9
#define C64_INT10   10
#define C64_INT11   11
#define C64_INT12   12
#define C64_INT13   13
#define C64_INT14   14
#define C64_INT15   15

/*
 *  Interrupt vector mask bits
 */
#define C64_EINT4   (1 << C64_INT4)
#define C64_EINT5   (1 << C64_INT5)
#define C64_EINT6   (1 << C64_INT6)
#define C64_EINT7   (1 << C64_INT7)
#define C64_EINT8   (1 << C64_INT8)
#define C64_EINT9   (1 << C64_INT9)
#define C64_EINT10  (1 << C64_INT10)
#define C64_EINT11  (1 << C64_INT11)
#define C64_EINT12  (1 << C64_INT12)
#define C64_EINT13  (1 << C64_INT13)
#define C64_EINT14  (1 << C64_INT14)
#define C64_EINT15  (1 << C64_INT15)

/*
 *  Timer Controller Registers 
 */
typedef volatile struct {
    Uns       control;        /* Global Control Register              */
    Uns       period;         /* Timer Period                         */
    Uns       counter;        /* Timer Counter                        */
} C64_TRegs;

/*
 * Timer Global Control Register Bits
 */
#define C64_FUNC        0x001   /* Configure TCLK as a timer            */
#define C64_INVOUT      0x002   /* TOUT inverter control.               */
#define C64_DATOUT      0x004   /* TCLK output value                    */
#define C64_DATIN       0x008   /* TCLK input value                     */
#define C64_GO          0x040   /* Reset and start timer                */
#define C64_HLDNOT      0x080   /* Don't hold the timer                 */
#define C64_CP          0x100   /* Clock/Pulse mode control             */
#define C64_CLKSRC      0x200   /* Use internal clock                   */
#define C64_INVINP      0x400   /* TINP inverter control bit            */
#define C64_TSTAT       0x800   /* Timer status                         */
 
/*
 * Peripheral Register Addresses
 */
#define C64_TREGS0      ((C64_TRegs *)0x01940000) /* Timer 0     */
#define C64_TREGS1      ((C64_TRegs *)0x01980000) /* Timer 1     */

/*
 * Preserved for backwards compatibility
 */
#define C64_disable(mask) C64_disableIER(mask)
#define C64_enable(mask) C64_enableIER(mask)

#if defined(_64P_) || defined(_674_) || defined(_64T_) || defined(_66_)
#define C64_disableGIE() (ti_sysbios_family_c64p_Hwi_disable())
#else
#define C64_disableGIE() (ti_sysbios_family_c64_Hwi_disable())
#endif

#if defined(_64P_) || defined(_674_) || defined(_64T_) || defined(_66_)
#define C64_enableGIE(mask) ti_sysbios_family_c64p_Hwi_restore(mask)
#else
#define C64_enableGIE(mask) ti_sysbios_family_c64_Hwi_restore(mask)
#endif

/*
 *  ======== C64_disableIER ========
 *  disable interrupts specified in mask and return old mask
 */
static inline Uns C64_disableIER( Uns mask )
{
#if defined(_64P_) || defined(_674_) || defined(_64T_) || defined(_66_)
    return ti_sysbios_family_c64p_Hwi_disableIER(mask);
#else
    return ti_sysbios_family_c64_Hwi_disableIER(mask);
#endif
}

/*
 *  ======== C64_enableIER ========
 *  enable interrupts specified by mask
 */
static inline Void C64_enableIER( Uns mask )
{
#if defined(_64P_) || defined(_674_) || defined(_64T_) || defined(_66_)
    ti_sysbios_family_c64p_Hwi_enableIER(mask);
#else
    ti_sysbios_family_c64_Hwi_enableIER(mask);
#endif
}

/*
 *  ======== C64_mask ========
 */
#define C64_mask(devid) ((Uns)(1 << devid-1))

/*
 *  ======== C64_plug ========
 */
/* ARGSUSED */
static inline Void C64_plug( Int devid, Void (*isr)(), Int dmachan )
{
#if defined(_64P_) || defined(_674_) || defined(_64T_) || defined(_66_)
    ti_sysbios_family_c64p_Hwi_plug(devid, isr);
#else
    ti_sysbios_family_c64_Hwi_plug(devid, isr);
#endif
}

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  /* C64_ */
