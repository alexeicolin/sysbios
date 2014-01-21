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
 *  ======== SWI.xs ========
 *
 */

 var bios6xDefaults = {};

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

    this.create("PRD_swi");
    
    /* 
     * get hold of 6x module. Make sure legacy module is brought in first
     * scroll away 6xMod defaults that need to be changed
     */

    var Swi = xdc.module('ti.sysbios.knl.Swi');
    var BIOS = xdc.module('ti.sysbios.BIOS');
    this.$private.bios6xMod = Swi;

    if (Swi.$used) {
        this.$logError("This module should be used BEFORE " + 
        Swi.$name, this);
    }
    
    if (BIOS.$used) {
        this.$logError("This module should be used BEFORE " + 
        BIOS.$name, this);
    }
    
    bios6xDefaults["swiEnabled"] = BIOS.swiEnabled;
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
 *  Create a Swi instance for each SWI instance.  Make the SWI instance
 *  name a global symbol. This function is called in close() of package.xs
 */
function close()
{
    var Swi = xdc.module("ti.sysbios.knl.Swi");

    /* Loop through each SWI instance and create a Swi instance */
    var insts = this.$instances;

    if (insts.length == 1) {
            var MEM = xdc.module('ti.bios.MEM');
            var CLK = xdc.module('ti.bios.CLK');
            var TSK = xdc.module('ti.bios.TSK');
        if (MEM.NOMEMORYHEAPS == true && CLK.ENABLECLK == false &&
            TSK.ENABLETSK == false) {
            var BIOS = xdc.module('ti.sysbios.BIOS');
            setConfig(BIOS, "swiEnabled", false);
        }
    }

    for (var i = 0; i < insts.length; i++) {

        if (insts[i].order != undefined) {
            this.$logWarning("'order' not supported", insts[i]);
        }

        if (insts[i].name != "PRD_swi") {
             /* We create a wrapper of the type Swi_FuncPtr because the 
             * supplied function is most likely not of that type (CQ19651).
             */
            var fxn = insts[i].fxn;
            var swiParams = new Swi.Params();
            if (typeof(insts[i].arg0) != "number") {
                swiParams.arg0 = $externPtr(insts[i].arg0);
            }
            else {
                swiParams.arg0 = insts[i].arg0;
            }
            if (typeof(insts[i].arg1) != "number") {
                swiParams.arg1 = $externPtr(insts[i].arg1);
            }
            else {
                swiParams.arg1 = insts[i].arg1;
            }
            
            var is28x = Program.build.target.name == "C28_large" || 
                        Program.build.target.name == "C28_float";
            
            if (internal.wrapperFunc(fxn) || is28x) {
                Program.symbol[insts[i].name] = 
                    Swi.create("&" + insts[i].fxn + "__swiFxn", swiParams);
            }
            else {
                    Program.symbol[insts[i].name] = 
                    Swi.create("&" + insts[i].fxn, swiParams);
            }
            
            Program.symbol[insts[i].name].priority = insts[i].priority;
            Program.symbol[insts[i].name].trigger = insts[i].mailbox;
        }
    }
}

/*
 *  ======== exportMod ========
 */
function exportMod()
{
    Program.exportModule("ti.sysbios.knl.Swi");
    Program.exportModule("ti.sysbios.knl.Intrinsics");
}

/*
 *  ======== destroy ========
 */
function destroy()
{
    this.$module.$logError("Please remove the associated create() call, " +
        "the destroy() method is not supported", this);
}

/* 
 *  ======== setConfig ========
 */
function setConfig (mod, configParam, value)
{
    if (mod[configParam] == bios6xDefaults[configParam]) {
        mod[configParam] = value;
    }
    else {
        if (mod[configParam] != value) {
            var SWI = xdc.module("ti.bios.SWI");
            SWI.$logWarning("Cannot change " + mod.$name + "." + configParam + 
            ". Parameter changed elsewhere", SWI);
        }
    }
}
