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
 *  ======== dio_tskStaticOpen.c ========
 *
 *  Split into separate function to resolve issue with unresolved
 *  symbols. The variables DIO_NUMTSKSTATIC and DIO_TSKTABLE do
 *  not get generated if STATICCREATE is false or if STATICCREATE is
 *  true.
 */
 
#include <ti/bios/include/std.h>

#include <ti/bios/include/dev.h>
#include <ti/bios/include/sys.h>
#include <ti/bios/include/que.h>
#include <ti/bios/include/sem.h>
#include <ti/bios/include/mem.h>

#include <ti/bios/include/iom.h>
#include <ti/bios/include/dio.h>


extern far Int DIO_NUMTSKSTATIC;        /* Number of SEM based DIO */
extern far DIO_Obj DIO_TSKTABLE[];      /* Table of SEM based DIO */

static DIO_Handle tskStaticMkPort(DEV_Handle device, String name);

/*
 *  ======== tskStaticMkPort ========
 *  Creates a DIO object and binds the controller.
 */
static DIO_Handle tskStaticMkPort(DEV_Handle device, String name)
{
    DIO_Params *params = (DIO_Params *)device->params;
    DIO_Handle dio;
    DEV_Device  *entry;
    Uns         mode;
    Int         status;
    static __FAR__ Int  numUsed = 0;

    /* supports only the number of statically created SEM based DIO */
    if (numUsed >= DIO_NUMTSKSTATIC) {
        return (NULL);
    }

    /* params should contain name of mini-driver */
    if (params == NULL) {
        return (NULL);
    }
    
    /*
     * check to see that name of mini-driver matches one in the device table
     * and its type is of DEV_IOMTYPE.
     */
    (void)DEV_match(params->name, &entry);
    if (entry == NULL || entry->type != DEV_IOMTYPE) {
        return (NULL);
    }

    /* allocate dio object */
    dio = &DIO_TSKTABLE[numUsed++];

    dio->fxns = (IOM_Fxns *)entry->fxns;

    mode = (device->mode == DEV_INPUT) ? IOM_INPUT : IOM_OUTPUT;

    /* create a channel from the mini-driver */
    status = dio->fxns->mdCreateChan(&dio->chanp, entry->devp, name, mode,
                params->chanParams, DIO_tskCallback, device); 

    if (status != IOM_COMPLETED) {
        return (NULL);
    }

    return (dio);
}

/*
 *  ======== DIO_tskStaticOpen ========
 */
Int DIO_tskStaticOpen(DEV_Handle device, String name)
{
    /* allocates DIO_Obj and creates mini-driver channel */
    if ((device->object = (Ptr)tskStaticMkPort(device, name)) != NULL) {
        return (SYS_OK);
    }
    else {
        return (SYS_EBADIO);
    }
} 


