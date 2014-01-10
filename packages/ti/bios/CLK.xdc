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
 *  ======== CLK.xdc ========
 *  metaonly module to support the DSP/BIOS legacy CLK module.
 *
 */

/*!
 *  ======== CLK ========
 *  CLK module
 *
 *  legacy module to support DSP/BIOS 5.x, see DSP/BIOS 5.x docs for more info
 */

@Template("./CLK.xdt")
metaonly module CLK
{
    /*! The memory segment that contains the CLK objects */
    config MEM.Instance OBJMEMSEG = null;

    /*!
     *  If true, Use the on-device timer to drive high and low resolution
     *  times and to trigger execution of CLK functions. On some devices
     *  with ENABLEHTIME property, this enables only low-resolution timer.
     */
    config Bool ENABLECLK = true;

    /*! Select the on-device timer to drive the timer services for CLK */
    config String TIMERSELECT;
    
    /*! This property is no longer supported */
    config Bool HIRESTIME;

    /*! This property enables the high-resolution timer. (C64x+ only) */
    config Bool ENABLEHTIME = true;

    /*! The number of microseconds between interrupts */
    config Float MICROSECONDS = 1000;

    /*! This property is no longer supported */
    config Bool CONFIGURETIMER;

    /*! This property is no longer supported */
    config Int16 PRD;

    /*! Selects the high-resolution timer */
    config String WHICHHIRESTIMER = null;

    /*! This property is no longer supported */
    config Bool SPECIFYRATE;

    /*! This property is no longer supported */
    config Float INPUTCLK;

    /*!
     *  Allow DSP/BIOS to initialize the timer to the mode specified by TIMMODE
     *  and release the timer from reset. (C64x+ only)
     */
    config Bool RESETTIMER;

    /*! This property specifies the timer mode. (C64x+ only) */
    config String TIMMODE = "32-bit unchained";

    /*! This property is no longer supported */
    config Bool FIXTDDR;
    
    /*! This property is no longer supported */
    config Int TCRTDDR;

    /*! This property is no longer supported */
    config String POSTINITFXN;

    /*! This property is no longer supported */
    config Bool CONONDEBUG;

    /*! This property is no longer supported */
    config Bool STARTBOTH;
    
    /*!
     *  This property sets the GPTimer5 base address.
     *  (C64x+ OMAP devices only)
     */
    config UInt32 TIMER5BASE;

    /*!
     *  This property sets the GPTimer6 base address.
     *  (C64x+ OMAP devices only)
     */
    config UInt32 TIMER6BASE;

    /*!
     *  This property sets the GPTimer7 base address.
     *  (C64x+ OMAP devices only)
     */
    config UInt32 TIMER7BASE;

    /*!
     *  This property sets the GPTimer8 base address.
     *  (C64x+ OMAP devices only)
     */
    config UInt32 TIMER8BASE;

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

    /*! create an instance */
    create(String name);
 
instance:

    /*! Use to save the name of the object */
    config String name = "";

    /*! Type a comment to identify this object */
    config String comment = "<add comments here>";

    /*! The function to be executed when the timer interrupt occurs */
    config String fxn;

    /*! This property is no longer supported */
    config Int16 order;
    
    /*!
     *  ======== destroy ========
     *  @_nodoc
     *  function is just a place holder for now.
     */
    Void destroy();
}

