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
 *  ======== Mmu.xdc ========
 *
 *
 */

package ti.sysbios.family.arm.a8;

import xdc.rov.ViewInfo;

/*!
 *  ======== Mmu ========
 *  Memory Management Unit Manager.
 *
 *  This module allows the ARM processor to map a virtual address to a
 *  different physical address and enable/disable the MMU.  It does this
 *  through a translation table in memory.  The translation table is
 *  16KB and manages only the first level descriptor table.  Each entry
 *  in the table defines the properties of memory areas of size 1MB.
 *  These properties include memory access permissions, cacheability,
 *  bufferability, and domain access.
 *
 *  By default, the MMU translation table is initialized with
 *  cache-enabled entries for every memory segment defined in the platform.
 *  Cache-disabled entries are also added for the peripheral
 *  addresses used by SYS/BIOS (i.e. Timers, Interrupt controller).
 *
 *  The translation table is placed in
 *  an output section called "ti.sysbios.family.arm.a8.mmuTableSection".
 *  This section is placed into the platform's default dataMemory and
 *  in order to minimize object file size,
 *  specified to not be initialized via the "NOLOAD" type on GNU compilers
 *  and "NOINIT" on TI compilers.
 *
 *  This module does not manage the second level descriptor tables.
 *  A 'SECTION' mapped access requires only a first level fetch.  In
 *  this case, there is no need for a second level descriptor table.
 *  A 'PAGE_TABLE' mapped access requires a second level
 *  descriptor table which can be supplied by the user.
 *
 *  The following is an example of how to place the MMU table
 *  and how to enable L1 data caching for the address range
 *  0x80000000-0x90000000 in the *.cfg file:
 *
 *  @p(code)
 *
 *    var Cache = xdc.useModule('ti.sysbios.family.arm.a8.Cache');
 *    var Mmu = xdc.useModule('ti.sysbios.family.arm.a8.Mmu');
 *
 *    // Enable the cache
 *    Cache.enableCache = true;
 *
 *    // Enable the MMU (Required for L1/L2 data caching)
 *    Mmu.enableMMU = true;
 *
 *    // descriptor attribute structure
 *    var attrs = {
 *        type: Mmu.FirstLevelDesc_SECTION,  // SECTION descriptor
 *        bufferable: true,                  // bufferable
 *        cacheable: true,                   // cacheable
 *    };
 *
 *    // Set the descriptor for each entry in the address range
 *    for (var i=0x80000000; i < 0x90000000; i = i + 0x00100000) {
 *        // Each 'SECTION' descriptor entry spans a 1MB address range
 *        Mmu.setFirstLevelDescMeta(i, i, attrs);
 *    }
 *
 *    var memmap = Program.cpu.memoryMap;
 *    var DDR = null;
 *
 *    // Find DDR in memory map
 *    for (var i=0; i < memmap.length; i++) {
 *        if (memmap[i].name == "DDR") {
 *            DDR = memmap[i];
 *        }
 *    }
 *
 *    // Place the MMU table in the DDR memory segment if it exists
 *    if (DDR != null) {
 *        var sectionName = "ti.sysbios.family.arm.a8.mmuTableSection";
 *        Program.sectMap[sectionName] = new Program.SectionSpec();
 *        Program.sectMap[sectionName].type = "NOINIT";
 *        Program.sectMap[sectionName].loadSegment = "DDR";
 *    }
 *    else {
 *        print("No DDR memory segment was found");
 *    }
 *
 *  @p
 *
 *  The following example demonstrates how to add a peripheral's address
 *  to the MMU table so that it can be accessed by code at runtime:
 *
 *  @p(code)
 *    var Cache = xdc.useModule('ti.sysbios.family.arm.a8.Cache');
 *    var Mmu = xdc.useModule('ti.sysbios.family.arm.a8.Mmu');
 *
 *    // Enable the cache
 *    Cache.enableCache = true;
 *
 *    // Enable the MMU (Required for L1/L2 data caching)
 *    Mmu.enableMMU = true;
 *
 *    // Force peripheral section to be NON cacheable
 *    var peripheralAttrs = {
 *        type : Mmu.FirstLevelDesc_SECTION, // SECTION descriptor
 *        bufferable : false,                // bufferable
 *        cacheable  : false,                // cacheable
 *        shareable  : false,                // shareable
 *        noexecute  : true,                 // not executable
 *    };
 *
 *    // Define the base address of the 1 Meg page
 *    // the peripheral resides in.
 *    var peripheralBaseAddr = 0xa0400000;
 *
 *    // Configure the corresponding MMU page descriptor accordingly
 *    Mmu.setFirstLevelDescMeta(peripheralBaseAddr,
 *                              peripheralBaseAddr,
 *                              peripheralAttrs);
 *  @p
 *
 *  Notes:
 *  @p(blist)
 *      -The 'Supersection' table descriptor is NOT supported.
 *      -The 'not global' page descriptor setting is NOT supported.
 *      -The 'non-secure' page descriptor setting is NOT supported.
 *      -There are size and alignment requirements on the second
 *       level descriptor tables depending on the page size.
 *      -See the ARM Architecture Reference Manual for more info.
 *  @p
 *
 */

@Template ("./Mmu.xdt") /* generate function to init MMU page table */
@ModuleStartup          /* call to initTableBuf() in startup */

module Mmu
{
    // -------- ROV views --------

    /*! @_nodoc */
    metaonly struct PageView {
        String      Type;
        Ptr         AddrVirtual;
        Ptr         AddrPhysical;
        Ptr         Level2TablePtr;
        Bool        Bufferable;
        Bool        Cacheable;
        Bool        Shareable;
        Bool        Noexecute;
        String      L1DWritePolicy;
        UInt        IMP;
        UInt        Domain;
        String      Access;
    };

    @Facet
    metaonly config ViewInfo.Instance rovViewInfo =
        ViewInfo.create({
            viewMap: [
                ['PageView', {
                    type: ViewInfo.MODULE_DATA,
                    viewInitFxn: 'viewPages',
                    structName: 'PageView'
                }]
           ]
       });

   /*! First Level descriptors */
    enum FirstLevelDesc {
        FirstLevelDesc_FAULT = 0,       /*! Virtual address is unmapped */
        FirstLevelDesc_PAGE_TABLE = 1,  /*! Page table addr descriptor  */
        FirstLevelDesc_SECTION = 2      /*! Section descriptor          */
    };

    /*!
     *  Structure for setting first level descriptor entries
     *
     *  If the 'cacheable' attribute is true and 'bufferable' is true,
     *  L1 data cache operates as a write-back cache.
     *  If 'cacheable' is true but 'bufferable' is false,
     *  L1 data cache operates as a write-through cache.
     *
     *  See the 'Translation Tables' section of the ARM v7 Architecture
     *  Reference Manual for more details.
     */
    struct FirstLevelDescAttrs {
        FirstLevelDesc type;    /*! first level descriptor type         */
        Bool  bufferable;       /*! is memory section bufferable        */
        Bool  cacheable;        /*! is memory section cacheable         */
        Bool  shareable;        /*! is memory section shareable         */
        Bool  noexecute;        /*! is memory section not executable    */
        UInt8 imp;              /*! implementation defined              */
        UInt8 domain;           /*! domain access control value 0-15    */
        UInt8 accPerm;          /*! access permission bits value 0-3    */
    };

    /*!
     *  ======== A_nullPointer ========
     *  Assert raised when a pointer is null
     */
    config xdc.runtime.Assert.Id A_nullPointer  = {
        msg: "A_nullPointer: Pointer is null"
    };

    /*!
     *  ======== A_unknownDescType ========
     *  Assert raised when the descriptor type is not recognized.
     */
    config xdc.runtime.Assert.Id A_unknownDescType =
        {msg: "A_unknownDescType: Descriptor type is not recognized"};

    /*! default descriptor attributes structure */
    config FirstLevelDescAttrs defaultAttrs = {
        type: FirstLevelDesc_SECTION,   /* SECTION descriptor           */
        bufferable: false,              /* false by default             */
        cacheable: false,               /* false by default             */
        shareable: false,               /* false by default             */
        noexecute: false,               /* false by default             */
        imp: 1,                         /* set to 1 for A8 devices      */
        domain: 0,                      /* default Domain is 0          */
        accPerm: 3,                     /* allow read/write             */
    };

    /*!
     *  ======== enableMMU ========
     *  Configuration parameter to enable MMU.
     */
    config Bool enableMMU = true;

    /*!
     *  ======== cachePlatformMemory ========
     *  Flag to automatically mark platform's code/data/stack memory as
     *  cacheable in MMU descriptor table
     *
     *  By default, all memory regions defined in the platform an
     *  application is built with are marked as cacheable.
     *
     *  @see xdc.bld.Program#platform
     *
     *  If manual configuration of memory regions is required, set
     *  this config parameter to 'false'.
     */
    metaonly config Bool cachePlatformMemory = true;

    /*!
     *  ======== setFirstLevelDescMeta ========
     *  Statically sets the descriptor for the virtual address.
     *
     *  The first level table entry for the virtual address is mapped
     *  to the physical address with the attributes specified. The
     *  descriptor table is effective when the MMU is enabled.
     *
     *  @param(virtualAddr)  The modified virtual address
     *  @param(phyAddr)      The physical address
     *  @param(attrs)        Pointer to first level descriptor attribute struct
     */
    metaonly Void setFirstLevelDescMeta(Ptr virtualAddr, Ptr phyAddr,
                                        FirstLevelDescAttrs attrs);

    /*!
     *  ======== disable ========
     *  Disables the MMU.
     *
     *  If the MMU is already disabled, then simply return.
     *  Otherwise this function does the following:
     *  If the L1 data cache is enabled, write back invalidate all
     *  of L1 data cache.  If the L1 program cache is enabled,
     *  invalidate all of L1 program cache. This function does not
     *  change the cache L1 data/program settings.
     */
    @DirectCall
    Void disable();

    /*!
     *  ======== enable ========
     *  Enables the MMU.
     *
     *  If the MMU is already enabled, then simply return.
     *  Otherwise this function does the following:
     *  If the L1 program cache is enabled, invalidate all of L1
     *  program cache.  This function does not change the L1
     *  data/program cache settings.
     */
    @DirectCall
    Void enable();

    /*!
     *  ======== initDescAttrs() ========
     *  Initializes the first level descriptor attribute structure
     *
     *  @param(attrs)      Pointer to first level descriptor attribute struct
     */
    @DirectCall
    Void initDescAttrs(FirstLevelDescAttrs *attrs);

    /*!
     *  ======== isEnabled ========
     *  Determines if the MMU is enabled
     */
    @DirectCall
    Bool isEnabled();

    /*!
     *  ======== setFirstLevelDesc ========
     *  Sets the descriptor for the virtual address.
     *
     *  The first level table entry for the virtual address is mapped
     *  to the physical address with the attributes specified. The
     *  descriptor table is effective when the MMU is enabled.
     *
     *  @param(virtualAddr)  The modified virtual address
     *  @param(phyAddr)      The physical address
     *  @param(attrs)        Pointer to first level descriptor attribute struct
     */
    @DirectCall
    Void setFirstLevelDesc(Ptr virtualAddr, Ptr phyAddr,
                           FirstLevelDescAttrs *attrs);


internal:

    /*! static array to hold first level dscriptor table */
    metaonly config UInt32 tableBuf[];

    /*!
     *  ======== enableAsm ========
     *  Assembly function to enable the MMU.
     */
    Void enableAsm();

    /*!
     *  ======== disableAsm ========
     *  Assembly function to disable the MMU.
     */
    Void disableAsm();

    /*! function generated to initialize first level descriptor table */
    Void initTableBuf(Module_State *mod);

    /*! Module state */
    struct Module_State {
        UInt32 tableBuf[];      /*! 16KB array for first-level descriptors */
    }
}
