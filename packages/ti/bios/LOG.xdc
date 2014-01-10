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
 *  ======== LOG.xdc ========
 *  metaonly module to support the DSP/BIOS legacy LOG module.
 *
 */

/*!
 *  ======== LOG ========
 *  LOG module
 *
 *  legacy module to support DSP/BIOS 5.x, see DSP/BIOS 5.x docs for more info
 */

metaonly module LOG
{
    /*! The memory segment that contains the LOG objects */
    config MEM.Instance OBJMEMSEG = null;

    /*! If set to true, it will allow LOGs to be flushed */
    config Bool enableFlush = false;
    
    /*! This property is no longer supported */
    config Bool TS;

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

    config MEM.Instance bufSeg = null;
    
    /*! The length of the log buffer (in words) */
    config UInt bufLen = 64;

    /*! The type of the log */
    config String logType = "circular";
    
    /*! This property is no longer supported */
    config String dataType;

    /*! This property is no longer supported */
    config String format;

    /*! Flush buffer on exit */
    config Bool exitFlush = false;

    /*!
     *  ======== destroy ========
     *  @_nodoc
     *  function is just a place holder for now.
     */
    Void destroy();
}

