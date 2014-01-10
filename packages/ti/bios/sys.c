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
 *  ======== sys.c ========
 */

#include <xdc/std.h>

#include <xdc/runtime/System.h>

#include <ti/bios/include/sys.h>
#include <ti/bios/support/Sys.h>

#include <stdarg.h>

/*
 *  ======== SYS_atexit ========
 *  Use System atexit handling feature.
 */
Bool SYS_atexit(Fxn fxn)
{
    return (System_atexit((System_AtexitHandler)fxn));
}

/*
 *  ======== SYS_error ========
 *  Call the support module to manage the error.
 */
Void SYS_error(String fmt, Int errno, ...)
{
    va_list va;

    va_start(va, errno);
    Sys_error(fmt, errno, va);
    va_end(va);
}

/*
 *  ======== SYS_exit ========
 */
Void SYS_exit(Int status)
{
    Sys_exit(status);
}

/*
 *  ======== SYS_printf ========
 */
Void SYS_printf(String fmt, ...)
{
    va_list va;

    va_start(va, fmt);
    System_vprintf(fmt, va);
    va_end(va);
}

/*
 *  ======== SYS_putchar ========
 */
Void SYS_putchar(Char c)
{
    System_putch(c);
}

/*
 *  ======== SYS_sprintf ========
 */
Void SYS_sprintf(Char *buf, String fmt, ...)
{
    va_list va;

    va_start(va, fmt);
    System_vsprintf(buf, fmt, va);
    va_end(va);
}

/*
 *  ======== SYS_vprintf ========
 */
Void SYS_vprintf(String fmt, va_list va)
{
    System_vprintf(fmt, va);
}

/*
 *  ======== SYS_vsprintf ========
 */
Void SYS_vsprintf(Char *buf, String fmt, va_list va)
{
    System_vsprintf(buf, fmt, va);
}

/*
 *  ======== SYS_nop ========
 */
Void SYS_nop(void)
{
    return;
}

/*
 *  ======== SYS_one ========
 */
Int SYS_one(void)
{
    return (1);
}

/*
 *  ======== SYS_zero ========
 */
Int SYS_zero(void)
{
    return (0);
}
