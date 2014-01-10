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

/* TODO .sysmem and .printf */
RunSectionNameMap = {
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
};

LoadSectionNameMap = {
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
    "LOADRTDXTEXTSEG", "ENABLELOADADDR"];

var localSectMap = {};

/* data structure to hold set of valid values for "space" attribute */
var segmentSpaceTypes = {"code" : {}, "data" : {}, "code/data" : {}, "io" : {}, };

/*
 *  ======== run ========
 *  Main function, executed from the command line. Parses the arguments
 *  using getopt() and calls the above. If any function raises an
 *  exception, prints the error message and the usage.
 */
function run(cmdr, args)
{

    /* print the usage message if requested */
    if (args.length < 1) {
        cmdr.usage();
        return 1;
    }

    /* run the command and turn exceptions into one-liners */
    var status;
    try {
        status = this.convert(args[0]);
    }
    catch (e) {
        cmdr.error(e);
        status = 1;
    }

    /*
     * If a non-zero exit status, force exit with return code.
     * This functionality should be moved into ICmd.
     */
    if (status) {
        java.lang.System.exit(status);
    }
}

/*
 *  ======== convert ========
 */
function convert(infile)
{
    for (var i = 0; i < this.defines.length; i++) {
        var pair = this.defines[i].split('=', 2);
        var name = pair[0];
        print("name " + name);
        if (pair.length > 1) {
            value = pair[1];
        }
        else {
            value = true;
        }
        if (name == "") {
            print("Warning: empty -D option");
        }
        else {
            environment[name] = value;
            print(name + "   " + environment[name]);
        }
    }

    if (environment['config.importPath'] != undefined) {
        print("Warning: ti.bios.conversion ignores config.importPath.");
        print("Please use the xs option --xp to add directories to the " +
            "search path.");
    }
    environment['config.importPath'] = environment['xdc.path'] + ";" +
        xdc.csd() + "/bios5";

    var $$conversion = {};
    $$conversion.pname = this.platformName;
    $$conversion.prepo = this.platformRepository;
    utils.oldLoadPlat = utils.loadPlatform;
    utils.loadPlatform = function (name, params) {
        if ($$conversion.platform == undefined) {
            $$conversion.platform = name;
        }
        $$conversion.params = params;

        try {
            utils.oldLoadPlat(name, params);
        }
        catch (e) {
            print(e);
        }

        var targType = prog.cpu.attrs.cpuCore.substring(0,2);
        if (prog.cpu.attrs.cpuCore.substring(2,4) == "x+") {
            targType = targType + "P";
        }

        // ensure that target user is working on is supported:
        checkIfTargetSupported(targType);

        prog.gen = function () {
            $$conversion.target = "ti.targets.C" + targType;

            if (targType == "28") {
                get28(bios, $$conversion, targType);
            }

            if (targType.charAt(0) == "6") {
                get6x(bios, $$conversion, targType);
            }

            $$conversion.clockRate = bios.GBL.CPUCLOCK;

            /*-----------------------------------------------------------------*/
            print("Platform: " + $$conversion.platform);
            for (var p in $$conversion.params) {
                // skip mem and regs - they are objects
                if (p == "mem" || p == "regs") {
                    continue;
                }
                print("\tparams." + p + ":" + $$conversion.params[p]);
            }
            print("Target: " + $$conversion.target);
            print("Clock Rate: " + $$conversion.clockRate);

            var mem = bios.MEM.instances();
            var j = 0;

            /*
             * create a temporary File to output memory information to. This
             * file will contain the memory segment information in XML format,
             * and will then be sent as an argument to the platform wizard.
             *
             * This file will be stored in the default temporary-file directory,
             * and will have the prefix "ti_bios_conversion" and extension
             * ".tmp"
             *
             */
            try {
                var tempXmlFile =
                        java.io.File.createTempFile("ti_bios_conversion", null);
            }
            catch (e) {
                print("Error: " + e);
            }

            /*
             * start platform wizard file generation code
             *
             * The platform wizard takes as input an xml file which specifies
             * a memory map, amongst other settings.  Use a Java PrintWriter
             * to generate this xml code from the legacy tcf file's memory map.
             */
            try {
                var xmlFileWriter = java.io.PrintWriter(tempXmlFile);
            }
            catch (e) {
                print("Error: " + e);
            }

            xmlFileWriter.println("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
            xmlFileWriter.println("<platform>");

            for (var i = 0; i < mem.length; i++) {
                if (mem[i].name != "MEM_NULL" && mem[i].len > 0) {

                    xmlFileWriter.println("    <customMemoryMap>");
                    xmlFileWriter.println("        <name>" + mem[i].name + "</name>");

                    if (mem[i].base < 0) {
                        var real = 0x80000000 * 2 + mem[i].base;

                        xmlFileWriter.println("        <base>0x" + real.toString(16) + "</base>");
                    }
                    else {
                        xmlFileWriter.println("        <base>0x" + mem[i].base.toString(16) + "</base>");
                    }

                    xmlFileWriter.println("        <len>0x" + mem[i].len.toString(16) + "</len>");

                    // validate that this segment's space is legal. Only generate space xml code if 'space' value is legal
                    if (segmentSpaceTypes[mem[i].space] != undefined) {

                        xmlFileWriter.println("        <space>" + mem[i].space + "</space>");
                    }

                    xmlFileWriter.println("    </customMemoryMap>");

                    j++;
                }
            }

            var catPackage = xdc.loadPackage($$conversion.params.catalogName);
            var deviceName = $$conversion.params.deviceName;
            var deviceName1 = "TMS320C" + $$conversion.params.deviceName;
            var deviceName2 = "TMS320" + $$conversion.params.deviceName;
            var deviceName3 = "OMAP" + $$conversion.params.deviceName;
            if (deviceName1 in catPackage) {
                deviceName = deviceName1;
            }
            else if (deviceName2 in catPackage) {
                deviceName = deviceName2;
            }
            else if (deviceName3 in catPackage) {
                deviceName = deviceName3;
            }

            xmlFileWriter.println("    <cpuCore>");
            xmlFileWriter.println("        <name>CPU</name>");
            xmlFileWriter.println("        <clockRate>" + $$conversion.clockRate + "</clockRate>");
            xmlFileWriter.println("        <catalogName>" + $$conversion.params.catalogName + "</catalogName>");
            xmlFileWriter.println("        <deviceName>" + deviceName + "</deviceName>");
            xmlFileWriter.println("        <codeMemory>" + bios.MEM.TEXTSEG.name + "</codeMemory>");
            xmlFileWriter.println("        <dataMemory>" + bios.MEM.DATASEG.name + "</dataMemory>");
            xmlFileWriter.println("        <stackMemory>" + bios.MEM.STACKSEG.name + "</stackMemory>");

            if ($$conversion.l1PMode != undefined) {
                xmlFileWriter.println("        <register>");
                xmlFileWriter.println("            <name>l1PMode</name>");
                xmlFileWriter.println("            <value>" + $$conversion.l1PMode + "</value>");
                xmlFileWriter.println("        </register>");
            }
            if ($$conversion.l1DMode != undefined) {
                xmlFileWriter.println("        <register>");
                xmlFileWriter.println("            <name>l1DMode</name>");
                xmlFileWriter.println("            <value>" + $$conversion.l1DMode + "</value>");
                xmlFileWriter.println("        </register>");
            }
            if ($$conversion.l2Mode != undefined) {
                xmlFileWriter.println("        <register>");
                xmlFileWriter.println("            <name>l2Mode</name>");
                xmlFileWriter.println("            <value>" + $$conversion.l2Mode + "</value>");
                xmlFileWriter.println("        </register>");
            }

            xmlFileWriter.println("    </cpuCore>");
            xmlFileWriter.println("</platform>");

            // close and clean up the temp file
            xmlFileWriter.close();
            try {
                tempXmlFile.deleteOnExit(); // delete file when program exits
            }
            catch (e) {
                print("Error: " + e);
            }

            /*generate the platform package */
            var Server = Packages.xdc.tools.Server(
            "xdc.tools.platformWizard",
            ["-r", $$conversion.prepo,
             "-x", tempXmlFile.getAbsoluteFile(),
             $$conversion.pname]);

            Server.start();

        };
    }
    utils.load(infile);

    /* Now, read the file line by line and create a new file with the extension
     * 'cfg'.
     */
    var File = xdc.module('xdc.services.io.File');
    var oldFile = File.open(infile, "r");
    if (this.outputFile == null) {
        this.outputFile = infile.replace(/\.tcf/, ".cfg");
    }
    var outCanPath = File.getCanonicalPath(this.outputFile);
    var inCanPath = File.getCanonicalPath(infile);
    if (inCanPath == outCanPath) {
        print("\nERROR: "
            + "*** The input and the output file are the same ***");        
        java.lang.System.exit(1);
    }

    var newFile = File.open(this.outputFile, "w");
    _convert(oldFile, newFile, this.includeImports);
    _outputSectMap(newFile);
    newFile.writeLine("prog = xdc.module('xdc.cfg.Program');");
    newFile.close();
    oldFile.close();

    /* BIOS 5 configurations used to generate "appcfg.h" file with externs. Externs are now in "xdc/cfg/global.h", so
     * we generate "appcfg.h" that includes "xdc/cfg/global.h".
     */
    var inBase = inCanPath.match(/\/([^\/]+)\.\w*/);
    if (inBase == null) {
        /* If there is no '.' after the base name, assume there is
         * no extension.
         */
        inBase = inCanPath.match(/\/([^\/]+)$/);
    }

    var outDir = outCanPath.match(/(.*\/)[^\/]+/);
    if (inBase.length != 2 || outDir.length != 2) {
        print("\nERROR: "
            + "*** Cannot determine the name for the header file ***");
    }
    else {
        var cfgFile = File.open(outDir[1] + inBase[1] + "cfg.h", "w");
        cfgFile.writeLine("#include <xdc/cfg/global.h>");
        cfgFile.close();
    }


    /* return the XDC command's exit status */
    return (0);
}


/*---------------- private functions ----------------- */
/* From this point on, 'this' refers to the file scope, not module Main */

function _convert(oldFile, newFile, copyImp)
{
    var line;
    while ((line = oldFile.readLine()) != null) {
        /* The lines with internal comments and prog.gen() lines are not
         * copied to the output file.
         */
        if (line.match(/^\s*\*\!.*/) || line.match(/^\s*prog.gen/)) {
        }
        else if (line.match(/^\s*utils.loadPlatform.*/)) {
            newFile.writeLine("xdc.loadPackage('ti.bios.tconf');");
        }
        else if (line.match(/^\s*utils.importFile/)) {
            if (!copyImp) {
                newFile.writeLine(line);
            }
            else {
                var File = xdc.module('xdc.services.io.File');
                var iFile = line.match(/^\s*utils.importFile\((.*)\)/)[1];
                var tmpFileName = iFile.substring(1, iFile.length - 1);
                iFile = utils.findFile(tmpFileName);
                if (iFile == null) {
                    print("\nERROR: *** File " + tmpFileName + " cannot be"
                        + " found on the package path!\nPlease check"
                        + " the path supplied in --xp option. ***");
                    java.lang.System.exit(1);                    
                }
                newFile.writeLine("/*** Copying " + iFile + " ***/");
                var impFile = File.open(iFile, "r");
                _convert(impFile, newFile, copyImp);
                newFile.writeLine("/*** Closing " + iFile + " ***/");
                impFile.close();
            }
        }
        else if (line.match(/\WPIP\W/)) {
            newFile.writeLine("// WARNING: PIP module is not supported in BIOS 6.");
            newFile.writeLine(line);
        }
        else if (line.match(/\WHST\W/)) {
            newFile.writeLine("// WARNING: HST module is not supported in BIOS 6.");
            newFile.writeLine(line);
        }
        else if (line.match(/\WRTDX\W/)) {
            newFile.writeLine("// WARNING: RTDX module is not supported in BIOS 6.");
            newFile.writeLine(line);
        }
        else if (line.match(/\WUSERCOMMANDFILE\W/)) {
            newFile.writeLine("// WARNING: MEM.USERCOMMANDFILE property is not supported in BIOS 6.");
            newFile.writeLine(line);
        }
        else if (line.match(/\Worder\W/)) {
            newFile.writeLine("// WARNING: 'order' property is not supported in BIOS 6.")
            newFile.writeLine(line);
        }
        /* The next 6 'ifs' are fixing CQ22447 and CQ27574 */
        else if (line.match(/arg\w*\s*=\s*prog.extern/i)) {
            newFile.writeLine(line.replace("prog.extern", "\$externPtr"));
        }
        else if (line.match(/\["arg\w*"\]\s*=\s*prog.extern/i)) {
            newFile.writeLine(line.replace("prog.extern", "\$externPtr"));
        }
        else if (line.match(/arg\w*\s*=\s*prog.decl/i)) {
            newFile.writeLine(line.replace("prog.decl", "\$externPtr"));
        }
        else if (line.match(/\["arg\w*"\]\s*=\s*prog.decl/i)) {
            newFile.writeLine(line.replace("prog.decl", "\$externPtr"));
        }
        else if (line.match(/prog\.extern/)) {
            newFile.writeLine(line.replace(/prog\.extern\((.*)\)/, "$1"));
        }
        else if (line.match(/prog\.decl/)) {
            newFile.writeLine(line.replace(/prog\.decl\((.*)\)/, "$1"));
        }
        else {
            var found = false;
            for (var i = 0; i < unsupported.length; i++) {
                if (line.match(unsupported[i])) {
                    found = true;
                    break;
                }
            }
            if (found == true) {
                continue;
            }
            for (var k in LoadSectionNameMap) {
                if (line.match(k)) {
                    var segment = line.split("\"")[1];
                    var section = LoadSectionNameMap[k].name;
                    if (section in localSectMap) {
                        localSectMap[section].loadSegment = segment;
                    }
                    else {
                        localSectMap[section] = new Object();
                        localSectMap[section].loadSegment = segment;
                    }
                    found = true;
                    break;
                }
            }
            if (found == true) {
                continue;
            }
            for (var k in RunSectionNameMap) {
                if (line.match(k)) {
                    var segment = line.split("\"")[1];
                    var section = RunSectionNameMap[k].name;
                    if (section in localSectMap) {
                        localSectMap[section].runSegment = segment;
                    }
                    else {
                        localSectMap[section] = new Object();
                        localSectMap[section].runSegment = segment;
                    }
                    found = true;
                    break;
                }
            }
            if (found == true) {
                continue;
            }

            newFile.writeLine(line);
        }
    }
}

function get6x(bios, conv, target) {
    if (bios.GBL.ENDIANMODE == "big") {
        conv.target = conv.target + "_big_endian";
    }

    if (target == "64P" && bios.GBL.C64PLUSCONFIGURE == true) {
        if (bios.GBL.C64PLUSL1PCFG != "32k") {
            conv.l1PMode = bios.GBL.C64PLUSL1PCFG;
        }
        if (bios.GBL.C64PLUSL1DCFG != "32k") {
            conv.l1DMode = bios.GBL.C64PLUSL1DCFG;
        }
        if (bios.GBL.C64PLUSL2CFG != "0k") {
            conv.l2Mode = bios.GBL.C64PLUSL2CFG;
        }
    }
    if (target == "64" && bios.GBL.C641XCONFIGUREL2 == true) {
        if (bios.GBL.C641XCCFGL2MODE != "4-way cache (0k)") {
            conv.l2Mode = bios.GBL.C641XCCFGL2MODE;
        }
    }
    if (target == "62" && bios.GBL.C621XCONFIGUREL2 == true) {
        if (bios.GBL.C621XCCFGL2MODE != "SRAM") {
            conv.l2Mode = bios.GBL.C621XCCFGL2MODE;
        }
    }
}

function get28(bios, conv, target) {
    if (bios.GBL.MEMORYMODEL == "LARGE") {
        conv.target = conv.target + "_large";
    }
}

/*
 *  ======== checkIfTargetSupported ========
 *  This function is meant to catch users who are attempting to migrate to BIOS6
 *  using an unsupported target (e.g. C64, C62, C55, etc.).
 */
function checkIfTargetSupported(targType) {

    //ideally throw an error
    if (targType == "64" || targType == "67" || targType == "62" || targType == "55" || targType == "54") {

        print("\nERROR: *** Target No Longer Supported! ***\n    The C" + targType + " target is no" +
          " longer supported.  For a full list of supported devices " +
          "and targets, please refer to the SYS/BIOS Release Notes" +
          " documentation, which may be found in your SYS/BIOS" +
          " installation directory.");
        java.lang.System.exit(1);
    }
}

function _outputSectMap(newFile)
{
    for (var k in localSectMap) {
        newFile.writeLine('Program.sectMap["' + k + '"] = new Program.SectionSpec()');
        if (localSectMap[k].runSegment) {
            if (localSectMap[k].loadSegment) {
                newFile.writeLine('Program.sectMap["' + k  + '"].runSegment = "' + localSectMap[k].runSegment + '";');
            }
            else {
                newFile.writeLine('Program.sectMap["' + k  + '"] = "' + localSectMap[k].runSegment + '";');
            }
        }
        if (localSectMap[k].loadSegment) {
            if (localSectMap[k].runSegment) {
                newFile.writeLine('Program.sectMap["' + k + '"].loadSegment = "' + localSectMap[k].loadSegment + '";');
            }
            else {
                newFile.writeLine('Program.sectMap["' + k + '"] = "' + localSectMap[k].loadSegment + '";');
            }
        }
    }
}
