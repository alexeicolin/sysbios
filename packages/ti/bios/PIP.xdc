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
 *  ======== PIP.xdc ========
 *  metaonly module to support the DSP/BIOS legacy PIP module.
 *
 */

/*!
 *  ======== PIP ========
 *  PIP module
 *
 *  legacy module to support DSP/BIOS 5.x, see DSP/BIOS 5.x docs for more info
 */

@Template("./PIP.xdt")
metaonly module PIP
{
    /*! This property is no longer supported */
    config MEM.Instance OBJMEMSEG = null;

    /*!
     *  ======== close ========
     *  @_nodoc
     */
    function close();

    /*! 
     *  ======== create ========
     *  Create a static PIP
     */
    create(String name);

instance:

    /*! @_nodoc */
    config String name = "";

    /*! This property is no longer supported */
    config String comment;

    /*! Segment for PIP buffers */
    config MEM.Instance bufSeg = null;

    /*! Buffer alignment. */
    config UInt bufAlign = 1;

    /*! Frame size in minimum addressable units */
    config UInt frameSize = 0;

    /*! Number of frames */
    config UInt numFrames = 2;

    /*! This property is no longer supported */
    config String monitor;

    /*! Function to execute when a frame of free space is available */
    config String notifyWriterFxn = null;

    /*! Argument 1 for notifyWriterFxn */
    config UArg notifyWriterArg0 = 0;

    /*! Argument 2 for notifyWriterFxn */
    config UArg notifyWriterArg1 = 0;

    /*! Function to notify when a frame of data is available */
    config String notifyReaderFxn = null;

    /*! Argument 1 for notifyReaderFxn */
    config UArg notifyReaderArg0 = 0;

    /*! Argument 2 for notifyReaderFxn */
    config UArg notifyReaderArg1 = 0;

    /*!
     *  ======== destroy ========
     *  @_nodoc
     *  function is just a place holder for now.
     */
    Void destroy();
}
