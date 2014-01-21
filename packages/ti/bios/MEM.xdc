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
 *  ======== MEM.xdc ========
 *
 */
import ti.sysbios.heaps.HeapMem;

/*!
 *  ======== MEM ========
 *  MEM module
 *
 *  legacy module to support DSP/BIOS 5.x, see DSP/BIOS 5.x docs for more info
 */

@Template("./MEM.xdt")
metaonly module MEM
{
    /*!
     *  ======== HeapList ========
     *  @_nodoc          
     */
    metaonly struct HeapList {
        String   heapLabel;
        Int      heapMemIndex;
    }
    
    /*! If true, it will disable ability to allocate memory dynamically */
    config Bool NOMEMORYHEAPS = false;

    /*! Memory segment for malloc() and free() */
    config MEM.Instance MALLOCSEG;

    /*! Memory segment for DSP/BIOS objects created at run-time with XXX_create */
    config MEM.Instance BIOSOBJSEG;

    /*! The size allocated for command-line args */
    config UInt ARGSSIZE = 8;

    /*! This property is no longer supported */
    config MEM.Instance ARGSSEG;

    /*! The size of the global stack in MADUs */
    config UInt STACKSIZE = 0x400;

    /*! This property is no longer supported */
    config MEM.Instance STACKSEG;

    /*! This property is no longer supported */
    config MEM.Instance GBLINITSEG;

    /*! This property is no longer supported */
    config MEM.Instance TRCDATASEG;

    /*! This property is no longer supported */
    config MEM.Instance SYSDATASEG;

    /*! This property is no longer supported */
    config MEM.Instance OBJSEG;

    /*! This property is no longer supported */
    config MEM.Instance BIOSSEG;

    /*! This property is no longer supported */
    config MEM.Instance SYSINITSEG;

    /*! This property is no longer supported */
    config MEM.Instance HWISEG;

    /*! This property is no longer supported */
    config MEM.Instance HWIVECSEG;

    /*! This property is no longer supported */
    config MEM.Instance RTDXTEXTSEG;

    /*! This property is no longer supported */
    config Bool USERCOMMANDFILE = false;
    
    /*! This property is no longer supported */
    config MEM.Instance TEXTSEG;

    /*! This property is no longer supported */
    config MEM.Instance DATASEG;

    /*! This property is no longer supported */
    config MEM.Instance SWITCHSEG;

    /*! This property is no longer supported */
    config MEM.Instance BSSSEG;

    /*! This property is no longer supported */
    config MEM.Instance EBSSSEG;

    /*! This property is no longer supported */
    config MEM.Instance FARSEG;

    /*! This property is no longer supported */
    config MEM.Instance CINITSEG;

    /*! This property is no longer supported */
    config MEM.Instance PINITSEG;

    /*! This property is no longer supported */
    config MEM.Instance CONSTSEG;

    /*! This property is no longer supported */
    config MEM.Instance ECONSTSEG;

    /*! This property is no longer supported */
    config MEM.Instance CIOSEG;

    /*! This property is no longer supported */
    config Bool ENABLELOADADDR = false;

    /*! This property is no longer supported */
    config MEM.Instance LOADBIOSSEG;

    /*! This property is no longer supported */
    config MEM.Instance LOADSYSINITSEG;

    /*! This property is no longer supported */
    config MEM.Instance LOADGBLINITSEG;

    /*! This property is no longer supported  */
    config MEM.Instance LOADTRCDATASEG;

    /*! This property is no longer supported */
    config MEM.Instance LOADTEXTSEG;

    /*! This property is no longer supported */
    config MEM.Instance LOADSWITCHSEG;

    /*! This property is no longer supported */
    config MEM.Instance LOADCINITSEG;

    /*! This property is no longer supported */
    config MEM.Instance LOADPINITSEG;

    /*! This property is no longer supported */
    config MEM.Instance LOADCONSTSEG;

    /*! This property is no longer supported */
    config MEM.Instance LOADECONSTSEG;

    /*! This property is no longer supported */
    config MEM.Instance LOADHWISEG;

    /*! This property is no longer supported */
    config MEM.Instance LOADHWIVECSEG;

    /*! This property is no longer supported */
    config MEM.Instance LOADRTDXTEXTSEG;

    /*! 
     *  List of "other" HeapMem instances who requires a segId 
     *
     *  When a HeapMem instance is created by a call to HeapMem.create()
     *  and when this HeapMem instance needs to be used by legacy code
     *  calling MEM_alloc() it needs a segment Id.
     *  Adding the HeapMem instance by calling this function allows MEM 
     *  to assign it a segId.
     */
    metaonly MEM.Instance addHeapMem(HeapMem.Handle handle, String name);

    /*! create an instance */
    create(String name);

    /*!
     *  ======== getHeapHandle ========
     *  @_nodoc
     *  function returns a Heap
     */
    xdc.runtime.IHeap.Handle getHeapHandle(String segment);

    /*!
     *  ======== inst ========
     *  @_nodoc
     *  function that creates MEM instances
     */
    Instance inst(String name);

    /*!
     *  ======== close ========
     *  @_nodoc
     *  function that processes the data
     */
    function close();
    
    /*!
     *  ======== currentNumHeapMems ========
     *  @_nodoc     
     *  Use internally to keep track of current of HeapMems created before we
     *  start making ones that map to the static MEM instances.
     */
    config Int currentNumHeapMems = 0;
        
    /*!
     *  ======== heapList ========
     *  @_nodoc     
     *  Array of heaps sorted by heapId to be used in the 
     *  template.
     */
    config HeapList heapList[];

instance:
    
    /*!
     *  ======== destroy ========
     *  @_nodoc
     *  function makes sure heap is not being created.
     */
    Void destroy();
    
    /*!
     *  ======== loadRunHandling ========
     *  @_nodoc     
     */
    Void loadRunHandling(MEM.Instance load, String loadStr, 
                         MEM.Instance run, String runStr, String sectionName);

    /*! Use to save the name of the object */
    config String name = "";

    /*! Type a comment to identify this object */
    config String comment = "<add comments here>";

    /*! Use as place holder only, property no longer supported */
    config UInt base = 0x00000000;

    /*! Use as place holder only, property no longer supported */
    config UInt len = 0x00000000;

    /*! Create a heap in this memory segment */
    config Bool createHeap = false;
    
    /*! The size of the heap in MADUs to be created in this memory segment */
    config UInt heapSize = 0x08000;

    /*! Enter a user defined heap identifier */
    config Bool enableHeapLabel = false;

    /*! Heap identifier label */
    config String heapLabel = null;

    /*! Use as place holder only, property no longer supported */
    config String space = "data";
    
    /*!
     *  ======== heapId ========
     *  @_nodoc     
     *  Use internally to keep track of heap segment id
     */
    config UInt heapId;    

    /*!
     *  ======== heapMemInst ========
     *  @_nodoc     
     *  Use internally to keep track of associated heapMemInst
     */
    config HeapMem.Handle heapMemInst;    
}

