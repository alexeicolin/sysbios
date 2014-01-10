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
 *  ======== msgq.h ========
 *
 */

#ifndef MSGQ_
#define MSGQ_

#include <ti/bios/include/std.h>
#include <ti/bios/include/fxn.h>
#include <ti/bios/include/gbl.h>
#include <ti/bios/include/que.h>
#include <ti/bios/include/pool.h>

#ifdef __cplusplus
extern "C" {
#endif


/* Error type for transport asynchronous errors */ 
typedef Uint16   MSGQ_MqtError;  

/* 
 *  MSGQ_Id is unique for a processor.
 *  MSGQ_Queue is unique for the system.
 */ 
typedef Uint16   MSGQ_Id;  
typedef Uint32   MSGQ_Queue;

/* Used to denote an invalid message queue */
#define MSGQ_INVALIDMSGQ       0xFFFF

/* Message ids ranges */
#define MSGQ_INTERNALIDSSTART  0xFF00  /* Start of internal MSGQ msg id range*/
#define MSGQ_ASYNCLOCATEMSGID  0xFF00  /* Asynchronous locate message id     */
#define MSGQ_ASYNCERRORMSGID   0xFF01  /* Asynchronous error message id      */
#define MSGQ_INTERNALIDSEND    0xFF7f  /* End of internal MSGQ msg id range  */
#define MSGQ_MQTMSGIDSSTART    0xFF80  /* Start of transport msg id range    */
#define MSGQ_MQTMSGIDSEND      0xFFFE  /* End of transport msg id range      */
#define MSGQ_INVALIDMSGID      0xFFFF  /* Used to denote no message id value.*/

/* Codes for async errors */ 
#define MSGQ_MQTERROREXIT      (MSGQ_MqtError)-1
#define MSGQ_MQTFAILEDPUT      (MSGQ_MqtError)-2
#define MSGQ_MQTERRORINTERNAL  (MSGQ_MqtError)-3
#define MSGQ_MQTERRORPHYSICAL  (MSGQ_MqtError)-4
#define MSGQ_MQTERRORALLOC     (MSGQ_MqtError)-5


/* Typedefs for the post and pend functions. */
typedef Bool (*MSGQ_Pend)(Ptr notifyHandle, Uns timeout);
typedef Void (*MSGQ_Post)(Ptr notifyHandle);

/* The following is the message queue object. */
typedef struct MSGQ_Obj {
    String               name;             /* Unique name of the queue        */
    QUE_Obj              queue;            /* Actual queue repository         */
    Ptr                  notifyHandle;     /* Used in the pend/post functions */
    MSGQ_Pend            pend;             /* Called within MSGQ_get()        */
    MSGQ_Post            post;             /* Called within MSGQ_put()        */
    Uns                  status;           /* Has the queue been initialized  */
} MSGQ_Obj, *MSGQ_Handle;

/* The message queue attributes.  Used when opening a message queue. */
typedef struct MSGQ_Attrs {
    Ptr        notifyHandle;               /* Used in the pend/post functions */
    MSGQ_Pend  pend;                       /* Called within MSGQ_get()        */
    MSGQ_Post  post;                       /* Called within MSGQ_put()        */
} MSGQ_Attrs;

/* The locate attributes.  Used when locating a message queue. */
typedef struct MSGQ_LocateAttrs {    
    Uns         timeout;                   /* Max time for transport to wait  */
} MSGQ_LocateAttrs;

/* The locate async attributes.  Used when async. locating a message queue. */
typedef struct MSGQ_LocateAsyncAttrs {        
    Uint16      poolId;                    /* Where to allocate locate msg    */
    Arg         arg;                       /* Passed back in locate msg       */
} MSGQ_LocateAsyncAttrs;

/*
 *  The following is the message header.  This structure must be the first
 *  field of all messages passed on a message queue.
 *  This structure requires that all fields be exact size, since it might
 *  go across a processor boundary.
 */
typedef struct MSGQ_MsgHeader { 
    Uint32 reserved[2];         /* Transport specific                         */
    Uint16 srcProcId;           /* Proc Id for the src message queue          */
    Uint16 poolId;              /* Id of the allocator that allocated the msg */
    Uint16 size;                /* Size of the allocated msg                  */
    Uint16 dstId;               /* Destinaton message queue id                */
    Uint16 srcId;               /* Source message queue id                    */
    Uint16 msgId;               /* User specified message id                  */
} MSGQ_MsgHeader, *MSGQ_Msg;

/* Asynchronous locate message */
typedef struct MSGQ_AsyncLocateMsg {
    MSGQ_MsgHeader  header;
    MSGQ_Queue      msgqQueue;
    Arg             arg;
} MSGQ_AsyncLocateMsg;

/* Asynchronous error message */
typedef struct MSGQ_AsyncErrorMsg {
    MSGQ_MsgHeader  header;
    MSGQ_MqtError   errorType;
    Uint16          mqtId;
    Uint16          parameter;
} MSGQ_AsyncErrorMsg;

typedef struct MSGQ_TransportObj *MSGQ_TransportHandle;

/* Typedefs for the transport interface and init functions. */
typedef Void (*MSGQ_MqtInit)(Void);
typedef Int  (*MSGQ_MqtOpen)(MSGQ_TransportHandle mqtHandle);
typedef Int  (*MSGQ_MqtClose)(MSGQ_TransportHandle mqtHandle);
typedef Int  (*MSGQ_MqtLocate)(MSGQ_TransportHandle mqtHandle,
                               String               queueName,
                               Bool                 sync,
                               MSGQ_Queue          *msgqQueue,
                               Ptr                  locateAttrs);
typedef Int  (*MSGQ_MqtRelease)(MSGQ_TransportHandle mqtHandle, 
                                MSGQ_Queue msgqQueue);
typedef Int  (*MSGQ_MqtPut)(MSGQ_TransportHandle mqtHandle, MSGQ_Msg msg);

/* The transport interface function definition. */
typedef struct MSGQ_TransportFxns {
    MSGQ_MqtOpen           open;            /* Called during BIOS startup   */
    MSGQ_MqtClose          close;           /* Used to close a transport    */
    MSGQ_MqtLocate         locate;          /* Called when locating a queue */
    MSGQ_MqtRelease        release;         /* Called to release a queue    */
    MSGQ_MqtPut            put;             /* Called to send a msg         */
} MSGQ_TransportFxns;

/* The transport object */
typedef struct MSGQ_TransportObj {
    MSGQ_MqtInit        initFxn;    /* Transport init function             */
    MSGQ_TransportFxns *fxns;       /* Transport interface functions       */
    Ptr                 params;     /* Transport-specific setup parameters */
    Ptr                 object;     /* Transport-specific object           */
    Uint16              procId;     /* Processor Id that mqt talks to      */
} MSGQ_TransportObj;

/* Default no transport definition */
#define MSGQ_NOTRANSPORT {FXN_F_nop, NULL, NULL, NULL, 0}

/* Configuration structure */
typedef struct MSGQ_Config {
    MSGQ_Obj           *msgqQueues;        /* Array of message queue handles */
    MSGQ_TransportObj  *transports;        /* Array of transports            */
    Uint16              numMsgqQueues;     /* Number of message queue handles*/
    Uint16              numProcessors;     /* Number of processors           */
    Uint16              startUninitialized;/* First msgq to init             */
    MSGQ_Queue          errorQueue;        /* Receives async transport errors*/
    Uint16              errorPoolId;       /* Alloc error msgs from poolId   */
} MSGQ_Config;

/* Default creation and location attributes */
extern __FAR__ const MSGQ_Attrs            MSGQ_ATTRS;
extern __FAR__ const MSGQ_LocateAttrs      MSGQ_LOCATEATTRS;
extern __FAR__ const MSGQ_LocateAsyncAttrs MSGQ_LOCATEASYNCATTRS;

/* Inlines used to manage data in a message */
static inline Uint16 MSGQ_getMsgId(MSGQ_Msg msg)
{ 
    return (msg->msgId); 
}

static inline Void MSGQ_setMsgId(MSGQ_Msg msg, Uint16 msgId)
{
    msg->msgId = msgId;
}

static inline Void MSGQ_getDstQueue(MSGQ_Msg msg, MSGQ_Queue *msgqQueue)
{
    *msgqQueue = ((Uint32)GBL_getProcId() << 16) | msg->dstId;
}

static inline Void MSGQ_setSrcQueue(MSGQ_Msg msg, MSGQ_Queue msgqQueue)
{
    msg->srcId     = (MSGQ_Id)msgqQueue;
    msg->srcProcId = (msgqQueue >> 16);
}

static inline Uint16 MSGQ_getMsgSize(MSGQ_Msg msg)
{ 
    return (msg->size);
}

static inline Bool MSGQ_isLocalQueue(MSGQ_Queue msgqQueue)
{ 
    return ((GBL_getProcId() == (msgqQueue >> 16)) ? TRUE : FALSE);
}

/* Function prototypes */
extern Void MSGQ_init(Void);
extern Void MSGQ_startup(Void);
extern Void MSGQ_exit(Void);
extern Int  MSGQ_open(String      queueName, 
                      MSGQ_Queue *msgqQueue,
                      MSGQ_Attrs *attrs);
extern Int  MSGQ_locate(String            queueName, 
                        MSGQ_Queue       *msgqQueue,
                        MSGQ_LocateAttrs *locateAttrs);
extern Int  MSGQ_locateAsync(String                 queueName, 
                             MSGQ_Queue             replyQueue,
                             MSGQ_LocateAsyncAttrs *locateAsyncAttrs);
extern Int  MSGQ_close(MSGQ_Queue msgqQueue);
extern Int  MSGQ_release(MSGQ_Queue msgqQueue);
extern Int  MSGQ_alloc(Uint16 poolId, MSGQ_Msg *msg, Uint16 size);
extern Int  MSGQ_free(MSGQ_Msg msg);
extern Int  MSGQ_put(MSGQ_Queue msgqQueue, MSGQ_Msg msg);       
extern Int  MSGQ_get(MSGQ_Queue msgqQueue, MSGQ_Msg *msg, Uns timeout);
extern Int  MSGQ_getAttrs(MSGQ_Queue msgqQueue, MSGQ_Attrs *attrs);
extern Int  MSGQ_getSrcQueue(MSGQ_Msg msg, MSGQ_Queue *msgqQueue);
extern Int  MSGQ_setErrorHandler(MSGQ_Queue errorQueue, Uint16 poolId);
extern Int  MSGQ_count(MSGQ_Queue msgqQueue, Uns *count);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* MSGQ_ */
