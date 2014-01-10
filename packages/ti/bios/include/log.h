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
 *  ======== log.h ========
 *
 */

#ifndef LOG_
#define LOG_

#ifdef __cplusplus
extern "C" {
#endif

#define xdc_runtime__LoggerBuf__nolocalnames

#include <xdc/runtime/LoggerBuf.h>

#include <ti/bios/include/trc.h>

typedef xdc_runtime_LoggerBuf_Struct LOG_Obj, *LOG_Handle;

extern __FAR__ LOG_Obj LOG_system;

Void LOG_printf(LOG_Handle handle, String format, ...);
Void LOG_printf4(LOG_Handle handle, String format, ...);

static inline Void LOG_disable(LOG_Handle handle)
{
    xdc_runtime_LoggerBuf_disable((xdc_runtime_LoggerBuf_Handle)handle);
}

static inline Void LOG_enable(LOG_Handle handle)
{
    xdc_runtime_LoggerBuf_enable((xdc_runtime_LoggerBuf_Handle)handle);
}

static inline Void LOG_error(String format, Arg arg0)
{
    xdc_runtime_LoggerBuf_write4((xdc_runtime_LoggerBuf_Handle)&LOG_system,
        NULL, NULL, (IArg)format, (IArg)arg0, (IArg)0, (IArg)0);
}

static inline Void LOG_message(String format, Arg arg0)
{
    xdc_runtime_LoggerBuf_write4((xdc_runtime_LoggerBuf_Handle)&LOG_system,
        NULL, NULL, (IArg)format, (IArg)arg0, (IArg)0, (IArg)0);
}

static inline Void LOG_event(LOG_Handle handle, Arg arg0, Arg arg1, Arg arg2)
{
    xdc_runtime_LoggerBuf_write4((xdc_runtime_LoggerBuf_Handle)handle,
        NULL, NULL, (IArg)arg0, (IArg)arg1, (IArg)arg2, (IArg)0);
}

static inline Void LOG_event5(LOG_Handle handle, Arg arg0, Arg arg1, Arg arg2,
    Arg arg3, Arg arg4)
{
    xdc_runtime_LoggerBuf_write8((xdc_runtime_LoggerBuf_Handle)handle,
                           NULL, NULL, (IArg)arg0, (IArg)arg1, (IArg)arg2,
                           (IArg)arg3, (IArg)arg4, (IArg)0, (IArg)0, (IArg)0);
}

static inline Void LOG_reset(LOG_Handle handle)
{
    xdc_runtime_LoggerBuf_reset((xdc_runtime_LoggerBuf_Handle)handle);
}

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  /* LOG_ */
