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
 *  ======== mbx.h ========
 *
 *
 */

#ifndef MBX_
#define MBX_

#ifdef __cplusplus
extern "C" {
#endif

#define ti_sysbios_knl_Mailbox__nolocalnames
#include <ti/sysbios/knl/Mailbox.h>

#include <stddef.h>
#include <ti/bios/include/que.h>
#include <ti/bios/include/sem.h>
#include <ti/bios/include/sys.h>

typedef ti_sysbios_knl_Mailbox_Struct    MBX_Obj, *MBX_Handle;

typedef struct MBX_Attrs {
    Int         segid;
    String      name;
} MBX_Attrs;

extern __FAR__ MBX_Attrs MBX_ATTRS;

extern MBX_Handle MBX_create(Uns size, Uns length, MBX_Attrs *attrs);

static inline Void MBX_delete(MBX_Handle mbx)
{
    ti_sysbios_knl_Mailbox_Handle handle = (ti_sysbios_knl_Mailbox_Handle)mbx;

    ti_sysbios_knl_Mailbox_delete(&handle);
}

static inline String MBX_getname(MBX_Handle mbx)
{
    ti_sysbios_knl_Mailbox_Handle handle = (ti_sysbios_knl_Mailbox_Handle)mbx;

    return (ti_sysbios_knl_Mailbox_Handle_name(handle));
}

static inline Bool MBX_pend(MBX_Handle mbx, Ptr msg, Uns timeout)
{
    ti_sysbios_knl_Mailbox_Handle handle = (ti_sysbios_knl_Mailbox_Handle)mbx;

    return (ti_sysbios_knl_Mailbox_pend(handle, msg, timeout));
}

static inline Bool MBX_post(MBX_Handle mbx, Ptr msg, Uns timeout)
{
    ti_sysbios_knl_Mailbox_Handle handle = (ti_sysbios_knl_Mailbox_Handle)mbx;

    return (ti_sysbios_knl_Mailbox_post(handle, msg, timeout));
}

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* MBX_ */
