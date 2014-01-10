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
 *  ======== dev_crea.c ========
 */

#include <ti/bios/include/std.h>
#include <ti/bios/include/dev.h>
#include <ti/bios/include/_dev.h>
#include <ti/bios/include/iom.h>
#include <ti/bios/include/que.h>
#include <ti/bios/include/sys.h>

/*
 *  ======== DEV_init ========
 *  This function is called from DEV_init macro as part of BIOS_init sequence.
 */
Void DEV_init(Void)
{
    DEV_TableElem *objDevHead;
    DEV_TableElem *objDev;
    DEV_Device *dptr;
    IOM_Fxns *fxns;
    Int status;
    Int i;

    /* Hook up the linked list ... */
    for (i = 0; i < _DEV_numStaticDevs; i++) {
        QUE_put(&DEV_table, &(_DEV_staticDevTable[i]));
    }

    /*
     *  For each device driver, call its DXX_init function *before* the
     *  statically created streams are "opened" (calling the device's
     *  open function for each static stream below).
     */
    for (i = (Int) _DEV_numStaticDevs - 1; i >= 0; i--) {
        if (_DEV_initFxn[i] != NULL) {
           (_DEV_initFxn[i])();
        }
    }

    /*
     *  Call IOM bind device function (mdBindDev) if driver is of type IOM
     */
    objDevHead = (DEV_TableElem *)&DEV_table;
    for(objDev = (DEV_TableElem *) QUE_next((Ptr)objDevHead);
                objDev != objDevHead;
                objDev = (DEV_TableElem *) QUE_next((Ptr)objDev)) {

        dptr = &objDev->device;

        if (dptr->type == DEV_IOMTYPE ) {
            fxns = (IOM_Fxns *)dptr->fxns;
            status = fxns->mdBindDev(&dptr->devp, dptr->devid, dptr->params);

            if (status != IOM_COMPLETED) {
                SYS_abort("ERROR - Device %s Config Failed", dptr->name);
            }
        }
    }
}



