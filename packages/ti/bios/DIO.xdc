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
 *  ======== DIO.xdc ========
 *  metaonly module to support the DSP/BIOS legacy DIO module.
 *
 */

/*!
 *  ======== DIO ========
 *  DIO Manager.
 *
 *  legacy module to support DSP/BIOS 5.x, see DSP/BIOS 5.x docs for more info
 */

@Template("./DIO.xdt")

metaonly module DIO
{
    /*!
     *  ======== close ========
     *  @_nodoc
     */
    function close();
    
    /*! Create a static DIO device */
    create(String name);
    
    /*! Set this to true if you want DIO objects to be created statically. */
    config Bool STATICCREATE = false;
        
    /*! Enter the memory segment from which to allocate DIO objects. */
    config Ptr OBJMEMSEG = null;

instance:        
    /*! Name of the DIO device. */
    config String name = null;
    
    /*! Type a comment to identify this object. */
    config String comment = null;
    
    /*! Set this to true if you want to use DIO with a callback function. */
    config Bool useCallBackFxn = false;
    
    /*! Name of the device to use with this DIO object. */
    config Ptr deviceName = null;
    
    /*! Lets you pass an optional argument to mini-driver create function. */
    config Ptr chanParams = null;    
}

