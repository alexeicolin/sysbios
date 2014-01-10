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
 *  ======== que.h ========
 *
 */

#ifndef QUE_
#define QUE_

#include <ti/bios/include/fxn.h>
#include <ti/bios/include/mem.h>

/*
 *
 *
 * QUEUEs
 * ======
 * 
 *      Queues are doubly linked with dummy node to eliminate special
 *      cases for speed.
 *          _______        _______        _______      _______
 *  ,----->|_______|----->|_______|----->|_______|--->|_______|--//---,
 *  | ,----|_______|<-----|_______|<-----|_______|<---|_______|<-//-, |
 *  | |    prev           queue          elem         next          | |
 *  | |_____________________________________________________________| |
 *  |_________________________________________________________________|
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct QUE_Elem {
    struct QUE_Elem *next;
    struct QUE_Elem *prev;
} QUE_Elem, *QUE_Handle;

typedef struct QUE_Elem QUE_Obj;

typedef struct QUE_Attrs {
    Int dummy;
} QUE_Attrs;

extern __FAR__ QUE_Attrs QUE_ATTRS;

/*
 *  ======== QUE_create ========
 */
extern QUE_Handle QUE_create(QUE_Attrs *attrs);

/*
 *  ======== QUE_delete ========
 */
#define QUE_delete(queue)       MEM_free(0, (queue), sizeof(QUE_Obj))

/*
 *  ======== QUE_dequeue ========
 *
 *  get elem from front of "queue".
 *  This operation is *NOT* atomic.  External synchronization must
 *  be used to protect this queue from simultaneous access by interrupts
 *  or other tasks. 
 *
 *               _______        _______        _______      _______
 *  Before:     |_______|----->|_______|----->|_______|--->|_______|--->
 *              |_______|<-----|_______|<-----|_______|<---|_______|<---
 *               prev           queue          elem         next
 *
 *
 *               _______        _______        _______
 *  After:      |_______|----->|___*___|----->|_______|--->
 *              |_______|<-----|_______|<-----|___*___|<---
 *               prev           queue          next
 *               _______
 *      elem -->|___x___|       * = modified
 *              |___x___|       x = undefined
 *
 */
static inline Ptr QUE_dequeue(QUE_Handle queue)
{
    QUE_Elem *elem = queue->next;
    QUE_Elem *next = elem->next;

    queue->next = next;
    next->prev = queue;

    return (elem);
}

/*
 *  ======== QUE_empty ========
 */
#define QUE_empty(queue)        ((queue)->next == (queue))

/*
 *  ======== QUE_enqueue ========
 *
 *  put "elem" at end of "queue".
 *  This operation is *NOT* atomic.  External synchronization must
 *  be used to protect this queue from simultaneous access by interrupts
 *  or other tasks. 
 *
 *               _______        _______        _______
 *  Before:     |_______|----->|_______|----->|_______|--->
 *              |_______|<-----|_______|<-----|_______|<---
 *               prev           queue          next
 *               _______
 *      elem -->|___x___|       * = modified
 *              |___x___|       x = undefined
 *
 *               _______        _______        _______      _______
 *  After:      |___*___|----->|___*___|----->|_______|--->|_______|--->
 *              |_______|<-----|___*___|<-----|___*___|<---|_______|<---
 *               prev           elem          queue         next
 *
 */
static inline Void QUE_enqueue(QUE_Handle queue, Ptr elem)
{
    QUE_Elem *prev = queue->prev;

    ((QUE_Elem *)elem)->next = queue;
    ((QUE_Elem *)elem)->prev = prev;
    prev->next = (QUE_Elem *)elem;
    queue->prev = (QUE_Elem *)elem;
}

/*
 *  ======== QUE_get ========
 *  disable interrupts and returns the first element in the queue.
 */
extern Ptr QUE_get(QUE_Handle queue);

/*
 *  ======== QUE_head ========
 */
#define QUE_head(queue)         ((Ptr)((queue)->next))

/*
 *  ======== QUE_init ========
 */
#define QUE_init        FXN_F_nop

/*
 *  ======== QUE_insert ========
 */
#define QUE_insert(qElem, elem)         QUE_enqueue((QUE_Handle)qElem, elem)

/*
 *  ======== QUE_new ========
 */
#define QUE_new(elem)           (elem)->next = (elem)->prev = (elem)

/*
 *  ======== QUE_next ========
 */
#define QUE_next(elem)          ((Ptr)((QUE_Elem *)(elem))->next)

/*
 *  ======== QUE_prev ========
 */
#define QUE_prev(elem)          ((Ptr)((QUE_Elem *)(elem))->prev)

/*
 *  ======== QUE_print ========
 */
extern Void QUE_print(QUE_Handle queue);

/*
 *  ======== QUE_put ========
 *  Disable interrupts and put "elem" at end of "queue".  
 */
extern Void QUE_put(QUE_Handle queue, Ptr elem);

/*
 *  ======== QUE_remove ========
 */
#define QUE_remove(elem) {\
    ((QUE_Elem *)elem)->prev->next = ((QUE_Elem *)elem)->next;  \
    ((QUE_Elem *)elem)->next->prev = ((QUE_Elem *)elem)->prev;  \
    }

#ifdef _BIOSROM_

extern Ptr (*QUE_getPtr)(QUE_Handle queue);
extern Void (*QUE_putPtr)(QUE_Handle queue, Ptr elem);

#define QUE_getJ(queue)         (QUE_getPtr)(queue)
#define QUE_putJ(queue, elem)   (QUE_putPtr)(queue, elem)

#else

#define QUE_getJ(queue)         QUE_get(queue)
#define QUE_putJ(queue, elem)   QUE_put(queue, elem)

#endif

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* QUE_*/

