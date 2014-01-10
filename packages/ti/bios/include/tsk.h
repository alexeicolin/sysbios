/*
 * Copyright (c) 2013, Texas Instruments Incorporated
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
 *  ======== tsk.h ========
 *
 */

#ifndef TSK_
#define TSK_

#ifdef __cplusplus
extern "C" {
#endif

#define ti_sysbios_knl_Task__nolocalnames
#define ti_sysbios_knl_Clock__nolocalnames
#define ti_sysbios_BIOS__nolocalnames

#include <stdarg.h>
#include <stddef.h>
#include <ti/bios/include/std.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/BIOS.h>
#include <ti/bios/include/prd.h>
#include <ti/bios/include/sem.h>
#include <ti/bios/include/sts.h>
#include <ti/bios/include/sys.h>
#include <ti/bios/include/trg.h>
#include <ti/bios/include/swi.h>

#define TSK_IDLEPRI     (0)
#define TSK_MINPRI      (1)
#define TSK_MAXPRI      (15)
#define TSK_MAXARGS     (8)
#define TSK_STACKSTAMP 0xBEEF

#define TSK_RUNNING    ti_sysbios_knl_Task_Mode_RUNNING
#define TSK_READY      ti_sysbios_knl_Task_Mode_READY
#define TSK_BLOCKED    ti_sysbios_knl_Task_Mode_BLOCKED
#define TSK_TERMINATED ti_sysbios_knl_Task_Mode_TERMINATED

typedef ti_sysbios_knl_Task_Struct TSK_Obj, *TSK_Handle;

typedef Int TSK_Mode;

typedef struct TSK_Attrs {
    Int         priority;       /* task priority */
    Ptr         stack;          /* stack supplied  */
    size_t      stacksize;      /* size of stack */
#ifdef _55_
    size_t      sysstacksize;  /* size of system stack */
#endif
    Int         stackseg;       /* segment to allocate stack from */
    Ptr         environ;        /* environment pointer */
    String      name;           /* printable name */
    Bool        exitflag;       /* FALSE for server tasks */
    Bool        initstackflag;  /* FALSE disables stack initialization */
} TSK_Attrs;

extern __FAR__ TSK_Attrs TSK_ATTRS;
extern __FAR__ TSK_Obj KNL_dummy;

typedef struct TSK_Stat {
    TSK_Attrs   attrs;          /* task attributes */
    TSK_Mode    mode;           /* TSK_RUNNING, TSK_READY, ... */
    Ptr         sp;             /*stack pointer(not valid when TSK_RUNNING!)*/
#ifdef _55_
    Ptr         ssp;            /* System stack pointer only forr 55x*/
                                /* Unused in _55Pb_ */
#endif
    size_t      used;           /* stack used */
#ifdef _55_
    size_t      sysUsed;        /* System stack used             */
#endif
} TSK_Stat;

/*
 * TSK_Glue is used to hold 8 TSK function args (TSK supported 8, Task
 * only supports 2).  It also holds seldom used errno which is supported
 * in Task due to RTSC's superior Error_Block mechanism.
 *
 * It may someday hold STS object (or pointer to same) if we decide to
 * support TSK_settime/resettime/deltatime/getsts APIs.
 */
typedef struct TSK_Glue {
    Fxn         fxn;                    /* TSK task function */
    Arg         args[TSK_MAXARGS];
    Int         errnum;
} TSK_Glue;

extern TSK_Handle TSK_create(Fxn fxn, TSK_Attrs *attrs, ...);
extern Void TSK_delete(TSK_Handle task);
extern Int TSK_geterr(TSK_Handle task);
extern Void TSK_seterr(TSK_Handle task, Int err);
extern Void TSK_stat(TSK_Handle task, TSK_Stat *statbuf);
extern String TSK_getname(TSK_Handle task);
extern Void TSK_checkstacks(TSK_Handle oldtask, TSK_Handle newtask);
extern Void TSK_disable(void);
extern Void TSK_enable(void);

static inline Void TSK_exit(void)
{
    ti_sysbios_knl_Task_exit();
}

static inline Ptr TSK_getenv(TSK_Handle task)
{
    return ti_sysbios_knl_Task_getEnv((ti_sysbios_knl_Task_Handle)task);
}

static inline Int TSK_getpri(TSK_Handle task)
{
    return ti_sysbios_knl_Task_getPri((ti_sysbios_knl_Task_Handle)task);
}

static inline Void TSK_itick()
{
    ti_sysbios_knl_Clock_tick();
}

static inline TSK_Handle TSK_self()
{
    return ((TSK_Handle)ti_sysbios_knl_Task_self());
}

static inline Void TSK_setenv(TSK_Handle task, Ptr env)
{
    ti_sysbios_knl_Task_setEnv((ti_sysbios_knl_Task_Handle)task, env);
}

static inline Int TSK_setpri(TSK_Handle task, Int newpri)
{
    return ti_sysbios_knl_Task_setPri((ti_sysbios_knl_Task_Handle)task, newpri);
}

static inline Void TSK_sleep(Uns nticks)
{
    ti_sysbios_knl_Task_sleep(nticks);
}


static inline Uns TSK_time()
{
    return ti_sysbios_knl_Clock_getTicks();
}

static inline Void TSK_yield(void)
{
    ti_sysbios_knl_Task_yield();
}

static inline Bool TSK_isTSK()
{
    return (ti_sysbios_BIOS_getThreadType() == ti_sysbios_BIOS_ThreadType_Task);
}

/* ARGSUSED */
static inline Void TSK_settime(TSK_Handle task)
{
}

/* ARGSUSED */
static inline Void TSK_deltatime(TSK_Handle task)
{
}

/* ARGSUSED */
static inline Void TSK_resettime(TSK_Handle task)
{
}

/* ARGSUSED */
static inline STS_Handle TSK_getsts(TSK_Handle task)
{
    return (NULL);
}

/* ARGSUSED */
static inline Void TSK_switchFxn(TSK_Handle oldtask, TSK_Handle newtask)
{
}

extern Void TSK_tick();

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* TSK_ */
