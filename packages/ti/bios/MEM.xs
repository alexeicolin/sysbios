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
 *  ======== MEM.xs ========
 *
 *                      say on section placement.
 *! 27-Mar-2008 jv      Fix SDSCM00024628 - place .sysmem to MEM.CIOSEG.
 *! 06-Mar-2008 toddm   Temp warning that sectMap is being over-written. Need 
 *!                     better long term solution...
 *! 19-Jul-2007 jv      For undefined memory segments set it to "code/data".
 *! 09-Jul-2007 toddm   Set HeapMem's gate
 *! 20-Jun-2007 jv      Fix the init of MEM segments from Program object.
 *! 15-Jun-2007 jv      Added getHeapHandle function.
 *! 15-May-2007 jv      added revision history.
 *! xx-Jan-2007 sasa    created
 */

var bios6xDefaults = {};

var SectionNameMap = {
    "ARGSSEG" : {name:".args"},
    "STACKSEG" : {name:".stack"},
    "TEXTSEG" : {name:".text"},
    "DATASEG" : {name:".data"},
    "SWITCHSEG" : {name:".switch"},
    "BSSSEG" : {name:".bss"},
    "EBSSSEG" : {name:".ebss"},
    "FARSEG" : {name:".far"},
    "CINITSEG" : {name:".cinit"},
    "PINITSEG" : {name:".pinit"},
    "CONSTSEG" : {name:".const"},
    "ECONSTSEG" : {name:".econst"},
    "CIOSEG" : {name:".cio"},
    "LOADTEXTSEG" : {name:".text"},
    "LOADSWITCHSEG" : {name:".switch"},
    "LOADCINITSEG" : {name:".cinit"},
    "LOADPINITSEG" : {name:".pinit"},
    "LOADCONSTSEG" : {name:".const"},
    "LOADECONSTSEG" : {name:".econst"},
    "LOADHWISEG" : {name:".vecs"},
};

var unsupported = ["GBLINITSEG", "TRCDATASEG", "SYSDATASEG", "OBJSEG",
    "BIOSSEG", "SYSINITSEG", "HWISEG", "RTDXTEXTSEG", "LOADBIOSSEG",
    "LOADTRCDATASEG", "LOADGBLINITSEG", "LOADSYSINITSEG",  "LOADHWISEG",
    "LOADRTDXTEXTSEG", "LOADTEXTSEG", "TEXTSEG", "LOADSWITCHSEG", 
    "SWITCHSEG", "LOADCINITSEG", "CINITSEG", "LOADPINITSEG", "PINITSEG",
    "LOADCONSTSEG", "CONSTSEG", "LOADHWIVECSEG", "HWIVECSEG", "ARGSSEG",
    "DATASEG", "STACKSEG", "BSSSEG", "FARSEG", "CIOSEG"]

/*
 *  ======== inst ========
 */
function inst(instName)
{
    /*
     * If the named memory instance doesn't exist, create it. We are not going
     * to use such instances anyway, because they are later destroyed,
     * assuming the underlying TCF script is valid.
     */
    for (var i = 0; i < this.$instances.length; i++) {
        if (this.$instances[i].name == instName) {
            return (this.$instances[i]);
        }
    }

    var temp = this.create(instName, {name: instName})
    return (temp);
}

/*
 *  ======== module$meta$init ========
 *  Initializes the default section placement
 */
function module$meta$init()
{
    /* Only process during "cfg" phase */
    if (xdc.om.$name != "cfg") {
        return;
    }

    this.$$bind('instance', this.inst);

    /* All supported MEM properties should get an initial value here. We can't
     * have the default value in MEM.xdc because those values differ for
     * different platforms.
     */
    var data = null;
    var code = null;
    var stack = null;
    var hwivec = null;
    var data1 = null;
    var code1 = null;
    var stack1 = null;
    var hwivec1 = null;
    
    var memmap = Program.cpu.memoryMap;
    var codeSeg = Program.platform.codeMemory;
    var dataSeg = Program.platform.dataMemory;
    var stackSeg = Program.platform.stackMemory;
    
    if (Program.build.target.name == "C28_large" || 
        Program.build.target.name == "C28_float") {
        /* 
         * C28 is the only target that requires specific placement of
         * interrupt vectors (into PIEVECT)
         */
        var hwiVecSeg = "PIEVECT";
    }
    else {
        var hwiVecSeg = Program.platform.codeMemory;
    }

    for (var i=0; i < memmap.length; i++) {
        /*
        print("name = " + memmap[i].name +
              ", base = 0x" + memmap[i].base.toString(16) +
              ", len = 0x" + memmap[i].len.toString(16) +
              ", space = " + memmap[i].space);
        */

        /* some platform memory segments do not have 'space' defined */
        if (memmap[i].space == undefined) {
            memmap[i].space = "code/data";
        }

        var seg = this.create(memmap[i].name, {
                name: memmap[i].name,
                base: memmap[i].base,
                len: memmap[i].len,
                space: memmap[i].space,
        });
        
        if (codeSeg == seg.name) {
            code = seg;
        }
        else if (memmap[i].space == "code" || memmap[i].space == "code/data") {
            code1 = seg;
        }

        if (dataSeg == seg.name) {
            data = seg;
        }
        else if (memmap[i].space == "data" || memmap[i].space == "code/data") {
            data1 = seg;
        }
        
        if (stackSeg == seg.name) {
            stack = seg;
        }
        else if (memmap[i].space == "data" || memmap[i].space == "code/data") {
            stack1 = seg;
        }
        
        if (hwiVecSeg == seg.name) {
            hwivec = seg;
        }
        else if (memmap[i].space == "code" || memmap[i].space == "code/data") {
            hwivec1 = seg;
        }
    }

    if (code == null) {
        this.$logWarning("Default code section not set. Setting to ", code1);
        code = code1;
    }
    if (data == null) {
        this.$logWarning("Default data section not set. Setting to ", data1);
        data = data1;
    }
    if (stack == null) {
        this.$logWarning("Default stack section not set. Setting to ", stack1);
        stack = stack1;
    }
    if (hwivec == null) {
        this.$logWarning("Default hwi vectors section not set. Setting to ", hwivec1);
        hwivec = hwivec1;
    }

    switch (Program.build.target.isa) {
        case "62":
        case "67":
            this.instance("IPRAM0");
            this.instance("IPRAM1");
            this.instance("IDRAM");
            this.instance("SBSRAM");
            this.instance("SDRAM");
            this.instance("SDRAM0");
            this.instance("SDRAM1");
            break;
        case "64":
        case "67P":
            this.instance("IRAM");
            this.instance("SDRAM");
            this.instance("DDR");
            break;
        case "64P":
            this.instance("IRAM");
            this.instance("ARM_RAM");
            this.instance("L1DSRAM");
            this.instance("L1PSRAM");
            this.instance("L2RAM");
            this.instance("LL2RAM");
            this.instance("SL2RAM");
            this.instance("DDR");
            this.instance("DDR2");
            this.instance("EMIFA");
            this.instance("EMIFB");
            this.instance("SRAM");
            break;
        case "674":
            this.instance("IRAM");
            this.instance("L1DSRAM");
            this.instance("L1PSRAM");
            this.instance("SDRAM0");
            this.instance("SDRAM1");
            break;
        case "28":
            this.instance("MSARAM");    
            this.instance("PIEVECT");    
            this.instance("L03SARAM");    
            this.instance("L47SARAM");    
            this.instance("OTP");    
            this.instance("SRAM"); 
            break;
    }

    var memnull = this.instance("MEM_NULL");

    this.MALLOCSEG = memnull;
    this.BIOSOBJSEG = memnull;

    /* 
     * get hold of 6x module. Make sure legacy module is brought in first
     * scroll away 6xMod defaults that need to be changed
     */

    var HeapMem = xdc.module('ti.sysbios.heaps.HeapMem');
    this.$private.bios6xMod = HeapMem;

    if (HeapMem.$used) {
        this.$logError("This module should be used BEFORE " + 
        HeapMem.$name, this);
    }
    bios6xDefaults["stack"] = Program.stack;
    bios6xDefaults["argSize"] = Program.argSize;
    
}

/*
 *  ======== instance$meta$init ========
 *  Initialize instance name parameter
 */
function instance$meta$init(name)
{
    /* If an instance with the same name already exists, do nothing */
    if (name.indexOf('_') == 0) {
        name = name.substr(1);
    }
    this.name = name;
}

/*
 *  ======== close ========
 *  Sets the Program.sectMap
 */
function close()
{
    if (this.NOMEMORYHEAPS != true) {
        var Startup = xdc.module("xdc.runtime.Startup");
        Startup.firstFxns.$add('&MEM_init');
    }

    setConfig(Program, "stack", this.STACKSIZE);
    setConfig(Program, "argSize", this.ARGSSIZE);

    if (this.USERCOMMANDFILE == true) {
        this.$logWarning("bios.MEM.USERCOMMANDFILE is not supported.", this, null);
    }

    for (var i = 0; i < unsupported.length; i++) {
        if (this[unsupported[i]] != undefined) {
            this.$logWarning("bios.MEM." + unsupported[i] + 
            " is not supported.", this, null);

            if (SectionNameMap[unsupported[i]] != undefined) {
                this.$logWarning('Replace with Program.sectMap["' 
                + SectionNameMap[unsupported[i]].name + '"] = "' + 
                this[unsupported[i]].name + '";', this, null)
            }
        }
    }
}

/*
 *  ======== destroy ========
 *  Ensure that heap is disabled.
 *
 */
function destroy()
{
    /* Just ensure that heap is not set, we don't care about rest */
    this.createHeap = 0;
}

/*
 *  ======== getHeapHandle ========
 */
function getHeapHandle(segment)
{
    var HeapMem = xdc.module('ti.sysbios.heaps.HeapMem');
    for (var i = 0; i < HeapMem.$instances.length; i++) {
        if (HeapMem.$instances[i].sectionName == "_" + segment + "$heap") {
            return (HeapMem.$instances[i])
        }
    }
    return null;
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
            var MEM = xdc.module("ti.bios.MEM");
            MEM.$logWarning("Cannot change " + mod.$name + "." + configParam + 
            ". Parameter changed elsewhere", MEM);
        }
    }
}


/*
 * ======== addHeapMem ========
 */
function addHeapMem(handle, name)
{
    var MEM = xdc.module('ti.bios.MEM');
    var memPrms = new MEM.Params();
    memPrms.createHeap = true;
    memPrms.heapMemInst = handle;
    if (name) {
        memPrms.enableHeapLabel = true;
        memPrms.heapLabel = name;
    }

    var mem = MEM.create(name, memPrms);

    return(mem);
}
