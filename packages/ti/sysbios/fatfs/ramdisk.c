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
 * ======== ramdisk.c ========
 * Simple RAM disk layer for FatFs.
 *
 */

#include <stdio.h>
#include <string.h>
#include <ffconf.h>
#include <diskio.h>

#include <ti/sysbios/fatfs/ff.h>

#define SECTORSIZE 512

static FATFS ramdisk_filesystems[_VOLUMES]; 

static unsigned char *diskMem[_VOLUMES] = { NULL, NULL, NULL, NULL };

static unsigned    numSectors[_VOLUMES] = { 0, 0, 0, 0 };

/*
 * ======== ramdisk_init ========
 */
DSTATUS ramdisk_init(BYTE drive)
{
    return RES_OK;
}

/*
 * ======== ramdisk_status ========
 */
DSTATUS ramdisk_status(BYTE drive)
{
    return RES_OK;
}

/*
 * ======== ramdisk_read ========
 *
 */
DRESULT ramdisk_read(BYTE drive, BYTE * buf, DWORD start, BYTE num)
{
    memcpy(buf, diskMem[drive] + start * SECTORSIZE, num * SECTORSIZE);

    return RES_OK;
}

/*
 * ======== ramdisk_write ========
 *
 */
#if     _READONLY == 0
DRESULT ramdisk_write(BYTE drive, const BYTE * buf, DWORD start, BYTE num)
{
    memcpy(diskMem[drive] + start * SECTORSIZE, buf, num * SECTORSIZE);

    return RES_OK;
}
#endif

/*
 * ======== ramdisk_ioctl ========
 *
 */
DRESULT ramdisk_ioctl(BYTE drive, BYTE cmd, void * buf)
{
    DRESULT result;

    switch (cmd) {
        case CTRL_SYNC:
            result = RES_OK;
            break;
            
        case GET_BLOCK_SIZE:
        case CTRL_ERASE_SECTOR:
            result = RES_PARERR;
            break;

        case GET_SECTOR_SIZE:
            *(WORD *)buf = SECTORSIZE;
            result = RES_OK;
            break;

        case GET_SECTOR_COUNT:
            *(DWORD *)buf = numSectors[drive];
            result = RES_OK;
            break;

        default:
            result = RES_ERROR;
            break;
    }

    return (result);
}

/*
 * ======== ramdisk_start ========
 *
 */
DRESULT ramdisk_start(BYTE drive, unsigned char *data, int numBytes, int mkfs)
{
    DRESULT result;

    /* ensure 'drive' is a valid index */
    if (drive >= _VOLUMES) {
        return RES_PARERR;
    }

    /* ensure this volume isn't already in use */
    if (diskMem[drive] != NULL) {
        return RES_PARERR;
    }

    /*
     *  Register the ramdisk functions with the diskio module.  FatFS will
     *  call these via the diskio function table.
     */
    if ((result = disk_register(drive, ramdisk_init, ramdisk_status,
            ramdisk_read, ramdisk_write, ramdisk_ioctl)) != RES_OK) {
        return result;
    }

    /* if creating a new ramdisk, zero out 'data[]' and call f_mkfs() */
    if (mkfs) {
        memset(data, 0, numBytes);
    }

    diskMem[drive]    = data;
    numSectors[drive] = numBytes / SECTORSIZE;

    /* mount the drive */
    if (f_mount(drive, &(ramdisk_filesystems[drive])) != FR_OK) {
        return RES_ERROR;
    }

    if (mkfs) {
        if (f_mkfs(drive, 0, 512) != FR_OK) {
            return RES_ERROR; 
        }
    }

    return RES_OK;
}

/*
 * ======== ramdisk_stop ========
 *
 */
DRESULT ramdisk_stop(BYTE drive)
{
    DRESULT result;

    /* ensure 'drive' is a valid index */
    if (drive >= _VOLUMES) {
        return RES_PARERR;
    }

    diskMem[drive]    = NULL;
    numSectors[drive] = 0;

    if ((result = disk_unregister(drive)) != RES_OK) {
        return result;
    }

    /* un-mount the drive */
    if (f_mount(drive, NULL) != FR_OK) {
        return RES_ERROR;
    }

    return RES_OK;
}

