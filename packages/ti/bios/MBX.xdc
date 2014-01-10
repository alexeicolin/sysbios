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
 *  ======== MBX.xdc ========
 *  metaonly module to support the DSP/BIOS legacy MBX module.
 *
 */

/*!
 *  ======== MBX ========
 *  MBX module
 *
 *  legacy module to support DSP/BIOS 5.x, see DSP/BIOS 5.x docs for more info
 */

metaonly module MBX
{
    /*! The memory segment that contains the MBX objects */
    config MEM.Instance OBJMEMSEG = null;

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

    /*! The size (in MADUs) of the messages this mailbox can contain */
    config Int messageSize = 1;

    /*! The number of messages this mailbox can contain 
     *  
     *  Reflects old parameter length.
     */
    config Int messageLength = 1;

    /*! The memory segment to contain the mailbox data buffers */
    config MEM.Instance elementSeg = null;

    /*!
     *  ======== destroy ========
     *  @_nodoc
     *  function is just a place holder for now.
     */
    Void destroy();
}

