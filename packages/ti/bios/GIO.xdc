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
 *  ======== GIO.xdc ========
 *  metaonly module to support the DSP/BIOS legacy GIO module.
 *
 */
 
/*!
 *  ======== GIO ========
 *  GIO Manager.
 *
 *  This module aids in the creation of GIO devices.
 *
 *  legacy module to support DSP/BIOS 5.x, see DSP/BIOS 5.x docs for more info
 */
 
@Template("./GIO.xdt")

metaonly module GIO
{
    /*!
     *  ======== close ========
     *  @_nodoc
     *  Function that ties everything up.
     */ 
    function close();    
    
    /*! Set this property to true to enable use of the GIO module. */
    config Bool ENABLEGIO = false;

    /*! The function the GIO module should use to create a sync object. */
    config String CREATEFXN = "SEM_create";
    
    /*! The function the GIO module should use to delete the sync object. */
    config String DELETEFXN = "SEM_delete";
    
    /*! The function the GIO module should use to pend on a sync object. */
    config String PENDFXN = "SEM_pend";
    
    /*! The function the GIO module should use to post a sync object. */
    config String POSTFXN = "SEM_post";
}

