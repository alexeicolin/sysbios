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
 *  ======== Sys.xdc ========
 *
 */

package ti.bios.support;

/*!
 *  ======== Sys ========
 *  @_nodoc
 *  Sys Support Module
 *
 */
module Sys
{
    /*! Legacy error Id */
    config xdc.runtime.Error.Id E_error  =
        {msg: "SYS_error called: error id = 0x%x"};

    /*! Typedef of the error function */
    typedef Void (*ErrorFxn)(String, Int, va_list);
    
    /*! Typedef of the abort function (abortFxn) */
    typedef Void (*AbortFxn)(String, va_list);

    /*! Typedef of the exit function (exitFxn) */
    typedef Void (*ExitFxn)(Int);
    
    /*! The function to run if an error flagged by SYS_error occurs */
    config ErrorFxn errorFxn = null;

    /*! The function to run if an error flagged by SYS_abort occurs */
    config AbortFxn abortFxn = null;

    /*! The function to run if an error flagged by SYS_exit occurs */
    config ExitFxn exitFxn = null;
    
    /*! function to log SYS_errors */
    Void error(String error, Int errno, va_list va);
    
    /*! function to log SYS_abort */
    Void abort(String str, va_list va);
    
    /*! function to log SYS_errors */
    Void exit(Int stat);    
}
