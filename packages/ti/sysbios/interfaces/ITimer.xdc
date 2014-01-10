/*
 * Copyright (c) 2012-2013, Texas Instruments Incorporated
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
 *  ======== ITimer.xdc ========
 *
 *
 */

import xdc.runtime.Types;

/*
 * See SequenceInTimerAPIs.txt for details on how to implement 
 * this interface
 */

/*!
 *  ======== ITimer ========
 *  Interface for Timer Peripherals Manager.
 */

interface ITimer
{
    /*! Timer tick function prototype */
    typedef Void (*FuncPtr)(UArg);

    /*! Const used to specify any timer */
    const UInt ANY = ~0;

    /*! 
     *  Timer Start Modes
     *
     *  @c(StartMode_AUTO)
     *  Statically created/constructed Timers will be started in BIOS_start().
     *  Dynamically created Timers will start at create() time. This includes
     *  timers created before BIOS_start().
     *
     *  @c(StartMode_USER)
     *  Timer will be started by the user using start().
     */
    enum StartMode {
        StartMode_AUTO,         /*! timer starts automatically */
        StartMode_USER          /*! timer will be started by user */
    };

    /*! 
     *  Timer Run Modes
     *
     *  @c(RunMode_CONTINUOUS)
     *  Timer is periodic and runs continuously.
     *
     *  @c(RunMode_ONESHOT)
     *  Timer runs for a single period value and stops.
     *
     *  @c(RunMode_DYNAMIC)
     *  Timer is dynamically reprogrammed for the next required tick.
     *
     */
    enum RunMode {
        RunMode_CONTINUOUS,     /*! periodic and continuous */
        RunMode_ONESHOT,        /*! one-shot */
        RunMode_DYNAMIC         /*! dynamically reprogrammed (available on subset of devices) */
    };

    /*! 
     *  Timer Status
     *
     *  @c(Status_INUSE)
     *  Timer is in use. A timer is marked in use from the time it gets 
     *  created to the time it gets deleted.
     *
     *  @c(Status_FREE)
     *  Timer is free and can be acquired using create.
     */
    enum Status {
        Status_INUSE,           /*! timer in use */
        Status_FREE             /*! timer is free */
    };

    /*! 
     *  ======== PeriodType ========
     *  Timer period units
     *
     *  @c(PeriodType_MICROSECS)
     *  Period value is in microseconds.
     *
     *  @c(PeriodType_COUNTS)
     *  Period value is in counts.
     */
    enum PeriodType {
        PeriodType_MICROSECS,   /*! period in microsecs */
        PeriodType_COUNTS       /*! period in counts */
    };

    /*! 
     *  @_nodoc
     *  Timer supports RunMode_DYNAMIC?
     *
     *  Default is false.  Can be overriden by Timer drivers that indeed 
     *  support RunMode_DYNAMIC.
     */
    metaonly config Bool supportsDynamic = false;

    /*! 
     *  @_nodoc
     *  Default to RunMode_DYNAMIC?
     *
     *  Default is false.  Can be overriden by Timer drivers that support
     *  RunMode_DYNAMIC, who want DYNAMIC mode to be used by default.
     */
    metaonly config Bool defaultDynamic = false;

    /*! 
     *  ======== getNumTimers ========
     *  Returns number of timer peripherals on the platform.
     *
     *  @b(returns)     Number of timer peripherals.
     */
    @DirectCall
    UInt getNumTimers();

    /*! 
     *  ======== getStatus ========
     *  Returns timer status (free or in use).
     *
     *  @b(returns)     timer status
     */
    @DirectCall
    Status getStatus(UInt id);

    /*! 
     *  ======== startup ========
     *  @_nodoc
     *  Startup function to be called during BIOS_start
     *
     *  This function starts statically created timers with
     *  startMode = StartMode_AUTO.
     */
    @DirectCall
    Void startup();

    /*!
     *  @_nodoc
     *  ======== getFreqMeta ========
     *  Return timer frequency in Hz
     *
     *  This is the effective frequency of the clock incrementing the timer
     *  counter register after all scaling factors are taken into account.
     *  (including pre-scalars).
     */
    metaonly Types.FreqHz getFreqMeta(UInt id);

instance:

    /*!
     *  ======== create ========
     *  Create a timer.
     *
     *  Create could fail if timer peripheral is unavailable. To
     *  request any available timer use {@link #ANY} as the id.
     *  TimerId's are logical ids. The family-specific implementations
     *  map the ids to physical peripherals.
     *
     *  @param(id)      Timer id ranging from 0 to a platform specific value,
     *                  or {@link #ANY}
     *  @param(tickFxn) function that runs upon timer expiry.
     */
    @DirectCall
    create(Int id, FuncPtr tickFxn);

    /*!
     *  Timer run mode
     *
     *  Default is {@link #RunMode_CONTINUOUS}.
     */
    config RunMode runMode = RunMode_CONTINUOUS;

    /*!
     *  Timer start mode
     *
     *  Default is {@link #StartMode_AUTO}.
     */
    config StartMode startMode = StartMode_AUTO;

    /*!
     *  Argument for tick function
     *
     *  Default is null.
     */
    config UArg arg = null;

    /*!
     *  Period of a tick
     *
     *  The period can be specified in timer counts or microseconds
     *  and its default value is 0.
     *
     *  The implementation of ITimer will support a period of UInt32
     *  timer counts and use pre-scalars if necessary.
     */
    config UInt32 period = 0;

    /*!
     *  Period type
     *
     *  Default is PeriodType_MICROSECS
     */
    config PeriodType periodType = PeriodType_MICROSECS;

    /*!
     *  Timer frequency
     *
     *  This parameter is meaningfull only on platforms where the timer's
     *  input clock can be changed. If value is left at zero, then input clock
     *  to the timer clock is assumed.
     *
     *  This value is used to convert timer ticks to real time units; seconds,
     *  milliseconds, etc.
     */
    config xdc.runtime.Types.FreqHz extFreq  = {lo:0, hi:0};

    /*!
     *  @_nodoc
     *  ======== getMaxTicks ========
     *  Gets the maximum number of timer ticks that can be skipped (for Clock
     *  tick suppression), given the current timer configuration.
     *
     *  This API is used internally by SYS/BIOS for dynamic Clock tick
     *  suppression.  It is not intended to be used for any other purpose.
     */
    @DirectCall
    UInt32 getMaxTicks(UInt32 periodCounts);

    /*!
     *  @_nodoc
     *  ======== setNextTick ========
     *  Dynamically reprograms the timer with a new period value,
     *  corresponding to the next required tick.  The timer is left running
     *  after the call, and it does not need to be stopped and restarted by
     *  the caller.
     *
     *  This API is used internally by SYS/BIOS for dynamic Clock tick
     *  suppression.  It is not intended to be used for any other purpose.
     *
     *  @param(newPeriod)       new timer period, in units of timer counts
     *  @param(countsPerTick)   timer counts corresponding to a single tick
     */
    @DirectCall
    Void setNextTick(UInt32 newPeriod, UInt32 countsPerTick);

    /*!
     *  ======== start ========
     *  Reload and start the timer
     *
     *  Thread safety must be observed when using the {@link #start}
     *  and {@link #stop} APIs to avoid possible miss-
     *  configuration of the timers and unintended behaviors.
     *  To protect against re-entrancy, surround the start/stop invocations
     *  with {@link ti.sysbios.hal.Hwi#disable Hwi_disable()} and
     *  {@link ti.sysbios.hal.Hwi#restore Hwi_restore()} calls:
     *
     *  @p(code)
     *  // disable interrupts if an interrupt could lead to
     *  // another call to Timer_start().
     *  key = Hwi_disable();
     *  Timer_stop();
     *  ...
     *  Timer_start();
     *  Hwi_restore(key);
     *  @p
     *
     *  @a(side effects)
     *  Enables the timer's interrupt.
     */
    @DirectCall
    Void start();

    /*!
     *  ======== stop ========
     *  Stop the timer
     *
     *  Thread safety must be observed when using the {@link #start}
     *  and {@link #stop} APIs to avoid possible miss-
     *  configuration of the timers and unintended behaviors.
     *  To protect against re-entrancy, surround the start/stop invocations
     *  with {@link ti.sysbios.hal.Hwi#disable Hwi_disable()} and
     *  {@link ti.sysbios.hal.Hwi#restore Hwi_restore()} calls:
     *
     *  @p(code)
     *  // disable interrupts if an interrupt could lead to
     *  // another call to Timer_start().
     *  key = Hwi_disable();
     *  Timer_stop();
     *  ...
     *  Timer_start();
     *  Hwi_restore(key);
     *  @p
     *
     *  @a(side effects)
     *  Disables the timer's interrupt.
     */
    @DirectCall
    Void stop();

    /*!
     *  ======== setPeriod ========
     *  Set timer period specified in timer counts
     *
     *  Timer_setPeriod() invokes Timer_stop() prior to setting the period
     *  and leaves the timer in the stopped state.
     *
     *  To dynamically change the period of a timer you must
     *  protect against re-entrancy by disabling interrupts.
     *  Use the following call sequence to guarantee proper results:
     *
     *  @p(code)
     *  // disable interrupts if an interrupt could lead to
     *  // another call to Timer_start().
     *  key = Hwi_disable();
     *  Timer_setPeriod(period);
     *  Timer_start();
     *  Hwi_restore(key);
     *  @p
     *
     *  ITimer implementation must support UInt32 and use pre-scalars whenever
     *  necessary
     *
     *  @a(side effects)
     *  Calls Timer_stop(), and disables the timer's interrupt.
     *
     *  @param(period)          period in timer counts
     */
    @DirectCall
    Void setPeriod(UInt32 period);

    /*!
     *  ======== setPeriodMicroSecs ========
     *  Set timer period specified in microseconds.
     *
     *  A best-effort method will be used to set the period register.
     *  There might be a slight rounding error based on resolution of timer
     *  period register. If the timer frequency cannot support the requested
     *  period, i.e. the timer period register cannot support the requested
     *  period, then this function returns false.
     *
     *  Timer_setPeriodMicroSecs() invokes Timer_stop() prior to setting
     *  the period and leaves the timer in the stopped state.
     *
     *  To dynamically change the period of a timer you must
     *  protect against re-entrancy by disabling interrupts.
     *  Use the following call sequence to guarantee proper results:
     *
     *  @p(code)
     *  // disable interrupts if an interrupt could lead to
     *  // another call to Timer_start().
     *  key = Hwi_disable();
     *  Timer_setPeriodMicroSecs(period);
     *  Timer_start();
     *  Hwi_restore(key);
     *  @p
     *
     *  @param(period)          period in microseconds
     */
    @DirectCall
    Bool setPeriodMicroSecs(UInt32 microsecs);

    /*!
     *  ======== getPeriod ========
     *  Get timer period in timer counts
     *
     *  @b(returns)     period in timer counts
     */
    @DirectCall
    UInt32 getPeriod();

    /*!
     *  ======== getCount ========
     *  Read timer counter register
     *
     *  @b(returns)     timer counter value
     */
    @DirectCall
    UInt32 getCount();

    /*!
     *  ======== getFreq ========
     *  Return timer frequency in Hz
     *
     *  This is the effective frequency of the clock incrementing the timer
     *  counter register after all scaling factors are taken into account.
     *  (including pre-scalars).
     *
     *  @param(freq)    frequency in Hz
     */
    @DirectCall
    Void getFreq(xdc.runtime.Types.FreqHz *freq);

    /*!
     *  ======== getFunc ========
     *  Get Timer function and arg
     *
     *  @param(arg)     pointer for returning Timer's function argument
     *  @b(returns)     Timer's function
     */
    @DirectCall
    FuncPtr getFunc(UArg *arg);

    /*!
     *  ======== setFunc ========
     *  Overwrite Timer function and arg
     *
     *  Replaces a Timer object's tickFxn function originally
     *  provided in {@link #create}.
     *
     *  @param(fxn)     pointer to function
     *  @param(arg)     argument to function
     */
    @DirectCall
    Void setFunc(FuncPtr fxn, UArg arg);

    /*!
     *  ======== trigger ========
     *  Trigger timer function
     *
     *  @_nodoc
     *  Timer runs for specified number of cycles. The runMode
     *  must be Mode_ONESHOT.
     *
     *  This function should interrupt the cpu after specified number of
     *  cpu cycles.
     *
     *  The last instruction of trigger will start the timer. Depending on how
     *  the code is compiled, there may be one or more instructions in between
     *  the timer start and client code. The number of instructions specified
     *  is counted from when the timer is started.
     *
     *  @param(instructions)    cpu cycles
     */
    @DirectCall
    Void trigger(UInt32 cycles);

    /*!
     *  ======== getExpiredCounts ========
     *  Get current timer counter
     *
     *  @_nodoc
     *  Reads timer counter and adds period if IFR was set 
     *  before counter read. Used exclusively by TimestampProvider.
     *
     *  Must be called with interrupts disabled.
     *
     *  @b(returns)     expired counts.
     */
    @DirectCall
    UInt32 getExpiredCounts();

}
