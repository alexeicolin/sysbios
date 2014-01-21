;
;  Copyright (c) 2013, Texas Instruments Incorporated
;  All rights reserved.
; 
;  Redistribution and use in source and binary forms, with or without
;  modification, are permitted provided that the following conditions
;  are met:
; 
;  *  Redistributions of source code must retain the above copyright
;     notice, this list of conditions and the following disclaimer.
; 
;  *  Redistributions in binary form must reproduce the above copyright
;     notice, this list of conditions and the following disclaimer in the
;     documentation and/or other materials provided with the distribution.
; 
;  *  Neither the name of Texas Instruments Incorporated nor the names of
;     its contributors may be used to endorse or promote products derived
;     from this software without specific prior written permission.
; 
;  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
;  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
;  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
;  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
;  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
;  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
;  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
;  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
;  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
;  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
;  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;
;
; ======== Hwi_switch_asm.asm ========
;
;
    .cdecls C,NOLIST,"package/internal/Hwi.xdc.h"

    .global ti_sysbios_family_xxx_Hwi_switchToIsrStack
    .global ti_sysbios_family_xxx_Hwi_switchToTaskStack
    .global ti_sysbios_family_xxx_Hwi_switchAndRunFunc


;
;  ======== ti_sysbios_family_xxx_Hwi_switchToIsrStack ========
;
;  oldTaskSP = Hwi_switchToIsrStack();
;
;  Pseudo code:
;  
;      load taskSP from Hwi module state
;      pop return address (to caller) from stack
;      if (taskSP == 0) {
;          load isrStack pointer from Hwi module state
;          save current stack pointer as new taskSP in Hwi module state
;          swap isrStack pointer as current stack pointer
;      }
;      jump back to caller
;

    .align 2
    .sect ".text:ti_sysbios_family_xxx_Hwi_switchToIsrStack"
    .clink
ti_sysbios_family_xxx_Hwi_switchToIsrStack:
    .asmfunc

 .if $DEFINED(__LARGE_CODE_MODEL__) & $DEFINED(__LARGE_DATA_MODEL__)

    ; setup pointer to Hwi module state structure
    MOV.A #ti_sysbios_family_msp430_Hwi_Module__state__V, R15

    ; load current taskSP value
    MOV.A ti_sysbios_family_msp430_Hwi_Module_State.taskSP(R15), R12

    ; pop the return address from stack
    POP.A R14           

    ; if taskSP was not zero: do nothing, jump to finish
    CMP.A #0, R12
    JNE doneSwitchI

    ; else, load current isrStack pointer
    MOV.A ti_sysbios_family_msp430_Hwi_Module_State.isrStack(R15), R13

    ; store current (task) stack pointer as taskSP 
    MOV.A SP, ti_sysbios_family_msp430_Hwi_Module_State.taskSP(R15)

    ; swap isrStack pointer in as the SP
    MOV.A R13, SP       

doneSwitchI:

    ; branch back to caller (using return address popped above)
    BRA R14     


 .elseif $DEFINED(__LARGE_CODE_MODEL__)

    ; setup pointer to Hwi module state structure
    MOV.W #ti_sysbios_family_msp430_Hwi_Module__state__V, R15

    ; load current taskSP value
    MOV.W ti_sysbios_family_msp430_Hwi_Module_State.taskSP(R15), R12

    ; pop the return address from stack
    POP.A R14           

    ; if taskSP was not zero: do nothing, jump to finish
    CMP.W #0, R12
    JNE doneSwitchI

    ; else, load current isrStack pointer
    MOV.W ti_sysbios_family_msp430_Hwi_Module_State.isrStack(R15), R13

    ; store current (task) stack pointer as taskSP 
    MOV.W SP, ti_sysbios_family_msp430_Hwi_Module_State.taskSP(R15)

    ; swap isrStack pointer in as the SP
    MOV.W R13, SP       

doneSwitchI:

    ; branch back to caller (using return address popped above)
    BRA R14     


 .else
    ; setup pointer to Hwi module state structure
    MOV.W #ti_sysbios_family_msp430_Hwi_Module__state__V, R15

    ; load current taskSP value
    MOV.W ti_sysbios_family_msp430_Hwi_Module_State.taskSP(R15), R12

    ; pop the return address from stack
    POP.W R14           

    ; if taskSP was not zero: do nothing, jump to finish
    CMP.W #0, R12
    JNE doneSwitchI

    ; else, load current isrStack pointer
    MOV.W ti_sysbios_family_msp430_Hwi_Module_State.isrStack(R15), R13

    ; store current (task) stack pointer as taskSP 
    MOV.W SP, ti_sysbios_family_msp430_Hwi_Module_State.taskSP(R15)

    ; swap isrStack pointer in as the SP
    MOV.W R13, SP       

doneSwitchI:

    ; branch back to caller (using return address popped above)
    BR R14      

 .endif
 .endasmfunc


;
;  ======== ti_sysbios_family_xxx_Hwi_switchToTaskStack ========
;
;  Hwi_switchToTaskStack(oldTaskSP);
;
;  Pseudo code:
;  
;      pop return address (to caller) from stack
;      if (oldTaskSP == 0) {
;          load taskSP from Hwi module state into stack pointer
;          clear the taskSP value in Hwi module state
;      }
;      jump back to caller
;

    .align 2
    .sect ".text:ti_sysbios_family_xxx_Hwi_switchToTaskStack"
    .clink
ti_sysbios_family_xxx_Hwi_switchToTaskStack:
    .asmfunc

 .if $DEFINED(__LARGE_CODE_MODEL__) & $DEFINED(__LARGE_DATA_MODEL__)

    ; pop the return address from stack
    POP.A R14           

    ; if oldTaskSP is not zero, nothing to do, jump to finish
    CMP.A #0, R12
    JNE doneSwitchT

    ; else, setup pointer to Hwi module state structure
    MOV.A #ti_sysbios_family_msp430_Hwi_Module__state__V, R15

    ; swap task stack pointer back into to SP
    MOV.A ti_sysbios_family_msp430_Hwi_Module_State.taskSP(R15), SP

    ; write zero to taskSP to indicate back on a task stack
    MOV.A #0, ti_sysbios_family_msp430_Hwi_Module_State.taskSP(R15)

doneSwitchT:

    ; branch back to caller (using return address popped above)
    BRA R14     


 .elseif $DEFINED(__LARGE_CODE_MODEL__)

    ; pop the return address from stack
    POP.A R14           

    ; if oldTaskSP is not zero, nothing to do, jump to finish
    CMP.W #0, R12
    JNE doneSwitchT

    ; else, setup pointer to Hwi module state structure
    MOV.W #ti_sysbios_family_msp430_Hwi_Module__state__V, R15

    ; swap task stack pointer back into to SP
    MOV.W ti_sysbios_family_msp430_Hwi_Module_State.taskSP(R15), SP

    ; write zero to taskSP to indicate back on a task stack
    MOV.W #0, ti_sysbios_family_msp430_Hwi_Module_State.taskSP(R15)

doneSwitchT:

    ; branch back to caller (using return address popped above)
    BRA R14     


 .else

    ; pop the return address from stack
    POP.W R14           

    ; if oldTaskSP is not zero, nothing to do, jump to finish
    CMP.W #0, R12
    JNE doneSwitchT

    ; else, setup pointer to Hwi module state structure
    MOV.W #ti_sysbios_family_msp430_Hwi_Module__state__V, R15

    ; swap task stack pointer back into to SP
    MOV.W ti_sysbios_family_msp430_Hwi_Module_State.taskSP(R15), SP

    ; write zero to taskSP to indicate back on a task stack
    MOV.W #0, ti_sysbios_family_msp430_Hwi_Module_State.taskSP(R15)

doneSwitchT:

    ; branch back to caller (using return address popped above)
    BR R14      

    .endif

    .endasmfunc


;
;  ======== ti_sysbios_family_xxx_Hwi_switchAndRunFunc ========
;
;  Hwi_switchAndRunFunc(Void (*func)());
;
;  Switch to ISR stack, call the function Func() and then switch
;  back to Task stack.
;
;

    .align 2
    .sect ".text:ti_sysbios_family_xxx_Hwi_switchAndRunFunc"
    .clink
ti_sysbios_family_xxx_Hwi_switchAndRunFunc:
    .asmfunc

 .if $DEFINED(__LARGE_CODE_MODEL__) & $DEFINED(__LARGE_DATA_MODEL__)

    ; save R4 on the stack
    PUSH.A R4

    ; move R12 = func pointer to R4 so it is preserved across
    ; function calls
    MOV R12, R4

    ; Switch to ISR stack
    MOV.A #ti_sysbios_family_xxx_Hwi_switchToIsrStack, R15
    CALLA R15

    ; Save oldTaskSP
    PUSH.A R12

    ; Call func()
    CALLA R4

    ; Restore oldTaskSP
    POP.A R12

    ; Switch back to Task stack
    MOV.A #ti_sysbios_family_xxx_Hwi_switchToTaskStack, R15
    CALLA R15

    ; restore R4 from stack
    POP.A R4

    ; pop the return address from stack
    POP.A R14

    ; branch back to caller (using return address popped above)
    BRA R14


 .elseif $DEFINED(__LARGE_CODE_MODEL__)

    ; save R4 on the stack
    PUSH.A R4

    ; move R12 = func pointer to R4 so it is preserved across
    ; function calls
    MOV R12, R4

    ; Switch to ISR stack
    MOV.A #ti_sysbios_family_xxx_Hwi_switchToIsrStack, R15
    CALLA R15

    ; Save oldTaskSP
    PUSH.A R12

    ; Call func()
    CALLA R4

    ; Restore oldTaskSP
    POP.A R12

    ; Switch back to Task stack
    MOV.A #ti_sysbios_family_xxx_Hwi_switchToTaskStack, R15
    CALLA R15

    ; restore R4 from stack
    POP.A R4

    ; pop the return address from stack
    POP.A R14

    ; branch back to caller (using return address popped above)
    BRA R14


 .else

    ; save R4 on stack
    PUSH.W R4

    ; move R12 = func pointer to R4 so it is preserved across
    ; function calls
    MOV R12, R4

    ; Switch to ISR stack
    MOV.W #ti_sysbios_family_xxx_Hwi_switchToIsrStack, R15
    CALL R15

    ; Save oldTaskSP
    PUSH.W R12

    ; Call func()
    CALL R4

    ; Restore oldTaskSP
    POP.W R12

    ; Switch back to Task stack
    MOV.W #ti_sysbios_family_xxx_Hwi_switchToTaskStack, R15
    CALL R15

    ; restore R4 from stack
    POP.W R4

    ; pop the return address from stack
    POP.W R14

    ; branch back to caller (using return address popped above)
    BR R14

    .endif

    .endasmfunc
