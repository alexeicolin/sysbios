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
 *  ======== Timer.xdc ========
 *
 *
 */

package ti.sysbios.family.arm.tms570;

import xdc.rov.ViewInfo;

import xdc.runtime.Error;
import xdc.runtime.Types;
import ti.sysbios.interfaces.ITimer;
/*!
 *  ======== Timer ========
 *  Timer Peripherals Manager for arm_tms570 family.
 *
 *  @p(html)
 *  <h3> Calling Context </h3>
 *  <table border="1" cellpadding="3">
 *    <colgroup span="1"></colgroup> <colgroup span="5" align="center"></colgroup>
 *
 *    <tr><th> Function                 </th><th>  Hwi   </th><th>  Swi   </th><th>  Task  </th><th>  Main  </th><th>  Startup  </th></tr>
 *    <!--                                                                                                                 -->
 *    <tr><td> {@link #getNumTimers}            </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #getStatus}               </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #Params_init}             </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #construct}               </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #create}                  </td><td>   N    </td><td>   N    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #delete}                  </td><td>   N    </td><td>   N    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #destruct}                </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #getCount}                </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td><td>   N    </td></tr>
 *    <tr><td> {@link #getFreq}                 </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #getPeriod}               </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #reconfig}                </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #setPeriod}               </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #setPeriodMicroSecs}      </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> {@link #start}                   </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td><td>   N    </td></tr>
 *    <tr><td> {@link #stop}                    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   N    </td><td>   N    </td></tr>
 *    <tr><td colspan="6"> Definitions: <br />
 *       <ul>
 *         <li> <b>Hwi</b>: API is callable from a Hwi thread. </li>
 *         <li> <b>Swi</b>: API is callable from a Swi thread. </li>
 *         <li> <b>Task</b>: API is callable from a Task thread. </li>
 *         <li> <b>Main</b>: API is callable during any of these phases: </li>
 *           <ul>
 *             <li> In your module startup after this module is started (e.g. Timer_Module_startupDone() returns TRUE). </li>
 *             <li> During xdc.runtime.Startup.lastFxns. </li>
 *             <li> During main().</li>
 *             <li> During BIOS.startupFxns.</li>
 *           </ul>
 *         <li> <b>Startup</b>: API is callable during any of these phases:</li>
 *           <ul>
 *             <li> During xdc.runtime.Startup.firstFxns.</li>
 *             <li> In your module startup before this module is started (e.g. Timer_Module_startupDone() returns FALSE).</li>
 *           </ul>
 *       </ul>
 *    </td></tr>
 *
 *  </table>
 *  @p
 *
 */
@InstanceFinalize       /* To cleanup */
@InstanceInitError      /* To report unavailability of timer */
@ModuleStartup          /* to configure static timers */

module Timer inherits ti.sysbios.interfaces.ITimer
{
    /*! Max value of Timer period for PeriodType_COUNTS*/
    const UInt MAX_PERIOD = 0xffffffff;

    /*! Number of timer peripherals on chip */
    const Int NUM_TIMER_DEVICES = 2;

    /*! @_nodoc
     *  Min instructions to use in trigger().
     */
    const Int MIN_SWEEP_PERIOD = 8;

    /*! Timer Configuration struct.
     *
     *  @field(cos)     When true timer stops counting during emulation halt.
     *                  When false timer continues to count during emulation
     *                  halt.
     */
    struct Control {
        Bool cos;      /*! 0=stop in debug mode; 1=continue in debug mode */
    };

    /*! @_nodoc */
    @XmlDtd
    metaonly struct BasicView {
        Ptr         halTimerHandle;
        String      label;
        UInt        id;
        String      startMode;
        String      runMode;
        UInt        period;
        String      periodType;
        UInt        intNum;
        String      tickFxn[];
        UArg        arg;
        String      extFreq;
        UInt32      accuTicks;
        String      hwiHandle;
    };

    /*! @_nodoc */
    metaonly struct DeviceView {
        UInt        id;
        String      device;
        String      devAddr;
        UInt        intNum;
        String      runMode;
        UInt        period;
        UInt        currCount;
        UInt        remainingCount;
        String      state;
    };

    /*! @_nodoc */
    metaonly struct ModuleView {
        String      availMask;      /* available 32-bit timer halves */
    }

    /*! @_nodoc */
    @Facet
    metaonly config ViewInfo.Instance rovViewInfo =
        ViewInfo.create({
            viewMap: [
            [
                'Basic',
                {
                    type: ViewInfo.INSTANCE,
                    viewInitFxn: 'viewInitBasic',
                    structName: 'BasicView'
                }
            ],
            [
                'Device',
                {
                    type: ViewInfo.INSTANCE,
                    viewInitFxn: 'viewInitDevice',
                    structName: 'DeviceView'
                }
            ],
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

    /*!
     *  Error raised when timer id specified is not supported.
     */
    config Error.Id E_invalidTimer  = {msg: "E_invalidTimer: Invalid Timer Id %d"};

    /*!
     *  Error raised when timer requested is in use
     */
    config Error.Id E_notAvailable  =
        {msg: "E_notAvailable: Timer not available %d"};

    /*!
     *  Error raised when Hwi Params has mask where self is turned on.
     *
     *  This is not allowed because the timers on this platform do not
     *  support one-shot mode and a stub is used to stop it.
     *  Another timer interrupt cannot go off when the ISR is running.
     */
    config Error.Id E_invalidHwiMask  =
        {msg: "E_InvalidMask: Mask in hwiParams cannot enable self"};

    /*!
     *  Error raised when period requested is not supported
     */
    config Error.Id E_cannotSupport  =
        {msg: "E_cannotSupport: Timer cannot support requested period %d"};

    /*!
     *  ======== anyMask ========
     *  Available mask to be used when select = Timer_ANY
     */
    config UInt anyMask = 0x3;

    /*!
     *  ======== oneShotStub ========
     *  @_nodoc
     *  RTI timer does not support one shot mode. This stub stops timer
     *  and disables its interrupt.
     *
     *  @param(arg)     Timer Handle.
     */
    Void oneShotStub(UArg arg);

    /*!
     *  ======== periodicStub ========
     *  @_nodoc
     *  When accuTicks = true, this stub does the tick accumulation.
     *
     *  @param(arg)     Timer Handle.
     */
    Void periodicStub(UArg arg);

    /*!
     *  ======== getHandle ========
     *  @_nodoc
     *  Used by TimestampProvider module to get hold of timer handle used by
     *  Clock.
     *
     *  @param(id)      timer Id.
     */
    Handle getHandle(UInt id);

instance:


    /*! Control register configuration. Default cos=0. */
    config Control controlRegInit = {cos: 0};

    /*! Hwi Params for Hwi Object. Default is null.*/
    config Hwi.Params *hwiParams = null;

    /*! Hwi intNum to be used by Timer. */
    config Int intNum = -1;

    /*!
     *  ======== reconfig ========
     *  Used to modify static timer instances at runtime.
     *
     *  @param(timerParams)     timer Params
     *  @param(tickFxn)         functions that runs when timer expires.
     */
    Void reconfig(FuncPtr tickFxn, const Params *timerParams, Error.Block *eb);

    /*!
     *  ======== getAccuTicks ========
     *  @_nodoc
     *  returns accumulated ticks. Used by TimestampProvider.
     *
     *  @b(returns)     get accumulated ticks
     */
    UInt32 getAccuTicks();

    /*!
     *  ======== ackInterrupt ========
     *  clear the current timer interrupt flag
     */
    Void ackInterrupt();

internal:   /* not for client use */

    /*!
     *  ======== noStartupNeeded ========
     *  Flag used to prevent misc code from being brought in
     *  un-necessarily
     */
    config UInt startupNeeded = false;

    /*
     * array of Timer intNums
     */
    config UInt8 intNumDef[2];

    /*
     *  ======== initDevice ========
     *  reset timer to its resting state
     */
    Void initDevice(Object *timer);

    /*
     *  ======== postInit ========
     *  finish initializing static and dynamic Timers
     */
    Int postInit(Object *timer, Error.Block *eb);

    /*
     *  ======== checkOverflow ========
     */
    Bool checkOverflow(UInt32 a, UInt32 b);

    struct Instance_State {
        Bool                    staticInst;
        Int                     id;
        UInt                    controlRegInit;
        ITimer.RunMode          runMode;
        ITimer.StartMode        startMode;
        UInt                    period;
        ITimer.PeriodType       periodType;
        UInt                    intNum;
        UArg                    arg;
        Hwi.FuncPtr             tickFxn;
        Types.FreqHz            extFreq;
        volatile UInt32         accuTicks;
        Hwi.Handle              hwi;
    }

    struct Module_State {
        UInt            availMask;      /* available peripherals */
        Handle          handles[NUM_TIMER_DEVICES];
                                        /* array of handles based on id */
    }
}
