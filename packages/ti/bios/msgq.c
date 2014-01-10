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
 *  ======== msgq.c ========
 */


#include <ti/bios/include/std.h>
#include <ti/bios/include/atm.h>
#include <ti/bios/include/gbl.h>
#include <ti/bios/include/fxn.h>
#include <ti/bios/include/hwi.h>
#include <ti/bios/include/msgq.h>
#include <ti/bios/include/que.h>
#include <ti/bios/include/pool.h>
#include <ti/bios/include/sys.h>

#include <string.h>

#define MSGQ_EMPTY 0
#define MSGQ_INUSE 1

/* Default message queue attributes. */
const __FAR__ MSGQ_Attrs MSGQ_ATTRS = {NULL,                   /* notifyHandle */
                               (MSGQ_Pend)SYS_zero,    /* pend         */
                               FXN_F_nop};             /* post         */

/* 
 *  Defaults are
 *     time: SYS_FOREVER.  A transport will wait forever for a positive
 *                         or negative locate reponse.
 */
const __FAR__ MSGQ_LocateAttrs MSGQ_LOCATEATTRS = {SYS_FOREVER};

/* 
 *  Defaults are
 *     poolId: 0.  There has to be at least one allocator
 *     arg:    0.  
 */
const __FAR__ MSGQ_LocateAsyncAttrs MSGQ_LOCATEASYNCATTRS = {0, 0};

/* This must be defined by the application that is using MSGQ */
extern __FAR__ MSGQ_Config MSGQ_config;

extern __FAR__ POOL_Config *POOL;


/* Hook up the MSGQ variable.  Never access MSGQ_config directly! */
__FAR__ MSGQ_Config *MSGQ = &MSGQ_config;

/*
 *  ======== MSGQ_init ========
 *  Initialize some of the config parameters and open the transports.
 */
Void MSGQ_init(Void)
{
    Uint16 i;
    static __FAR__ Bool runFlag = FALSE;

    /* Make sure this does not run more than once */
    if (runFlag == TRUE) {
        return;
    }
    runFlag = TRUE;

    /* Make sure the MSGQ variable is ok */
    if (MSGQ == NULL) {
        SYS_abort("MSGQ: Failed to define MSGQ_config properly");
    }

    /* Initialize for the local message queues from the requested spot. */
    for (i = MSGQ->startUninitialized; i < MSGQ->numMsgqQueues; i++) {
        MSGQ->msgqQueues[i].status = MSGQ_EMPTY;        
    }

    for (i = 0; i < MSGQ->numProcessors; i++) {
        
        /* 
         *  For migration purposes, adding this check. An init
         *  function should be specified, but just in case...
         */
        if (MSGQ->transports[i].initFxn == NULL) {
            continue;
        }

        MSGQ->transports[i].initFxn();
    }
}

/*
 *  ======== MSGQ_exit ========
 *  Close the transports.
 */
Void MSGQ_exit(Void)
{
    Uint16 i;
    MSGQ_TransportHandle mqtHandle;   

    /* Loop through all the transports. */
    for (i = 0; i < MSGQ->numProcessors; i++) {
        
        mqtHandle = &(MSGQ->transports[i]);

        /* 
         * Do not try the local processor (which should have 
         * mqtHandle->fxns == NULL!) and any transport that 
         * has no interface functions.
         */
        if (i == GBL_getProcId() || mqtHandle->fxns == NULL) {
            continue;
        }

        /* Call the transport's close function. */
        if (mqtHandle->fxns->close(mqtHandle) != SYS_OK) {
            SYS_abort("MSGQ: Failed to close a transport");
        }
    }
}


/*
 *  ======== MSGQ_startup ========
 *  
 */
Void MSGQ_startup(Void)
{
    Uint16 i;
    MSGQ_TransportHandle  mqtHandle;
    static __FAR__ Bool runFlag = FALSE;

    /* Make sure this does not run more than once */
    if (runFlag == TRUE) {
        return;
    }
    runFlag = TRUE;

    for (i = 0; i < MSGQ->numProcessors; i++) {
 
        mqtHandle = &(MSGQ->transports[i]);        

        /* 
         * Do not try the local processor (which should have 
         * mqtHandle->fxns == NULL!) and any transport that 
         * has no interface functions.
         */
        if (i == GBL_getProcId() || mqtHandle->fxns == NULL) {
            continue;
        }

        /* Call the transport's open function */
        if (mqtHandle->fxns->open(mqtHandle) != SYS_OK) {
            SYS_abort("MSGQ: Failed to open a transport %d", i);
        }
    }
}

/*
 *  ======== MSGQ_open ========
 *  Opens a MSG queue under given name. If the attrs variable is NULL,
 *  the default MSGQ_Attrs values will be used.
 * 
 *  The queueName string must be persistent for the life of the message queue.
 *  Note: the queueName can be NULL, but then it cannot be located.
 */
Int MSGQ_open(String      queueName,
              MSGQ_Queue *msgqQueue,
              MSGQ_Attrs *attrs)
{     
    MSGQ_Id i;    
    MSGQ_Handle msgqHandle;
    
    /* assert(msgqQueue != NULL); */
    
    *msgqQueue = MSGQ_INVALIDMSGQ;
    
    /* Use the default attributes if none are supplied */
    if (attrs == NULL) {
        attrs = (MSGQ_Attrs *)&MSGQ_ATTRS; 
    }

    /*  Search the local message queue array to find a free slot in it. */ 
    for (i = 0; i < MSGQ->numMsgqQueues; i++) {
        
        /* 
         *  The following is atomic to allow multiple MSGQ_open()'s at once.
         *  If ATM_setu() returns MSGQ_EMPTY, use that slot.
         *  If ATM_setu() returns !MSGQ_EMPTY, continue looking...
         */
        if (ATM_setu((Uns *)&(MSGQ->msgqQueues[i].status), MSGQ_INUSE) ==
                MSGQ_EMPTY) {           
            /* 
             *  Found empty slot...
             *  1. Create the MSGQ_Queue value 
             *  2. break of this loop.
             */
            *msgqQueue = ((Uint32)GBL_getProcId() << 16) | i;
            break;
        }
    }

    /* See if we found a free slot */
    if (*msgqQueue == MSGQ_INVALIDMSGQ) {
        return (SYS_ENOTFOUND);
    }
    
    msgqHandle = &(MSGQ->msgqQueues[i]);
    
    /* Initialize the fields in the returned MSGQ_Obj */
    QUE_new(&(msgqHandle->queue));
    msgqHandle->name         = queueName;
    msgqHandle->notifyHandle = attrs->notifyHandle;
    msgqHandle->pend         = attrs->pend;
    msgqHandle->post         = attrs->post;
        
    return (SYS_OK);
}

/*
 *  ======== MSGQ_close ========
 *  Closes the message queue by setting the status field to empty.
 */
Int MSGQ_close(MSGQ_Queue msgqQueue)
{
    MSGQ_Msg msg;
    QUE_Handle queHandle = &(MSGQ->msgqQueues[(MSGQ_Id)msgqQueue].queue);

    /* assert(msgqQueue != MSGQ_INVALIDMSGQ); */

    /* Clear out any existing messages. */
    while ((QUE_Handle)(msg = QUE_get(queHandle)) != queHandle) {
        MSGQ_free(msg);
    }
    
    MSGQ->msgqQueues[(MSGQ_Id)msgqQueue].status = MSGQ_EMPTY;
    
    return (SYS_OK);
}


/*
 *  ======== MSGQ_locateLocal ========
 *  This function searches the local queue maintained by MSGQ for the 
 *  specified message queue.
 */
Int MSGQ_locateLocal(String queueName, MSGQ_Queue *msgqQueue)
{
    Uint16      i;

    /* assert(queueName != NULL); */
    /* assert(msgqQueue != NULL); */
    
    /* loop through the local message queues. */
    for (i = 0; i < MSGQ->numMsgqQueues; i++) {
        
        /* Check the message queue if it is opened and the name matches. */
        if ((MSGQ->msgqQueues[i].status == MSGQ_INUSE) && 
            (MSGQ->msgqQueues[i].name != NULL)         &&
            (!strcmp(MSGQ->msgqQueues[i].name, queueName))) {

            /*  Found the match. Return it. */
            *msgqQueue = ((Uint32)GBL_getProcId() << 16) | i;
            return (SYS_OK);                 
        }        
    }
    return (SYS_ENOTFOUND);
}


/*
 *  ======== MSGQ_locate ========
 *  Get a reference to an existing message queue via its name
 *  This is a synchronous function.
 */
Int MSGQ_locate(String             queueName,
                MSGQ_Queue        *msgqQueue,
                MSGQ_LocateAttrs  *locateAttrs)
{   
    Uint16                i;
    Int                   status;
    MSGQ_TransportHandle  mqtHandle;    

    /* assert(queueName != NULL); */
    /* assert(msgqQueue != NULL); */

    if (locateAttrs == NULL) {
        locateAttrs = (MSGQ_LocateAttrs *)&MSGQ_LOCATEATTRS;
    }
    
    /* Check the local message queue's first. */    
    status = MSGQ_locateLocal(queueName, msgqQueue);
        
    /*
     *  There are 2 different variations:
     *    1. Synchronous locate and message queue found
     *       Action: Return success and the located msgqQueue.
     *
     *    2. Synchronous locate and message queue not found
     *       Action: continue and search the transports.
     */
    if (status == SYS_OK) {
        /*
         *  If the message queue was located return success.
         */
        return (SYS_OK);
    }

    /*
     *  Loop through all the transports and see if a transport
     *  can locate the message queue. 
     */

    for (i = 0; i < MSGQ->numProcessors; i++) {        

        mqtHandle = &(MSGQ->transports[i]);

        /* 
         * Do not try the local processor (which should have 
         * mqtHandle->fxns == NULL!) and any transport that 
         * has no interface functions.
         */
        if (i == GBL_getProcId() || mqtHandle->fxns == NULL) {
            continue;
        }
        
        /*
         *  Since the locate is sync, the transport will
         *     - return SYS_OK and fill in msgqQueue if located.
         *       In this case, return success and the msgqQueue
         *     - return a failure code if not located.
         *       In this case, continue searching
         */
        status = mqtHandle->fxns->locate(mqtHandle,
                                         queueName,
                                         TRUE,
                                         msgqQueue,
                                         (Ptr)locateAttrs);
        if (status == SYS_OK) {
            return (SYS_OK);
        }        
    }
    return (SYS_ENOTFOUND);    
}

/*
 *  ======== MSGQ_locateAsync ========
 *  Get a reference to an existing message queue via its name
 *  This is an asynchronous function.
 */
Int MSGQ_locateAsync(String                  queueName,
                     MSGQ_Queue              replyQueue,
                     MSGQ_LocateAsyncAttrs  *locateAsyncAttrs)
{   
    Uint16                i;
    Int                   status;
    MSGQ_TransportHandle  mqtHandle;
    MSGQ_AsyncLocateMsg  *msg;
    MSGQ_Queue            tempQueue;

    /* assert(queueName != NULL); */
    /* assert(replyQueue != MSGQ_INVALIDMSGQ); */

    if (locateAsyncAttrs == NULL) {
        locateAsyncAttrs = (MSGQ_LocateAsyncAttrs *)&MSGQ_LOCATEASYNCATTRS;
    }
    
    /* Check the local message queue's first. */    
    status = MSGQ_locateLocal(queueName, &tempQueue);
        
    /*
     *  There are 2 different variations:
     *    1. Asynchronous locate and message queue found
     *       Action: Send the message and return success.
     *
     *    2. Asynchronous locate and message queue not found
     *       Action: continue and search the remote transports.
     */
    if (status == SYS_OK) {
        
        /*
         *  The asynchronous case.
         * 
         *  Allocate the locator message from the allocator
         *  specified in the locateAsyncAttrs.
         */
        status = MSGQ_alloc(locateAsyncAttrs->poolId,
                            (MSGQ_Msg *)&msg,
                            sizeof(MSGQ_AsyncLocateMsg));
        
        if (status != SYS_OK) {
            return (SYS_EALLOC);
        }
        
        /* Fill in the field of the message. */
        msg->msgqQueue = tempQueue;
        msg->arg       = locateAsyncAttrs->arg;            
        MSGQ_setMsgId((MSGQ_Msg)msg, MSGQ_ASYNCLOCATEMSGID);

        status = MSGQ_put(replyQueue, (MSGQ_Msg)msg);
        if (status != SYS_OK) {
            /*
             *  Sending the message failed. Free the message
             *  and return the failure back.
             */
            MSGQ_free((MSGQ_Msg)msg);                
        }        
        return (status);
    }

    /*
     *  Loop through all the transports and see if a transport
     *  can locate the message queue. 
     */

    for (i = 0; i < MSGQ->numProcessors; i++) {
        
        mqtHandle = &(MSGQ->transports[i]);

        /* 
         * Do not try the local processor (which should have 
         * mqtHandle->fxns == NULL!) and any transport that 
         * has no interface functions.
         */
        if (i == GBL_getProcId() || mqtHandle->fxns == NULL) {
            continue;
        }

        /*
         *  Since the locate is async, the transport will
         *     - return SYS_OK if async locate was initiated.
         *       In this case, continue searching
         *     - return a failure code if async located could not be initiated.
         *       In this case, continue searching...It is up to the transport
         *       to send an error message.
         */
        mqtHandle->fxns->locate(mqtHandle, 
                                queueName,
                                FALSE,
                                &replyQueue, 
                                (Ptr)locateAsyncAttrs);
    }

    /* At this point of an async locate, just return. */         
    return (SYS_OK);
}

/*
 *  ======== MSGQ_release ========
 *  This function is to release a message queue that was located
 *  via the MSGQ_locate() or MSGQ_locateAsync() APIs
 *  If the message queue is local, do nothing. Otherwise
 *  call the transport's function.
 */
Int MSGQ_release(MSGQ_Queue msgqQueue)
{
    Uint16  procId  = msgqQueue >> 16;

    /* assert(msgqQueue != MSGQ_INVALIDMSGQ); */

    /* If it is a local message queue, nothing to release. */
    if (procId == GBL_getProcId()) {
        return (SYS_OK);
    }
 
    /* Call the mqtRelease to allow it to free resources if applicable. */
    return (MSGQ->transports[procId].fxns->release(
                &(MSGQ->transports[procId]), msgqQueue));
}


/*
 *  ======== MSGQ_alloc ========
 *  Allocates a message from the buffer pool
 */
Int MSGQ_alloc(Uint16 poolId, MSGQ_Msg *msg, Uint16 size)
{
    POOL_Handle poolHandle;

    /* assert(msg != NULL); */
    /* assert(poolId != POOL_INVALIDID); */

    /*
     *  Need the following check (instead of an assert) because the 
     *  transports may call this function with a poolId supplied
     *  by another system.  If there are more allocators on the 
     *  other system, the transport needs to know that (via returned code)
     *  and accommodate accordingly.
     */
    if (poolId >= POOL->numAllocators) {
        return (SYS_EINVAL);
    }

    /* Aid in readability. */
    poolHandle = &(POOL->allocators[poolId]);

    /* 
     *  Call the interface's allocation method and fill in 
     *  the allocated message.
     */ 
    if (poolHandle->fxns->alloc(poolHandle->object, (Ptr *)msg, size) == 
        SYS_OK) {
        /*
         *  Fill in some of the MSGQ_MsgHeader's fields.
         *  The other fields are filled in on the MSGQ_put() or by 
         *  the application (e.g. MSGQ_setSrcQueue()).
         *
         *  size:   Need to place the requested size of the buffer in the msg,
         *          so the caller does not need to remember it.
         *  poolId: Need this to call the correct allocator to free the buffer
         *  srcId:  If the user does not set it via MSGQ_setSrcQueue().
         *  msgId:  If the user does not set it via MSGQ_setMsgId().
         */
        (*msg)->size   = size;
        (*msg)->poolId = poolId;
        (*msg)->srcId  = MSGQ_INVALIDMSGQ;
        (*msg)->msgId  = MSGQ_INVALIDMSGID;

        return (SYS_OK);
    }

    return (SYS_EALLOC);
}


/*
 *  ======== MSGQ_free ========
 *  Frees a message back to the buffer pool
 */
Int MSGQ_free(MSGQ_Msg msg)
{   
    POOL_Handle poolHandle; 
    
    /* assert(msg != NULL); */
    
    poolHandle = &(POOL->allocators[msg->poolId]);

    /* Call the allocator's free function. */
    poolHandle->fxns->free(poolHandle->object, msg, msg->size);
    
    return (SYS_OK);
}

/*
 *  ======== MSGQ_get ========
 *  Get a message from the QUE associated with this message queue.
 */
Int MSGQ_get(MSGQ_Queue msgqQueue, MSGQ_Msg *msg, Uns timeout)
{    
    QUE_Handle queHandle;
    MSGQ_Handle msgqHandle = &(MSGQ->msgqQueues[(MSGQ_Id)msgqQueue]);
    
    /* assert(msg != NULL); */
    /* assert(msgqQueue != MSGQ_INVALIDMSGQ); */
    
    /* Get the address of the QUE_Obj embedded in the data structure */
    queHandle = &(msgqHandle->queue);

    /* Keep looping while there is no element in the QUE. */
    while ((QUE_Handle)(*msg = QUE_get(queHandle)) == queHandle) {
        /* 
         *  Block until notified.  If FALSE is returned, the pend has
         *  timed-out and no message should be returned to the caller
         */
        if (msgqHandle->pend(msgqHandle->notifyHandle, timeout) == FALSE) {            
            return (SYS_ETIMEOUT);
        }
    }    
    
    return (SYS_OK);
}

/*
 *  ======== MSGQ_put ========
 *  If the message queue is remote, give it to the transport to
 *  send to the other processor.
 *  If the message queue is local, place the msg on its QUE.
 *  This function never blocks.
 */
Int MSGQ_put(MSGQ_Queue msgqQueue, MSGQ_Msg msg)
{
    MSGQ_Handle msgqHandle;
    Uint16      dstProcId  = msgqQueue >> 16;    
    
    /* assert(msg != NULL); */
    /* assert(msgqQueue != MSGQ_INVALIDMSGQ); */
    
    msg->dstId = (MSGQ_Id)msgqQueue;    
    
    if (dstProcId != GBL_getProcId()) {
        /* Call the transport associated with this message queue */
        return (MSGQ->transports[dstProcId].fxns->put(
                    &(MSGQ->transports[dstProcId]), msg));
    }    
    
    msgqHandle = &(MSGQ->msgqQueues[(MSGQ_Id)msgqQueue]);
    
    /* Place the message on the QUE */
    QUE_put(&(msgqHandle->queue), msg);

    /* Notify the reader. */
    msgqHandle->post(msgqHandle->notifyHandle);
    
    return (SYS_OK);
}

/*
 *  ======== MSGQ_count ========
 *  Return the number of messages in a message queue. Assuming
 *  caller made thread safe.
 */
Int MSGQ_count(MSGQ_Queue msgqQueue, Uns *count)
{    
    QUE_Elem   *queElem;
    QUE_Handle  queHandle;
    
    /* assert(count != NULL); */
    /* assert(msgqQueue != MSGQ_INVALIDMSGQ); */
    /* assert(this is a local message queue); */

    *count = 0;
    
    /* Get the address of the QUE_Obj embedded in the data structure */
    queHandle = &(MSGQ->msgqQueues[(MSGQ_Id)msgqQueue].queue);

    queElem = QUE_head(queHandle);

    /* Keep looping while there is an element in the QUE. */
    while ((QUE_Handle)queElem != queHandle) {
        (*count)++;
        queElem = QUE_next(queElem);
    }    
    
    return (SYS_OK);
}

/*
 *  ======== MSGQ_getAttrs ========
 *  Return the attributes for a message queue.  The attributes were 
 *  defined in the MSGQ_open for the message queue.
 */
Int MSGQ_getAttrs(MSGQ_Queue msgqQueue, MSGQ_Attrs *attrs)
{    
    MSGQ_Handle msgqHandle;

    /* assert(attrs != NULL); */
    /* assert(msgqQueue != MSGQ_INVALIDMSGQ); */

    /* Make sure the message queue is local */
    if (MSGQ_isLocalQueue(msgqQueue)) {
        msgqHandle = &(MSGQ->msgqQueues[(MSGQ_Id)msgqQueue]);
        attrs->notifyHandle = msgqHandle->notifyHandle;
        attrs->pend         = msgqHandle->pend;
        attrs->post         = msgqHandle->post;
        return (SYS_OK);
    }
    return (SYS_EINVAL);
}

/*
 *  ======== MSGQ_getSrcQueue ========
 *  Using the fields in the message, return the message queue 
 *  that the sender specified in the MSGQ_setSrcQueue().
 */
Int MSGQ_getSrcQueue(MSGQ_Msg msg, MSGQ_Queue *msgqQueue)
{   
    /* assert(msg != NULL); */
    /* assert(msgqQueue != NULL); */

    /* If the srcId is invalid there is nothing can be done. */
    if (msg->srcId == MSGQ_INVALIDMSGQ) {        
        return (SYS_ENOTFOUND);
    }
    
    *msgqQueue = (((Uint32)msg->srcProcId) << 16) | msg->srcId;

    return (SYS_OK);
}

/*
 *  ======== MSGQ_sendErrorMsg ========
 *  If the application has called MSGQ_setErrorHandler(), then this
 *  function allocates an error message and sends it to the 
 *  specified message queue. If the application has not called 
 *  MSGQ_setErrorHandler(), this function does nothing.
 */
Void MSGQ_sendErrorMsg(Uint16 errorType, Uint16 mqtId, Uint16 parameter)
{
    MSGQ_AsyncErrorMsg *msg;
    Int  status;

    /* If the error handler has not been set-up, just return to the caller. */
    if (MSGQ->errorQueue != MSGQ_INVALIDMSGQ) {
        /*
         *  Allocate an asynchronous error message. Do nothing if 
         *  message allocation fails, just return to the caller.
         */
        status = MSGQ_alloc(MSGQ->errorPoolId,                            
                            (MSGQ_Msg *)&msg,
                            sizeof(MSGQ_AsyncErrorMsg));
        if (status == SYS_OK) {
            
            /* Fill in the fields of the message. */
            msg->errorType = errorType;
            msg->mqtId     = mqtId;
            msg->parameter = parameter;
            
            MSGQ_setMsgId((MSGQ_Msg)msg, MSGQ_ASYNCERRORMSGID);

            /* Send the message to the user specified error handler. */
            status = MSGQ_put(MSGQ->errorQueue, (MSGQ_Msg)msg);
            /* If put fails, free the message. */
            if (status != SYS_OK) {
                MSGQ_free((MSGQ_Msg)msg);
            }
        }
    }
}

/*
 *  ======== MSGQ_setErrorHandler ========
 *  Sets the fields to allow a transport to send async. error messages
 */
Int MSGQ_setErrorHandler(MSGQ_Queue errorQueue, Uint16 poolId)
{
    Uns key;

    /* assert(poolId < MSGQ->numAllocators); */
    /* assert(errorQueue != MSGQ_INVALIDMSGQ); */
    
    key = HWI_disable();
    MSGQ->errorQueue = errorQueue;
    MSGQ->errorPoolId = poolId;    
    HWI_restore(key);
    
    return (SYS_OK);
}
