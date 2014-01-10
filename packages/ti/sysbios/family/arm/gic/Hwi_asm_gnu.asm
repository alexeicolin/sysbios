@
@  Copyright (c) 2013, Texas Instruments Incorporated
@  All rights reserved.
@
@  Redistribution and use in source and binary forms, with or without
@  modification, are permitted provided that the following conditions
@  are met:
@
@  *  Redistributions of source code must retain the above copyright
@     notice, this list of conditions and the following disclaimer.
@
@  *  Redistributions in binary form must reproduce the above copyright
@     notice, this list of conditions and the following disclaimer in the
@     documentation and/or other materials provided with the distribution.
@
@  *  Neither the name of Texas Instruments Incorporated nor the names of
@     its contributors may be used to endorse or promote products derived
@     from this software without specific prior written permission.
@
@  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
@  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
@  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
@  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
@  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
@  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
@  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
@  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
@  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
@  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
@  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
@
@ ======== Hwi_asm_gnu.asm ========
@
@

        .arm

        .global ti_sysbios_family_arm_gic_Hwi_dispatch__I
        .global ti_sysbios_family_arm_gic_Hwi_dispatchC__I
        .global ti_sysbios_family_arm_gic_Hwi_init__I
        .global ti_sysbios_family_arm_gic_Hwi_vectors
        .global ti_sysbios_family_arm_gic_Hwi_disable__E
        .global ti_sysbios_family_arm_gic_Hwi_enable__E
        .global ti_sysbios_family_arm_gic_Hwi_restore__E
        .global ti_sysbios_family_xxx_Hwi_switchToTaskStack
        .global ti_sysbios_family_xxx_Hwi_switchToIsrStack

@
@  ======== Hwi_dispatchIRQ ========
@
@  Here on IRQ interrupt
@
@  task stack while in C code:
@
@       sp + 0x28 =  cpsr_sys
@       sp + 0x24 =  RA
@       sp + 0x20 =  lr_sys
@       sp + 0x1c =  saved r12
@       sp + 0x18 =  saved r5
@       sp + 0x14 =  saved r4
@       sp + 0x10 =  saved r3
@       sp + 0x0c =  saved r2
@       sp + 0x08 =  saved r1
@       sp + 0x04 =  saved r0
@       sp + 0x00 =  tskKey
@

        .text
        .func ti_sysbios_family_arm_gic_Hwi_dispatch__I
        .align 2

ti_sysbios_family_arm_gic_Hwi_dispatch__I:
        sub     lr, lr, #4
        srsdb   sp!, #0x1f          @ push sys CPSR and LR onto SYS stack
        cps     #0x1f               @ switch back to System mode
        push    {r0-r5, r12, lr}    @ save required C registers
        ldr     r5, [sp, #32]       @ retrieve IRP

#if (defined(__VFP_FP__) && !defined(__SOFTFP__))
        vstmdb  r13!, {D0-D7}   @ save vfp scratch regs
        vstmdb  r13!, {D16-D31} @ save vfp scratch regs
        fmrx    r1, FPEXC
        fmrx    r2, FPSCR
        push    {r1-r2}
#endif

        mov     r4, sp          @ save sp
        bic     sp, sp, #0x7    @ align task stack to 8 bytes
        ldr     r1, hwiTaskDisable
        ldr     r1, [r1]
        cmp     r1, #0
        blxne   r1              @ call Task_disable()
        push    {r0}            @ save tskKey

        @ switch to isr stack
        ldr     r1, switchToIsrStack
        blx     r1
        push    {r0}            @ save oldTaskSP

        mov     r0, r5          @ pass IRP as argument to dispatchC()
        mov     r5, sp          @ save sp
        bic     sp, sp, #0x7    @ align isr stack to 8 bytes

        ldr     r1, dispatchC
        blx     r1

        mov     sp, r5          @ restore sp

        @ switch back to task stack if at the bottom of Hwi stack
        pop     {r0}            @ restore oldTaskSP
        ldr     r1, switchToTaskStack
        blx     r1
        pop     {r0}            @ restore tskKey

        @ run task scheduler
        ldr     r1, hwiTaskRestore
        ldr     r1, [r1]
        cmp     r1, #0
        blxne   r1              @ call Task_restoreHwi()
                                @ returns with interrupts disabled
        mov     sp, r4          @ restore sp

#if (defined(__VFP_FP__) && !defined(__SOFTFP__))
        pop     {r1-r2}
        fmxr    FPEXC, r1
        fmxr    FPSCR, r2
        vldmia  r13!, {D16-D31} @ restore vfp scratch regs
        vldmia  r13!, {D0-D7}   @ restore vfp scratch regs
#endif

        pop     {r0-r5, r12, lr}
        rfeia   sp!
        .endfunc

        .align 2
hwiTaskDisable:
        .word   ti_sysbios_family_arm_gic_Hwi_taskDisable__C

hwiTaskRestore:
        .word   ti_sysbios_family_arm_gic_Hwi_taskRestoreHwi__C

dispatchC:
        .word   ti_sysbios_family_arm_gic_Hwi_dispatchC__I

switchToIsrStack:
        .word   ti_sysbios_family_xxx_Hwi_switchToIsrStack

switchToTaskStack:
        .word   ti_sysbios_family_xxx_Hwi_switchToTaskStack

@
@  ======== Hwi_init ========
@  Low level ARM mode-specific register initialization
@

        .text
        .func ti_sysbios_family_arm_gic_Hwi_init__I

ti_sysbios_family_arm_gic_Hwi_init__I:

@ Set vector table base address
        ldr r0, vbar
        mcr p15, #0, r0, c12, c0, #0

        bx  lr
        .endfunc

        .align 2
vbar:
        .word   ti_sysbios_family_arm_gic_Hwi_vectors

        .end
