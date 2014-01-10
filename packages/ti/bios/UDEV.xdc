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
 *  ======== UDEV.xdc ========
 *  metaonly module to support the DSP/BIOS legacy UDEV module.
 *
 */

/*!
 *  ======== UDEV ========
 *  UDEV Manager.
 *
 *  Used to create User defined devices.
 *
 *  legacy module to support DSP/BIOS 5.x, see DSP/BIOS 5.x docs for more info
 */

  
metaonly module UDEV
{
    /*!
     *  ======== close ========
     *  @_nodoc
     *  Function that ties everything up.
     */ 
    function close();
    
    /*!
     *  ======== get ========
     *  @_nodoc
     *  Used in tconf.package.xs to help with prog.get(name).
     */ 
    function get(name);
    
    /*!
     *  ======== create ========
     *  Create a static user device
     */
    create(String name);    

instance:

    /*!
     *  ======== name ========
     *  Name of the user device
     */
    config String name = "";
    
    /*!
     *  ======== comment ========
     *  Comment to describe the user device
     */
    config String comment = null;
    
    /*!
     *  ======== initFxn ========
     *  The function to run to initialize this device
     * 
     *  The default is NULL.
     */
    config String initFxn = null;
    
    /*!
     *  ======== fxnTable ========
     *  The name of the device functions table for the driver or mini-driver
     *
     *  The default is NULL.
     */
    config String fxnTable = null;
    
    /*!
     *  ======== fxnTableType ========
     *  Type of function table used by the driver to which this 
     *  device interfaces 
     *
     *  The options are "DEV_Fxns" or "IOM_Fxns".
     *
     *  The default is "DEV_Fxns".
     */
    config String fxnTableType = "DEV_Fxns";
    
    /*!
     *  ======== deviceId ========
     *  The device id
     *
     *  The purpose of such a value is driver-specific.
     *
     *  The default is NULL.
     */
    config Ptr deviceId = null;
    
    /*!
     *  ======== params ========
     *  Additional params to the device
     *
     *  The purpose of such a value is driver-specific.
     *
     *  The default is NULL.
     */
    config Ptr params = null;
    
    /*!
     *  ======== deviceGlobalDataPtr ========
     *  Global device data pointer
     *
     *  The purpose of such a value is driver-specific.
     *
     *  The default is NULL.
     */
    config Ptr deviceGlobalDataPtr = 0;

    /*!
     *  ======== stackingDevice ========
     *  @_nodoc
     *  Is the device stacking?  This field is not used anymore.
     *
     *  The default is false.
     */
    config Bool stackingDevice = false;
}

