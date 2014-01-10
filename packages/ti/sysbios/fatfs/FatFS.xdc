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
 *  ======== FatFS.xdc ========
 */

/*!
 *  ======== FatFS ========
 *  Configuration and integration of the open-source FAT filesystem project,
 *  FatFs, with SYS/BIOS.
 *
 *  The FatFs project homepage: http://elm-chan.org/fsw/ff/00index_e.html
 *
 *  This initial integration supports (only) a simple RAM disk. The FatFs source
 *  is included and pre-built, together with support for SYS/BIOS
 *  synchronization. Also included is an implementation of the "device I/O"
 *  model for the TI compiler C runtime support (RTS) library. This provides
 *  transparent support for using the C stdio.h library functions (fopen,
 *  fwrite, fclose) instead of the raw, project-specific, API functions
 *  provided by FatFs (f_open, f_write).
 *
 *  The configuration options initially defined here are subject to change
 *  as more device driver support is added, such as MMC/SD, SATA, and USB.
 *
 *  To incorporate the FatFs functionality into an application, simply "use"
 *  this module in a SYS/BIOS configuration. This will automatically generate
 *  a startup function to "plug in" the filesystem into the TI RTS, and provide
 *  the needed libraries for linking. xgconf users can simply right-click on
 *  the module icon and select "Use Module". Script users simply add a
 *  xdc.useModule("ti.sysbios.fatfs.FatFS") to their .cfg file.
 *
 *  There are currently three configuration options, one related to the C
 *  RTS stdio implementation, and two for the ramdisk support. First, the
 *  fatfsPrefix parameter is used as the namespace "prefix" for the C RTS
 *  fopen() function. See the TI C Compiler User's Guide (such as spru187)
 *  for more details. Basically, this prefix must be supplied as part of the
 *  filename when calling fopen(). For example, with the default value of
 *  "fat", an application would use:
 *
 *  @p(code)
 *  FILE * fp = fopen("fat:/signal.dat", "w");
 *  @p
 *
 *  Note the ":" as a separator. This must be part of the fopen() call, but
 *  is not specified as part of the fatfsPrefix configuration parameter.
 *
 *  The other parameters control how the RAM disk is configured into the
 *  system. The ramdiskSymbolName parameter can be used to specify an
 *  address of a pre-initialized (formatted) filesystem. This would be
 *  typically provided by linking in a (large) C data array. When using this
 *  parameter, the other configuration parameter, ramdiskSize, should be set
 *  to the size of the data array in units of bytes.
 *
 *  For example, assume a FAT filesystem that has been "serialized"
 *  into the file ramdisk.c that contains the following:
 *
 *  @p(code)
 *  unsigned int ramdiskData[32768] = {
 *  @p
 *
 *  then the values for these two configuration parameters should be:
 *
 *  @p(code)
 *  var FatFS = xdc.useModule("ti.sysbios.fatfs.FatFS");
 *  FatFS.ramdiskSymbolName = "ramdiskData";
 *  FatFS.ramdiskSize = 32768 * 4;
 *  @p
 *
 *  When the ramdiskSymbolName parameter is left "empty", .ie., "",
 *  then the ramdiskSize
 *  parameter is used to create an empty FAT filesystem at boot time of size
 *  ramdiskSize.
 *
 *  User Defined Hook For get_fattime()
 *
 *  The proper implementation of the get_fattime() function requires hardware
 *  for a real time clock.  Since not all TI hardware platforms support a real
 *  time clock, the get_fattime() function will by default return a hard coded
 *  time value.  However, it is possible to override this by defining a hook
 *  function instead that can be written to take advantage of a real time clock
 *  that's present.  This function will then be called from get_fattime().
 *
 *  A hook function must be defined using the following format:
 *
 *  @p(code)
 *      Int32 <functionName>(Void)
 *  @p
 *
 *  Note if the user specifies a hook function in the RTSC configuration
 *  script, but does not define this function in their C code, a linker error
 *  will result.

 */

@Template("./FatFS.xdt")
metaonly module FatFS
{
    /*! Function signature for user defined get_fattime() hook function */
    typedef Int32 (*fatTimeHookFxn)();

    /*!
     *  User defined hook function to return the time from a real time clock 
     *
     *  By default, a hard coded time is returned from get_fattime(). If the
     *  user's hardware supports a real time clock, then the code to read the
     *  time from that hardware component can be implemented in a user defined
     *  function hook function and set to this configuration parameter.  This
     *  hook function will then be called from the get_fattime() function.
     *
     *  As an example, to configure a hook function called "myTimeFxn", you
     *  would first need to implement "myTimeFxn" in C to get the time from
     *  the real time clock on your hardware.  Then, in your RTSC configuration
     *  script, set "getFatTimeHook" to point to "myTimeFxn":
     *
     *  @p(code)
     *  var FatFS = xdc.useModule('ti.sysbios.fatfs.FatFS");
     *  FatFS.getFatTimeHook = '&myTimeFxn';
     *  @p
     *
     *  Note if the user specifies a hook function in the RTSC configuration
     *  script, but does not define this function in their C code, a linker error
     *  will result.
     */
    config fatTimeHookFxn getFatTimeHook = null;

    /*!
     * File name prefix for this filesystem for use with TI C RTS
     *
     * This prefix should be supplied as part of the filename passed when
     * calling fopen().  For example: 
     *  @p(code)
     *  FILE * fp = fopen("fat:/signal.dat", "w");
     *  @p
     */
    config String fatfsPrefix = "fat";
    
    /*!
     * Symbol name or address of a pre-existing FAT File System.
     *
     * The ramdisk allows for the loading of an existing file system image
     * stored in an C array or at a particular address.  The value of this
     * parameter should be the name of this array or the address of where the
     * file system image is located. If left empty, an empty filesystem will be
     * created at boot time.
     */
    config String ramdiskSymbolName = "";

    /*! Size of the FAT filesystem in bytes. */
    config UInt ramdiskSize = 512 * 256;

    /*!
     * Disable the ramdisk or select the drive ID for a ramdisk (-1 to disable).
     *
     * Parameter used to specify whether to create a ramdisk or not, and if so,
     * specify the drive ID to use for the ramdisk.  Valid ID numbers are in the
     * set [0, 1, 2, 3].  If using a ramdisk in combination with other media,
     * the user is responsible for managing these drive IDs so that they do not
     * conflict with one another.
     *
     * By default, no ramdisk is created.
     *
     * -1 : do not create a ramdisk
     *  0 : create a ramdisk with drive ID 0
     *  1 : create a ramdisk with drive ID 1
     *  2 : create a ramdisk with drive ID 2
     *  3 : create a ramdisk with drive ID 3
     */
    config Int ramdiskDriveId = -1;
}
