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
 *  ======== DGN.xs ========
 *
 */

/*
 *  ======== instance$meta$init ========
 *  Initializes instance name to the param 'name'.
 */
function instance$meta$init(name)
{
    if (name.indexOf('_') == 0) {
        name = name.substr(1);
    }
    this.name = name;
    this.paramsType = "DGN_Params";
}

/*
 *  ======== close ========
 */
function close()
{
    DEV = xdc.useModule("ti.bios.DEV");    
    
    insts = this.$instances;
    
    /* Set-up the default params for the create. */
    var params = new DEV.Params();    
    params.initFxn             = null;
    params.fxnTable            = "DGN_FXNS";
    params.fxnTableType        = "DEV_SIOTYPE";        
    params.includePath         = "ti/bios/include/dgn.h";
    params.deviceGlobalDataPtr = null;

    for (var i = 0; i < insts.length; i++) {    
    
        /* Determine the type of device */
        if (insts[i].device == "user" ) {
            params.deviceId = "DGN_USER";
            insts[i].fxn = null;
            insts[i].arg = 0;
        }
        else if (insts[i].device == "printHex") {
            params.deviceId = "DGN_USER";
            insts[i].fxn = "DGN_printHex";
        }
        else if (insts[i].device == "printInt") {
            params.deviceId = "DGN_USER";
            insts[i].fxn = "DGN_printInt";
        }
        else if (insts[i].device == "sine") {
            params.deviceId = "DGN_SINE";
        }
        else if (insts[i].device == "random") {
            params.deviceId = "DGN_RAND";
        }
        else if ((insts[i].device == "const") || (insts[i].device == "constant")) {
            params.deviceId = "DGN_CONST";
        }
        else {
            var errMsg = "No such device " + insts[i].device + ".";
            this.$logError(errMsg, this);
            continue;
        }
        
        /* Set params and paramsType */
        params.params = insts[i].name + "instance_params";
        params.paramsType = insts[i].paramsType;        

        if (typeof(insts[i].arg) == "string") {
            insts[i].arg = "(UArg)&" + insts[i].arg;
        }

        DEV.create(insts[i].name, params);                   
    }
}
