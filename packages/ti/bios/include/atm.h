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
 *  ======== atm.h ========
 *
 *  Atomic processor operations:
 *      ATM_and()   ATM_clear() ATM_dec()
 *      ATM_inc()   ATM_or()    ATM_set()
 *
 */

#ifndef ATM_
#define ATM_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== ATM_and ========
 *  atomically and integer or unsigned with mask and return its old value
 */
extern Int ATM_andi( volatile Int *dst, Int mask );
extern Uns ATM_andu( volatile Uns *dst, Uns mask );

/*
 *  ======== ATM_clear ========
 *  atomically clear integer or unsigned and return its old value
 */
extern Int ATM_cleari( volatile Int *dst );
extern Uns ATM_clearu( volatile Uns *dst );

/*
 *  ======== ATM_dec ========
 *  atomically decrement integer or unsigned and return new value
 */
extern Int ATM_deci( volatile Int *dst );
extern Uns ATM_decu( volatile Uns *dst );

/*
 *  ======== ATM_inc ========
 *  atomically increment integer or unsigned and return new value
 */
extern Int ATM_inci( volatile Int *dst );
extern Uns ATM_incu( volatile Uns *dst );

/*
 *  ======== ATM_or ========
 *  atomically or integer or unsigned with mask and return its old value
 */
extern Int ATM_ori( volatile Int *dst, Int mask );
extern Uns ATM_oru( volatile Uns *dst, Uns mask );

/*
 *  ======== ATM_set ========
 *  atomically set integer or unsigned and return its old value
 */
extern Int ATM_seti( volatile Int *dst, Int x );
extern Uns ATM_setu( volatile Uns *dst, Uns x );

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* ATM_ */
