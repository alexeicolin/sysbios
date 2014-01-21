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
 *  ======== IDL.xs ========
 *
 */

/*
 *  ======== module$meta$init ========
 */
function module$meta$init(name)
{
    /* Only process during "cfg" phase */
    if (xdc.om.$name != "cfg") {
        return;
    }

    this.create("IDL_cpuLoad");
    this.create("LNK_dataPump");
    this.create("RTA_dispatcher");
    
    var Idle = xdc.module('ti.sysbios.knl.Idle');
    this.$private.bios6xMod = Idle;
}

/*
 *  ======== instance$meta$init ========
 *  Initializes instance name to the name parameter.
 */
function instance$meta$init(name)
{
    if (name.indexOf('_') == 0) {
        name = name.substr(1);
    }
    this.name = name;
}

/*
 *  ======== close ========
 *  Add an Idle function for each IDL instance.
 */
function close()
{
    var Idle = xdc.module("ti.sysbios.knl.Idle");

    if (this.AUTOCALCULATE != null) {
        this.$logWarning("bios.IDL.AUTOCALCULATE not supported", this);
    }

    if (this.LOOPINSTCOUNT != null) {
        this.$logWarning("bios.IDL.LOOPINSTCOUNT not supported", this);
    }
    
    /* Loop through each IDL instances and add an Idle function */
    var insts = this.$instances;

    for (var i = 0; i < insts.length; i++) {
        if (insts[i].name != "IDL_cpuLoad" && insts[i].name != "LNK_dataPump"
            && insts[i].name != "RTA_dispatcher") {
            Idle.addFunc("&" + insts[i].fxn);

            if (insts[i].calibration != null) {
                this.$logWarning("'calibration' not supported", insts[i]);
            }
            if (insts[i].order != null) {
                this.$logWarning("'order' not supported", insts[i]);
            }
        }
    }
}

/*
 *  ======== exportMod ========
 */
function exportMod()
{
    Program.exportModule("ti.sysbios.knl.Idle");
}

/*
 *  ======== destroy ========
 */
function destroy()
{
    this.$module.$logError("Please remove the associated create() call, " +
        "the destroy() method is not supported", this);
}
