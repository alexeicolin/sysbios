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
 *  ======== LOG.xs ========
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
    
    var logsystem = this.create("LOG_system");
    this.$$bind("LOG_system", logsystem);
    
    /* 
     * get hold of 6x module. Make sure legacy module is brought in first
     * scroll away 6xMod defaults that need to be changed
     */

    var bios6xMod = xdc.module('xdc.runtime.LoggerBuf');
    this.$private.bios6xMod = bios6xMod;

    if (bios6xMod.$used) {
        this.$logError("This module should be used BEFORE " + 
        bios6xMod.$name, this);
    }
    bios6xDefaults["enableFlush"] = bios6xMod.enableFlush;
}

/*
 *  ======== instance$meta$init ========
 *  Create pre-created objects from BIOS 5.x
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
 */
function close()
{
    var LoggerBuf = xdc.module('xdc.runtime.LoggerBuf');

    if (this.enableFlush == true) {
        setConfig(LoggerBuf, "enableFlush", true);
    }

    /* create LoggerBuf instances */
    var insts = this.$instances;

    for (var i = 0; i < insts.length; i++) {
        var params = {};

        if (insts[i].logType == "circular") {
            params.bufType = LoggerBuf.BufType_CIRCULAR;
        }
        else if (insts[i].logType == "fixed") {
            params.bufType = LoggerBuf.BufType_FIXED;
        }
        else {
            this.$logError(insts[i].logType + " is illegal value for " +
                            "LOG.logType", insts[i]);
        }

        // TODO: support dataType and format
        if (insts[i].dataType != undefined) {
            this.$logWarning("'dataType' is not supported", insts[i]);
        }

        if (insts[i].format != undefined) {
            this.$logWarning("'format' is not supported", insts[i]);
        }

        if (insts[i].exitFlush == true) {
            params.exitFlush = true;
        }

        params.numEntries = insts[i].bufLen/8;
        
        if (insts[i].bufLen == 0) {
            if (insts[i].name != "LOG_system") {
                this.$logWarning("Creating buffer with bufLen = 0", insts[i]);
                Program.symbol[insts[i].name] = LoggerBuf.create(params);
            }
        }
        else {
            Program.symbol[insts[i].name] = LoggerBuf.create(params);
        }
    }
}

/*
 *  ======== exportMod ========
 */
function exportMod()
{
    /*
     *  Do export LoggerBuf if LOG_system is the only instance
     *  and it has a bufLen of 0.
     */
    if (this.$instances.length > 1 || this.$instances[0].bufLen != 0) {
        Program.exportModule('xdc.runtime.LoggerBuf');
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
        var LOG = xdc.module("ti.bios.LOG");
            LOG.$logWarning("Cannot change " + mod.$name + "." + configParam + 
            ". Parameter changed elsewhere", LOG);
        }
    }
}
