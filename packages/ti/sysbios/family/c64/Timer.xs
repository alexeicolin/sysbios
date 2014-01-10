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
 *  ======== Timer.xs ========
 *
 */

var Timer = null;
var Hwi = null;
var BIOS = null;

var intNumDef =  [14,15,10];
var eventIdDef = [1,2,19];

/*
 *  ======== module$meta$init ========
 */
function module$meta$init()
{
    /* Only process during "cfg" phase */
    if (xdc.om.$name != "cfg") {
        return;
    }
    Timer = this;
    /* 
     * set fxntab to false because ti.sysbios.hal.Timer can be used 
     * instead of an abstract intsance
     */
    Timer.common$.fxntab = false;
}

/*
 *  ======== module$use ========
 */
function module$use()
{
    Hwi = xdc.useModule("ti.sysbios.family.c64.Hwi");
    BIOS = xdc.useModule('ti.sysbios.BIOS');
    // To force Timer_startup in BIOS_start     
    xdc.useModule("ti.sysbios.hal.Timer");
}

/*
 *  ======== module$static$init ========
 */
function module$static$init(mod, params)
{
    mod.baseAddress[0] = 0x1940000;
    mod.baseAddress[1] = 0x1980000;
    mod.baseAddress[2] = 0x1AC0000;

    /* availMask has 3 bits set for the three timers */
    mod.availMask = 0x7; 

    if (params.anyMask > mod.availMask) {
        Timer.$logError("Incorrect anyMask (" + params.anyMask
            + "). Should be <= " + mod.availMask + ".", Timer, "anyMask");
    }

    for (var i = 0; i < Timer.NUM_TIMER_DEVICES; i++) {
        mod.handles[i] = null;
    }
}

/*
 *  ======== instance$static$init ========
 */
function instance$static$init(obj, id, tickFxn, params)
{
    var modObj = this.$module.$object;

    /* set flag because static instances need to be started */
    Timer.startupNeeded = true;

    obj.staticInst = true;
    
    if ((id >= Timer.NUM_TIMER_DEVICES)) {
        if (id != Timer.ANY) {
            Timer.$logFatal("Invalid Timer ID " + id + "!", this);
        }
    }

    if (id == Timer.ANY) {
        for (var i = 0; i < Timer.NUM_TIMER_DEVICES; i++) {
            if ((Timer.anyMask & (1 << i)) && (modObj.availMask & (1 << i))) {
                modObj.availMask &= ~(1 << i);
                obj.id = i;
                break;
            }
        }
    }
    else if (modObj.availMask & (1 << id)) {
        modObj.availMask &= ~(1 << id);
        obj.id = id;
    }

    /*
     * If a timer has not been assigned because either the requested timer was
     * unavailable or 'any timer' was requested and none were available...
     */
    if (obj.id == undefined) {
        if (id == Timer.ANY) {
            Timer.$logFatal("No available timers.", this);
        }
        else {
            Timer.$logFatal("Timer " + id + " is already in use or reserved (check availMask).", this);
        }
    }

    var clkSrc = (params.extFreq.lo)?0:1;
    obj.controlRegInit = (params.controlRegInit.func |
                       (params.controlRegInit.invout << 1) |
                       (params.controlRegInit.datout << 2) |
                       (params.controlRegInit.pwid << 4) |
                       (params.controlRegInit.cp << 8) |
                       (clkSrc << 9) |
                       (params.controlRegInit.spnd << 15));

    obj.runMode = params.runMode;
    obj.startMode = params.startMode;
    obj.period = params.period;
    obj.periodType = params.periodType;
    if (params.intNum == -1) {
        obj.intNum = intNumDef[obj.id];
    }
    else {
        obj.intNum = params.intNum;
    }
    obj.arg = params.arg;
    obj.tickFxn = tickFxn;
    obj.extFreq.lo = params.extFreq.lo;
    obj.extFreq.hi = params.extFreq.hi;
 
    if (obj.tickFxn) {
        if (!params.hwiParams) {
            params.hwiParams = new Hwi.Params();
        }
        var hwiParams = params.hwiParams;
        hwiParams.eventId = eventIdDef[obj.id];

        if (obj.runMode == Timer.RunMode_ONESHOT) {
            if (hwiParams.maskSetting == Hwi.MaskingOption_NONE) {
                Timer.$logFatal("maskSetting must disable self.", this);
            }
            if (hwiParams.maskSetting == Hwi.MaskingOption_BITMASK) {
                if (!(hwiParams.disableMask & (1 << obj.intNum))) {
                    Timer.$logFatal("maskSetting must disable self.", this);
                }
            }
        }
        if (Hwi.inUseMeta(obj.intNum) == true) {
            Timer.$logError("Timer interrupt " + obj.intNum + 
                                " already in use!", this);
            return;
        }
        

        if (obj.runMode == Timer.RunMode_CONTINUOUS) {
            hwiParams.arg = obj.arg;
            obj.hwi = Hwi.create(obj.intNum, obj.tickFxn, hwiParams);
        }
        else {
            hwiParams.arg = this;
            obj.hwi = Hwi.create(obj.intNum, Timer.oneShotStub , hwiParams);
        }
    }
    else {
        obj.hwi = null;
    }

    modObj.handles[obj.id] = this;
}

/*
 *  ======== module$view$init ========
 */
function module$view$init(view, mod)
{
    var Timer = xdc.useModule('ti.sysbios.family.c64.Timer');
    for (var i=0; i<Timer.NUM_TIMER_DEVICES; i++) {
        view.timers[i].id = i;
        if (mod.availMask & (0x1 << i)) {
            view.timers[i].inUse = false;
        }
        else {
            view.timers[i].inUse = true;
        }
    }
}

/*
 *  ======== instance$view$init ========
 */
function instance$view$init(view, obj)
{
    view.label = obj.$label;
    view.id = obj.id;
    view.configuration.func = (obj.controlRegInit) & 0x0001;
    view.configuration.invout = (obj.controlRegInit >> 1) & 0x0001;
    view.configuration.datout = (obj.controlRegInit >> 2) & 0x0001;
    view.configuration.pwid = (obj.controlRegInit >> 4) & 0x0001;
    view.configuration.cp = (obj.controlRegInit >> 8) & 0x0001;
    view.configuration.spnd = (obj.controlRegInit >> 15) & 0x0001;
    view.runMode = obj.runMode;
    view.startMode = obj.startMode;
    view.period = obj.period;
    view.intNum = obj.intNum;
    view.arg = obj.arg;
    
    var fxn = String(obj.tickFxn).replace("@", "0x");
    view.isrFxn = Program.lookupFuncName(Number(fxn));

    view.extFreq = obj.extFreq;
    view.accuTicks = obj.accuTicks;
    //TODO scan Hwi Instance
    view.hwiHandle = String(obj.hwi);
}

