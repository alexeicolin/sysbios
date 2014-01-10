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
 *  ======== Load.xs ========
 */

var Task = null;
var Idle = null;
var Swi = null;
var Hwi = null;
var Load = null;
var BIOS = null;
var Timestamp = null;
var Queue = null;
var Diags = null;
var Core = null;

/* 
 *  QSHIFT and QFACTOR are used to compute the cpu load.  This is duplicated
 *  from Load.c
 */
var QSHIFT = 4;
var QFACTOR = 1 * Math.pow(2, QSHIFT); // translation of C code "1 << QSHIFT"

/*
 *  ======== module$use ========
 */
function module$use()
{
    Load = this;
    
    BIOS = xdc.useModule("ti.sysbios.BIOS");
    Idle = xdc.useModule("ti.sysbios.knl.Idle");
    Timestamp = xdc.useModule("xdc.runtime.Timestamp");
    Queue = xdc.useModule("ti.sysbios.knl.Queue");
    Hwi = xdc.useModule("ti.sysbios.hal.Hwi");
    Diags = xdc.useModule('xdc.runtime.Diags');

    if ((BIOS.smpEnabled == true) || (BIOS.libType == BIOS.LibType_Debug)) {
        Core = xdc.useModule('ti.sysbios.hal.Core');
    }

    if (BIOS.swiEnabled == true) {
        Swi = xdc.useModule("ti.sysbios.knl.Swi");
    }

    if (BIOS.taskEnabled == true) {
        Task = xdc.useModule("ti.sysbios.knl.Task");
    }

}

/*
 *  ======== module$static$init ========
 */
function module$static$init(mod, params)
{
    var numCores = 1;

    if (BIOS.smpEnabled == true) {
        numCores = Core.numCores;
    }

    if (Load.hwiEnabled == true) {
        Hwi.addHookSet({
            createFxn: null,
            beginFxn: Load.hwiBeginHook,
            endFxn: Load.hwiEndHook,
        });
    }
    
    if ((BIOS.taskEnabled == true) && (Load.taskEnabled == true)) {
        Task.addHookSet({
            registerFxn: '&ti_sysbios_utils_Load_taskRegHook__E',
            createFxn: Load.taskCreateHook,
            readyFxn:  null,
            switchFxn: Load.taskSwitchHook,
            exitFxn: null,
            deleteFxn: Load.taskDeleteHook,
        });
        if (Load.common$.diags_USER4 == Diags.ALWAYS_ON ||
            Load.common$.diags_USER4 == Diags.RUNTIME_ON ) {
            Task.common$.namedInstance = true; /* auto turn on task names*/
        }
    }
    
    if ((BIOS.swiEnabled == true) && (Load.swiEnabled == true)) {
        Swi.addHookSet({
            registerFxn: null,
            createFxn: null,
            readyFxn:  null,
            beginFxn: Load.swiBeginHook,
            endFxn: Load.swiEndHook,
            deleteFxn: null,
        });
    }
    
    Idle.addFunc(Load.idleFxn);

    /* Construct the taskList */
    Queue.construct(mod.taskList);
    
    mod.taskStartTime.length = numCores;
    mod.runningTask.length = numCores;

    for (var i = 0; i < numCores; i++) {
        mod.taskStartTime[i] = 0;
        mod.runningTask[i] = null;
    }

    mod.taskHId = 0;
    mod.timeElapsed = 0;
    mod.firstSwitchDone = false;
    mod.swiStartTime = 0;
    mod.swiCnt = 0; 
    mod.swiEnv.totalTimeElapsed = 0;
    mod.swiEnv.totalTime = 0;         
    mod.swiEnv.nextTotalTime = 0;    
    mod.swiEnv.timeOfLastUpdate = 0;
    mod.swiEnv.threadHandle = null;
    Queue.elemClearMeta(mod.swiEnv.qElem);
    mod.hwiStartTime = 0;
    mod.hwiCnt = 0; 
    mod.hwiEnv.totalTimeElapsed = 0;
    mod.hwiEnv.totalTime = 0;         
    mod.hwiEnv.nextTotalTime = 0;    
    mod.hwiEnv.timeOfLastUpdate = 0;
    mod.hwiEnv.threadHandle = null;
    mod.timeSlotCnt = 0;
    mod.minLoop = 0xFFFFFFFF;       
    mod.minIdle = Load.minIdle;       
    mod.t0 = 0;
    mod.idleCnt = 0;        
    mod.cpuLoad = 0; 
    Queue.elemClearMeta(mod.hwiEnv.qElem);
}

/*
 *  ======== module$validate ========
 *  some redundant tests are here to catch changes since
 *  module$static$init()
 */
function module$validate()
{
    if ((Load.swiEnabled == true) && (BIOS.swiEnabled == false)) {
        Load.$logWarning("Load has swi monitoring enabled, but BIOS has swi"
            + " disabled.", Load, "swiEnabled");
    } 
    
    if ((Load.taskEnabled == true) && (BIOS.taskEnabled == false)) {
        Load.$logWarning("Load has task monitoring enabled, but BIOS has task"
            + " disabled.", Load, "taskEnabled");
    } 
    
    if ((Load.taskEnabled == false) && (Load.autoAddTasks == true)) {
        Load.autoAddTasks = false;
    }
    
    if ((Load.updateInIdle == true) && (Load.windowInMs == 0)) {
        Load.$logWarning("windowInMs must be set to a non-zero value.", Load, "windowInMs");
    }
}

/*
 *  ======== viewInitModule ========
 *  Initialize the Load 'Module' view.
 */
function viewInitModule(view, obj)
{
    var Task = xdc.useModule('ti.sysbios.knl.Task');
    
    var LoadCfg = Program.getModuleConfig('ti.sysbios.utils.Load');
    
    /* compute cpu load in the same manner as Load_getCPULoad() C API: */
    var cpuLoadValue = ((obj.cpuLoad + QFACTOR / 2) / QFACTOR);
    if ((cpuLoadValue > 100) || (cpuLoadValue < 0)) {
        view.$status["cpuLoad"] = 
            "Error: CPU load computation resulted in out of range number: "
            + cpuLoadValue;
    }
    /* call toFixed() to specify only 1 digit after the decimal */
    view.cpuLoad = cpuLoadValue.toFixed(1);

    /*
     *  Only compute Swi stats if user set Load module hwiEnabled flag. Also
     *  check for divide by zero:
     */
    if (LoadCfg.swiEnabled && obj.swiEnv.totalTimeElapsed > 0) {
        /* first compute the load to get a floating point value */
        var swiLoadValue = obj.swiEnv.totalTime / obj.swiEnv.totalTimeElapsed * 100;

        if ((swiLoadValue > 100) || (swiLoadValue < 0)) {
            view.$status["swiLoad"] = 
                "Error: Swi load computation resulted in out of range number: "
                + swiLoadValue;
        }

        /* call toFixed() to specify only 1 digit after the decimal */
        view.swiLoad = swiLoadValue.toFixed(1);
    }
    else if (!(LoadCfg.swiEnabled)) { 
        view.swiLoad = "disabled";
    }
    else if (obj.swiEnv.totalTimeElapsed == 0) {
        /* protect against divide by 0 */
        view.swiLoad = "undefined";
    }
    else {
        view.$status["swiLoad"] = "Error: Swi time elaspsed is negative: "
                + obj.swiEnv.totalTimeElapsed;
    }
        
    /*
     *  Only compute Hwi stats if user set Load module hwiEnabled flag. Also
     *  check for divide by zero:
     */
    if (LoadCfg.hwiEnabled && obj.hwiEnv.totalTimeElapsed > 0) {
        /* first compute the load to get a floating point value */
        var hwiLoadValue = obj.hwiEnv.totalTime / obj.hwiEnv.totalTimeElapsed * 100;

        if ((hwiLoadValue > 100) || (hwiLoadValue < 0)) {
            view.$status["hwiLoad"] = 
                "Error: Swi load computation resulted in out of range number: "
                + hwiLoadValue;
        }

        /* call toFixed() to specify only 1 digit after the decimal */
        view.hwiLoad = hwiLoadValue.toFixed(1);
    }
    else if (!(LoadCfg.hwiEnabled)) { 
        view.hwiLoad = "disabled";
    }
    else if (obj.hwiEnv.totalTimeElapsed == 0) {
        /* protect against divide by 0 */
        view.hwiLoad = "undefined";
    }
    else {
        view.$status["hwiLoad"] = "Error: Hwi time elaspsed is negative: "
                + obj.hwiEnv.totalTimeElapsed;
    }
    
    /* compute an accuracy metric */
    var minIdle = obj.minLoop > obj.minIdle ? obj.minLoop : obj.minIdle;
    if (minIdle > 0) {
        var err = 1 - ((minIdle - 1) / minIdle);
        view.idleError = (err * 100).toFixed(1) + "%";

        /* if total error is more than 15% warn user */
        err = err * (1 - obj.cpuLoad / (100 * QFACTOR));
        if (err >= 0.15) {
            view.$status["idleError"] = 
                "Warning: estimated error in total CPU load may be off by as much as "
                + (err * 100).toFixed(1);
        }
    }
    else {
        view.idleError = "unknown";
    }
}
