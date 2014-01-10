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
 *  ======== TimerSupport.c ========
 */

#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Startup.h>

#include "package/internal/TimerSupport.xdc.h"

#define PSC_MAX_TRANSITION_WAIT         10000

/* 64LC specific PSC registers */
#define PSC_PTCMD               (volatile UInt32 *)0x01C41120
#define PSC_PTSTAT              (volatile UInt32 *)0x01C41128
#define PSC_MDSTAT_TIMER0       (volatile UInt32 *)0x01C4186C
#define PSC_MDCTL_TIMER0        (volatile UInt32 *)0x01C41A6C

/*
 *  ======== TimerSupport_enable ========
 *  This function power enables the Timer that is being used by the CLK module
 *  for the 64LC parts only.
 */
Void TimerSupport_enable(UInt timerId, Error_Block *eb)
{
    Int i;

    volatile UInt32 *ptcmd = PSC_PTCMD;
    volatile UInt32 *ptstat = PSC_PTSTAT;
    volatile UInt32 *mdctl = PSC_MDCTL_TIMER0;
    volatile UInt32 *mdstat = PSC_MDSTAT_TIMER0;
    UInt32 mdctlVal;

    mdctlVal = mdctl[timerId];          /* read PSC timer ctrl register */
    mdctlVal &= 0xfffffffc;             /* disable PSC timer ctrl */
    mdctlVal |= 3;                      /* enable PSC timer ctrl */
    *(&mdctl[timerId]) = mdctlVal;      /* writeback PSC timer ctrl register */

    *ptcmd = 1;                         /* cause power domain transition */

    /* wait for transition to finish */
    for (i = 0; i < (PSC_MAX_TRANSITION_WAIT); i++) {
        if ((*ptstat & 0x1) == 0) {
            break;
        }
    }
    
    /* wait for PSC timer control to be enabled */
    for (i = 0; i < 10000; i++) {
        if ((*(&mdstat[timerId]) & 0x3f) == 3) {
            break;
        }
    }
}


