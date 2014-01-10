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
 *  ======== obj.h ========
 *
 */

#ifndef OBJ_
#define OBJ_

#include <ti/bios/include/mem.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Object types, current support debug SEM, MBX, TSK, SIO, SWI modules.
 */

#define OBJ_SEM         0
#define OBJ_SWI         1
#define OBJ_TSK         2
#define OBJ_MBX         3
#define OBJ_SIO         4
#define OBJ_DEV         5
#define OBJ_BUF         6

#define OBJ_MODULES     7
#define OBJ_KEY         0xabc0      /* key for type check */
#define OBJ_DEADKEY     0xdead      /* indicates object has been deleted */

/*
 *  ======== OBJ_create ========
 */
#define OBJ_create(type, size) MEM_calloc(0, size, 0)

/*
 *  ======== OBJ_delete ========
 */
#define OBJ_delete(type, ptr, size) MEM_free(0, ptr, size)

/*
 *  ======== OBJ_validate ========
 *  Validate an object.
 *  Returns:
 *      SYS_OK      a valid object.
 *      SYS_EDEAD   object was previously deleted
 *      SYS_EBADOBJ invalid object
 */
#define OBJ_validate(type, ptr) 1

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* OBJ_ */

