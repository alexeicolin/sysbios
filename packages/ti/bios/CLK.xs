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
 *  ======== CLK.xs ========
 *
 */

if (xdc.om.$name == "cfg") {
    var deviceTable = {
        "ti.catalog.c6000": {
            "TMS320CDM6446": {
                timerId: 1,
                master: false
            },
            "TMS320CDM6437": {
                timerId: 1,
                master: true
            }
        },
        "ti.catalog.c6000.nda": {
        },
        "ti.catalog.c2800": {
        }    
    }
    if (Program.cpu.catalogName == "ti.catalog.c6000") {
        deviceTable[Program.cpu.catalogName]["TMS320CDM420"] =
            deviceTable[Program.cpu.catalogName]["TMS320CDM6446"];
        deviceTable[Program.cpu.catalogName]["TMS320CDM6467"] =
            deviceTable[Program.cpu.catalogName]["TMS320CDM6446"];
        deviceTable[Program.cpu.catalogName]["TMS320C6424"] =
            deviceTable[Program.cpu.catalogName]["TMS320CDM6437"];
    }
}

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

    var Clock = xdc.module('ti.sysbios.knl.Clock');
    var BIOS = xdc.module('ti.sysbios.BIOS');

    var Settings = xdc.module('ti.sysbios.family.Settings');
    var Timer = xdc.module(Settings.getDefaultTimerDelegate());
    var Timestamp = xdc.module('xdc.runtime.Timestamp');
    var TimestampDelegate = xdc.module(Settings.getDefaultTimestampDelegate());
    Timestamp.SupportProxy = TimestampDelegate;
        
    this.$private.bios6xMod = Clock;

    if (Clock.$used) {
        this.$logError("This module should be used BEFORE " + 
        Clock.$name, this);
    }
    
    if (BIOS.$used) {
        this.$logError("This module should be used BEFORE " + 
        BIOS.$name, this);
    }
    
    if (Timer.$used) {
        this.$logError("This module should be used BEFORE " + 
        Timer.$name, this);
    }
    
    if (Timestamp.$used) {
        this.$logError("This module should be used BEFORE " + 
        Timestamp.$name, this);
    }
    
    if (TimestampDelegate.$used) {
        this.$logError("This module should be used BEFORE " + 
        TimestampDelegate.$name, this);
    }
    
    bios6xDefaults["clockEnabled"] = BIOS.clockEnabled;
    bios6xDefaults["tickSource"] = Clock.tickSource;
    bios6xDefaults["" + Clock + "timerId"] = Clock.timerId;
    bios6xDefaults["tickPeriod"] = Clock.tickPeriod;
    
    if ("timerId" in TimestampDelegate) {
        bios6xDefaults["" + TimestampDelegate + "timerId"] 
            = TimestampDelegate.timerId;
    }
    
    if ("timerSettings" in Timer) {
        bios6xDefaults["timerSettings"] = Timer.timerSettings.$copy();
    }
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
 */
function close()
{
    var BIOS = xdc.module("ti.sysbios.BIOS");
    var Clock = xdc.module("ti.sysbios.knl.Clock");
    var device = deviceTable[Program.cpu.catalogName][Program.cpu.deviceName];
    var clockId;

    if (this.ENABLECLK == false) {
        setConfig(BIOS, "clockEnabled", false);
        setConfig(Clock, "tickSource", Clock.TickSource_NULL);

        if (this.$instances.length > 0) {
            this.$logError("CLK.ENABLECLK is not enabled", this);
        }

        return;
    }
        
    /*
     *  if CLK.TIMERSELECT not defined then set it to default from table above.
     *  if CLK.TIMERSELECT and Clock.timerId both set then warn about this.
     */
    if ((this.TIMERSELECT == undefined) && (device != null)) {
        setConfig(Clock, "timerId", device.timerId);
        clockId = device.timerId;
    }

    /* set Clock.timerId accordingly if CLK.TIMERSELECT defined */
    if (this.TIMERSELECT == "Timer 0") {
        setConfig(Clock, "timerId", 0);
        clockId = 0;
    }
    else if (this.TIMERSELECT == "Timer 1") {
        setConfig(Clock, "timerId", 1);
        clockId = 1;
    }
    else if (this.TIMERSELECT == "Timer 2") {
        setConfig(Clock, "timerId", 2);
        clockId = 2;
    }

    /* These settings do not apply to new devices like C66 */
    if (Program.build.target.isa == "64P" ||
        Program.build.target.isa == "674" ) {
        if (this.TIMERSELECT == "Timer 5") {
            setConfig(Clock, "timerId", 0);
            clockId = 0;
        }
        else if (this.TIMERSELECT == "Timer 6") {
            setConfig(Clock, "timerId", 1);
            clockId = 1;
        }
    }

    setConfig(Clock, "tickPeriod", this.MICROSECONDS);

    var Settings = xdc.module('ti.sysbios.family.Settings');
    var Timer = xdc.module(Settings.getDefaultTimerDelegate());
     
    if (device != null) {
        if (Program.build.target.isa == "64P" ||
            Program.build.target.isa == "674") {

            if (this.RESETTIMER == undefined) {
                setArrayConfig(Timer, "timerSettings", clockId, "master", 
                    device.master);
            }
            else {
                setArrayConfig(Timer, "timerSettings", clockId, "master", 
                    this.RESETTIMER);
            }

            if (this.TIMMODE == "32-bit chained") {
                setArrayConfig(Timer, "timerSettings", clockId, "mode", 
                    Timer.Mode_CHAINED);
            }
            else {
                setArrayConfig(Timer, "timerSettings", clockId, "mode", 
                    Timer.Mode_UNCHAINED);
            }
        }
    }

    if (this.TIMER5BASE != undefined) {
        setArrayConfig(Timer, "timerSettings", 0, "baseAddr", 
            this.TIMER5BASE); 
    }
    if (this.TIMER6BASE != undefined) {
        setArrayConfig(Timer, "timerSettings", 1, "baseAddr", 
            this.TIMER6BASE);
    }
    if (this.TIMER7BASE != undefined) {
        setArrayConfig(Timer, "timerSettings", 2, "baseAddr", 
            this.TIMER7BASE);        
    }
    if (this.TIMER8BASE != undefined) {
        setArrayConfig(Timer, "timerSettings", 3, "baseAddr", 
            this.TIMER8BASE); 
    }

    var Settings = xdc.module('ti.sysbios.family.Settings');
    var TimestampDelegate = xdc.module(Settings.getDefaultTimestampDelegate());
    var timerId = Clock.timerId;

    if (Program.build.target.isa == "64") {
        if (this.WHICHHIRESTIMER == "Timer 0") {
            timerId = 0;
        }
        else if (this.WHICHHIRESTIMER == "Timer 1") {
            timerId = 1;
        }
        else if (this.WHICHHIRESTIMER == "Timer 2") {
            timerId = 2;
        }
        else {
            timerId = Clock.timerId;
        }

        if (timerId != Clock.timerId) {
            this.$logWarning("bios.CLK.WHICHRESTIMER setting high resolution" +
            " timer to a timer different than the system tick", this);
        }
        setConfig(TimestampDelegate, "timerId", timerId);
    }

    for (var j in this) {
        if (j != "ENABLECLK"    && j != "TIMERSELECT" &&
            j != "MICROSECONDS" && j != "WHICHHIRESTIMER" &&
            j != "RESETTIMER"   && j != "TIMMODE"     &&
            j != "ENABLEHTIME"  && j != "OBJMEMSEG" &&
            j != "TIMER5BASE"  && j != "TIMER6BASE" &&
            j != "TIMER7BASE"  && j != "TIMER8BASE" &&
            this[j] != undefined) {
            this.$logWarning("bios.CLK." + j + " not supported", this);
        }
    }

    /* Loop through each CLK instances */
    var insts = this.$instances;

    for (var i = 0; i < insts.length; i++) {
        if (insts[i].order != undefined) {
            this.$logWarning("'order' not supported", insts[i]);
        }

        var params = new Clock.Params();
        params.startFlag = true;
        params.period = 1;

        var fxn = this.$instances[i].fxn;
        if (internal.wrapperFunc(fxn)) {
            /* We create a wrapper of the type Clock_FuncPtr because the 
             * supplied function is most likely not of that type (CQ19651).
             */
            if (internal.wrapperFunc(fxn).args > 0) {
                this.$logError("The function " + fxn + " cannot be assigned to "
                    + "a CLK instance fxn parameter.", insts[i]);    
            }
            Clock.create("&" + this.$instances[i].fxn + "__clkFxn", 1, params);
        }
        else {
            Clock.create("&" + this.$instances[i].fxn, 1, params);    
        }
    }
}


/*
 *  ======== exportMod ========
 */
function exportMod()
{
    if (this.ENABLECLK) {
        Program.exportModule("ti.sysbios.knl.Clock");

        if (Program.build.target.isa == "64P" ||
            Program.build.target.isa == "674" ) {
            if (this.ENABLEHTIME) {
                Program.exportModule("xdc.runtime.Timestamp");
            }
        }
        else {
            Program.exportModule("xdc.runtime.Timestamp");
        }
    }
    else {
        if (Program.build.target.isa == "64P" ||
            Program.build.target.isa == "674" ) {
            if (this.ENABLEHTIME) {
                Program.exportModule("xdc.runtime.Timestamp");
            }
        }
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
    var mapName = configParam;
    
    if (configParam == "timerId") {
        mapName = "" + mod + configParam;
    }
    
    if (mod[configParam] == bios6xDefaults[mapName]) {
        mod[configParam] = value;
    }
    else {
        if (mod[configParam] != value) {
            var CLK = xdc.module("ti.bios.CLK");
            CLK.$logWarning("Cannot change " + mod.$name + "." + configParam 
            + ". Parameter changed elsewhere", CLK);
        }
    }
}

/* 
 *  ======== setArrayConfig ========
 */
function setArrayConfig (mod, configParam, index, subfield, value)
{
    if (mod[configParam][index][subfield] == 
        bios6xDefaults[configParam][index][subfield]) {
        mod[configParam][index][subfield] = value;
    }
    else {
        if (mod[configParam][index][subfield] != value) {
        var CLK = xdc.module("ti.bios.CLK");
            CLK.$logWarning("Cannot change " + mod.$name + "." + configParam + 
            "[" + index + "]" + "." + subfield + 
            ". Parameter changed elsewhere", CLK);
        }
    }
}
