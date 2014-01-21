@
@ ======== Exception_asm.s470 ========
@
@

        @.cdecls C,NOLIST,"package/internal/Exception.xdc.h" 

        .arm

        .global ti_sysbios_family_arm_exc_Exception_excHandlerAsm__I
        .global ti_sysbios_family_arm_exc_Exception_excHandlerDataAsm__I
        .global ti_sysbios_family_arm_exc_Exception_excHandler__I
        .global ti_sysbios_family_arm_exc_Exception_init__I

@ti_sysbios_family_arm_exc_Exception_Module__state__V .tag ti_sysbios_family_arm_exc_Exception_Module_State

@
@  ======== Exception_excHandlerAsm ========
@  Common Exception handler
@

        .text
        .func ti_sysbios_family_arm_exc_Exception_excHandlerAsm__I

ti_sysbios_family_arm_exc_Exception_excHandlerAsm__I:
        stmfd   sp!, {r0-r12}   @ save r4-r12 while we're at it

        mrc     p15, #0, r12, c5, c0, #0 @ read DFSR into r12
        stmfd   sp!, {r12}      	 @ save DFSR
        mrc     p15, #0, r12, c5, c0, #1 @ read IFSR into r12
        stmfd   sp!, {r12}      	 @ save IFSR
        mrc     p15, #0, r12, c6, c0, #0 @ read DFAR into r12
        stmfd   sp!, {r12}      	 @ save DFAR
        mrc     p15, #0, r12, c6, c0, #2 @ read IFAR into r12
        stmfd   sp!, {r12}      	 @ save IFAR

        mrs     r12, cpsr
        push    {r12}           @ save current CPSR

        mov     r0, sp          @ pass sp to exception handler
        sub     lr, lr, #4      @ back up to offending instruction
        mov     r1, lr          @ pass lr (=pc)

        mrs     r12, cpsr
        orr     r12, r12, #0x1f @ switch back to SYS mode
        msr     cpsr_cf, r12    @

        stmfd   r0!, {lr}       @ save system lr
        mov     r12, sp
        stmfd   r0!, {r12}      @ save system sp
        mrs     r12, cpsr
        stmfd   r0!, {r12}      @ save system cpsr

        ldr     pc, excHandlerAddr

excHandlerAddr:
        .word   ti_sysbios_family_arm_exc_Exception_excHandler__I
        .endfunc

@
@  ======== Exception_excHandlerDataAsm ========
@  Data Abort Exception handler
@

        .func ti_sysbios_family_arm_exc_Exception_excHandlerDataAsm__I

ti_sysbios_family_arm_exc_Exception_excHandlerDataAsm__I:
        stmfd   sp!, {r0-r12}   @ save r4-r12 while we're at it

        mrc     p15, #0, r12, c5, c0, #0 @ read DFSR into r12
        stmfd   sp!, {r12}      @ save DFSR
        mrc     p15, #0, r12, c5, c0, #1 @ read IFSR into r12
        stmfd   sp!, {r12}      @ save DFSR
        mrc     p15, #0, r12, c6, c0, #0 @ read DFAR into r12
        stmfd   sp!, {r12}      @ save DFAR
        mrc     p15, #0, r12, c6, c0, #2 @ read IFAR into r12
        stmfd   sp!, {r12}      @ save IFAR

        mrs     r12, cpsr
        add     r12, r12, #1    @ replace abort (0x17) with 0x18 
        push    {r12}           @ save current CPSR

        mov     r0, sp          @ pass sp to exception handler
        sub     lr, lr, #8      @ back up to offending instruction
        mov     r1, lr          @ pass lr (=pc)

        mrs     r12, cpsr
        orr     r12, r12, #0x1f @ switch back to SYS mode
        msr     cpsr_cf, r12    @

        stmfd   r0!, {lr}       @ save system lr too
        mov     r12, sp
        stmfd   r0!, {r12}      @ save system sp
        mrs     r12, cpsr
        stmfd   r0!, {r12}      @ save system cpsr

        ldr     pc, excHandlerAddr
        .endfunc


@
@  ======== Exception_init ========
@  Low level ARM mode-specific register initialization
@

        .text
        .func ti_sysbios_family_arm_exc_Exception_init__I

ti_sysbios_family_arm_exc_Exception_init__I:

@ Initialize Abort sp
        mrs     r12, cpsr
        bic     r12, r12, #0x1f @ switch to Abort mode
        orr     r12, r12, #0x17 @ to initialize Abort sp
        msr     cpsr_cf, r12    @

        ldr     sp, excStackAddr
        ldr     sp, [sp]            @ base of exc stack
        ldr         r12, excStackSize
        ldr         r12, [r12]      @ size of exc stack
        add         sp, r12, sp     @ sp = initial exc stack

@ Initialize Undefined sp
        mrs     r12, cpsr
        bic     r12, r12, #0x1f @ switch to Undef mode
        orr     r12, r12, #0x1b @ to initialize Undef sp
        msr     cpsr_cf, r12    @

        ldr     sp, excStackAddr
        ldr     sp, [sp]            @ base of exc stack
        ldr         r12, excStackSize
        ldr         r12, [r12]      @ size of exc stack
        add         sp, r12, sp     @ sp = initial exc stack

@ Done
        mrs     r12, cpsr
        orr     r12, r12, #0x1f @ switch back to SYS mode
        msr     cpsr_cf, r12    @

        bx      lr
        .endfunc

excStackAddr:
        .word   ti_sysbios_family_arm_exc_Exception_Module__state__V + 0xC

excStackSize:
        .word   ti_sysbios_family_arm_exc_Exception_Module__state__V + 0x10

        .end

