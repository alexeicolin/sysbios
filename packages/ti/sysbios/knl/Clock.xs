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
 *  ======== Clock.xs ========
 *
 */

var Queue = null;
var Clock = null;
var Timer = null;
var Swi = null;
var BIOS = null;
var Settings = null;

/*
 *  ======== module$meta$init ========
 */
function module$meta$init()
{
    /* Only process during "cfg" phase */
    if (xdc.om.$name != "cfg") {
        return;
    }

    Clock = this;

    Settings = xdc.module("ti.sysbios.family.Settings");
    Clock.tickPeriod = Settings.getDefaultClockTickPeriod();

}

/*
 *  ======== module$use ========
 */
function module$use()
{
    BIOS = xdc.useModule('ti.sysbios.BIOS');
    /* last parameter is true to disable expected cycle warnings  */
    Queue = xdc.useModule('ti.sysbios.knl.Queue');
    Timer = xdc.useModule('ti.sysbios.hal.Timer');
    xdc.useModule('ti.sysbios.hal.Hwi');
    xdc.useModule('xdc.runtime.Log');
    xdc.useModule('xdc.runtime.Assert');

    if (BIOS.swiEnabled) {
        Swi = xdc.useModule('ti.sysbios.knl.Swi');
        if (Clock.swiPriority === undefined) {
            Clock.swiPriority = Swi.numPriorities - 1;
        }
        else if (Clock.swiPriority >= Swi.numPriorities) {
             Clock.$logError (
                 "Clock.swiPriority (" + Clock.swiPriority +
                 ") must be less than Swi.numPriorities (" +
                 Swi.numPriorities + ")", Clock, "swiPriority");
        }
    }
    else {
        Clock.swiPriority = 0;
    }
    
    if (!BIOS.clockEnabled) {
        if (Clock.$written("tickSource") && 
           (Clock.tickSource != Clock.TickSource_NULL)) {
            Clock.$logWarning("The Clock.tickSource setting is internally "
            + "forced to Clock.TickSource_NULL if the Clock Manager has "
            + "been disabled (BIOS.clockEnabled == false).",
            Clock, "tickSource");
        }
        /* 
         * Set Clock.tickSource to NULL to avoid instantiating timer
         */
        Clock.tickSource = Clock.TickSource_NULL;
    }

    /* if user has not defined the TickMode ... */
    if (Clock.tickMode == undefined) {
        if (Timer.TimerProxy.defaultDynamic == true) {
            Clock.tickMode = Clock.TickMode_DYNAMIC;
        }
        else {
            Clock.tickMode = Clock.TickMode_PERIODIC;
        }
    }
}

/*
 *  ======== module$static$init ========
 */
function module$static$init(mod, params)
{
    /* for some reason BIOS from module$use doesn't exist anymore */
    BIOS = xdc.module('ti.sysbios.BIOS');

    Queue.construct(mod.clockQ);

    mod.ticks = 0;
    mod.swiCount = 0;
    mod.numTickSkip = 0;
    mod.periodCounts = 0;
    mod.inWorkFunc = false;
    mod.skipsWorkFunc = 0;

    /* 
     * Minimize footprint by only creating Clock Swi if Clock and Swi
     * are enabled. 
     */
    if ((BIOS.clockEnabled == true) && (BIOS.swiEnabled == true)) {
        /* Create Clock Swi */
        var swiParams = new Swi.Params();
        swiParams.arg0 = 0;
        swiParams.arg1 = 0;
        swiParams.priority = Clock.swiPriority;
        swiParams.trigger = 0;
        if (Clock.tickMode == Clock.TickMode_DYNAMIC) {
            mod.swi = Swi.create(Clock.workFuncDynamic, swiParams);
        }
        else {
            mod.swi = Swi.create(Clock.workFunc, swiParams);
        }
    }
    else {
        mod.swi = null;
    }
    
    if (Clock.tickSource == Clock.TickSource_TIMER) {
        var BIOS = xdc.om['ti.sysbios.BIOS'];
        var timerParams = new Timer.Params();
        if (Clock.tickMode == Clock.TickMode_DYNAMIC) {
            timerParams.runMode = Timer.RunMode_DYNAMIC;
        }
        else {
            timerParams.runMode = Timer.RunMode_CONTINUOUS;
        }
        timerParams.startMode = Timer.StartMode_AUTO;
        // Compute period in timer counts
        timerParams.period = Clock.tickPeriod;
        timerParams.periodType = Timer.PeriodType_MICROSECS;
        mod.timer = Timer.create(Clock.timerId, Clock.doTick, timerParams);
    }
    else {
        mod.timer = null;
    }
}

/*
 *  ======== module$validate ========
 */
function module$validate()
{
    /* check constraints for using TickMode_DYNAMIC */
    if (Clock.tickMode == Clock.TickMode_DYNAMIC) {

        /* verify that the bound Timer supports RunMode_DYNAMIC */
        if (Timer.TimerProxy.supportsDynamic == false) {
            Clock.$logError("Clock tick suppression (TickMode_DYNAMIC) not supported on this device", Clock, "tickMode");
        }

        /* ensure TickSource_USER is NOT specified */
        if (Clock.tickSource == Clock.TickSource_USER) {
            Clock.$logError("Clock tick suppression requires the tick source to be TickSource_TIMER", Clock, "tickSource");
        }
    }

    /* validate all "created" instances */
    for (var i = 0; i < Clock.$instances.length; i++) {
        instance_validate(Clock.$instances[i]);
    }

    /* validate all "constructed" instances */
    for (var i = 0; i < Clock.$objects.length; i++) {
        instance_validate(Clock.$objects[i]);
    }
}

/*
 *  ======== instance_validate ========
 *  common function to test instance configuration
 */
function instance_validate(instance)
{    
    if (instance.$object.fxn == null) {
        Clock.$logError("function cannot be null", instance);
    }
}

/*
 *  ======== instance$static$init ========
 */
function instance$static$init(obj, func, timeout, params)
{
    var modObj = this.$module.$object;

    if ((BIOS.clockEnabled == false) || (BIOS.swiEnabled == false)) {
        Clock.$logFatal("Can't create a Clock with BIOS.clockEnabled == " + 
                        " false or BIOS.swiEnabled == false", this);
    }

    /* Integer-ize the timeout param before checking it */
    timeout = Math.floor(timeout);

    if ((timeout == 0) && (params.startFlag == true)) {
        Clock.$logError("The timeout parameter must not be zero.", this,
                        "timeout");
    }

    obj.timeout = timeout;
    obj.currTimeout = timeout;
    obj.period = params.period;
    obj.fxn = func;
    obj.arg = params.arg;
    obj.active = false;

    /*
     * Clock object is always placed on Clock work Q
     */
    modObj.clockQ.putMeta(obj.elem);
  
    /* if enabled, set active flag to true */
    if (params.startFlag && (timeout != 0)) {
        obj.active = true;
    }
}

/*
 *  ======== viewInitBasic ========
 */
function viewInitBasic(view, obj)
{
    var Program = xdc.useModule('xdc.rov.Program');
    var Model = xdc.useModule("xdc.rov.Model");

    view.label = obj.$label;
    view.timeout = obj.timeout;
    view.period = obj.period;
    
    view.fxn = Program.lookupFuncName(Number(obj.fxn));
    
    view.arg = obj.arg;
    
    /* The inst is started if active is TRUE */
    if (obj.active == false) {
        view.started = false;
    }
    else {
        view.started = true;

        var modRaw = Program.scanRawView("ti.sysbios.knl.Clock");

        var remain = obj.currTimeout - modRaw.modState.ticks;
        
        /* 
         * Check if 'currTimeout' has wrapped.
         *
         * 'currTimeout' is initially calculated as the timeout value plus the
         * current tick count. This value may be greater than 2^32 and wrap. 
         * We need to account for this here.
         */ 
        if (remain < 0) {
            remain += Math.pow(2, 32);
        }

        /* if skipping ticks indicate stale data */
        if (modRaw.modState.numTickSkip) {
            view.tRemaining = String(remain) + " (stale data)";
        }
        else {
            view.tRemaining = String(remain);
        }

    }

    view.periodic = (obj.period > 0);
}

/*
 *  ======== viewInitModule ========
 */
function viewInitModule(view, mod)
{
    var Program = xdc.useModule('xdc.rov.Program');
    var Timer = xdc.useModule('ti.sysbios.hal.Timer');
    var modRaw = Program.scanRawView("ti.sysbios.knl.Clock");

    /* if skipping ticks indicate stale data */
    if (modRaw.modState.numTickSkip) {
        view.ticks = String(mod.ticks) + " (stale data)";
    }
    else {
        view.ticks = String(mod.ticks);
    }
    
    view.tickSource = Program.$modules['ti.sysbios.knl.Clock'].tickSource;
    view.tickMode = Program.$modules['ti.sysbios.knl.Clock'].tickMode;

    try {
        var timerRawView = Program.scanRawView('ti.sysbios.hal.Timer');
    }
    catch (e) {
        print (e);
        return;
    }

    /* Check each instance */
    for (var i in timerRawView.instStates) {
        if (Number(mod.timer) == Number(timerRawView.instStates[i].$addr)) {
            view.timerHandle = String(timerRawView.instStates[i].pi);
            view.timerId = Number(i);
        }
    }
 
    view.swiPriority = Program.$modules['ti.sysbios.knl.Clock'].swiPriority;
    view.tickPeriod = Program.$modules['ti.sysbios.knl.Clock'].tickPeriod;
    view.nSkip = mod.numTickSkip;
}
