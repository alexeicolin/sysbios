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
 *  ======== package.xs ========
 *
 *
 */

/*
 *  ======== init ========
 */
function init()
{
    /* Only process during "cfg" phase */
    if (xdc.om.$name != "cfg") {
        return;
    }

    /* This package is built only for c6x and c28x targets */
    if (Program.cpu.catalogName != "ti.catalog.c6000" &&
            Program.cpu.catalogName != "ti.catalog.c2800") { 
        throw new Error(
           "The ti.bios package contains support for the legacy BIOS 5.x " +
           "APIs.  These APIs are only supported for the c6x and c28x. " +
           "The " + Program.cpu.catalogName + " devices are not supported." 
        );
    }

    Program.$logWarning("Support for the legacy DSP/BIOS 5.x APIs " +
        "has been deprecated. " +
        "These APIs may not be supported in a future release of SYS/BIOS. " +
        "Please update your code to use the equivalent SYS/BIOS APIs.", this);

    /* function to return the instance with the specified name */
    function inst(name)
    {
        for (var i = 0; i < this.$instances.length; i++) {
            if (this.$instances[i].name == name) {
                return (this.$instances[i]);
            }
        }
    }

    /* function to return all the instances of a module */
    function getInstances()
    {
        return (this.$instances);
    }


    var biosPkg = xdc.loadPackage("ti.bios");

    for (var j = 0; j < biosPkg.$modules.length; j++) {
        var mod = xdc.module(biosPkg.$modules[j].$name);
        mod.$$bind('instance', inst);
        mod.$$bind('instances', getInstances);
    }
}

/*
 *  ======== close ========
 *  Call the close function of each module if they haven't been called.
 *  This function is necessary when ti.bios. package is not used.
 *  This function should only execute once.
 */
function close()
{
    /* Only process during "cfg" phase */
    if (xdc.om.$name != "cfg") {
        return;
    }

    var MEM = xdc.module('ti.bios.MEM');
    
    var memInstances = MEM.$instances;

    /* When heaps are created, we have to select a heap manager explicitly.
     * There is no default one.
     */
    var HeapMem = xdc.module('ti.sysbios.heaps.HeapMem');
    var Memory = xdc.module('xdc.runtime.Memory');

    /* Assign ids to heaps starting with one. Zero is special */
    var heapId = 1;  

    /* 
     * Go through all MEM instances and create HeapMems if they don't
     * have an associated HeapMem already
     */
    for (var i = 0; i < memInstances.length; i++) {
        /* Does this MEM segment have a heap? */
        if (memInstances[i].createHeap == true) {

            /* 
             *  If the heapLabel is specified, use it. 
             *  Otherwise use segment name.
             */
            if (memInstances[i].heapLabel == null) {
                memInstances[i].heapLabel = memInstances[i].name;
            }

            /* Assign a segid */
            memInstances[i].heapId = heapId;

            if (!memInstances[i].heapMemInst) {
                /* 
                 * Create a heap and change sect map, so that this heap will 
                 * end up in the right physical memory.
                 */
                memInstances[i].heapMemInst = HeapMem.create({
                        size: memInstances[i].heapSize, 
                        sectionName: "_" + memInstances[i].name + "$heap"});

                Program.sectMap["_" + memInstances[i].name + "$heap"] =
                                {loadSegment: memInstances[i].name};
            }

            var hp = memInstances[i].heapMemInst;

            /* If this is the malloc heap, set the defaultHeapinstance */
            if (this.MEM.MALLOCSEG.name == memInstances[i].name) {
                if (!Program.$capsule._romAsm) {
                    if (!Memory.defaultHeapInstance) {
                        Memory.defaultHeapInstance = hp;
                            memInstances[i].heapId = 0;
                        heapId--;
                    }
                    else {
                        if (Memory.defaultHeapInstance != hp) {
                            Program.$logWarning("Setting Memory.defaultHeapInstance to " + memInstances[i].name + " failed. Parameter is sealed or already set in cfg flow", this);
                        }
                    }
                }
            }
            
            if (this.MEM.BIOSOBJSEG.name == memInstances[i].name) {
                var Defaults = xdc.module('xdc.runtime.Defaults');

                if (Defaults.common$.instanceHeap == null) {
                    Defaults.common$.instanceHeap = hp;
                }
                else {
                    Program.$logWarning("Failed to set" +
                    " Defaults.common$.instanceHeap setting with " + memInstances[i].name + ". Parameter already set in cfg flow", this);
                }
                
                if (this.MEM.MALLOCSEG == undefined) {
                    if (!Program.$capsule._romAsm) {
                        if (!Memory.defaultHeapInstance) {
                            Memory.defaultHeapInstance = hp;
                                memInstances[i].heapId = 0;
                            heapId--;
                        }
                        else {
                            if (Memory.defaultHeapInstance != hp) {
                                Program.$logWarning("Setting Memory.defaultHeapInstance to " +  memInstances[i].name + " failed. Parameter is sealed or already set in cfg flow", this);
                            }
                        }
                    }
                }
            }

            heapId++;
        }
    }


    MEM.heapList.length = heapId;

    /* There are heaps, build the heapList table for the template file */
    for (var i = 0; i < memInstances.length; i++) {                    
        
        if (memInstances[i].createHeap == true) {
            
            MEM.heapList[memInstances[i].heapId].heapLabel = 
                memInstances[i].heapLabel;
           
            /* make sure there are no duplicates in MEM.heapList */
            for (var j = 0; j < MEM.heapList.length; j++) {
                if (getHeapMemIndex(memInstances[i].heapMemInst) ==
                    MEM.heapList[j].heapMemIndex) {
                    
                    Program.$logFatal("HeapMem instance found more than once in MEM.heapList", this);
                }
            }

            MEM.heapList[memInstances[i].heapId].heapMemIndex = 
                getHeapMemIndex(memInstances[i].heapMemInst);
        }
    }

    /* In BIOS 5, OBJMEMSEG would point to a segment for static instances.
     * The dynamic instances go to BIOSOBJSEG. So, if OBJMEMSEG is set, we need
     * to use it for Module.instanceSection.
     */
    for (var j = 0; j < this.$modules.length; j++) {
        var Mod = this.$modules[j];
        var bios6xMod = Mod.$private.bios6xMod;
        if ("OBJMEMSEG" in Mod && Mod.OBJMEMSEG != null && bios6xMod != undefined) {
            if (!bios6xMod.common$.instanceSection) {
                bios6xMod.common$.instanceSection = "_" + Mod.$name;
                Program.sectMap["_" + Mod.$name] = Mod.OBJMEMSEG.name;
            }
            else if (bios6xMod.common$.instanceSection != "_" + Mod.$name) {
                Program.$logWarning("Failed to set " + bios6xMod +
                    ".common$.instanceSection. " +
                    "Parameter already set in cfg flow", this);
            }
        }
    }
    
    var biosPkg = xdc.loadPackage('ti.bios');
    var biosMods = biosPkg.$modules;
    
    /* the order of close() functions is important. See SDSCM00027493 */
    if (this.GBL.$used){
        this.GBL.close();
    }
    if (this.MEM.$used){
        this.MEM.close();
    }
    if (this.BUF.$used){
        this.BUF.close();
    }
    if (this.SYS.$used){
        this.SYS.close();
    }
    if (this.LOG.$used){
        this.LOG.close();
    }
    if (this.TSK.$used){
        this.TSK.close();
    }
    if (this.CLK.$used){
        this.CLK.close();
    }
    if (this.SEM.$used){
        this.SEM.close();
    }
    if (this.SWI.$used){
        this.SWI.close();
    }
    if (this.IDL.$used){
        this.IDL.close();
    }
    if (this.MBX.$used){
        this.MBX.close();
    }
    if (this.STS.$used){
        this.STS.close();
    }
    if (this.PRD.$used){
        this.PRD.close();
    }
    if (this.ECM.$used){
        this.ECM.close();
    }
    if (this.HWI.$used){
        this.HWI.close();
    }
    /* PIE is a C2000 only module */
    if ((Program.build.target.isa == "28") ||
            (Program.build.target.isa == "28FP")) {
        if (this.PIE.$used){
            this.PIE.close();
        }
    }
    if (this.LCK.$used){
        this.LCK.close();
    }
    if (this.HOOK.$used){
        this.HOOK.close();
    }
    if (this.MSGQ.$used){
        this.MSGQ.close();
    }
    if (this.POOL.$used){
        this.POOL.close();
    }
    if (this.STATICPOOL.$used){
        this.STATICPOOL.close();
    }
    if (this.UDEV.$used){
        this.UDEV.close();
    }
    if (this.DGN.$used){
        this.DGN.close();
    }
    if (this.DPI.$used){
        this.DPI.close();
    }
    if (this.DIO.$used){
        this.DIO.close();
    }
    if (this.SIO.$used){
        this.SIO.close();
    }
    if (this.GIO.$used){
        this.GIO.close();
    }
    if (this.DHL.$used){
        this.DHL.close();
    }
    if (this.DEV.$used){
        this.DEV.close();
    }
    /* modules that are not being supported */ 
    if (this.PIP.$used){
        this.PIP.close(); 
    }
    if (this.HST.$used){
        this.HST.close(); 
    }
    if (this.RTDX.$used){
        this.RTDX.close();
    }

    Program.exportModule("xdc.runtime.Memory");
    Program.exportModule("xdc.runtime.Error");
    
    /* HeapMem module is required */
    Program.exportModule("ti.sysbios.heaps.HeapMem");

    /* Some of the modules in this package are using SYS api's */
    Program.exportModule("ti.bios.support.Sys");

    for (var i = 0; i < biosMods.length; i++) {
        var mod = xdc.module(biosMods[i].$name);;
        if (mod.$used && ("exportMod" in mod)) {
            mod.exportMod();
        }
    }
}


/*
 *  ======== Package.getLibs ========
 *  This function is called when a program's configuration files are
 *  being generated and it returns the name of a library appropriate
 *  for the program's configuration.
 */

function getLibs(prog)
{
    var name = this.$name + ".a" + prog.build.target.suffix;
    var lib = "";

    lib = "lib/" + "release/" + name;

    return lib;
}

/*  ======== internal.wrapperFunc ========
 *  For certain BIOS 5 functions used as function pointer parameters, we need to
 *  create wrapper functions. The original functions have signatures that differ
 *  from signatures generated by XDC config.
 */
internal = {};

internal.wrapperFunc = function (funcName) 
{
    if (funcName in internal.wrapperTable) {
        return (internal.wrapperTable[funcName]);        
    }
    else {
        return (null);
    }
}

internal.wrapperTable = {
    "PRD_tick": {args: 0},        
    "SEM_post": {args: 1, types: ["SEM_Handle"]},        
    "SEM_postBinary": {args: 1, types: ["SEM_Handle"]},        
    "SWI_andn": {args: 2, types: ["SWI_Handle", "Uns"]},
    "SWI_andnHoook": {args: 2, types: ["SWI_Handle", "Arg"]},
    "SWI_inc": {args: 1, types: ["SWI_Handle"]},        
    "SWI_or": {args: 2, types: ["SWI_Handle", "Uns"]},        
    "SWI_orHook": {args: 2, types: ["Arg", "Arg"]},        
    "SWI_post": {args: 1, types: ["SWI_Handle"]},        
    "TSK_yield": {args: 0},        
    "TSK_tick": {args: 0},        
    "TSK_itick": {args: 0}
}


function getHeapMemIndex(handle)
{
    var HeapMem = xdc.module("ti.sysbios.heaps.HeapMem");
    for (var i = 0; i < HeapMem.$instances.length; i++)
    {
        if (HeapMem.$instances[i] == handle) {
            return (i);
        }
    }

    Program.$logError("Failed to find index for HeapMem handle", this);
}

function validate()
{
    var Memory = xdc.module('xdc.runtime.Memory');
    var MEM = xdc.module('ti.bios.MEM');

    /* Ensure that Memory.defaultHeapInstance is a HeapMem instance */
    if ((MEM.NOMEMORYHEAPS == false) && 
        (Memory.defaultHeapInstance.$super.$module.$name != 
            "ti.sysbios.heaps.HeapMem")) 
    {
        
        if ((MEM.MALLOCSEG.name == "MEM_NULL") || 
            (MEM.BIOSOBJSEG.name == "MEM_NULL")) {
            Program.$logFatal(" MEM.MALLOCSEG and MEM.BIOSOBJSEG are not set. Both these parameters have to be set or Memory.defaultHeapInstance must be set to a HeapMem instance", this);
        }
        else {
            Program.$logFatal(" Memory.defaultHeapInstance must be a HeapMem instance. Please ensure that the segment assigned to MEM.MALLOCSEG and MEM.BIOSOBJSEG has property createHeap = true", this);
        }

    }
}

/*
 *  ======== Package.getSects ========
 */
function getSects()
{
    return "ti/bios/linkcmd.xdt";
}
