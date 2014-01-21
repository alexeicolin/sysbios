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

/* List of modules to be included in ROM */
var Swi = xdc.useModule('ti.sysbios.knl.Swi');
var Hwi = xdc.useModule('ti.sysbios.hal.Hwi');
var Idle = xdc.useModule('ti.sysbios.knl.Idle');
var Task = xdc.useModule('ti.sysbios.knl.Task');
var Event = xdc.useModule('ti.sysbios.knl.Event');
var Timer = xdc.useModule('ti.sysbios.hal.Timer');
var Clock = xdc.useModule('ti.sysbios.knl.Clock');
var Mailbox = xdc.useModule('ti.sysbios.knl.Mailbox');
var HeapMem = xdc.useModule('ti.sysbios.heaps.HeapMem');
var Mailbox = xdc.useModule('ti.sysbios.knl.Mailbox');
var Semaphore = xdc.useModule('ti.sysbios.knl.Semaphore');
var GateMutex = xdc.useModule('ti.sysbios.gates.GateMutex');
var HwiDelegate = xdc.useModule('ti.sysbios.family.c28.Hwi');
var TaskSupport = xdc.useModule('ti.sysbios.family.c28.TaskSupport');
var TimerDelegate = xdc.useModule('ti.sysbios.family.c28.Timer');
var TimestampProvider = xdc.useModule('ti.sysbios.family.c28.f2837x.TimestampProvider');
