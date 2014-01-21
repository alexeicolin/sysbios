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
 *  ======== ReentSupport.xs ========
 */

var BIOS = null;
var Task = null;
var Semaphore = null;
var ReentSupport = null;

/*
 *  ======== module$use ========
 */
function module$use()
{
    ReentSupport = this;
    
    BIOS = xdc.useModule("ti.sysbios.BIOS");
    Semaphore = xdc.useModule("ti.sysbios.knl.Semaphore");

    if (BIOS.taskEnabled == true) {
        Task = xdc.useModule("ti.sysbios.knl.Task");
    }

}

/*
 *  ======== module$static$init ========
 */
function module$static$init(mod, params)
{
    var semParams;

    if ((BIOS.taskEnabled == true) &&
        (ReentSupport.enableReentSupport == true)) {
        Task.addHookSet({
            registerFxn: ReentSupport.taskRegHook,
            createFxn: null,
            readyFxn:  null,
            switchFxn: null,
            exitFxn: null,
            deleteFxn: ReentSupport.taskDeleteHook
        });

        /*
         * Create a binary semaphore. Used to protect libc
         * lock acquire/release APIs.
         */
        semParams = new Semaphore.Params();
        mod.lock = Semaphore.create(1, semParams);
    }

    mod.taskHId = 0;
}

/*
 *  ======== module$validate ========
 *  some redundant tests are here to catch changes since
 *  module$static$init()
 */
function module$validate()
{
    if (!Program.build.target.$name.match(/gnu/)) {
        ReentSupport.$logError("This module does not support non-GNU targets.",
            ReentSupport);
    }

    if ((ReentSupport.enableReentSupport == true) &&
        (BIOS.taskEnabled == false)) {
        ReentSupport.$logWarning("Reentrancy support has been enabled for "
            + "library calls from Task threads, but BIOS.taskEnabled is false.",
            ReentSupport, "enableReentSupport");
    } 
}

/*
 *  ======== viewInitModule ========
 *  Initialize the ReentSupport 'Module' view.
 */
function viewInitModule(view, obj)
{
    var modCfg = Program.getModuleConfig("ti.sysbios.rts.gnu.ReentSupport");

    view.enableReentSupport = modCfg.enableReentSupport;
}
