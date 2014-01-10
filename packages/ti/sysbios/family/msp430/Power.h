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

#ifndef ti_sysbios_family_msp430_Power__include
#define ti_sysbios_family_msp430_Power__include

#ifndef __nested__
#define __nested__
#define ti_sysbios_family_msp430_Power__top__
#endif

#ifdef __cplusplus
#define __extern extern "C"
#else
#define __extern extern
#endif

#define ti_sysbios_family_msp430_Power___VERS 150


/*
 * ======== INCLUDES ========
 */

#include <xdc/std.h>

#include <xdc/runtime/xdc.h>
#include <xdc/runtime/Types.h>
#include <ti/sysbios/family/msp430/package/package.defs.h>

#include <xdc/runtime/Error.h>
#include <xdc/runtime/IModule.h>


/*
 * ======== AUXILIARY DEFINITIONS ========
 */

/* SleepPrepFuncPtr */
typedef xdc_Void (*ti_sysbios_family_msp430_Power_SleepPrepFuncPtr)(void);

/* IdleMode */
enum ti_sysbios_family_msp430_Power_IdleMode {
    ti_sysbios_family_msp430_Power_LPM0 = 0x10,
    ti_sysbios_family_msp430_Power_LPM1 = 0x50,
    ti_sysbios_family_msp430_Power_LPM2 = 0x90,
    ti_sysbios_family_msp430_Power_LPM3 = 0xD0,
    ti_sysbios_family_msp430_Power_LPM4 = 0xF0
};
typedef enum ti_sysbios_family_msp430_Power_IdleMode ti_sysbios_family_msp430_Power_IdleMode;

/* SleepMode */
enum ti_sysbios_family_msp430_Power_SleepMode {
    ti_sysbios_family_msp430_Power_Sleep_LPM4_5
};
typedef enum ti_sysbios_family_msp430_Power_SleepMode ti_sysbios_family_msp430_Power_SleepMode;


/*
 * ======== INTERNAL DEFINITIONS ========
 */


/*
 * ======== MODULE-WIDE CONFIGS ========
 */

/* Module__diagsEnabled */
typedef xdc_Bits32 CT__ti_sysbios_family_msp430_Power_Module__diagsEnabled;
__extern __FAR__ const CT__ti_sysbios_family_msp430_Power_Module__diagsEnabled ti_sysbios_family_msp430_Power_Module__diagsEnabled__C;

/* Module__diagsIncluded */
typedef xdc_Bits32 CT__ti_sysbios_family_msp430_Power_Module__diagsIncluded;
__extern __FAR__ const CT__ti_sysbios_family_msp430_Power_Module__diagsIncluded ti_sysbios_family_msp430_Power_Module__diagsIncluded__C;

/* Module__diagsMask */
typedef xdc_Bits16* CT__ti_sysbios_family_msp430_Power_Module__diagsMask;
__extern __FAR__ const CT__ti_sysbios_family_msp430_Power_Module__diagsMask ti_sysbios_family_msp430_Power_Module__diagsMask__C;

/* Module__gateObj */
typedef xdc_Ptr CT__ti_sysbios_family_msp430_Power_Module__gateObj;
__extern __FAR__ const CT__ti_sysbios_family_msp430_Power_Module__gateObj ti_sysbios_family_msp430_Power_Module__gateObj__C;

/* Module__gatePrms */
typedef xdc_Ptr CT__ti_sysbios_family_msp430_Power_Module__gatePrms;
__extern __FAR__ const CT__ti_sysbios_family_msp430_Power_Module__gatePrms ti_sysbios_family_msp430_Power_Module__gatePrms__C;

/* Module__id */
typedef xdc_runtime_Types_ModuleId CT__ti_sysbios_family_msp430_Power_Module__id;
__extern __FAR__ const CT__ti_sysbios_family_msp430_Power_Module__id ti_sysbios_family_msp430_Power_Module__id__C;

/* Module__loggerDefined */
typedef xdc_Bool CT__ti_sysbios_family_msp430_Power_Module__loggerDefined;
__extern __FAR__ const CT__ti_sysbios_family_msp430_Power_Module__loggerDefined ti_sysbios_family_msp430_Power_Module__loggerDefined__C;

/* Module__loggerObj */
typedef xdc_Ptr CT__ti_sysbios_family_msp430_Power_Module__loggerObj;
__extern __FAR__ const CT__ti_sysbios_family_msp430_Power_Module__loggerObj ti_sysbios_family_msp430_Power_Module__loggerObj__C;

/* Module__loggerFxn0 */
typedef xdc_runtime_Types_LoggerFxn0 CT__ti_sysbios_family_msp430_Power_Module__loggerFxn0;
__extern __FAR__ const CT__ti_sysbios_family_msp430_Power_Module__loggerFxn0 ti_sysbios_family_msp430_Power_Module__loggerFxn0__C;

/* Module__loggerFxn1 */
typedef xdc_runtime_Types_LoggerFxn1 CT__ti_sysbios_family_msp430_Power_Module__loggerFxn1;
__extern __FAR__ const CT__ti_sysbios_family_msp430_Power_Module__loggerFxn1 ti_sysbios_family_msp430_Power_Module__loggerFxn1__C;

/* Module__loggerFxn2 */
typedef xdc_runtime_Types_LoggerFxn2 CT__ti_sysbios_family_msp430_Power_Module__loggerFxn2;
__extern __FAR__ const CT__ti_sysbios_family_msp430_Power_Module__loggerFxn2 ti_sysbios_family_msp430_Power_Module__loggerFxn2__C;

/* Module__loggerFxn4 */
typedef xdc_runtime_Types_LoggerFxn4 CT__ti_sysbios_family_msp430_Power_Module__loggerFxn4;
__extern __FAR__ const CT__ti_sysbios_family_msp430_Power_Module__loggerFxn4 ti_sysbios_family_msp430_Power_Module__loggerFxn4__C;

/* Module__loggerFxn8 */
typedef xdc_runtime_Types_LoggerFxn8 CT__ti_sysbios_family_msp430_Power_Module__loggerFxn8;
__extern __FAR__ const CT__ti_sysbios_family_msp430_Power_Module__loggerFxn8 ti_sysbios_family_msp430_Power_Module__loggerFxn8__C;

/* Module__startupDoneFxn */
typedef xdc_Bool (*CT__ti_sysbios_family_msp430_Power_Module__startupDoneFxn)(void);
__extern __FAR__ const CT__ti_sysbios_family_msp430_Power_Module__startupDoneFxn ti_sysbios_family_msp430_Power_Module__startupDoneFxn__C;

/* Object__count */
typedef xdc_Int CT__ti_sysbios_family_msp430_Power_Object__count;
__extern __FAR__ const CT__ti_sysbios_family_msp430_Power_Object__count ti_sysbios_family_msp430_Power_Object__count__C;

/* Object__heap */
typedef xdc_runtime_IHeap_Handle CT__ti_sysbios_family_msp430_Power_Object__heap;
__extern __FAR__ const CT__ti_sysbios_family_msp430_Power_Object__heap ti_sysbios_family_msp430_Power_Object__heap__C;

/* Object__sizeof */
typedef xdc_SizeT CT__ti_sysbios_family_msp430_Power_Object__sizeof;
__extern __FAR__ const CT__ti_sysbios_family_msp430_Power_Object__sizeof ti_sysbios_family_msp430_Power_Object__sizeof__C;

/* Object__table */
typedef xdc_Ptr CT__ti_sysbios_family_msp430_Power_Object__table;
__extern __FAR__ const CT__ti_sysbios_family_msp430_Power_Object__table ti_sysbios_family_msp430_Power_Object__table__C;

/* E_dynamicModeNotAllowed */
#define ti_sysbios_family_msp430_Power_E_dynamicModeNotAllowed (ti_sysbios_family_msp430_Power_E_dynamicModeNotAllowed__C)
typedef xdc_runtime_Error_Id CT__ti_sysbios_family_msp430_Power_E_dynamicModeNotAllowed;
__extern __FAR__ const CT__ti_sysbios_family_msp430_Power_E_dynamicModeNotAllowed ti_sysbios_family_msp430_Power_E_dynamicModeNotAllowed__C;

/* idle */
#ifdef ti_sysbios_family_msp430_Power_idle__D
#define ti_sysbios_family_msp430_Power_idle (ti_sysbios_family_msp430_Power_idle__D)
#else
#define ti_sysbios_family_msp430_Power_idle (ti_sysbios_family_msp430_Power_idle__C)
typedef xdc_Bool CT__ti_sysbios_family_msp430_Power_idle;
__extern __FAR__ const CT__ti_sysbios_family_msp430_Power_idle ti_sysbios_family_msp430_Power_idle__C;
#endif

/* idleMode */
#define ti_sysbios_family_msp430_Power_idleMode (ti_sysbios_family_msp430_Power_idleMode__C)
typedef ti_sysbios_family_msp430_Power_IdleMode CT__ti_sysbios_family_msp430_Power_idleMode;
__extern __FAR__ const CT__ti_sysbios_family_msp430_Power_idleMode ti_sysbios_family_msp430_Power_idleMode__C;

/* allowDynamicMode */
#ifdef ti_sysbios_family_msp430_Power_allowDynamicMode__D
#define ti_sysbios_family_msp430_Power_allowDynamicMode (ti_sysbios_family_msp430_Power_allowDynamicMode__D)
#else
#define ti_sysbios_family_msp430_Power_allowDynamicMode (ti_sysbios_family_msp430_Power_allowDynamicMode__C)
typedef xdc_Bool CT__ti_sysbios_family_msp430_Power_allowDynamicMode;
__extern __FAR__ const CT__ti_sysbios_family_msp430_Power_allowDynamicMode ti_sysbios_family_msp430_Power_allowDynamicMode__C;
#endif

/* sleepPrepFunction */
#define ti_sysbios_family_msp430_Power_sleepPrepFunction (ti_sysbios_family_msp430_Power_sleepPrepFunction__C)
typedef ti_sysbios_family_msp430_Power_SleepPrepFuncPtr CT__ti_sysbios_family_msp430_Power_sleepPrepFunction;
__extern __FAR__ const CT__ti_sysbios_family_msp430_Power_sleepPrepFunction ti_sysbios_family_msp430_Power_sleepPrepFunction__C;


/*
 * ======== FUNCTION DECLARATIONS ========
 */

/* Module_startup */
#define ti_sysbios_family_msp430_Power_Module_startup( state ) (-1)

/* Module__startupDone__S */
xdc__CODESECT(ti_sysbios_family_msp430_Power_Module__startupDone__S, "ti_sysbios_family_msp430_Power_Module__startupDone")
__extern xdc_Bool ti_sysbios_family_msp430_Power_Module__startupDone__S( void );

/* idleCPU__E */
#define ti_sysbios_family_msp430_Power_idleCPU ti_sysbios_family_msp430_Power_idleCPU__E
xdc__CODESECT(ti_sysbios_family_msp430_Power_idleCPU__E, "ti_sysbios_family_msp430_Power_idleCPU")
__extern xdc_Void ti_sysbios_family_msp430_Power_idleCPU__E( void );
xdc__CODESECT(ti_sysbios_family_msp430_Power_idleCPU__F, "ti_sysbios_family_msp430_Power_idleCPU")
__extern xdc_Void ti_sysbios_family_msp430_Power_idleCPU__F( void );
__extern xdc_Void ti_sysbios_family_msp430_Power_idleCPU__R( void );

/* changeIdleMode__E */
#define ti_sysbios_family_msp430_Power_changeIdleMode ti_sysbios_family_msp430_Power_changeIdleMode__E
xdc__CODESECT(ti_sysbios_family_msp430_Power_changeIdleMode__E, "ti_sysbios_family_msp430_Power_changeIdleMode")
__extern xdc_Void ti_sysbios_family_msp430_Power_changeIdleMode__E( ti_sysbios_family_msp430_Power_IdleMode mode );
xdc__CODESECT(ti_sysbios_family_msp430_Power_changeIdleMode__F, "ti_sysbios_family_msp430_Power_changeIdleMode")
__extern xdc_Void ti_sysbios_family_msp430_Power_changeIdleMode__F( ti_sysbios_family_msp430_Power_IdleMode mode );
__extern xdc_Void ti_sysbios_family_msp430_Power_changeIdleMode__R( ti_sysbios_family_msp430_Power_IdleMode mode );

/* sleepCPU__E */
#define ti_sysbios_family_msp430_Power_sleepCPU ti_sysbios_family_msp430_Power_sleepCPU__E
xdc__CODESECT(ti_sysbios_family_msp430_Power_sleepCPU__E, "ti_sysbios_family_msp430_Power_sleepCPU")
__extern xdc_Void ti_sysbios_family_msp430_Power_sleepCPU__E( ti_sysbios_family_msp430_Power_SleepMode sleepMode );
xdc__CODESECT(ti_sysbios_family_msp430_Power_sleepCPU__F, "ti_sysbios_family_msp430_Power_sleepCPU")
__extern xdc_Void ti_sysbios_family_msp430_Power_sleepCPU__F( ti_sysbios_family_msp430_Power_SleepMode sleepMode );
__extern xdc_Void ti_sysbios_family_msp430_Power_sleepCPU__R( ti_sysbios_family_msp430_Power_SleepMode sleepMode );

/* wakeCPU__E */
#define ti_sysbios_family_msp430_Power_wakeCPU ti_sysbios_family_msp430_Power_wakeCPU__E
xdc__CODESECT(ti_sysbios_family_msp430_Power_wakeCPU__E, "ti_sysbios_family_msp430_Power_wakeCPU")
__extern xdc_Void ti_sysbios_family_msp430_Power_wakeCPU__E( void );
xdc__CODESECT(ti_sysbios_family_msp430_Power_wakeCPU__F, "ti_sysbios_family_msp430_Power_wakeCPU")
__extern xdc_Void ti_sysbios_family_msp430_Power_wakeCPU__F( void );
__extern xdc_Void ti_sysbios_family_msp430_Power_wakeCPU__R( void );

/* getModeChangeFlag__E */
#define ti_sysbios_family_msp430_Power_getModeChangeFlag ti_sysbios_family_msp430_Power_getModeChangeFlag__E
xdc__CODESECT(ti_sysbios_family_msp430_Power_getModeChangeFlag__E, "ti_sysbios_family_msp430_Power_getModeChangeFlag")
__extern xdc_UInt ti_sysbios_family_msp430_Power_getModeChangeFlag__E( void );
xdc__CODESECT(ti_sysbios_family_msp430_Power_getModeChangeFlag__F, "ti_sysbios_family_msp430_Power_getModeChangeFlag")
__extern xdc_UInt ti_sysbios_family_msp430_Power_getModeChangeFlag__F( void );
__extern xdc_UInt ti_sysbios_family_msp430_Power_getModeChangeFlag__R( void );

/* defaultSleepPrepFunction__I */
#define ti_sysbios_family_msp430_Power_defaultSleepPrepFunction ti_sysbios_family_msp430_Power_defaultSleepPrepFunction__I
xdc__CODESECT(ti_sysbios_family_msp430_Power_defaultSleepPrepFunction__I, "ti_sysbios_family_msp430_Power_defaultSleepPrepFunction")
__extern xdc_Void ti_sysbios_family_msp430_Power_defaultSleepPrepFunction__I( void );

/* blockedTaskFunction__I */
#define ti_sysbios_family_msp430_Power_blockedTaskFunction ti_sysbios_family_msp430_Power_blockedTaskFunction__I
xdc__CODESECT(ti_sysbios_family_msp430_Power_blockedTaskFunction__I, "ti_sysbios_family_msp430_Power_blockedTaskFunction")
__extern xdc_Void ti_sysbios_family_msp430_Power_blockedTaskFunction__I( void );


/*
 * ======== SYSTEM FUNCTIONS ========
 */

/* Module_startupDone */
#define ti_sysbios_family_msp430_Power_Module_startupDone() ti_sysbios_family_msp430_Power_Module__startupDone__S()

/* Object_heap */
#define ti_sysbios_family_msp430_Power_Object_heap() ti_sysbios_family_msp430_Power_Object__heap__C

/* Module_heap */
#define ti_sysbios_family_msp430_Power_Module_heap() ti_sysbios_family_msp430_Power_Object__heap__C

/* Module_id */
static inline CT__ti_sysbios_family_msp430_Power_Module__id ti_sysbios_family_msp430_Power_Module_id( void ) 
{
    return ti_sysbios_family_msp430_Power_Module__id__C;
}

/* Module_hasMask */
static inline xdc_Bool ti_sysbios_family_msp430_Power_Module_hasMask( void ) 
{
    return ti_sysbios_family_msp430_Power_Module__diagsMask__C != NULL;
}

/* Module_getMask */
static inline xdc_Bits16 ti_sysbios_family_msp430_Power_Module_getMask( void ) 
{
    return ti_sysbios_family_msp430_Power_Module__diagsMask__C != NULL ? *ti_sysbios_family_msp430_Power_Module__diagsMask__C : 0;
}

/* Module_setMask */
static inline xdc_Void ti_sysbios_family_msp430_Power_Module_setMask( xdc_Bits16 mask ) 
{
    if (ti_sysbios_family_msp430_Power_Module__diagsMask__C != NULL) *ti_sysbios_family_msp430_Power_Module__diagsMask__C = mask;
}


/*
 * ======== EPILOGUE ========
 */

#ifdef ti_sysbios_family_msp430_Power__top__
#undef __nested__
#endif

#endif /* ti_sysbios_family_msp430_Power__include */


/*
 * ======== STATE STRUCTURES ========
 */

#if defined(__config__) || (!defined(__nested__) && defined(ti_sysbios_family_msp430_Power__internalaccess))

#ifndef ti_sysbios_family_msp430_Power__include_state
#define ti_sysbios_family_msp430_Power__include_state

/* Module_State */
struct ti_sysbios_family_msp430_Power_Module_State {
    xdc_UInt idleMode;
    xdc_Bool newMode;
};

/* Module__state__V */
extern struct ti_sysbios_family_msp430_Power_Module_State__ ti_sysbios_family_msp430_Power_Module__state__V;

#endif /* ti_sysbios_family_msp430_Power__include_state */

#endif


/*
 * ======== PREFIX ALIASES ========
 */

#if !defined(__nested__) && !defined(ti_sysbios_family_msp430_Power__nolocalnames)

#ifndef ti_sysbios_family_msp430_Power__localnames__done
#define ti_sysbios_family_msp430_Power__localnames__done

/* module prefix */
#define Power_SleepPrepFuncPtr ti_sysbios_family_msp430_Power_SleepPrepFuncPtr
#define Power_IdleMode ti_sysbios_family_msp430_Power_IdleMode
#define Power_SleepMode ti_sysbios_family_msp430_Power_SleepMode
#define Power_Module_State ti_sysbios_family_msp430_Power_Module_State
#define Power_LPM0 ti_sysbios_family_msp430_Power_LPM0
#define Power_LPM1 ti_sysbios_family_msp430_Power_LPM1
#define Power_LPM2 ti_sysbios_family_msp430_Power_LPM2
#define Power_LPM3 ti_sysbios_family_msp430_Power_LPM3
#define Power_LPM4 ti_sysbios_family_msp430_Power_LPM4
#define Power_Sleep_LPM4_5 ti_sysbios_family_msp430_Power_Sleep_LPM4_5
#define Power_E_dynamicModeNotAllowed ti_sysbios_family_msp430_Power_E_dynamicModeNotAllowed
#define Power_idle ti_sysbios_family_msp430_Power_idle
#define Power_idleMode ti_sysbios_family_msp430_Power_idleMode
#define Power_allowDynamicMode ti_sysbios_family_msp430_Power_allowDynamicMode
#define Power_sleepPrepFunction ti_sysbios_family_msp430_Power_sleepPrepFunction
#define Power_idleCPU ti_sysbios_family_msp430_Power_idleCPU
#define Power_changeIdleMode ti_sysbios_family_msp430_Power_changeIdleMode
#define Power_sleepCPU ti_sysbios_family_msp430_Power_sleepCPU
#define Power_wakeCPU ti_sysbios_family_msp430_Power_wakeCPU
#define Power_getModeChangeFlag ti_sysbios_family_msp430_Power_getModeChangeFlag
#define Power_Module_name ti_sysbios_family_msp430_Power_Module_name
#define Power_Module_id ti_sysbios_family_msp430_Power_Module_id
#define Power_Module_startup ti_sysbios_family_msp430_Power_Module_startup
#define Power_Module_startupDone ti_sysbios_family_msp430_Power_Module_startupDone
#define Power_Module_hasMask ti_sysbios_family_msp430_Power_Module_hasMask
#define Power_Module_getMask ti_sysbios_family_msp430_Power_Module_getMask
#define Power_Module_setMask ti_sysbios_family_msp430_Power_Module_setMask
#define Power_Object_heap ti_sysbios_family_msp430_Power_Object_heap
#define Power_Module_heap ti_sysbios_family_msp430_Power_Module_heap

#endif /* ti_sysbios_family_msp430_Power__localnames__done */
#endif
