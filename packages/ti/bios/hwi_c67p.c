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
 *  ======== hwi_c67p.c ========
 */

#include <xdc/std.h>

#include <ti/sysbios/family/c67p/Hwi.h>

#include <xdc/runtime/Error.h>

#include <ti/bios/include/std.h>
#include <ti/bios/include/hwi.h>


__FAR__ HWI_Attrs HWI_ATTRS = {
    1,                          /* IER bitmask, 1="self" */
    1,                          /* CSR CC bitmask, 1="leave alone" */
    0                           /* function argument */
};

/*
 *  ======== HWI_dispatchPlug ========
 *  Plug the HWI dispatcher table.
 */
Void HWI_dispatchPlug(Int vecid, Fxn fxn, Int dmachan, HWI_Attrs *attrs)
{
    Hwi_Params hwiParams;
    Hwi_Handle hwi;
    Error_Block eb;

    if (attrs == NULL) {
        attrs = &HWI_ATTRS;
    }

    Hwi_Params_init(&hwiParams);
    
    Error_init(&eb);

    if (attrs->intrMask == 1) {
        hwiParams.maskSetting = Hwi_MaskingOption_SELF;
    }
    else {
        hwiParams.maskSetting = Hwi_MaskingOption_BITMASK;
    }
    
    hwiParams.disableMask = hwiParams.restoreMask = attrs->intrMask;

    hwiParams.arg = attrs->arg;
    hwiParams.enableInt = FALSE; 

    hwi = Hwi_getHandle(vecid);
    if (hwi == NULL) {
        Hwi_create(vecid, (Hwi_FuncPtr)fxn, &hwiParams, &eb);
    }
    else {
        Hwi_reconfig(hwi, (Hwi_FuncPtr)fxn, &hwiParams);
    }
}


