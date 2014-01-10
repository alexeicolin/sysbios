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
 *  ======== trg.h ========
 *
 *  Target processor dependencies:
 *
 */

#ifndef TRG_
#define TRG_

#include <stddef.h>
/*
 *  TaRGet processor attributes include:
 *
 *  TRG_MAUBITS         Native MAU size in BITS
 *  TRG_WORDBITS        Native WORD size in BITS
 *  TRG_LONGBITS        Native LONG size in BITS
 *
 *  TRG_WORDMAUS        Native WORD size in MAUS
 *
 *  TRG_StackEntry      Stack data type
 *  TRG_STACKGROWTH     -1 if stack grows toward lower memory
 *                      +1 if stack grows toward higher memory
 *  TRG_STACKSTAMP      Distinguished stack element for TSK_stat
 *  TRG_STACKALIGN      In sizeof() operator units (see mem_alloc.c)
 *
 */

#include <ti/bios/include/fxn.h>

#if defined(_54_)
#define TRG_MAUBITS             16
#define TRG_WORDBITS            16
#define TRG_LONGBITS            32
#define TRG_WORDMAUS            1
#define TRG_STACKGROWTH         -1
#define TRG_STACKSTAMP          0xBEEF
#define TRG_STACKALIGN          1

#elif defined(_55_)
#define TRG_MAUBITS             16
#define TRG_WORDBITS            16
#define TRG_LONGBITS            32
#define TRG_WORDMAUS            1
#define TRG_STACKGROWTH         -1
#define TRG_STACKSTAMP          0xBEEF
#define TRG_SYSSTACKSTAMP       0xFEEB 
#define TRG_STACKALIGN          2

#elif defined(_6x_)
#include <ti/bios/include/hwi.h>
#define TRG_MAUBITS             8
#define TRG_WORDBITS            32
#define TRG_LONGBITS            32      /* Note! sizeof(long) == 5 not 4! */
#define TRG_WORDMAUS            4
#define TRG_STACKGROWTH         -1
#define TRG_STACKSTAMP          0xBE
#define TRG_STACKALIGN          8

#elif defined(_28_)
#define TRG_MAUBITS             16
#define TRG_WORDBITS            16
#define TRG_LONGBITS            32      /* Note! sizeof(long) == 5 not 4! */
#define TRG_WORDMAUS            1
#define TRG_STACKGROWTH         1
#define TRG_STACKSTAMP          0xBEEF
#define TRG_STACKALIGN          2

#elif defined(_SUN_)
#define TRG_MAUBITS             8
#define TRG_WORDBITS            32
#define TRG_LONGBITS            32
#define TRG_WORDMAUS            4
#define TRG_STACKGROWTH         -1              /* ?? */
#define TRG_STACKSTAMP          0xBE
#define TRG_STACKALIGN          1

#else
#error Unsupport target processor
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef Uns     TRG_IntState;
#ifdef _55_
typedef Uns     TRG_StackEntry;
#else
typedef unsigned char   TRG_StackEntry;
#endif
typedef TRG_StackEntry  *TRG_StackPtr;
 
/*
 *  ======== TRG_init ========
 */
#define TRG_init        FXN_F_nop

/*
 *  ======== TRG_stkbottom ========
 *  Returns the bottom of a stack given the base address and size.
 *  (dependent on whether stacks grow up or down)
 */
#if TRG_STACKGROWTH < 0

#define TRG_stkbottom(base, size) \
        ( (Ptr)(((Char *)(base)) + ((size) - TRG_STACKALIGN)) )

#else   /* TRG_STACKGROWTH < 0 */

#define TRG_stkbottom(base, size)       (base)

#endif  /* TRG_STACKGROWTH < 0 */

/*
 *  ======== TRG_stkok ========
 */
#if TRG_STACKGROWTH < 0

#define TRG_stkok(base, size) ( *(TRG_StackPtr)base == TRG_STACKSTAMP )

#else

#define TRG_stkok(base, size) \
    ( *((TRG_StackPtr)base + size - 1) == TRG_STACKSTAMP )

#endif

#if defined(_55_)
#define TRG_sysstkok(base, size) ( *(TRG_StackPtr)base == TRG_SYSSTACKSTAMP )
#endif

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* TRG_ */
