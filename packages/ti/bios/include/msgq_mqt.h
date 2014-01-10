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
 *  ======== msgq_mqt.h ========
 *  This file is for use by transports.
 *
 */

#ifndef MSGQ_MQT_
#define MSGQ_MQT_

#include <ti/bios/include/msgq.h>
#include <ti/bios/include/sys.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Internal constants used by both MSGQ and the MQTs. */
#define MSGQ_INVALIDPROCID   (Uint16)0xFFFF

/* The different status values for a MSGQ_Obj */ 
#define MSGQ_EMPTY    0      /* Messaging queue not being used    */
#define MSGQ_INUSE    1      /* Messaging queue being used        */

extern __FAR__ MSGQ_Config *MSGQ;

static inline Bool MSGQ_exist(MSGQ_Queue msgqQueue)
{ 
    return ((MSGQ->msgqQueues[(MSGQ_Id) msgqQueue].status == MSGQ_INUSE) ? 
                TRUE : FALSE);
}

/* 
 *  Use the following stub function definitions if a transport
 *  function is not implemented.
 */
#define MSGQ_MQTOPENNOTIMPL    (MSGQ_MqtOpen)SYS_zero
#define MSGQ_MQTCLOSENOTIMPL   (MSGQ_MqtClose)SYS_zero
#define MSGQ_MQTLOCATENOTIMPL  (MSGQ_MqtLocate)SYS_zero
#define MSGQ_MQTRELEASENOTIMPL (MSGQ_MqtRelease)SYS_zero
#define MSGQ_MQTPUTNOTIMPL     (MSGQ_MqtPut)SYS_zero

/* Function prototypes for transport-only MSGQ APIs */
extern Void MSGQ_sendErrorMsg(MSGQ_MqtError errorType, 
                              Uint16 mqtId, 
                              Uint16 parameter);
extern Int  MSGQ_locateLocal(String queueName, MSGQ_Queue *msgqQueue);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* MSGQ_MQT_ */
