var BIOS = xdc.useModule('ti.sysbios.BIOS');
BIOS.libType = BIOS.LibType_Custom;
BIOS.buildingAppLib = false;
BIOS.assertsEnabled = true;
BIOS.logsEnabled = true;

var SourceDir = xdc.module("xdc.cfg.SourceDir");
SourceDir.verbose = 1;

var Hwi = xdc.useModule('ti.sysbios.hal.Hwi');
Hwi.numHooks = 0;       /* Force Hwi hooks to be disabled */
                        /* works for all but C64T (Wugen) */

var Swi = xdc.useModule('ti.sysbios.knl.Swi');
Swi.hooks.length = 0;   /* Force Swi hooks to be disabled */

/* remove all symbolic debug info */
if (Program.build.target.$name.match(/gnu/)) {
    BIOS.customCCOpts = BIOS.customCCOpts.replace("-g","");
}
else {
    if (!Program.build.target.$name.match(/iar/)) {
        BIOS.customCCOpts = BIOS.customCCOpts.replace("-g","");
        BIOS.customCCOpts = BIOS.customCCOpts.replace("--optimize_with_debug","");
        BIOS.customCCOpts += "--symdebug:none ";
        /* suppress warnings regarding .asmfunc and .endasmfunc */
        BIOS.customCCOpts += "--asm_define\".asmfunc= \" --asm_define\".endasmfunc= \" ";
    }
}
