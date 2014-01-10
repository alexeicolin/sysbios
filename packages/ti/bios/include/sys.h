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
 *  ======== sys.h ========
 *
 */

#ifndef SYS_
#define SYS_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>
#include <ti/bios/include/fxn.h>

#define SYS_FOREVER     ((Uns)-1)       /* used by SEM_pend(), ... */
#define SYS_POLL        ((Uns)0)        /* used by SEM_pend(), ... */

#define SYS_NUMHANDLERS 8
/*
 *  error codes
 */
#define SYS_OK          0               /* no error */
#define SYS_EALLOC      1               /* memory allocation error */
#define SYS_EFREE       2               /* memory free error */
#define SYS_ENODEV      3               /* device driver not found */
#define SYS_EBUSY       4               /* device driver busy */
#define SYS_EINVAL      5               /* invalid parameter for device */
#define SYS_EBADIO      6               /* IO failure */
#define SYS_EMODE       7               /* bad mode for device driver */
#define SYS_EDOMAIN     8               /* domain error */
#define SYS_ETIMEOUT    9               /* timeout error */
#define SYS_EEOF        10              /* end-of-file */
#define SYS_EDEAD       11              /* previously deleted object */
#define SYS_EBADOBJ     12              /* invalid object */
#define SYS_ENOTIMPL    13              /* action not implemented */
#define SYS_ENOTFOUND   14              /* resource not found */

#define SYS_EUSER       256             /* user errors start here */

extern __FAR__ String SYS_errors[];             /* system error strings */

extern Void SYS_abort(String fmt, ...);
extern Bool SYS_atexit(Fxn fxn);
extern Void SYS_error(String s, Int errno, ...);
extern Void SYS_exit(Int status);
extern Void SYS_printf(String fmt, ...);
extern Void SYS_putchar(Char c);
extern Void SYS_sprintf(Char *buf, String fmt, ...);
extern Void SYS_vprintf(String fmt, va_list va);
extern Void SYS_vsprintf(Char *buf, String fmt, va_list va);

extern Int SYS_one(void);
extern Int SYS_zero(void);
extern Void SYS_nop(void);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* SEM_ */
