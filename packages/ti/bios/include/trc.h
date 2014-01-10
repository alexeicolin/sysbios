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
 *  ======== trc.h ========
 *
 */

#ifndef TRC_
#define TRC_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== Trace Event Classes ========
 */
#define TRC_LOGSWI      0x0001
#define TRC_LOGPRD      0x0002
#define TRC_LOGCLK      0x0004

#define TRC_LOGTSK      0x0008          /* Reserved for SPOX */
#define TRC_PWRMINST    0x0010

#define TRC_STSSWI      0x0020
#define TRC_STSPRD      0x0040
#define TRC_STSPIP      0x0080
#define TRC_STSHWI      0x0100

#define TRC_STSTSK      0x0200          /* Reserved for SPOX */
#define TRC_STSSWO      0x0400          /* Reserved for SPOX */
#define TRC_USER0       0x0800
#define TRC_USER1       0x1000
#define TRC_USER2       0x2000
#define TRC_GBLTARG     0x4000          /* set/cleared by target */
#define TRC_GBLHOST     0x8000          /* set/cleared by host */

extern __FAR__ Uns TRC_R_mask;

/*
 *  ======== TRC_disable ========
 *  Clear the trace bits specified by mask.
 */
#define TRC_disable(mask) (TRC_R_mask = TRC_R_mask & ~(mask))

/*
 *  ======== TRC_enable ========
 *  Set the trace bits specified by mask.
 */
#define TRC_enable(mask) (TRC_R_mask = TRC_R_mask | (mask))

/*
 *  ======== TRC_query ========
 *  Returns 0 if all the trace bits specified by mask are set and both
 *  TRC_GBLTRG and TRC_GBLHOST are set; otherwise TRC_query returns a
 *  non-zero value.
 *
 *  For example,
 *      if (TRC_query(TRC_USER0 | TRC_USER1) == 0) {
 *          LOG_printf("both user0 and user1 trace is enabled");
 *      }
 *
 */
#define TRC_query(mask) \
        ( (((mask) | (TRC_GBLHOST | TRC_GBLTARG)) & TRC_R_mask)^ \
          ((mask) | (TRC_GBLHOST | TRC_GBLTARG)) )      

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* TRC_ */
