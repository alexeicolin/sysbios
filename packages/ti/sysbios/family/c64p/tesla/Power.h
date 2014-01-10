/*
 *  Do not modify this file; it is automatically 
 *  generated and any modifications will be overwritten.
 *
 * @(#) xdc-y51
 */

/*
 * ======== GENERATED SECTIONS ========
 *     
 *     PROLOGUE
 *     INCLUDES
 *     
 *     INTERNAL DEFINITIONS
 *     MODULE-WIDE CONFIGS
 *     FUNCTION DECLARATIONS
 *     SYSTEM FUNCTIONS
 *     
 *     EPILOGUE
 *     STATE STRUCTURES
 *     PREFIX ALIASES
 */


/*
 * ======== PROLOGUE ========
 */

#ifndef ti_sysbios_family_c64p_tesla_Power__include
#define ti_sysbios_family_c64p_tesla_Power__include

#ifndef __nested__
#define __nested__
#define ti_sysbios_family_c64p_tesla_Power__top__
#endif

#ifdef __cplusplus
#define __extern extern "C"
#else
#define __extern extern
#endif

#define ti_sysbios_family_c64p_tesla_Power___VERS 150


/*
 * ======== INCLUDES ========
 */

#include <xdc/std.h>

#include <xdc/runtime/xdc.h>
#include <xdc/runtime/Types.h>
#include <ti/sysbios/family/c64p/tesla/package/package.defs.h>

#include <xdc/runtime/IModule.h>


/*
 * ======== AUXILIARY DEFINITIONS ========
 */

/* Suspend */
enum ti_sysbios_family_c64p_tesla_Power_Suspend {
    ti_sysbios_family_c64p_tesla_Power_Suspend_RETENTION,
    ti_sysbios_family_c64p_tesla_Power_Suspend_HIBERNATE
};
typedef enum ti_sysbios_family_c64p_tesla_Power_Suspend ti_sysbios_family_c64p_tesla_Power_Suspend;


/*
 * ======== INTERNAL DEFINITIONS ========
 */

/* CpuSysRegs */
struct ti_sysbios_family_c64p_tesla_Power_CpuSysRegs {
    xdc_UInt32 AMR;
    xdc_UInt32 CSR;
    xdc_UInt32 IER;
    xdc_UInt32 ISTP;
    xdc_UInt32 IRP;
    xdc_UInt32 SSR;
    xdc_UInt32 GPLYB;
    xdc_UInt32 GFPGFR;
    xdc_UInt32 TSR;
    xdc_UInt32 ITSR;
    xdc_UInt32 IERR;
};

/* IntcConfig */
struct ti_sysbios_family_c64p_tesla_Power_IntcConfig {
    xdc_UInt32 EVTMASK0;
    xdc_UInt32 EVTMASK1;
    xdc_UInt32 EVTMASK2;
    xdc_UInt32 EVTMASK3;
    xdc_UInt32 EXPMASK0;
    xdc_UInt32 EXPMASK1;
    xdc_UInt32 EXPMASK2;
    xdc_UInt32 EXPMASK3;
    xdc_UInt32 INTMUX1;
    xdc_UInt32 INTMUX2;
    xdc_UInt32 INTMUX3;
    xdc_UInt32 AEGMUX0;
    xdc_UInt32 AEGMUX1;
    xdc_UInt32 INTDMASK;
};

/* SyscConfig */
struct ti_sysbios_family_c64p_tesla_Power_SyscConfig {
    xdc_UInt32 SYSCONFIG;
    xdc_UInt32 VBUSM2OCP;
    xdc_UInt32 EDMA;
    xdc_UInt32 CORE;
    xdc_UInt32 IVA_ICTRL;
    xdc_UInt32 IDLEDLY;
};

/* UnicacheConfig */
struct ti_sysbios_family_c64p_tesla_Power_UnicacheConfig {
    xdc_UInt32 CONFIG;
    xdc_UInt32 OCP;
};

/* AmmuConfig */
typedef xdc_UInt32 __T1_ti_sysbios_family_c64p_tesla_Power_AmmuConfig__largeAddr;
typedef xdc_UInt32 __ARRAY1_ti_sysbios_family_c64p_tesla_Power_AmmuConfig__largeAddr[8];
typedef __ARRAY1_ti_sysbios_family_c64p_tesla_Power_AmmuConfig__largeAddr __TA_ti_sysbios_family_c64p_tesla_Power_AmmuConfig__largeAddr;
typedef xdc_UInt32 __T1_ti_sysbios_family_c64p_tesla_Power_AmmuConfig__largePolicy;
typedef xdc_UInt32 __ARRAY1_ti_sysbios_family_c64p_tesla_Power_AmmuConfig__largePolicy[8];
typedef __ARRAY1_ti_sysbios_family_c64p_tesla_Power_AmmuConfig__largePolicy __TA_ti_sysbios_family_c64p_tesla_Power_AmmuConfig__largePolicy;
typedef xdc_UInt32 __T1_ti_sysbios_family_c64p_tesla_Power_AmmuConfig__medAddr;
typedef xdc_UInt32 __ARRAY1_ti_sysbios_family_c64p_tesla_Power_AmmuConfig__medAddr[7];
typedef __ARRAY1_ti_sysbios_family_c64p_tesla_Power_AmmuConfig__medAddr __TA_ti_sysbios_family_c64p_tesla_Power_AmmuConfig__medAddr;
typedef xdc_UInt32 __T1_ti_sysbios_family_c64p_tesla_Power_AmmuConfig__medPolicy;
typedef xdc_UInt32 __ARRAY1_ti_sysbios_family_c64p_tesla_Power_AmmuConfig__medPolicy[7];
typedef __ARRAY1_ti_sysbios_family_c64p_tesla_Power_AmmuConfig__medPolicy __TA_ti_sysbios_family_c64p_tesla_Power_AmmuConfig__medPolicy;
typedef xdc_UInt32 __T1_ti_sysbios_family_c64p_tesla_Power_AmmuConfig__smallAddr;
typedef xdc_UInt32 __ARRAY1_ti_sysbios_family_c64p_tesla_Power_AmmuConfig__smallAddr[3];
typedef __ARRAY1_ti_sysbios_family_c64p_tesla_Power_AmmuConfig__smallAddr __TA_ti_sysbios_family_c64p_tesla_Power_AmmuConfig__smallAddr;
typedef xdc_UInt32 __T1_ti_sysbios_family_c64p_tesla_Power_AmmuConfig__smallPolicy;
typedef xdc_UInt32 __ARRAY1_ti_sysbios_family_c64p_tesla_Power_AmmuConfig__smallPolicy[3];
typedef __ARRAY1_ti_sysbios_family_c64p_tesla_Power_AmmuConfig__smallPolicy __TA_ti_sysbios_family_c64p_tesla_Power_AmmuConfig__smallPolicy;
struct ti_sysbios_family_c64p_tesla_Power_AmmuConfig {
    __TA_ti_sysbios_family_c64p_tesla_Power_AmmuConfig__largeAddr largeAddr;
    __TA_ti_sysbios_family_c64p_tesla_Power_AmmuConfig__largePolicy largePolicy;
    __TA_ti_sysbios_family_c64p_tesla_Power_AmmuConfig__medAddr medAddr;
    __TA_ti_sysbios_family_c64p_tesla_Power_AmmuConfig__medPolicy medPolicy;
    __TA_ti_sysbios_family_c64p_tesla_Power_AmmuConfig__smallAddr smallAddr;
    __TA_ti_sysbios_family_c64p_tesla_Power_AmmuConfig__smallPolicy smallPolicy;
    xdc_UInt32 mmuConfig;
};

/* EdmaConfig */
typedef xdc_UInt32 __T1_ti_sysbios_family_c64p_tesla_Power_EdmaConfig__DCHMAP;
typedef xdc_UInt32 __ARRAY1_ti_sysbios_family_c64p_tesla_Power_EdmaConfig__DCHMAP[64];
typedef __ARRAY1_ti_sysbios_family_c64p_tesla_Power_EdmaConfig__DCHMAP __TA_ti_sysbios_family_c64p_tesla_Power_EdmaConfig__DCHMAP;
typedef xdc_UInt32 __T1_ti_sysbios_family_c64p_tesla_Power_EdmaConfig__QCHMAP;
typedef xdc_UInt32 __ARRAY1_ti_sysbios_family_c64p_tesla_Power_EdmaConfig__QCHMAP[8];
typedef __ARRAY1_ti_sysbios_family_c64p_tesla_Power_EdmaConfig__QCHMAP __TA_ti_sysbios_family_c64p_tesla_Power_EdmaConfig__QCHMAP;
typedef xdc_UInt32 __T1_ti_sysbios_family_c64p_tesla_Power_EdmaConfig__DMAQNUM;
typedef xdc_UInt32 __ARRAY1_ti_sysbios_family_c64p_tesla_Power_EdmaConfig__DMAQNUM[8];
typedef __ARRAY1_ti_sysbios_family_c64p_tesla_Power_EdmaConfig__DMAQNUM __TA_ti_sysbios_family_c64p_tesla_Power_EdmaConfig__DMAQNUM;
typedef xdc_UInt32 __T1_ti_sysbios_family_c64p_tesla_Power_EdmaConfig__regionAccessBits;
typedef xdc_UInt32 __ARRAY1_ti_sysbios_family_c64p_tesla_Power_EdmaConfig__regionAccessBits[24];
typedef __ARRAY1_ti_sysbios_family_c64p_tesla_Power_EdmaConfig__regionAccessBits __TA_ti_sysbios_family_c64p_tesla_Power_EdmaConfig__regionAccessBits;
typedef xdc_UInt32 __T1_ti_sysbios_family_c64p_tesla_Power_EdmaConfig__PaRAMs;
typedef xdc_UInt32 __ARRAY1_ti_sysbios_family_c64p_tesla_Power_EdmaConfig__PaRAMs[1024];
typedef __ARRAY1_ti_sysbios_family_c64p_tesla_Power_EdmaConfig__PaRAMs __TA_ti_sysbios_family_c64p_tesla_Power_EdmaConfig__PaRAMs;
struct ti_sysbios_family_c64p_tesla_Power_EdmaConfig {
    xdc_UInt32 CLKGDIS;
    __TA_ti_sysbios_family_c64p_tesla_Power_EdmaConfig__DCHMAP DCHMAP;
    __TA_ti_sysbios_family_c64p_tesla_Power_EdmaConfig__QCHMAP QCHMAP;
    __TA_ti_sysbios_family_c64p_tesla_Power_EdmaConfig__DMAQNUM DMAQNUM;
    xdc_UInt32 QDMAQNUM;
    xdc_UInt32 QUETCMAP;
    xdc_UInt32 QUEPRI;
    __TA_ti_sysbios_family_c64p_tesla_Power_EdmaConfig__regionAccessBits regionAccessBits;
    xdc_UInt32 QWMTHRA;
    xdc_UInt32 AETCTL;
    xdc_UInt32 IER;
    xdc_UInt32 IERH;
    xdc_UInt32 QEER;
    __TA_ti_sysbios_family_c64p_tesla_Power_EdmaConfig__PaRAMs PaRAMs;
};

/* SubsystemContext */
struct ti_sysbios_family_c64p_tesla_Power_SubsystemContext {
    ti_sysbios_family_c64p_tesla_Power_CpuSysRegs cpuSysRegs;
    ti_sysbios_family_c64p_tesla_Power_UnicacheConfig configL1;
    ti_sysbios_family_c64p_tesla_Power_UnicacheConfig configL2;
    ti_sysbios_family_c64p_tesla_Power_IntcConfig configINTC;
    ti_sysbios_family_c64p_tesla_Power_SyscConfig configSYSC;
    ti_sysbios_family_c64p_tesla_Power_AmmuConfig configAMMU;
    ti_sysbios_family_c64p_tesla_Power_EdmaConfig configEDMA;
    xdc_Bool tscRunning;
};


/*
 * ======== MODULE-WIDE CONFIGS ========
 */

/* Module__diagsEnabled */
typedef xdc_Bits32 CT__ti_sysbios_family_c64p_tesla_Power_Module__diagsEnabled;
__extern __FAR__ const CT__ti_sysbios_family_c64p_tesla_Power_Module__diagsEnabled ti_sysbios_family_c64p_tesla_Power_Module__diagsEnabled__C;

/* Module__diagsIncluded */
typedef xdc_Bits32 CT__ti_sysbios_family_c64p_tesla_Power_Module__diagsIncluded;
__extern __FAR__ const CT__ti_sysbios_family_c64p_tesla_Power_Module__diagsIncluded ti_sysbios_family_c64p_tesla_Power_Module__diagsIncluded__C;

/* Module__diagsMask */
typedef xdc_Bits16* CT__ti_sysbios_family_c64p_tesla_Power_Module__diagsMask;
__extern __FAR__ const CT__ti_sysbios_family_c64p_tesla_Power_Module__diagsMask ti_sysbios_family_c64p_tesla_Power_Module__diagsMask__C;

/* Module__gateObj */
typedef xdc_Ptr CT__ti_sysbios_family_c64p_tesla_Power_Module__gateObj;
__extern __FAR__ const CT__ti_sysbios_family_c64p_tesla_Power_Module__gateObj ti_sysbios_family_c64p_tesla_Power_Module__gateObj__C;

/* Module__gatePrms */
typedef xdc_Ptr CT__ti_sysbios_family_c64p_tesla_Power_Module__gatePrms;
__extern __FAR__ const CT__ti_sysbios_family_c64p_tesla_Power_Module__gatePrms ti_sysbios_family_c64p_tesla_Power_Module__gatePrms__C;

/* Module__id */
typedef xdc_runtime_Types_ModuleId CT__ti_sysbios_family_c64p_tesla_Power_Module__id;
__extern __FAR__ const CT__ti_sysbios_family_c64p_tesla_Power_Module__id ti_sysbios_family_c64p_tesla_Power_Module__id__C;

/* Module__loggerDefined */
typedef xdc_Bool CT__ti_sysbios_family_c64p_tesla_Power_Module__loggerDefined;
__extern __FAR__ const CT__ti_sysbios_family_c64p_tesla_Power_Module__loggerDefined ti_sysbios_family_c64p_tesla_Power_Module__loggerDefined__C;

/* Module__loggerObj */
typedef xdc_Ptr CT__ti_sysbios_family_c64p_tesla_Power_Module__loggerObj;
__extern __FAR__ const CT__ti_sysbios_family_c64p_tesla_Power_Module__loggerObj ti_sysbios_family_c64p_tesla_Power_Module__loggerObj__C;

/* Module__loggerFxn0 */
typedef xdc_runtime_Types_LoggerFxn0 CT__ti_sysbios_family_c64p_tesla_Power_Module__loggerFxn0;
__extern __FAR__ const CT__ti_sysbios_family_c64p_tesla_Power_Module__loggerFxn0 ti_sysbios_family_c64p_tesla_Power_Module__loggerFxn0__C;

/* Module__loggerFxn1 */
typedef xdc_runtime_Types_LoggerFxn1 CT__ti_sysbios_family_c64p_tesla_Power_Module__loggerFxn1;
__extern __FAR__ const CT__ti_sysbios_family_c64p_tesla_Power_Module__loggerFxn1 ti_sysbios_family_c64p_tesla_Power_Module__loggerFxn1__C;

/* Module__loggerFxn2 */
typedef xdc_runtime_Types_LoggerFxn2 CT__ti_sysbios_family_c64p_tesla_Power_Module__loggerFxn2;
__extern __FAR__ const CT__ti_sysbios_family_c64p_tesla_Power_Module__loggerFxn2 ti_sysbios_family_c64p_tesla_Power_Module__loggerFxn2__C;

/* Module__loggerFxn4 */
typedef xdc_runtime_Types_LoggerFxn4 CT__ti_sysbios_family_c64p_tesla_Power_Module__loggerFxn4;
__extern __FAR__ const CT__ti_sysbios_family_c64p_tesla_Power_Module__loggerFxn4 ti_sysbios_family_c64p_tesla_Power_Module__loggerFxn4__C;

/* Module__loggerFxn8 */
typedef xdc_runtime_Types_LoggerFxn8 CT__ti_sysbios_family_c64p_tesla_Power_Module__loggerFxn8;
__extern __FAR__ const CT__ti_sysbios_family_c64p_tesla_Power_Module__loggerFxn8 ti_sysbios_family_c64p_tesla_Power_Module__loggerFxn8__C;

/* Module__startupDoneFxn */
typedef xdc_Bool (*CT__ti_sysbios_family_c64p_tesla_Power_Module__startupDoneFxn)(void);
__extern __FAR__ const CT__ti_sysbios_family_c64p_tesla_Power_Module__startupDoneFxn ti_sysbios_family_c64p_tesla_Power_Module__startupDoneFxn__C;

/* Object__count */
typedef xdc_Int CT__ti_sysbios_family_c64p_tesla_Power_Object__count;
__extern __FAR__ const CT__ti_sysbios_family_c64p_tesla_Power_Object__count ti_sysbios_family_c64p_tesla_Power_Object__count__C;

/* Object__heap */
typedef xdc_runtime_IHeap_Handle CT__ti_sysbios_family_c64p_tesla_Power_Object__heap;
__extern __FAR__ const CT__ti_sysbios_family_c64p_tesla_Power_Object__heap ti_sysbios_family_c64p_tesla_Power_Object__heap__C;

/* Object__sizeof */
typedef xdc_SizeT CT__ti_sysbios_family_c64p_tesla_Power_Object__sizeof;
__extern __FAR__ const CT__ti_sysbios_family_c64p_tesla_Power_Object__sizeof ti_sysbios_family_c64p_tesla_Power_Object__sizeof__C;

/* Object__table */
typedef xdc_Ptr CT__ti_sysbios_family_c64p_tesla_Power_Object__table;
__extern __FAR__ const CT__ti_sysbios_family_c64p_tesla_Power_Object__table ti_sysbios_family_c64p_tesla_Power_Object__table__C;

/* tpccRegs */
#define ti_sysbios_family_c64p_tesla_Power_tpccRegs (ti_sysbios_family_c64p_tesla_Power_tpccRegs__C)
typedef xdc_Ptr CT__ti_sysbios_family_c64p_tesla_Power_tpccRegs;
__extern __FAR__ const CT__ti_sysbios_family_c64p_tesla_Power_tpccRegs ti_sysbios_family_c64p_tesla_Power_tpccRegs__C;

/* syscRegs */
#define ti_sysbios_family_c64p_tesla_Power_syscRegs (ti_sysbios_family_c64p_tesla_Power_syscRegs__C)
typedef xdc_Ptr CT__ti_sysbios_family_c64p_tesla_Power_syscRegs;
__extern __FAR__ const CT__ti_sysbios_family_c64p_tesla_Power_syscRegs ti_sysbios_family_c64p_tesla_Power_syscRegs__C;

/* idle */
#ifdef ti_sysbios_family_c64p_tesla_Power_idle__D
#define ti_sysbios_family_c64p_tesla_Power_idle (ti_sysbios_family_c64p_tesla_Power_idle__D)
#else
#define ti_sysbios_family_c64p_tesla_Power_idle (ti_sysbios_family_c64p_tesla_Power_idle__C)
typedef xdc_Bool CT__ti_sysbios_family_c64p_tesla_Power_idle;
__extern __FAR__ const CT__ti_sysbios_family_c64p_tesla_Power_idle ti_sysbios_family_c64p_tesla_Power_idle__C;
#endif


/*
 * ======== FUNCTION DECLARATIONS ========
 */

/* Module_startup */
#define ti_sysbios_family_c64p_tesla_Power_Module_startup( state ) (-1)

/* Module__startupDone__S */
xdc__CODESECT(ti_sysbios_family_c64p_tesla_Power_Module__startupDone__S, "ti_sysbios_family_c64p_tesla_Power_Module__startupDone")
__extern xdc_Bool ti_sysbios_family_c64p_tesla_Power_Module__startupDone__S( void );

/* idleCPU__E */
#define ti_sysbios_family_c64p_tesla_Power_idleCPU ti_sysbios_family_c64p_tesla_Power_idleCPU__E
xdc__CODESECT(ti_sysbios_family_c64p_tesla_Power_idleCPU__E, "ti_sysbios_family_c64p_tesla_Power_idleCPU")
__extern xdc_Void ti_sysbios_family_c64p_tesla_Power_idleCPU__E( void );
xdc__CODESECT(ti_sysbios_family_c64p_tesla_Power_idleCPU__F, "ti_sysbios_family_c64p_tesla_Power_idleCPU")
__extern xdc_Void ti_sysbios_family_c64p_tesla_Power_idleCPU__F( void );
__extern xdc_Void ti_sysbios_family_c64p_tesla_Power_idleCPU__R( void );

/* suspend__E */
#define ti_sysbios_family_c64p_tesla_Power_suspend ti_sysbios_family_c64p_tesla_Power_suspend__E
xdc__CODESECT(ti_sysbios_family_c64p_tesla_Power_suspend__E, "ti_sysbios_family_c64p_tesla_Power_suspend")
__extern xdc_UInt ti_sysbios_family_c64p_tesla_Power_suspend__E( ti_sysbios_family_c64p_tesla_Power_Suspend level );
xdc__CODESECT(ti_sysbios_family_c64p_tesla_Power_suspend__F, "ti_sysbios_family_c64p_tesla_Power_suspend")
__extern xdc_UInt ti_sysbios_family_c64p_tesla_Power_suspend__F( ti_sysbios_family_c64p_tesla_Power_Suspend level );
__extern xdc_UInt ti_sysbios_family_c64p_tesla_Power_suspend__R( ti_sysbios_family_c64p_tesla_Power_Suspend level );

/* resetFxn__I */
#define ti_sysbios_family_c64p_tesla_Power_resetFxn ti_sysbios_family_c64p_tesla_Power_resetFxn__I
xdc__CODESECT(ti_sysbios_family_c64p_tesla_Power_resetFxn__I, "ti_sysbios_family_c64p_tesla_Power_resetFxn")
__extern xdc_Void ti_sysbios_family_c64p_tesla_Power_resetFxn__I( void );

/* standby__I */
#define ti_sysbios_family_c64p_tesla_Power_standby ti_sysbios_family_c64p_tesla_Power_standby__I
xdc__CODESECT(ti_sysbios_family_c64p_tesla_Power_standby__I, "ti_sysbios_family_c64p_tesla_Power_standby")
__extern xdc_UInt ti_sysbios_family_c64p_tesla_Power_standby__I( xdc_UInt32* cpuRegs );


/*
 * ======== SYSTEM FUNCTIONS ========
 */

/* Module_startupDone */
#define ti_sysbios_family_c64p_tesla_Power_Module_startupDone() ti_sysbios_family_c64p_tesla_Power_Module__startupDone__S()

/* Object_heap */
#define ti_sysbios_family_c64p_tesla_Power_Object_heap() ti_sysbios_family_c64p_tesla_Power_Object__heap__C

/* Module_heap */
#define ti_sysbios_family_c64p_tesla_Power_Module_heap() ti_sysbios_family_c64p_tesla_Power_Object__heap__C

/* Module_id */
static inline CT__ti_sysbios_family_c64p_tesla_Power_Module__id ti_sysbios_family_c64p_tesla_Power_Module_id( void ) 
{
    return ti_sysbios_family_c64p_tesla_Power_Module__id__C;
}

/* Module_hasMask */
static inline xdc_Bool ti_sysbios_family_c64p_tesla_Power_Module_hasMask( void ) 
{
    return ti_sysbios_family_c64p_tesla_Power_Module__diagsMask__C != NULL;
}

/* Module_getMask */
static inline xdc_Bits16 ti_sysbios_family_c64p_tesla_Power_Module_getMask( void ) 
{
    return ti_sysbios_family_c64p_tesla_Power_Module__diagsMask__C != NULL ? *ti_sysbios_family_c64p_tesla_Power_Module__diagsMask__C : 0;
}

/* Module_setMask */
static inline xdc_Void ti_sysbios_family_c64p_tesla_Power_Module_setMask( xdc_Bits16 mask ) 
{
    if (ti_sysbios_family_c64p_tesla_Power_Module__diagsMask__C != NULL) *ti_sysbios_family_c64p_tesla_Power_Module__diagsMask__C = mask;
}


/*
 * ======== EPILOGUE ========
 */

#ifdef ti_sysbios_family_c64p_tesla_Power__top__
#undef __nested__
#endif

#endif /* ti_sysbios_family_c64p_tesla_Power__include */


/*
 * ======== STATE STRUCTURES ========
 */

#if defined(__config__) || (!defined(__nested__) && defined(ti_sysbios_family_c64p_tesla_Power__internalaccess))

#ifndef ti_sysbios_family_c64p_tesla_Power__include_state
#define ti_sysbios_family_c64p_tesla_Power__include_state


#endif /* ti_sysbios_family_c64p_tesla_Power__include_state */

#endif

/*
 * ======== PREFIX ALIASES ========
 */

#if !defined(__nested__) && !defined(ti_sysbios_family_c64p_tesla_Power__nolocalnames)

#ifndef ti_sysbios_family_c64p_tesla_Power__localnames__done
#define ti_sysbios_family_c64p_tesla_Power__localnames__done

/* module prefix */
#define Power_Suspend ti_sysbios_family_c64p_tesla_Power_Suspend
#define Power_CpuSysRegs ti_sysbios_family_c64p_tesla_Power_CpuSysRegs
#define Power_IntcConfig ti_sysbios_family_c64p_tesla_Power_IntcConfig
#define Power_SyscConfig ti_sysbios_family_c64p_tesla_Power_SyscConfig
#define Power_UnicacheConfig ti_sysbios_family_c64p_tesla_Power_UnicacheConfig
#define Power_AmmuConfig ti_sysbios_family_c64p_tesla_Power_AmmuConfig
#define Power_EdmaConfig ti_sysbios_family_c64p_tesla_Power_EdmaConfig
#define Power_SubsystemContext ti_sysbios_family_c64p_tesla_Power_SubsystemContext
#define Power_Suspend_RETENTION ti_sysbios_family_c64p_tesla_Power_Suspend_RETENTION
#define Power_Suspend_HIBERNATE ti_sysbios_family_c64p_tesla_Power_Suspend_HIBERNATE
#define Power_tpccRegs ti_sysbios_family_c64p_tesla_Power_tpccRegs
#define Power_syscRegs ti_sysbios_family_c64p_tesla_Power_syscRegs
#define Power_idle ti_sysbios_family_c64p_tesla_Power_idle
#define Power_idleCPU ti_sysbios_family_c64p_tesla_Power_idleCPU
#define Power_suspend ti_sysbios_family_c64p_tesla_Power_suspend
#define Power_Module_name ti_sysbios_family_c64p_tesla_Power_Module_name
#define Power_Module_id ti_sysbios_family_c64p_tesla_Power_Module_id
#define Power_Module_startup ti_sysbios_family_c64p_tesla_Power_Module_startup
#define Power_Module_startupDone ti_sysbios_family_c64p_tesla_Power_Module_startupDone
#define Power_Module_hasMask ti_sysbios_family_c64p_tesla_Power_Module_hasMask
#define Power_Module_getMask ti_sysbios_family_c64p_tesla_Power_Module_getMask
#define Power_Module_setMask ti_sysbios_family_c64p_tesla_Power_Module_setMask
#define Power_Object_heap ti_sysbios_family_c64p_tesla_Power_Object_heap
#define Power_Module_heap ti_sysbios_family_c64p_tesla_Power_Module_heap

#endif /* ti_sysbios_family_c64p_tesla_Power__localnames__done */
#endif
