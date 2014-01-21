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
 *  ======== PRD.xs ========
 *
 */

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
 *  Create a Clock instance for each PRD instance.  Make the PRD instance
 *  name a global symbol. This function is called in close() of package.xs
 */
function close()
{
    var Clock = xdc.module("ti.sysbios.knl.Clock");

    if (this.USECLK != undefined) {
        this.$logWarning("bios.PRD.USECLK not supported", this);
    }
    /* Loop through all PRD objects and create a Clock object */
    var insts = this.$instances;

    for (var i = 0; i < insts.length; i++) {
        var prd = insts[i].period;
        var mode = insts[i].mode;
        var start = true;

        if (mode == "one-shot") {
            prd = 0;
            start = false;
        }

        if (insts[i].arg1 != undefined) {
            this.$logWarning("'arg1' not supported", insts[i]);
        }

        if (insts[i].order != undefined) {
            this.$logWarning("'order' not supported", insts[i]);
        }

        var fxn = insts[i].fxn;
        if (internal.wrapperFunc(fxn)) {
            if (internal.wrapperFunc(fxn).args > 1) {
                this.$logError("The function " + fxn + " cannot be assigned to "
                    + "a PRD instance fxn parameter.", insts[i]);       
            }
            /* We create a wrapper of the type Clock_FuncPtr because the 
             * supplied function is most likely not of that type (CQ19651).
             */
            Program.symbol[insts[i].name] = Clock.create("&" + insts[i].fxn +
                "__clkFxn", insts[i].period, {
                    startFlag: start, arg: insts[i].arg0, period: prd
                }
            );
        }
        else {
            if (Program.build.target.name == "C28_large" || 
                Program.build.target.name == "C28_float") {
                Program.symbol[insts[i].name] = Clock.create("&" + insts[i].fxn +
                    "__clkFxn", insts[i].period, {
                        startFlag: start, arg: insts[i].arg0, period: prd
                    }
                );
            }
            else {
                Program.symbol[insts[i].name] = Clock.create("&" + insts[i].fxn,
                    insts[i].period, {
                        startFlag: start, arg: insts[i].arg0, period: prd
                    }
                );
            }
        }
    }
}

/*
 *  ======== exportMod ========
 */
function exportMod()
{
    if (this.$instances.length > 0) {
            Program.exportModule("ti.sysbios.knl.Clock");
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
