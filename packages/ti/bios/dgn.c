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
 *  ======== dgn.c ========
 *  Streaming Generator Driver
 *
 *  A DGN device is a "pseudo-device" that generates one of several
 *  possible data streams.  For example, a DGN can generate sin/cos
 *  series or white noise.  This driver can be very useful for testing
 *  applications that require an input stream of data.
 */


#include <ti/bios/include/std.h>
#include <stddef.h>

#include <ti/bios/include/dev.h>
#include <ti/bios/include/mem.h>
#include <ti/bios/include/que.h>
#include <ti/bios/include/sys.h>

#include <ti/bios/include/dgn.h>

#define MAXRANGE        ((1U << (16 - 1)) - 1)

__FAR__ DGN_Params DGN_PARAMS = {
    { 1 },                      /* constant: value */
    { 1, -MAXRANGE, MAXRANGE }, /* rand: seed, lower, upper */
    { MAXRANGE, 1, 0, 256 },    /* sine: gain, freq, phase, sample rate */
    { NULL, NULL }              /* user: fxn, arg */
};

Int DGN_close(DEV_Handle device);
Int DGN_idle(DEV_Handle device, Bool flush);
Int DGN_open(DEV_Handle device, String name);
Int DGN_ioFunc(DEV_Handle device);

/*
 *  Driver function table.
 */
__FAR__ DEV_Fxns DGN_FXNS = {
    DGN_close,          /* close */
    DEV_CTRL,           /* ctrl */
    DGN_idle,           /* idle */
    DGN_ioFunc,         /* issue */
    DGN_open,           /* open */
    DEV_READY,          /* ready */     /* generator is always ready */
    (DEV_Treclaim)SYS_zero,     /* reclaim */
};

/*
 *  ======== DGN_close ========
 *  All that needs to be done to close a generator is to
 *  free memory associated with the driver.  And return
 *  a status when done.
 */
Int DGN_close(DEV_Handle device)
{
    MEM_free(0, device->object, sizeof(DGN_GenObj));
    
    return (SYS_OK);
}

/*
 *  ======== DGN_idle ========
 *  For DGN idling, we don't need to do any queue manipulation since
 *  there's no way for frames to build up on a queue (they always get
 *  processed).
 *  All we need to do is reset state variables.
 */
Int DGN_idle(DEV_Handle device, Bool flush)
{
    DGN_GenObj  *gen = (DGN_GenObj *)device->object;

    gen->seed = gen->dparams->rand.seed;

    gen->index = 0;

    return (SYS_OK);
}

/*
 *  ======== DGN_ioFunc ========
 *  This routine removes a frame from the 'todevice' list, fills it
 *  with data, and moves it to the 'fromdevice' list.
 */
Int DGN_ioFunc(DEV_Handle device)
{
    DGN_GenObj  *gen;
    DEV_Frame   *frame;
    
    frame = QUE_get(device->todevice);
    gen = (DGN_GenObj *)device->object;

    (*gen->fxn)(gen, frame->addr, frame->size);

    QUE_put(device->fromdevice, frame);

    return (SYS_OK);
}

/*
 *  ======== DGN_open ========
 *  This routine creates a generator object that will be used
 *  when manipulating this device (ie. getting data, getting
 *  status, etc.).
 */
Int DGN_open(DEV_Handle device, String name)
{
    DGN_GenObj  *gen;
    Int         gain;
    Int         cnst;

    cnst = 0;
    
    /* allocate generator object */
    if ((gen = MEM_alloc(0, sizeof(DGN_GenObj), 0)) == MEM_ILLEGAL) {
        SYS_error("DGN", SYS_EALLOC);
        return (SYS_EALLOC);
    }

    gen->dparams = (device->params) ?
      (DGN_Params *)device->params : &DGN_PARAMS;

    /*
     * bug fix MR 4493
     * gain = gain - 1 to fix boundary case --
     * gain of 128 was yielding values between -256 and 256
     * gain of 16 gave -32 to 32
     * etc.
     */
    gain = gen->dparams->sine.gain - 1;
    while (gain > 0) {
        cnst++;
        gain >>= 1;
    }

    gen->shift = 16 - 1 - cnst;
    gen->step = (256 * gen->dparams->sine.freq) / gen->dparams->sine.rate;
    gen->index = 0;

    gen->seed = gen->dparams->rand.seed;

    gen->fxn = (Fxn)device->devid;

    device->object = (Ptr)gen;

    return (SYS_OK);
}

/*
 *  ======== DGN_user ========
 *  Fill the buffer whose address is "addr" with random values generated
 *  "size" times.  The random values are generated using a recursive
 *  equation.
 */
Void DGN_user(DGN_GenObj *gen, Ptr addr, size_t size)
{
    Arg         arg;
    Fxn         fxn;

    arg = gen->dparams->user.arg;
    fxn = gen->dparams->user.fxn;

    (*fxn)(arg, addr, size);
}

/*
 *  ======== DGN_printHex ========
 */
Void DGN_printHex(Arg arg, Ptr addr, size_t size)
{
    Int16       *buf = (Int16 *)addr;
    Int         tmp;
        
    size = size / sizeof(Int16);

    for (; size > 0; size--, buf++) {
        tmp = *buf & 0xffff;
        SYS_printf("0x%04x\n", tmp);
    }
}

/*
 *  ======== DGN_printInt ========
 */
Void DGN_printInt(Arg arg, Ptr addr, Uns size)
{
    Int16       *buf = (Int16 *)addr;
        
    size = size / sizeof(Int16);

    for (; size > 0; size--, buf++) {
        SYS_printf("%d\n", *buf);
    }
}

/*
 *  ======== DGN_iconst ========
 *  Fill the buffer whose address is "addr" with constant values generated
 *  "size" times.
 */
Void DGN_iconst(DGN_GenObj *gen, Int16 *addr, size_t size)
{
    size_t        i;

    for (i = (size / sizeof(Int16)); i > 0; i--) {
        *addr++ = gen->dparams->constant.value;
    }
}

/*
 *  ======== DGN_irand ========
 *  Fill the buffer whose address is "addr" with random values generated
 *  "size" times.  The random values are generated using a recursive
 *  equation.
 */
Void DGN_irand(DGN_GenObj *gen, Int16 *addr, size_t size)
{
    size_t         i;
    Uns         range;
    Uns         rnd;
    LgUns       next;

    range = gen->dparams->rand.upper - gen->dparams->rand.lower + 1;
    next = gen->seed;

    for (i = (size / sizeof(Int16)); i > 0; i--) {
        /*
         *      Random numbers are generated using a linear congruential
         *  psuedo random generator using the equation:
         *      f(n+1) = (a * f(n) + c) mod M,  for (n >= 0)
         *  to insure the cycle length to be M = 2^N for an N-bit
         *  binary two's complement machine:
         *      a mod 8 == 5
         *      c mod 2 == 1
         *  See Knuth Vol II Seminumerical Algorithms for theory.
         */
        next = next * 1103515245 + 12345;

        /*
         *  Note the top bits are "more random" than the bottom bits
         *  so to scale the number treat the top 16 bits as a binary
         *  fraction (from 0.0 - < 1.0) and multiply by the desired
         *  range and truncate the result.
         */
        rnd = (next >> 16);
        if (range) {
            rnd = (Uint16) (((LgUns) rnd * (LgUns) range) >> 16);
        }
        *addr++ = rnd + gen->dparams->rand.lower;
    }

    gen->seed = next;
}

/*
 *  ======== DGN_isine ========
 *  Fill the buffer whose address is "addr" with sine values generated
 *  "size" times.
 *
 *  NOTE:  If the sampling rate is not evenly divisible by the
 *         frequency, the generated output is not a sine wave
 *         but a sequence of a sine-like wave.
 */
Void DGN_isine(DGN_GenObj *gen, Int16 *addr, size_t size)
{
    size_t         i;
static __FAR__ Int16 sineTable[256] = {
    0x0000,0x0324,0x0647,0x096A,0x0C8B,0x0FAB,0x12C8,0x15E2,
    0x18F8,0x1C0B,0x1F19,0x2223,0x2528,0x2826,0x2B1F,0x2E11,
    0x30FB,0x33DE,0x36BA,0x398C,0x3C56,0x3F17,0x41CE,0x447A,
    0x471C,0x49B4,0x4C3F,0x4EBF,0x5133,0x539B,0x55F5,0x5842,
    0x5A82,0x5CB4,0x5ED7,0x60EC,0x62F2,0x64E8,0x66CF,0x68A6,
    0x6A6D,0x6C24,0x6DCA,0x6F5F,0x70E2,0x7255,0x73B5,0x7504,
    0x7641,0x776C,0x7884,0x798A,0x7A7D,0x7B5D,0x7C29,0x7CE3,
    0x7D8A,0x7E1D,0x7E9D,0x7F09,0x7F62,0x7FA7,0x7FD8,0x7FF6,
    0x7FFF,0x7FF6,0x7FD8,0x7FA7,0x7F62,0x7F09,0x7E9D,0x7E1D,
    0x7D8A,0x7CE3,0x7C29,0x7B5D,0x7A7D,0x798A,0x7884,0x776C,
    0x7641,0x7504,0x73B5,0x7255,0x70E2,0x6F5F,0x6DCA,0x6C24,
    0x6A6D,0x68A6,0x66CF,0x64E8,0x62F2,0x60EC,0x5ED7,0x5CB4,
    0x5A82,0x5842,0x55F5,0x539B,0x5133,0x4EBF,0x4C3F,0x49B4,
    0x471C,0x447A,0x41CE,0x3F17,0x3C56,0x398C,0x36BA,0x33DE,
    0x30FB,0x2E11,0x2B1F,0x2826,0x2528,0x2223,0x1F19,0x1C0B,
    0x18F8,0x15E2,0x12C8,0x0FAB,0x0C8B,0x096A,0x0647,0x0324,
    0x0000,0xFCDB,0xF9B8,0xF695,0xF374,0xF054,0xED37,0xEA1D,
    0xE707,0xE3F4,0xE0E6,0xDDDC,0xDAD7,0xD7D9,0xD4E0,0xD1EE,
    0xCF04,0xCC21,0xC945,0xC673,0xC3A9,0xC0E8,0xBE31,0xBB85,
    0xB8E3,0xB64B,0xB3C0,0xB140,0xAECC,0xAC64,0xAA0A,0xA7BD,
    0xA57D,0xA34B,0xA128,0x9F13,0x9D0D,0x9B17,0x9930,0x9759,
    0x9592,0x93DB,0x9235,0x90A0,0x8F1D,0x8DAA,0x8C4A,0x8AFB,
    0x89BE,0x8893,0x877B,0x8675,0x8582,0x84A2,0x83D6,0x831C,
    0x8275,0x81E2,0x8162,0x80F6,0x809D,0x8058,0x8027,0x8009,
    0x8000,0x8009,0x8027,0x8058,0x809D,0x80F6,0x8162,0x81E2,
    0x8275,0x831C,0x83D6,0x84A2,0x8582,0x8675,0x877B,0x8893,
    0x89BE,0x8AFB,0x8C4A,0x8DAA,0x8F1D,0x90A0,0x9235,0x93DB,
    0x9592,0x9759,0x9930,0x9B17,0x9D0D,0x9F13,0xA128,0xA34B,
    0xA57D,0xA7BD,0xAA0A,0xAC64,0xAECC,0xB140,0xB3C0,0xB64B,
    0xB8E3,0xBB85,0xBE31,0xC0E8,0xC3A9,0xC673,0xC945,0xCC21,
    0xCF04,0xD1EE,0xD4E0,0xD7D9,0xDAD7,0xDDDC,0xE0E6,0xE3F4,
    0xE707,0xEA1D,0xED37,0xF054,0xF374,0xF695,0xF9B8,0xFCDB,
};

    for (i = (size / sizeof(Int16)); i > 0; i--) {
        *addr++ = (Int16) sineTable[gen->index] >> (gen->shift); /* modified by gain */
        gen->index += gen->step;
        gen->index = gen->index & 0xff;                 /* count % 256 */
    }
}



