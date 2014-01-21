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
 *  ======== MBX.xs ========
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

    var Mailbox = xdc.module('ti.sysbios.knl.Mailbox');
    this.$private.bios6xMod = Mailbox;
}


/*
 *  ======== myGetter ========
 */
function myGetter(name) {
    return this.messageLength;
}

/*
 *  ======== mySetter ========
 */
function mySetter(name, value) {
    this.messageLength = value;
}

/*
 *  ======== instance$meta$init ========
 *  Initializes instance name to the name parameter.
 */
function instance$meta$init(name)
{
    /* 
     * The lines below along with functons above map the original parameter
     * "length" to new parameter called "messageLength" since "length" is 
     * a keyword.
     */
    this.$$proto.$$fldv(
        'length',
        $$T_CNum('(xdc_Int)'), undefined, 'wh',
        myGetter, mySetter
    );
    if (name.indexOf('_') == 0) {
        name = name.substr(1);
    }

    this.name = name;
}

/*
 *  ======== close ========
 *  Create a Mailbox instance for each MBX instance created. The MBX name will
 *  be a global symbol.
 */
function close()
{
    var Mailbox = xdc.module("ti.sysbios.knl.Mailbox");

    var mbxParams = new Mailbox.Params();

    /* Loop through each instance and create a Mailbox instance */
    var insts = this.$instances;

    for (var i = 0; i < insts.length; i++) {

        if (insts[i].elementSeg != null) {
            mbxParams.sectionName = "_" + insts[i].name;
            Program.sectMap["_" + insts[i].name] = insts[i].elementSeg.name;
        }

        Program.symbol[insts[i].name] = Mailbox.create(insts[i].messageSize,
            insts[i].messageLength, mbxParams);
    }
}

/*
 *  ======== exportMod ========
 */
function exportMod()
{
    Program.exportModule("ti.sysbios.knl.Mailbox");
}

/*
 *  ======== destroy ========
 */
function destroy()
{
    this.$module.$logError("Please remove the associated create() call, " +
        "the destroy() method is not supported", this);
}
