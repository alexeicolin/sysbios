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
 *  ======== TSK.xdc ========
 *  metaonly module to support the DSP/BIOS legacy TSK module.
 *
 */

/*!
 *  ======== TSK ========
 *  TSK module
 *
 *  legacy module to support DSP/BIOS 5.x, see DSP/BIOS 5.x docs for more info
 */

@Template("./TSK.xdt")
metaonly module TSK
{
    /*! Enable tasks in the system */
    config Bool ENABLETSK = true;

    /*! The memory segment that contains the TSK objects */
    config MEM.Instance OBJMEMSEG = null;

    /*! The default size of the stack used by tasks */
    config UInt STACKSIZE;

    /*! Stack segment for dynamic tasks */
    config MEM.Instance STACKSEG = null;

    /*! The default priority level for tasks that are created dynamically */
    config Int PRIORITY = 1;

    /*! This property is no longer supported */
    config String DRIVETSKTICK;

    /*! Function to call when any task is created */
    config String CREATEFXN = "FXN_F_nop";

    /*! Function to call when any task is deleted */
    config String DELETEFXN = "FXN_F_nop";

    /*! Function to call when any task exits */
    config String EXITFXN = "FXN_F_nop";

    /*! This property is no longer supported */
    config Bool CALLSWITCHFXN;

    /*! This property is no longer supported */
    config String SWITCHFXN;

    /*! Call ready function when any task becomes ready to run */
    config Bool CALLREADYFXN = false;

    /*! Function to call when any tasks becomes ready to run */
    config String READYFXN = "FXN_F_nop";
    
    /*!
     *  ======== close ========
     *  @_nodoc
     *  
     */
    function close();

    /*!
     *  ======== exportMod ========
     *  @_nodoc
     *  function that exports the modules needed
     */
    function exportMod();

    /*! function to create an instance */
    create(String name);

instance:

    /*! Specify the name of the object */
    config String name = "";

    /*! Type a comment to identify this object */
    config String comment = "<add comments here>";

    /*! Automatically allocate task's private stack space */
    config Bool autoAllocateStack = true;

    /*! This property is no longer supported */
    config String manualStack;

    /*! Size of the stack to allocate for this task */
    config Int stackSize = 0;

    /*! Specify the memory segment to contain the stack space for this task */
    config MEM.Instance stackMemSeg = null;

    /*! The priority level for this task */
    config Int priority;

    /*! Function to be executed when task runs */
    config String fxn = null;

    /*! First argument pass to the task function */
    config UArg arg0 = 0;

    /*! Second argument pass to the task function */
    config UArg arg1 = 0;

    /*! Third argument pass to the task function */
    config UArg arg2 = 0;

    /*! Fourth argument pass to the task function */
    config UArg arg3 = 0;

    /*! Fifth argument pass to the task function */
    config UArg arg4 = 0;

    /*! Sixth argument pass to the task function */
    config UArg arg5 = 0;

    /*! Seventh argument pass to the task function */
    config UArg arg6 = 0;

    /*! Eighth argument pass to the task function */
    config UArg arg7 = 0;

    /*! A pointer to a globally-defined data structure */
    config UArg envPointer = null;

    /*! Don't shut down system while this task is still running */
    /*! Second argument pass to the task function */
    config Bool exitFlag = true;

    /*! Set this to true if you want the task name allocated on the target  */
    config Bool allocateTaskName = false;

    /*! This property is no longer supported */
    config Int order;

    /*!
     *  ======== destroy ========
     *  @_nodoc
     *  function is just a place holder for now.
     */
    Void destroy();
}
