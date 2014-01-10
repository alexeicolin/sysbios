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
 *  ======== ECM.xs ========
 *
 */

var bios6xDefaults = {};
 
/*
 *  ======== module$meta$init ========
 *  Create pre-created objects from BIOS 5.x
 */
function module$meta$init(name)
{
    /* Only process during "cfg" phase */
    if (xdc.om.$name != "cfg") {
        return;
    }

    /* Create the 128 event objects */
    for (var i=0; i < 128; i++) {
        this.create("EVENT" + i);
    }

    /* 
     * get hold of 6x module. Make sure legacy module is brought in first
     * scroll away 6xMod defaults that need to be changed
     */
    /* only availabe on the C64P target */
    if (Program.build.target.isa == "64P" ||
        Program.build.target.isa == "64T" ||
        Program.build.target.isa == "66"  ||
        Program.build.target.isa == "674") {
        var bios6xMod = xdc.module('ti.sysbios.family.c64p.EventCombiner');
        this.$private.bios6xMod = bios6xMod;
    
        if (bios6xMod.$used) {
            this.$logError("This module should be used BEFORE " + 
            bios6xMod.$name, this);
        }
        
        bios6xDefaults["events"] = bios6xMod.events.$copy();
    }
}    

/*
 *  ======== instance$meta$init ========
 *  Initializes instance name to the param 'name'.
 */
function instance$meta$init(name)
{
    this.name = name;
}

/*
 *  ======== close ========
 *  Use EventCombiner module from BIOS 6.x and fill the events array.
 */
function close()
{
    var insts = this.$instances;
    
    /* only availabe C64P like targets */
    if (Program.build.target.isa == "64P" ||
        Program.build.target.isa == "64T" ||
        Program.build.target.isa == "66"  ||
        Program.build.target.isa == "674") {
        if (this.ENABLE) {
            var Ecm = xdc.module('ti.sysbios.family.c64p.EventCombiner');

            for (var i=0; i < 128; i++) {
                if (insts[i].fxn != null) {
                    setArrayConfig(Ecm, "events", i, "fxn", "&" + insts[i].fxn);
                    setArrayConfig(Ecm, "events", i, "arg", insts[i].arg);
                    setArrayConfig(Ecm, "events", i, "unmask", insts[i].unmask);
                }
            }
        }
    }
}

/*
 *  ======== exportMod ========
 */
function exportMod()
{
    if (Program.build.target.isa == "64P" ||
        Program.build.target.isa == "64T" ||
        Program.build.target.isa == "66"  ||
        Program.build.target.isa == "674") {
        if (this.ENABLE) {
            Program.exportModule('ti.sysbios.family.c64p.EventCombiner');
        }
    }
}

/* 
 *  ======== setArrayConfig ========
 */
function setArrayConfig (mod, configParam, index, subfield, value)
{    
    if (bios6xDefaults[configParam][index] && 
        mod[configParam][index][subfield] == 
        bios6xDefaults[configParam][index][subfield]) {
        mod[configParam][index][subfield] = value;
    }
    else {
        if (mod[configParam][index][subfield] != value) {
            var ECM = xdc.module("ti.bios.ECM");
            ECM.$logWarning("Cannot change " + mod.$name + "." + configParam + 
            "[" + index + "]" + "." + subfield +
            ". Parameter changed elsewhere", ECM);
        }
    }
}
