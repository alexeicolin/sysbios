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
 *  ======== pip_peek.c ========
 */

#include <ti/bios/include/std.h>
#include <ti/bios/include/pip.h>

/*
 *  ======== PIP_peek ========
 *  Gets the pipe frame size and address without actually claiming
 *  the pipe frame
 *
 */

Int PIP_peek(PIP_Handle pipe, Ptr *addr, Uns rw)
{
    Int retval = -1;
    Uns nf, size;
    PIP_Curdesc *desc;

    if (rw == PIP_READER) {
        nf = PIP_getReaderNumFrames(pipe);
    }
    else {
        nf = PIP_getWriterNumFrames(pipe);
    }

    if (nf != 0) {
        if (rw == PIP_READER) {
            desc = (PIP_Curdesc *)PIP_getReaderCurdesc(pipe);
            if (PIP_getReaderAddr(pipe) == desc->addr) {
                /* PIP_get has been called, so get next desc */
                desc = desc->next;
            } 
        }
        else {
            desc = (PIP_Curdesc *)PIP_getWriterCurdesc(pipe);
            if (PIP_getWriterAddr(pipe) == desc->addr) {
                /* PIP_alloc has been called, so get next desc */
                desc = desc->next;
            } 
        }

        *addr = PIP_getAddrInDesc(desc);
        size = PIP_getSizeInDesc(desc);
        retval = (Int)size;
    }
 
    return(retval);
}



