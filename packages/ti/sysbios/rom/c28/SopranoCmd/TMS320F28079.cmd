/* --COPYRIGHT--,BSD
 * Copyright (c) $(CPYYEAR), Texas Instruments Incorporated
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
 * --/COPYRIGHT--*/
/*
 *  ======== TMS320F28069.cmd ========
 *  Define the memory block start/length for the F28069
 *
 *! Revision History
 *! ================
 *! 26-May-2011 jv  created to support c28 generic platform.
 */

/* 
 *  PAGE 0 will be used to organize program sections
 *  PAGE 1 will be used to organize data sections
 *
 *  Notes:
 *      Memory blocks on F2806x are uniform (ie same
 *      physical memory) in both PAGE 0 and PAGE 1.
 *      That is the same memory region should not be
 *      defined for both PAGE 0 and PAGE 1.
 *      Doing so will result in corruption of program
 *      and/or data.
 */

MEMORY
{
PAGE 0 :   /* Program Memory */

    BEGIN       : origin = 0x000000, length = 0x000002

    RESET       : origin = 0x3FFFC0, length = 0x000002

    IQTABLES    : origin = 0x3FE000, length = 0x000B50     /* IQ Math Tables in Boot ROM */
    IQTABLES2   : origin = 0x3FEB50, length = 0x00008C     /* IQ Math Tables in Boot ROM */
    IQTABLES3   : origin = 0x3FEBDC, length = 0x0000AA     /* IQ Math Tables in Boot ROM */

    FLASH       : origin = 0x080000, length = 0x040000     /* on-chip FLASH */
                                                         
    BOOTROM     : origin = 0x3FF27C, length = 0x000D44
    OTP         : origin = 0x078000, length = 0x000D00     /* on-chip OTP */

    FPUTABLES   : origin = 0x3FD21A, length = 0x0006A0

    ROM         : origin = 0x3F8002, length = 0x002000     /* Boot ROM */

PAGE 1 :   /* Data Memory */

    RAMM0       : origin = 0x000002, length = 0x0003FE
    RAMM1       : origin = 0x000400, length = 0x000400     /* on-chip RAM block M1 */

    RAMLS05D01  : origin = 0x008000, length = 0x004000

    RAMGS0GS3   : origin = 0x00C000, length = 0x004000     /* on-chip RAM block GS0-GS3  */
    RAMGS4GS15  : origin = 0x010000, length = 0x00C000     /* on-chip RAM block GS4-GS15 */

    PIEVECT     : origin = 0x000CE0, length = 0x000220
}

/*
 *  Allocate sections to memory blocks.
 *  Note:
 *      codestart   user defined section in DSP28_CodeStartBranch.asm
 *                  used to redirect code execution when booting to flash
 *
 *      ramfuncs    user defined section to store functions that will be
 *                  copied from Flash into RAM
 */ 

SECTIONS
{
    /* Allocate program areas: */
    .cinit              : > RAMGS4GS15            PAGE = 1
    .pinit              : > RAMGS4GS15            PAGE = 1
    .text               : > RAMGS4GS15            PAGE = 1
    codestart           : > BEGIN                 PAGE = 0
    ramfuncs            : LOAD = FLASH            PAGE = 0,
                          RUN  = RAMGS4GS15       PAGE = 1,
                          LOAD_START(_RamfuncsLoadStart),
                          LOAD_SIZE(_RamfuncsLoadSize),
                          LOAD_END(_RamfuncsLoadEnd),
                          RUN_START(_RamfuncsRunStart)

    /* Allocate uninitalized data sections: */
    /* Note: SP is 16-bit and therefore the stack should be < 0xFFFF */
    .stack              : >  RAMLS05D01             PAGE = 1
    .ebss               : >> RAMLS05D01 | RAMGS0GS3 PAGE = 1
    .esysmem            : >  RAMGS4GS15             PAGE = 1
    .cio                : >  RAMGS4GS15             PAGE = 1

    /* Initalized sections go in Flash */
    /* For SDFlash to program these, they must be allocated to page 0 */
    .econst             : > RAMGS4GS15            PAGE = 1
    .switch             : > RAMGS4GS15            PAGE = 1
    .args               : > RAMGS4GS15            PAGE = 1

    /* Allocate IQ math areas: */
    IQmath              : > RAMGS4GS15            PAGE = 1    /* Math Code */
    IQmathTables        : > IQTABLES              PAGE = 0, TYPE = NOLOAD

    /* Allocate FPU math areas: */
    FPUmathTables       : > FPUTABLES             PAGE = 0, TYPE = NOLOAD

    /*
     *  Uncomment the section below if calling the IQNexp() or IQexp()
     *  functions from the IQMath.lib library in order to utilize the 
     *  relevant IQ Math table in Boot ROM (This saves space and Boot ROM 
     *  is 1 wait-state). If this section is not uncommented, IQmathTables2
     *  will be loaded into other memory (SARAM, Flash, etc.) and will take
     *  up space, but 0 wait-state is possible.
     */
    /*
    IQmathTables2       : > IQTABLES2   PAGE = 0, TYPE = NOLOAD
    {
        IQmath.lib<IQNexpTable.obj> (IQmathTablesRam)
    }
    */

    /*
     *  Uncomment the section below if calling the IQNasin() or IQasin()
     *  functions from the IQMath.lib library in order to utilize the
     *  relevant IQ Math table in Boot ROM (This saves space and Boot ROM
     *  is 1 wait-state). If this section is not uncommented, IQmathTables2
     *  will be loaded into other memory (SARAM, Flash, etc.) and will take
     *  up space, but 0 wait-state is possible.
     */
    /*
    IQmathTables3       : > IQTABLES3   PAGE = 0, TYPE = NOLOAD
    {
        IQmath.lib<IQNasinTable.obj> (IQmathTablesRam)
    }
    */
}
