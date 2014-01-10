/* "useHwiMacros" allows for faster inline disable/enable */
#define ti_sysbios_Build_useHwiMacros

#include <xdc/std.h>
#include <xdc/runtime/Types.h>
#include <xdc/cfg/global.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Swi.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <xdc/runtime/Timestamp.h>
#include <xdc/runtime/System.h>

#ifndef __IAR_SYSTEMS_ICC__
/* 
 * "nolocalnames" removes conflicts of having both family
 * specific and the hal HWI modules included
 */
#define ti_sysbios_family_arm_a8_intcps_Hwi__nolocalnames
#include <ti/sysbios/family/arm/a8/intcps/Hwi.h>
#endif
#include <ti/sysbios/hal/Hwi.h>


/*--------------------------------------
 *  Function Prototypes
 *--------------------------------------*/
int main(Void);
Void benchmarkTask(UArg a0, UArg a1);
Void task1Func(UArg a0, UArg a1);
Void task2Func(UArg a0, UArg a1);
Void swi1Func(Void);
Void swi2Func(Void);
Void hwi1Func(UArg a0);
Void hwi2Func(UArg a0);
Void hwi3Func(UArg a0);
Void hwi4Func(UArg a0);
Void genericHwiFunc(UArg a0);
Void (*doHwi)(UArg);

/*--------------------------------------
 *  Global Variables
 *--------------------------------------*/
volatile UInt32 t1;            /* temp var for holding first Timestamp */
volatile UInt32 t2;            /* temp var for holding second Timestamp */
UInt32 delta;         /* var for output of t2-t1-offset in TIME() */

/*
 *  Macro used to LOOP functions and total averages
 *  Inputs: FXN, delta
 *  Outputs: min, max, avg
 *  Assumes delta is externally updated by FXN such as TIME()
 */
#define NUM_LOOPS 10000
#define LOOP(FXN) {              \
    min = ~(0);                  \
    max = 0;                     \
    total = 0;                   \
    for (i = 0; i < NUM_LOOPS; i++) {\
        FXN;                     \
        total += delta;          \
        if(delta < min)          \
            min = delta;         \
        if (delta > max)         \
            max = delta;         \
    }                            \
    avg = total / NUM_LOOPS;     \
}

/*
 *  Macro used to TIME function executions
 *  Inputs: FXN, offset
 *  Output: delta
 */
#define TIME(FXN) {            \
    t1 = Timestamp_get32();    \
    FXN;                       \
    t2 = Timestamp_get32();    \
    delta = (t2);         \
    delta = delta - (t1);         \
}

/*
 *  Macro and definitions for interrupt compatibility across boards
 */
#if defined(ti_targets_arm_elf_A8F) \
    | defined(ti_targets_arm_elf_A8Fnv) \
    | defined(gnu_targets_arm_A8F) /* Cortex A8 */
    #define HWI_INT 13
    #define CONFIGURE_INTERRUPTS() /* NOP */
    #define TRIGGER_INTERRUPT() \
        Hwi_post(HWI_INT);
    #define CLEAR_INTERRUPT() \
        ti_sysbios_family_arm_a8_intcps_Hwi_intc.ISR_CLEAR0 = (1 << HWI_INT);
    #define CHANGE_HWI(hwiHandle, hwiFXN) \
        Hwi_setFunc(hwiHandle, hwiFXN, NULL);
#elif defined(xdc_target__isaCompatible_v7M) \
    | defined(xdc_target__isaCompatible_v7M4)/* CortexM4, CortexM3 */
    #define HWI_INT 17
    #define CONFIGURE_INTERRUPTS() /* NOP */
    #define TRIGGER_INTERRUPT() \
        Hwi_post(HWI_INT);
    #define CLEAR_INTERRUPT() /* NOP */
    #define CHANGE_HWI(hwiHandle, hwiFXN) \
        Hwi_setFunc(hwiHandle, hwiFXN, NULL);
#elif defined(xdc_target__isaCompatible_430X) /* MSP430 */
    extern volatile unsigned int SFRIFG1_L;
    extern volatile unsigned int SFRIE1_L;
    #undef NUM_LOOPS
    #define NUM_LOOPS 10
    #define CONFIGURE_INTERRUPTS() {\
        SFRIFG1_L &= !0x08; \
        SFRIE1_L |= 0x08; \
    }  /* Clear pending interrupt bit, Enable VMA interrupts */
    #define TRIGGER_INTERRUPT() \
        __data20_write_char(0xFFFFF,1);
    #define CLEAR_INTERRUPT() \
        SFRIFG1_L &= !0x08;  /* Clear pending interrupt bit */
    #define CHANGE_HWI(hwiHandle, hwiFXN) \
        doHwi=hwiFXN;
#elif defined(ti_targets_arm_elf_Arm9) /* ARM9 */
    #define HWI_INT 13
    #define CONFIGURE_INTERRUPTS() /* NOP */
    #define TRIGGER_INTERRUPT() \
        Hwi_post(HWI_INT); \
        asm("   nop");     \
        asm("   nop");     \
        asm("   nop");     \
        asm("   nop");     \
        asm("   nop");     \
        asm("   nop");     \
        asm("   nop");     \
        asm("   nop");     \
        asm("   nop");     \
        asm("   nop");     \
        asm("   nop");     \
        asm("   nop");
    #define CLEAR_INTERRUPT() /* NOP */
    #define CHANGE_HWI(hwiHandle, hwiFXN) \
        Hwi_setFunc(hwiHandle, hwiFXN, NULL);
#elif defined(xdc_target__isaCompatible_674) \
    | defined(xdc_target__isaCompatible_64P) \
    | defined(xdc_target__isaCompatible_64T) \
    | defined(xdc_target__isaCompatible_66) \
    | defined(xdc_target__isaCompatible_28) \
    | defined(xdc_target__isaCompatible_arp32) \
    | defined(ti_targets_arm_elf_A15Fnv) \
    | defined(gnu_targets_arm_A15F) /* C6000, C2000, ARP32, A15 */
    #define HWI_INT 13
    #define CONFIGURE_INTERRUPTS() /* NOP */
    #define TRIGGER_INTERRUPT() \
        Hwi_post(HWI_INT);
    #define CLEAR_INTERRUPT() /* NOP */
    #define CHANGE_HWI(hwiHandle, hwiFXN) \
        Hwi_setFunc(hwiHandle, hwiFXN, NULL);
#else
    #warn "Target not Supported"
    /* If a target throws this warning it has not been extensively tested */
    #define HWI_INT 13
    #define CONFIGURE_INTERRUPTS() /* NOP */
    #define TRIGGER_INTERRUPT() \
        Hwi_post(HWI_INT);
    #define CLEAR_INTERRUPT() /* NOP */
    #define CHANGE_HWI(hwiHandle, hwiFXN) \
        Hwi_setFunc(hwiHandle, hwiFXN, NULL);
#endif

/*
 * ======== benchmarkTask ========
 */
Void benchmarkTask(UArg a0, UArg a1)
{
    UInt32 total;         /* temporary variable used by LOOP() */
    UInt32 i;             /* temporary variable used for LOOP ctrl */
    UInt32 min;           /* minimum cycle count for LOOP (output) */
    UInt32 max;           /* maximum cycle count for LOOP (output) */
    UInt32 avg;           /* average cycle count for LOOP (output) */
    UInt32 minTimestamp;  /* used to keep min Timestamp cycle count */
    UInt32 avgTimestamp;  /* used to keep avg Timestamp cycle count */
    UInt key;             /* used to store previous IEB when disabling */
    Float factor;         /* used for clock ratio cpu/timestamp */
    Types_FreqHz freq1, freq2;     /* used to keep BIOS freq and Timestamp freq */
    Task_Handle task2;
    Task_Params taskParams;

    /*
     * This will calculate the factor needed top correlate Timestamp delta
     * to the CPU cycles
     */
    Timestamp_getFreq(&freq1);
    BIOS_getCpuFreq(&freq2);
    factor = (Float)freq2.lo/freq1.lo;
    //System_printf("%lu\t%lu\t%lu\t Timestamp Freq, BIOS Freq, Factor\n",\
    freq1.lo, freq2.lo, (UInt32) factor);

    /*
     * This will calculate the overhead of making a Timestamp_get32()
     * Function call. The ";" is to hold the place of "FXN" as the input param
     * replacing the ";" with " " is valid but more easily overlooked
     */
    LOOP(TIME(;));
    minTimestamp=min;
    avgTimestamp=avg;
    //System_printf("%lu\t%lu\t%lu\t Timestamps\n",\
    (UInt32)(minTimestamp * factor), (UInt32)(maxTimestamp * factor), (UInt32)(avgTimestamp * factor));

    CONFIGURE_INTERRUPTS();

    LOOP(
        key = Hwi_disable();
        TIME(Hwi_restore(key));
    );
    System_printf("%lu\t%lu\t%lu\t Hwi_restore()\n",\
    (UInt32)((min - minTimestamp) * factor), (UInt32)((max - avgTimestamp) * factor), (UInt32)((avg-avgTimestamp) * factor));

    LOOP(
        TIME(key = Hwi_disable());
        Hwi_restore(key);
    );
    System_printf("%lu\t%lu\t%lu\t Hwi_disable()\n",\
    (UInt32)((min - minTimestamp) * factor), (UInt32)((max - avgTimestamp) * factor), (UInt32)((avg-avgTimestamp) * factor));

    CHANGE_HWI(hwi1, &hwi1Func);
    LOOP(
        t1 = Timestamp_get32();
        TRIGGER_INTERRUPT();
        delta = t2; 
        delta = delta - t1;
    );
    System_printf("%lu\t%lu\t%lu\t Hwi dispatcher prolog\n",\
    (UInt32)((min - minTimestamp) * factor), (UInt32)((max - avgTimestamp) * factor), (UInt32)((avg-avgTimestamp) * factor));

    LOOP(
        TRIGGER_INTERRUPT();
        t1 = Timestamp_get32();
        delta = t1; 
        delta = delta - t2;
    );
    System_printf("%lu\t%lu\t%lu\t Hwi dispatcher epilog\n",\
    (UInt32)((min - minTimestamp) * factor), (UInt32)((max - avgTimestamp) * factor), (UInt32)((avg-avgTimestamp) * factor));

    CHANGE_HWI(hwi1, &hwi2Func);  /* change to empty Hwi */
    LOOP(TIME(TRIGGER_INTERRUPT()));
    System_printf("%lu\t%lu\t%lu\t Hwi dispatcher()\n",\
    (UInt32)((min - minTimestamp) * factor), (UInt32)((max - avgTimestamp) * factor), (UInt32)((avg-avgTimestamp) * factor));

    CHANGE_HWI(hwi1, &hwi3Func);  /* change to start task Hwi */
    Task_setPri(task1, 3);  /* Make task1 higher priority than this task */
    LOOP(
        t1 = Timestamp_get32();
        TRIGGER_INTERRUPT();
        delta = t2; 
        delta = delta - t1;
    );
    System_printf("%lu\t%lu\t%lu\t Hardware Interrupt to Blocked Task\n",\
    (UInt32)((min - minTimestamp) * factor), (UInt32)((max - avgTimestamp) * factor), (UInt32)((avg-avgTimestamp) * factor));

    CHANGE_HWI(hwi1, &hwi4Func);  /* change to post swi task */
    LOOP(
        t1 = Timestamp_get32();
        TRIGGER_INTERRUPT();
        delta = t2; 
        delta = delta - t1;
    );
    System_printf("%lu\t%lu\t%lu\t Hardware Interrupt to Software Interrupt\n"\
    ,(UInt32)((min - minTimestamp) * factor), (UInt32)((max - avgTimestamp) * factor), (UInt32)((avg-avgTimestamp) * factor));

    LOOP(
        Swi_disable();
        TIME(Swi_enable());
    );
    System_printf("%lu\t%lu\t%lu\t Swi_enable()\n",\
    (UInt32)((min - minTimestamp) * factor), (UInt32)((max - avgTimestamp) * factor), (UInt32)((avg-avgTimestamp) * factor));

    LOOP(
        TIME(Swi_disable());
        Swi_enable();
    );
    System_printf("%lu\t%lu\t%lu\t Swi_disable()\n",\
    (UInt32)((min - minTimestamp) * factor), (UInt32)((max - avgTimestamp) * factor), (UInt32)((avg-avgTimestamp) * factor));
    
    Swi_disable();
    Swi_post(swi1);
    LOOP(TIME(Swi_post(swi1)));
    System_printf("%lu\t%lu\t%lu\t Post Software Interrupt Again\
\n", (UInt32)((min - minTimestamp) * factor), (UInt32)((max - avgTimestamp) * factor), (UInt32)((avg-avgTimestamp) * factor));
    Swi_enable();

    LOOP(Swi_post(swi2));
    System_printf("%lu\t%lu\t%lu\t Post Software Interrupt without Context \
Switch\n", (UInt32)((min - minTimestamp) * factor), (UInt32)((max - avgTimestamp) * factor), (UInt32)((avg-avgTimestamp) * factor));

    LOOP(
        t1 = Timestamp_get32();
        Swi_post(swi1);
        delta = t2; 
        delta = delta - t1;
    );
    System_printf("%lu\t%lu\t%lu\t Post Software Interrupt with Context \
Switch\n", (UInt32)((min - minTimestamp) * factor), (UInt32)((max - avgTimestamp) * factor), (UInt32)((avg-avgTimestamp) * factor));

    Task_Params_init(&taskParams);
    taskParams.priority = 2;  /* same priority as $this */
    taskParams.stackSize = 512;
    LOOP(
        TIME(task2 = Task_create(task2Func,&taskParams, NULL));
        Task_delete(&task2);
    );
    System_printf("%lu\t%lu\t%lu\t Create a New Task without Context \
Switch\n", (UInt32)((min - minTimestamp) * factor), (UInt32)((max - avgTimestamp) * factor), (UInt32)((avg-avgTimestamp) * factor));

    /*  task1 is blocked */
    LOOP(
        TIME(Task_setPri(task1, 1));
        Task_setPri(task1, 2);
        /* raise the priority so that function above runs full */
    );
    System_printf("%lu\t%lu\t%lu\t Set a Task Priority without a Context \
Switch\n", (UInt32)((min - minTimestamp) * factor), (UInt32)((max - avgTimestamp) * factor), (UInt32)((avg-avgTimestamp) * factor));

    /* task1 is same priority from operations above */
    LOOP(
        Semaphore_post(sema2);
        t1 = Timestamp_get32();
        Task_yield();
        delta = t2; 
        delta = delta - t1;
    );
    System_printf("%lu\t%lu\t%lu\t Task_yield\n",\
    (UInt32)((min - minTimestamp) * factor), (UInt32)((max - avgTimestamp) * factor), (UInt32)((avg-avgTimestamp) * factor));

    LOOP(TIME(Semaphore_post(sema1)));
    System_printf("%lu\t%lu\t%lu\t Post Semaphore, No Waiting Task\n",\
    (UInt32)((min - minTimestamp) * factor), (UInt32)((max - avgTimestamp) * factor), (UInt32)((avg-avgTimestamp) * factor));

    LOOP(
        Task_yield();
        /*
         *  1. Put task1Func back in blocked state
         *  2. Posting semaphore2 will not cause task switch as task1 is
         *     same priority
         *  3. Delta is calculated in "TIME()" so t2 from task1 is
         *     not of concern
         */
        TIME(Semaphore_post(sema2));
    );
    System_printf("%lu\t%lu\t%lu\t Post Semaphore No Task Switch\n",\
    (UInt32)((min - minTimestamp) * factor), (UInt32)((max - avgTimestamp) * factor), (UInt32)((avg-avgTimestamp) * factor));

    Task_setPri(task1, 3);  /* Make task1 a higher priority */
    LOOP(
        t1 = Timestamp_get32();
        Semaphore_post(sema2);
        delta = t2; 
        delta = delta - t1;
    );
    System_printf("%lu\t%lu\t%lu\t Post Semaphore with Task Switch\n",\
    (UInt32)((min - minTimestamp) * factor), (UInt32)((max - avgTimestamp) * factor), (UInt32)((avg-avgTimestamp) * factor));

    /* sema1 = NUM_LOOPS from "post semaphore again" above */
    LOOP(TIME(Semaphore_pend(sema1, BIOS_WAIT_FOREVER)));
    System_printf("%lu\t%lu\t%lu\t Pend on Semaphore, No Context Switch\n",\
    (UInt32)((min - minTimestamp) * factor), (UInt32)((max - avgTimestamp) * factor), (UInt32)((avg-avgTimestamp) * factor));

    /* Task 1 is blocked as it is a higher priority and we're running here */
    Task_setPri(task1, 2);      /* Make task1 the same priority as this */
    Semaphore_reset(sema1, 0);   /* Clear sema1 so we can re-use */
    LOOP(
            Semaphore_post(sema2); /* Ready task1 */
            t1 = Timestamp_get32();
            Semaphore_pend(sema1, BIOS_WAIT_FOREVER);
            delta = t2; 
            delta = delta - t1;
        );
    System_printf("%lu\t%lu\t%lu\t Pend on Semaphore with Task Switch\n",\
    (UInt32)((min - minTimestamp) * factor), (UInt32)((max - avgTimestamp) * factor), (UInt32)((avg-avgTimestamp) * factor));

    LOOP(TIME(Clock_getTicks()));
    System_printf("%lu\t%lu\t%lu\t Clock_getTicks\n",\
    (UInt32)((min - minTimestamp) * factor), (UInt32)((max - avgTimestamp) * factor), (UInt32)((avg-avgTimestamp) * factor));

    BIOS_exit(1);
}

/*
 *  ======== main ========
 */
int main()
{ 
    BIOS_start();
    return 0;
}

/*
 *  ======== swi1Func ========
 */
Void swi1Func(Void)
{
    t2 = Timestamp_get32();
}

/*
 *  ======== swi2Func =======
 */
Void swi2Func(Void)
{
    TIME(Swi_post(swi1));
}

/*
 *  ======== hwi1Func ========
 */
Void hwi1Func(UArg a0)
{
    CLEAR_INTERRUPT();
    t2 = Timestamp_get32();
}

/*
 *  ======== hwi2Func ========
 */
Void hwi2Func(UArg a0)
{
    CLEAR_INTERRUPT();
}

/*
 *  ======== hwi3Func ========
 */
Void hwi3Func(UArg a0)
{
    Semaphore_post(sema2);
    CLEAR_INTERRUPT();
}

/*
 *  ======== hwi4Func ========
 */
Void hwi4Func(UArg a0)
{
    Swi_post(swi1);
    CLEAR_INTERRUPT();
}

/*
 *  ======== genericHwiFunc ========
 */
Void genericHwiFunc(UArg a0)
{
    doHwi(a0);
}

/*
 *  ======== task1Func ========
 *
 */
Void task1Func(UArg a0, UArg a1)
{
    while (1) {
        Semaphore_pend(sema2, BIOS_WAIT_FOREVER);
        t2 = Timestamp_get32();
        Semaphore_post(sema1);
    }
}

/*
 *  ======== task2Func ========
 */
Void task2Func(UArg a0, UArg a1)
{
}
