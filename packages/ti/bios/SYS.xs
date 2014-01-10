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
 *  ======== SYS.xs ========
 *
 */

var SysMin = null;
var System = null;
var Sys    = null;
var bios6xDefaults = {};

/*
 *  ======== module$meta$init ========
 */
function module$meta$init(name)
{
    /* Only process during "cfg" phase */
    if (xdc.om.$name != "cfg") {
        return;
    }

    /* This module will be the SystemSupport module for the application */
    SysMin = xdc.module("xdc.runtime.SysMin");
    System = xdc.module("xdc.runtime.System");
    Sys    = xdc.module("ti.bios.support.Sys");
    
    /* 
     * get hold of 6x module. Make sure legacy module is brought in first
     * scroll away 6xMod defaults that need to be changed
     */

    this.$private.bios6xMod = SysMin;

    if (SysMin.$used) {
        this.$logError("This module should be used BEFORE " + 
        SysMin.$name, this);
    }
    bios6xDefaults["bufSize"] = SysMin.bufSize;
}

/*
 *  ======== exportMod ========
 */
function exportMod()
{
    Program.exportModule("xdc.runtime.SysMin");
    Program.exportModule("ti.bios.support.Sys");
    Program.exportModule("xdc.runtime.System");
}

/*
 *  ======== close ========
 */
function close()
{
    /*
     * Give the Sys module the error function to call
     */
    if ((Sys.errorFxn != null) && (this.ERRORFXN != null)) {
        this.$logWarning("ti.bios.support.Sys.errorFxn is already set. Ignoring SYS.ERRORFXN config", this);
    }
    else if ((this.ERRORFXN != null) && (this.ERRORFXN != "UTL_doError")) {
        Sys.errorFxn = "&" + this.ERRORFXN;
    }

    /*
     * Give the Sys module the abort function to call
     */
    if ((Sys.abortFxn != null) && (this.ABORTFXN != null)) {
        this.$logWarning("ti.bios.support.Sys.abortFxn is already set. Ignoring SYS.ABORTFXN config", this);
    }
    else if ((this.ABORTFXN != null) && (this.ABORTFXN != "UTL_doAbort")){
        Sys.abortFxn = "&" + this.ABORTFXN;
    }

    /*
     * Give the Sys module the exit function to call
     */
    if ((Sys.exitFxn != null) && (this.EXITFXN != null)) {
        this.$logWarning("ti.bios.support.Sys.exitFxn is already set. Ignoring SYS.EXITFXN config", this);
    }
    else if ((this.EXITFXN != null) && (this.EXITFXN != "UTL_halt")) {
        Sys.exitFxn = "&" + this.EXITFXN;
    }

    /* 
     *  If SupportProxy is SysMin, slam the TRACESIZE setting if (if defined).
     *  If SysStd, it is a nop, since there is no size for SysStd.
     *  Otherwise, give a warning that it is ignored.
     */
    if (this.TRACESIZE !== undefined) {
        if (System.SupportProxy.delegate$.$name == "xdc.runtime.SysMin") {
            setConfig(SysMin, "bufSize", this.TRACESIZE);
        }
        else if (System.SupportProxy.delegate$.$name != "xdc.runtime.SysStd") {
            this.$logWarning("The TRACESIZE cannot be set on your System.SupportProxy module (" + System.SupportProxy.delegate$.$name + "). Therefore SYS.TRACESIZE is being ignored", this);
        }        
    }

    /*
     *  Check to see if user is trying to use non-supported config parameters.
     */

    if (this.TRACESEG !== undefined) {
        this.$logWarning("SYS.TRACESEG is not supported. Refer to the XDC User Guide (SPRUEX4) and your System.SupportProxy module (" +
              System.SupportProxy.delegate$.$name + ") for more details.", this);
    }

    if (this.PUTCFXN != null) {
        this.$logWarning("SYS.PUTCFXN is not supported.", this);
    }
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
            var SYS = xdc.module("ti.bios.SYS");
            SYS.$logWarning("Cannot change " + mod.$name + "." + configParam + 
            ". Parameter changed elsewhere", SYS);
        }
    }
}
