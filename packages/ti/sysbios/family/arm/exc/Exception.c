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
 *  ======== Exception.c ========
 */

#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Startup.h>
#include <xdc/runtime/System.h>

#define ti_sysbios_knl_Task__internalaccess
#include <ti/sysbios/knl/Task.h>

#include <ti/sysbios/knl/Swi.h>
#include <ti/sysbios/BIOS.h>

#include "package/internal/Exception.xdc.h"

__extern __FAR__ UInt8 __STACK_SIZE;
__extern __FAR__ UInt8 __TI_STACK_BASE;
#define STACK_BASE &__TI_STACK_BASE

/*
 *  ======== Exception_Module_startup ========
 *  must initialize IRQ, FIQ, (and SWI?) SPs (R13s)
 */
Int Exception_Module_startup (Int startupPhase)
{
    /* set abort and undefined exception stacks */
    Exception_init();

    return Startup_DONE;
}

/*
 *  ======== Exception_excHandler ========
 */
Void Exception_excHandler(UInt *excStack, UInt pc)
{
    Exception_ExcContext excContext, *excContextp;
    SizeT stackSize = 0;
    UInt8 *stack = NULL;

    Exception_module->excActive = TRUE;

    if (Exception_module->excContext == NULL) {
        Exception_module->excContext = &excContext;
        excContextp = &excContext;
    }
    else {
        excContextp = Exception_module->excContext;
    }

    /* copy registers from stack to excContext */
    excContextp->r0  = (Ptr)excStack[4];        /* r0 */
    excContextp->r1  = (Ptr)excStack[5];        /* r1 */   
    excContextp->r2  = (Ptr)excStack[6];        /* r2 */   
    excContextp->r3  = (Ptr)excStack[7];        /* r3 */   
    excContextp->r4  = (Ptr)excStack[8];        /* r4 */   
    excContextp->r5  = (Ptr)excStack[9];        /* r5 */   
    excContextp->r6  = (Ptr)excStack[10];       /* r6 */   
    excContextp->r7  = (Ptr)excStack[11];       /* r7 */   
    excContextp->r8  = (Ptr)excStack[12];       /* r8 */   
    excContextp->r9  = (Ptr)excStack[13];       /* r9 */     
    excContextp->r10 = (Ptr)excStack[14];       /* r10 */     
    excContextp->r11 = (Ptr)excStack[15];       /* r11 */     
    excContextp->r12 = (Ptr)excStack[16];       /* r12 */    
    excContextp->sp  = (Ptr)excStack[1];        /* sp */     
    excContextp->lr  = (Ptr)excStack[2];        /* lr */    
    excContextp->pc  = (Ptr)pc;         /* pc */
    excContextp->psr = (Ptr)excStack[0];        /* psr */

    excContextp->type = (Exception_Type)(excStack[3] &0x1f);    /* psr */

    excContextp->threadType = BIOS_getThreadType();

    switch (excContextp->threadType) {
        case BIOS_ThreadType_Task: {
            if (BIOS_taskEnabled == TRUE) {
                excContextp->threadHandle = (Ptr)Task_self();
                stack = (UInt8 *)(Task_self())->stack;
                stackSize = (Task_self())->stackSize;
            }
            break;
        }
        case BIOS_ThreadType_Swi: {
            if (BIOS_swiEnabled == TRUE) {
                excContextp->threadHandle = (Ptr)Swi_self();
                stack = STACK_BASE;
                stackSize = (SizeT)(&__STACK_SIZE);
            }
            break;
        }
        case BIOS_ThreadType_Hwi: {
                excContextp->threadHandle = NULL;
                stack = STACK_BASE;
                stackSize = (SizeT)(&__STACK_SIZE);
            break;
        }
        case BIOS_ThreadType_Main: {
                excContextp->threadHandle = NULL;
                stack = STACK_BASE;
                stackSize = (SizeT)(&__STACK_SIZE);
            break;
        }
    }

    excContextp->threadStackSize = stackSize;
    excContextp->threadStack = (Ptr)stack;

    /* copy thread's stack contents if user has provided a buffer */
    if (Exception_module->excStackBuffer != NULL) {
        UInt8 *from, *to;
        from = stack;
        to = (UInt8 *)Exception_module->excStackBuffer;
        while (stackSize--) {
            *to++ = *from++;
        }
    }

    /* Force MAIN threadtype So we can safely call System_printf */

    BIOS_setThreadType(BIOS_ThreadType_Main);

    if (Exception_enableDecode == TRUE) {
        Exception_excDumpContext(pc);
    }

    /* Call user's exception hook */
    if (Exception_excHookFunc != NULL) {
        Exception_excHookFunc(excContextp);
    }

    /* raise a corresponding Error */
    switch(excContextp->type) {

        case Exception_Type_Supervisor: 
            Error_raise(0, Exception_E_swi, pc, excStack[2]);
            break;

        case Exception_Type_PreAbort: 
            Error_raise(0, Exception_E_prefetchAbort, pc, excStack[2]);
            break;

        case Exception_Type_DataAbort: 
            Error_raise(0, Exception_E_dataAbort, pc, excStack[2]);
            break;

        case Exception_Type_UndefInst: 
            Error_raise(0, Exception_E_undefinedInstruction, pc, excStack[2]);
            break;
    }
}

/*
 *  ======== Exception_excDumpContext ========
 */
Void Exception_excDumpContext(UInt pc)
{
    Exception_ExcContext *excp;
    Char *ttype;

    excp = Exception_module->excContext;

    switch (excp->threadType) {
        case BIOS_ThreadType_Task: {
            ttype = "Task";
            break;
        }
        case BIOS_ThreadType_Swi: {
            ttype = "Swi";
            break;
        }
        case BIOS_ThreadType_Hwi: {
            ttype = "Hwi";
            break;
        }
        case BIOS_ThreadType_Main: {
            ttype = "Main";
            break;
        }
    }

    System_printf("Exception occurred in ThreadType_%s.\n", ttype);
    System_printf("%s handle: 0x%x.\n", ttype, excp->threadHandle);
    System_printf("%s stack base: 0x%x.\n", ttype, excp->threadStack);
    System_printf("%s stack size: 0x%x.\n", ttype, excp->threadStackSize);

    System_printf ("R0 = 0x%08x  R8  = 0x%08x\n", excp->r0, excp->r8);
    System_printf ("R1 = 0x%08x  R9  = 0x%08x\n", excp->r1, excp->r9);
    System_printf ("R2 = 0x%08x  R10 = 0x%08x\n", excp->r2, excp->r10);
    System_printf ("R3 = 0x%08x  R11 = 0x%08x\n", excp->r3, excp->r11);
    System_printf ("R4 = 0x%08x  R12 = 0x%08x\n", excp->r4, excp->r12);
    System_printf ("R5 = 0x%08x  SP(R13) = 0x%08x\n", excp->r5, excp->sp);
    System_printf ("R6 = 0x%08x  LR(R14) = 0x%08x\n", excp->r6, excp->lr);
    System_printf ("R7 = 0x%08x  PC(R15) = 0x%08x\n", excp->r7, excp->pc);
    System_printf ("PSR = 0x%08x\n", excp->psr);
} 


