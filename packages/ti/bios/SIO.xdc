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
 *  ======== SIO.xdc ========
 *  metaonly module to support the DSP/BIOS legacy SIO module.
 *
 */

/*!
 *  ======== SIO ========
 *  SIO Manager.
 *
 *  This module is creates SIO devices. It currently does not 
 *  support this functionality.
 *
 *  legacy module to support DSP/BIOS 5.x, see DSP/BIOS 5.x docs for more info
 */
 
metaonly module SIO
{
    function close();
    
    /*! Static SIO objects not supported */  
    create(String name);
    
    config Bool USEISSUERECLAIM = false;
    
    /*! Static SIO objects not supported */  
    config Ptr OBJMEMSEG = null;

instance:      
    /*! Static SIO objects not supported */  
    config String name = null;
    
    /*! Static SIO objects not supported */  
    config String comment = null;
    
    /*! Static SIO objects not supported */  
    config Ptr deviceName = null;
    
    /*! Static SIO objects not supported */  
    config String controlParameter = null;
    
    /*! Static SIO objects not supported */  
    config String mode = null;
    
    /*! Static SIO objects not supported */  
    config String bufSize = null;
    
    /*! Static SIO objects not supported */  
    config String numBufs = null;
    
    /*! Static SIO objects not supported */  
    config String flush = null;
    
    /*! Static SIO objects not supported */  
    config String bufAlign = null;
    
    /*! Static SIO objects not supported */  
    config String bufSegId = null;
    
    /*! Static SIO objects not supported */  
    config String modelName = null;
    
    /*! Static SIO objects not supported */  
    config String allocStaticBuf = null;
    
    /*! Static SIO objects not supported */  
    config String timeout = null;
    
    /*! Static SIO objects not supported */  
    config String useCallBackFxn = null;
    
    /*! Static SIO objects not supported */  
    config String callBackFxn = null;
    
    /*! Static SIO objects not supported */  
    config String arg0 = null;
    
    /*! Static SIO objects not supported */  
    config String arg1 = null;
}

