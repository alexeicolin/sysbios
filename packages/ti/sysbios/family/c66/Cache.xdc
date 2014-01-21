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
 *  ======== Cache.xdc ========
 *
 *
 */

package ti.sysbios.family.c66;

import xdc.rov.ViewInfo;

/*!
 *  ======== Cache ========
 *  Cache Module
 *
 *  This Cache module provides C66 family-specific implementations of the
 *  APIs defined in {@link ti.sysbios.interfaces.ICache ICache}.  It also
 *  provides additional C66 specific cache functions.
 *
 *  Unconstrained Functions
 *  All functions
 *
 *  @p(html)
 *  <h3> Calling Context </h3>
 *  <table border="1" cellpadding="3">
 *    <colgroup span="1"></colgroup> <colgroup span="5" align="center"></colgroup>
 *
 *    <tr><th> Function                 </th><th>  Hwi   </th><th>  Swi   </th><th>  Task  </th><th>  Main  </th><th>  Startup  </th></tr>
 *    <!--                                                                                                                 -->
 *    <tr><td> {@link #disable}    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #enable}     </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #getMar*}    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #getMode*}   </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #getSize*}   </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #inv}        </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #invL1pAll*} </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #setMar*}    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #setMode*}   </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #setSize*}   </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #wait}       </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #wb}         </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #wbAll*}     </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #wbInv}      </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #wbInvAll}   </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td colspan="6"> Definitions: <br />
 *       <ul>
 *         <li> <b>Hwi</b>: API is callable from a Hwi thread. </li>
 *         <li> <b>Swi</b>: API is callable from a Swi thread. </li>
 *         <li> <b>Task</b>: API is callable from a Task thread. </li>
 *         <li> <b>Main</b>: API is callable during any of these phases: </li>
 *           <ul>
 *             <li> In your module startup after this module is started (e.g. Mod_Module_startupDone() returns TRUE). </li>
 *             <li> During xdc.runtime.Startup.lastFxns. </li>
 *             <li> During main().</li>
 *             <li> During BIOS.startupFxns.</li>
 *           </ul>
 *         <li> <b>Startup</b>: API is callable during any of these phases:</li>
 *           <ul>
 *             <li> During xdc.runtime.Startup.firstFxns.</li>
 *             <li> In your module startup before this module is started (e.g. Mod_Module_startupDone() returns FALSE).</li>
 *           </ul>
 *       <li> <b>*</b>: These APIs are intended to be made at initialization time, but are not restricted to this. </li>
 *       </ul>
 *    </td></tr>
 *
 *  </table>
 *  @p
 */

@ModuleStartup

module Cache inherits ti.sysbios.interfaces.ICache
{
    // -------- Module Types --------

    /*!
     *  ======== ModuleView ========
     *  @_nodoc
     */
    metaonly struct ModuleView {
        String  L1PCacheSize;
        String  L1PMode;
        String  L1DCacheSize;
        String  L1DMode;
        String  L2CacheSize;
        String  L2Mode;
    };

    /*!
     *  ======== MarRegisterView ========
     *  @_nodoc
     */
    metaonly struct MarRegisterView {
        UInt    number;
        Ptr     addr;
        Ptr     startAddrRange;
        Ptr     endAddrRange;
        Bool    cacheable;
        Bool    prefetchable;
        String  marRegisterValue;
    };

    /*!
     *  ======== rovViewInfo ========
     *  @_nodoc
     */
    @Facet
    metaonly config ViewInfo.Instance rovViewInfo =
        ViewInfo.create({
            viewMap: [
                ['Module',
                    {
                        type: ViewInfo.MODULE,
                        viewInitFxn: 'viewInitModule',
                        structName: 'ModuleView'
                    }
                ],
                ['MARs',
                    {
                        type: xdc.rov.ViewInfo.MODULE_DATA,
                        viewInitFxn: 'viewInitMarRegisters',
                        structName: 'MarRegisterView'
                    }
                ]
            ]
        });

    /*! Lists of cache modes for L1/L2 caches */
    enum Mode {
        Mode_FREEZE,    /*! No new cache lines are allocated */
        Mode_BYPASS,    /*! All access result in long-distance access */
        Mode_NORMAL     /*! Normal operation of cache */
    };

    /*! Level 1 cache size type definition. Can be used for both L1D & L1P */
    enum L1Size {
        L1Size_0K = 0,  /*! Amount of cache is 0K, Amount of SRAM is 32K */
        L1Size_4K = 1,  /*! Amount of cache is 4K, Amount of SRAM is 28K */
        L1Size_8K = 2,  /*! Amount of cache is 8K, Amount of SRAM is 24K */
        L1Size_16K = 3, /*! Amount of cache is 16K, Amount of SRAM is 16K */
        L1Size_32K = 4  /*! Amount of cache is 32K, Amount of SRAM is 0K */
    };

    /*! Level 2 cache size type definition. */
    enum L2Size {
        L2Size_0K = 0,   /*! L2 is all SRAM */
        L2Size_32K = 1,  /*! Amount of cache is 32K */
        L2Size_64K = 2,  /*! Amount of cache is 64K */
        L2Size_128K = 3, /*! Amount of cache is 128K */
        L2Size_256K = 4, /*! Amount of cache is 256K */
        L2Size_512K = 5, /*! Amount of cache is 512K */
        L2Size_1024K = 6 /*! Amount of cache is 1024K */
    };

    /*! MAR register setting type definition. */
    enum Mar {
        Mar_DISABLE = 0, /*! The Permit Copy bit of MAR register is disabled */
        Mar_ENABLE = 1   /*! The Permit Copy bit of MAR register is enabled */
    };

    const UInt32 PC  = 1;     /*! Permit Caching                     */
    const UInt32 WTE = 2;     /*! Write through enabled              */
    const UInt32 PCX = 4;     /*! Permit caching in external cache   */
    const UInt32 PFX = 8;     /*! Prefetchable by external engines   */

    /*! Structure for specifying all cache sizes. */
    struct Size {
        L1Size l1pSize;         /*! L1 Program cache size */
        L1Size l1dSize;         /*! L1 Data data size */
        L2Size l2Size;          /*! L2 cache size */
    };

    /*! Default sizes of caches.
     * @_nodoc
     */
    config Size initSize = {
        l1pSize: L1Size_32K,
        l1dSize: L1Size_32K,
        l2Size: L2Size_0K
    };

    /*! @_nodoc
     *  MAR 00 - 31 register bitmask. (for addresses 0x00000000 - 0x1FFFFFFF)
     *
     *  If undefined by the user, this parameter is configured to match the
     *  memory map of the platform.
     *  Each memory region defined in the platform will have all of its
     *  corresponding MAR bits set.
     *
     *  To override the default behavior you must initialize this parameter
     *  in your configuration script:
     *
     *  @p(code)
     *  // disable MAR bits for addresses 0x00000000 to 0x1FFFFFFF
     *  Cache.MAR0_31 = 0x00000000;
     *  @p
     */
    metaonly config UInt32 MAR0_31;

    /*! @_nodoc
     *  MAR 32 - 63 register bitmask (for addresses 0x20000000 - 0x3FFFFFFF)
     *
     *  see {@link #MAR0_31} for more info
     */
    metaonly config UInt32 MAR32_63;

    /*! @_nodoc
     *  MAR 64 - 95 register bitmask (for addresses 0x40000000 - 0x5FFFFFFF)
     *
     *  see {@link #MAR0_31} for more info
     */
    metaonly config UInt32 MAR64_95;

    /*! @_nodoc
     *  MAR 96 - 127 register bitmask (for addresses 0x60000000 - 0x7FFFFFFF)
     *
     *  see {@link #MAR0_31} for more info
     */
    metaonly config UInt32 MAR96_127;

    /*! @_nodoc
     *  MAR 128 - 159 register bitmask (for addresses 0x80000000 - 0x9FFFFFFF)
     *
     *  see {@link #MAR0_31} for more info
     */
    metaonly config UInt32 MAR128_159;

    /*! @_nodoc
     *  MAR 160 - 191 register bitmask (for addresses 0xA0000000 - 0xBFFFFFFF)
     *
     *  see {@link #MAR0_31} for more info
     */
    metaonly config UInt32 MAR160_191;

    /*! @_nodoc
     *  MAR 192 - 223 register bitmask (for addresses 0xC0000000 - 0xDFFFFFFF)
     *
     *  see {@link #MAR0_31} for more info
     */
    metaonly config UInt32 MAR192_223;

    /*! @_nodoc
     *  MAR 224 - 255 register bitmask (for addresses 0xE0000000 - 0xFFFFFFFF)
     *
     *  see {@link #MAR0_31} for more info
     */
    metaonly config UInt32 MAR224_255;

    /*! @_nodoc
     *
     *  This parameter is used to break up large blocks into multiple
     *  small blocks which are done atomically.  Each block of the
     *  specified size waits for the cache operation to finish before
     *  starting the next block.  Setting this size to 0, means the
     *  cache operations are not done atomically.
     */
    config UInt32 atomicBlockSize = 1024;

    /*!
     *  ======== getMarMeta ========
     *  Gets the current MAR value for the specified base address
     *
     *  @param(baseAddr)  address for which MAR value is requested
     *
     *  @b(returns)       MAR value for specified address
     */
    metaonly UInt32 getMarMeta(Ptr baseAddr);

    /*!
     *  ======== setMarMeta ========
     *  Set MAR register(s) that corresponds to the specified address range.
     *
     *  The 'pc' ("Permit Caching") field is enabled for all memory regions
     *  in the device platform.  Only set the fields of the Mar structure
     *  which need to be modified.  Any field not set retains its reset value.
     *
     *  @a(Note)
     *  The 'wte' (Bit 1) and 'pcx' (Bit 2) MAR bits are reserved on
     *  C66x CorePac devices.
     *
     *  @param(baseAddr)        start address for which to set MAR
     *  @param(byteSize)        size (in bytes) of memory block
     *  @param(value)           value for setting MAR register
     */
    metaonly Void setMarMeta(Ptr baseAddr, SizeT byteSize, UInt32 value);

    /*!
     *  ======== disable ========
     *  Disables the 'type' cache(s)
     *
     *  Disabling of L2 cache is currently not supported.
     */
    @DirectCall
    override Void disable(Bits16 type);

    /*!
     *  ======== getMode ========
     *  Get mode of a cache
     *
     *  @param(type)     bit mask of cache type
     *  @b(returns)      mode of specified level of cache
     */
    @DirectCall
    Mode getMode(Bits16 type);

    /*!
     *  ======== setMode ========
     *  Set mode of a cache
     *
     *  @param(type)    bit mask of cache type
     *  @param(mode)    mode of cache
     *
     *  @b(returns)     previous mode of cache
     */
    @DirectCall
    Mode setMode(Bits16 type, Mode mode);

    /*!
     *  ======== getSize ========
     *  Get sizes of all caches
     *
     *  @param(size)    pointer to structure of type Cache_Size
     */
    @DirectCall
    Void getSize(Size *size);

    /*!
     *  ======== setSize ========
     *  Set sizes of all caches
     *
     *  @param(size)    pointer to structure of type Cache_Size
     */
    @DirectCall
    Void setSize(Size *size);

    /*!
     *  ======== getMar ========
     *  Gets the MAR register for the specified base address
     *
     *  @param(baseAddr)  address for which MAR is requested
     *
     *  @b(returns)       value of MAR register
     */
    @DirectCall
    UInt32 getMar(Ptr baseAddr);

    /*!
     *  ======== setMar ========
     *  Set MAR register(s) that corresponds to the specified address range.
     *
     *  All cached entries in L1 and L2 are written back and invalidated.
     *
     *  @a(Note)
     *  The 'wte' (Bit 1) and 'pcx' (Bit 2) MAR bits are reserved on
     *  C66x CorePac devices.
     *
     *  @param(baseAddr)        start address for which to set MAR
     *  @param(byteSize)        size (in bytes) of memory block
     *  @param(value)           value for setting MAR register
     */
    @DirectCall
    Void setMar(Ptr baseAddr, SizeT byteSize, UInt32 value);

    /*!
     *  ======== invL1pAll ========
     *  Invalidate all of L1 Program cache
     *
     *  Performs a global invalidate of L1P cache.
     *  Polls the L1P invalidate register until done.
     */
    @DirectCall
    Void invL1pAll();

    /*!
     *  ======== wbAll ========
     *  Write back all caches
     *
     *  Perform a global write back.  There is no effect on L1P cache.
     *  All cache lines are left valid in L1D cache and the data in L1D cache
     *  is written back to L2 or external.  All cache lines are left valid in
     *  L2 cache and the data in L2 cache is written back to external.
     */
    @DirectCall
    override Void wbAll();

    /*!
     *  ======== wbInvAll ========
     *  Write back invalidate all caches
     *
     *  Performs a global write back and invalidate.  All cache lines are
     *  invalidated in L1P cache.  All cache lines are written back to L2 or
     *  external and then invalidated in L1D cache.  All cache lines are
     *  written back to external and then invalidated in L2 cache.
     */
    @DirectCall
    override Void wbInvAll();


internal:

    /*!
     *  ======== invPrefetchBuffer ========
     *  Invalidate the prefetch buffer
     */
    Void invPrefetchBuffer();

    /*
     *  ======== Cache_all ========
     */
    Void all(volatile UInt32 *cacheReg);

    /*
     *  ======== block ========
     *  This internal function used by the block cache APIs.
     */
    Void block(Ptr blockPtr, SizeT byteCnt, Bool wait,
               volatile UInt32 *barReg);

    /* cache configuration registers */
    const UInt32 L2CFG  = 0x01840000;
    const UInt32 L1PCFG = 0x01840020;
    const UInt32 L1PCC  = 0x01840024;
    const UInt32 L1DCFG = 0x01840040;
    const UInt32 L1DCC  = 0x01840044;
    const UInt32 MAR    = 0x01848000;

    /* For setting the MAR registers at startup */
    config UInt32 marvalues[256];
}
