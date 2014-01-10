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
 *  ======== Lck.c ========
 *  Implementation of functions specified in Semaphore.xdc.
 */

#include <xdc/std.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Task.h>

#include "package/internal/Lck.xdc.h"

/*
 *  ======== Lck_Instance_init ========
 */
Void Lck_Instance_init(Lck_Object *obj, const Lck_Params *params)
{
    Semaphore_Handle sem;

    sem = Lck_Instance_State_sem(obj);
    Semaphore_construct(Semaphore_struct(sem), 1, NULL);
    obj->value = 0;
    obj->owner = NULL;
}

/*
 *  ======== Lck_Instance_finalize ========
 */
Void Lck_Instance_finalize(Lck_Object *obj )
{
    Semaphore_Handle sem;

    sem = Lck_Instance_State_sem(obj);
    Semaphore_destruct(Semaphore_struct(sem));
}

/*
 *  ======== Lck_pend ========
 */
Bool Lck_pend(Lck_Object *lock, UInt timeout)
{
    Bool retval = TRUE;
    Semaphore_Handle sem;

    if (lock->owner != Task_self()) {
        sem = Lck_Instance_State_sem(lock);
        retval = Semaphore_pend(sem, timeout);
    }

    if (retval) {
        lock->owner = Task_self();
        lock->value++;
    }

    return (retval);
}

/*
 *  ======== Lck_post ========
 */
Void Lck_post(Lck_Object *lock)
{
    Semaphore_Handle sem;

    if (--lock->value == 0) {
        lock->owner = NULL;

        sem = Lck_Instance_State_sem(lock);
        Semaphore_post(sem);
    }
}
