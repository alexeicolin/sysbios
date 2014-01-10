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
 *  ======== fattest1.c ========
 *  Read a pre-existing FatFS RAM disk image which contains 2 text files. Mount
 *  this image, then open, read and print the contents of the text files.
 *
 */

#include <xdc/std.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

#include <stdio.h>

#define READ_BLOCK_SIZE 64

Void catTextFile(char *filename);
Void task(UArg arg0, UArg arg1);

/*
 *  ======== main ========
 */
Void main()
{
    Task_create(task, NULL, NULL);

    BIOS_start();
}

/*
 * ======== catTextFile ========
 *
 * cat the contents of a text file that exists on a RAM disk image.
 *
 * returns FR_OK on success, on failure it returns either the result returned by
 * the FatFS API which failed, or -1 if dynamic memory allocation failed.
 */
Void catTextFile(char *filename)
{
    unsigned char data[READ_BLOCK_SIZE];
    int           size;
    FILE         *fp;
    
    size = 0;
 
    /* open the ramdisk file for reading */
    fp = fopen(filename, "a+");
    if (fp == NULL) {
        printf("ERROR: could not open file %s\n", filename);
        System_exit(1);
    }

    do {
        /* read contents of the file into a buffer */
        size = fread(data, 1, sizeof(data), fp);

        /* output file contents */
        fwrite(data, 1, size, stdout);
    } while (size == sizeof(data));


    /* clean up */
    fclose(fp);
}


/*
 *  ======== task ========
 *  This task opens a text file that exists on a pre-created RAM disk.
 *
 *  The RAM disk, created with the mkimage utility, contains two text files:
 *
 *      - file1.txt
 *      - file2.txt
 *
 *  This task function opens the above two files and then prints out the
 *  contents of each file.
 */
Void task(UArg arg0, UArg arg1)
{
    catTextFile("fat:file1.txt");
    catTextFile("fat:file2.txt");
}


