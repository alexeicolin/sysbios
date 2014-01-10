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
 *  ======== Lck.xdc ========
 *  metaonly module to support the DSP/BIOS legacy LCK module.
 *
 */

package ti.bios.support;

/*!
 *  ======== Lck ========
 *  @_nodoc
 *  Lock Manager.
 */

@InstanceFinalize       /* destruct semaphore */

module Lck
{

instance:

    /*!
     * ======== create ========
     * Create a lock.
     *
     */
    create();

    /*!
     *  ======== pend ========
     *  Acquire ownership of a resource lock.
     *
     *  @b(returns) TRUE if successful, FALSE if timeout
     *
     *  @param(timeout) returns after this many system clock ticks
     */
    Bool pend(UInt timeout);

    /*!
     *  ======== post ========
     *  Relinquish ownership of a resource lock.
     *
     */
    Void post();


internal:

    /* -------- Internal Structures -------- */
    struct Instance_State {
        ti.sysbios.knl.Semaphore.Object    sem;
        Int                             value;
        ti.sysbios.knl.Task.Handle         owner;
    };
}
