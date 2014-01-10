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
 *  ======== Agent.xs ========
 *
 */

var Agent;

var Clock;
var Task;
var Semaphore;
var Swi;
var LoggerBuf;
var Event;
var Log;
var Diags;

/*
 *  ======== module$meta$init ========
 */
function module$meta$init()
{
    /* Only process during "cfg" phase */
    if (xdc.om.$name != "cfg") {
        return;
    }
    
    Agent = this;
}

/*
 *  ======== module$use ========
 */
function module$use()
{
    Agent.$logWarning("This module is deprecated and will not be supported " +
            "in a future release.  Please use the UIA product for the " +
            "equivalent functionality.", this);
    /*
     * Warn the user if they've modified any the configs that control the
     * runtime agent which is no longer supported.
     */
    unsupportedConfigs();

    /* Warn the user if they've modified any of the deprecated configs. */
    deprecateConfigs();
    
    LoggerBuf = xdc.useModule('xdc.runtime.LoggerBuf');
    Diags = xdc.useModule('xdc.runtime.Diags');
        
    xdc.useModule('xdc.runtime.Timestamp');
    xdc.useModule('xdc.runtime.Error');
    xdc.useModule('ti.sysbios.BIOS');
    
    /* Create and configure a log to store the timestamp frequency record. */
    if (Agent.addSupportLogger) {
        configureSupportLogger();
    }
    
    /* Create and configure a log to store all BIOS execution graph records */ 
    configureSystemLog();

    /* Create and configure a log to store all load records. */
    if (Agent.loadLogging || Agent.loadLoggingRuntimeControl) {
        configureLoadLog();
    }
    
    /* Setup Main and non-xdc modules loggers and diags mask */
    if (Agent.mainLogging || Agent.mainLoggingRuntimeControl) {
        configureMainLog();
    }    
}

/*
 *  ======== unsupportedConfigs ========
 *  Warn if the user has modified any of the no longer supported configs.
 */
function unsupportedConfigs()
{
    if (Agent.$written("periodInMs")) {
        Agent.$logWarning("'periodInMs' " + 
            "is no longer used.  The Agent module only supports stop mode.",
            Agent, "periodInMs");
    }

    if (Agent.$written("priority")) {
        Agent.$logWarning("'priority' " + 
            "is no longer used.  The Agent module only supports stop mode.",
            Agent, "priority");
    }

    if (Agent.$written("agentStackSize")) {
        Agent.$logWarning("'agentStackSize' " + 
            "is no longer used.  The Agent module only supports stop mode.",
            Agent, "agentStackSize");
    }

    if (Agent.$written("agentStackSection")) {
        Agent.$logWarning("'agentStackSection' " + 
            "is no longer used.  The Agent module only supports stop mode.",
            Agent, "agentStackSection");
    }

    if (Agent.$written("transferBufferSection")) {
        Agent.$logWarning("'transferBufferSection' " + 
            "is no longer used.  The Agent module only supports stop mode.",
            Agent, "transferBufferSection");
    }

    if (Agent.$written("supportControl")) {
        Agent.$logWarning("'supportControl' " + 
            "is no longer used.  The Agent module only supports stop mode.",
            Agent, "supportControl");
    }

    if (Agent.$written("transport") 
            && Agent.transport != Agent.Transport_STOP_MODE_ONLY) {
        Agent.$logWarning("'transport' " + 
            "is no longer used.  The Agent module only supports stop mode.",
            Agent, "transport");
    }

    if (Agent.$written("dataStream")) {
        Agent.$logWarning("'dataStream' " + 
            "is no longer used.  The Agent module only supports stop mode.",
            Agent, "dataStream");
    }

    if (Agent.$written("commandStream")) {
        Agent.$logWarning("'commandStream' " + 
            "is no longer used.  The Agent module only supports stop mode.",
            Agent, "commandStream");
    }

    if (Agent.$written("responseStream")) {
        Agent.$logWarning("'responseStream' " + 
            "is no longer used.  The Agent module only supports stop mode.",
            Agent, "responseStream");
    }
}

/*
 *  ======== deprecateConfigs ========
 *  Warn if the user has modified any of the deprecated configs.
 */
function deprecateConfigs()
{
    if (Agent.$written("configureSystemLog")) {
        Agent.$logWarning("The 'configureSystemLog' config is no longer " +
                          "used, please use the 'sysbiosTaskLogging', " + 
                          "'sysbiosSwiLogging', and 'sysbiosHwiLogging' " +
                          "configs.", Agent);
    }
    if (Agent.$written("numSystemRecords")) {
        Agent.$logWarning("The 'numSystemRecords' config is no longer " +
                          "used, please use 'sysbiosLoggerSize'.", Agent);
    }
    if (Agent.$written("configureLoadLog")) {
        Agent.$logWarning("The 'configureLoadLog' config is no longer " +
                          "used, please use 'loadLogging'.", Agent);
    }
    if (Agent.$written("numLoadRecords")) {
        Agent.$logWarning("The 'numLoadRecords' config is no longer " +
                          "used, please use 'loadLoggerSize'.", Agent);
    }
}

/*
 *  ======== configureSystemLog ========
 *  Creates the 'RTASystemLog' and associates it with the appropriate BIOS
 *  modules to support the RTA execution graph.
 */
function configureSystemLog()
{   
    /* 
     * If we're not configuring logging for any of the modules, we can just
     * return.
     */
    if (!(Agent.sysbiosTaskLogging ||
          Agent.sysbiosTaskLoggingRuntimeControl ||
          Agent.sysbiosSwiLogging ||
          Agent.sysbiosSwiLoggingRuntimeControl ||
          Agent.sysbiosHwiLogging ||
          Agent.sysbiosHwiLoggingRuntimeControl)) {
        return;
    }

    /* Retrieve the hal hwi module delegate. */
    var HalHwi = xdc.useModule('ti.sysbios.hal.Hwi');
    var HwiDelegate = HalHwi.HwiProxy.delegate$;

    /* Create the RTASystemLog. */
    
    /* Create the parameters object. */
    var sysLogPrms = new LoggerBuf.Params();
    
    /* Calculate the number of records based on the requested logger size. */
    sysLogPrms.numEntries = Agent.sysbiosLoggerSize / LoggerBuf.Entry.$sizeof();

    /* Add the buffer section if one was specified. */
    if (Agent.sysbiosLoggerBufferSection != "[Default]") {
        sysLogPrms.bufSection = Agent.sysbiosLoggerBufferSection;
    }
    
    /* Create the logger and give it the required name. */
    var sysLog = LoggerBuf.create(sysLogPrms);
    sysLog.instance.name = "RTASystemLog";
    
    /* Setup SYSBIOS Hwi and Clock diags masks. */
    if ((Agent.sysbiosHwiLogging) ||
        (Agent.sysbiosHwiLoggingRuntimeControl)) {
        
        /* Get the diags setting that will be applied to all bits. */
        var setting = getDiagsSetting(Agent.sysbiosHwiLogging, 
                                      Agent.sysbiosHwiLoggingRuntimeControl);

        /* Enable Hwi diags bits. */
        if (HwiDelegate.common$.diags_USER1 === undefined) {
            HwiDelegate.common$.diags_USER1 = setting;
        }
        if (HwiDelegate.common$.diags_USER2 === undefined) {
            HwiDelegate.common$.diags_USER2 = setting;
        }
        
        /* Set Hwi's logger. */
        setLogger(HwiDelegate, sysLog);

        /* Enable Clock diags bits. */
        Clock = xdc.useModule('ti.sysbios.knl.Clock');
        if (Clock.common$.diags_USER1 === undefined) {
            Clock.common$.diags_USER1 = setting;
        }
        if (Clock.common$.diags_USER2 === undefined) {
            Clock.common$.diags_USER2 = setting;
        }
        
        /* Set Clock's logger. */
        setLogger(Clock, sysLog);        
    }

    /* Setup SYSBIOS Swi diags mask */
    if ((Agent.sysbiosSwiLogging) ||
        (Agent.sysbiosSwiLoggingRuntimeControl)) {
        
        /* Get the diags setting that will be applied to all bits. */
        var setting = getDiagsSetting(Agent.sysbiosSwiLogging, 
                                      Agent.sysbiosSwiLoggingRuntimeControl);
        
        /* Enable Swi diags bits. */
        Swi = xdc.useModule('ti.sysbios.knl.Swi');
        if (Swi.common$.diags_USER1 === undefined) {
            Swi.common$.diags_USER1 = setting;
        }
        if (Swi.common$.diags_USER2 === undefined) {
            Swi.common$.diags_USER2 = setting;
        }
        
        /* Set Swi's logger. */
        setLogger(Swi, sysLog);
    }

    /* Setup SYSBIOS Task and related modules diags mask */
    if ((Agent.sysbiosTaskLogging) ||
        (Agent.sysbiosTaskLoggingRuntimeControl)) {
        
        /* Get the diags setting that will be applied to all bits. */
        var setting = getDiagsSetting(Agent.sysbiosTaskLogging, 
                                      Agent.sysbiosTaskLoggingRuntimeControl);
        
        /* Enable Task diags bits. */
        Task = xdc.useModule('ti.sysbios.knl.Task');       
        if (Task.common$.diags_USER1 === undefined) {
            Task.common$.diags_USER1 = setting;
        }
        if (Task.common$.diags_USER2 === undefined) {
            Task.common$.diags_USER2 = setting;
        }        

        /* Set Task's logger. */
        setLogger(Task, sysLog);

        /* Enable Semaphore diags bits. */
        Semaphore = xdc.useModule('ti.sysbios.knl.Semaphore');       
        if (Semaphore.common$.diags_USER1 === undefined) {
            Semaphore.common$.diags_USER1 = setting;
        }
        if (Semaphore.common$.diags_USER2 === undefined) {
            Semaphore.common$.diags_USER2 = setting;
        }
        
        /* Set Semaphore's logger. */
        setLogger(Semaphore, sysLog);

        /* Enable Event diags bits. */
        Event = xdc.useModule('ti.sysbios.knl.Event');       
        if (Event.common$.diags_USER1 === undefined) {
            Event.common$.diags_USER1 = setting;
        }
        if (Event.common$.diags_USER2 === undefined) {
            Event.common$.diags_USER2 = setting;
        }

        /* Set Event's logger. */
        setLogger(Event, sysLog);        
    }
}

/*
 *  ======== setLogger ========
 *  Set the specified module's logger. If the logger has already been set, warn
 *  the user and overwrite anyway.
 */
function setLogger(mod, logger)
{
    /* Warn if the user has already set the logger for this module. */
    if (mod.common$.logger != undefined) {
        mod.$logWarning(mod.$name + ".common$.logger has already " +
                           "been assigned, Agent reassigning logger to " +
                           logger.instance.name, mod);
    }
    
    /* Set the module's logger. */
    mod.common$.logger = logger;
}

/*
 *  ======== configureLoadLog ========
 *  Creates the 'RTALoadLog' and associates it with the Load module to store
 *  load records separately from the execution graph data.
 */
function configureLoadLog()
{   
    /* Create the parameters object. */
    var loadLogPrms = new LoggerBuf.Params();
    
    /* Calculate the number of records based on the requested buffer size. */
    loadLogPrms.numEntries = Agent.loadLoggerSize / LoggerBuf.Entry.$sizeof();
    
    /* Add the buffer section if one was specified. */
    if (Agent.loadLoggerBufferSection != "[Default]") {
        loadLogPrms.bufSection = Agent.loadLoggerBufferSection;
    }
    
    /* Create a LoggerBuf 'RTALoadLog' for the Load module. */
    var loadLog = LoggerBuf.create(loadLogPrms);
    
    /* Give the logger the required name. */
    loadLog.instance.name = "RTALoadLog";
    
    var Load = xdc.useModule('ti.sysbios.utils.Load');
    
    /* Warn if the user has already set the logger for this module. */
    if (Load.common$.logger != undefined) {
        Load.$logWarning(Load.$name + ".common$.logger has already " +
                         "been assigned, Agent reassigning logger to " +
                         "RTALoadLog", Load);
    }
    Load.common$.logger = loadLog;
    
    /* Only set these if the user hasn't set them already. */
    if (Load.common$.diags_USER4 == undefined) {
        Load.common$.diags_USER4 = 
            getDiagsSetting(Agent.loadLogging,
                            Agent.loadLoggingRuntimeControl);
    }
}

/*
 *  ======== getDiagsSetting ========
 */
function getDiagsSetting(enabled, runtimeCtrl)
{
    if (!enabled && !runtimeCtrl) {
        return (Diags.ALWAYS_OFF);
    }
    else if (!enabled && runtimeCtrl) {
        return (Diags.RUNTIME_OFF);
    }
    else if (enabled && !runtimeCtrl) {
        return (Diags.ALWAYS_ON);
    }
    else if (enabled && runtimeCtrl) {
        return (Diags.RUNTIME_ON);
    }

    return (undefined);
}

/*
 *  ======== configureMainLog ========
 *  Creates a logger and associates it with the Main module for application
 *  logs.
 */
function configureMainLog()
{
    /* Create the parameters object. */
    var mainLogPrms = new LoggerBuf.Params();
    
    /* Calculate the number of records based on the requested logger size. */
    mainLogPrms.numEntries = Agent.mainLoggerSize / LoggerBuf.Entry.$sizeof();

    /* Add the buffer section if one was specified. */
    if (Agent.mainLoggerBufferSection != "[Default]") {
        mainLogPrms.bufSection = Agent.mainLoggerBufferSection;
    }
    
    /* Create a LoggerBuf 'Main Logger' for Main. */
    var mainLog = LoggerBuf.create(mainLogPrms);
    mainLog.instance.name = "Main Logger";
    
    var Main = xdc.useModule('xdc.runtime.Main');
    
    /* Warn if the user has already set the logger for this module. */
    if (Main.common$.logger != undefined) {
        Main.$logWarning(Main.$name + ".common$.logger has already " +
                         "been assigned, Agent reassigning logger to " +
                         "'Main Logger'", Main);
    }
    Main.common$.logger = mainLog;

    /* Get the diags setting that will be applied to all bits. */
    var setting = getDiagsSetting(Agent.mainLogging, 
                                  Agent.mainLoggingRuntimeControl);
    
    /* Turn on the Main module diags mask */
    if (Main.common$.diags_ANALYSIS === undefined) {
        Main.common$.diags_ANALYSIS = setting;
    }
    if (Main.common$.diags_INFO === undefined) {
        Main.common$.diags_INFO = setting;
    }
    if (Main.common$.diags_USER1 === undefined) {
        Main.common$.diags_USER1 = setting;
    }
    if (Main.common$.diags_USER2 === undefined) {
        Main.common$.diags_USER2 = setting;
    }
    if (Main.common$.diags_USER3 === undefined) {
        Main.common$.diags_USER3 = setting;
    }
    if (Main.common$.diags_USER4 === undefined) {
        Main.common$.diags_USER4 = setting;
    }
    if (Main.common$.diags_USER5 === undefined) {
        Main.common$.diags_USER5 = setting;
    }
    if (Main.common$.diags_USER6 === undefined) {
        Main.common$.diags_USER6 = setting;
    }
}

/*
 *  ======== configureSupportLogger ========
 */
function configureSupportLogger()
{
    var SupportLogger = xdc.useModule('ti.sysbios.rta.SupportLogger');
    
    /* 
     * For now, create a logger with 1 record to hold the timestamp
     * frequency. The SupportLogger currently does not log the create logs.
     */
    var loggerParams = new LoggerBuf.Params();
    loggerParams.numEntries = 1;
    loggerParams.instance.name = "RTASupportLog";
    SupportLogger.common$.logger = LoggerBuf.create(loggerParams);
    
    /* 
     * Enable USER1 for logging the timestampFreq event.
     *
     * Don't enable the timestamp frequency event on MSP430 targets
     * with 16-bit args. The frequency value does not fit in the 16-bit
     * arguments, causing the RTA tools to receive the incorrect conversion
     * value. To workaround this, we simply don't log this event, so that
     * raw timestamp values are displayed in RTA instead of nanoseconds.
     */
    var argSizeBits = Program.build.target.stdTypes.t_IArg.size * 
        Program.build.target.bitsPerChar;
        
    if (argSizeBits == 16) {
        SupportLogger.common$.diags_USER1 = Diags.ALWAYS_OFF;
    }
    else {
        SupportLogger.common$.diags_USER1 = Diags.ALWAYS_ON;
    }
}
