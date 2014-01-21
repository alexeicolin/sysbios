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
 *  ======== TSK.xs ========
 *
 */
var bios6xDefaults = {};

/*
 *  ======== module$meta$init ========
 */
function module$meta$init()
{
    /* Only process during "cfg" phase */
    if (xdc.om.$name != "cfg") {
        return;
    }
    this.create("TSK_idle");
    
    /* 
     * get hold of 6x module. Make sure legacy module is brought in first
     * scroll away 6xMod defaults that need to be changed
     */

    var Task = xdc.module('ti.sysbios.knl.Task');
    var BIOS = xdc.module('ti.sysbios.BIOS');
        
    this.$private.bios6xMod = Task;

    if (Task.$used) {
        this.$logError("This module should be used BEFORE " + 
        Task.$name, this);
    }
    
    if (BIOS.$used) {
        this.$logError("This module should be used BEFORE " + 
        BIOS.$name, this);
    }
    
    bios6xDefaults["taskEnabled"] = BIOS.taskEnabled;
    bios6xDefaults["defaultStackHeap"] = Task.defaultStackHeap;
    bios6xDefaults["defaultStackSection"] = Task.defaultStackSection;
    bios6xDefaults["defaultStackSize"] = Task.defaultStackSize;
    bios6xDefaults["idleTaskStackSize"] = Task.idleTaskStackSize;
    bios6xDefaults["idleTaskStackSection"] = Task.idleTaskStackSection;
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
    /* Error if TSK instances created but TSK had not been enabled */
    if (this.ENABLETSK == false) {
        var BIOS = xdc.module("ti.sysbios.BIOS");
        setConfig(BIOS, "taskEnabled", false);

        if ((this.$instances.length == 1) &&
            (this.$instances[0].name == "TSK_idle")) {
            return;
        }
        if (this.$instances.length > 1) {
            this.$logError("bios.TSK.ENABLETSK is not enabled", this);
        }
        return;
    }

    var Task = xdc.module("ti.sysbios.knl.Task");
    var Clock = xdc.module("ti.sysbios.knl.Clock");
    var MEM = xdc.useModule("ti.bios.MEM");
    var Hook = xdc.useModule("ti.bios.HOOK");
    var hookInst = Hook.$instances;

    /* enable names to support TSK_getname() */
    if (!Program.$capsule._romAsm) {
        if (Task.common$.namedInstance == undefined) {
            Task.common$.namedInstance = true;
        }
        else {
            if (Task.common$.namedInstance != true) {
                this.$logWarning("Task.common$.namedInstance must be true for"
                + " TSK_getName. Cfg script has changed it to false", this);
            }
        }
    }

    if (this.DRIVETSKTICK != undefined) {
        this.$logWarning("bios.TSK.DRIVETSKTICK not supported", this);
    }

    if (this.CALLSWITCHFXN != undefined) {
        this.$logWarning("bios.TSK.CALLSWITCHFXN not supported", this);
    }

    if (this.SWITCHFXN != undefined) {
        this.$logWarning("bios.TSK.SWITCHFXN not supported", this);
    }

    /* Hook functons are now part of Task */
    for (var i = 0 ; i < hookInst.length; i++) {
        var fxnParams = {};
        
        var hookKnlReq = false;

        /* The HOOK_KNL properties are set through TSK */
        if (hookInst[i].name == "HOOK_KNL") {
            if (this.CREATEFXN == "FXN_F_nop") {
                hookInst[i].createFxn = null;
            }
            else {
                hookInst[i].createFxn = this.CREATEFXN;
                hookKnlReq = true;
            }
            
            if (this.READYFXN == "FXN_F_nop") {
                hookInst[i].readyFxn = null;
            }
            else {
                hookInst[i].readyFxn = this.READYFXN;
                hookKnlReq = true;
            }
            
            if (this.EXITFXN == "FXN_F_nop") {
                hookInst[i].exitFxn = null;
            }
            else {
                hookInst[i].exitFxn = this.EXITFXN;
                hookKnlReq = true;
            }

            if (this.DELETEFXN == "FXN_F_nop") {
                hookInst[i].deleteFxn = null;
            }
            else {
                hookInst[i].deleteFxn = this.DELETEFXN;
                hookKnlReq = true;
            }

            if (this.CALLREADYFXN) {
                    hookInst[i].callReadyFxn = true;
            }
        
            if (hookKnlReq == false) {
                continue;
            }
            }

        /* Check to make sure functions are not null before assignment */
        if (hookInst[i].initFxn != null) {
            fxnParams.registerFxn = "&" + hookInst[i].initFxn;
        }

        if (hookInst[i].createFxn != null) {
            fxnParams.createFxn = "&" + hookInst[i].createFxn;
        }

        if (hookInst[i].readyFxn != null) {
            if (hookInst[i].callReadyFxn) {
                fxnParams.readyFxn = "&" + hookInst[i].readyFxn;
            }
        }

        if (hookInst[i].switchFxn != null) {
            if (hookInst[i].callSwitchFxn) {
                fxnParams.switchFxn = "&" + hookInst[i].switchFxn;
            }
        }

        if (hookInst[i].exitFxn != null) {
            fxnParams.exitFxn = "&" + hookInst[i].exitFxn;
        }

        if (hookInst[i].deleteFxn != null) {
            fxnParams.deleteFxn = "&" + hookInst[i].deleteFxn;
        }

        /* Add the new hook */
        Task.addHookSet(fxnParams);
    }

    /* Process the STACKSEG assignment if its not null */
    if (this.STACKSEG != null) {
        setConfig(Task, "defaultStackHeap", MEM.getHeapHandle(this.STACKSEG.name));
        setConfig(Task, "defaultStackSection", ".defaultStackSection");
        Program.sectMap[".defaultStackSection"] = new Program.SectionSpec(
            {loadSegment: this.STACKSEG.name});

        if (Task.defaultStackHeap == null) {
            this.$logWarning(this.STACKSEG.name + " does not have a heap",this);
        }
    }
        
    /* if no default value set, set a default depending on target */
    if (this.STACKSIZE === undefined) {
        if (Program.build.target.name == "C28_large" ||
            Program.build.target.name == "C28_float") {
            this.STACKSIZE = 256;
        }    
        else {
            this.STACKSIZE = 1024;
        }
    }

    setConfig(Task, "defaultStackSize", this.STACKSIZE);

    var insts = this.$instances;

    /* Loop through all instances and create a new Task with specified params */
    for (var i = 0; i < insts.length; i++) {

        if (insts[i].stackSize == 0) {
            insts[i].stackSize = this.STACKSIZE;
        }
        
        if (insts[i].priority == undefined ||
            insts[i].priority == null) {
            insts[i].priority = this.PRIORITY;
        }

        if (insts[i].manualStack != undefined) {
            this.$logWarning("'manualStack' not supported", insts[i]);
        }
        if (insts[i].order != undefined) {
            this.$logWarning("'order' not supported", insts[i]);
        }

        var params = new Task.Params();
        params.stackSize = insts[i].stackSize;
        params.priority = insts[i].priority;

        //TSK_staticGlueTab has one less entry, no entry for TSK_idle
        params.arg0 = i - 1;      /* used during startup to get Glue pointer */

        /* encode arg1 with fxn for ROV to use prior to task running */
        if (insts[i].fxn != null) {
            params.arg1 = $externFxn(insts[i].fxn);
        }

        for (var j = 0; j < 8; j++) {        
            if (typeof(insts[i]["arg" + j]) != "number")  {
                insts[i]["arg" + j] = "(UArg)" + insts[i]["arg" + j];
            }
        }

        if (insts[i].allocateTaskName == true) {
            params.instance.name = insts[i].name;
        }

        if (insts[i].autoAllocateStack == false) {
            this.$logError("'allocateStack' must be set to true", insts[i]);
        }
        else {
            if (insts[i].stackMemSeg != null) {
                /* place the stack into specified MEM segment */
                params.stackSection = "_" + insts[i].name + "$stack";
                Program.sectMap["_" + insts[i].name + "$stack"] = 
                    insts[i].stackMemSeg.name;
            }
            else if (this.STACKSEG != null) {
                /* place the stack into TSK.STACKSEG segment */
                params.stackSection = "_" + insts[i].name + "$stack";
                Program.sectMap["_" + insts[i].name + "$stack"] = 
                    this.STACKSEG.name;
            }
        }
        
        params.env = insts[i].envPointer;
        params.vitalTaskFlag = insts[i].exitFlag;

        /* TSK_idle is a special case, it already exists */
        if (insts[i].name != "TSK_idle") {
            Program.symbol[insts[i].name] =
                Task.create("&_TSK_staticGlue", params);
        }
        else {
            setConfig(Task, "idleTaskStackSize", insts[i].stackSize);
            if (insts[i].stackMemSeg != null) {
                setConfig(Task, "idleTaskStackSection", "_" + insts[i].name + "$stack");
            }
        }
    }    
}

/*
 *  ======== exportMod ========
 */
function exportMod()
{
    /* if not ENABLETSK then do not use Task or Clock here */
    if (this.ENABLETSK == false) {
            return
    }

    Program.exportModule("ti.sysbios.knl.Task");
    Program.exportModule("ti.sysbios.knl.Clock");
        
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
            var TSK = xdc.module("ti.bios.TSK");
            TSK.$logWarning("Cannot change " + mod.$name + " " + configParam + 
            ". Parameter changed elsewhere", TSK);
        }
    }
}
