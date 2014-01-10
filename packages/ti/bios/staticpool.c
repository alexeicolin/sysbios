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
 *  ======== staticpool.c ========
 */
 
#include <ti/bios/include/std.h>
#include <ti/bios/include/fxn.h>
#include <ti/bios/include/pool.h>
#include <ti/bios/include/que.h>
#include <ti/bios/include/sys.h>
#include <ti/bios/include/staticpool.h>

#include <stddef.h>

/* 
 *  With these defaults, nothing will crash, but no messages can
 *  be allocated.
 */
static const __FAR__ STATICPOOL_Params defaultParams = {NULL, 0 , 8};

/* 
 *  Allocator interface functions
 *  There is no action for the close function, so just using
 *  FXN_F_nop to save footprint.
 */
const __FAR__ POOL_Fxns STATICPOOL_FXNS = {
    _STATICPOOL_open,
    POOL_CLOSENOTIMPL,
    _STATICPOOL_alloc,
    _STATICPOOL_free
};

/*
 *  ======== poolAlloc ========
 *  Function to return a pointer into the static buffer.
 *  The buf parameter is used both as an in and out parameter.
 */
Int _STATICPOOL_alloc(Ptr object, Ptr *buf, size_t size)
{
    /* assert(buf != NULL); */
    /* assert(object != NULL); */
    
    /* Make sure the message is large enough. */
    if (size > ((STATICPOOL_Handle)object)->bufferSize) {
        return (SYS_EINVAL);
    }

    /* Get a free message from the queue and make sure it is valid */
    if ((*buf = QUE_get(&(((STATICPOOL_Handle)object)->queue))) == 
         &(((STATICPOOL_Handle)object)->queue)) {

        /* No message, return error */
        return (SYS_EALLOC);
    }
    
    return (SYS_OK);
}

/*
 *  ======== poolFree ========
 *  Return the buffer back to the linked list.
 */
Void _STATICPOOL_free(Ptr       object,
                      Ptr       buffer,
                      size_t    size)
{
    /* assert(object != NULL); */
    /* assert(buffer != NULL); */
    /* assert(size <= ((STATICPOOL_Handle)object)->bufferSize)); */
    
    /* Place the message back on the queue */
    QUE_put(&(((STATICPOOL_Handle)object)->queue), (QUE_Elem *)buffer);
}


/*
 *  ======== poolOpen ========
 *  Allocates and initializes the state information structure.
 */
Int _STATICPOOL_open(Ptr *object, Ptr params)
{
    size_t size;
    size_t length;
    Char *buffer;
    STATICPOOL_Handle pool;
    STATICPOOL_Params *staticPoolParams;
    
    if (params == NULL) {
        staticPoolParams = (STATICPOOL_Params *)&defaultParams;
    }
    else {
        staticPoolParams = (STATICPOOL_Params *)params;
    }

    /* Ensure the bufferSize is a multiple of 8 MADUs. */
    if ((staticPoolParams->bufferSize & 0x7) != 0) {
        return (SYS_EINVAL);
    }    

    /* 
     *  This allocator must have the object defined. 
     *  This is not an assert, it has to be a runtime check.
     */
    if (*object == NULL) {
        return (SYS_EBADOBJ);
    }
    pool = (STATICPOOL_Handle)*object;

    /* Initialize the object's information. */
    pool->bufferSize = staticPoolParams->bufferSize;
    QUE_new(&(pool->queue));
    
    /*
     *  Force the first buffer to be on a 8 MADUs boundary. This 
     *  may waste a little space at the top of the block of memory
     *  given by the application in the parameters. 
     *  Also if there is some space wasted at the top, it may result
     *  in fewer messages than the user expected being placed on the queue.
     *  Note: need to adjust the length by the offset amount.
     */
    length = staticPoolParams->length - 
        ((0x8 - ((LgUns)(staticPoolParams->addr) & 0x7)) & 0x7);

    buffer = (Char *)(((LgUns)(staticPoolParams->addr) + 0x7) & ~0x7); 

    size = staticPoolParams->bufferSize;    
    
    /* Chop the buffer up and place the pieces (i.e. messages) on a queue */    
    while(size <= length) {

        /* Add onto the free queue */
        QUE_put(&(pool->queue), (QUE_Elem *)buffer);

        /* Increment by the bufferSize */
        buffer += staticPoolParams->bufferSize;

        /* Update the size to determine when to stop */
        size += staticPoolParams->bufferSize;
    }    

    return (SYS_OK);
}


