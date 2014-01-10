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
 *  ======== SYS.xdc ========
 *  metaonly module to support the DSP/BIOS legacy SYS module.
 *
 */

/*!
 *  ======== SYS ========
 *  SYS module
 *
 *  legacy module to support DSP/BIOS 5.x, see DSP/BIOS 5.x docs for more info
 */

metaonly module SYS
{
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
    
    /*! 
     *  ======== TRACESIZE ========
     *  The size of the buffer that contains system trace information.
     *
     *  If xdc.runtime.SysMin is the System SupportProxy, TRACESIZE overrides
     *  the bufSize configuration.
     */
    config Int TRACESIZE;

    /*! This property is no longer supported */
    config MEM.Instance TRACESEG;
    
    /*! The function to run if the application aborts by calling SYS_abort */
    config String ABORTFXN = null;

    /*! The function to run if an error flagged by SYS_error occurs */
    config String ERRORFXN = null;

    /*! The function to run when the application exits by calling SYS_exit */
    config String EXITFXN = null;

    /*! This property is no longer supported */
    config String PUTCFXN = null;
}

