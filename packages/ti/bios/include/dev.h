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
 *  ======== dev.h ========
 *
 */

#ifndef DEV_
#define DEV_
#define DEV_H_

#define xdc_runtime_Error__nolocalnames
#define ti_sdo_ipc_DriverTypes__nolocalnames

#include <stddef.h>
#include <ti/bios/include/std.h>
#include <ti/bios/include/fxn.h>
#include <ti/bios/include/que.h>
#include <ti/bios/include/sem.h>
#include <ti/bios/include/sys.h>

#include <xdc/runtime/Error.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DEV_INPUT   (0)
#define DEV_OUTPUT  (1)
#define DEV_MODES   (2)

/*
 *  These types are used to identify the type of the DEV driver
 *  in DEV_D_devtab[].
 *  WARNING -- These ids must match those defined .h62, etc. files!
 */
#define DEV_SIOTYPE             0
#define DEV_IOMTYPE             1
#define DEV_OTHERTYPE           2

typedef struct DEV_Obj *DEV_Handle;

/*
 * DEV_Frame represents hangers which hold the stream buffers
 * All the buffer exchange between SIO/DEV happens using these frames.
 *
 * IMPORTANT NOTE: The DEV_Frame struct has to match DriverTypes_Packet
 * The size of type of each field within this struct should match.
 */
typedef struct DEV_Frame {      /* frame object */
    QUE_Elem                            link;   /* queue link */
    Ptr                                 addr;   /*! buffer address */
    SizeT                               origSize; /*! size requested */
    SizeT                               size;   /*! processed size */
    Arg                                 arg;    /*! arg to be used by end app */
    Bits32                              cmd;    /*! command for mini-driver */
    xdc_runtime_Error_Id                error;  /*! error id */
    Arg                                 misc;   /*! reserved */
    Int                                 status; /*! reserved for IOM support */
    UArg                                drvArg; /*! reserved for driver */
} DEV_Frame;

typedef Int     (*DEV_Tclose)(DEV_Handle);
typedef Int     (*DEV_Tctrl)(DEV_Handle, Uns, Arg);
typedef Int     (*DEV_Tidle)(DEV_Handle, Bool);
typedef Void    (*DEV_Tinit)(Void);
typedef Int     (*DEV_Tissue)(DEV_Handle);
typedef Int     (*DEV_Topen)(DEV_Handle, String);
typedef Bool    (*DEV_Tready)(DEV_Handle, SEM_Handle);
typedef Int     (*DEV_Treclaim)(DEV_Handle);

/*
 * DEV_Fxns represent driver functions template in SIO/DEV model
 */
typedef struct DEV_Fxns {
    DEV_Tclose          close;
    DEV_Tctrl           ctrl;
    DEV_Tidle           idle;
    DEV_Tissue          issue;
    DEV_Topen           open;
    DEV_Tready          ready;
    DEV_Treclaim        reclaim;
} DEV_Fxns;

typedef struct DEV_Callback {   /* DEV callback structure */
    Fxn        fxn;            /* function */
    Arg        arg0;           /* argument 0 */
    Arg        arg1;           /* argument 1 */
} DEV_Callback;

/*
 * DEV_Obj provides interface to DEV layer and its part of SIO object.
 */
typedef struct DEV_Obj {    /* must be first field in device object */
    QUE_Handle  todevice;   /* downstream frames go here */
    QUE_Handle  fromdevice; /* upstream frames go here */
    size_t      bufsize;    /* buffer size */
    Uns         nbufs;      /* number of buffers */
    Int         segid;      /* buffer segment id */
    Int         mode;       /* DEV_INPUT/DEV_OUTPUT */
#if (defined(_54_) && defined(_FAR_MODE)) || defined(_55_) || defined(_28_)
    LgInt       devid;      /* must fit code addr.-e.g. DGN passes fxn ptr */
#else
    Int         devid;      /* device id */
#endif
    Ptr         params;     /* device parameters */
    Ptr         object;     /* pointer to device specific object */
    DEV_Fxns    fxns;       /* driver function table */
    Uns         timeout;    /* timeout for DEV_reclaim() */
    Uns         align;      /* buffer alignment */
    DEV_Callback *callback; /* pointer to callback */
} DEV_Obj;

/*
 * DEV_Device holds the device attributes as specified by the user
 * in the GCONF DEV template or by the DEV_createDevice API.
 */
typedef struct DEV_Device {     /* device driver specifier */
    String      name;           /* device name */
    Void        *fxns;          /* device function table */
#if (defined(_54_) && defined(_FAR_MODE)) || defined(_55_) || defined(_28_)
    LgInt       devid;          /* device id */
#else
    Int         devid;          /* device id */
#endif
    Ptr         params;         /* device parameters */
    Uns         type;           /* type of the device */
    Ptr         devp;           /* pointer to device global data */
} DEV_Device;

typedef struct DEV_TableElem {
    QUE_Elem    qElem;
    DEV_Device  device;
} DEV_TableElem;

/*
 * DEV_Attrs is used while creating the device dynamically
 */
typedef struct DEV_Attrs{
#if (defined(_54_) && defined(_FAR_MODE)) || defined(_55_) || defined(_28_)
    LgInt       devid;          /* device id */
                                /* device id could be function pointer
                                   hence 54f,55 & 28 its declared as LgUns */
#else
    Int         devid;          /* device id */
#endif
    Ptr         params;         /* device parameters */
    Uns         type;           /* type of the device */
    Ptr         devp;           /* pointer to device global data */
}DEV_Attrs;

/*
 * DEV function macros make SIO code more readable.
 */
#define DEV_close(_dev)                 (*((_dev)->fxns.close))(_dev)
#define DEV_ctrl(_dev,_cmd,_arg)        (*((_dev)->fxns.ctrl))(_dev,_cmd,_arg)
#define DEV_idle(_dev,_flush)           (*((_dev)->fxns.idle))(_dev,_flush)
#define DEV_issue(_dev)                 (*((_dev)->fxns.issue))(_dev)
#define DEV_open(_dev,_name)            (*((_dev)->fxns.open))(_dev,_name)
#define DEV_ready(_dev,_sem)            (*((_dev)->fxns.ready))(_dev,_sem)
#define DEV_reclaim(_dev)               (*((_dev)->fxns.reclaim))(_dev)

/*
 *  ======== default fxns ========
 */
#define DEV_CLOSE       ((DEV_Tclose)SYS_zero)
#define DEV_CTRL        ((DEV_Tctrl)DEV_ebadio)
#define DEV_EXCHANGE    ((DEV_Tinput)DEV_ebadio)
#define DEV_IDLE        ((DEV_Tidle)SYS_zero)
#define DEV_ISSUE       ((DEV_Tissue)DEV_ebadio)
#define DEV_OPEN        ((DEV_Topen)SYS_zero)
#define DEV_READY       ((DEV_Tready)SYS_one)
#define DEV_RECLAIM     ((DEV_Treclaim)DEV_ebadio)

/* for backwards compatibility ... */
extern Void             DEV_init(Void);

extern Int              DEV_ebadio(DEV_Handle);

extern String           DEV_match(String name, DEV_Device **driver);
extern Void             DEV_find(String name, DEV_Device **driver);
extern DEV_Frame        *DEV_mkframe(Int segid, Uns size, Uns align);
extern Void             DEV_rmframe(DEV_Frame *frame, Int segid, Uns size);
extern Int DEV_createDevice(String name, Void *fxns, Fxn initFxn,
                                DEV_Attrs *attrs);
extern Int DEV_deleteDevice(String name);

extern __FAR__ DEV_Attrs        DEV_ATTRS;

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* DEV_*/
