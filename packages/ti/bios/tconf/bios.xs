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
 * ======== bios.xs ========
 * Define bios.xxx functions for supporting bios.xxx functions from BIOS 5.x
 * The final state of the memory map is output as part of the conversion tool
 * therefore all bios functions dealing with modifying the memory map are
 * not supported.
 *
 */

bios.internal = {};

/* Table of code sections for each ISA supported */
bios.internal.codeSectionTable = {

    "2800":["TEXTSEG", "SWITCHSEG", "CINITSEG", "PINITSEG", "HWIVECSEG"
           ],

    "6000":["TEXTSEG", "HWIVECSEG"
           ]
};

/* Table of data sections not requiring a heap for each ISA supported */
bios.internal.dataSectionTable = {

    "2800":["ARGSSEG", "STACKSEG", "BSSSEG", "EBSSSEG", "ECONSTSEG",
            "CONSTSEG", "DATASEG", "CIOSEG", "OBJMEMSEG",
            "bufSeg", "stackMemSeg", "elementSeg"
           ],

    "6000":["ARGSSEG", "STACKSEG", "SWITCHSEG", "BSSSEG", "FARSEG",
            "CINITSEG", "PINITSEG", "CONSTSEG", "DATASEG", "CIOSEG",
            "OBJMEMSEG",
            "bufSeg", "stackMemSeg", "elementSeg"
           ]
};

/* define tables for Primus */
bios.internal.codeSectionTable["Primus"] =
    bios.internal.codeSectionTable["6000"];
bios.internal.dataSectionTable["Primus"] =
    bios.internal.dataSectionTable["6000"];

/* define tables for Tesla */
bios.internal.codeSectionTable["Tesla"] =
    bios.internal.codeSectionTable["6000"];
bios.internal.dataSectionTable["Tesla"] =
    bios.internal.dataSectionTable["6000"];

/*
 *  ======== bios.createMemoryHeap ========
 */
bios.createMemoryHeap = function(prog, memInput, heapSize)
{
    var core = Program.cpu.attrs.cpuCore;
    var isa = null;
    var result = null;
    var heapSizeTable = {       // default heap size for different ISA's.
        "2800": 0x1000,
        "6000": 0x4000,
    };

    if (memInput == null) {
        var memInst = bios.MEM.$instances;
        for (var i=0; i < memInst.length; i++) {
            if ((memInst[i].space == "data" || memInst[i].space == "code/data")
                    && memInst[i].len != 0) {
                if (memInst[i].createHeap == false) {
                    if (memInput == null) { 
                        memInput = memInst[i];
                    }
                    else if (memInst[i].len > memInput.len) {
                        memInput = memInst[i];
                    }
                }
            }
        }
        if (memInput == null) {
            print("MEM segment is not specified, using segment for (.data)");
            memInput = bios.MEM.DATASEG;
        }
    }

    if (heapSize == null) {
        print("heap size is not specified, using default for target");
        for (isa in heapSizeTable) {
            result = core.match(isa);
            if (result != null) {
                break;
            }
        }
        if (result == null) {
            result = core.match(/6\d+/);
            if (result != null) {
                isa = "6000";
            }
        }
        heapSize = heapSizeTable[isa];
    }

    /* create the heap or throw error if heap cannot be created */
    if (heapSize > memInput.len) {
        throw new Error("Cannot create heap with size: " + heapSize +
            "in MEM segment: " + memInput.name);
        return (null);
    }
    else {
        memInput.createHeap = true;
        memInput.heapSize = heapSize;
    }

    return (memInput);
}


/*
 *  ======== bios.disableCsl ========
 *  This function is not supported in BIOS 6.x
 */
bios.disableCsl = function(prog)
{
    print("bios.disableCsl is not supported");
}


/*
 *  ======== bios.disableMemoryHeaps ========
 */
bios.disableMemoryHeaps = function(prog)
{
    bios.MEM.NOMEMORYHEAPS = true;
}


/*
 *  ======== bios.disableRealTimeAnalysis ========
 *  This function is not supported in BIOS 6.x
 */
bios.disableRealTimeAnalysis = function(prog)
{
    print("bios.disableRealTimeAnalysis is not supported");
}


/*
 *  ======== bios.disableRtdx ========
 *  This function is not supported in BIOS 6.x
 */
bios.disableRtdx = function(prog)
{
    print("bios.disableRtdx is not supported");
}


/*
 *  ======== bios.disableTskManager ========
 *  This function disables TSK from being used in the systerm.
 */
bios.disableTskManager = function(prog)
{
    bios.TSK.ENABLETSK = false;
}


/*
 *  ======== bios.enableCsl ========
 *  This function is not supported in BIOS 6.x
 */
bios.enableCsl = function(prog)
{
    print("bios.enableCsl is not supported");
}


/*
 *  ======== bios.enableFullBios ========
 *  This function will only support enabling the TSK Manager.
 */
bios.enableFullBios = function(prog)
{
    bios.enableMemoryHeaps(prog);
    bios.enableTskManager(prog);
}


/*
 *  ======== bios.enableMemoryHeaps ========
 *  This function is not supported in BIOS 6.x
 */
bios.enableMemoryHeaps = function(prog)
{
    bios.MEM.NOMEMORYHEAPS = false;
}


/*
 *  ======== bios.enableRealTimeAnalysis ========
 *  This function is not supported in BIOS 6.x
 */
bios.enableRealTimeAnalysis = function(prog)
{
    print("bios.enableRealTimeAnalysis is not supported");
}


/*
 *  ======== bios.enableRtdx ========
 *  This function is not supported in BIOS 6.x
 */
bios.enableRtdx = function(prog)
{
    print("bios.enableRtdx is not supported");
}


/*
 *  ======== bios.enableTskManager ========
 *  This function will enable the TSK Manager.
 */
bios.enableTskManager = function(prog)
{
    bios.TSK.ENABLETSK = true;
}


/*
 *  ======== bios.initializeMemory ========
 *  This function is not supported in BIOS 6.x
 */
bios.initializeMemory = function(prog, codeSeg, dataSeg)
{
    print("bios.initializeMemory is not supported.");
}


/*
 *  ======== bios.minimizeBios ========
 *  This function will only support disabling the TSK Manager
 */
bios.minimizeBios = function(prog)
{
    bios.disableMemoryHeaps(prog);
    bios.disableTskManager(prog);
}

/*
 *  ======== bios.internal.isMemCodeSection ========
 *  This function determines if a section is a "code" section or not.
 */
bios.internal.isMemCodeSection = function (prog, field)
{
    var retVal = false;
    var core = Program.cpu.attrs.cpuCore;
    var table = bios.internal.codeSectionTable;
    var isa = null;
    var result = null;

    /* Determine the ISA of the program object */
    for (isa in table) {
        result = core.match(isa);
        if (result != null) {
            break;
        }
    }
    if (result == null) {
        result = core.match(/6\d+/);
        if (result != null) {
            isa = "6000";
        }
    }

    /* If field is found in the table then it must be a code section */
    if (result != null) {
        for (var i=0; i < table[isa].length; i++) {
            if (table[isa][i] == field) {
                retVal = true;
                break;
            }
        }
    }
    else {
        throw new Error("Unsupported Architecture: " + core);
    }

    return retVal;
}

/*
 *  ======== bios.internal.isMemDataSection ========
 *  This function determines if a section is a "data" section or not.
 */
bios.internal.isMemDataSection = function (prog, field)
{
    var retVal = false;
    var core = Program.cpu.attrs.cpuCore;
    var table = bios.internal.dataSectionTable;
    var isa = null;
    var result = null;

    /* Determine the ISA of the program object */
    for (isa in table) {
        result = core.match(isa);
        if (result != null) {
            break;
        }
    }
    if (result == null) {
        result = core.match(/6\d+/);
        if (result != null) {
            isa = "6000";
        }
    }

    /* If field is found in the table then it must be a data section */
    if (result != null) {
        for (var i=0; i < table[isa].length; i++) {
            if (table[isa][i] == field) {
                retVal = true;
                break;
            }
        }
    }
    else {
        throw new Error("Unsupported Architecture: " + core);
    }

    return retVal;
}

/*
 *  ======== bios.internal.setCodeSections ========
 *  Set all code sections to memSegment.
 */
bios.internal.setCodeSections = function(pkg, memSegment)
{
    for (var i = 0; i < pkg.$modules.length; i++) {
        var mod = pkg.$modules[i];
        for (var j in mod) {
            if (bios.internal.isMemCodeSection(prog, j)) {
                mod[j] = memSegment;
            }
        }
    }
}

/*
 *  ======== bios.setMemCodeSections ========
 *  Set all code sections to memSegment.
 */
bios.setMemCodeSections = function (prog, memSegment)
{
    if (memSegment != null) {
        /* Set any memory reference in "code" space to memSegment */
        var pkg = xdc.loadPackage('ti.bios');
        bios.internal.setCodeSections(pkg, memSegment);
    }
    else {
        throw new Error("MEM segment does not exists.");
    }
}

/*
 *  ======== bios.setMemDataHeapSections ========
 *  Set all "data" sections that require a heap to memSegment.
 *  Make sure memSegment has a heap created in it.
 */
bios.setMemDataHeapSections = function (prog, memSegment)
{
    if (memSegment != null) {
        /* set memory references which require a heap to memSegment */
        bios.MEM.BIOSOBJSEG = memSegment;
        bios.MEM.MALLOCSEG = memSegment;
        bios.TSK.STACKSEG = memSegment;
    }
    else {
        throw new Error("MEM segment does not exists.");
    }
}

/*
 *  ======== bios.internal.setDataSections ========
 *  Set all data sections to the specified memSegment
 */
bios.internal.setDataSections = function(pkg, memSegment)
{
    for (var i = 0; i < pkg.$modules.length; i++) {
        var mod = pkg.$modules[i];
        for (var j in mod) {
            if (bios.internal.isMemDataSection(prog, j)) {
                if (!(mod == "ti.bios.TSK" && j == "STACKSEG")) {
                    mod[j] = memSegment;
                }
            }
        }
        var inst = mod.$instances;
        for (var j=0; j < inst.length; j++) {
            for (var k in inst[j]) {
                if (bios.internal.isMemDataSection(prog, k)) {
                    if (!(mod == "ti.bios.HST" ||
                        mod == "ti.bios.PIP")) {
                        inst[j][k] = memSegment;
                    }
                }
            }
        }
    }
}

/*
 *  ======== bios.setMemDataNoHeapSections ========
 *  Set all "data" sections that don't require a heap to memSegment
 *  The data sections are specified in a table at the top of this file.
 */
bios.setMemDataNoHeapSections = function (prog, memSegment)
{
    if (memSegment != null) {
        /*
         * set memory references in "data" space which do not require a
         * heap to be set to memSegment.
         */
        var pkg = xdc.loadPackage('ti.bios');
        bios.internal.setDataSections(pkg, memSegment);
    }
    else {
        throw new Error("MEM segment does not exist.");
    }
}

