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
 *  ======== Hwi.xs ========
 *
 */

var Hwi = null;
var Memory = null;

/*
 *  ======== module$meta$init ========
 */
function module$meta$init()
{
    /* Only process during "cfg" phase */
    if (xdc.om.$name != "cfg") {
        return;
    }

    Hwi = this;

    /* set fxntab default */
    Hwi.common$.fxntab = false;

    /*
     * Initialize meta-only Hwi object array
     */

    /* all others use vector to Hwi_nonPluggedInt by default */
    for (var intNum = 0; intNum < Hwi.NUM_INTERRUPTS; intNum++) {
        Hwi.interrupt[intNum].used = false;
        Hwi.interrupt[intNum].useDispatcher = false;
    }

    if (Hwi.resetFunc === undefined) {
        Hwi.resetFunc = '&_c_int00';
    }

    if (Hwi.undefinedInstFunc === undefined) {
//      Hwi.undefinedInstFunc = '&c_int00';
    }

    if (Hwi.swiFunc === undefined) {
        Hwi.swiFunc = '&ti_sysbios_family_arm_HwiCommon_swiHandler__I';
    }

    if (Hwi.prefetchAbortFunc === undefined) {
//      Hwi.prefetchAbortFunc = '&c_int00';
    }

    if (Hwi.dataAbortFunc === undefined) {
//      Hwi.dataAbortFunc = '&c_int00';
    }

    if (Hwi.reservedFunc === undefined) {
//      Hwi.reservedFunc = '&c_int00';
    }

    if (Hwi.irqFunc === undefined) {
        Hwi.irqFunc = '&ti_sysbios_family_arm_omap1030_Hwi_dispatchIRQ';
    }

    if (Hwi.fiqFunc === undefined) {
        Hwi.fiqFunc = '&ti_sysbios_family_arm_omap1030_Hwi_dispatchFIQC__I';
    }
}

/*
 *  ======== module$use ========
 */
function module$use()
{
    Memory = xdc.useModule('xdc.runtime.Memory');

    xdc.useModule('xdc.runtime.Log');
    xdc.useModule("ti.sysbios.hal.Cache");
    xdc.useModule("ti.sysbios.family.arm.HwiCommon");

    var BIOS = xdc.useModule("ti.sysbios.BIOS");

    if (Hwi.dispatcherSwiSupport == undefined) {
        Hwi.dispatcherSwiSupport = BIOS.swiEnabled;
    }
    if (Hwi.dispatcherTaskSupport == undefined) {
        Hwi.dispatcherTaskSupport = BIOS.taskEnabled;
    }
    if (Hwi.dispatcherSwiSupport) {
        if (BIOS.swiEnabled) {
            xdc.useModule("ti.sysbios.knl.Swi");
            Hwi.swiDisable = '&ti_sysbios_knl_Swi_disable__E';
            Hwi.swiRestoreHwi = '&ti_sysbios_knl_Swi_restoreHwi__E';
        }
        else {
            Hwi.$logError("Dispatcher Swi support can't be enabled if ti.sysbios.BIOS.swiEnabled is false.", Hwi, "dispatcherSwiSupport");
        }
    }
    else {
        Hwi.swiDisable = null;
        Hwi.swiRestoreHwi = null;
    }

    if (Hwi.dispatcherTaskSupport) {
        if (BIOS.taskEnabled) {
            xdc.useModule("ti.sysbios.knl.Task");
            Hwi.taskDisable = '&ti_sysbios_knl_Task_disable__E';
            Hwi.taskRestoreHwi = '&ti_sysbios_knl_Task_restoreHwi__E';
        }
        else {
            Hwi.$logError ("Dispatcher Task support can't be enabled if ti.sysbios.BIOS.taskEnabled is false.", Hwi, "dispatcherTaskSupport");
        }
    }
    else {
        Hwi.taskDisable = null;
        Hwi.taskRestoreHwi = null;
    }

    /* place .vecs section */
    if (Program.sectMap[".vecs"] === undefined) {
        Program.sectMap[".vecs"] = new Program.SectionSpec();
        Program.sectMap[".vecs"].loadAddress = 0x20000000;
    }
}

/*
 *  ======== module$static$init ========
 */
function module$static$init(mod, params)
{
    mod.mir0Mask = params.mir0Mask & ~5;/* L2 IRQ & FIQ enabled by default */
    mod.mir1Mask = params.mir1Mask;
    mod.mir2Mask = params.mir2Mask;

    mod.irq0Mask = 0xfffffffb;          /* L2 FIQ is routed to FIQ */

    mod.irp = 0;

    mod.taskSP = null;

    /* 
     * ROM 
     * These members of the module state are added for ROM. They are tied to
     * their respective symbol name because the symbols will not be defined
     * at the time the ROM assembly is made.
     */
    mod.isrStack = null;
    mod.isrStackBase = $externPtr('__TI_STACK_BASE');
    mod.isrStackSize = $externPtr('__TI_STACK_SIZE');

    for (var intNum = 0; intNum < Hwi.NUM_INTERRUPTS; intNum++) {
        mod.dispatchTable[intNum] = null;
    }

    mod.fiqStackSize = params.fiqStackSize;

    if (params.fiqStack) {
        mod.fiqStack = params.fiqStack;
    }
    else {
        mod.fiqStack.length = params.fiqStackSize;
        Memory.staticPlace(mod.fiqStack, 4, params.fiqStackSection);
    }
}

/*
 *  ======== module$validate ========
 */
function module$validate()
{
    /* validate all "created" instances */
    for (var i = 0; i < Hwi.$instances.length; i++) {
        instance_validate(Hwi.$instances[i]);
    }

    /* validate all "constructed" instances */
    for (var i = 0; i < Hwi.$objects.length; i++) {
        instance_validate(Hwi.$objects[i]);
    }
}

/*
 *  ======== instance_validate ========
 *  common function to test instance configuration
 */
function instance_validate(instance)
{    
    if (instance.$object.fxn == null) {
        Hwi.$logError("function cannot be null", instance);
    }
}

/*
 *  ======== instance$static$init ========
 */
function instance$static$init(obj, intNum, fxn, params)
{
    // print ("In Hwi Instance init", intNum, fxn);

    var mod = this.$module.$object;

    if (Hwi.interrupt[intNum].used == true) {
        Hwi.$logError("Hwi " + intNum + " already in use (by " +
                Hwi.interrupt[intNum].fxn + ").", this);
    }

    Hwi.interrupt[intNum].used = true;
    Hwi.interrupt[intNum].useDispatcher = true;
    Hwi.interrupt[intNum].fxn = fxn;

    mod.dispatchTable[intNum] = this;
    obj.arg = params.arg;
    obj.fxn = fxn;
    obj.irp = null;

    obj.sense = params.sense;
    obj.type = params.type;
    obj.priority = params.priority;

    if ( intNum < 32 ) {
        if (params.type == Hwi.Type_IRQ) {
            mod.irq0Mask |= (1 << intNum);
        }
        else {
            mod.irq0Mask &= ~(1 << intNum);
        }
    }

    if (params.enableInt) {
        if ( intNum < 32 ) {
            mod.mir0Mask &= ~(1 << intNum);
        }
        else if ( intNum < 64 ) {
            intNum %= 32;        // normalize to 0 - 31
            mod.mir1Mask &= ~(1 << intNum);
        }
        else if ( intNum < 96 ) {
            intNum %= 32;        // normalize to 0 - 31
            mod.mir2Mask &= ~(1 << intNum);
        }
    }

    obj.hookEnv.length = Hwi.hooks.length;
}

/*
 *  ======== inUseMeta ========
 */
function inUseMeta(intNum)
{
    return Hwi.interrupt[intNum].used;
}

/*
 *  ======== addHookSet ========
 */
function addHookSet(hookSet)
{
    /* use "===" so 'null' is not flagged */
    if (hookSet.registerFxn === undefined) {
        hookSet.registerFxn = null;
    }
    if (hookSet.createFxn === undefined) {
        hookSet.createFxn = null;
    }
    if (hookSet.beginFxn === undefined) {
        hookSet.beginFxn = null;
    }
    if (hookSet.endFxn === undefined) {
        hookSet.endFxn = null;
    }
    if (hookSet.deleteFxn === undefined) {
        hookSet.deleteFxn = null;
    }

    this.hooks.$add(hookSet);
}

/*
 *  ======== viewInitBasic ========
 *  Initialize the 'Basic' Task instance view.
 */
function viewInitBasic(view, obj)
{    
    var Hwi = xdc.useModule('ti.sysbios.family.arm.omap1030.Hwi');
    var Program = xdc.useModule('xdc.rov.Program');
    var halHwi = xdc.useModule('ti.sysbios.hal.Hwi');

    view.halHwiHandle =  halHwi.viewGetHandle(obj.$addr);
    view.label = Program.getShortName(obj.$label);
    view.intNum = obj.intNum;
    view.priority = obj.priority;
    
    var fxn = Program.lookupFuncName(Number(obj.fxn));
    view.fxn = fxn[0];
    view.arg = obj.arg;
    view.irp = obj.irp;
}

/*!
 *  ======== viewGetStackInfo ========
 */
function viewGetStackInfo()
{
    var IHwi = xdc.useModule('ti.sysbios.interfaces.IHwi');

    var stackInfo = new IHwi.StackInfo();

    /* Fetch the Hwi stack */
    try {
        var size = Program.getSymbolValue("__STACK_SIZE");
        var stackBase = Program.getSymbolValue("__stack");
        var stackData = Program.fetchArray({type: 'xdc.rov.support.ScalarStructs.S_UChar', isScalar: true}, stackBase, size);
    }
    catch (e) {
        stackInfo.hwiStackSize = 0;     /* signal error to caller */
        return (stackInfo);
    }

    var index = 0;

    /* 
     * The stack is filled with 0xbe.
     */
    while (stackData[index] == 0xbe) {
        index++;
    }

    stackInfo.hwiStackPeak = size - index;
    stackInfo.hwiStackSize = size;
    stackInfo.hwiStackBase = stackBase;

    return (stackInfo);
}

/*
 *  ======== viewInitModule ========
 *  Initialize the Task module view.
 */
function viewInitModule(view, mod)
{
    var Program = xdc.useModule('xdc.rov.Program');
    var halHwiModCfg = Program.getModuleConfig('ti.sysbios.hal.Hwi');

    var hwiModCfg = Program.getModuleConfig('ti.sysbios.family.arm.omap1030.Hwi');

    view.options[0] = "Hwi.autoNestingSupport = ";
    view.options[1] = "Hwi.swiSupport = ";
    view.options[2] = "Hwi.taskSupport = ";
    view.options[3] = "Hwi.irpSupport = ";

    view.options[0] += hwiModCfg.dispatcherAutoNestingSupport ? "true" : "false";
    view.options[1] += hwiModCfg.dispatcherSwiSupport ? "true" : "false";
    view.options[2] += hwiModCfg.dispatcherTaskSupport ? "true" : "false";
    view.options[3] += hwiModCfg.dispatcherIrpTrackingSupport ? "true" : "false";

    var stackInfo = viewGetStackInfo();

    if (stackInfo.hwiStackSize == 0) {
        view.$status["hwiStackPeak"] =
        view.$status["hwiStackSize"] =
        view.$status["hwiStackBase"] = "Error fetching Hwi stack info!";
    }
    else {
        if (halHwiModCfg.initStackFlag) {
            view.hwiStackPeak = String(stackInfo.hwiStackPeak);
            view.hwiStackSize = stackInfo.hwiStackSize;
            view.hwiStackBase = "0x"+ stackInfo.hwiStackBase.toString(16);

            if (stackInfo.hwiStackPeak == stackInfo.hwiStackSize) {
                view.$status["hwiStackPeak"] = "Overrun!  ";
                /*                                  ^^  */
                /* (extra spaces to overcome right justify) */
            }
        }
	else {
            view.hwiStackPeak = "n/a - set Hwi.initStackFlag";
            view.hwiStackSize = stackInfo.hwiStackSize;
            view.hwiStackBase = "0x"+ stackInfo.hwiStackBase.toString(16);
	}
    }
}
