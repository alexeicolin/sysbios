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
 *  ======== Wugen.c ========
 *  Contains functions specific to OMAP4 Tesla WUGEN module.
 */

#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Startup.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/interfaces/IHwi.h>
#include <ti/sysbios/family/c64p/Hwi.h>

#include "package/internal/Wugen.xdc.h"

#define INTRMUXREG1     0x01800104      /* interrupt mux register on GEM */
#define EVTCLR0         0x01800040      /* event clear register on GEM */
#define EVTMASK         0x01800080      /* event mask register on GEM */

#define WUGENMEVT0      0x01c21060      /* WUGEN_MEVT0 register on IVA2 */
#define WUGENMEVTCLR0   0x01c21074      /* WUGEN_MEVTCLR0 register on IVA2 */
#define WUGENMEVTSET0   0x01c21088      /* WUGEN_MEVTSET0 register on IVA2 */
#define WUGENPNDEVT0    0x01c2109c      /* WUGEN_PENDEVT0 register on IVA2 */
#define WUGENPNDEVTCLR0 0x01c210b0      /* WUGEN_PENDEVTCLR0 register on IVA2 */

extern cregister volatile unsigned int IER;

/*
 *  ======== Wugen_Module_startup ========
 */
Int Wugen_Module_startup(Int status)
{
    UInt32 mask[5];
    volatile UInt32 *evtclr0 = (volatile UInt32 *)EVTCLR0;

    /* initialize the WUGEN to its default state from reset */
    /* IRQs */
    mask[0] = 0xffffffff;       /* 0 - 31 */
    mask[1] = 0xffffffff;       /* 32 - 63 */
    mask[2] = 0x00007fff;       /* 64 - 78 */

    /* DMAs */
    mask[3] = 0xffffffff;       /* 0 - 31 */
    mask[4] = 0x07ffffff;       /* 32 - 58 */

    Wugen_applyMasks(mask);

    /* clear event flag registers 0-3 with mask[0] value */
    evtclr0[0] = mask[0];
    evtclr0[1] = mask[0];
    evtclr0[2] = mask[0];
    evtclr0[3] = mask[0];

    return (Startup_DONE);
}


/*
 *  ======== Wugen_applyMasks ========
 */
Void Wugen_applyMasks(UInt32 mask[])
{
    volatile UInt32 *mevtclr = (volatile UInt32 *)WUGENMEVTCLR0;
    volatile UInt32 *mevtset = (volatile UInt32 *)WUGENMEVTSET0;

    /* clear IRQ masks */
    mevtclr[0] = ~(mask[0]);    
    mevtclr[1] = ~(mask[1]);
    mevtclr[2] = 0x00007fff & ~(mask[2]);

    /* clear DMA requests */
    mevtclr[3] = ~(mask[3]);
    mevtclr[4] = ~(mask[4]);

    /* set IRQ masks */
    mevtset[0] = mask[0];
    mevtset[1] = mask[1];
    mevtset[2] = 0x00007fff & mask[2];

    /* set DMA requests */
    mevtset[3] = mask[3];
    mevtset[4] = 0x07ffffff & mask[4];
}

/*
 *  ======== Wugen_disableDma ========
 */
Void Wugen_disableDma(UInt dmaid)
{
    volatile UInt32 *mevtset = (volatile UInt32 *)WUGENMEVTSET0;

    if (dmaid < 32) {
        mevtset[3] = 1 << dmaid;
    }
    else {
        mevtset[4] = 1 << (dmaid - 32);
    }
}

/*
 *  ======== Wugen_disableEvent ========
 */
Void Wugen_disableEvent(UInt eventid)
{
    UInt irqNum;
    volatile UInt32 *mevtset = (volatile UInt32 *)WUGENMEVTSET0;
    
    irqNum = Wugen_evtToIrq(eventid);

    if (irqNum < 32) {
        mevtset[0] = 1 << irqNum;
    }
    else if (irqNum < 64) {
        mevtset[1] = 1 << (irqNum - 32);
    }
    else if (irqNum < 96) {
        mevtset[2] = 1 << (irqNum - 64);
    }
}

/*
 *  ======== Wugen_enableDma ========
 */
Void Wugen_enableDma(UInt dmaid)
{
    volatile UInt32 *mevtclr = (volatile UInt32 *)WUGENMEVTCLR0;
    
    if (dmaid < 32) {
        mevtclr[3] = 1 << dmaid;
    }
    else {
        mevtclr[4] = 1 << (dmaid - 32);
    }
}

/*
 *  ======== Wugen_enableEvent ========
 */
Void Wugen_enableEvent(UInt eventid)
{
    UInt irqNum;
    volatile UInt32 *mevtclr = (volatile UInt32 *)WUGENMEVTCLR0;
    
    irqNum = Wugen_evtToIrq(eventid);

    if (irqNum < 32) {
        mevtclr[0] = 1 << irqNum;
    }
    else if (irqNum < 64) {
        mevtclr[1] = 1 << (irqNum - 32);
    }
    else if (irqNum < 96) {
        mevtclr[2] = 1 << (irqNum - 64);
    }
}

/*
 *  ======== Wugen_getMasks ========
 */
Void Wugen_getMasks(UInt32 mask[])
{
    volatile UInt32 *mevt = (volatile UInt32 *)WUGENMEVT0;

    mask[0] = mevt[0];
    mask[1] = mevt[1];
    mask[2] = 0x00007fff & mevt[2];

    mask[3] = mevt[3];
    mask[4] = 0x07ffffff & mevt[4];
}

/*
 *  ======== Wugen_ierToMasks ========
 */
Void Wugen_ierToMasks(UInt32 mask[])
{
    Int intNum;
    Int eventNum;       /* The event number */
    Int irqVal;         /* The IVA 2 IRQ values */
    UInt32 evtmask;     /* The value of Event Mask Register */
    volatile UInt32 *evtMaskReg = (volatile UInt32 *)EVTMASK;
    
    /* mask all interrupts */
    mask[0] = ~0;
    mask[1] = ~0;
    mask[2] = ~0;
    mask[3] = ~0;
    mask[4] = ~0;

    /* unmask interrupts 4-15 which are in use */
    for (intNum = 4; intNum < 16; intNum++) {
        if (IER & (1 << intNum)) {

            eventNum = Wugen_intToEvt(intNum);

            if (eventNum > 3) { /* handle non-combined ints */
                irqVal = Wugen_intToIrq(intNum);
                if (irqVal < 32) {
                    mask[0] &= ~(1 << irqVal);
                }
                else if (irqVal < 32) {
                    mask[1] &= ~(1 << (irqVal - 32));
                }
                else {
                    mask[2] &= ~(1 << (irqVal - 64));
                }
            }
            else if (eventNum == 0) {
                /* check the combined events for event 0 */
                /* events 15 to 26 are routed to irqs 0 - 11 */
                evtmask = evtMaskReg[0] & 0x007f8000;
                evtmask = evtmask >> 15;
                /* merge into IRQs 0 - 11 */
                mask[0] &= ~evtmask;
            }
            else if (eventNum == 1) {
                /* check the combined events for event 1 */
                /* events 45 to 63 are routed to irqs 12 - 30 */
                evtmask = evtMaskReg[1] & 0xffffe000;
                evtmask = evtmask >> 1;
                mask[0] &= ~evtmask;
                /* event 64 is routed to irq 31 */
                evtmask = evtMaskReg[2] & 0x00000001;
                evtmask = evtmask << 31;
                mask[0] &= ~evtmask;
            }
            else if (eventNum == 2) {
                /* check the combined events for event 2 */
                /* events 65 to 95 are routed to irqs 32 - 62 */
                evtmask = evtMaskReg[2] & 0xfffffffe;
                evtmask = evtmask >> 1;
                mask[1] &= ~evtmask;
                /* event 96 is routed to irq 63 */
                evtmask = evtMaskReg[3] & 0x00000001;
                evtmask = evtmask << 31;
                mask[1] &= ~evtmask;
            }
            else if (eventNum == 3) {
                /* check the combined events for event 3 */
                /* events 97 to 111 are routed to irqs 64 - 78 */
                evtmask = evtMaskReg[3] & 0x0000fffe;
                evtmask = evtmask >> 1;
                mask[2] &= ~evtmask;
            }
        }
    }
}

/*!
 *  ======== Wugen_intToEvt ========
 */
UInt Wugen_intToEvt(UInt intNum)
{
    Int muxnum;         /* Interrupt mux register number */
    Int bitpos;         /* The bit position of the interrupt select */
    volatile UInt32 *muxReg = (volatile UInt32 *)INTRMUXREG1;

    muxnum = (intNum - 4) >> 2;
    bitpos = (intNum % 4) << 3;

    return ((muxReg[muxnum] & (0x7f << bitpos)) >> bitpos);
}

/*!
 *  ======== Wugen_evtToIrq ========
 */
UInt Wugen_evtToIrq(UInt evtNum)
{
    if (evtNum > 14 && evtNum < 27) { 
        return (evtNum - 15);
    }
    else if (evtNum > 44 && evtNum < 112) { 
        return (evtNum - 33);
    }
    else {
        return(~0);
    }
}

/*!
 *  ======== Wugen_intToIrq ========
 */
UInt Wugen_intToIrq(UInt intNum)
{
    UInt evtNum;

    evtNum = Wugen_intToEvt(intNum);

    return (Wugen_evtToIrq(evtNum));
}

/*!
 *  ======== Wugen_hwiRegisterHook ========
 */
Void Wugen_hwiRegisterHook(Int id)
{
    Wugen_module->hwiHookId = id;
}

/*!
 *  ======== Wugen_hwiCreateHook ========
 */
Void Wugen_hwiCreateHook(IHwi_Handle hwi, Error_Block *eb)
{
    UInt intNum, irqNum;

    for (intNum = 0; intNum < 16 ;intNum++) {
        if (hwi == (IHwi_Handle)Hwi_getHandle(intNum)) {
            break;
        }
    }

    irqNum = Wugen_intToIrq(intNum);

    Hwi_setHookContext((Hwi_Handle)hwi, Wugen_module->hwiHookId, (Ptr)irqNum);
}

/*!
 *  ======== Wugen_hwiBeginHook ========
 */
Void Wugen_hwiBeginHook(IHwi_Handle hwi)
{
    UInt irqNum;
    volatile UInt32 *pendclr = (volatile UInt32 *)WUGENPNDEVTCLR0;

    irqNum = (UInt)Hwi_getHookContext((Hwi_Handle)hwi, Wugen_module->hwiHookId);
    
    if (irqNum < 32) {
        pendclr[0] = 1 << irqNum;
    }
    else if (irqNum < 64) {
        pendclr[1] = 1 << (irqNum - 32);
    }
    else if (irqNum < 96) {
        pendclr[2] = 1 << (irqNum - 64);
    }
}
