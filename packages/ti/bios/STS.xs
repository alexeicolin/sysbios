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
 *  ======== STS.xs ========
 *
 */
 
/*
 *  ======== module$meta$init ========
 *  Stores defaults. Sets up $private.realModule.
 */
function module$meta$init()
{
    /* Only process during "cfg" phase */
    if (xdc.om.$name != "cfg") {
        return;
    }

    var Stats = xdc.module('ti.sysbios.misc.Stats');
    this.$private.bios6xMod = Stats;
}

/*
 *  ======== instance$meta$init ========
 *  Initializes object.name to the name parameter.
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
 *  Create a Stats instance for each STS instance.  Make the STS name a global
 *  symbol.
 */
function close()
{
    var Stats = xdc.module("ti.sysbios.misc.Stats");

    /*
     * Loop through STS instances and create a Stats instance with specified
     * params.  Each instance will be a global symbol.
     */
    var insts = this.$instances;

    for (var i = 0; i < insts.length; i++) {
        Program.symbol[insts[i].name] = Stats.create();
        Program.symbol[insts[i].name].previousVal = insts[i].previousVal;
        Program.symbol[insts[i].name].numA = insts[i].numA;
        Program.symbol[insts[i].name].numB = insts[i].numB;
        Program.symbol[insts[i].name].numC = insts[i].numC;
        Program.symbol[insts[i].name].unitType = insts[i].unitType;
        Program.symbol[insts[i].name].operation = insts[i].operation;
    }
}

/*
 *  ======== exportMod ========
 */
function exportMod()
{
    if (this.$instances.length > 0) {
        Program.exportModule("ti.sysbios.misc.Stats");
    }
}

/*
 *  ======== destroy ========
 */
function destroy()
{
    this.$module.$logError("Please remove the associated create() call, " +
        "the destroy() method is not supported", this);
}
