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
 *  ======== dtr.h ========
 *  Transformer Device Module Interface
 *
 */

#ifndef DTR_
#define DTR_

#include <stddef.h>
#include <ti/bios/include/std.h>

#include <ti/bios/include/dev.h>
#include <ti/bios/include/fxn.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _FLOAT_
typedef Float   DTR_Scale;
#else  /* _FLOAT_ */
typedef Int     DTR_Scale;
#endif /* _FLOAT_ */

#ifndef _FLOAT_
typedef Int16   DTR_ScaleInt16;
typedef DTR_ScaleInt16  DTR_ElemInt16;
extern Void DTR_multiplyInt16(Arg arg, DTR_ElemInt16 *addr, size_t size);
#define DTR_SCALE_INT16 (Int)DTR_multiplyInt16  /* float = float * arg */
#endif

typedef DTR_Scale       DTR_Elem;

#define DTR_init        ((Fxn)FXN_F_nop)

extern __FAR__ DEV_Fxns DTR_FXNS;

#ifdef _FLOAT_
extern Void DTR_fix2flt(Arg arg, DTR_Elem *addr, size_t size);
extern Void DTR_flt2fix(Arg arg, DTR_Elem *addr, size_t size);
#endif /* _FLOAT_ */
extern Void DTR_multiply(Arg arg, DTR_Elem *addr, size_t size);

#ifdef _FLOAT_
#define DTR_FLOAT       (Int)DTR_fix2flt        /* fix-to-float */
#define DTR_FIX         (Int)DTR_flt2fix        /* float-to-fix */
#endif /* _FLOAT_ */
#define DTR_SCALE       (Int)DTR_multiply       /* float = float * arg */
#define DTR_USER        (Int)NULL               /* user transformation */

/*
 *  ======== DTR_Params ========
 */
typedef struct {        /* device parameters */
    struct {
        DTR_Scale       value;
    } scale;
    struct {
        Arg             arg;
        Fxn             fxn;
    } user;
} DTR_Params;

extern __FAR__ DTR_Params DTR_PARAMS;           /* default parameter values */

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* DTR_ */
