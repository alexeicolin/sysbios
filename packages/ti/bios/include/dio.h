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
 *  ======== dio.h ========
 *
 */

#ifndef DIO_
#define DIO_

#include <ti/bios/include/std.h>

#include <ti/bios/include/dev.h>
#include <ti/bios/include/sem.h>

#include <ti/bios/include/iom.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Function table to be use with dynamically created TSK
 */
extern DEV_Fxns DIO_tskDynamicFxns;

/*
 * Function table to be use with statically created TSK
 */
extern DEV_Fxns DIO_tskStaticFxns;

/*
 * Function table to be use with dynamically created callback thread
 */
extern DEV_Fxns DIO_cbDynamicFxns;

/*
 * Function table to be use with statically created callback thread
 */
extern DEV_Fxns DIO_cbStaticFxns;

/*
 *  ======== DIO_Params ========
 */
typedef struct DIO_Params {
    String      name;           /* name of mini driver */
    Ptr         chanParams;     /* optional mini driver create argument */
} DIO_Params;

/*
 * ======== DIO_Obj ========
 * In TSK version, there will contain a complete and ready semaphore.
 * In callback version, there will be a DEV_Callback object which
 * contains a function and two arguments
 */
typedef struct DIO_Obj {        
    IOM_Fxns    *fxns;                  /* mini-driver function table */
    Ptr         chanp;                  /* mini-driver handle */

    union context {
        struct sems {
            SEM_Handle  complete;       /* tasks wait for I/O here */
            SEM_Handle  ready;          /* used by DIO_ready */
        } sems; 
        DEV_Callback    cb;             /* use by callback version */
    } context;
} DIO_Obj;

typedef DIO_Obj *DIO_Handle;            /* device object handle */

extern Void DIO_init(Void);
extern Int  DIO_ctrl(DEV_Handle device, Uns cmd, Arg arg);
extern Int  DIO_cbIdle(DEV_Handle device, Bool flush);
extern Int  DIO_cbIssue(DEV_Handle device);
extern Bool DIO_cbReady(DEV_Handle device, SEM_Handle sem);
extern Int  DIO_cbReclaim(DEV_Handle device);
extern Void DIO_cbCallback(Ptr devp, DEV_Frame *frame);
extern Int  DIO_tskIdle(DEV_Handle device, Bool flush);
extern Int  DIO_tskIssue(DEV_Handle device);
extern Bool DIO_tskReady(DEV_Handle device, SEM_Handle sem);
extern Int  DIO_tskReclaim(DEV_Handle device);
extern Void DIO_tskCallback(Ptr devp, DEV_Frame *frame);
extern Int  DIO_cbStaticClose(DEV_Handle device);
extern Int  DIO_cbStaticOpen(DEV_Handle device, String name);
extern Int  DIO_cbDynamicClose(DEV_Handle device);
extern Int  DIO_cbDynamicOpen(DEV_Handle device, String name);
extern Int  DIO_tskStaticClose(DEV_Handle device);
extern Int  DIO_tskStaticOpen(DEV_Handle device, String name);
extern Int  DIO_tskDynamicClose(DEV_Handle device);
extern Int  DIO_tskDynamicOpen(DEV_Handle device, String name);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* DIO_ */

