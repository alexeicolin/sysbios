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
 *  ======== UDEV.xs ========
 *
 */

/*
 *  ======== close ========
 *  Make sure the name is set.
 */
function instance$meta$init(name)
{    
    if (name.indexOf('_') == 0) {
        name = name.substr(1);
    }
    this.name = name;
}

/*
 *  ======== get ========
 *  See if this UDEV device exists.
 */
function get(name)
{
    for (var i = 0; i < this.$instances.length; i++) {    
        if (name == this.$instances[i].name) {
            return(name);
        }
    }
    return(undefined);
}

/*
 *  ======== close ========
 *  Do the real work now.
 */
function close()
{
    var DEV = xdc.useModule("ti.bios.DEV");    
    
    /* Set-up the default params for the create. */
    var params = new DEV.Params();    
    
    for (var i = 0; i < this.$instances.length; i++) {    
    
        /* Fill in the params */
        params.initFxn             = this.$instances[i].initFxn;
        params.fxnTable            = this.$instances[i].fxnTable
        params.fxnTableType        = this.$instances[i].fxnTableType;
        params.deviceId            = this.$instances[i].deviceId;
        params.params              = this.$instances[i].params;
        params.deviceGlobalDataPtr = this.$instances[i].deviceGlobalDataPtr;
        
        /* Create the user device */
        DEV.create(this.$instances[i].name, params);
    }
}
