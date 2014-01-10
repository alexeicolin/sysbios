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
 *  ======== Exception.xs ========
 *
 */

var Exception = null;
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

    Exception = this;

    /* by default, exception context is saved on local ISR stack */
    Exception.excContextBuffer = 0;

    /* by default, the exception thread stack is NOT copied */
    Exception.excStackBuffer = null;
}

/*
 *  ======== module$use ========
 */
function module$use()
{
    var BIOS = xdc.useModule("ti.sysbios.BIOS");
    Memory = xdc.useModule("xdc.runtime.Memory");
}

/*
 *  ======== module$static$init ========
 */
function module$static$init(mod, params)
{
    /* setup common Exception stack */
    mod.excActive = false;
    mod.excContext = $addr(Exception.excContextBuffer);
    mod.excStackBuffer = Exception.excStackBuffer;
    mod.excStackSize = params.excStackSize;
    mod.excStack.length = params.excStackSize;
    Memory.staticPlace(mod.excStack, 4, params.excStackSection);
}

function viewGetType(mode)
{
    switch(mode & 0x1f) {

        case 0x13:
            return("Supervisor");

        case 0x17:
            return("Prefetch Abort");

        case 0x18:
            return("Data Abort");

        case 0x1b:
            return("Undefined Instruction");

    }
    return("Unknown");
}

/*
 *  ======== viewCallStack ========
 */
function viewCallStack(excContext) {
    try {
        var CallStack = xdc.useModule('xdc.rov.CallStack');
    }
    catch (e) {
        return (null);
    }

    CallStack.setRegister("R13", Number(excContext["sp"]));
    CallStack.setRegister("R14", Number(excContext["lr"]));
    CallStack.setRegister("PC", Number(excContext["pc"]));

    CallStack.setRegister("R0", Number(excContext["r0"]));
    CallStack.setRegister("R1", Number(excContext["r1"]));
    CallStack.setRegister("R2", Number(excContext["r2"]));
    CallStack.setRegister("R3", Number(excContext["r3"]));
    CallStack.setRegister("R4", Number(excContext["r4"]));
    CallStack.setRegister("R5", Number(excContext["r5"]));
    CallStack.setRegister("R6", Number(excContext["r6"]));
    CallStack.setRegister("R7", Number(excContext["r7"]));
    CallStack.setRegister("R8", Number(excContext["r8"]));
    CallStack.setRegister("R9", Number(excContext["r9"]));
    CallStack.setRegister("R10", Number(excContext["r10"]));
    CallStack.setRegister("R11", Number(excContext["r11"]));
    CallStack.setRegister("R12", Number(excContext["r12"]));

    /* fetch back trace string */
    var frames = CallStack.toText();

    /* break up into separate lines */
    frames = frames.split("\n");

    /*
     * Strip off "Unwind halted ... " from last frame
     */
    frames.length -= 1;

    for (var i = 0; i < frames.length; i++) {
        var line = frames[i];
        /* separate frame # from frame text a little */
        line = line.replace(" ", "    ");
        var file = line.substr(line.indexOf(" at ") + 4);
        file = file.replace(/\\/g, "/");
        file = file.substr(file.lastIndexOf("/")+1);
        if (file != "") {
            frames[i] = line.substring(0,
                                   line.indexOf(" at ") + 4);
            /* tack on file info */
            frames[i] += file;
        }
    }

    /*
     * Invert the frames[] array so that the strings become the index of a
     * new associative array.
     *
     * This is done because the TREE view renders the array index (field)
     * on the left and the array value on the right.
     *
     * At the same time, extract the "PC = ..." substrings and make them
     * be the value of the array who's index is the beginning of the
     * frame text.
     */
    var invframes = new Array();

    for (var i = 0; i < frames.length; i++) {
        invframes[frames[i].substring(0,frames[i].indexOf("PC")-1)] =
            frames[i].substr(frames[i].indexOf("PC"));
    }

    return (invframes);
}

/*
 *  ======== viewInitBasic ========
 */
function viewInitBasic()
{
    var Program = xdc.useModule('xdc.rov.Program');
    var Exception = xdc.useModule('ti.sysbios.family.arm.exc.Exception');

    try {
        var excRawView = Program.scanRawView('ti.sysbios.family.arm.exc.Exception');
    }
    catch (e) {
        return null;
    }

    if (excRawView.modState.excActive == 0) {
        return("No Exception Context");
    }

    try {
        var excContext = Program.fetchStruct(Exception.ExcContext$fetchDesc,
                             excRawView.modState.excContext);
    }
    catch (e) {
        return null;
    }

    var excDecode = {};

    excDecode.Decoded = viewGetType(excContext.type);

    var excCallStack = viewCallStack(excContext);

    var obj = {};

    obj["Decoded exception"] = excDecode;

    obj["Exception context"] = excContext;

    obj["Exception call stack"] = excCallStack;

    return (obj);
}

