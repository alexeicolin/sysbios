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
 *  ======== pool.h ========
 *
 */

#ifndef POOL_
#define POOL_

#include <stddef.h>

#include <ti/bios/include/fxn.h>
#include <ti/bios/include/sys.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Reserved for denoting an invalid pool Id */
#define POOL_INVALIDID   (Uint16)-1

/* Typedefs for the allocator interface and init functions */
typedef Void  (*POOL_Init)(Void);
typedef Int   (*POOL_Open)(Ptr *object, Ptr params);
typedef Void  (*POOL_Close)(Ptr object);
typedef Int   (*POOL_Alloc)(Ptr object, Ptr *buf, size_t size);
typedef Void  (*POOL_Free)(Ptr object, Ptr buf, size_t size);

/* Not implemented defines for POOL functions */
#define POOL_OPENNOTIMPL    (POOL_Open)SYS_zero
#define POOL_CLOSENOTIMPL   (POOL_Close)FXN_F_nop
#define POOL_ALLOCNOTIMPL   (POOL_Alloc)SYS_one
#define POOL_FREENOTIMPL    (POOL_Free)FXN_F_nop

/* The allocator interface function definition. */
typedef struct POOL_Fxns {
    POOL_Open     open;
    POOL_Close    close;
    POOL_Alloc    alloc;
    POOL_Free     free;
} POOL_Fxns;

/* The allocator object */
typedef struct POOL_Obj {
    POOL_Init     initFxn;  /* Allocator init function             */
    POOL_Fxns    *fxns;     /* Allocator interface functions       */
    Ptr           params;   /* Allocator-specific setup parameters */
    Ptr           object;   /* Allocator-specific object           */
} POOL_Obj, *POOL_Handle;

/* Used in the nop allocator */
extern __FAR__ POOL_Fxns POOL_FXNS;

/* No allocator object definition */
#define POOL_NOENTRY {FXN_F_nop, &POOL_FXNS, NULL, NULL}

/* The Allocator Configuration */
typedef struct POOL_Config {
    POOL_Obj   *allocators;     /* Array of allocators            */
    Uint16      numAllocators;  /* Number of allocators in the array */    
} POOL_Config;

extern Void POOL_init(Void);
extern Void POOL_exit(Void);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* POOL_ */
