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
 *  ======== dnl.c ========
 *  NULL device ala /dev/null
 */

#include <ti/bios/include/std.h>

#include <ti/bios/include/dev.h>
#include <ti/bios/include/que.h>
#include <ti/bios/include/sys.h>

#include <ti/bios/include/dnl.h>

Int DNL_exchange();

/*
 *  Driver function table.
 */
__FAR__ DEV_Fxns DNL_FXNS = {
    DEV_CLOSE,          /* close */
    DEV_CTRL,           /* ctrl */
    DEV_IDLE,           /* idle */
    DNL_exchange,       /* issue */
    DEV_OPEN,           /* open */
    DEV_READY,          /* ready */
    (DEV_Treclaim)SYS_zero      /* reclaim */
};

/*
 *  ======== DNL_exchange ========
 */
Int DNL_exchange(DEV_Handle device)
{
    QUE_put(device->fromdevice, QUE_get(device->todevice));

    return (SYS_OK);
}
