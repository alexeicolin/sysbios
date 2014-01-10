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
 *  ======== c62.h ========
 *  'C62xx Peripheral Register Definitions
 *
 */

#ifndef C62_
#define C62_

#ifdef __cplusplus
extern "C" {
#endif

#include <ti/bios/include/std.h>
#include <ti/bios/include/hwi.h>

#if defined(_64P_) || defined(_674_) || defined(_66_)
#define ti_sysbios_family_c64p_Hwi__nolocalnames
#include <ti/sysbios/family/c64p/Hwi.h>

#elif defined(_67P_)
#define ti_sysbios_family_c67p_Hwi__nolocalnames
#include <ti/sysbios/family/c67p/Hwi.h>

#elif defined(_64_)
#define ti_sysbios_family_c64_Hwi__nolocalnames
#include <ti/sysbios/family/c64/Hwi.h>

#else
#error "target not supported"
#endif

/*
 *  Interrupt vector id
 */
#define C62_NMI     1
#define C62_INT4    4
#define C62_INT5    5
#define C62_INT6    6
#define C62_INT7    7
#define C62_INT8    8
#define C62_INT9    9
#define C62_INT10   10
#define C62_INT11   11
#define C62_INT12   12
#define C62_INT13   13
#define C62_INT14   14
#define C62_INT15   15

/*
 *  Interrupt vector mask bits
 */
#define C62_EINT4   (1 << C62_INT4)
#define C62_EINT5   (1 << C62_INT5)
#define C62_EINT6   (1 << C62_INT6)
#define C62_EINT7   (1 << C62_INT7)
#define C62_EINT8   (1 << C62_INT8)
#define C62_EINT9   (1 << C62_INT9)
#define C62_EINT10  (1 << C62_INT10)
#define C62_EINT11  (1 << C62_INT11)
#define C62_EINT12  (1 << C62_INT12)
#define C62_EINT13  (1 << C62_INT13)
#define C62_EINT14  (1 << C62_INT14)
#define C62_EINT15  (1 << C62_INT15)

/*
 *  Timer Controller Registers 
 */
typedef volatile struct {
    Uns       control;        /* Global Control Register              */
    Uns       period;         /* Timer Period                         */
    Uns       counter;        /* Timer Counter                        */
} C62_TRegs;

/*
 * Timer Global Control Register Bits
 */
#define C62_FUNC        0x001   /* Configure TCLK as a timer            */
#define C62_INVOUT      0x002   /* TOUT inverter control.               */
#define C62_DATOUT      0x004   /* TCLK output value                    */
#define C62_DATIN       0x008   /* TCLK input value                     */
#define C62_GO          0x040   /* Reset and start timer                */
#define C62_HLDNOT      0x080   /* Don't hold the timer                 */
#define C62_CP          0x100   /* Clock/Pulse mode control             */
#define C62_CLKSRC      0x200   /* Use internal clock                   */
#define C62_INVINP      0x400   /* TINP inverter control bit            */
#define C62_TSTAT       0x800   /* Timer status                         */
 
/*
 * Peripheral Register Addresses
 */
#define C62_TREGS0      ((C62_TRegs *)0x01940000) /* Timer 0     */
#define C62_TREGS1      ((C62_TRegs *)0x01980000) /* Timer 1     */

/*
 * Preserved for backwards compatibility
 */
#define C62_disable(mask) C62_disableIER(mask)
#define C62_enable(mask) C62_enableIER(mask)

#if defined(_674_)
#define C62_disableGIE() (ti_sysbios_family_c64p_Hwi_disable())
#elif defined(_67P_)
#define C62_disableGIE() (ti_sysbios_family_c67p_Hwi_disable())
#elif defined(_64P_)
#define C62_disableGIE() (ti_sysbios_family_c64p_Hwi_disable())
#elif defined(_66_)
#define C62_disableGIE() (ti_sysbios_family_c64p_Hwi_disable())
#else
#define C62_disableGIE() (ti_sysbios_family_c64_Hwi_disable())
#endif

#if defined(_674_)
#define C62_enableGIE(mask) ti_sysbios_family_c64p_Hwi_restore(mask)
#elif defined(_67P_)
#define C62_enableGIE(mask) ti_sysbios_family_c67p_Hwi_restore(mask)
#elif defined(_64P_)
#define C62_enableGIE(mask) ti_sysbios_family_c64p_Hwi_restore(mask)
#elif defined(_66_)
#define C62_enableGIE(mask) ti_sysbios_family_c64p_Hwi_restore(mask)
#else
#define C62_enableGIE(mask) ti_sysbios_family_c64_Hwi_restore(mask)
#endif


/*
 *  ======== C62_disableIER ========
 *  disable interrupts specified in mask and return old mask
 */
static inline Uns C62_disableIER( Uns mask )
{
#if defined(_674_)
    return ti_sysbios_family_c64p_Hwi_disableIER(mask);
#elif defined(_67P_)
    return ti_sysbios_family_c67p_Hwi_disableIER(mask);
#elif defined(_64P_)
    return ti_sysbios_family_c64p_Hwi_disableIER(mask);
#elif defined(_66_)
    return ti_sysbios_family_c64p_Hwi_disableIER(mask);
#else
    return ti_sysbios_family_c64_Hwi_disableIER(mask);
#endif
}

/*
 *  ======== C62_enableIER ========
 *  enable interrupts specified by mask
 */
static inline Void C62_enableIER( Uns mask )
{
#if defined(_674_)
    ti_sysbios_family_c64p_Hwi_enableIER(mask);
#elif defined(_67P_)
    ti_sysbios_family_c67p_Hwi_enableIER(mask);
#elif defined(_64P_)
    ti_sysbios_family_c64p_Hwi_enableIER(mask);
#elif defined(_66_)
    ti_sysbios_family_c64p_Hwi_enableIER(mask);
#else
    ti_sysbios_family_c64_Hwi_enableIER(mask);
#endif
}

/*
 *  ======== C62_clearIFR ========
 *  clear pending interrupts specified by mask
 */
static inline Void C62_clearIFR( Uns mask )
{
    extern cregister volatile unsigned int ICR;
    
    ICR = mask;
}

/*
 *  ======== C62_mask ========
 */
#define C62_mask(devid) ((Uns)(1 << devid-1))

/*
 *  ======== C62_plug ========
 */
static inline Void C62_plug( Int devid, Void (*isr)(), Int dmachan )
{
#if defined(_674_)
    ti_sysbios_family_c64p_Hwi_plug(devid, isr);
#elif defined(_67P_)
    ti_sysbios_family_c67p_Hwi_plug(devid, isr);
#elif defined(_64P_)
    ti_sysbios_family_c64p_Hwi_plug(devid, isr);
#elif defined(_66_)
    ti_sysbios_family_c64p_Hwi_plug(devid, isr);
#else
    ti_sysbios_family_c64_Hwi_plug(devid, isr);
#endif
}

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  /* C62_ */
