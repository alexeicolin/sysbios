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
 *  ======== sys_abort.c ========
 *
 *  Created this file to avoid multiple defined issues with Alchemy
 *  and partial links.
 */

#include <xdc/std.h>

#include <ti/bios/support/Sys.h>

#include <ti/bios/include/sys.h>


/*
 * Simplified from BIOS 5.x since SYS_error() compatibility function does
 * not support args list
 */
__FAR__ String SYS_errors[] = {
    "(SYS_OK)",
    "(SYS_EALLOC)",
    "(SYS_EFREE)",
    "(SYS_ENODEV)",
    "(SYS_EBUSY)",
    "(SYS_EINVAL)",
    "(SYS_EBADIO)",
    "(SYS_EMODE)",
    "(SYS_EDOMAIN)",
    "(SYS_ETIMEOUT)",
    "(SYS_EEOF)",
    "(SYS_EDEAD)",
    "(SYS_EBADOBJ)",
    "(SYS_ENOTIMPL)",
    "(SYS_ENOTFOUND)",
};


/*
 *  ======== SYS_abort ========
 *  Use Sys_abort handling feature.
 */
Void SYS_abort(String fmt, ...)
{
    va_list va;

    va_start(va, fmt);
    Sys_abort(fmt, va);
    va_end(va);
}
