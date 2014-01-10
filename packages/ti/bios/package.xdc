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
 *  ======== package.xdc ========
 *
 */

requires ti.sysbios.knl;
requires ti.sysbios.misc;
requires ti.sysbios.heaps;
requires ti.sysbios.knl;

/*!
 *  ======== ti.bios ========
 *  Contains header files included by legacy programs.
 *
 *  SYS/BIOS 6 provides legacy support for DSP/BIOS 5 applications.
 *  Most APIs are supported to make it easy for you to migrate 
 *  DSP/BIOS 5 applications to SYS/BIOS 6.  Only the C6x and 28x targets
 *  are supported.
 *
 *  @p(blist)
 *  - The vast majority of DSP/BIOS 5 C APIs are fully supported and 
 *    callable within SYS/BIOS 6, requiring no changes to existing C code.
 *  - Tconf configuration (*.tcf) code is supported within SYS/BIOS 6 with 
 *    some adjustments.
 *  - SYS/BIOS 6 does not support all targets that were previously supported 
 *    by DSP/BIOS. See the list of 
 *    {@link ../sysbios/family/doc-files/delegates.html supported targets}
 *    for the targets supported by SYS/BIOS 6.
 *  @p
 *
 *  @p(html)
 *  <p>For details about legacy support, see the 
 *  <a href=http://www-s.ti.com/sc/techlit/spraas7 target="new"><i>Migrating 
 *  a DSP/BIOS 5 Application to SYS/BIOS 6</i></a> application note.</p>
 *  @p
 *
 */
package ti.bios [5,6,0] {
    module SYS;
    module LOG, MEM, TSK, CLK;
    module SEM, SWI, IDL, MBX, STS, QUE, PRD;
    module HWI, PIE, BUF, LCK;
    module HOOK, GBL, HST, PIP, RTDX, ECM;
    module DEV, DGN, DHL, DPI;
    module DIO, GIO, SIO, UDEV;
    module MSGQ, POOL, STATICPOOL;
    module PIP;
}
