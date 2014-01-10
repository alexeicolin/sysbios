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
 *  ======== GBL.xdc ========
 *  metaonly module to support the DSP/BIOS legacy GBL module.
 *
 */

/*!
 *  ======== GBL ========
 *  GBL module
 *
 *  legacy module to support DSP/BIOS 5.x, see DSP/BIOS 5.x docs for more info
 */

@Template("./GBL.xdt")

metaonly module GBL
{
    /*! This property is no longer supported */
    config String BOARDNAME;

    /*! ID used to communicate with other processors using MSGQ */
    config UInt   PROCID = 0;

    /*! Frequency of the input clock in Khz */
    config UInt   CLKIN = 0;

    /*! The DSP speed in Mhz */
    config Float  CLKOUT = 0;

    /*! This property is no longer supported */
    config Bool   SPECIFYRTSLIB;

    /*! This property is no longer supported */
    config String RTSLIB;

    /*! This property is no longer supported */
    config String ENDIANMODE;

    /*! If set to true, the user init function will be called */
    config Bool   CALLUSERINITFXN = 0;
    
    /*! This property is no longer supported */
    config String   CDBRELATIVEPATH;

    /*! The user init function */
    config String USERINITFXN = "FXN_F_nop";

    /*! This property is no longer supported */
    config Bool   ENABLEINST;

    /*! This property is no longer supported */
    config Bool   INSTRUMENTED;

    /*! Enable All TRC Trace Event Clases */
    config Bool   ENABLEALLTRC = true;

    /*! Configure the memory caches */
    config Bool   C64PLUSCONFIGURE = 0;

    /*! This property is no longer supported */
    config String C64PLUSL1PCFG;

    /*! This property is no longer supported */
    config String C64PLUSL1DCFG;

    /*! This property is no longer supported */
    config String C64PLUSL2CFG;

    /*! MAR bitmask for MAR0 to MAR31 */
    config UInt   C64PLUSMAR0to31 = 0;

    /*! MAR bitmask for MAR32 to MAR63 */
    config UInt   C64PLUSMAR32to63 = 0;

    /*! MAR bitmask for MAR64 to MAR95 */
    config UInt   C64PLUSMAR64to95 = 0;

    /*! MAR bitmask for MAR96 to MAR127 */
    config UInt   C64PLUSMAR96to127 = 0;

    /*! MAR bitmask for MAR128 to MAR159 */
    config UInt   C64PLUSMAR128to159 = 0;

    /*! MAR bitmask for MAR160 to MAR191 */
    config UInt   C64PLUSMAR160to191 = 0;

    /*! MAR bitmask for MAR192 to MAR223 */
    config UInt   C64PLUSMAR192to223 = 0;

    /*! MAR bitmask for MAR224 to MAR255 */
    config UInt   C64PLUSMAR224to255 = 0;

    /*! Configure the memory caches */
    config Bool   C641XCONFIGUREL2;

    /*! Initial size for L2 cache */
    config String C641XCCFGL2MODE;

    /*! This property is no longer supported */
    config UInt16 C641XMAREMIFB;

    /*! This property is no longer supported */
    config UInt16 C641XMARCE0;

    /*! This property is no longer supported */
    config UInt16 C641XMARCE1;

    /*! This property is no longer supported */
    config UInt16 C641XMARCE2;

    /*! This property is no longer supported */
    config UInt16 C641XMARCE3;

    /*! This property is no longer supported */
    config String C641XCCFGP;

    /*! This property is no longer supported */
    config UInt16 C641XSETL2ALLOC;

    /*! This property is no longer supported */
    config UInt16 C641XL2ALLOC0;

    /*! This property is no longer supported */
    config UInt16 C641XL2ALLOC1;

    /*! This property is no longer supported */
    config UInt16 C641XL2ALLOC2;

    /*! This property is no longer supported */
    config UInt16 C641XL2ALLOC3;

    /*! This property is no longer supported */
    config String CSRPCC;

    /*! Configure the memory caches */
    config Bool   C621XCONFIGUREL2;

    /*! Initial size for L2 cache */
    config String C621XCCFGL2MODE;

    /*! This property is no longer supported */
    config UInt16 C621XMAR;

    /*! This property is no longer supported */
    config String PLLTYPE;

    /*! This property is no longer supported */
    config Bool   MODIFYPLLCR0;

    /*! This property is no longer supported */
    config UInt   PLLCR0;

    /*! This property is no longer supported */
    config String USERLIMPMODEABORTFXN;

    /*! This property is no longer supported */
    config Bool   MODIFYPLLCR1;

    /*! This property is no longer supported */
    config UInt   PLLCR1;

    /*! This property is no longer supported */
    config UInt   PLLWAITCYCLE;

    /*! This property is no longer supported */
    config UInt16 LOWERPMST;

    /*! This property is no longer supported */
    config UInt16 SWWSR;

    /*! This property is no longer supported */
    config Bool   MODIFYSWCR;

    /*! This property is no longer supported */
    config UInt16 SWCR;

    /*! This property is no longer supported */
    config UInt16 BSCR;

    /*! This property is no longer supported */
    config Bool   MODIFYCLKMD;

    /*! This property is no longer supported */
    config UInt16 CLKMD;

    /*! This property specifies which libraries are linked with C54 programs
     *
     *  In SYS/BIOS 6, the function call model is determined by the target option
     *  on the configuro command line. When converting a Tconf script into an
     *  XDC script, the conversion tool `ti.bios.conversion` prints out the
     *  name of the target to be used on the configuro command line.
     */
    config String CALLMODEL;

    /*! This property specifies the address reach within C28 and C55 programs
     *
     *  In SYS/BIOS 6, the memory model is determined by the target option on the
     *  configuro command line. When converting a Tconf script into an XDC
     *  script, the conversion tool `ti.bios.conversion` prints out the name of
     *  the target to be used on the configuro command line.
     */
    config String MEMORYMODEL;

    /*!
     *  ======== close ========
     *  @_nodoc
     *  function that processes the data
     */
    function close();
    
    /*!
     *  ======== exportMod ========
     *  @_nodoc
     *  function that exports the modules needed
     */
    function exportMod();
}

