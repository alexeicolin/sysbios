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
 *  ======== GBL.xs ========
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

    var BIOS = xdc.module('ti.sysbios.BIOS');
    var Settings = xdc.module('ti.sysbios.family.Settings');

    if (Settings.getDefaultCacheDelegate() == null) {
        var Cache = xdc.module("ti.sysbios.hal.CacheNull");
    }
    else {
        var Cache = xdc.module(Settings.getDefaultCacheDelegate());
    }
    this.$private.bios6xMod = Cache;

    if (Cache.$used) {
        this.$logError("This module should be used BEFORE " + 
        Cache.$name, this);
    }
    if (BIOS.$used) {
        this.$logError("This module should be used BEFORE " + 
        BIOS.$name, this);
    }
    
    bios6xDefaults["cpuFreq"] = BIOS.cpuFreq;
    
    if ("MAR0_31" in Cache) {
        bios6xDefaults["MAR0_31"] = Cache.MAR0_31;
    }    
    if ("MAR32_63" in Cache) {
        bios6xDefaults["MAR32_63"] = Cache.MAR32_63;
    }
    if ("MAR64_95" in Cache) {
        bios6xDefaults["MAR64_95"] = Cache.MAR64_95;
    }
    if ("MAR96_127" in Cache) {
        bios6xDefaults["MAR96_127"] = Cache.MAR96_127;
    }
    if ("MAR128_159" in Cache) {
        bios6xDefaults["MAR128_159"] = Cache.MAR128_159;
    }
    if ("MAR160_191" in Cache) {
        bios6xDefaults["MAR160_191"] = Cache.MAR160_191;
    }
    if ("MAR192_223" in Cache) {
        bios6xDefaults["MAR192_223"] = Cache.MAR192_223;
    }
    if ("MAR224_255" in Cache) {
        bios6xDefaults["MAR224_255"] = Cache.MAR224_255;
    }
}

/*
 *  ======== exportMod ========
 */
function exportMod()
{
    /* exportModule is equivalent to saying xdc.useModule in .cfg file */
    Program.exportModule("ti.sysbios.BIOS");
    
    if (Program.build.target.isa == "64P" ||
        Program.build.target.isa == "64T" ||
        Program.build.target.isa == "674") {
        Program.exportModule("ti.sysbios.family.c64p.Cache");
    }
    else if (Program.build.target.isa == "66") {
        Program.exportModule("ti.sysbios.family.c66.Cache");
    }
}

/*
 *  ======== close ========
 *  Set the BIOS.cpuFrequency. All other GBL config settings from BIOS 5.x
 *  is ignored.
 */
function close()
{
    var BIOS = xdc.module("ti.sysbios.BIOS");

    if (this.CLKOUT != 0) {
        setSubConfig(BIOS, "cpuFreq", "lo", this.CLKOUT * 1000000);
    }
    else {
        if (Program.cpu.clockRate != null) {
            this.CLKOUT = Program.cpu.clockRate;
            setSubConfig(BIOS, "cpuFreq", "lo", this.CLKOUT * 1000000);
        }
    }

    if (this.CALLUSERINITFXN) {
        var Startup = xdc.module("xdc.runtime.Startup");
        Startup.firstFxns.$add("&" + this.USERINITFXN);
    }

    if (this.C64PLUSCONFIGURE) {
        if (Program.build.target.isa == "64P" ||
            Program.build.target.isa == "64T" ||
            Program.build.target.isa == "674") {
            var Cache = xdc.module("ti.sysbios.family.c64p.Cache");
        }
        else if (Program.build.target.isa == "66") {
            var Cache = xdc.module("ti.sysbios.family.c66.Cache");
        }

        setConfig(Cache, "MAR0_31", this.C64PLUSMAR0to31);
        setConfig(Cache, "MAR32_63", this.C64PLUSMAR32to63);
        setConfig(Cache, "MAR64_95", this.C64PLUSMAR64to95);
        setConfig(Cache, "MAR96_127", this.C64PLUSMAR96to127);
        setConfig(Cache, "MAR128_159", this.C64PLUSMAR128to159);
        setConfig(Cache, "MAR160_191", this.C64PLUSMAR160to191);
        setConfig(Cache, "MAR192_223", this.C64PLUSMAR192to223);
        setConfig(Cache, "MAR224_255", this.C64PLUSMAR224to255);
    }

    for (var j in this) {
        /* Special message for cache size configs which are not supported */
        if ((j == "C64PLUSL1PCFG" || j == "C64PLUSL1DCFG" || 
             j == "C64PLUSL2CFG") && this[j] != undefined) {
            this.$logWarning("bios.GBL." + j + " is not supported. Set Cache sizes using the Platform wizard. See BIOS 6.x User Guide for more details", this);
        }
        else if (j != "CLKOUT"             && j != "CLKIN"              &&
                 j != "CALLUSERINITFXN"    && j != "USERINITFXN"        &&
                 j != "C64PLUSCONFIGURE"   && j != "C64PLUSMAR0to31"    && 
                 j != "C64PLUSMAR32to63"   && j != "C64PLUSMAR64to95"   && 
                 j != "C64PLUSMAR96to127"  && j != "C64PLUSMAR128to159" && 
                 j != "C64PLUSMAR160to191" && j != "C64PLUSMAR192to223" && 
                 j != "C64PLUSMAR224to255" && j != "PROCID"             && 
                 j != "ENABLEALLTRC"       &&
                 this[j] != undefined) {
            this.$logWarning("bios.GBL." + j + " not supported ... ", this);
        }
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
            var GBL = xdc.module("ti.bios.GBL");
            GBL.$logWarning("Cannot change " + mod.$name + "." + configParam + 
            ". Parameter changed elsewhere", GBL);
        }
    }
}

/* 
 *  ======== setSubConfig ========
 */
function setSubConfig (mod, configParam, subfield, value)
{
    if (mod[configParam][subfield] == bios6xDefaults[configParam][subfield]) {
        mod[configParam][subfield] = value;
    }
    else {
        if (mod[configParam][subfield] != value) {
            var GBL = xdc.module("ti.bios.GBL");
            GBL.$logWarning("Cannot change " + mod.$name + "." + configParam + 
            "." + subfield + ". Parameter changed elsewhere", GBL);
        }
    }
}
