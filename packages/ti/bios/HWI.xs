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
 *  ======== HWI.xs ========
 *
 */

var intSourceToEventId = {
    ARINT0: 29,
    AXINT0: 28,
    ARINT1: 25,
    AXINT1: 24,
    DSPINT: 0,
    EDMA_Controller: 8,
    EDMA_Interrupt: 8,
    EMAC_MDIO_INT: 24,
    EMIF_SDRAM_Timer: 3,
    EMIFA_SDRAM_Timer: 3,
    EMIFB_SDRAM_Timer: 20,
    EMU_DTDMA: 9,
    EMU_RTDX_RX: 10,
    EMU_RTDX_TX: 11,
    External_Pin_4: 4,
    External_Pin_5: 5,
    External_Pin_6: 6,
    External_Pin_7: 7,
    GPIO_Interrupt_0: 16,
    GPIO_Interrupt_4: 4,
    GPIO_Interrupt_5: 5,
    GPIO_Interrupt_6: 6,
    GPIO_Interrupt_7: 7,
    Host_Port_Host_to_DSP: 0,
    HPI_PCI_to_DSP: 0,
    ICINT0: 22,
    ICINT1: 23,
    JTAGRTDX9: 9,
    MCSP_0_Receive: 13,
    MCSP_0_Transmit: 12,
    MCSP_1_Receive: 15,
    MCSP_1_Transmit: 14,
    MCSP_2_Receive: 18,
    MCSP_2_Transmit: 17,
    PCI_WAKEUP: 21,
    Timer_0: 1,
    Timer_1: 2,
    Timer_2: 19,
    UTOPIA_Interrupt: 23,
    VPINT0: 25,
    VPINT1: 26,
    VPINT2: 27
}


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

    var temp;
    if (Program.build.target.isa == "64P" ||
        Program.build.target.isa == "66"  ||
        Program.build.target.isa == "67P" ||
        Program.build.target.isa == "674") {
        this.create("HWI_RESET");
        this.create("HWI_NMI");
        this.create("HWI_RESERVED0");
        this.create("HWI_RESERVED1");

        temp = this.create("HWI_INT4");
        temp.interruptSource = "External_Pin_4";

        temp = this.create("HWI_INT5");
        temp.interruptSource = "External_Pin_5";

        temp = this.create("HWI_INT6");
        temp.interruptSource = "External_Pin_6";

        temp = this.create("HWI_INT7");
        temp.interruptSource = "External_Pin_7";

        temp = this.create("HWI_INT8");
        temp.interruptSource = "EDMA_Controller";

        temp = this.create("HWI_INT9");
        temp.interruptSource = "JTAGRTDX9";

        temp = this.create("HWI_INT10");
        temp.interruptSource = "Timer_2";

        temp = this.create("HWI_INT11");
        temp.interruptSource = "MCSP_0_Receive";

        temp = this.create("HWI_INT12");
        temp.interruptSource = "MCSP_1_Transmit";

        temp = this.create("HWI_INT13");
        temp.interruptSource = "Host_Port_Host_to_DSP";

        temp = this.create("HWI_INT14");
        temp.interruptSource = "Timer_0";

        temp = this.create("HWI_INT15");
        temp.interruptSource = "Timer_1";
    }
    else if (Program.build.target.isa == "64T") {
        this.create("HWI_RESET");
        this.create("HWI_NMI");
        this.create("HWI_RESERVED0");
        this.create("HWI_RESERVED1");

        /* GPTIMER 7 */
        temp = this.create("HWI_INT4");
        temp.interruptSource = "Timer_7";

        /* GPTIMER 8 */
        temp = this.create("HWI_INT5");
        temp.interruptSource = "Timer_8";

        temp = this.create("HWI_INT6");
        temp.interruptSource = "External_Pin_6";

        temp = this.create("HWI_INT7");
        temp.interruptSource = "External_Pin_7";

        temp = this.create("HWI_INT8");
        temp.interruptSource = "EDMA_Controller";

        temp = this.create("HWI_INT9");
        temp.interruptSource = "JTAGRTDX9";

        temp = this.create("HWI_INT10");
        temp.interruptSource = "External_Pin_10";

        temp = this.create("HWI_INT11");
        temp.interruptSource = "MCSP_0_Receive";

        temp = this.create("HWI_INT12");
        temp.interruptSource = "MCSP_1_Transmit";

        temp = this.create("HWI_INT13");
        temp.interruptSource = "Host_Port_Host_to_DSP";

        /* GPTIMER 5 */
        temp = this.create("HWI_INT14");
        temp.interruptSource = "Timer_5";

        /* GPTIMER 6 */
        temp = this.create("HWI_INT15");
        temp.interruptSource = "Timer_6";
    }
    else if (Program.build.target.isa == "28" || 
             Program.build.target.isa == "28FP") {
        this.create("HWI_RESET");
        this.create("HWI_INT1");
        this.create("HWI_INT2");
        this.create("HWI_INT3");
        this.create("HWI_INT4");
        this.create("HWI_INT5");
        this.create("HWI_INT6");
        this.create("HWI_INT7");
        this.create("HWI_INT8");
        this.create("HWI_INT9");
        this.create("HWI_INT10");
        this.create("HWI_INT11");
        this.create("HWI_INT12");
        this.create("HWI_INT13");
        this.create("HWI_TINT");
        this.create("HWI_DLOG");
        this.create("HWI_RTOS");
        this.create("HWI_RESERVED");
        this.create("HWI_NMI");
        this.create("HWI_ILLEGAL");
        this.create("HWI_USER1");
        this.create("HWI_USER2");
        this.create("HWI_USER3");
        this.create("HWI_USER4");
        this.create("HWI_USER5");
        this.create("HWI_USER6");
        this.create("HWI_USER7");
        this.create("HWI_USER8");
        this.create("HWI_USER9");
        this.create("HWI_USER10");
        this.create("HWI_USER11");
        this.create("HWI_USER12");
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
    var insts = this.$instances;

    var Settings = xdc.module('ti.sysbios.family.Settings');
    var Hwi = xdc.module(Settings.getDefaultHwiDelegate());

    /* loop through instances of HWI */
    for (var i = 0; i < insts.length; i++) {

        var intNum = insts[i].interruptSelectNumber;
        if (Program.build.target.isa == "64P" ||
            Program.build.target.isa == "64T" ||
            Program.build.target.isa == "66"  ||
            Program.build.target.isa == "674") {
            if (intNum >= 0 && intNum < 4) {
                if (insts[i].fxn != "HWI_unused") {
                    this.$logWarning("Plugging combined event with something other than ECM_dispatch() handler", insts[i]);
                }
                else {
                    insts[i].fxn = "ti_sysbios_family_c64p_EventCombiner_dispatch";
                    insts[i].useDispatcher = true;
                    insts[i].arg = intNum;
                }
            }
        }

        if (insts[i].fxn != "HWI_unused") {
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
            if (insts[i].interruptMask == "none") {
                params.maskSetting = Hwi.MaskingOption_NONE;
            }
            else if (insts[i].interruptMask == "all") {
                params.maskSetting = Hwi.MaskingOption_ALL;
            }
            else if (insts[i].interruptMask == "bitmask") {
                params.maskSetting = Hwi.MaskingOption_BITMASK;
                params.disableMask = insts[i].interruptBitMask;
                params.restoreMask = insts[i].interruptBitMask;
            }
            else {
                params.maskSetting = Hwi.MaskingOption_SELF;
            }

            if (Program.build.target.isa == "64") {
                if (insts[i].interruptSource != null) {
                    params.eventId =
                        intSourceToEventId[insts[i].interruptSource];
                }
            }
            
            if (Program.build.target.isa == "64P" ||
                Program.build.target.isa == "64T" ||
                Program.build.target.isa == "66"  ||
                Program.build.target.isa == "67P" ||
                Program.build.target.isa == "674") {
                if (insts[i].interruptSelectNumber != undefined) {
                    params.eventId = insts[i].interruptSelectNumber;
                }
            }
            else if (Program.build.target.isa == "28" ||
                     Program.build.target.isa == "28FP") {
                /* 
                 * BIOS 5 applications do not expect the HWI dispatcher to ACK
                 * C28 PIE interrupts.  BIOS 6 applications, however, do expect
                 * this functionality
                 */
                params.enableAck = false;
            }

            if (!(insts[i].useDispatcher)) {
                /* if not using dispatcher, just plug the function */
                Hwi.plugMeta(i, "&" + insts[i].fxn);
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
                    Hwi.create(i, "&" + insts[i].fxn + "__hwiFxn", params);
                }
                else {
                    Hwi.create(i, "&" + insts[i].fxn, params);
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

    switch (Program.build.target.isa) {
        case "64":
            Program.exportModule('ti.sysbios.family.c64.Hwi');
            break;
        case "64P":
        case "66":
        case "64T":
        case "674":
            Program.exportModule('ti.sysbios.family.c64p.Hwi');
            break;
        case "67P":
            Program.exportModule('ti.sysbios.family.c67p.Hwi');
            break;
        case "28":
        case "28FP":
            Program.exportModule('ti.sysbios.family.c28.Hwi');
            break;
        default:
            this.$logError(Program.build.target.name + " Target not supported.",
                this);
            break;    
    }
}
