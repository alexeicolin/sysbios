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
 *  ======== Soprano.xs ========
 *
 */

var BIOS = null;
var Build = null;
var Program = null;
var Soprano = null;

/*
 *  ======== module$meta$init ========
 */
function module$meta$init()
{
    /* Only process during "cfg" phase */
    if (xdc.om.$name != "cfg") {
        return;
    }

    BIOS = xdc.useModule('ti.sysbios.BIOS');
    Build = xdc.useModule('ti.sysbios.Build');
    Soprano = xdc.module('ti.sysbios.rom.c28.Soprano');

    BIOS.logsEnabled = false;
    BIOS.assertsEnabled = false;

    Program = xdc.useModule('xdc.cfg.Program');
    if (Program.platformName.match(/ti\.platforms\.tms320x28:TMS320F2807/) ||
        Program.platformName.match(/ti\.platforms\.tms320x28:TMS320F2837/)) {
        var PlatformMod = xdc.module('ti.platforms.tms320x28.Platform');
        PlatformMod.genAliases = false;
    }

    if (Build.buildROM == false) {
        Build.buildROMApp = true;
        BIOS.libDir = "Soprano/";
    }
    else {
        Build.buildROMApp = false;
        BIOS.libDir = null;
        Soprano.libDir = String(Soprano.$package.packageBase + "Soprano/");
    }
}

/*
 *  ======== module$use ========
 */
function module$use()
{
    /*
     * Pull in all modules that were used in building the ROM to prevent
     * symbol undefined errors.
     */
    xdc.includeFile(String(Soprano.$package.packageBase + "Soprano.cfg.xs"));

    if (!Program.platformName.match(/ti\.platforms\.tms320x28:TMS320F2807/) &&
        !Program.platformName.match(/ti\.platforms\.tms320x28:TMS320F2837/) &&
         Build.buildROM == false) {
        this.$logError(
                "Soprano ROM Applications only support Generic Platform",
                this);
    }

    if (Program.linkTemplate != null) {
        print("Warning: linker template change will not take effect!");
    }
    Program.linkTemplate = String(Soprano.$package.packageBase + "SopranoCmd/linkcmd.xdt");
}

/*
 *  ======== module$validate ========
 */
function module$validate()
{
    var Settings = xdc.module("ti.sysbios.family.Settings");
    var Swi = xdc.module('ti.sysbios.knl.Swi');
    var Hwi = xdc.module('ti.sysbios.hal.Hwi');
    var Task = xdc.module('ti.sysbios.knl.Task');
    var Semaphore = xdc.module('ti.sysbios.knl.Semaphore');
    var HwiDelegate = xdc.module(Settings.getDefaultHwiDelegate());
    var Defaults = xdc.module('xdc.runtime.Defaults');
    var Types = xdc.module("xdc.runtime.Types");

    if (BIOS.$written("libType") && (Build.buildROM == false)) {
        this.$logWarning(
            "BIOS.libType is ignored when using rom", BIOS);
    }

    if (BIOS.assertsEnabled == true) {
        this.$logError(
            "BIOS.assertsEnabled must be set to 'false' when using rom", BIOS);
    }

    if (BIOS.logsEnabled == true) {
        this.$logError(
            "BIOS.logsEnabled must be set to 'false' when using rom", BIOS);
    }

    if (BIOS.taskEnabled == false) {
        this.$logError(
            "BIOS.taskEnabled must be set to 'true' when using rom", BIOS);
    }

    if (BIOS.swiEnabled == false) {
        this.$logError(
            "BIOS.swiEnabled must be set to 'true' when using rom", BIOS);
    }

    if (Swi.hooks.length != 0) {
        this.$logError("Swi hooks are not supported when using rom", Swi);
    }

    if ((Hwi.numHooks != 0) || (HwiDelegate.hooks.length != 0)) {
        this.$logError("Hwi hooks are not supported when using rom", Hwi);
    }

    if (Semaphore.supportsEvents == true) {
        this.$logError(
            "Semaphore.supportEvents must be set to 'false' when using rom",
            Semaphore);
    }

    /* Default numPriorities for C28x is 16 */
    if (Swi.$written("numPriorities") && Swi.numPriorities != 16) {
        this.$logError(
            "Swi.numPriorities must not be changed when using rom",
            Swi);
    }

    if (HwiDelegate.dispatcherIrpTrackingSupport == false) {
        this.$logError(
            "Hwi.dispatcherIrpTrackingSupport must be set to 'true' when using rom",
            HwiDelegate);
    }

    if (HwiDelegate.dispatcherTaskSupport == false) {
        this.$logError(
            "Hwi.dispatcherTaskSupport must be set to 'true' when using rom",
            HwiDelegate);
    }

    if (HwiDelegate.dispatcherSwiSupport == false) {
        this.$logError(
            "Hwi.dispatcherSwiSupport must be set to 'true' when using rom",
            HwiDelegate);
    }

    if (HwiDelegate.dispatcherAutoNestingSupport == false) {
        this.$logError(
            "Hwi.dispatcherAutoNestingSupport must be set to 'true' when using rom",
            HwiDelegate);
    }

    if (HwiDelegate.zeroLatencyIERMask != 0) {
        this.$logError(
            "Hwi.zeroLatencyIERMask must be set to 0x0 when using rom",
            HwiDelegate);
    }

    if (Defaults.common$.memoryPolicy == Types.STATIC_POLICY) {
        this.$logError(
            "Static memory policy is not supported when using rom",
            Defaults);
    }
}
