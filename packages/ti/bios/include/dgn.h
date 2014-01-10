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
 *  ======== dgn.h ========
 *  Generator Device Module Interface
 *
 */

#ifndef DGN_
#define DGN_

#include <ti/bios/include/std.h>
#include <stddef.h>
#include <ti/bios/include/dev.h>
#include <ti/bios/include/sys.h>

#ifdef __cplusplus
extern "C" {
#endif

extern __FAR__ DEV_Fxns DGN_FXNS;

/*
 *  ======== DGN_Params ========
 */
typedef struct {                /* device parameters */
    struct {
        Int     value;          /* constant value */
    } constant;
    struct {
        LgUns   seed;           /* seed value */
        Int     lower;          /* lower limit of generator */
        Int     upper;          /* upper limit of generator */
    } rand;
    struct {
        Int     gain;           /* gain */
        Int     freq;           /* frequency (cycles / sec) */
        Int     phase;          /* phase (in radians) */
        Int     rate;           /* sample rate (samples / sec) */
    } sine;
    struct {
        Fxn     fxn;            /* user function */
        Arg     arg;            /* and argument */
    } user;
} DGN_Params;

typedef DGN_Params DGN_IParams;

extern __FAR__ DGN_Params DGN_PARAMS;          /* default parameter values */

typedef struct DGN_GenObj {     
    Fxn         fxn;
    Arg         arg;
    LgUns       seed;           /* current seed value */
    Int         index;          /* current index into sine value table */
    Int         step;           /* the step for indexing the sine table */
    Int         shift;          /* sine shift value to achieve gain */
    DGN_Params  *dparams;       /* parameters */
} DGN_GenObj;

extern Void DGN_iconst(DGN_GenObj *gen, Int16 *addr, size_t size);
extern Void DGN_irand(DGN_GenObj *gen, Int16 *addr,  size_t size);
extern Void DGN_isine(DGN_GenObj *gen, Int16 *addr, size_t size);
extern Void DGN_user(DGN_GenObj *gen, Ptr addr, size_t size);

#ifdef _28_
#define DGN_CONST       (LgInt)DGN_iconst         /* constant series device id */
#define DGN_RAND        (LgInt)DGN_irand          /* random series device id */
#define DGN_SINE        (LgInt)DGN_isine          /* sine wave device id */
#define DGN_USER        (LgInt)DGN_user           /* user function */
#else
#define DGN_CONST       (Int)DGN_iconst         /* constant series device id */
#define DGN_RAND        (Int)DGN_irand          /* random series device id */
#define DGN_SINE        (Int)DGN_isine          /* sine wave device id */
#define DGN_USER        (Int)DGN_user           /* user function */
#endif

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* DGN_ */
