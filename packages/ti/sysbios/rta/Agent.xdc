/*
 * Copyright (c) 2013, Texas Instruments Incorporated
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
 *  ======== Agent.xdc ========
 *
 */

package ti.sysbios.rta;

import xdc.rov.ViewInfo;
import xdc.runtime.LoggerBuf;
import xdc.runtime.Log;
import xdc.runtime.Assert;
import xdc.runtime.Error;
import xdc.runtime.Diags;
import ti.sysbios.knl.Clock;
import ti.sysbios.knl.Event;

/*! 
 *  ======== Agent ========
 *
 *  This module is deprecated and will not be supported in a future release.
 *  Please use the UIA product for the equivalent functionality.
 *
 *  The Agent module provides target support for the CCS RTA tools.
 *
 *  The Agent module includes configuration parameters to help set up the
 *  SYS/BIOS logging on the target.
 *  
 *  Earlier versions of the Agent provided support for transfer of data
 *  during runtime.   Runtime support has been removed and
 *  the Agent now only supports "stop mode" updates.   Log data is stored
 *  in buffers in RAM and only updated on the host with the target is halted.
 *
 *  Runtime support is now handled via the System Analyzer and UIA products.
 */
metaonly module Agent
{
    /*!
     *  ======== loadLogging ========
     *  Enable the Load module event logging.
     *
     *  If this is false, the events will be disabled and cannot be enabled
     *  at runtime (ALWAYS_OFF).
     *  If this is true, the events will be enabled and can also be modified
     *  at runtime (RUNTIME_ON).
     */
    metaonly config Bool loadLogging = true;
    
    /*!
     *  ======== loadLoggingRuntimeControl ========
     *  Specify whether load logging can be enabled / disabled at runtime.
     *
     *  This determines what diags settings are applied to the module's diags
     *  mask. If 'false', the diags bits will be configured as ALWAYS_OFF or 
     *  ALWAYS_ON, meaning they can't be changed at runtime. If 'true', the 
     *  bits will be configured as 'RUNTIME_OFF' or 'RUNTIME_ON'.
     */
    metaonly config Bool loadLoggingRuntimeControl = true;
    
    /*!
     *  ======== loadLoggerSize ========
     *  Size (in MAUs) of logger used for the Load module Log events.
     *
     *  To specify the size in number of records, instead of MAUs, use the
     *  following code:
     *  
     *  @p(code)
     *  var Agent = xdc.useModule('ti.sysbios.rta.Agent');
     *  var LoggerBuf = xdc.useModule('xdc.runtime.LoggerBuf');
     *  
     *  // Set the load logger size to 50 records.
     *  Agent.loadLoggerSize = 50 * LoggerBuf.Entry.$sizeof();
     *
     *  // Print the final size to see how much memory is used.
     *  print("Load Logger Size (MAUs): " + Agent.loadLoggerSize);
     *  @p
     */
    metaonly config SizeT loadLoggerSize = 512;

    /*!
     *  ======== loadLoggerBufferSection ========
     *  Section to place the Load LoggerBuf's buffer.
     */
    metaonly config String loadLoggerBufferSection = "[Default]";
        
    /*!
     *  ======== mainLogging ========
     *  Enable main and non-XDC modules event logging.
     */
    metaonly config Bool mainLogging = true;
   
    /*!
     *  ======== mainLoggingRuntimeControl ========
     *  Specify whether main logging can be enabled / disabled at runtime.
     *
     *  This determines what diags settings are applied to the module's diags
     *  mask. If 'false', the diags bits will be configured as ALWAYS_OFF or 
     *  ALWAYS_ON, meaning they can't be changed at runtime. If 'true', the 
     *  bits will be configured as 'RUNTIME_OFF' or 'RUNTIME_ON'.
     */
    metaonly config Bool mainLoggingRuntimeControl = true;
    
    /*!
     *  ======== mainLoggerSize ========
     *  Size (in MAUs) of logger used for main and non-XDC modules Log events.
     *
     *  To specify the size in number of records, instead of MAUs, use the
     *  following code:
     *  
     *  @p(code)
     *  var Agent = xdc.useModule('ti.sysbios.rta.Agent');
     *  var LoggerBuf = xdc.useModule('xdc.runtime.LoggerBuf');
     *  
     *  // Set the main logger size to 50 records.
     *  Agent.mainLoggerSize = 50 * LoggerBuf.Entry.$sizeof();
     *
     *  // Print the final size to see how much memory is used.
     *  print("Application Logger Size (MAUs): " + Agent.mainLoggerSize);
     *  @p
     */
    metaonly config SizeT mainLoggerSize = 1024;
    
    /*!
     *  ======== mainLoggerBufferSection ========
     *  Section to place the Main LoggerBuf's buffer.
     */
    metaonly config String mainLoggerBufferSection = "[Default]";
    
    /*!
     *  ======== sysbiosHwiLogging ========
     *  Enable SYSBIOS Hwi module's event logging.
     */
    metaonly config Bool sysbiosHwiLogging = false;
    
    /*!
     *  ======== sysbiosHwiLoggingRuntimeControl ========
     *  Specify whether Hwi logging can be enabled / disabled at runtime.
     *
     *  This determines what diags settings are applied to the module's diags
     *  mask. If 'false', the diags bits will be configured as ALWAYS_OFF or 
     *  ALWAYS_ON, meaning they can't be changed at runtime. If 'true', the 
     *  bits will be configured as 'RUNTIME_OFF' or 'RUNTIME_ON'.
     */
    metaonly config Bool sysbiosHwiLoggingRuntimeControl = false;
    
    /*!
     *  ======== sysbiosSwiLogging ========
     *  Enable SYSBIOS Swi module's event logging.
     */
    metaonly config Bool sysbiosSwiLogging = false;

    /*!
     *  ======== sysbiosSwiLoggingRuntimeControl ========
     *  Specify whether Swi logging can be enabled / disabled at runtime.
     *
     *  This determines what diags settings are applied to the module's diags
     *  mask. If 'false', the diags bits will be configured as ALWAYS_OFF or 
     *  ALWAYS_ON, meaning they can't be changed at runtime. If 'true', the 
     *  bits will be configured as 'RUNTIME_OFF' or 'RUNTIME_ON'.
     */
    metaonly config Bool sysbiosSwiLoggingRuntimeControl = false;
    
    /*!
     *  ======== sysbiosTaskLogging ========
     *  Enable SYSBIOS Task module's event logging.
     */
    metaonly config Bool sysbiosTaskLogging = true;

    /*!
     *  ======== sysbiosTaskLoggingRuntimeControl ========
     *  Specify whether Task logging can be enabled / disabled at runtime.
     *
     *  This determines what diags settings are applied to the module's diags
     *  mask. If 'false', the diags bits will be configured as ALWAYS_OFF or 
     *  ALWAYS_ON, meaning they can't be changed at runtime. If 'true', the 
     *  bits will be configured as 'RUNTIME_OFF' or 'RUNTIME_ON'.
     */
    metaonly config Bool sysbiosTaskLoggingRuntimeControl = true;
    
    /*!
     *  ======== sysbiosLoggerSize ========
     *  Size (in MAUs) of the logger used for the SYS/BIOS modules' Log events.
     *
     *  To specify the size in number of records, instead of MAUs, use the
     *  following code:
     *  
     *  @p(code)
     *  var Agent = xdc.useModule('ti.sysbios.rta.Agent');
     *  var LoggerBuf = xdc.useModule('xdc.runtime.LoggerBuf');
     *  
     *  // Set the SYS/BIOS logger size to 50 records.
     *  Agent.sysbiosLoggerSize = 50 * LoggerBuf.Entry.$sizeof();
     *
     *  // Print the final size to see how much memory is used.
     *  print("SYS/BIOS Logger Size (MAUs): " + Agent.sysbiosLoggerSize);
     *  @p
     */
    metaonly config SizeT sysbiosLoggerSize = 4096;
    
    /*!
     *  ======== sysbiosLoggerBufferSection ========
     *  Section to place the SYS/BIOS LoggerBuf's buffer.
     */
    metaonly config String sysbiosLoggerBufferSection = "[Default]";
    
    /*!
     *  ======== configureSystemLog ========
     *  Deprecated.
     */
    metaonly config Bool configureSystemLog = true;
    
    /*!
     *  @_nodoc
     *  ======== numSystemRecords ========
     *  Deprecated.
     */
    metaonly config Int numSystemRecords = 64;
    
    /*!
     *  @_nodoc
     *  ======== configureLoadLog ========
     *  Deprecated.
     */
    metaonly config Bool configureLoadLog = true;
    
    /*!
     *  @_nodoc
     *  ======== numLoadRecords ========
     *  Deprecated.
     */
    metaonly config Int numLoadRecords = 64;
    
    /*!
     *  @_nodoc
     *  ======== periodInMs ========
     *  This configuration parameter is no longer supported since the I/O part
     *  of the Agent is no longer supported.
     */
    metaonly config Int periodInMs = 100;
    
    /*!
     *  @_nodoc
     *  ======== priority ========
     *  This configuration parameter is no longer supported since the I/O part
     *  of the Agent is no longer supported.
     */
    metaonly config Int priority = 1;
    
    /*!
     *  @_nodoc
     *  ======== agentStackSize ========
     *  This configuration parameter is no longer supported since the I/O part
     *  of the Agent is no longer supported.
     */
    metaonly config SizeT agentStackSize = 1024;
    
    /*!
     *  @_nodoc
     *  ======== agentStackSection ========
     *  This configuration parameter is no longer supported since the I/O part
     *  of the Agent is no longer supported.
     */
    metaonly config String agentStackSection = "[Default]";
    
    /*!
     *  @_nodoc
     *  ======== transferBufferSize ========
     *  This configuration parameter is no longer supported since the I/O part
     *  of the Agent is no longer supported.
     */
    config Int transferBufferSize = 85;
    
    /*!
     *  @_nodoc
     *  ======== transferBufferSection ========
     *  This configuration parameter is no longer supported since the I/O part
     *  of the Agent is no longer supported.
     */
    metaonly config String transferBufferSection;
    
    /*! 
     *  @_nodoc
     *  ======== addSupportLogger ========
     *  Whether to include the SupportLogger module.
     *
     *  The SupportLogger module logs timestamp information for use by the
     *  RTA tools on the host side.
     *
     *  Default is true.
     */
    metaonly config Bool addSupportLogger = true;
    
    /*!
     *  @_nodoc
     *  ======== supportControl ========
     *  This configuration parameter is no longer supported since the I/O part
     *  of the Agent is no longer supported.
     */
    metaonly config Bool supportControl = true;
    
    /*!
     *  @_nodoc
     *  ======== Transport ========
     *  The Transport enum lists the transports which the Agent is able
     *  to automatically configure. See the {@link #transport} config parameter.
     */
    metaonly enum Transport {
        Transport_RTDX,  //! RTDX (no longer supported).
        Transport_STOP_MODE_ONLY, //! Stop Mode Only
        Transport_USER  //! (not supported)
    };

    /*!
     *  @_nodoc
     *  ======== transport ========
     *  This configuration parameter is no longer supported since the I/O part
     *  of the Agent is no longer supported.
     */
    metaonly config Transport transport = Transport_STOP_MODE_ONLY;
    
    /*!
     *  @_nodoc
     *  ======== dataStream ========
     *  This configuration parameter is no longer supported since the I/O part
     *  of the Agent is no longer supported.
     */
    metaonly config String dataStream = "/rtdx0";
    
    /*!
     *  @_nodoc
     *  ======== commandStream ========
     *  This configuration parameter is no longer supported since the I/O part
     *  of the Agent is no longer supported.
     */
    metaonly config String commandStream = "/rtdx1";

    /*!
     *  @_nodoc
     *  ======== responseStream ========
     *  This configuration parameter is no longer supported since the I/O part
     *  of the Agent is no longer supported.
     */
    config String responseStream = "/rtdx1";
}
