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
#include <ti/bios/include/pool.h>
#include <ti/bios/include/sys.h>

/* 
 *  Use in POOL_NOENTRY
 */
__FAR__ POOL_Fxns POOL_FXNS = {
    POOL_OPENNOTIMPL,
    POOL_CLOSENOTIMPL,
    POOL_ALLOCNOTIMPL,
    POOL_FREENOTIMPL
};

extern __FAR__ POOL_Config POOL_config;

/* 
 *  POOL module should never access POOL_config directly.
 *  It should always use POOL instead.
 */
__FAR__ POOL_Config *POOL = &POOL_config;


/*
 *  ======== POOL_init ========
 *  Open the allocators 
 */
Void POOL_init(Void)
{
    Uint16      i;    
    POOL_Handle  poolHandle;
    static __FAR__ Bool runFlag = FALSE;

    /* Make sure this does not run more than once */
    if (runFlag == TRUE) {
        return;
    }
    runFlag = TRUE;
    
    /* Make sure everything is hooked up */
    if (POOL == NULL) {
       SYS_abort("POOL: Failed to define POOL_config properly");
    }

    /* Loop through all the allocators. */
    for (i = 0; i < POOL->numAllocators; i++) {
        
        poolHandle = &(POOL->allocators[i]);

        /* Call the allocator's init and open function.  */
        poolHandle->initFxn();
        
        if (poolHandle->fxns->open(&(poolHandle->object),poolHandle->params) !=
            SYS_OK) {
            SYS_abort("POOL: Failed to open an allocator");
        }
    }    
}

/*
 *  ======== POOL_exit ========
 *  Close the allocators.
 */
Void POOL_exit(Void)
{
    Uint16 i;
    
    for (i = 0; i < POOL->numAllocators; i++) {
        
        /* Call the allocator's close function. */
        POOL->allocators[i].fxns->close(POOL->allocators[i].object);
    }
}


