;
;  Copyright (c) 2012, Texas Instruments Incorporated
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
; ======== Hwi_switch_asm_iar.asm ========
;
;

#ifdef __LARGE_DATA_MODEL__    ; data model = Large
#define offset_taskSP    0
#define offset_isrStack  4
#else
#define offset_taskSP    0
#define offset_isrStack  2
#endif

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

    RSEG CODE:CODE

    NAME ti_sysbios_family_xxx_Hwi_switchToIsrStack

    PUBLIC ti_sysbios_family_xxx_Hwi_switchToIsrStack

    EXTERN ti_sysbios_family_msp430_Hwi_Module__state__V

ti_sysbios_family_xxx_Hwi_switchToIsrStack:

#if __CORE__ == __430X_CORE__

#ifdef __LARGE_DATA_MODEL__    ; data model = Large

    ; setup pointer to Hwi module state structure
    MOVX.A #ti_sysbios_family_msp430_Hwi_Module__state__V, R15


    ; load current taskSP value
    MOVX.A offset_taskSP(R15), R12

    ; pop the return address from stack
    POPX.A R14          

    ; if taskSP was not zero: do nothing, jump to finish
    CMPX.A #0, R12
    JNE doneSwitchI

    ; else, load current isrStack pointer
    MOVX.A offset_isrStack(R15), R13

    ; store current (task) stack pointer as taskSP 
    MOVX.A SP, offset_taskSP(R15)

    ; swap isrStack pointer in as the SP
    MOVX.A R13, SP      

doneSwitchI:

    ; branch back to caller (using return address popped above)
    BRA R14     


#else                           ; data model = Small

    ; setup pointer to Hwi module state structure
    MOVX.W #ti_sysbios_family_msp430_Hwi_Module__state__V, R15

    ; load current taskSP value
    MOVX.W offset_taskSP(R15), R12

    ; pop the return address from stack
    POPX.A R14          

    ; if taskSP was not zero: do nothing, jump to finish
    CMPX.W #0, R12
    JNE doneSwitchI

    ; else, load current isrStack pointer
    MOVX.W offset_isrStack(R15), R13

    ; store current (task) stack pointer as taskSP 
    MOVX.W SP, offset_taskSP(R15)

    ; swap isrStack pointer in as the SP
    MOVX.W R13, SP      

doneSwitchI:

    ; branch back to caller (using return address popped above)
    BRA R14     


#endif

#else    ; __CORE__ == __430_CORE__

    ; setup pointer to Hwi module state structure
    MOV.W #ti_sysbios_family_msp430_Hwi_Module__state__V, R15

    ; load current taskSP value
    MOV.W offset_taskSP(R15), R12

    ; pop the return address from stack
    POP.W R14           

    ; if taskSP was not zero: do nothing, jump to finish
    CMP.W #0, R12
    JNE doneSwitchI

    ; else, load current isrStack pointer
    MOV.W offset_isrStack(R15), R13

    ; store current (task) stack pointer as taskSP 
    MOV.W SP, offset_taskSP(R15)

    ; swap isrStack pointer in as the SP
    MOV.W R13, SP       

doneSwitchI:

    ; branch back to caller (using return address popped above)
    BR R14      

#endif

    ENDMOD


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

    RSEG CODE:CODE

    NAME ti_sysbios_family_xxx_Hwi_switchToTaskStack

    PUBLIC ti_sysbios_family_xxx_Hwi_switchToTaskStack

    EXTERN ti_sysbios_family_msp430_Hwi_Module__state__V

ti_sysbios_family_xxx_Hwi_switchToTaskStack:
 
#if __CORE__ == __430X_CORE__

#ifdef __LARGE_DATA_MODEL__   ; data model = Large

    ; pop the return address from stack
    POPX.A R14          

    ; if oldTaskSP is not zero, nothing to do, jump to finish
    CMPX.A #0, R12
    JNE doneSwitchT

    ; else, setup pointer to Hwi module state structure
    MOVX.A #ti_sysbios_family_msp430_Hwi_Module__state__V, R15

    ; swap task stack pointer back into to SP
    MOVX.A offset_taskSP(R15), SP

    ; write zero to taskSP to indicate back on a task stack
    MOVX.A #0, offset_taskSP(R15)

doneSwitchT:

    ; branch back to caller (using return address popped above)
    BRA R14     

#else                           ; data model = Small

    ; pop the return address from stack
    POPX.A R14          

    ; if oldTaskSP is not zero, nothing to do, jump to finish
    CMP.W #0, R12
    JNE doneSwitchT

    ; else, setup pointer to Hwi module state structure
    MOVX.W #ti_sysbios_family_msp430_Hwi_Module__state__V, R15

    ; swap task stack pointer back into to SP
    MOVX.W offset_taskSP(R15), SP

    ; write zero to taskSP to indicate back on a task stack
    MOVX.W #0, offset_taskSP(R15)

doneSwitchT:

    ; branch back to caller (using return address popped above)
    BRA R14     

#endif

#else    ; __CORE__ == __430_CORE__

    ; pop the return address from stack
    POP.W R14           

    ; if oldTaskSP is not zero, nothing to do, jump to finish
    CMP.W #0, R12
    JNE doneSwitchT

    ; else, setup pointer to Hwi module state structure
    MOV.W #ti_sysbios_family_msp430_Hwi_Module__state__V, R15

    ; swap task stack pointer back into to SP
    MOV.W offset_taskSP(R15), SP

    ; write zero to taskSP to indicate back on a task stack
    MOV.W #0, offset_taskSP(R15)

doneSwitchT:

    ; branch back to caller (using return address popped above)
    BR R14      

#endif

    ENDMOD

    END
