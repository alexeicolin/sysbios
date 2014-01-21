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
 *  ======== Package.getLibs ========
 *  This function is called when a program's configuration files are
 *  being generated and it returns the name of a library appropriate
 *  for the program's configuration.
 */

function getLibs(prog)
{
    var name = this.$name + ".a" + prog.build.target.suffix;
    var lib = "";
    var libdir = "";

    var BIOS = xdc.module("ti.sysbios.BIOS");
    var Build = xdc.module("ti.sysbios.Build");

    if (BIOS.smpEnabled == true) {
        libdir = "lib/smpbios/";
    }
    else {
        libdir = "lib/sysbios/";
    }

    switch (BIOS.libType) {
        case BIOS.LibType_Instrumented:
        case BIOS.LibType_NonInstrumented:
        case BIOS.LibType_Custom:
        case BIOS.LibType_Debug:
            if (BIOS.$used == false) return (null);
            if (Build.buildROM == true) return (null);
            if (Build.buildROMApp == true) return (null);
            lib = Build.$private.outputDir + Build.$private.libraryName;
            return ("!" + String(java.io.File(lib).getCanonicalPath()));
            break;

        default:
            lib = libdir + "debug/" + name;
            if (java.io.File(this.packageBase + lib).exists()) {
                return lib;
            }
            break;
    }

    /* could not find any library, throw exception */
    throw Error("Library not found: " + name);
}

/*
 *  ======== package.close ========
 */
function close()
{
    if (xdc.om.$name != 'cfg') {
        return;
    }

    /*
     * Force the Build module to get used if any module
     * in this package is used
     */
    for (var mod in this.$modules) {
        if (this.$modules[mod].$used == true) {
            xdc.useModule('ti.sysbios.Build');
            break;
        }
    }

    /* IAR specific fix to get rid of meta strings till noload is supported */
    var Text = xdc.module('xdc.runtime.Text');
    if (Program.build.target.$name.match(/iar\.targets/)
        && (Text.isLoaded == false)) {
        var Defaults = xdc.module('xdc.runtime.Defaults');
        var Main = xdc.module('xdc.runtime.Main');
        var Memory = xdc.module('xdc.runtime.Memory');

        Defaults.common$.namedModule = false;
        Defaults.common$.namedInstance = false;
        Main.common$.namedModule = false;
        Memory.common$.namedModule = false;

        var emsg = [];
        var amsg = [];
        var lmsg = [];
        /* Loop through all modules */
        for (var i = 0; i < xdc.om.$modules.length; i++) {
             var mod = xdc.om.$modules[i];

            /* check all Error_Desc's declared in any module */
            if ('$$errorDescCfgs' in mod && mod.$$errorDescCfgs.length > 0) {
                for each (var cn in mod.$$errorDescCfgs) {
                    var desc = mod[cn];

                    /* Strip and create a new string */
                    var mnew = "E" + emsg.length;
                    var fmtStrings = desc.msg.match(/%[$\w\d]+/g);
                    for each (var fmt in fmtStrings) {
                        mnew += " " + fmt;
                    }

                    /* Is the new string is not bigger than the orig? */
                    if (mnew.length < desc.msg.length) {
                        var msg = {
                            new: mnew,
                            orig: desc.msg,
                            mod: mod
                        }
                        /* Save for later */
                        emsg[emsg.length++] = msg;

                        /* replace the string */
                        desc.msg = msg.new;

                    }
                }
            }

            /* check all Assert_Desc's declared in any module */
            if ('$$assertDescCfgs' in mod && mod.$$assertDescCfgs.length > 0) {
                for each (var cn in mod.$$assertDescCfgs) {
                    var desc = mod[cn];

                    /* Strip and create a new string */
                    var mnew = "A" + amsg.length;
                    var fmtStrings = desc.msg.match(/%[$\w\d]+/g);
                    for each (var fmt in fmtStrings) {
                        mnew += " " + fmt;
                    }

                    /* Is the new string is not bigger than the orig? */
                    if (mnew.length < desc.msg.length) {
                        var msg = {
                            new: mnew,
                            orig: desc.msg,
                            mod: mod
                        }
                        /* Save for later */
                        amsg[amsg.length++] = msg;

                        /* replace the string */
                        desc.msg = msg.new;
                    }
                }
            }

            /* create an Id for all Log_Event's declared in the module */
            if ('$$logEvtCfgs' in mod && mod.$$logEvtCfgs.length > 0) {
                for each (var cn in mod.$$logEvtCfgs) {
                    var desc = mod[cn];

                    /* Strip and create a new string */
                    var mnew = "L" + lmsg.length;
                    var fmtStrings = desc.msg.match(/%[$\w\d]+/g);
                    for each (var fmt in fmtStrings) {
                        mnew += " " + fmt;
                    }

                    /* Is the new string is not bigger than the orig? */
                    if (mnew.length < desc.msg.length) {
                        var msg = {
                            new: mnew,
                            orig: desc.msg,
                            mod: mod
                        }
                        /* Save for later */
                        lmsg[lmsg.length++] = msg;

                        /* replace the string */
                        desc.msg = msg.new;
                    }
                }
            }

        }
        /* Pass info to template */
        var thisObj = {
            emsg:  emsg,
            amsg:  amsg,
            lmsg:  lmsg,
        };

        /* Generate an html page */
        var Build = xdc.module("ti.sysbios.Build");
        var output = Build.$private.outputDir;
        var tplt = xdc.loadTemplate(this.packageBase + "/DescMsg.xdt");
        tplt.genFile(output + "/DescriptorMessage.html", thisObj, []);
    }
}

