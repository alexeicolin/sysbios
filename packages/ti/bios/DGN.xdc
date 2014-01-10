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
 *  ======== DGN.xdc ========
 *  metaonly module to support the DSP/BIOS legacy DGN module.
 *
 */

/*!
 *  ======== DGN ========
 *  DGN Manager.
 *
 *  legacy module to support DSP/BIOS 5.x, see DSP/BIOS 5.x docs for more info
 */

@Template("./DGN.xdt")

metaonly module DGN
{
    /*!
     *  ======== close ========
     *  @_nodoc
     *  Function that ties everything up.
     */ 
    function close();
    
    /*! Create a static DGN device */
    create(String name);

instance:

    /*!  Name of the DGN device */
    config String name = "";
    
    /*!  Comment to describe the DGN device */
    config String comment = "<add comments here>";
    
    /*! Type of data stream produced */
    config String device = "user";
    
    /*! Use default parameters */
    config Bool useDefaultParam = 0;
    
    /*! Constant value */
    config Int constant = 1;
    
    /*! Seed value for number generator */
    config Int seedValue = 1;
    
    /*! Lowest value to be generated */
    config Int lowerLimit = -32767;
    
    /*! Highest value to be generated */
    config Int upperLimit = 32767;
    
    /*! Amplitutde scaling factor */
    config Int gain = 32767;
    
    /*! Frequency of generated sine wave */
    config Int frequency = 1;
    
    /*! Phase of generated sine wave */
    config Int phase = 0;
    
    /*! Sample rate of generated sine wave */
    config Int rate = 256;
    
    /*! Function to be used. */
    config String fxn = null;
    
    /*! Argument for the function */
    config UArg arg = 0;
    
    /*!
     *  ======== paramsType ========
     *  @_nodoc
     *  Used internally when creating DGN.
     */ 
    config String paramsType = null;
}

