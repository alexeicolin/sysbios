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
 *  ======== BUF.xs ========
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

    /* 
     * get hold of 6x module. Make sure legacy module is brought in first
     * scroll away 6xMod defaults that need to be changed
     */

    var bios6xMod = xdc.module('ti.sysbios.heaps.HeapBuf');
    this.$private.bios6xMod = bios6xMod;

    if (bios6xMod.$used) {
        this.$logError("This module should be used BEFORE " + 
        bios6xMod.$name, this);
    }
    bios6xDefaults["trackMaxAllocs"] = bios6xMod.trackMaxAllocs;
}

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
}

/*
 *  ======== close ========
 *  Create a HeapBuf instance for each BUF instance.
 */
function close()
{
    /* Loop through BUF instances and create a HeapBuf instance for each */
    var insts = this.$instances;

    if (insts.length > 0) {
        var HeapBuf = xdc.module("ti.sysbios.heaps.HeapBuf");

        /* track the max # of allocs in this HeapBuf (needed for maxbuff API) */
        setConfig(HeapBuf, "trackMaxAllocs", true);

        var params = new HeapBuf.Params();

        for (var i = 0; i < insts.length; i++) {
            
            if (!(insts[i].bufCount && insts[i].size)) {
                var BUF = xdc.module("ti.bios.BUF");
                BUF.$logFatal("Both bufCount and size must be set", 
                    insts[i].name);
                
            }
            params.align = insts[i].align;
            params.numBlocks = insts[i].bufCount;
            params.blockSize = insts[i].size;

            if (insts[i].bufSeg != null) {
                params.sectionName = "_" + insts[i].name;
                Program.sectMap["_" + insts[i].name] = insts[i].bufSeg.name;
            }

            Program.symbol[insts[i].name] = HeapBuf.create(params);
        }
    }
}

/*
 *  ======== exportMod ========
 */
function exportMod()
{
    /* use the HeapBuf module */
    Program.exportModule("ti.sysbios.heaps.HeapBuf");
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
            var BUF = xdc.module("ti.bios.BUF");
            BUF.$logWarning("Cannot change " + mod.$name + "." + configParam + 
            ". Parameter changed elsewhere", BUF);
        }
    }
}
