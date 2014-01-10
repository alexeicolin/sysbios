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
 *  ======== DEV.xs ========
 *
 */

/*
 *  ======== instance$meta$init ========
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
 *  ======== close ========
 *  Do the real work now.
 */
function close()
{
    var insts = this.$instances;

    /* Add DEV_init() into the startup if there are static devices */
    if (insts.length > 0) {
        var Startup = xdc.module("xdc.runtime.Startup");
        Startup.lastFxns.$add('&DEV_init');
    }

    /* Sorting by descending name length */
    for (var i = 0; i < insts.length - 1; i++) {
        for (var j = i + 1; j < insts.length; j++) {
            if (insts[i].name.length < insts[j].name.length) {
                var temp = insts[i];
                insts[i] = insts[j];
                insts[j] = temp;
            }
        }
    }

    /* Convert zero or "NULL" to null.  Makes the .xdt cleaner */
    for (var i = 0; i < insts.length; i++) {

        if (insts[i].initFxn == "NULL") {
            insts[i].initFxn = null;
        }
        
        if (insts[i].fxnTable == "NULL") {
            insts[i].fxnTable = null;
        }
        
        if (insts[i].deviceId == 0) {
            insts[i].deviceId = null;
        }
        
        if (insts[i].params == 0) {
            insts[i].params = null;
        }        
        
        if (insts[i].deviceGlobalDataPtr == 0) {
            insts[i].deviceGlobalDataPtr = null;
        }
    }
}
