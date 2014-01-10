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
 *  ======== log.c ========
 */

#include <stdarg.h>

#include <xdc/std.h>
#include <xdc/runtime/Main.h>
#include <xdc/runtime/Diags.h>

#include <ti/bios/include/log.h>

/* 
 *  ======== LOG_printf ========
 */
Void LOG_printf(LOG_Handle handle, String format, ...)
{
    IArg arg0, arg1, arg2;
    va_list va;

    va_start(va, format);

    arg0 = va_arg(va, IArg);
    arg1 = va_arg(va, IArg);
    arg2 = va_arg(va, IArg);

    /*
     *  Call LoggerBuf_write4 directly, passing
     *    - The handle
     *    - The log event. This is a printf so the event id is 0.
     *      Just use Diags_INFO for the event mask.
     *    - The module id, use xdc.runtime.Main.
     *    - The format string and the arguments.
     */
    xdc_runtime_LoggerBuf_write4(
        (LoggerBuf_Handle) handle,
        xdc_runtime_Diags_INFO,
        xdc_runtime_Main_Module_id(),
        (IArg) format, arg0, arg1, arg2);

    va_end(va);
}

/* 
 *  ======== LOG_printf4 ========
 */
Void LOG_printf4(LOG_Handle handle, String format, ...)
{
    IArg arg0, arg1, arg2, arg3;
    va_list va;

    va_start(va, format);

    arg0 = va_arg(va, IArg);
    arg1 = va_arg(va, IArg);
    arg2 = va_arg(va, IArg);
    arg3 = va_arg(va, IArg);

    /*
     *  Call LoggerBuf_write4 directly, passing
     *    - The handle
     *    - The log event. This is a printf so the event id is 0.
     *      Just use Diags_INFO for the event mask.
     *    - The module id, use xdc.runtime.Main.
     *    - The format string and the arguments.
     */    
    xdc_runtime_LoggerBuf_write8(
        (LoggerBuf_Handle) handle,
        xdc_runtime_Diags_INFO,
        xdc_runtime_Main_Module_id(),
        (IArg) format, arg0, arg1, arg2, arg3, (IArg) 0, (IArg) 0, (IArg) 0);

    va_end(va);
}


