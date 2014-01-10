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

var intNumDef =  [2, 3];

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

    Timer.intNumDef[0]= 2;
    Timer.intNumDef[1]= 3;
}

/*
 *  ======== module$use ========
 */
function module$use()
{
    Hwi = xdc.useModule("ti.sysbios.family.arm.tms570.Hwi");
    BIOS = xdc.useModule('ti.sysbios.BIOS');
    // To force Timer_startup in BIOS_start
    xdc.useModule("ti.sysbios.hal.Timer");
}

/*
 *  ======== module$static$init ========
 */
function module$static$init(mod, params)
{
    /* availMask has 2 bits set for the two timers */
    mod.availMask = 0x3;

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

    if (obj.id == undefined) {
        Timer.$logFatal("Timer device unavailable.", this);
    }


    obj.controlRegInit = (params.controlRegInit.cos << 15);

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
    obj.accuTicks = 0;

    if (obj.tickFxn) {
        if (!params.hwiParams) {
            params.hwiParams = new Hwi.Params();
        }
        var hwiParams = params.hwiParams;

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

        if (hwiParams.type == Hwi.Type_IRQ) {
            hwiParams.arg = this;

            if (obj.runMode == Timer.RunMode_CONTINUOUS) {
                obj.hwi = Hwi.create(obj.intNum, Timer.periodicStub, hwiParams);
            }
            else {
                obj.hwi = Hwi.create(obj.intNum, Timer.oneShotStub , hwiParams);
            }
        }
        else { /* FIQ interrupts go straight to ISR, no stub */
            obj.hwi = Hwi.create(obj.intNum, obj.tickFxn , hwiParams);
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
 *  ======== viewInitBasic ========
 *  Initialize the 'Basic' Timer instance view.
 */
function viewInitBasic(view, obj)
{
    var Program = xdc.useModule('xdc.rov.Program');
    var halTimer = xdc.useModule('ti.sysbios.hal.Timer');

    view.halTimerHandle =  halTimer.viewGetHandle(obj.$addr);
    view.label       = Program.getShortName(obj.$label);
    view.id          = obj.id;

    view.startMode   = getEnumString(obj.startMode);
    view.runMode     = getEnumString(obj.runMode);
    view.period      = obj.period;
    view.periodType  = getEnumString(obj.periodType);
    view.intNum = obj.intNum;

    view.tickFxn     = Program.lookupFuncName(Number(obj.tickFxn));
    view.arg         = obj.arg;
    view.accuTicks   = obj.accuTicks;

    view.extFreq     = Number(obj.extFreq.lo + (obj.extFreq.hi << 32)).toString(10);
    view.hwiHandle   = "0x" + Number(obj.hwi).toString(16);
}

/*
 *  ======== viewInitDevice ========
 */
function viewInitDevice(view, obj)
{
    var Program = xdc.useModule('xdc.rov.Program');

    view.id          = obj.id;
    view.device      = "timer"+view.id;
    view.intNum = obj.intNum;
    view.runMode     = getEnumString(obj.runMode);

    var gptmAddr = 0xFFFFFC00;
    view.devAddr = "0x" + gptmAddr.toString(16);

    var timer = Program.fetchArray(
        {   type: 'xdc.rov.support.ScalarStructs.S_UInt32',
            isScalar: true
        },
        Number(view.devAddr), 24, false);

    if (view.id == 0) {
        /* udcp0 */
        view.period = timer[21];
        /* comp0 - frc0 */
        view.remainingCount = timer[20] - timer[4];

        /* gctrl */
        if (timer[0] & 1) {
            view.state = "Enabled";
        }
        else {
            view.state = "Disabled";
        }
    }
    else {
        /* udcp1 */
        view.period = timer[23];
        /* comp1 - frc1 */
        view.remainingCount = timer[22] - timer[12];

        /* gctrl */
        if (timer[0] & 2) {
            view.state = "Enabled";
        }
        else {
            view.state = "Disabled";
        }
    }
    view.currCount = view.period - view.remainingCount;
}

/*
 *  ======== viewInitModule ========
 *  Initialize the Timer 'Module' view.
 */
function viewInitModule(view, obj)
{
    view.availMask = Number(obj.availMask).toString(2);
}



