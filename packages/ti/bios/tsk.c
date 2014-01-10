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
 *  ======== tsk.c ========
 */

#include <xdc/std.h>

#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/IHeap.h>

#define ti_sysbios_knl_Task__internalaccess
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/hal/Hwi.h>

#include <ti/bios/include/std.h>
#include <ti/bios/include/mem.h>
#include <ti/bios/include/tsk.h>


/*
 *  ======== dynamicGlue ========
 *  used to create dynamically created tasks
 */
static Void dynamicGlue(UArg arg0)
{
    TSK_Glue    *glue = ((TSK_Glue *)arg0);
    Fxn         fxn = glue->fxn;
    Arg         *args = glue->args;
    Task_Handle tsk;

    tsk = Task_self();
    tsk->fxn = (ti_sysbios_knl_Task_FuncPtr)fxn;

    fxn(args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7]);
}

/*
 *  ======== TSK_checkstacks ========
 */
Void TSK_checkstacks(TSK_Handle oldtask, TSK_Handle newtask)
{
    Task_checkStacks((Task_Handle)oldtask, (Task_Handle)newtask);
}

/*
 *  ======== TSK_create ========
 */
TSK_Handle TSK_create(Fxn fxn, TSK_Attrs *attrs, ...)
{
    Int                 i;
    va_list             arglist;
    TSK_Handle          task;
    TSK_Glue            *glue;
    Task_Params         params;
    Error_Block         eb;


    Task_Params_init(&params);

    /* Use segid 0. No alignment needed */
    glue = MEM_alloc(0, sizeof(TSK_Glue), 0);

    if (glue == NULL) {
        return (NULL);
    }

    if (attrs == NULL) {
        attrs = &TSK_ATTRS;
    }

    params.instance->name      = attrs->name;
    params.priority  = attrs->priority;
    params.stack     = attrs->stack;
    params.stackSize = attrs->stacksize;
    params.stackHeap = MEM_getHandle(attrs->stackseg);
    params.env       = attrs->environ;
    params.vitalTaskFlag  = attrs->exitflag;

    params.arg0 = (UArg)glue;

    /*
     * Encode arg1 with the fxn for ROV to use prior to task running
     */
    params.arg1 = (UArg)fxn;

    /* copy args from stack into argument buffer */
    va_start(arglist, attrs);
    for (i = 0; i < TSK_MAXARGS; i++) {
        glue->args[i] = (Arg)va_arg(arglist, UArg);
    }
    va_end(arglist);

    glue->fxn = fxn;
    glue->errnum = 0;

    Error_init(&eb);
    task = (TSK_Handle)Task_create((Task_FuncPtr)dynamicGlue, &params, &eb);

    if (task == NULL) {
        /* Use segid 0 */
        MEM_free(0, glue, sizeof(TSK_Glue));
    }

    return (task);
}

/*
 *  ======== TSK_delete ========
 */
Void TSK_delete(TSK_Handle task)
{
    TSK_Glue    *glue;
    Task_Handle handle = (Task_Handle)task;

    /* clean up the Glue structure before deleting task */
    glue = (TSK_Glue *)Task_getArg0(handle);

    /* Use segid 0 */
    MEM_free(0, glue, sizeof(TSK_Glue));

    Task_delete(&handle);
}

/*
 *  ======== TSK_geterr ========
 */
Int TSK_geterr(TSK_Handle task)
{
    TSK_Glue    *glue;

    glue = (TSK_Glue *)Task_getArg0((Task_Handle)task);

    return (glue->errnum);
}

/*
 *  ======== TSK_seterr ========
 */
Void TSK_seterr(TSK_Handle task, Int err)
{
    TSK_Glue    *glue;

    glue = (TSK_Glue *)Task_getArg0((Task_Handle)task);

    glue->errnum = err;
}

/*
 *  ======== TSK_getname ========
 */
String TSK_getname(TSK_Handle task)
{
    return (Task_Handle_name((Task_Handle)task));
}

/*
 *  ======== TSK_stat ========
 */
Void TSK_stat(TSK_Handle task, TSK_Stat *newStatBuf)
{
    Task_Stat statBuf;

    Task_stat((Task_Handle)task, &statBuf);

    newStatBuf->attrs.priority = statBuf.priority;
    newStatBuf->attrs.stack = statBuf.stack;
    newStatBuf->attrs.stacksize = statBuf.stackSize;
    newStatBuf->attrs.environ = statBuf.env;

    newStatBuf->attrs.name = Task_Handle_name((Task_Handle)task);

    newStatBuf->mode = statBuf.mode;
    newStatBuf->sp = statBuf.sp;
    newStatBuf->used = statBuf.used;

    newStatBuf->attrs.stackseg = MEM_getSegId(statBuf.stackHeap);
}

static __FAR__ UInt nest = 0;
static __FAR__ UInt tskKey = 0;

/*
 * ======== TSK_disable ========
 *
 * Calling constraints:
 *    can not be called from SWI or HWI
 * Calling non-constraints:
 *    CAN be called with interrupts disabled
 *
 * The sequence of operations must be atomic.  We were previously doing
 * a test-and-set atomic operation on the 'nest' variable followed by a
 * KNL_enter().  The atomic operation actually needs to be
 *      "test-and-set-and-KNL_enter-if-0"
 * since if a higher priority task preempts the task calling TSK_disable()
 * after the 'nest++' but before the KNL_enter, and it calls TSK_disable(),
 * it will detect 'nest' as non-zero, not do the KNL_enter(), and proceed
 * into the critical section with TSKs enabled (not a good thing).
 */
Void TSK_disable(void)
{
    UInt key;
    UInt hwiKey;

    hwiKey = Hwi_disable();

    key = Task_disable();

    if (nest == 0) {
        tskKey = key;
    }
    nest++;

    Hwi_restore(hwiKey);
}

/*
 * ======== TSK_enable ========
 *
 * Calling constraints:
 *    can not be called from SWI or HWI
 *    can NOT be called with interrupts disabled (due to KNL_exit())
 *
 * There's no need for atomicity here as there is for TSK_disable().
 * Since this function can't be called from HWI or SWI, and since
 * TSK_disable() must be called previously, the TSK scheduler is
 * disabled for the duration of this function (until KNL_exit() returns),
 * and therefore no other thread can be inside this function simultaneously.
 */
Void TSK_enable(void)
{
    nest--;

    if (nest == 0) {
        Task_restore(tskKey);
    }
}
