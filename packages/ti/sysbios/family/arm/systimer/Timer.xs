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
 *  ======== Timer.xs ========
 */

var halTimer = null;
var Timer = null;
var Hwi = null;
var BIOS = null;
var BIG;
var PERIOD;
var PREV_THRESHOLD;
var NEXT_THRESHOLD;

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

    Timer.intNumDef[0] = 30;
    Timer.intNumDef[1] = 27;
}

/*
 *  ======== module$use ========
 */
function module$use()
{
    Hwi = xdc.useModule('ti.sysbios.family.arm.gic.Hwi');
    BIOS = xdc.useModule('ti.sysbios.BIOS');

    /*
     *  This is to force ti_sysbios_hal_Timer_startup() to be called from
     *  BIOS_start(). hal/Timer_startup will call the proxy_Timer_startup.
     *  This is required for the Timer to startup.
     */
    halTimer = xdc.useModule("ti.sysbios.hal.Timer");
}

/*
 *  ======== module$static$init ========
 */
function module$static$init(mod, params)
{
    /* availMask has 2 bits set for the two timers */
    mod.availMask = Timer.availMask;

    if (params.anyMask > mod.availMask) {
        Timer.$logError("Incorrect anyMask (" + params.anyMask
            + "). Should be <= " + mod.availMask + ".", Timer, "anyMask");
    }

    mod.handles.length = Timer.NUM_TIMER_DEVICES;

    for (var i = 0; i < Timer.NUM_TIMER_DEVICES; i++) {
        mod.handles[i] = null;
    }

    /*
     * if this timer module is not the hal.Timer delegate,
     * plug Timer.startup into BIOS.startupFxns array
     */
    if (halTimer.TimerProxy.delegate$.$name !=
        "ti.sysbios.family.arm.systimer.Timer") {
        BIOS.addUserStartupFunction(Timer.startup);
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
        Timer.$logFatal("Timer device unavailable.", this);
    }
   
    obj.runMode       = params.runMode;
    obj.startMode     = params.startMode;
    obj.periodType    = params.periodType;
    obj.intNum        = Timer.intNumDef[obj.id];
    obj.arg           = params.arg;
    obj.tickFxn       = tickFxn;
    obj.extFreq.lo    = params.extFreq.lo;
    obj.extFreq.hi    = params.extFreq.hi;
    obj.prevThreshold = 0;
    obj.nextThreshold = 0;

    if (params.period64 == 0) {
        obj.period    = params.period;
    }
    else {
        obj.period    = params.period64;
    }
 
    if (obj.tickFxn) {
        if (!params.hwiParams) {
            params.hwiParams = new Hwi.Params();
        }
        var hwiParams = params.hwiParams;

        // TODO Do I need to change this ?
        if (obj.runMode == Timer.RunMode_ONESHOT) {
            if (hwiParams.maskSetting == Hwi.MaskingOption_NONE) {
                Timer.$logFatal("Mask in hwiParams cannot" + 
                    "enable self.", this);
            }
            if (hwiParams.maskSetting == Hwi.MaskingOption_BITMASK) {
                if (!(hwiParams.disableMask & (1 << obj.intNum))) {
                    Timer.$logFatal("Mask in hwiParams cannot" + 
                        "enable self.", this);
                }
            }
        }

        if (Hwi.inUseMeta(obj.intNum) == true) {
            Timer.$logError("Timer interrupt " + obj.intNum + 
                    " already in use!", obj);
            return;
        }

        if (hwiParams.type == Hwi.Type_IRQ) {
            hwiParams.arg = this;
            if ((obj.runMode == Timer.RunMode_CONTINUOUS) || 
                (obj.runMode == Timer.RunMode_DYNAMIC)) {
                obj.hwi = Hwi.create(obj.intNum, Timer.periodicStub, hwiParams);
            }
            else {
                obj.hwi = Hwi.create(obj.intNum, Timer.oneShotStub, hwiParams);
            }
        }
    }
    else {
        obj.hwi = null;
    }

    modObj.handles[obj.id] = this;
}

/*
 *  ======== getEnumString ========
 *  Get the enum value string name, not 0, 1, 2 or 3, etc.  For an enumeration
 *  type property.
 *
 *  Example usage:
 *  if obj contains an enumeration type property "Enum enumProp"
 *
 *  view.enumString = getEnumString(obj.enumProp);
 *
 */
function getEnumString(enumProperty)
{
    /*
     *  Split the string into tokens in order to get rid of the huge package
     *  path that precedes the enum string name. Return the last 2 tokens
     *  concatenated with "_" 
     */
    var enumStrArray = String(enumProperty).split(".");
    var len = enumStrArray.length;
    return (enumStrArray[len - 1]);
}

/*
 *  ======== getStructElem64 ========
 *  Read a 64bit structure element and conver it to a string
 */
function getStructElem64(objAddr, offset)
{
    var structElem          = Program.fetchArray(
                            {
                                type: 'xdc.rov.support.ScalarStructs.S_UInt32',
                                isScalar: true
                            },
                            objAddr + offset,
                            2,
                            false);

    return ("0x" + Number(structElem[1 ^ BIG]).toString(16) + 
                   Number(structElem[0 ^ BIG]).toString(16));
}

/*
 *  ======== viewInitBasic ========
 *  Initialize the 'Basic' Timer instance view.
 */
function viewInitBasic(view, obj)
{
    var Program = xdc.useModule('xdc.rov.Program');
    var halTimer = xdc.useModule('ti.sysbios.hal.Timer');
    var modName = "ti.sysbios.family.arm.systimer.Timer";
    /* Compute structure offsets required in the ROV view */
    BIG = (Program.build.target.model.endian == "big");
    PERIOD = xdc.om[modName + ".Instance_State"].$offsetof("period");
    PREV_THRESHOLD = 
        xdc.om[modName + ".Instance_State"].$offsetof("prevThreshold");
    NEXT_THRESHOLD = 
        xdc.om[modName + ".Instance_State"].$offsetof("nextThreshold");

    view.halTimerHandle = halTimer.viewGetHandle(obj.$addr);
    view.label          = Program.getShortName(obj.$label);
    view.id             = obj.id;
    view.startMode      = getEnumString(obj.startMode);
    view.runMode        = getEnumString(obj.runMode);
    view.period         = getStructElem64(obj.$addr, PERIOD);
    view.periodType     = getEnumString(obj.periodType);
    view.intNum         = obj.intNum;
    view.tickFxn        = Program.lookupFuncName(Number(obj.tickFxn));
    view.arg            = obj.arg;

    view.extFreq        = 
        Number(obj.extFreq.lo + (obj.extFreq.hi << 32)).toString(10);

    view.hwiHandle      = "0x" + Number(obj.hwi).toString(16);
    view.prevThreshold  = getStructElem64(obj.$addr, PREV_THRESHOLD);
    view.nextThreshold  = getStructElem64(obj.$addr, NEXT_THRESHOLD);
}

/*
 *  ======== viewInitModule ========
 *  Initialize the Timer 'Module' view.
 */
function viewInitModule(view, obj)
{
    view.availMask = Number(obj.availMask).toString(2);
}
