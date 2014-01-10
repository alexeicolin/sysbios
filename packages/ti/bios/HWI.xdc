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
 *  ======== HWI.xdc ========
 *  metaonly module to support the DSP/BIOS legacy HWI module.
 *
 */

/*!
 *  ======== HWI ========
 *  HWI module
 *
 *  legacy module to support DSP/BIOS 5.x, see DSP/BIOS 5.x docs for more info
 */

@Template("./HWI.xdt")
metaonly module HWI
{
    /*! This property is currently not supported */
    config Bool RESETVECTOR;

    /*! This property is currently not supported */
    config UInt RESETVECTORADDR;

    /*! This property is no longer supported */
    config String EXTPIN4POLARITY;

    /*! This property is no longer supported */
    config String EXTPIN5POLARITY;

    /*! This property is no longer supported */
    config String EXTPIN6POLARITY;

    /*! This property is no longer supported */
    config String EXTPIN7POLARITY;

    /*! This property is currently not supported */
    config Bool ENABLEEXC;

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

    /*! function to create an instance */
    create(String name);

instance:

    /*! Specify the name of the object */
    config String name = "";

    /*! Type a comment to identify this object */
    config String comment = "<add comments here>";

    /*! Select the source of the interrupt (Only used on C64xx)*/
    config String interruptSource = null;

    /*! The event number associated with an interrupt source */
    config UInt interruptSelectNumber;

    /*! The interrupt service routine to execute */
    config String fxn = "HWI_unused";

    /*! This property is no longer supported */
    config String monitor;

    /*! This property is no longer supported */
    config UInt addr;

    /*! This property is no longer supported */
    config String dataType;

    /*! This property is no longer supported */
    config String operation;

    /*! Controls whether the HWI dispatcher is used or not */
    config Bool useDispatcher = false;

    /*! This Argument is passed to the function */
    config UArg arg = 0x00000000;

    /*! Specifies which interrupts should be disabled before calling function */
    config String interruptMask = "self";

    /*! A hexadecimal integer bitmask specifying the interrupts to disable */
    config UInt16 interruptBitMask = 0x0000;
}

