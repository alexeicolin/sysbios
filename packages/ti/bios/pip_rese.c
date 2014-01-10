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
 *  ======== pip_rese.c ========
 */

#include <ti/bios/include/std.h>
#include <ti/bios/include/pip.h>

static Ptr getDescTabBeg(Ptr desc);

/*
 *  ======== PIP_reset ========
 *  Reset all fields of a pipe object to their original values
 *  
 *  Calling constraints:
 *      1. PIP_reset should not be called between the PIP_alloc call and
 *         the PIP_put call and should not be called between the PIP_get call
 *         and the PIP_free call.
 *
 *      2. PIP_reset should be called when interrupts are disabled to avoid
 *         the race condition.
 */

Void PIP_reset(PIP_Handle pipe)
{
    Uns threshold;
    Uns rnf, wnf;
    Ptr desc, descTabBeg;

    threshold = PIP_getThreshold(pipe);

    PIP_setReaderAddr(pipe, 0); 
    PIP_setReaderSize(pipe, threshold);
    desc = PIP_getReaderCurdesc(pipe);
    descTabBeg = getDescTabBeg(desc);
    PIP_setReaderCurdesc(pipe, descTabBeg);
    rnf = PIP_getReaderNumFrames(pipe);
    PIP_setReaderNumFrames(pipe, 0);

    PIP_setWriterAddr(pipe, 0); 
    PIP_setWriterSize(pipe, threshold);
    PIP_setWriterCurdesc(pipe, descTabBeg);
    wnf = PIP_getWriterNumFrames(pipe);
    wnf = rnf + wnf;
    PIP_setWriterNumFrames(pipe, wnf);
}

static Ptr getDescTabBeg(Ptr desc)
{
    PIP_Curdesc *p, *next;
    Ptr retval;

    p = (PIP_Curdesc *)desc;
    next = p->next;

    while (next > p) {
        next = next->next;
    }
        
    retval = (Ptr)next;
    return(retval);
}



