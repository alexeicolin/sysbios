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
 *  ======== std.h ========
 *
 */

#ifndef ti_bios_include_std__include
#define ti_bios_include_std__include

#ifndef xdc_target_types__
#define xdc_target_types__ ti/targets/std.h
#endif

#ifdef _TMS320C28X
  #define _28_ 1
  #ifdef LARGE_MODEL
    #ifdef __TMS320C28XX_FPU32__
      #define _28FP_ 1
      #ifndef xdc_target_name__
        #define xdc_target_name__ C28_float
      #endif
    #else
      #define _28L_ 1
      #ifndef xdc_target_name__
        #define xdc_target_name__ C28_large
      #endif
    #endif
  #endif  
#endif

#ifdef _TI_C6X_TESLA
  #define _64T_
  #define _6x_ 1
  #ifndef xdc_target_name__
    #define xdc_target_name__ C64T
  #endif
#endif

#ifdef _TMS320C6600
  #define _66_ 1
  #define _6x_ 1
  #ifndef xdc_target_name__
    #define xdc_target_name__ C66
  #endif
#endif

#ifdef _TMS320C6740
  #define _674_ 1
  #define _6x_ 1
  #ifndef xdc_target_name__
    #define xdc_target_name__ C674
  #endif
#endif

#ifdef _TMS320C6400_PLUS
  #define _64P_ 1
  #define _6x_ 1
  #ifndef xdc_target_name__
    #define xdc_target_name__ C64P
  #endif
#endif

#ifdef _TMS320C6400
  #define _64_ 1
  #define _6x_ 1
  #ifndef xdc_target_name__
    #define xdc_target_name__ C64
  #endif
#endif

#ifdef _TMS320C6700_PLUS
  #define _67P_ 1
  #define _6x_ 1
  #ifndef xdc_target_name__
    #define xdc_target_name__ C67P
  #endif
#endif

#ifdef _TMS320C6700
  #define _67_ 1
  #define _6x_ 1
  #ifndef xdc_target_name__
    #define xdc_target_name__ C67
  #endif
#endif

#ifdef _TMS320C6200
  #define _62_ 1
  #define _6x_ 1
  #ifndef xdc_target_name__
    #define xdc_target_name__ C62
  #endif
#endif

#include <xdc/std.h>

/*
 * legacy symbols and types *not* defined in xdc/std.h ...
 */

/*
 *  ======== _TI_ ========
 *  _TI_ is defined for all TI targets
 */
#if defined (_6x_) || defined (_28_)
#define _TI_    1
#endif

/*
 *  ======== _FLOAT_ ========
 *  _FLOAT_ is defined for all targets that natively support floating point
 */
#if defined(_28FP_)||  defined(_67_) || defined(_67P_) || defined(_674_)
#define _FLOAT_ 1
#endif

/*
 *  ======== _FIXED_ ========
 *  _FIXED_ is defined for all fixed point target architectures
 */
#if defined (_62_) || defined(_64_) || defined(_64P_) || defined (_28L_) || defined (_64T_)
#define _FIXED_ 1
#endif


/*
 *  8, 16, 32-bit type definitions
 *
 *  Sm* - 8-bit type
 *  Md* - 16-bit type
 *  Lg* - 32-bit type
 *
 *  *Int - signed type
 *  *Uns - unsigned type
 *  *Bits - unsigned type (bit-maps)
 */
typedef char SmInt;             /* SMSIZE-bit signed integer */
typedef short MdInt;            /* MDSIZE-bit signed integer */
#if defined(_6x_)
typedef int LgInt;              /* LGSIZE-bit signed integer */
#else
typedef long LgInt;             /* LGSIZE-bit signed integer */
#endif

typedef unsigned char SmUns;    /* SMSIZE-bit unsigned integer */
typedef unsigned short MdUns;   /* MDSIZE-bit unsigned integer */
#if defined(_6x_)
typedef unsigned LgUns;         /* LGSIZE-bit unsigned integer */
#else
typedef unsigned long LgUns;    /* LGSIZE-bit unsigned integer */
#endif

typedef unsigned char SmBits;   /* SMSIZE-bit bit string */
typedef unsigned short MdBits;  /* MDSIZE-bit bit string */
#if defined(_6x_)
typedef unsigned LgBits;        /* LGSIZE-bit bit string */
#else
typedef unsigned long LgBits;   /* LGSIZE-bit bit string */
#endif

typedef SmBits Byte;            /* smallest unit of addressable store */

#ifndef NULL
#define NULL 0
#endif

#ifndef TRUE
#define FALSE ((Bool)0)
#define TRUE  ((Bool)1)
#endif

/*
 * These macros are used to cast 'Arg' types to 'Int' or 'Ptr'.
 * These macros were added for the 55x since Arg is not the same
 * size as Int and Ptr in 55x large model.
 */
#if defined(_28_)
#define ArgToInt(A)     ((Int)((long)(A) & 0xffff))
#define ArgToPtr(A)     ((Ptr)(A))
#else
#define ArgToInt(A)     ((Int)(A))
#define ArgToPtr(A)     ((Ptr)(A))
#endif

#endif /* ti_bios_include_std__include */
