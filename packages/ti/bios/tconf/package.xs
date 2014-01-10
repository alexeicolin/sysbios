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
 *  ======== package.xs ========
 *
 */


function init()
{
    /* Only process during "cfg" phase */
    if (xdc.om.$name != "cfg") {
        return;
    }

    
    /* redefine utils.getProgObjs here */
    utils.getProgObjs = function(prog, name) {
        var biosPkg = xdc.loadPackage('ti.bios');
        var biosMods = biosPkg.$modules;

        for (var i = 0; i < biosMods.length; i++) {
            var mod = "" + biosMods[i];
            name[(mod.substr(mod.lastIndexOf('.')+1))] = biosMods[i];

            var insts = biosMods[i].$instances;
            for (var j = 0; j < insts.length; j++) {
                name[insts[j].name] = insts[j];
            }
        }
        return(null)
    }

    this.$private.realProg = prog;

    prog = {};

    prog.get = function(name) {
        /* In most cases prog.get() looks for a memory instance. That's why we
         * search that module first.
         */
        var MEM = xdc.module('ti.bios.MEM');
        for (var i = 0; i < MEM.$instances.length; i++) {
            if (MEM.$instances[i].name == name) {
                return (MEM.$instances[i]);
            }
        }

        /* look for modules */
        var ret = prog.module(name);
        if (ret != null) {
            return (ret);
        }

        var pkg = xdc.loadPackage('ti.bios');
        for (var i = 0; i < pkg.$modules.length; i++) {
            var mod = pkg.$modules[i];
            for (var j = 0; j < mod.$instances.length; j++) {
                if (mod.$instances[j].name != undefined &&
                    mod.$instances[j].name == name) {
                    return (mod.$instances[j]);    
                }
            }
        }

        /*
        var plat = Program.platform;
    
        if (plat.getExeContext(Program).memoryMap[name] != undefined) {
            return (name);
        }
        */

        Program.$logError("Object " + name + " is undefined.", Program);
        return (null);
    }

    prog.extern = function(name) {
        Program.$logError("The function prog.extern is not supported. "
            + "Replace it with $externPtr or with a string in the argument.",
            Program);
        return (name);
    }

    prog.decl = function(name) {
        Program.$logError("The function prog.decl is not supported. "
            + "Replace it with $externPtr or with a string in the argument.",
            Program);
        return (name);
    }

    prog.module = function(name) {
        var pkg = xdc.om['ti.bios.tconf'];
        
        if (name in pkg) {
            return (pkg[name]);
        }
        
        pkg = xdc.om['ti.bios'];
        if (name in pkg) {
            return (pkg[name]);
        }

        return (null);
    }

    this.$private.biosProg = prog;
    
    /* All modules and all instances must be available through 'bios'. */
    bios = {};
    var biosPkg = xdc.loadPackage('ti.bios');
    for (var j = 0; j < biosPkg.$modules.length; j++) {
        var mod = xdc.useModule(biosPkg.$modules[j].$name);
        bios[mod.$name.substr(mod.$name.lastIndexOf('.')+1)] = mod;
    }
    
    xdc.loadCapsule("ti/bios/tconf/bios.xs");

    for (var i = 0; i < bios.MEM.$instances.length; i++) {
        bios[bios.MEM.$instances[i].name] = bios.MEM.$instances[i];
    }
    for (var i = 0; i < bios.HWI.$instances.length; i++) {
        bios[bios.HWI.$instances[i].name] = bios.HWI.$instances[i];
    }
    for (var i = 0; i < bios.PIE.$instances.length; i++) {
        bios[bios.PIE.$instances[i].name] = bios.PIE.$instances[i];
    }
    for (var i = 0; i < bios.ECM.$instances.length; i++) {
        bios[bios.ECM.$instances[i].name] = bios.ECM.$instances[i];
    }
    for (var i = 0; i < bios.LOG.$instances.length; i++) {
        bios[bios.LOG.$instances[i].name] = bios.LOG.$instances[i];
    }
    for (var i = 0; i < bios.SWI.$instances.length; i++) {
        bios[bios.SWI.$instances[i].name] = bios.SWI.$instances[i];
    }
    
}
