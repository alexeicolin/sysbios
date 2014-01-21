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
var Exception = null;

if (xdc.om.$name == "cfg") {
    var deviceTable = {
        "Vayu": {
            numInterrupts      : 192,
            numPriorityBits    : 4,
            useCodeMemory      : true,
            vectorTableAddress : null,
            gicdBaseAddress    : 0x48211000,
            giccBaseAddress    : 0x48212000
        },
        "OMAP5430": {
            numInterrupts      : 192,
            numPriorityBits    : 4,
            useCodeMemory      : true,
            vectorTableAddress : null,
            gicdBaseAddress    : 0x48211000,
            giccBaseAddress    : 0x48212000
        },
        "OMAP4430": {
            numInterrupts      : 160,
            numPriorityBits    : 4,
            useCodeMemory      : true,
            vectorTableAddress : null,
            gicdBaseAddress    : 0x48241000,
            giccBaseAddress    : 0x48240100
        },
        "AM437X": {
            numInterrupts      : 256,
            numPriorityBits    : 4,
            useCodeMemory      : true,
            vectorTableAddress : null,
            gicdBaseAddress    : 0x48241000,
            giccBaseAddress    : 0x48240100
        }
    };

    deviceTable["DRA7XX"] = deviceTable["Vayu"];
}


/*
 *  ======== deviceSupportCheck ========
 *  Check validity of device
 */
function deviceSupportCheck() 
{
    /* look for exact match first */
    for (device in deviceTable) {
        if (device == Program.cpu.deviceName) {
            return device;
            }
    }

    /* now look for a wildcard match */
    for (device in deviceTable) {
        if (Program.cpu.deviceName.match(device)) {
            return device;
            }
    }

    /* 
     * no match, print all catalog devices supported 
     * and then raise an error 
     */
    var catalog = xdc.loadPackage(Program.cpu.catalogName);

    /* build associative array of supported devices */
    var supportedDevices = new Object();

    for (var i = 0; i < catalog.$modules.length; i++) {
        catalogName = catalog.$modules[i].$name.substring(Program.cpu.catalogName.length+1);
        for (device in deviceTable) {
            if (catalogName.match(device)) {
                supportedDevices[catalogName] = catalogName;
            }
        }
    }

    /* copy it into a sortable array */
    var sd = new Array();

    for (var i in supportedDevices) {
        sd[sd.length++] = i;
    }

    /* sort it for a nicer report */
    sd.sort();

    print(
            "The " + 
            Program.cpu.deviceName + 
            " device is not currently supported."
         );

    print(
            "The following devices are supported for the " + 
            Program.build.target.name + 
            " target:"
         );

    for (var i = 0; i < sd.length; i++) {
        print("\t" + sd[i]);
    }

    throw new Error ("Unsupported device!");
}

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

    Exception = xdc.module('ti.sysbios.family.arm.exc.Exception');

    var device = deviceSupportCheck();

    Hwi.NUM_INTERRUPTS = deviceTable[device].numInterrupts;
    Hwi.NUM_PRIORITY_BITS = deviceTable[device].numPriorityBits;
    Hwi.gicdBaseAddress = deviceTable[device].gicdBaseAddress;
    Hwi.giccBaseAddress = deviceTable[device].giccBaseAddress;

    var Mask = 1 << (8 - Hwi.NUM_PRIORITY_BITS);
    Hwi.MIN_INT_PRIORITY = 0xFF & (~(Mask - 1));
    Hwi.DEFAULT_INT_PRIORITY = Hwi.MIN_INT_PRIORITY ^ Mask;

    /*
     * Initialize meta-only Hwi object array
     */

    Hwi.interrupt.length = Hwi.NUM_INTERRUPTS;
    for (var intNum = 0; intNum < Hwi.NUM_INTERRUPTS; intNum++) {
        Hwi.interrupt[intNum].used = false;
        Hwi.interrupt[intNum].fxn = null;
    }

    Hwi.resetFunc = '&_c_int00';

    Hwi.undefinedInstFunc = Exception.excHandlerAsm;

    Hwi.svcFunc = Exception.excHandlerAsm;

    Hwi.prefetchAbortFunc = Exception.excHandlerAsm;

    Hwi.dataAbortFunc = Exception.excHandlerDataAsm;

    Hwi.reservedFunc = Exception.excHandlerAsm;

    Hwi.irqFunc = Hwi.dispatch;

    Hwi.fiqFunc = Exception.excHandlerAsm;
}

/*
 *  ======== module$use ========
 */
function module$use()
{
    Memory = xdc.useModule('xdc.runtime.Memory');
    Reset = xdc.useModule('xdc.runtime.Reset');

    xdc.useModule('xdc.runtime.Log');
    xdc.useModule('ti.sysbios.hal.Cache');
    xdc.useModule('ti.sysbios.family.arm.exc.Exception');

    var BIOS = xdc.useModule('ti.sysbios.BIOS');

    if (Hwi.dispatcherSwiSupport == undefined) {
        Hwi.dispatcherSwiSupport = BIOS.swiEnabled;
    }
    if (Hwi.dispatcherTaskSupport == undefined) {
        Hwi.dispatcherTaskSupport = BIOS.taskEnabled;
    }
    if (Hwi.dispatcherSwiSupport) {
        if (BIOS.swiEnabled) {
            xdc.useModule('ti.sysbios.knl.Swi');
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
            xdc.useModule('ti.sysbios.knl.Task');
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
        if (deviceTable[device].useCodeMemory == true) {
            Program.sectMap[".vecs"].loadSegment = 
                Program.platform.codeMemory;
        }
        else {
            Program.sectMap[".vecs"].loadAddress = 
                deviceTable[device].vectorTableAddress;
        }
    }

    /* Install Hwi_init as a reset function */
    Reset.fxns[Reset.fxns.length++] = "&ti_sysbios_family_arm_gic_Hwi_init__I";
}

/*
 *  ======== module$static$init ========
 */
function module$static$init(mod, params)
{
    var reg;

    for (reg = 0; reg < 32; reg++) {
        mod.iser[reg] = 0;
    }

    mod.icfgr.length = Hwi.NUM_INTERRUPTS / 16;
    if (Program.build.target.name == "A15") {
        mod.icfgr[0] = 0xAAAAAAAA;
        mod.icfgr[1] = 0x55540000;
        for (reg = 2; reg < (Hwi.NUM_INTERRUPTS / 16); reg++) {
            mod.icfgr[reg] = 0x55555555;
        }
    }
    else {
        for (reg = 0; reg < (Hwi.NUM_INTERRUPTS / 16); reg++) {
            mod.icfgr[reg] = 0x55555555;
        }
    }

    mod.itargetsr.length = Hwi.NUM_INTERRUPTS / 4;
    for (reg = 0; reg < (Hwi.NUM_INTERRUPTS / 4); reg++) {
        mod.itargetsr[reg] = 0x01010101;
    }

    mod.spuriousInts = 0;
    mod.lastSpuriousInt = 0;

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

    var hwiParams = new Hwi.Params();
    Hwi.construct(mod.nonPluggedHwi, 0, Hwi.nonPluggedHwiHandler, hwiParams);

    mod.dispatchTable.length = Hwi.NUM_INTERRUPTS;

    for (var intNum = 0; intNum < Hwi.NUM_INTERRUPTS; intNum++) {
        mod.dispatchTable[intNum] = mod.nonPluggedHwi;
    }

    mod.curIntId = ~(0);
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
    var mod = this.$module.$object;

    if (intNum >= Hwi.NUM_INTERRUPTS) {
        Hwi.$logError("intnum " + intNum + " is out of range!", this);
    }

    if (Hwi.interrupt[intNum].used == true) {
        Hwi.$logError("Hwi " + intNum + " already in use (by " +
                Hwi.interrupt[intNum].fxn + ").", intNum);
    }

    if (obj.fxn != Hwi.nonPluggedHwiHandler) {
        Hwi.interrupt[intNum].used = true;
    }

    Hwi.interrupt[intNum].fxn = fxn;

    mod.dispatchTable[intNum] = this;
    obj.arg = params.arg;
    obj.fxn = fxn;
    obj.irp = null;
    obj.type = params.type;
    obj.triggerSensitivity = params.triggerSensitivity;
    obj.targetProcList = params.targetProcList;

    /* Set interrupt trigger sensitivity */
    var shift = (intNum & 0xF) << 1;
    var mask = 0x3 << shift;
    var newIcfgr = mod.icfgr[intNum >>> 4] & (~mask);
    mod.icfgr[intNum >>> 4] = newIcfgr | 
        ((obj.triggerSensitivity & 0x3) << shift);

    /* Set interrupt target processors */
    shift = (intNum & 0x3) << 3;
    mask = 0xF << shift;
    var newItargetsr = mod.itargetsr[intNum >>> 2] & (~mask);
    mod.itargetsr[intNum >>> 2] = newItargetsr | 
        ((obj.targetProcList & 0xF) << shift);

    if (params.priority == -1) {
        obj.priority = Hwi.DEFAULT_INT_PRIORITY;
    }
    else {
        obj.priority = params.priority;
    }

    if (params.enableInt) 
    {
        mod.iser[intNum >>> 5] |= (1 << (intNum & 0x1f));
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
    var Hwi = xdc.useModule('ti.sysbios.family.arm.gic.Hwi');
    var Program = xdc.useModule('xdc.rov.Program');
    var halHwi = xdc.useModule('ti.sysbios.hal.Hwi');
    var modCfg = Program.getModuleConfig('ti.sysbios.family.arm.gic.Hwi');
    var SubPriorityMask = (1 << (modCfg.BPR + modCfg.NUM_PRIORITY_BITS - 7)) - 1;
    view.halHwiHandle =  halHwi.viewGetHandle(obj.$addr);
    view.label = Program.getShortName(obj.$label);
    view.absolutePriority = "0x" + 
        Number(obj.priority).toString(16).toUpperCase();
    view.relativeGrpPriority = (obj.priority >> (modCfg.BPR + 1));
    view.relativeSubPriority = (obj.priority >> (8 - modCfg.NUM_PRIOIRTY_BITS))
        & SubPriorityMask;
    var fxn = Program.lookupFuncName(Number(obj.fxn));
    view.fxn = fxn[0];
    view.arg = obj.arg;

    try {
        var hwiRawView = Program.scanRawView('ti.sysbios.family.arm.gic.Hwi');
        var mod = hwiRawView.modState;
        var dispatchTable = Program.fetchArray(mod.dispatchTable$fetchDesc, 
            mod.dispatchTable, modCfg.NUM_INTERRUPTS);

        for (var i=0; i<modCfg.NUM_INTERRUPTS; i++) {
            if (dispatchTable[i] == view.address) {
                view.intNum = i;
                break;
            }
        }
    }
    catch (e) {
        view.$status["intNum"] = "Unable to obtain intNum: " + e;
    }
}

/*
 *  ======== viewGicdFetch ========
 *  Once per halt, fetch current gicd contents
 *  Called from viewInitModule()
 */
function viewGicdFetch(that)
{
    var Hwi = xdc.useModule('ti.sysbios.family.arm.gic.Hwi');
    var modCfg = Program.getModuleConfig('ti.sysbios.family.arm.gic.Hwi');
    try {
        if (that.GICD === undefined) {
            that.GICD = Program.fetchStruct(
                                                Hwi.Gicd$fetchDesc,
                                                modCfg.gicdBaseAddress,
                                                false
                                           );
        }
    }
    catch (e) {
        print("Error: Problem fetching GIC Registers: " + e.toString());
    }
}

/*
 *  ======== viewInitDetailed ========
 *  Initialize the 'Detailed' Task instance view.
 */
function viewInitDetailed(view, obj)
{   
    var Hwi = xdc.useModule('ti.sysbios.family.arm.gic.Hwi');
    var Program = xdc.useModule('xdc.rov.Program');
    
    /* Detailed view builds off basic view. */
    viewInitBasic(view, obj);

    view.irp = obj.irp;

    viewGicdFetch(this);

    var enabled = false;
    var pending = false;
    var intNum = view.intNum;

    enabled = this.GICD.ISENABLER[intNum >>> 5] & (1 << intNum);
    pending = this.GICD.ISPENDR[intNum >>> 5] & (1 << intNum);

    if (enabled) {
        view.enabled = true;
    }
    else {
        view.enabled = false;
    }

    if (pending) {
        view.pending = true;
    }
    else {
        view.pending = false;
    }

    var icfgr = (this.GICD.ICFGR[intNum >>> 4] >>> ((intNum & 0xF) << 1)) & 0x3;
    view.triggerSensitivity = "b" + icfgr.toString(2);
    var itargetsr = 
        (this.GICD.ITARGETSR[intNum >>> 2] >>> ((intNum & 0x3) << 3)) & 0xF;
    view.targetProcList = "b" + itargetsr.toString(2);
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
        var stackData = Program.fetchArray(
            {type: 'xdc.rov.support.ScalarStructs.S_UChar', isScalar: true},
            stackBase, size);
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
    var hwiModCfg = Program.getModuleConfig('ti.sysbios.family.arm.gic.Hwi');

    viewGicdFetch(this);

    view.options[0] = "Hwi.autoNestingSupport = ";
    view.options[1] = "Hwi.swiSupport = ";
    view.options[2] = "Hwi.taskSupport = ";
    view.options[3] = "Hwi.irpSupport = ";

    view.options[0] += hwiModCfg.dispatcherAutoNestingSupport ? "true" : "false";
    view.options[1] += hwiModCfg.dispatcherSwiSupport ? "true" : "false";
    view.options[2] += hwiModCfg.dispatcherTaskSupport ? "true" : "false";
    view.options[3] += hwiModCfg.dispatcherIrpTrackingSupport ? "true" : "false";

    view.spuriousInterrupts = mod.spuriousInts;
    view.lastSpuriousInterrupt = mod.lastSpuriousInt;

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
