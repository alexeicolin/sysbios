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
 *  ======== dev.c ========
 */

#include <ti/bios/include/std.h>
#include <ti/bios/include/dev.h>
#include <ti/bios/include/_dev.h>
#include <ti/bios/include/hwi.h>
#include <ti/bios/include/iom.h>
#include <ti/bios/include/obj.h>
#include <ti/bios/include/que.h>
#include <ti/bios/include/sys.h>
#include <ti/bios/include/mem.h>
#include <ti/bios/include/tsk.h>

#include <string.h>

/*
 * DEV_createDevice creates device(DEV_Device) entry in the OBJ table
 * if device by that name do not exist in the system.
 * This API is not reentrant
 */
Int DEV_createDevice (String name, Void *fxns,   Fxn initFxn,
        DEV_Attrs *attrs)
{
    DEV_TableElem *objDevHead = (DEV_TableElem*) &DEV_table;
    DEV_TableElem *objDev, *objEntry;
    DEV_Device *dptr, *entry;
    IOM_Fxns *iomfxns;
    Int status;
    Uns key;

    /*
     * Crate a device entry, if not successful return
     * SYS_EALLOC. 
     */

    objEntry = MEM_calloc(0, sizeof(DEV_TableElem), 0);

    if (objEntry == NULL) {
        return(SYS_EALLOC);
    }

    TSK_disable();


    /*
     * Check if device already exists in the Device table, if yes return
     * SYS_EINVAL
     */
    DEV_find(name, &entry);

    if (entry != NULL) {
        TSK_enable();
        MEM_free(0, objEntry, sizeof(DEV_TableElem));
        SYS_error("DEV", SYS_EINVAL);
        return(SYS_EINVAL);
    }

    /*
     * Initialize new device entry(DEV_Device) in the OBJ table with
     * the parameters passed to API
     */
    entry = &objEntry->device;
    entry->name = name;
    entry->fxns = fxns;

    if (attrs == NULL) {
        attrs = &DEV_ATTRS;
    }
    entry->devid  = attrs->devid;
    entry->params = attrs->params;
    entry->type   = attrs->type;
    entry->devp   = attrs->devp;

    /*
     * Call the Device init function if its not NULL, with interrupts
     * disabled.
     */
    if (initFxn != NULL) {
        key = HWI_disable();
        (*initFxn)();
        HWI_restore(key);
    }

    /*
     * If device created is of type IOM then call mini driver function
     * mdBindDev with interrupts disabled.
     */
    if (entry->type == DEV_IOMTYPE) {
        iomfxns = (IOM_Fxns *) entry->fxns;

        key = HWI_disable();
        status = iomfxns->mdBindDev(&entry->devp, entry->devid,
                                     entry->params);
        HWI_restore(key);

        if (status != IOM_COMPLETED) {
            
            TSK_enable();

            /* Delete the just created device entry in device table */
            MEM_free(0, objEntry, sizeof(DEV_TableElem));

            SYS_error("DEV", SYS_EBADIO);

            return(status);
        }

    }

    /*
     * Device is ready for addition into OBJ_Table. Check new device
     * name length against existing device name lengths. If length of
     * new device is greater than one in OBJ_table, mark the location
     * and insert device ahead of device whose name length is shorter
     * else add it to the end.
     *
     * This will keep all the devices sorted in descending order, which is
     * required to pass additional parameters along with device name in 
     * DEV_open()
     */

    objDev = (DEV_TableElem *)QUE_next((Ptr)objDevHead);
    while (objDev != objDevHead) {
        dptr = &objDev->device;
        if (strlen(name) > strlen(dptr->name)) {
            break;
        }
        objDev = (DEV_TableElem *)QUE_next((Ptr)objDev);
    }

    /* Insert objEntry ahead of objDev */
    QUE_insert(objDev, objEntry);

    TSK_enable();

    return(SYS_OK);
}

/*
 * DEV_deleteDevice deletes device(DEV_Device) entry in the OBJ table
 * if device by that name exist in the system.
 * This API is not reentrant
 */
Int DEV_deleteDevice (String name)
{
    DEV_TableElem *objDev;
    DEV_Device *entry;
    IOM_Fxns *iomfxns;
    Int status = SYS_OK;
    Uns key;

    /* Check if device   exists in the Device table, if not return FALSE */
    DEV_find(name, &entry);
    if (entry == NULL) {
        SYS_error("DEV", SYS_ENODEV);
        return(SYS_ENODEV);
    }

    /*
     * If device to be deleted is of type IOM call mdUnBindDev with
     * interrupts disabled
     */
    if (entry->type == DEV_IOMTYPE) {
        iomfxns = (IOM_Fxns *)entry->fxns;

        key = HWI_disable();
        status = iomfxns->mdUnBindDev(entry->devp);
        HWI_restore(key);

        if (status != IOM_COMPLETED) {
            SYS_error("DEV", SYS_EBADIO);
        }
        else {
            status = SYS_OK;
        }

    }

    /* Free Device entry in the device table */
    objDev = (DEV_TableElem *)((char *)entry - sizeof(QUE_Elem));
    QUE_remove(objDev);
    MEM_free(0, objDev, sizeof(DEV_TableElem));


    return(status);
}

/*
 * ======== DEV_ebadio ========
 */
Int DEV_ebadio(DEV_Handle device)
{
    SYS_error("DEV", SYS_EBADIO);

    return SYS_EBADIO;
}

/*
 *  ======== DEV_find ========
 */
Void DEV_find(String name, DEV_Device **driver)
{
    DEV_TableElem *objDevHead = (DEV_TableElem*) &DEV_table;
    DEV_TableElem *objDev;
    DEV_Device *dptr;

    /*
     * Do the exact match, return device entry if successfull.
     */

    for (objDev = (DEV_TableElem *)QUE_next((Ptr)objDevHead); objDev != objDevHead;
        objDev = (DEV_TableElem *)QUE_next((Ptr)objDev)) {
        dptr = &objDev->device;
        if ( strcmp(name,dptr->name) == 0 ) {
            /* If driver exists in the devicetable, point the *driver
               to corresponding device entry */
            *driver = dptr;
            return;
        }
    }

    *driver = NULL;
    return;
}

/*
 *  ======== DEV_match ========
 */
String DEV_match(String name, DEV_Device **driver)
{
    DEV_TableElem *objDevHead = (DEV_TableElem*) &DEV_table;
    DEV_TableElem *objDev;
    DEV_Device *dptr;
    Int len;

    /*
     * Trace the existence of device through OBJ_table[OBJ_DEV].
     * If successfull *dptr points to the device entry.
     */

    for (objDev = (DEV_TableElem *)QUE_next((Ptr)objDevHead); objDev != objDevHead;
        objDev = (DEV_TableElem *)QUE_next((Ptr)objDev)) {
        dptr = &objDev->device;
        len = strlen(dptr->name);
        if ( (len == 0) || (strncmp(name,dptr->name,len) == 0) ) {
            /* If driver exists in the devicetable, point the *driver
               to corresponding device entry */
            *driver = dptr;
            return(name + len);
        }
    }

    *driver = NULL;
    return (name);

}

/*
 *  ======== DEV_mkframe ========
 */
DEV_Frame *DEV_mkframe(Int segid, Uns size, Uns align)
{
    DEV_Frame   *frame;

    if ((frame = MEM_alloc(0, sizeof(DEV_Frame), 0)) == MEM_ILLEGAL) {
        return (NULL);
    }

    /* don't allocate frame buffer if size is zero */
    if (size > 0) {
        if ((frame->addr = MEM_alloc(segid, size, align)) == MEM_ILLEGAL) {
            MEM_free(0, frame, sizeof(DEV_Frame));
            return (NULL);
        }
    }

    frame->size = size;

    return (frame);
}

/*
 *  ======== DEV_rmframe ========
 */
Void DEV_rmframe(DEV_Frame *frame, Int segid, Uns size)
{
    if (size > 0) {
        /* free buffer */
        MEM_free(segid, frame->addr, size);
    }

    /* free object */
    MEM_free(0, frame, sizeof(DEV_Frame));
}



