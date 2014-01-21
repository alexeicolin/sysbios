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
 *  ======== Build.xs ========
 */

var BIOS = null;
var Build = null;

var custom28xOpts = " -q -mo ";
var custom430xOpts = " -q --advice:power=1 ";
var customIar430xOpts = " --silent --diag_suppress=Pa050,Go005,Pe1053 -D_DLIB_FILE_DESCRIPTOR=1";
var custom6xOpts = " -q -mi10 -mo -pdr -pden -pds=238 -pds=880 -pds1110 ";
var customARP32xOpts = " -q --gen_func_subsections ";
var customArmOpts = " -q -ms --opt_for_speed=2 ";
var customGnuArmM3Opts = " ";
var customGnuArmM4Opts = " ";
var customGnuArmM4FOpts = " ";
var customGnuArmA9Opts = " ";
var customGnuArmA8Opts = " ";
var customGnuArmA15Opts = " ";
var customIarArmOpts = " --silent ";

var ccOptsList = {
    "ti.targets.C28_large"                      : custom28xOpts,
    "ti.targets.C28_float"                      : custom28xOpts,
    "ti.targets.msp430.elf.MSP430"              : custom430xOpts,
    "ti.targets.msp430.elf.MSP430X"             : custom430xOpts,
    "ti.targets.msp430.elf.MSP430X_small"       : custom430xOpts,
    "iar.targets.msp430.MSP430"                 : customIar430xOpts,
    "iar.targets.msp430.MSP430X_small"          : customIar430xOpts,
    "iar.targets.msp430.MSP430X_large"          : customIar430xOpts,
    "ti.targets.C64P"                           : custom6xOpts,
    "ti.targets.elf.C64P"                       : custom6xOpts,
    "ti.targets.C64P_big_endian"                : custom6xOpts,
    "ti.targets.elf.C64P_big_endian"            : custom6xOpts,
    "ti.targets.C674"                           : custom6xOpts,
    "ti.targets.elf.C674"                       : custom6xOpts,
    "ti.targets.elf.C67P"                       : custom6xOpts,
    "ti.targets.elf.C64T"                       : custom6xOpts,
    "ti.targets.elf.C66"                        : custom6xOpts,
    "ti.targets.elf.C66_big_endian"             : custom6xOpts,
    "ti.targets.arp32.elf.ARP32"                : customARP32xOpts,
    "ti.targets.arp32.elf.ARP32_far"            : customARP32xOpts,
    "ti.targets.arm.elf.Arm9"                   : customArmOpts,
    "ti.targets.arm.elf.A8F"                    : customArmOpts,
    "ti.targets.arm.elf.A8Fnv"                  : customArmOpts,
    "ti.targets.arm.elf.M3"                     : customArmOpts,
    "ti.targets.arm.elf.M4"                     : customArmOpts,
    "ti.targets.arm.elf.M4F"                    : customArmOpts,
    "gnu.targets.arm.M3"                        : customGnuArmM3Opts,
    "gnu.targets.arm.M4"                        : customGnuArmM4Opts,
    "gnu.targets.arm.M4F"                       : customGnuArmM4FOpts,
    "gnu.targets.arm.A8F"                       : customGnuArmA8Opts,
    "gnu.targets.arm.A9F"                       : customGnuArmA9Opts,
    "gnu.targets.arm.A15F"                      : customGnuArmA15Opts,
    "iar.targets.arm.M3"                        : customIarArmOpts,
    "iar.targets.arm.M4"                        : customIarArmOpts,
    "iar.targets.arm.M4F"                       : customIarArmOpts,
};

/*
 *  ======== module$meta$init ========
 */
function module$meta$init()
{
    /* Only process during "cfg" phase */
    if (xdc.om.$name != "cfg") {
        return;
    }

    Build = this;

    /*
     * Set default verbose level for custom build flow
     * User can override this in their cfg file.
     */
    var SourceDir = xdc.module("xdc.cfg.SourceDir");
    SourceDir.verbose = 2;
}

/*
 *  ======== module$use ========
 */
function module$use()
{
    BIOS = xdc.module("ti.sysbios.BIOS");

    var profile;

    /*
     * Get the profile associated with the ti.sysbios package.
     * The profile can be specified on a per package basis with a line like
     * this in your .cfg script:
     *
     * xdc.loadPackage('ti.sysbios').profile = "release";
     */
    if (this.$package.profile != undefined) {
        profile = this.$package.profile;
    }
    else {
        profile = Program.build.profile;
    }

    /*
     * Gracefully handle non-supported whole_program profiles
     */
    if (profile.match(/whole_program/)
        && (BIOS.libType != BIOS.LibType_Debug)) {
        /* allow build to proceed */
        BIOS.libType = BIOS.LibType_Debug;
        /* but warning the user */
        BIOS.$logWarning("The '" + profile +
            "' build profile will not be supported " +
            "in future releases of SYS/BIOS.  " +
            "Use 'release' or 'debug' profiles together with the " +
            "'BIOS.libType' configuration parameter to specify your " +
            "preferred library.  See the compatibility section of " +
            "your SYS/BIOS release notes for more information.",
            "Profile Deprecation Warning", BIOS);
    }

    if (Build.buildROMApp == true) {
        return;
    }

    /* inform getLibs() about location of library */
    switch (BIOS.libType) {
        case BIOS.LibType_Instrumented:
            this.$private.libraryName = "/sysbios.a" + Program.build.target.suffix;
            this.$private.outputDir = this.$package.packageBase + "lib/"
                        + (BIOS.smpEnabled ? "smpbios/instrumented/" : "sysbios/instrumented/");
            break;

        case BIOS.LibType_NonInstrumented:
            this.$private.libraryName = "/sysbios.a" + Program.build.target.suffix;
            this.$private.outputDir = this.$package.packageBase + "lib/"
                        + (BIOS.smpEnabled ? "smpbios/nonInstrumented/" : "sysbios/nonInstrumented/");
            break;

        case BIOS.LibType_Debug:
        case BIOS.LibType_Custom:
            this.$private.libraryName = "/sysbios.a" + Program.build.target.suffix;
            var SourceDir = xdc.useModule("xdc.cfg.SourceDir");
            /* if building a pre-built library */
            if (BIOS.buildingAppLib == false) {
                var appName = Program.name.substring(0, Program.name.lastIndexOf('.'));
                this.$private.libDir = this.$package.packageBase + BIOS.libDir;
                if (!java.io.File(this.$private.libDir).exists()) {
                    java.io.File(this.$private.libDir).mkdir();
                }
            }
            /*
             * If building an application in CCS or package.bld world
             * and libDir has been specified
             */
            if ((BIOS.buildingAppLib == true) && (BIOS.libDir !== null)) {
                SourceDir.outputDir = BIOS.libDir;
                var src = SourceDir.create("sysbios");
                src.libraryName = this.$private.libraryName.substring(1);
                this.$private.outputDir = src.getGenSourceDir();
            }
            else {
                var curPath = java.io.File(".").getCanonicalPath();
                /* If package.bld world AND building an application OR pre-built lib */
                if (java.io.File(curPath).getName() != "configPkg") {
                    var appName = Program.name.substring(0, Program.name.lastIndexOf('.'));
                    appName = appName + "_p" + Program.build.target.suffix + ".src";
                    SourceDir.outputDir = "package/cfg/" + appName;
                    SourceDir.toBuildDir = ".";
                    var src = SourceDir.create("sysbios");
                    src.libraryName = this.$private.libraryName.substring(1);
                    this.$private.outputDir = src.getGenSourceDir();
                }
                /* Here ONLY if building an application in CCS world */
                else {
                    /* request output source directory for generated files */
                    SourceDir.toBuildDir = "..";
                    var src = SourceDir.create("sysbios/");
                    src.libraryName = this.$private.libraryName.substring(1);

                    /* save this directory in our private state (to be read during
                    * generation, see Gen.xdt)
                    */
                    this.$private.outputDir = src.getGenSourceDir();
                }
            }
            break;
    }

}

var sysbiosSources = "BIOS.c ";

var gatesSources = "gates/GateHwi.c gates/GateAll.c " +
                   "gates/GateSwi.c gates/GateTask.c " +
                   "gates/GateTest.c gates/GateMutex.c " +
                   "gates/GateMutexPri.c ";

var halSources   =  "hal/Hwi.c hal/Hwi_stack.c hal/Hwi_startup.c " +
                    "hal/Timer.c hal/TimerNull.c hal/Cache.c " +
                    "hal/CacheNull.c hal/Core.c hal/CoreNull.c ";

var heapsSources = "heaps/HeapMem.c " +
                   "heaps/HeapBuf.c " +
                   "heaps/HeapNull.c " +
                   "heaps/HeapMultiBuf.c " +
                   "heaps/HeapCallback.c " +
                   "heaps/HeapTrack.c ";

var ioSources    = "io/DEV.c io/GIO.c io/IOM_stub.c ";

var knlSources   = "knl/Clock.c knl/Idle.c knl/Intrinsics.c " +
                   "knl/Swi.c knl/Swi_andn.c knl/Task.c " +
                   "knl/Event.c knl/Semaphore.c knl/Mailbox.c " +
                   "knl/Queue.c ";

var miscSources  = "misc/Stats.c ";

var rtaSources   = "rta/SupportLogger.c ";

var syncsSources = "syncs/SyncSem.c syncs/SyncSwi.c " +
                   "syncs/SyncEvent.c ";

var smpSources =    "smp/SysMin.c " +
                    "smp/SysStd.c " +
                    "smp/Load.c " +
                    "smp/LoggerBuf.c ";

var utilsSources = "utils/Load.c " +
                   "utils/Load_CPU.c ";

var xdcruntimeSources =
                   "xdcruntime/GateThreadSupport.c " +
                   "xdcruntime/GateProcessSupport.c " +
                   "xdcruntime/SemThreadSupport.c " +
                   "xdcruntime/SemProcessSupport.c " +
                   "xdcruntime/ThreadSupport.c " +
                   "xdcruntime/CacheSupport.c ";

var iarCommonSources = "rts/iar/MultithreadSupport.c ";

var commonSources = sysbiosSources + knlSources + heapsSources + gatesSources +
                    halSources + rtaSources + utilsSources + syncsSources +
                    xdcruntimeSources + ioSources + miscSources;

var dmtimerSources = "timers/dmtimer/Timer.c " +
                     "timers/dmtimer/TimestampProvider.c ";

var gptimerSources = "timers/gptimer/Timer.c ";

var timer64Sources = "timers/timer64/Timer.c " +
                     "timers/timer64/TimestampProvider.c ";

var C62Sources  = "family/c62/TaskSupport.c " +
                  "family/c62/IntrinsicsSupport.c ";

var C66Sources =  "family/c64p/Hwi.c " +
                  "family/c64p/Hwi_startup.c " +
                  "family/c64p/TimestampProvider.c " +
                  "family/c64p/Exception.c " +
                  "family/c64p/EventCombiner.c " +
                  "family/c64p/MemoryProtect.c " +
                  "family/c66/Cache.c " +
                  "family/c66/tci66xx/CpIntc.c " +
                  "family/c64p/tci6488/TimerSupport.c " +
                  "family/shared/vayu/TimerSupport.c " +
                  "family/shared/vayu/IntXbar.c ";

var C674Sources = "family/c64p/Hwi.c " +
                  "family/c64p/Hwi_startup.c " +
                  "family/c64p/TimestampProvider.c " +
                  "family/c64p/Exception.c " +
                  "family/c64p/EventCombiner.c " +
                  "family/c64p/Cache.c " +
                  "family/c64p/MemoryProtect.c " +
                  "family/c64p/omap3430/TimerSupport.c " +
                  "family/c64p/omap3430/Wugen.c " +
                  "family/c64p/primus/TimerSupport.c " +
                  "family/c64p/ti81xx/TimerSupport.c " +
                  "family/c674/Power.c ";

var C67PSources = "family/c67p/Hwi.c " +
                  "family/c67p/Timer.c " +
                  "family/c67p/TimestampProvider.c " +
                  "family/c67p/Cache.c ";

var C64PSources = "family/c64p/Hwi.c " +
                  "family/c64p/Hwi_startup.c " +
                  "family/c64p/TimestampProvider.c " +
                  "family/c64p/Exception.c " +
                  "family/c64p/EventCombiner.c " +
                  "family/c64p/Cache.c " +
                  "family/c64p/MemoryProtect.c " +
                  "family/c64p/dm6437/TimerSupport.c " +
                  "family/c64p/dm6446/TimerSupport.c " +
                  "family/c64p/omap3430/TimerSupport.c " +
                  "family/c64p/omap3430/Wugen.c " +
                  "family/c64p/tci6482/TimerSupport.c " +
                  "family/c64p/tci6488/TimerSupport.c " +
                  "family/c64p/ti81xx/TimerSupport.c ";

var C64TSources = "family/c64p/Hwi.c " +
                  "family/c64p/Hwi_startup.c " +
                  "family/c64p/TimestampProvider.c " +
                  "family/c64p/Exception.c " +
                  "family/c64p/EventCombiner.c " +
                  "family/c64p/tesla/Power.c " +
                  "family/c64p/tesla/TimerSupport.c " +
                  "family/c64p/tesla/Wugen.c " +
                  "hal/unicache/Cache.c " +
                  "hal/ammu/AMMU.c ";

var M3CommonSources   = "family/arm/m3/Hwi.c " +
                  "family/arm/m3/TaskSupport.c " +
                  "family/arm/m3/Timer.c " +
                  "family/arm/m3/TimestampProvider.c " +
                  "family/arm/m3/Power.c " +
                  "family/arm/lm3/Timer.c " +
                  "family/arm/lm3/TimestampProvider.c " +
                  "family/arm/lm4/Timer.c " +
                  "family/arm/lm4/TimestampProvider.c ";

var M3Sources   = M3CommonSources +
                  "family/arm/ducati/Core.c " +
                  "family/arm/ducati/GateDualCore.c " +
                  "family/arm/ducati/GateSmp.c " +
                  "family/arm/ducati/Timer.c " +
                  "family/arm/ducati/TimerSupport.c " +
                  "family/arm/ducati/TimestampProvider.c " +
                  "family/arm/ducati/Wugen.c " +
                  "family/arm/ducati/omap4430/Power.c " +
                  "family/arm/ducati/smp/Power.c " +
                  "family/arm/ducati/dm8148/IntMux.c " +
                  "family/arm/f28m35x/TimestampProvider.c " +
                  "hal/unicache/Cache.c " +
                  "hal/ammu/AMMU.c ";

var M3gSources  = M3Sources +
                  "rts/gnu/ReentSupport.c " +
                  "rts/gnu/SemiHostSupport.c ";

var M3rSources  = M3CommonSources +
                  iarCommonSources;

var M4CommonSources  = "family/arm/m3/Hwi.c " +
                  "family/arm/m3/TaskSupport.c " +
                  "family/arm/m3/Timer.c " +
                  "family/arm/m3/TimestampProvider.c " +
                  "family/arm/lm4/Timer.c " +
                  "family/arm/lm4/TimestampProvider.c " +
                  "family/arm/tm4l/Power.c " +
                  "family/arm/tm4l/Timer.c " +
                  "family/arm/tm4l/TimestampProvider.c ";

var M4Sources  =  M4CommonSources +
                  "family/arm/ducati/Core.c " +
                  "family/arm/ducati/GateDualCore.c " +
                  "family/arm/ducati/GateSmp.c " +
                  "family/arm/ducati/Timer.c " +
                  "family/arm/ducati/TimerSupport.c " +
                  "family/arm/ducati/TimestampProvider.c " +
                  "family/arm/ducati/Wugen.c " +
                  "family/arm/ducati/omap4430/Power.c " +
                  "family/arm/ducati/smp/Power.c " +
                  "family/arm/ducati/dm8148/IntMux.c " +
                  "family/shared/vayu/TimerSupport.c " +
                  "family/shared/vayu/IntXbar.c " +
                  "hal/unicache/Cache.c " +
                  "hal/ammu/AMMU.c ";

var M4gSources  = M4Sources +
                  "rts/gnu/ReentSupport.c " +
                  "rts/gnu/SemiHostSupport.c ";

var M4rSources  = M4CommonSources +
                  iarCommonSources;

var Arm9Sources = "family/arm/TaskSupport.c " +
                  "family/arm/HwiCommon.c " +
                  "family/arm/IntrinsicsSupport.c " +
                  "family/arm/arm9/Cache.c " +
                  "family/arm/arm9/Mmu.c " +
                  "family/arm/exc/Exception.c " +
                  "family/arm/da830/TimerSupport.c " +
                  "family/arm/da830/Clobber.c " +
                  "family/arm/da830/Hwi.c " +
                  "family/arm/dm6446/TimerSupport.c " +
                  "family/arm/dm6446/Clobber.c " +
                  "family/arm/dm6446/Hwi.c " +
                  "family/arm/omap1030/TimestampProvider.c " +
                  "family/arm/omap1030/Hwi.c " +
                  "family/arm/sim1030/Timer.c " +
                  "family/arm/sim1030/TimestampProvider.c " +
                  "family/arm/sim1030/Hwi.c ";

var A8FSources  = "family/arm/TaskSupport.c " +
                  "family/arm/IntrinsicsSupport.c " +
                  "family/arm/a8/Cache.c " +
                  "family/arm/a8/Mmu.c " +
                  "family/arm/a8/TimestampProvider.c " +
                  "family/arm/a8/intcps/Hwi.c " +
                  "family/arm/a8/omap3430/TimerSupport.c " +
                  "family/arm/a8/ti81xx/TimerSupport.c " +
                  "family/arm/exc/Exception.c " +
                  "family/arm/v7a/Pmu.c " +
                  "family/arm/v7a/Timer.c ";

var A8gSources  = A8FSources +
                  "rts/gnu/ReentSupport.c " +
                  "rts/gnu/SemiHostSupport.c ";

var A9Sources =   "family/arm/TaskSupport.c " +
                  "family/arm/IntrinsicsSupport.c " +
                  "family/arm/a15/TimestampProvider.c " +
                  "family/arm/a8/Mmu.c " +
                  "family/arm/a9/Cache.c " +
                  "family/arm/a9/Timer.c " +
                  "family/arm/a9/TimerSupport.c " +
                  "family/arm/a9/am437x/TimerSupport.c " +
                  "family/arm/exc/Exception.c " +
                  "family/arm/gic/Hwi.c " +
                  "family/arm/v7a/Pmu.c " +
                  "family/arm/v7a/Timer.c ";

var A9gSources  = A9Sources +
                  "rts/gnu/ReentSupport.c " +
                  "rts/gnu/SemiHostSupport.c ";

var A15gSources = "family/arm/TaskSupport.c " +
                  "family/arm/IntrinsicsSupport.c " +
                  "family/arm/a15/TimestampProvider.c " +
                  "family/arm/a15/Cache.c " +
                  "family/arm/a15/Mmu.c " +
                  "family/arm/exc/Exception.c " +
                  "family/arm/gic/Hwi.c " +
                  "family/arm/systimer/Timer.c " +
                  "family/arm/v7a/Pmu.c " +
                  "family/arm/v7a/Timer.c " +
                  "family/shared/vayu/TimerSupport.c " +
                  "family/shared/vayu/IntXbar.c " +
                  "rts/gnu/ReentSupport.c " +
                  "rts/gnu/SemiHostSupport.c ";

var C28Sources =  "family/c28/Hwi.c " +
                  "family/c28/TaskSupport.c " +
                  "family/c28/Timer.c " +
                  "family/c28/TimestampProvider.c " +
                  "family/c28/f2837x/TimestampProvider.c " +
                  "family/c28/f28m35x/TimestampProvider.c ";

var MSP430Sources = "family/msp430/Hwi.c " +
                  "family/msp430/IntrinsicsSupport.c " +
                  "family/msp430/Power.c " +
                  "family/msp430/TaskSupport.c " +
                  "family/msp430/ClockFreqs.c " +
                  "family/msp430/Timer.c " +
                  "family/msp430/TimestampProvider.c ";

var ARP32Sources =  "family/arp32/Hwi.c " +
                  "family/arp32/IntrinsicsSupport.c " +
                  "family/arp32/TaskSupport.c " +
                  "family/arp32/Timer.c " +
                  "family/arp32/Cache.c " +
                  "family/arp32/TimestampProvider.c " +
                  "family/shared/vayu/IntXbar.c ";

var cList = {
    "ti.targets.C28_large"          : commonSources + C28Sources,
    "ti.targets.C28_float"          : commonSources + C28Sources,

    "ti.targets.msp430.elf.MSP430"  : commonSources + MSP430Sources,
    "ti.targets.msp430.elf.MSP430X" : commonSources + MSP430Sources,
    "ti.targets.msp430.elf.MSP430X_small" : commonSources + MSP430Sources,

    "iar.targets.msp430.MSP430"     : commonSources + MSP430Sources
                                      + iarCommonSources,
    "iar.targets.msp430.MSP430X_small" : commonSources + MSP430Sources
                                         + iarCommonSources,
    "iar.targets.msp430.MSP430X_large" : commonSources + MSP430Sources
                                         + iarCommonSources,

    "ti.targets.C64P"               : commonSources + timer64Sources + dmtimerSources +
                                      gptimerSources + C62Sources + C64PSources,
    "ti.targets.elf.C64P"           : commonSources + timer64Sources + dmtimerSources +
                                      gptimerSources + C62Sources + C64PSources,
    "ti.targets.C64P_big_endian"    : commonSources + timer64Sources + dmtimerSources +
                                      gptimerSources + C62Sources + C64PSources,
    "ti.targets.elf.C64P_big_endian": commonSources + timer64Sources + dmtimerSources +
                                      gptimerSources + C62Sources + C64PSources,
    "ti.targets.C674"               : commonSources + timer64Sources + dmtimerSources +
                                      C62Sources + C674Sources,
    "ti.targets.elf.C674"           : commonSources + timer64Sources + dmtimerSources +
                                      C62Sources + C674Sources,
    "ti.targets.elf.C67P"           : commonSources + C62Sources + C67PSources,
    "ti.targets.elf.C64T"           : commonSources + dmtimerSources + C62Sources + C64TSources,
    "ti.targets.elf.C66"            : commonSources + timer64Sources + dmtimerSources +
                                      C62Sources + C66Sources,
    "ti.targets.elf.C66_big_endian" : commonSources + timer64Sources +
                                      C62Sources + C66Sources,
    "ti.targets.arp32.elf.ARP32"    : commonSources + ARP32Sources,
    "ti.targets.arp32.elf.ARP32_far": commonSources + ARP32Sources,

    "ti.targets.arm.elf.Arm9"       : commonSources + timer64Sources + dmtimerSources +
                                      Arm9Sources,
    "ti.targets.arm.elf.A8F"        : commonSources + dmtimerSources + gptimerSources +
                                      A8FSources,
    "ti.targets.arm.elf.A8Fnv"      : commonSources + dmtimerSources + gptimerSources +
                                      A8FSources,
    "ti.targets.arm.elf.M3"         : commonSources + dmtimerSources + M3Sources,
    "ti.targets.arm.elf.M4"         : commonSources + dmtimerSources + M4Sources,
    "ti.targets.arm.elf.M4F"        : commonSources + dmtimerSources + M4Sources,

    "gnu.targets.arm.M3"            : commonSources + dmtimerSources + M3gSources,
    "gnu.targets.arm.M4"            : commonSources + dmtimerSources + M4gSources,
    "gnu.targets.arm.M4F"           : commonSources + dmtimerSources + M4gSources,

    "gnu.targets.arm.A8F"           : commonSources + dmtimerSources +
                                      gptimerSources + A8gSources,
    "gnu.targets.arm.A9F"           : commonSources + dmtimerSources +
                                      gptimerSources + A9gSources,
    "gnu.targets.arm.A15F"          : commonSources + dmtimerSources +
                                      A15gSources,

    "iar.targets.arm.M3"            : commonSources + M3rSources,
    "iar.targets.arm.M4"            : commonSources + M4rSources,
    "iar.targets.arm.M4F"           : commonSources + M4rSources,
};

/*
 * Used for trimming the set of C files in the custom BIOS.lib
 * to the minimum required for the application.
 * Only odd case modules are in this list:
 * -More than just Mod.c files
 * -No Mod.c file
 */
var cFiles = {
    "ti.sysbios.utils.Load" :
        { cSources: ["Load.c", "Load_CPU.c"] },
    "ti.sysbios.hal.Hwi" :
        { cSources: ["Hwi.c", "Hwi_stack.c", "Hwi_startup.c"] },
    "ti.sysbios.io.GIO" :
        { cSources: ["GIO.c", "IOM_stub.c"] },
    "ti.sysbios.family.arm.m3.Clobber" :
        { cSources: [] },
    "ti.sysbios.family.arm.m3.IntrinsicsSupport" :
        { cSources: [] },
    "ti.sysbios.family.arm.ducati.CTM" :
        { cSources: [] },
    "ti.sysbios.family.c28.Clobber" :
        { cSources: [] },
    "ti.sysbios.family.c28.IntrinsicsSupport" :
        { cSources: [] },
    "ti.sysbios.family.arp32.CTM" :
        { cSources: [] },
    "ti.sysbios.knl.Swi" :
        { cSources: ["Swi.c", "Swi_andn.c"] },
    "ti.sysbios.family.c64p.Hwi" :
        { cSources: ["Hwi.c", "Hwi_startup.c"] },
    "xdc.runtime.Defaults" :
        { cSources: [] },
    "xdc.runtime.Main" :
        { cSources: [] },
    "xdc.runtime.Types" :
        { cSources: [] },
    "xdc.runtime.Core" :
        { cSources: ["Core-mem.c", "Core-smem.c", "Core-label.c", "Core-params.c"] },
};

var biosPackages = [
    "ti.sysbios.family.arm",
    "ti.sysbios.family.arm.a8",
    "ti.sysbios.family.arm.a8.intcps",
    "ti.sysbios.family.arm.a8.omap3430",
    "ti.sysbios.family.arm.a8.sim",
    "ti.sysbios.family.arm.a8.ti81xx",
    "ti.sysbios.family.arm.a9",
    "ti.sysbios.family.arm.a9.am437x",
    "ti.sysbios.family.arm.a15",
    "ti.sysbios.family.arm.gic",
    "ti.sysbios.family.arm.systimer",
    "ti.sysbios.family.arm.arm9",
    "ti.sysbios.family.arm.da830",
    "ti.sysbios.family.arm.dm6446",
    "ti.sysbios.family.arm.ducati",
    "ti.sysbios.family.arm.ducati.dm8148",
    "ti.sysbios.family.arm.ducati.omap4430",
    "ti.sysbios.family.arm.ducati.smp",
    "ti.sysbios.family.arm.exc",
    "ti.sysbios.family.arm.f28m35x",
    "ti.sysbios.family.arm.lm4",
    "ti.sysbios.family.arm.tm4l",
    "ti.sysbios.family.arm.lm3",
    "ti.sysbios.family.arm.m3",
    "ti.sysbios.family.arm.omap1030",
    "ti.sysbios.family.arm.sim1030",
    "ti.sysbios.family.arm.tms570",
    "ti.sysbios.family.arm.v7a",
    "ti.sysbios.family.c28",
    "ti.sysbios.family.c28.f28m35x",
    "ti.sysbios.family.c28.f2837x",
    "ti.sysbios.family.c62",
    "ti.sysbios.family.c64",
    "ti.sysbios.family.c64p",
    "ti.sysbios.family.c64p.dm6437",
    "ti.sysbios.family.c64p.dm6446",
    "ti.sysbios.family.c64p.omap3430",
    "ti.sysbios.family.c64p.primus",
    "ti.sysbios.family.c64p.tci6482",
    "ti.sysbios.family.c64p.tci6488",
    "ti.sysbios.family.c64p.tesla",
    "ti.sysbios.family.c64p.ti81xx",
    "ti.sysbios.family.c66",
    "ti.sysbios.family.c66.tci66xx",
    "ti.sysbios.family.c67p",
    "ti.sysbios.family.c674",
    "ti.sysbios.family.msp430",
    "ti.sysbios.family.arp32",
    "ti.sysbios.family.shared.vayu",
    "ti.sysbios.gates",
    "ti.sysbios.hal",
    "ti.sysbios.hal.ammu",
    "ti.sysbios.hal.unicache",
    "ti.sysbios.heaps",
    "ti.sysbios.io",
    "ti.sysbios.knl",
    "ti.sysbios.misc",
    "ti.sysbios.rta",
    "ti.sysbios.rts",
    "ti.sysbios.rts.gnu",
    "ti.sysbios.rts.iar",
    "ti.sysbios.rom.c28",
    "ti.sysbios.smp",
    "ti.sysbios.syncs",
    "ti.sysbios.timers.dmtimer",
    "ti.sysbios.timers.gptimer",
    "ti.sysbios.timers.timer64",
    "ti.sysbios.utils",
    "ti.sysbios.xdcruntime",
];

var asmList66 = [
    "family/c64p/Hwi_disp_always.s64P",
    "family/c64p/Hwi_asm.s62",
    "family/c64p/Hwi_asm_switch.s62",
    "family/c64p/Clobber_asm.s62",
    "family/c64p/Exception_asm.s64P",
    "family/c64p/MemoryProtect_asm.s64P",
    "family/c62/TaskSupport_asm.s62",
    "timers/timer64/Timer_asm.s64P",
    "timers/dmtimer/Timer_asm.s64P",
    "../sk/sk_cwrap.asm",
];

var asmList64P = [
    "family/c64p/Hwi_disp_always.s64P",
    "family/c64p/Hwi_asm.s62",
    "family/c64p/Hwi_asm_switch.s62",
    "family/c64p/Clobber_asm.s62",
    "family/c64p/Exception_asm.s64P",
    "family/c64p/MemoryProtect_asm.s64P",
    "family/c62/TaskSupport_asm.s62",
    "timers/timer64/Timer_asm.s64P",
    "timers/gptimer/Timer_asm.s64P",
    "timers/dmtimer/Timer_asm.s64P"
];

var asmList67P = [
    "family/c67p/Clobber_asm.s62",
    "family/c67p/Hwi_asm.s62",
    "family/c67p/Hwi_asm_switch.s62",
    "family/c67p/Hwi_disp_always.s67P",
    "family/c67p/Timer_asm.s67P",
    "family/c62/TaskSupport_asm.s62",
];

var asmList64T = [
    "family/c64p/Hwi_disp_always.s64T",
    "family/c64p/Hwi_asm.s64T",
    "family/c64p/Hwi_asm_switch.s64T",
    "family/c64p/Clobber_asm.s64T",
    "family/c64p/Exception_asm.s64T",
    "family/c64p/tesla/Power_resume.s64T",
    "family/c64p/tesla/Power_standby.s64T",
    "family/c62/TaskSupport_asm.s64T",
    "timers/dmtimer/Timer_asm.s64T"
];

var asmListM3 = [
    "family/arm/m3/Clobber_asm.sv7M",
    "family/arm/m3/IntrinsicsSupport_asm.sv7M",
    "family/arm/m3/TaskSupport_asm.sv7M",
    "family/arm/m3/Hwi_asm.sv7M",
    "family/arm/m3/Hwi_asm_switch.sv7M",
    "family/arm/ducati/CTM_asm.sv7M",
    "family/arm/ducati/Core_asm.sv7M",
    "family/arm/ducati/omap4430/Power_saveCpu.sv7M",
    "family/arm/ducati/omap4430/Power_resumeCpu.sv7M",
    "family/arm/ducati/smp/Power_saveCpu.sv7M",
    "family/arm/ducati/smp/Power_resumeCpu.sv7M",
    "timers/dmtimer/Timer_asm.sv7M"
];

var asmListM4 = [
    "family/arm/m3/Clobber_asm.sv7M",
    "family/arm/m3/IntrinsicsSupport_asm.sv7M",
    "family/arm/m3/TaskSupport_asm.sv7M",
    "family/arm/m3/Hwi_asm.sv7M",
    "family/arm/m3/Hwi_asm_switch.sv7M",
    "family/arm/ducati/CTM_asm.sv7M",
    "family/arm/ducati/Core_asm.sv7M",
    "family/arm/ducati/omap4430/Power_saveCpu.sv7M",
    "family/arm/ducati/omap4430/Power_resumeCpu.sv7M",
    "family/arm/ducati/smp/Power_saveCpu.sv7M",
    "family/arm/ducati/smp/Power_resumeCpu.sv7M",
    "timers/dmtimer/Timer_asm.sv7M"
];

var asmListM3g = [
    "family/arm/m3/Clobber_asm_gnu.sv7M",
    "family/arm/m3/IntrinsicsSupport_asm_gnu.sv7M",
    "family/arm/m3/TaskSupport_asm_gnu.sv7M",
    "family/arm/m3/Hwi_asm_gnu.sv7M",
    "family/arm/m3/Hwi_asm_switch_gnu.sv7M",
    "family/arm/ducati/CTM_asm_gnu.sv7M",
    "family/arm/ducati/Core_asm_gnu.sv7M",
    "family/arm/ducati/omap4430/Power_saveCpu_gnu.sv7M",
    "family/arm/ducati/omap4430/Power_resumeCpu_gnu.sv7M",
    "family/arm/ducati/smp/Power_saveCpu_gnu.sv7M",
    "family/arm/ducati/smp/Power_resumeCpu_gnu.sv7M",
    "timers/dmtimer/Timer_asm_gnu.sv7M"
];

var asmListM3r = [
    "family/arm/m3/Clobber_asm_iar.sv7M",
    "family/arm/m3/IntrinsicsSupport_asm_iar.sv7M",
    "family/arm/m3/TaskSupport_asm_iar.sv7M",
    "family/arm/m3/Hwi_asm_iar.sv7M",
    "family/arm/m3/Hwi_asm_switch_iar.sv7M",
];

var asmListM4g = [
    "family/arm/m3/Clobber_asm_gnu.sv7M",
    "family/arm/m3/IntrinsicsSupport_asm_gnu.sv7M",
    "family/arm/m3/TaskSupport_asm_gnu.sv7M",
    "family/arm/m3/Hwi_asm_gnu.sv7M",
    "family/arm/m3/Hwi_asm_switch_gnu.sv7M",
    "family/arm/ducati/CTM_asm_gnu.sv7M",
    "family/arm/ducati/Core_asm_gnu.sv7M",
    "family/arm/ducati/omap4430/Power_saveCpu_gnu.sv7M",
    "family/arm/ducati/omap4430/Power_resumeCpu_gnu.sv7M",
    "family/arm/ducati/smp/Power_saveCpu_gnu.sv7M",
    "family/arm/ducati/smp/Power_resumeCpu_gnu.sv7M",
    "timers/dmtimer/Timer_asm_gnu.sv7M"
];

var asmListM4r = [
    "family/arm/m3/Clobber_asm_iar.sv7M",
    "family/arm/m3/IntrinsicsSupport_asm_iar.sv7M",
    "family/arm/m3/TaskSupport_asm_iar.sv7M",
    "family/arm/m3/Hwi_asm_iar.sv7M",
    "family/arm/m3/Hwi_asm_switch_iar.sv7M",
];

var asmListA8F = [
    "family/arm/IntrinsicsSupport_asm.asm",
    "family/arm/TaskSupport_asm.asm",
    "family/arm/exc/Exception_asm.asm",
    "family/arm/a8/TimestampProvider_asm.asm",
    "family/arm/a8/Mmu_asm.asm",
    "family/arm/a8/Cache_asm.asm",
    "family/arm/a8/intcps/Hwi_asm.s470",
    "family/arm/a8/intcps/Hwi_asm_switch.s470",
    "timers/gptimer/Timer_asm.s470",
    "timers/dmtimer/Timer_asm.s470"
];

var asmListA8g = [
    "family/arm/IntrinsicsSupport_asm_gnu.asm",
    "family/arm/TaskSupport_asm_gnu.asm",
    "family/arm/exc/Exception_asm_gnu.asm",
    "family/arm/a8/TimestampProvider_asm_gnu.sv7A",
    "family/arm/a8/Mmu_asm_gnu.sv7A",
    "family/arm/a8/Cache_asm_gnu.sv7A",
    "family/arm/a8/intcps/Hwi_asm_gnu.sv7A",
    "timers/gptimer/Timer_asm_gnu.sv7A",
    "timers/dmtimer/Timer_asm_gnu.sv7A"
];

var asmListA9g = [
    "family/arm/IntrinsicsSupport_asm_gnu.asm",
    "family/arm/TaskSupport_asm_gnu.asm",
    "family/arm/exc/Exception_asm_gnu.asm",
    "family/arm/a15/TimestampProvider_asm_gnu.asm",
    "family/arm/a8/Mmu_asm_gnu.sv7A",
    "family/arm/a9/Cache_asm_gnu.sv7A",
    "family/arm/gic/Hwi_asm_gnu.asm",
    "timers/gptimer/Timer_asm_gnu.sv7A",
    "timers/dmtimer/Timer_asm_gnu.sv7A"
];

var asmListA15g = [
    "family/arm/IntrinsicsSupport_asm_gnu.asm",
    "family/arm/TaskSupport_asm_gnu.asm",
    "family/arm/exc/Exception_asm_gnu.asm",
    "family/arm/a15/TimestampProvider_asm_gnu.asm",
    "family/arm/a15/Mmu_asm_gnu.asm",
    "family/arm/a15/Cache_asm_gnu.asm",
    "family/arm/gic/Hwi_asm_gnu.asm",
    "family/arm/systimer/Timer_asm_gnu.asm",
    "timers/gptimer/Timer_asm_gnu.sv7A",
    "timers/dmtimer/Timer_asm_gnu.sv7A"
];

var asmListArm9 = [
    "family/arm/IntrinsicsSupport_asm.asm",
    "family/arm/TaskSupport_asm.asm",
    "family/arm/HwiCommon_asm.asm",
    "family/arm/exc/Exception_asm.asm",
    "family/arm/da830/Hwi_asm.s470",
    "family/arm/dm6446/Hwi_asm.s470",
    "family/arm/omap1030/Hwi_asm.s470",
    "family/arm/sim1030/Hwi_asm.s470",
    "family/arm/sim1030/Clobber_asm.s470",
    "family/arm/arm9/Mmu_asm.asm",
    "family/arm/arm9/Cache_asm.asm",
    "timers/timer64/Timer_asm.s470",
    "timers/dmtimer/Timer_asm.s470"
];

var asmList28 = [
    "family/c28/Clobber_asm.s28",
    "family/c28/Hwi_asm.s28",
    "family/c28/Hwi_disp.s28",
    "family/c28/IntrinsicsSupport_asm.s28",
    "family/c28/TaskSupport_asm.s28"
];

var asmList430 = [
    "family/msp430/TaskSupport_asm.asm",
    "family/msp430/Hwi_switch_asm.asm"
];

var asmListIar430 = [
    "family/msp430/TaskSupport_asm_iar.asm",
    "family/msp430/Hwi_switch_asm_iar.asm"
];

var asmListARP32 = [
    "family/arp32/CTM_asm.sarp32",
    "family/arp32/Hwi_disp_always.sarp32",
    "family/arp32/Hwi_switch_asm.sarp32",
    "family/arp32/TaskSupport_asm.sarp32"
];

var asmList = {
    "ti.targets.C28_large"              : asmList28,
    "ti.targets.C28_float"              : asmList28,
    "ti.targets.msp430.elf.MSP430"      : asmList430,
    "ti.targets.msp430.elf.MSP430X"     : asmList430,
    "ti.targets.msp430.elf.MSP430X_small" : asmList430,
    "iar.targets.msp430.MSP430"         : asmListIar430,
    "iar.targets.msp430.MSP430X_small"  : asmListIar430,
    "iar.targets.msp430.MSP430X_large"  : asmListIar430,
    "ti.targets.C64P"                   : asmList64P,
    "ti.targets.elf.C64P"               : asmList64P,
    "ti.targets.C64P_big_endian"        : asmList64P,
    "ti.targets.elf.C64P_big_endian"    : asmList64P,
    "ti.targets.C674"                   : asmList64P,
    "ti.targets.elf.C674"               : asmList64P,
    "ti.targets.elf.C67P"               : asmList67P,
    "ti.targets.elf.C64T"               : asmList64T,
    "ti.targets.elf.C66"                : asmList66,
    "ti.targets.elf.C66_big_endian"     : asmList66,
    "ti.targets.arp32.elf.ARP32"        : asmListARP32,
    "ti.targets.arp32.elf.ARP32_far"    : asmListARP32,
    "ti.targets.arm.elf.Arm9"           : asmListArm9,
    "ti.targets.arm.elf.A8F"            : asmListA8F,
    "ti.targets.arm.elf.A8Fnv"          : asmListA8F,
    "ti.targets.arm.elf.M3"             : asmListM3,
    "ti.targets.arm.elf.M4"             : asmListM4,
    "ti.targets.arm.elf.M4F"            : asmListM4,
    "gnu.targets.arm.M3"                : asmListM3g,
    "gnu.targets.arm.M4"                : asmListM4g,
    "gnu.targets.arm.M4F"               : asmListM4g,
    "gnu.targets.arm.A8F"               : asmListA8g,
    "gnu.targets.arm.A9F"               : asmListA9g,
    "gnu.targets.arm.A15F"              : asmListA15g,
    "iar.targets.arm.M3"                : asmListM3r,
    "iar.targets.arm.M4"                : asmListM4r,
    "iar.targets.arm.M4F"               : asmListM4r,
};

function getDefaultCustomCCOpts()
{
    var BIOS = xdc.module('ti.sysbios.BIOS');

    /* start with target.cc.opts */
    var customCCOpts = Program.build.target.cc.opts;

    /* add target unique custom ccOpts */
    if (!(ccOptsList[Program.build.target.$name] === undefined)) {
        customCCOpts += ccOptsList[Program.build.target.$name];
    }

    /* Gnu targets need to pick up ccOpts.prefix and suffix */
    if (Program.build.target.$name.match(/gnu/)) {
        customCCOpts += " -O3 ";
        customCCOpts = Program.build.target.ccOpts.prefix + " " + customCCOpts;
        customCCOpts += Program.build.target.ccOpts.suffix + " ";
    }
    else if (Program.build.target.$name.match(/iar/)) {
        customCCOpts += " --mfc -Ohs ";
    }
    else {
        /* ti targets do program level compile */
        customCCOpts += " --program_level_compile -o3 -g --optimize_with_debug ";
    }

    /* undo optimizations if this is a Debug build */
    if (BIOS.libType == BIOS.LibType_Debug) {
        if (Program.build.target.$name.match(/gnu/)) {
            customCCOpts = customCCOpts.replace("-O3","");
            /* add in stack frames for stack back trace */
            customCCOpts += " -mapcs ";
        }
        else if (Program.build.target.$name.match(/iar/)) {
            customCCOpts = customCCOpts.replace("-Ohs","--debug");
        }
        else {
            customCCOpts = customCCOpts.replace(" -o3","");
            customCCOpts = customCCOpts.replace(" --optimize_with_debug","");
            if (Program.build.target.$name.match(/arm/)) {
                customCCOpts = customCCOpts.replace(" --opt_for_speed=2","");
            }
        }
    }

    return (customCCOpts);
}

/*
 *  ======== getDefs ========
 */
function getDefs()
{
    var BIOS = xdc.module("ti.sysbios.BIOS");
    var Hwi = xdc.module("ti.sysbios.hal.Hwi");
    var Settings = xdc.module("ti.sysbios.family.Settings");
    var HwiDelegate = Settings.getDefaultHwiDelegate();
    var HwiDelegateName = HwiDelegate.replace(/\./g, "_");

    var Swi = xdc.module("ti.sysbios.knl.Swi");
    var Task = xdc.module("ti.sysbios.knl.Task");
    var Clock = xdc.module("ti.sysbios.knl.Clock");
    var Semaphore = xdc.module("ti.sysbios.knl.Semaphore");
    var Load = xdc.module("ti.sysbios.utils.Load");
    var Defaults = xdc.module('xdc.runtime.Defaults');
    var Diags = xdc.module("xdc.runtime.Diags");

    var defs =   " -Dti_sysbios_BIOS_swiEnabled__D=" + (BIOS.swiEnabled ? "TRUE" : "FALSE")
               + " -Dti_sysbios_BIOS_taskEnabled__D=" + (BIOS.taskEnabled ? "TRUE" : "FALSE")
               + " -Dti_sysbios_BIOS_clockEnabled__D=" + (BIOS.clockEnabled ? "TRUE" : "FALSE");

    if (xdc.module(HwiDelegate).hooks.length == 0) {
        defs += " -Dti_sysbios_hal_Hwi_DISABLE_ALL_HOOKS";
    }

    if (Swi.hooks.length == 0) {
        defs += " -Dti_sysbios_knl_Swi_DISABLE_ALL_HOOKS";
    }

    if (Task.hooks.length == 0) {
        defs += " -Dti_sysbios_knl_Task_DISABLE_ALL_HOOKS";
    }

    defs += " -Dti_sysbios_BIOS_smpEnabled__D="
            + (BIOS.smpEnabled ? "TRUE" : "FALSE");

    if (BIOS.smpEnabled == false) {
        defs += " -Dti_sysbios_Build_useHwiMacros";
    }

    if ((BIOS.buildingAppLib == true) && (Build.buildROM == false)) {
        defs += " -Dti_sysbios_knl_Swi_numPriorities__D=" + Swi.numPriorities;
        defs += " -Dti_sysbios_knl_Task_deleteTerminatedTasks__D=" + (Task.deleteTerminatedTasks ? "TRUE" : "FALSE");
        defs += " -Dti_sysbios_knl_Task_numPriorities__D=" + Task.numPriorities;

        if (BIOS.useSK) {
            defs += " -Dti_sysbios_BIOS_useSK__D=1";
        }

        /*
         * Add Clock module defs
         */
        switch (Clock.tickSource) {
            case Clock.TickSource_TIMER:
                defs += " -Dti_sysbios_knl_Clock_TICK_SOURCE=ti_sysbios_knl_Clock_TickSource_TIMER";
                break;
            case Clock.TickSource_USER:
                defs += " -Dti_sysbios_knl_Clock_TICK_SOURCE=ti_sysbios_knl_Clock_TickSource_USER";
                break;
            case Clock.TickSource_NULL:
                defs += " -Dti_sysbios_knl_Clock_TICK_SOURCE=ti_sysbios_knl_Clock_TickSource_NULL";
                break;
        }

        if (Clock.tickMode == Clock.TickMode_DYNAMIC) {
            defs += " -Dti_sysbios_knl_Clock_TICK_MODE=ti_sysbios_knl_Clock_TickMode_DYNAMIC";
        }
        else {
            defs += " -Dti_sysbios_knl_Clock_TICK_MODE=ti_sysbios_knl_Clock_TickMode_PERIODIC";
        }

        /*
         * map hal Core module APIs to their delegates
         */

        var coreDelegate;

        if (xdc.module('ti.sysbios.hal.Core').CoreProxy != null) {
            coreDelegate = xdc.module('ti.sysbios.hal.Core').CoreProxy.delegate$;
        }
        else {
            var coreDelegateString = Settings.getDefaultCoreDelegate();
            if (coreDelegateString == null) {
                coreDelegate = xdc.module('ti.sysbios.hal.CoreNull');
            }
            else {
                coreDelegate = xdc.module(coreDelegateString);
            }
        }

        var coreDelegateName = coreDelegate.$name;
        coreDelegateName = coreDelegateName.replace(/\./g, "_");

        defs += " -Dti_sysbios_hal_Core_delegate_getId="
                + coreDelegateName
                + "_getId__E";
        defs += " -Dti_sysbios_hal_Core_delegate_interruptCore="
                + coreDelegateName
                + "_interruptCore__E";
        defs += " -Dti_sysbios_hal_Core_delegate_lock="
                + coreDelegateName
             + "_lock__E";
        defs += " -Dti_sysbios_hal_Core_delegate_unlock="
                + coreDelegateName
             + "_unlock__E";

        defs += " -Dti_sysbios_hal_Core_numCores__D=" + coreDelegate.numCores;

        defs += " -D" + coreDelegateName + "_numCores__D=" + coreDelegate.numCores;

        /*
         * add Load module #defines
         */
        defs += " -Dti_sysbios_utils_Load_taskEnabled__D=" + (Load.taskEnabled ? "TRUE" : "FALSE");
        defs += " -Dti_sysbios_utils_Load_swiEnabled__D=" + (Load.swiEnabled ? "TRUE" : "FALSE");
    }

    if (Hwi.dispatcherSwiSupport) {
        defs += " -D"+HwiDelegateName+"_dispatcherSwiSupport__D=TRUE";
    }
    else {
        defs += " -D"+HwiDelegateName+"_dispatcherSwiSupport__D=FALSE";
    }

    if (Hwi.dispatcherTaskSupport) {
        defs += " -D"+HwiDelegateName+"_dispatcherTaskSupport__D=TRUE";
    }
    else {
        defs += " -D"+HwiDelegateName+"_dispatcherTaskSupport__D=FALSE";
    }

    if (Hwi.dispatcherAutoNestingSupport) {
        defs += " -D"+HwiDelegateName+"_dispatcherAutoNestingSupport__D=TRUE";
    }
    else {
        defs += " -D"+HwiDelegateName+"_dispatcherAutoNestingSupport__D=FALSE";
    }

    if (Hwi.dispatcherIrpTrackingSupport) {
        defs += " -D"+HwiDelegateName+"_dispatcherIrpTrackingSupport__D=TRUE";
    }
    else {
        defs += " -D"+HwiDelegateName+"_dispatcherIrpTrackingSupport__D=FALSE";
    }

    if (Semaphore.supportsEvents) {
        defs += " -Dti_sysbios_knl_Semaphore_supportsEvents__D=TRUE";
    }
    else {
        defs += " -Dti_sysbios_knl_Semaphore_supportsEvents__D=FALSE";
    }

    if ((BIOS.assertsEnabled == false) ||
        ((Defaults.common$.diags_ASSERT == Diags.ALWAYS_OFF)
            && (Defaults.common$.diags_INTERNAL == Diags.ALWAYS_OFF))) {
        defs += " -Dxdc_runtime_Assert_DISABLE_ALL";
    }

    if (BIOS.logsEnabled == false) {
        defs += " -Dxdc_runtime_Log_DISABLE_ALL";
    }

    switch (Program.build.target.$name) {
        case "ti.targets.C64P" :
        case "ti.targets.C64P_big_endian" :
        case "ti.targets.C674" :
        case "ti.targets.elf.C64P" :
        case "ti.targets.elf.C64P_big_endian" :
        case "ti.targets.elf.C674" :
        case "ti.targets.elf.C64T" :
        case "ti.targets.elf.C66"  :
        case "ti.targets.elf.C66_big_endian"  :
        case "ti.targets.elf.C67P" :
            if (BIOS.buildingAppLib == true) {
                var Exception = xdc.module("ti.sysbios.family.c64p.Exception");
                defs +=
                 " -Dti_sysbios_family_c64p_Exception_enableExternalMPC__D="
                 + (Exception.enableExternalMPC ? "TRUE" : "FALSE");
                defs +=
                 " -Dti_sysbios_family_c64p_Exception_enablePrint__D="
                 + (Exception.enablePrint ? "TRUE" : "FALSE");
            }
            break;

        case "gnu.targets.arm.M3" :
        case "gnu.targets.arm.M4" :
        case "gnu.targets.arm.M4F" :
        case "iar.targets.arm.M3" :
        case "iar.targets.arm.M4" :
        case "iar.targets.arm.M4F" :
        case "ti.targets.arm.elf.M3" :
        case "ti.targets.arm.elf.M4" :
        case "ti.targets.arm.elf.M4F" :
            var m3Hwi = xdc.module("ti.sysbios.family.arm.m3.Hwi");

            defs +=
             " -Dti_sysbios_family_arm_m3_Hwi_enableException__D="
             + (m3Hwi.enableException ? "TRUE" : "FALSE");

            if (BIOS.buildingAppLib == true) {
                defs +=
                 " -Dti_sysbios_family_arm_m3_Hwi_disablePriority__D="
                 + m3Hwi.disablePriority + "U";

                var GateDualCore =
                    xdc.module("ti.sysbios.family.arm.ducati.GateDualCore");

                if (GateDualCore.$used) {
                defs +=
                 " -Dti_sysbios_family_arm_ducati_GateDualCore_enableStats__D="
                 + (GateDualCore.enableStats ? "TRUE" : "FALSE");
                }

                var GateSmp =
                    xdc.module("ti.sysbios.family.arm.ducati.GateSmp");

                if (GateSmp.$used) {
                defs +=
                 " -Dti_sysbios_family_arm_ducati_GateSmp_enableStats__D="
                 + (GateSmp.enableStats ? "TRUE" : "FALSE");
                }
            }
            break;
        case "ti.targets.arm.elf.Arm9" :
        case "ti.targets.arm.elf.A8" :
        case "ti.targets.arm.elf.A8F" :
        case "ti.targets.arm.elf.A8Fnv" :
        case "gnu.targets.arm.A8F" :
        case "gnu.targets.arm.A9F" :
        case "gnu.targets.arm.A15F" :
            var Exception = xdc.module("ti.sysbios.family.arm.exc.Exception");
            defs +=
             " -Dti_sysbios_family_arm_exc_Exception_enableDecode__D="
             + (Exception.enableDecode ? "TRUE" : "FALSE");
            break;
        case "ti.targets.C28_large"  :
        case "ti.targets.C28_float"  :
            var c28Hwi = xdc.module("ti.sysbios.family.c28.Hwi");
            defs +=
             " -Dti_sysbios_family_c28_Hwi_zeroLatencyIERMask__D="
             + c28Hwi.zeroLatencyIERMask + "U";
            break;
    }

    return (defs);
}

/*
 *  ======== getCFiles ========
 */
function getCFiles(target)
{
    var biosSources ="";
    var xdcSources = "";

    /*
     * logic to trim the C files down to just what the application needs
     * 3/2/11 disabled for now ...
     */

    if (BIOS.buildingAppLib == true) {
        biosSources += "BIOS.c ";
        for each (var mod in Program.targetModules()) {
            var mn = mod.$name;
            var pn = mn.substring(0, mn.lastIndexOf("."));
            var packageMatch = false;

            /* sanity check package path */
            for (var i = 0; i < biosPackages.length; i++) {
                if (pn == biosPackages[i]) {
                    packageMatch = true;
                    break;
                }
            }

            if (packageMatch && !mn.match(/Proxy/) && !mn.match(/BIOS/)) {
                if (cFiles[mn] === undefined) {
                    var prefix = mn.substr(mn.indexOf("sysbios") + 8);
                    var mod = mn.substr(mn.lastIndexOf(".")+1);
                    prefix = prefix.substring(0, prefix.lastIndexOf('.')+1);
                    prefix = prefix.replace(/\./g, "/");
                    biosSources += prefix + mod + ".c ";
                }
                else {
                    for (i in cFiles[mn].cSources) {
                        var prefix = mn.substr(mn.indexOf("sysbios") + 8);
                        prefix = prefix.substring(0, prefix.lastIndexOf('.')+1);
                        prefix = prefix.replace(/\./g, "/");
                        biosSources += prefix + cFiles[mn].cSources[i] + " ";
                    }
                }
            }
        }
        if (Program.build.target.name.match(/430/)) {
            biosSources += "./HwiFuncs.c ";
        }
    }
    else {
        /* build with ALL sysbios C sources */
        biosSources = cList[target];

        if (BIOS.smpEnabled == true) {
            biosSources += smpSources;
        }
    }

    if (BIOS.includeXdcRuntime == true) {
        if (BIOS.buildingAppLib == true) {
            biosSources += "xdc/runtime/xdc_noinit.c ";
            for each (var mod in Program.targetModules()) {
                var mn = mod.$name;
                if (mn.match(/xdc\.runtime/) && !mn.match(/Proxy/)) {
                    if (cFiles[mn] === undefined) {
                        var prefix = mn.substr(mn.indexOf("xdc"));
                        var mod = mn.substr(mn.lastIndexOf(".")+1);
                        prefix = prefix.substring(0, prefix.lastIndexOf('.')+1);
                        prefix = prefix.replace(/\./g, "/");
                        biosSources += prefix + mod + ".c ";
                    }
                    else {
                        for (i in cFiles[mn].cSources) {
                            var prefix = mn.substr(mn.indexOf("xdc"));
                            prefix = prefix.substring(0, prefix.lastIndexOf('.')+1);
                            prefix = prefix.replace(/\./g, "/");
                            biosSources += prefix + cFiles[mn].cSources[i] + " ";
                        }
                    }
                }
            }
        }
        else {
            var RtsBuild = xdc.loadCapsule('xdc/runtime/build.xs');
            var KnlBuild = xdc.loadCapsule('xdc/runtime/knl/build.xs');

            var xdcObjs = RtsBuild.c_objs.concat(KnlBuild.c_objs);
            for each (var s in xdcObjs) {
                biosSources += s + " ";
            }
        }
    }

    /* remove trailing " " */
    biosSources = biosSources.substring(0, biosSources.length-1);

    return (biosSources);
}

/*
 *  ======== getAsmFiles ========
 */
function getAsmFiles(target)
{
    return (asmList[target]);
}

/*
 *  ======== getLibs ========
 */
function getLibs(pkg)
{
    var BIOS = xdc.module("ti.sysbios.BIOS");

    switch (BIOS.libType) {
        case BIOS.LibType_Custom:
        case BIOS.LibType_Instrumented:
        case BIOS.LibType_NonInstrumented:
        case BIOS.LibType_Debug:
            return null;
            break;
    }

    var lib = "";
    var name = pkg.$name + ".a" + prog.build.target.suffix;

    if (BIOS.smpEnabled == true) {
        lib = "lib/smpbios/debug/" + name;
    }
    else {
        lib = "lib/sysbios/debug/" + name;
    }

    if (java.io.File(pkg.packageBase + lib).exists()) {
        return lib;
    }

    /* could not find any library, throw exception */
    throw Error("Library not found: " + name);
}


/*
 *  ======== getProfiles ========
 *  Determines which profiles to build for.
 *
 *  Any argument in XDCARGS which does not contain platform= is treated
 *  as a profile. This way multiple build profiles can be specified by
 *  separating them with a space.
 */
function getProfiles(xdcArgs)
{
    /*
     * cmdlProf[1] gets matched to "whole_program,debug" if
     * ["abc", "profile=whole_program,debug"] is passed in as xdcArgs
     */
    var cmdlProf = (" " + xdcArgs.join(" ") + " ").match(/ profile=([^ ]+) /);

    if (cmdlProf == null) {
        /* No profile=XYZ found */
        return [];
    }

    /* Split "whole_program,debug" into ["whole_program", "debug"] */
    var profiles = cmdlProf[1].split(',');

    return profiles;
}

/*
 *  ======== buildLibs ========
 *  This function generates the makefile goals for the libraries
 *  produced by a ti.sysbios package.
 */
function buildLibs(objList, relList, filter, xdcArgs)
{
    for (var i = 0; i < xdc.module('xdc.bld.BuildEnvironment').targets.length; i++) {
        var targ = xdc.module('xdc.bld.BuildEnvironment').targets[i];

        /* skip target if not supported */
        if (!supportsTarget(targ, filter)) {
            continue;
        }

        var profiles = getProfiles(xdcArgs);

        /* If no profiles were assigned, use only the default one. */
        if (profiles.length == 0) {
            profiles[0] = "debug";
        }

        for (var j = 0; j < profiles.length; j++) {
            var ccopts = "";
            var asmopts = "";

            if (profiles[j] == "smp") {
                var libPath = "lib/smpbios/debug/";
                ccopts += " -Dti_sysbios_BIOS_smpEnabled__D=TRUE";
                asmopts += " -Dti_sysbios_BIOS_smpEnabled__D=TRUE";
            }
            else {
                var libPath = "lib/sysbios/debug/";
                /* build all package libs using Hwi macros */
                ccopts += " -Dti_sysbios_Build_useHwiMacros";
                ccopts += " -Dti_sysbios_BIOS_smpEnabled__D=FALSE";
                asmopts += " -Dti_sysbios_BIOS_smpEnabled__D=FALSE";
            }

            /* confirm that this target supports this profile */
            if (targ.profiles[profiles[j]] !== undefined) {
                var profile = profiles[j];
                var lib = Pkg.addLibrary(libPath + Pkg.name,
                                targ, {
                                profile: profile,
                                copts: ccopts,
                                aopts: asmopts,
                                releases: relList,
                                });
                lib.addObjects(objList);
                /* suppress debug libs from exports */
                Pkg.attrs.relScript = "ti/sysbios/libFilter.xs";
            }
        }
    }
}


/*
 *  ======== supportsTarget ========
 *  Returns true if target is in the filter object. If filter
 *  is null or empty, that's taken to mean all targets are supported.
 */
function supportsTarget(target, filter)
{
    var list, field;

    if (filter == null) {
        return true;
    }

    /*
     * For backwards compatibility, we support filter as an array of
     * target names.  The preferred approach is to specify filter as
     * an object with 'field' and 'list' elements.
     *
     * Old form:
     *     var trgFilter = [ "Arm9", "Arm9t", "Arm9t_big_endian" ]
     *
     * New (preferred) form:
     *
     *     var trgFilter = {
     *         field: "isa",
     *         list: [ "v5T", "v7R" ]
     *     };
     *
     */
    if (filter instanceof Array) {
        list = filter;
        field = "name";
    }
    else {
        list = filter.list;
        field = filter.field;
    }

    if (list == null || field == null) {
        throw("invalid filter parameter, must specify list and field!");
    }

    if (field == "noIsa") {
        if (String(','+list.toString()+',').match(','+target["isa"]+',')) {
            return (false);
        }
        return (true);
    }

    /*
     * add ',' at front and and tail of list and field strings to allow
     * use of simple match API.  For example, the string is updated to:
     * ',v5T,v7R,' to allow match of ',v5t,'.
     */
    if (String(','+list.toString()+',').match(','+target[field]+',')) {
        return (true);
    }

    return (false);
}

