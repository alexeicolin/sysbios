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
 *  ======== PIE.xs ========
 *
 */


/*
 *  ======== module$meta$init ========
 *  Create pre-created objects from BIOS 5.x
 */
function module$meta$init(name)
{
    /* Only process during "cfg" phase */
    if (xdc.om.$name != "cfg") {
        return;
    }

    var HWI = xdc.module("ti.bios.HWI");
    var isa = Program.build.target.isa;
    var pie;
    if (isa == "28" || isa == "28FP") {

        /* PIE Interrupts */
        this.create("PIE_INT1_1");
        this.create("PIE_INT1_2");
        this.create("PIE_INT1_3");
        this.create("PIE_INT1_4");
        this.create("PIE_INT1_5");
        this.create("PIE_INT1_6");
        this.create("PIE_INT1_7");
        this.create("PIE_INT1_8");

        this.create("PIE_INT2_1");
        this.create("PIE_INT2_2");
        this.create("PIE_INT2_3");
        this.create("PIE_INT2_4");
        this.create("PIE_INT2_5");
        this.create("PIE_INT2_6");
        this.create("PIE_INT2_7");
        this.create("PIE_INT2_8");

        this.create("PIE_INT3_1");
        this.create("PIE_INT3_2");
        this.create("PIE_INT3_3");
        this.create("PIE_INT3_4");
        this.create("PIE_INT3_5");
        this.create("PIE_INT3_6");
        this.create("PIE_INT3_7");
        this.create("PIE_INT3_8");

        this.create("PIE_INT4_1");
        this.create("PIE_INT4_2");
        this.create("PIE_INT4_3");
        this.create("PIE_INT4_4");
        this.create("PIE_INT4_5");
        this.create("PIE_INT4_6");
        this.create("PIE_INT4_7");
        this.create("PIE_INT4_8");

        this.create("PIE_INT5_1");
        this.create("PIE_INT5_2");
        this.create("PIE_INT5_3");
        this.create("PIE_INT5_4");
        this.create("PIE_INT5_5");
        this.create("PIE_INT5_6");
        this.create("PIE_INT5_7");
        this.create("PIE_INT5_8");

        this.create("PIE_INT6_1");
        this.create("PIE_INT6_2");
        this.create("PIE_INT6_3");
        this.create("PIE_INT6_4");
        this.create("PIE_INT6_5");
        this.create("PIE_INT6_6");
        this.create("PIE_INT6_7");
        this.create("PIE_INT6_8");

        this.create("PIE_INT7_1");
        this.create("PIE_INT7_2");
        this.create("PIE_INT7_3");
        this.create("PIE_INT7_4");
        this.create("PIE_INT7_5");
        this.create("PIE_INT7_6");
        this.create("PIE_INT7_7");
        this.create("PIE_INT7_8");

        this.create("PIE_INT8_1");
        this.create("PIE_INT8_2");
        this.create("PIE_INT8_3");
        this.create("PIE_INT8_4");
        this.create("PIE_INT8_5");
        this.create("PIE_INT8_6");
        this.create("PIE_INT8_7");
        this.create("PIE_INT8_8");

        this.create("PIE_INT9_1");
        this.create("PIE_INT9_2");
        this.create("PIE_INT9_3");
        this.create("PIE_INT9_4");
        this.create("PIE_INT9_5");
        this.create("PIE_INT9_6");
        this.create("PIE_INT9_7");
        this.create("PIE_INT9_8");

        this.create("PIE_INT10_1");
        this.create("PIE_INT10_2");
        this.create("PIE_INT10_3");
        this.create("PIE_INT10_4");
        this.create("PIE_INT10_5");
        this.create("PIE_INT10_6");
        this.create("PIE_INT10_7");
        this.create("PIE_INT10_8");

        this.create("PIE_INT11_1");
        this.create("PIE_INT11_2");
        this.create("PIE_INT11_3");
        this.create("PIE_INT11_4");
        this.create("PIE_INT11_5");
        this.create("PIE_INT11_6");
        this.create("PIE_INT11_7");
        this.create("PIE_INT11_8");

        this.create("PIE_INT12_1");
        this.create("PIE_INT12_2");
        this.create("PIE_INT12_3");
        this.create("PIE_INT12_4");
        this.create("PIE_INT12_5");
        this.create("PIE_INT12_6");
        this.create("PIE_INT12_7");
        this.create("PIE_INT12_8");
    }
}

/*
 *  ======== instance$meta$init ========
 *  Initializes instance name to the name parameter.
 */
function instance$meta$init(name)
{
    this.name = name;
}

/*
 *  ======== close ========
 *  Use the corresponding Hwi module and create an instance of it for each
 *  instance of HWI.
 */
function close()
{
    if ((Program.build.target.isa != "28") &&
            (Program.build.target.isa != "28FP")) {
        this.$logWarning("PIE module only supported for C28X!");
        return;
    }

    /*
     *  The c28xx.cdb file translates the values of VMAPMODE and PIESELECT.
     *  However, the family.c28.Hwi module function Hwi_Module_startup() is
     *  coded to always set ENPIE to true (always enables the PIE).  It also
     *  contains no code to change the VMAP mode (bit 3 of the DEVICECNF
     *  register).
     *
     *  So, these legacy config params are not used.
     */
    if (this.VMAPMODE != null) {
        this.$logWarning("'VMAPMODE' not supported.  SYS/BIOS uses the" +
               " default value of VMAP.", this);
    }

    if (this.PIESELECT != null) {
        this.$logWarning("'PIESELECT' not supported.  The PIE is always" +
               " enabled for the C2000 in SYS/BIOS.", this);
    }


    var insts = this.$instances;

    /* keep track of interrupt IDs.  PIE interrupts start at 32 */
    var intId = 32;

    var Settings = xdc.module('ti.sysbios.family.Settings');
    var Hwi = xdc.module(Settings.getDefaultHwiDelegate());

    /* loop through instances of PIE */
    for (var i = 0; i < insts.length; i++, intId++) {

        if (insts[i].fxn == "HWI_unused") {
            /* don't create Hwi instances for unused PIE interrupts */
            continue;
        }
        else {
            var params = new Hwi.Params();
            params.arg = insts[i].arg;

            /* log Warnings for unsupported properties that are modified */
            if (insts[i].monitor != null) {
                this.$logWarning("'monitor' not supported", insts[i]);
            }
            
            if (insts[i].addr != null) {
                this.$logWarning("'addr' not supported", insts[i]);
            }
            
            if (insts[i].dataType != null) {
                this.$logWarning("'dataType' not supported", insts[i]);
            }
            
            if (insts[i].operation != null) {
                this.$logWarning("'operation' not supported", insts[i]);
            }

            /* modify params fields */
            if (insts[i].interruptMask0 == "none") {
                params.maskSetting = Hwi.MaskingOption_NONE;
            }
            else if (insts[i].interruptMask0 == "all") {
                params.maskSetting = Hwi.MaskingOption_ALL;
            }
            else if (insts[i].interruptMask0 == "bitmask") {
                params.maskSetting = Hwi.MaskingOption_BITMASK;
                params.disableMask = insts[i].interruptBitMask;
                params.restoreMask = insts[i].interruptBitMask;
            }
            else {
                params.maskSetting = Hwi.MaskingOption_SELF;
            }

            /* 
             * BIOS 5 applications do not expect the HWI dispatcher to ACK
             * C28 PIE interrupts.  BIOS 6 applications, however, do expect
             * this functionality
             */
            params.enableAck = false;

            if (!(insts[i].useDispatcher)) {
                /* if not using dispatcher, just plug the function */
                Hwi.plugMeta(intId, "&" + insts[i].fxn);
            }
            else {
                /* if using dispatcher, create a Hwi instance */

                 /* We create a wrapper of the type Hwi_FuncPtr because the 
                 * supplied function is most likely not of that type (CQ19651).
                 * We don't do that for interrupts that don't use dispatcher
                 * because they are generated through the template.
                 */
                var fxn = insts[i].fxn;
                if (internal.wrapperFunc(fxn)) {
                    if (internal.wrapperFunc(fxn).args > 1) {
                        this.$logError("The function " + fxn + " cannot be " +
                            "assigned to a HWI instance fxn parameter.",
                             insts[i]);        
                    }
                    Hwi.create(intId, "&" + insts[i].fxn + "__hwiFxn", params);
                }
                else {
                    Hwi.create(intId, "&" + insts[i].fxn, params);
                }
            }
        }
    }
}

/*
 *  ======== exportMod ========
 */
function exportMod()
{
    /* export the hal.Hwi for ATM and possibly others who are using it */
    Program.exportModule('ti.sysbios.hal.Hwi');

    if (Program.build.target.isa == "28" ||
            Program.build.target.isa == "28FP") {
            Program.exportModule('ti.sysbios.family.c28.Hwi');
    }
}
