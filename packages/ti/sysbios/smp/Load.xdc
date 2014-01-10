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
 *  ======== Load.xdc ========
 *
 */

package ti.sysbios.smp;

import xdc.rov.ViewInfo;

import ti.sysbios.knl.Queue;
import ti.sysbios.knl.Task;
import ti.sysbios.knl.Swi;
import ti.sysbios.gates.GateMutex;
import ti.sysbios.interfaces.IHwi;

import xdc.rov.ViewInfo;
import xdc.runtime.Diags;
import xdc.runtime.Log;
import xdc.runtime.Error;

/*!
 *  ======== Load ========
 *  The Load module reports execution times and load information for threads
 *  in a system.
 *
 *  SYS/BIOS manages four distinct levels of execution threads: hardware
 *  interrupt service routines, software interrupt routines, tasks, and
 *  background idle functions. This module reports execution time and load on a
 *  per-task basis, and also provides information globally for hardware
 *  interrupt service routines, software interrupt routines and idle functions
 *  (in the form of the idle task). It can also report an estimate of the
 *  global CPU load, which is computed as the percentage of time in the
 *  benchmark window which was NOT spent in the idle loop.
 *  More specifically, the load is computed as follows:
 *
 *  global CPU load = 100 * (1 - (min. time for a trip around idle loop *
 *      # times in idle loop)/(benchmark time window) )
 *
 *  Any work done in the idle loop is included in the CPU load - in other
 *  words, any time spent in the loop beyond the shortest trip around the idle
 *  loop is counted as non-idle time.
 *
 *  Execution time is reported in units of
 *  {@link xdc.runtime.Timestamp Timestamp} counts, and load is reported
 *  in percentages.
 *
 *  By default, load data is gathered in all threads.
 *  {@link #hwiEnabled hwiEnabled}, {@link #swiEnabled swiEnabled}
 *  and {@link #taskEnabled taskEnabled} can be used to select which
 *  type(s) of threads are monitored. These statistics are automatically
 *  recorded to the load module's logger instance. Users can also choose
 *  to call {@link #getTaskLoad}, {@link #getGlobalSwiLoad},
 *  {@link #getGlobalHwiLoad} and {@link #getCPULoad} at any time to obtain the
 *  statistics at runtime.
 *
 *  The module relies on {@link #update} to be called to compute load and
 *  execution times from the time when {@link #update} was last called. This
 *  is automatically done for every period specified by
 *  {@link #windowInMs windowInMs} in an {@link ti.sysbios.knl.Idle Idle}
 *  function when {@link #updateInIdle updateInIdle} is
 *  set to true. The time between 2 calls to {@link #update} is called the
 *  benchmark time window.
 *
 *  By passing in a function pointer of type
 *  Void(*{@link #postUpdate})(Void) through the
 *  {@link #postUpdate postUpdate} config parameter, one can specify a
 *  {@link #postUpdate postUpdate} function that is
 *  automatically called by {@link #update} immediately after the statistics
 *  have been computed at the end of a benchmark time window.  Setting this
 *  function allows the user to optionally post-process the thread statistics
 *  once and only once per benchmark window.
 *
 *  Advanced users could optionally omit calling {@link #update} in the idle
 *  loop by setting {@link #updateInIdle updateInIdle} to false. They can
 *  then call {@link #update} somewhere else (e.g. in a periodic higher
 *  priority Task) instead to ensure statistics are computed even when the
 *  system is never idle.
 *
 *  @a(Examples)
 *  Configuration example: The following statements configure the Load module
 *  to obtain output in the CCS output window. This is useful for early
 *  development without RTA tools support in SYS/BIOS:
 *
 *  @p(code)
 *  var System = xdc.useModule('xdc.runtime.System');
 *  var SysStd = xdc.useModule('xdc.runtime.SysStd');
 *  System.SupportProxy = SysStd;
 *
 *  var Load = xdc.useModule('ti.sysbios.utils.Load');
 *  var LoggerSys = xdc.useModule('xdc.runtime.LoggerSys');
 *  var Defaults = xdc.useModule('xdc.runtime.Defaults');
 *  var Diags = xdc.useModule('xdc.runtime.Diags');
 *
 *  // Turn on statistics log messages for Load module
 *  Load.common$.diags_USER4 = Diags.ALWAYS_ON;
 *  // Define a program-wide logger to be the System Logger
 *  Defaults.common$.logger = LoggerSys.create();
 *  @p
 *
 *  Notes on above example:
 *  @p(blist)
 *  - When using the {@link xdc.runtime.LoggerSys System Logger},
 *  {@link xdc.runtime.System#printf System_printf} calls are made to produce
 *  the output in the output window. These calls are highly intrusive
 *  and are only suited for early development.
 *
 *  - Also make sure your task stack sizes are large enough when using the
 *  {@link xdc.runtime.LoggerSys System Logger} - particularly the idle task
 *  stack size since logging is done in the idle task by default.
 *  @p
 *
 *  @a(Caveats)
 *  @p(nlist)
 *  - For the module to return accurate load values, the {@link #update}
 *  function must be run at least once before the Timestamp count gets to
 *  wrap around. This means on a platform with a 32-bit
 *  {@link xdc.runtime.Timestamp Timestamp} frequency that runs at 200 MHz,
 *  the function must be called at least once every 21 sec. With faster
 *  {@link xdc.runtime.Timestamp Timestamp} frequencies it would have to be
 *  called even more frequently.
 *
 *  - If the {@link #updateInIdle updateInIdle} option is enabled, the
 *  module will call {@link #update} periodically according to
 *  {@link #windowInMs windowInMs}. The latter must be set to a time
 *  interval that is below
 *  (2^32) * 1000 / ({@link xdc.runtime.Timestamp Timestamp} Frequency)
 *  milliseconds
 *
 *  - If the {@link #updateInIdle updateInIdle} option is disabled, the user is
 *  then responsible for calling {@link #update} more often than the above
 *  time interval.
 *
 *  - Load values might not add up to 100%. Because the module minimizes
 *  its interrupt latency and avoids locking up the system to compute all
 *  thread loads at once, each thread's execution time is evaluated separately
 *  over its own time window, which could be slightly different to that of
 *  another thread.
 *
 *  - When disabling monitoring of a thread type of higher priority, the
 *  time spent in that thread type will be counted towards time spent in
 *  threads of lower priority. E.g. if {@link #hwiEnabled hwiEnabled} and
 *  {@link #swiEnabled swiEnabled} are both false, then time spent in Hwi's and
 *  Swi's will be counted towards the Tasks in which the interrupts happened.
 *  Thus, for better accuracy, it is best to leave monitoring on for threads of
 *  a higher priority relative to the thread type of interest.
 *
 *  - The implementation of {@link #getCPULoad()} self-calibrates the shortest
 *  path through the idle loop. It does this by keeping track of the shortest
 *  time between invocations of an idle function automatically inserted by the
 *  Load module, and assumes that to be the time it takes for one iteration
 *  through the idle loop. Because of this, the CPU load value is only an
 *  estimate since the idle loop might occasionally take longer to run
 *  (e.g. due to caching effects, stalls). The reported CPU load tends to be
 *  slightly higher than reality, especially when the load is low.
 *
 *  - If {@link #taskEnabled taskEnabled} is set to true,
 *  task name support will be automatically turned on so that the Load module
 *  can print out the task names in its log output.
 *
 *  - Currently does not support {@link xdc.runtime.Timestamp Timestamp}
 *  frequencies over 4 GHz.
 *  @p
 *
 *  @p(html)
 *  <h3> Calling Context </h3>
 *  <table border="1" cellpadding="3">
 *    <colgroup span="1"></colgroup> <colgroup span="5" align="center">
 *    </colgroup>
 *
 *    <tr><th> Function                 </th><th>  Hwi   </th><th>  Swi   </th>
 *    <th>  Task  </th><th>  Main  </th><th>  Startup  </th></tr>
 *    <!--                                                          -->
 *    <tr><td> {@link #getCPULoad}      </td><td>   Y    </td><td>   Y    </td>
 *    <td>   Y    </td><td>   N    </td><td>   N    </td></tr>
 *    <tr><td> {@link #getGlobalHwiLoad}</td><td>   Y    </td><td>   Y    </td>
 *    <td>   Y    </td><td>   N    </td><td>   N    </td></tr>
 *    <tr><td> {@link #getGlobalSwiLoad}</td><td>   Y    </td><td>   Y    </td>
 *    <td>   Y    </td><td>   N    </td><td>   N    </td></tr>
 *    <tr><td> {@link #getTaskLoad}     </td><td>   Y    </td><td>   Y    </td>
 *    <td>   Y    </td><td>   N    </td><td>   N    </td></tr>
 *    <tr><td> {@link #reset}           </td><td>   Y*   </td><td>   Y*   </td>
 *    <td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #update}          </td><td>   Y*   </td><td>   Y*   </td>
 *    <td>   Y    </td><td>   N    </td><td>   N    </td></tr>
 *    <tr><td colspan="6"> Definitions: <br />
 *       <ul>
 *         <li> <b>Hwi</b>: API is callable from a Hwi thread. </li>
 *         <li> <b>Swi</b>: API is callable from a Swi thread. </li>
 *         <li> <b>Task</b>: API is callable from a Task thread. </li>
 *         <li> <b>Main</b>: API is callable during any of these phases: </li>
 *           <ul>
 *             <li> In your module startup after this module is started
 *    (e.g. Load_Module_startupDone() returns TRUE). </li>
 *             <li> During xdc.runtime.Startup.lastFxns. </li>
 *             <li> During main().</li>
 *             <li> During BIOS.startupFxns.</li>
 *           </ul>
 *         <li> <b>Startup</b>: API is callable during any of these phases:</li>
 *           <ul>
 *             <li> During xdc.runtime.Startup.firstFxns.</li>
 *             <li> In your module startup before this module is started
 *    (e.g. Load_Module_startupDone() returns FALSE).</li>
 *           </ul>
 *       <li> <b>*</b>: Indicates only when {@link #taskEnabled taskEnabled}
 *    is set to false. </li>
 *       </ul>
 *    </td></tr>
 *  </table>
 *  @p
 */
module Load
{
    /*!
     *  ======== ModuleView ========
     *  @_nodoc
     */
    metaonly struct ModuleView {
        String cpuLoad;     /* total cpu load, in percent */
        String swiLoad;     /* cpu load due to Swi's, in percent */
        String hwiLoad;     /* cpu load due to Hwi's, in percent */
        String idleError;   /* estimated measurement error in one pass of idle
                             * loop, in percent
                             */
    }

    /*!
     *  ======== rovViewInfo ========
     *  @_nodoc
     */
    @Facet
    metaonly config ViewInfo.Instance rovViewInfo =
        ViewInfo.create({
            viewMap: [
            [
                'Module',
                {
                    type: ViewInfo.MODULE,
                    viewInitFxn: 'viewInitModule',
                    structName: 'ModuleView'
                }
            ],
            ]
        });


    /* -------- Module Types -------- */

    /*!
     *  ======== FuncPtr ========
     *  Callback function type
     */
    typedef Void (*FuncPtr)();

    /*!
     *  ======== Stat ========
     *  Load statistics info
     */
    struct Stat {
        UInt32 threadTime;
        UInt32 totalTime;
    };


    /* -------- Module Configuration Parameters -------- */

    /*!
     *  ======== LS_cpuLoad ========
     *  This event logs the global CPU load as a percentage
     *
     *  This event is logged whenever the Diags.USER4 level is enabled
     *  and the load is measured.
     *
     */
    config Log.Event LS_cpuLoad = {
        mask: Diags.USER4,
        msg: "LS_cpuLoad: %d%%"
    };

    /*!
     *  ======== LS_hwiLoad ========
     *  This event logs the Hwi load
     *
     *  This event is logged whenever the Diags.USER4 level is enabled
     *  {@link #hwiEnabled} is true, and the load is measured.
     *
     *  This event logs the Hwi load, with the form:
     *  <load time>,<measurement time>
     *
     *  Percentage load = <load time> / <measurement time> x 100%
     */
    config Log.Event LS_hwiLoad = {
        mask: Diags.USER4,
        msg: "LS_hwiLoad: %d,%d"
    };

    /*!
     *  ======== LS_swiLoad ========
     *  This event logs the Swi load
     *
     *  This event is logged whenever the Diags.USER4 level is enabled
     *  {@link #swiEnabled} is true, and the load is measured.
     *
     *  This event logs the Swi load, with the form:
     *  <load time>,<measurement time>
     *
     *  Percentage load = <load time> / <measurement time> x 100%
     */
    config Log.Event LS_swiLoad = {
        mask: Diags.USER4,
        msg: "LS_swiLoad: %d,%d"
    };

    /*!
     *  ======== LS_taskLoad ========
     *  This event logs Task thread loads
     *
     *  This event is logged whenever the Diags.USER4 level is enabled,
     *  {@link #taskEnabled} is true, and the load is measured.
     *
     *  This event logs Task thread loads, with the form:
     *  <Task handle>,<load time>,<measurement time>,<Task function pointer>
     *
     *  Percentage load = <load time> / <measurement time> x 100%
     */
    config Log.Event LS_taskLoad = {
        mask: Diags.USER4,
        msg: "LS_taskLoad: 0x%x,%d,%d,0x%x"
    };

    /*!
     *  ======== postUpdate ========
     *  User function called after each load update
     *
     *  If non-NULL, postUpdate is run immediately after each update.
     */
    config FuncPtr postUpdate = null;

    /*!
     *  ======== updateInIdle ========
     *  Automatically call {@link #update} in Idle loop
     *
     *  If this parameter is set to `true`, an {@link ti.sysbios.knl.Idle Idle}
     *  function, which computes and updates the CPU load, is added to the
     *  {@link ti.sysbios.knl.Idle#run Idle loop}.
     */
    config Bool updateInIdle = true;

    /*!
     *  ======== minIdle ========
     *  Specifies the minimum time used to compute idle time
     *
     *  During CPU load measurement, the minimum time observed executing
     *  the idle loop together with the count of the number of times the idle
     *  loop runs id used to compute the total idle time.  However, since the
     *  _actual_ time spent in the idle loop will almost always be greater
     *  then the observed minimum, the idle time is almost always under
     *  estimated.
     *
     *  This configuration parameter can be used to compensate for this
     *  underestimate by placing a lower bound on the minimum time used in
     *  the CPU measurements.  The minimum idle time used to compute CPU
     *  load is guarenteed to be greater than or equal to `minIdle`.
     */
    metaonly config UInt32 minIdle = 0;

    /*!
     *  ======== windowInMs ========
     *  Load averaging period
     *
     *  When {@link #updateInIdle} is enabled, this period determines the
     *  minimum elapsed time between updates of the load statistics in the
     *  idle loop.
     */
    config UInt windowInMs = 500;

    /*!
     *  ======== hwiEnabled ========
     *  Enables Hwi time monitoring
     *
     *  When this parameter is set to `true`, {@link ti.sysbios.hal.Hwi Hwi}
     *  processing time is accumulated between successive calls of
     *  {@link #update} and, as part of Load_update, an
     *  {@link #LS_hwiLoad event} with this cumulative time is logged.  This
     *  time, together with the total elapsed time, is used to estimate the
     *  CPU load due to any Hwi instances run between these updates.
     */
    config Bool hwiEnabled = false;

    /*!
     *  ======== swiEnabled ========
     *  Enables Swi time monitoring
     *
     *  When this parameter is set to `true`, Swi processing time is
     *  accumulated between successive calls of {@link #update} and, as
     *  part of Load_update, an {@link #LS_swiLoad event} with this
     *  cumulative time is logged.  This time together with the total
     *  elapsed time is used to estimate the CPU load due to any Swi
     *  instances run between these updates.
     */
    config Bool swiEnabled = false;

    /*!
     *  ======== taskEnabled ========
     *  Enables Task time monitoring
     *
     *  When this parameter is set to `true`, each Task's processing time is
     *  accumulated between successive calls of {@link #update} and, as
     *  part of Load_update, an {@link #LS_taskLoad event} with this
     *  cumulative time is logged for each task.  This time together with the
     *  total elapsed time is used to estimate the CPU load due to each Task
     *  run between these updates.
     */
    config Bool taskEnabled = true;

    /* -------- Module Functions -------- */

    /*!
     *  ======== getTaskLoad ========
     *  Return the load and time spent in a specific task
     *
     *  This function returns the load and time spent in a specific task
     *  along with the duration over which the measurement was done. Numbers
     *  are reported in  {@link xdc.runtime.Timestamp Timestamp} counts.
     *
     *  Task handle must be valid and have been registered with Load.
     *
     *  @param(task) Handle of the Task which time we are interested in.
     *  @param(stat) Load and time statistics info
     *  @b(returns) TRUE if success, FALSE if failure
     *
     */
    @DirectCall
    Bool getTaskLoad(Task.Handle task, Stat *stat);

    /*!
     *  ======== update ========
     *  Record thread statistics and reset all counters
     *
     *  If {@link #taskEnabled taskEnabled} is set to true, this function can
     *  only be called in task context.
     */
    @DirectCall
    Void update();

    /*!
     *  ======== reset ========
     *  Reset all internal load counters
     *
     *  If {@link #taskEnabled taskEnabled} is set to true, this function can
     *  only be called in task context.
     */
    @DirectCall
    Void reset();

    /*!
     *  ======== getGlobalSwiLoad ========
     *  Return the load and time spent in Swi's
     *
     *  This function returns the load and time spent in Swi's along with
     *  the time duration over which the measurement was done. Numbers are
     *  reported in  {@link xdc.runtime.Timestamp Timestamp} counts.
     *
     *  @param(stat) Load and time statistics info
     *  @b(returns) TRUE if success, FALSE if failure
     *
     */
    @DirectCall
    Bool getGlobalSwiLoad(Stat *stat);

    /*!
     *  ======== getGlobalHwiLoad ========
     *  Return the load and time spent in hwi's
     *
     *  This function computes the load and time spent in Hwi's along
     *  with the time duration over which the measurement was done. Numbers
     *  are reported in  {@link xdc.runtime.Timestamp Timestamp} counts.
     *
     *  @param(stat) Load and time statistics info
     *
     *  @b(returns) TRUE if success, FALSE if failure
     */
    @DirectCall
    Bool getGlobalHwiLoad(Stat *stat);

    /*!
     *  ======== getCPULoad ========
     *  Return an estimate of the global CPU load
     *
     *  This function returns an estimate of CPU load (% utilization of the
     *  CPU), with the idle time determined based on number of trips through
     *  the idle loop multiplied by the shortest amount of time through the
     *  loop.
     *
     *  This function requires the idle loop to be run during a benchmark time
     *  window.
     *
     *  Note: Time spent in kernel while switching to a Hwi/Swi/Task is
     *        considered non-idle time.
     *
     *  @b(returns) CPU load in %
     *
     */
    @DirectCall
    UInt32 getCPULoad();

    /*!
     *  ======== calculateLoad ========
     *  Compute total CPU load from a Load_Stat structure
     *
     *  This function computes percent load from the
     *  values in a Load_Stat structure.
     *
     *  @a(returns) Load value of a Load_Stat structure in %.
     */
    @DirectCall
    UInt32 calculateLoad(Stat *stat);

    /*!
     *  ======== setMinIdle ========
     *  Set lower bound on idle loop time used to compute CPU load
     *
     *  @see #minIdle
     */
    @DirectCall
    UInt32 setMinIdle(UInt32 newMinIdleTime);

    /*! @_nodoc
     *  ======== addTask ========
     *  Add a task to the list for benchmarking
     *
     *  Returns TRUE if task is successfully added to the list.
     *
     *  If {@link #taskEnabled} is set to true, this function can only be
     *  called in task context.
     *
     *  @param(task) Handle of the Task to be added to the list.
     *  @param(env) Handle of context structure to be used by the Task
     *  @b(returns) TRUE if success, FALSE if failure
     *
     */
    @DirectCall
    Bool addTask(Task.Handle task, HookContext *env);

    /*! @_nodoc
     *  ======== removeTask ========
     *  Remove a task from the list for benchmarking
     *
     *  Returns TRUE if task is successfully removed from the list.
     *
     *  If {@link #taskEnabled} is set to true, this funciton can only be
     *  called in task context.
     *
     *  @param(taskHandle) Handle of the Task to be removed from the list.
     *  @b(returns) TRUE if success, FALSE if failure
     *
     */
    @DirectCall
    Bool removeTask(Task.Handle task);

    /*!
     *  @_nodoc
     *  ======== idleFxn ========
     *  Idle function used to periodically update the Task time values
     *
     */
    @DirectCall
    Void idleFxn();

    /* -------- Hook Functions -------- */

    /*!
     *  @_nodoc
     *  ======== taskCreateHook ========
     *  Create hook function used to initialize all task's hook context
     *  to NULL during creation time. Also adds the task's hook context
     *  when {@link #autoAddTasks} is set to true.
     *
     *  @param(task) Handle of the Task to initialize.
     *  @param(eb) Error block.
     *
     */
    @DirectCall
    Void taskCreateHook(Task.Handle task, Error.Block *eb);

    /*!
     *  @_nodoc
     *  ======== taskDeleteHook ========
     *  Delete hook function used to remove the task's hook context
     *  when {@link #autoAddTasks} is set to true.
     *
     *  @param(task) Handle of the Task to delete.
     *
     */
    @DirectCall
    Void taskDeleteHook(Task.Handle task);

    /*!
     *  @_nodoc
     *  ======== taskSwitchHook ========
     *  Switch hook function used to perform benchmarks
     *
     *  @param(curTask) Handle of currently executing Task.
     *  @param(nextTask) Handle of the next Task to run
     *
     */
    @DirectCall
    Void taskSwitchHook(Task.Handle curTask, Task.Handle nextTask);

    /*!
     *  @_nodoc
     *  ======== swiBeginHook ========
     *  Swi begin hook function used to perform benchmarks
     *
     *  @param(swi) Handle of Swi to begin execution.
     *
     */
    @DirectCall
    Void swiBeginHook(Swi.Handle swi);

    /*!
     *  @_nodoc
     *  ======== swiEndHook ========
     *  Swi end hook function used to perform benchmarks
     *
     *  @param(swi) Handle of Swi to end execution.
     *
     */
    @DirectCall
    Void swiEndHook(Swi.Handle swi);

    /*!
     *  @_nodoc
     *  ======== hwiBeginHook ========
     *  Hwi begin hook function used to perform benchmarks
     *
     *  @param hwi Handle of Hwi to begin execution.
     *
     */
    @DirectCall
    Void hwiBeginHook(IHwi.Handle hwi);

    /*!
     *  @_nodoc
     *  ======== hwiEndHook ========
     *  Hwi end hook function used to perform benchmarks
     *
     *  @param hwi Handle of Hwi to end execution.
     *
     */
    @DirectCall
    Void hwiEndHook(IHwi.Handle hwi);

    /*!
     *  @_nodoc
     *  ======== taskRegHook ========
     *  Registration function for the module's hook
     *
     *  Moved out of the internal section for ROM purposes. This function
     *  is not referenced directly, so it must be a "public" function so
     *  that the linker does not drop it when creating a ROM image.
     *
     *  @param(id) The id of the hook for use in load.
     *
     */
    @DirectCall
    Void taskRegHook(Int id);

internal:   /* not for client use */

    /* -------- Internal Module Types -------- */

    /*!
     *  @_nodoc
     *  Hook Context
     */
    struct HookContext {
        Queue.Elem qElem;        /*! Queue element */
        UInt32 totalTimeElapsed; /*! Total amount of time elapsed */
        UInt32 totalTime;        /*! time spent in thread */
        UInt32 nextTotalTime;    /*! working counter of time spent in thread */
        UInt32 timeOfLastUpdate; /*! time when update was last called */
        Ptr threadHandle;        /*! handle to thread whose context this is */
    };

    /* -------- Internal Module Parameters -------- */

    /*! @_nodoc
     *  Automatically add all tasks
     */
    config Bool autoAddTasks = true;

    /* -------- Internal Module Functions -------- */

    /*!
     *  @_nodoc
     *  ======== logLoads ========
     *  Logs load values for all monitored threads.
     *  Statistics messages from the kernel must be enabled
     *  (via Load.common$.diags_USER4) in order to see the output.
     *
     *  If {@link #taskEnabled} is set to TRUE, this function can only be
     *  called in task context.
     */
    Void logLoads();


    struct Module_State {
        Queue.Object taskList;   /* List to hold registered task instances */

        Int taskHId;             /* Task Hook Context Id for this module */

        UInt32 taskStartTime[];  /* The start time of the current task */
        UInt32 timeElapsed;      /* Working count of time elapsed */
        Task.Handle runningTask[]; /* Currently running task */
        Bool firstSwitchDone;    /* Flag for first task switch */

        UInt32 swiStartTime;     /* Start time of the current Swi */
        HookContext swiEnv;      /* Singleton hook context for swi's */
        UInt32 swiCnt;           /* number of Swi's currently executing */

        UInt32 hwiStartTime;     /* Start time of the current Hwi */
        HookContext hwiEnv;      /* Singleton hook context for hwi's */
        UInt32 hwiCnt;           /* number of Hwi's currently executing */

        UInt32 timeSlotCnt;      /* count of number of time windows printed */

        UInt32 minLoop;          /* shortest time thru the idle loop */
        UInt32 minIdle;          /* minLoop is never set below this value */
        UInt32 t0;               /* start time of previous call to idle fxn */
        UInt32 idleCnt;          /* number of times through idle loop */
        UInt32 cpuLoad;          /* CPU load in previous time window */

        GateMutex.Object gate;   /* Gate to Task list */
    };
}
