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
 *  ======== Mmu.xs ========
 */

var Mmu = null;
var Memory = null;
var device = null;
var SECTION_DESC_SHIFT = 20;
var Cache = null;

/*
 *  This table is needed to map the physical base addresses
 *  to a virtual address so that no MMU fault is generated when
 *  the addresses are accessed. Only the 12 MSBs are important
 *  here for doing a 1-to-1 mapping between virtual and physical.
 */
if (xdc.om.$name == "cfg") {
    var deviceTable = {
        "OMAP4430": {
            intcBaseAddr:   0x48240000,
            timer1BaseAddr: 0x4A318000,
            timer2BaseAddr: 0x48032000,
            timer3BaseAddr: 0x48034000,
            timer4BaseAddr: 0x48036000,
            timer10BaseAddr: 0x48086000,
            cmClkCtrlBaseAddr: 0x4A000000,
        },
    }
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

    print("The " + Program.cpu.deviceName + " device is not currently supported.");
    print("The following devices are supported for the " + Program.build.target.name + " target:");

    for (var i=0; i<sd.length; i++) {
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

    Mmu = this;

    /* set fxntab default */
    Mmu.common$.fxntab = false;
    
    /* set the tableBuf size to 16KB or 4K words */
    Mmu.tableBuf.length = 0x1000;
}

/*
 *  ======== module$use ========
 */
function module$use()
{
    Memory = xdc.useModule('xdc.runtime.Memory');
    Cache  = xdc.useModule('ti.sysbios.family.arm.a9.Cache');

    device = deviceSupportCheck();
}

/*
 *  ======== module$static$init ========
 */
function module$static$init(mod, params)
{
    /* The MMU first level table descriptor needs to be 16KB or 4K words */
    mod.tableBuf.length = Mmu.tableBuf.length;
    
    /* The section name of the MMU table (documented in Mmu.xdc) */
    var tableSectionName = "ti.sysbios.family.arm.a9.mmuTableSection";

    /*
     *  Place the table buffer into the specified section name.
     *  The table buffer must be aligned on a 16KB boundary.
     */
    Memory.staticPlace(mod.tableBuf, 0x4000, tableSectionName);
 
    /* Place tableSection into default dataMemory section if not specified. */
    if (Program.sectMap[tableSectionName] === undefined) {
        Program.sectMap[tableSectionName] = new Program.SectionSpec();
        Program.sectMap[tableSectionName].type = "NOLOAD";
        Program.sectMap[tableSectionName].loadSegment = 
            Program.platform.dataMemory;
    }

    /* Make a 1-to-1 mapping of virtual to physical address for memory map */
    var map = Program.cpu.memoryMap;
    for (var i=0; i < map.length; i++) {
        /* Get base address and length and unsigned right shift to get index */
        var start = map[i].base >>> SECTION_DESC_SHIFT;
        var end = (map[i].base + map[i].len - 1) >>> SECTION_DESC_SHIFT;
        var index = start;
        var attrs =
            {
                type       : Mmu.FirstLevelDesc_SECTION,  // SECTION descriptor
                bufferable : true,                        // bufferable
                cacheable  : true,                        // cacheable
            };

        do {
            /* only set if table descriptor has not been set */
            if (Mmu.tableBuf[index] == undefined) {
                if (Mmu.cachePlatformMemory == true) {
                    setFirstLevelDescMeta(index << SECTION_DESC_SHIFT,
                                      index << SECTION_DESC_SHIFT,
                                      attrs);
                }
                else {
                    setFirstLevelDescMeta(index << SECTION_DESC_SHIFT,
                                      index << SECTION_DESC_SHIFT,
                                      Mmu.defaultAttrs);
                }
            }
            index++;
        } while (index <= end);
    }

    /* Force peripheral sections to be NON cacheable */    
    var peripheralAttrs =
            {
                type       : Mmu.FirstLevelDesc_SECTION,  // SECTION descriptor
                bufferable : false,                       // bufferable
                cacheable  : false,                       // cacheable
                shareable  : false,                       // shareable
                noexecute  : true,                        // not executable
            };

    /* 1-to-1 mapping of virtual to physical address from the table above */
    for each (var addr in deviceTable[device]) {
        setFirstLevelDescMeta(addr, addr, peripheralAttrs);
    }
}

/*
 *  ======== setFirstLevelDescMeta ========
 */
function setFirstLevelDescMeta(virtualAddr, phyAddr, attrs)
{
    var index = virtualAddr >>> SECTION_DESC_SHIFT;
    var desc = 0;
    
    /* Set type field to default if undefined */
    if (attrs.type === undefined) {
        attrs.type = Mmu.defaultAttrs.type;
    }

    /* Set tex field to default if undefined */
    if (attrs.tex === undefined) {
        attrs.tex = Mmu.defaultAttrs.tex;
    }

    /* Set bufferable field to default if undefined */
    if (attrs.bufferable === undefined) {
        attrs.bufferable = Mmu.defaultAttrs.bufferable;
    }

    /* Set cacheable field to default if undefined */
    if (attrs.cacheable === undefined) {
        attrs.cacheable = Mmu.defaultAttrs.cacheable;
    }

    /* Set shareable field to default if undefined */
    if (attrs.shareable === undefined) {
        attrs.shareable = Mmu.defaultAttrs.shareable;
    }

    /* Set noexecute field to default if undefined */
    if (attrs.noexecute === undefined) {
        attrs.noexecute = Mmu.defaultAttrs.noexecute;
    }

    /* Set imp field to default if undefined */
    if (attrs.imp === undefined) {
        attrs.imp = Mmu.defaultAttrs.imp;
    }

    /* Set domain field to default if undefined */
    if (attrs.domain === undefined) {
        attrs.domain = Mmu.defaultAttrs.domain;
    }

    /* Set accPerm field to default if undefined */
    if (attrs.accPerm === undefined) {
        attrs.accPerm = Mmu.defaultAttrs.accPerm;
    }
    
    switch (attrs.type) {
        /* Page table descriptor */
        case Mmu.FirstLevelDesc_PAGE_TABLE:
            desc = (attrs.type & 0x3) |
                   ((attrs.domain & 0xF) << 5) |
                   ((attrs.imp & 0x1) << 9) |
                   (phyAddr & 0xFFFFFC00);
            break;
        
        /* Section descriptor */
        case Mmu.FirstLevelDesc_SECTION:
            desc = (attrs.type & 0x3) |
                   ((attrs.bufferable & 0x1) << 2) |
                   ((attrs.cacheable & 0x1) << 3) |
                   ((attrs.noexecute & 0x1) << 4) |
                   ((attrs.domain & 0xF) << 5) |
                   ((attrs.imp & 0x1) << 9) |
                   ((attrs.accPerm & 0x3) << 10) |
                   ((attrs.tex & 0x7) << 12) |
                   ((attrs.accPerm & 0x4) << 13) |
                   ((attrs.shareable & 0x1) << 16) |
                   (phyAddr & 0xFFF00000);
            break;
        
        default:
            Mmu.$logError("Mmu.setFirstLevelDescMeta: the type " +
                "(" + attrs.type + ")" + " is not recognized.", Mmu);
            break;
    }

    Mmu.tableBuf[index] = convertToUInt32(desc);
}

/*
 *  ======== convertToUInt32 ========
 */
function convertToUInt32(value)
{
    /*
     *  Provide this workaround here due to javascript.
     *  Note: Javascript converts numbers to 32-bit signed numbers before
     *        evaluating bitwise operators.
     */
    if (value < 0) {
        value = value + 0xffffffff + 1;
    }
    
    return (value);
}

/*
 *  ======== viewGetPolicy ========
 *  Used as a helper function for the viewPages function
 */
function viewGetPolicy(elem, policyVal) 
{
    /* Determine the type of level 1 descriptor */
    if (policyVal & 0x3 == 0x1) {
        /* Case its a "Page Table Address" */
        elem.Type = "Page Table Address";
            
        /* Set the level 2 table pointer */
        elem.Level2TablePtr = utils.toHex(convertToUInt32(
                              Number(policyVal & 0xFFFFFC00)));
        
        /*
         *  The elem.AddrPhysical, elem.Bufferable, and elem.Cacheable
         *  are left blank because they are determined by the level 2
         *  descriptor.  Also elem.L1DWritePolicy and elemAccess are
         *  "unknown" because they are determine by level 2 descriptor.
         */
        elem.Domain = (policyVal & (0xF << 5)) >>> 5;
        elem.IMP = (policyVal & (0x1 << 9)) >>> 9;
        elem.L1DWritePolicy = "Unknown";
        elem.Access = "Unknown";
    }
    else {
        /* Case its a "Section" */
        elem.Type = "Section";

        /* Set the memory-region attributes type extension (TEX) field */
        elem.Tex = Number((policyVal >> 12) & 0x7).toString(2) + 'b';

        /* Set the bufferability and cacheability fields */
        elem.Bufferable = policyVal & (0x1 << 2);
        elem.Cacheable  = policyVal & (0x1 << 3);
        elem.Shareable  = policyVal & (0x1 << 16);
        elem.Noexecute  = policyVal & (0x1 << 4);
        
        /* Determine the cacheability policy */
        if (elem.Cacheable) {
            if (elem.Bufferable) {
                elem.L1DWritePolicy = "Write Back";
            }
            else {
                elem.L1DWritePolicy = "Write Through";
            }
        }
        else {
            elem.L1DWritePolicy = "Non cacheable";
        }
        
        /* Set the IMP and Domain fields */
        elem.Domain = (policyVal & (0xF << 5)) >>> 5;
        elem.IMP    = (policyVal & (0x1 << 9)) >>> 9;
        
        /* Determine the Access policy */
        var ap = ((policyVal >> 13) & 0x4) | ((policyVal >> 10) & 0x3);

        switch (ap)
        {
            case 0:  elem.Access = "No access";
                     break;
            case 1:
            case 2:
            case 3:  elem.Access = "Read/Write";
                     break;
            case 4:  elem.Access = "Unknown";
                     break;
            case 5:
            case 6:
            case 7:  elem.Access = "Read-only";
                     break;
            default: elem.Access = "Unknown";
        } 
        
        /* Set the physical address */
        elem.AddrPhysical = utils.toHex(convertToUInt32(
                            Number(policyVal & 0xFFF00000)));
    }
}

/*
 *  ======== viewPages ========
 */
function viewPages(view)
{
    var Program = xdc.useModule('xdc.rov.Program');
    var Mmu = xdc.useModule('ti.sysbios.family.arm.a9.Mmu');
    var mmuTableSize = 0x1000;             /* 4K words = 16KB */

    /* Scan the raw view in order to obtain the module state. */
    var rawView;
    try {
        rawView = Program.scanRawView('ti.sysbios.family.arm.a9.Mmu');
    }
    catch (e) {
        // TODO - report problem.
        print("Caught exception while retrieving raw view: " + e);
    }
    
    /* Get the module state */
    var mod = rawView.modState;

    /* Retrieve the MMU descriptor table */
    try {
        var mmuDescTable = Program.fetchArray(mod.tableBuf$fetchDesc,
                                              mod.tableBuf,
                                              mmuTableSize);
    }
    catch (e) {
        // TODO - Report problem
        print("Caught exception while trying to retrieve dispatch table: "+e);
    }

    /* Walk through the level 1 descriptor table */
    for (var i = 0; i < mmuTableSize; i++) {
        /* Only display mapped entries */
        if ((mmuDescTable[i] & 0x3) != 0) {
            var elem = Program.newViewStruct('ti.sysbios.family.arm.a9.Mmu',
                'PageView');

            /* Virtual address is the index shifted left by 20 */
            elem.AddrVirtual = utils.toHex(convertToUInt32(Number(i << 20)));
            
            /* Set the other descriptor properties */
            viewGetPolicy(elem, mmuDescTable[i]);

            /* Add the element to the list. */
            view.elements.$add(elem);
        }
    }
}
