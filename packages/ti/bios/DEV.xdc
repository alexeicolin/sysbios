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
 *  ======== DEV.xdc ========
 *  metaonly module to support the DSP/BIOS legacy DEV module.
 *
 */

/*!
 *  ======== DEV ========
 *  DEV Manager.
 *
 *  This module is creates static devices. It is called by
 *  the higher level driver modules: UDEV, DGN, DIO, and DPI.
 *
 *  legacy module to support DSP/BIOS 5.x, see DSP/BIOS 5.x docs for more info
 */
 
@Template("./DEV.xdt")

metaonly module DEV
{   
    /*!
     *  ======== close ========
     *  @_nodoc
     *  Function that ties everything up.
     */ 
    function close();
        
    /*!
     *  ======== create ========
     *  Create a static device
     */
    create(String name);
    
instance:

    /*!
     *  ======== name ========
     *  Name of the device
     */
    config String name = "";

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
     *  ======== paramsType ========
     *  @_nodoc
     *  Type of the params
     *
     *  Used to type the params if it is an internal DSP/BIOS struct.
     *
     *  The default is "Ptr".
     */
    config String paramsType = "Ptr";
    
    /*!
     *  ======== includePath ========
     *  @_nodoc
     *  Include path to be inserted into generated .c file
     *
     *  Needed if the paramsType field is needs an include file.
     *
     *  The default is NULL.
     */
    config String includePath = null;

    /*!
     *  ======== deviceGlobalDataPtr ========
     *  Global device data pointer
     *
     *  The purpose of such a value is driver-specific.
     *
     *  The default is NULL.
     */
    config Ptr deviceGlobalDataPtr = 0;
}

