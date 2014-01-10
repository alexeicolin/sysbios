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

#ifndef ti_sysbios_family_c64p_EventCombiner__include
#define ti_sysbios_family_c64p_EventCombiner__include

#ifndef __nested__
#define __nested__
#define ti_sysbios_family_c64p_EventCombiner__top__
#endif

#ifdef __cplusplus
#define __extern extern "C"
#else
#define __extern extern
#endif

#define ti_sysbios_family_c64p_EventCombiner___VERS 150


/*
 * ======== INCLUDES ========
 */

#include <xdc/std.h>

#include <xdc/runtime/xdc.h>
#include <xdc/runtime/Types.h>
#include <ti/sysbios/family/c64p/package/package.defs.h>

#include <xdc/runtime/IModule.h>
#include <xdc/runtime/Error.h>


/*
 * ======== AUXILIARY DEFINITIONS ========
 */

/* NUM_EVENTS */
#define ti_sysbios_family_c64p_EventCombiner_NUM_EVENTS (128)

/* FuncPtr */
typedef xdc_Void (*ti_sysbios_family_c64p_EventCombiner_FuncPtr)(xdc_UArg);


/*
 * ======== INTERNAL DEFINITIONS ========
 */

/* DispatchTabElem */
struct ti_sysbios_family_c64p_EventCombiner_DispatchTabElem {
    ti_sysbios_family_c64p_EventCombiner_FuncPtr fxn;
    xdc_UArg arg;
};

/* Module_State */
typedef ti_sysbios_family_c64p_EventCombiner_DispatchTabElem __T1_ti_sysbios_family_c64p_EventCombiner_Module_State__dispatchTab;
typedef ti_sysbios_family_c64p_EventCombiner_DispatchTabElem __ARRAY1_ti_sysbios_family_c64p_EventCombiner_Module_State__dispatchTab[128];
typedef __ARRAY1_ti_sysbios_family_c64p_EventCombiner_Module_State__dispatchTab __TA_ti_sysbios_family_c64p_EventCombiner_Module_State__dispatchTab;


/*
 * ======== MODULE-WIDE CONFIGS ========
 */

/* Module__diagsEnabled */
typedef xdc_Bits32 CT__ti_sysbios_family_c64p_EventCombiner_Module__diagsEnabled;
__extern __FAR__ const CT__ti_sysbios_family_c64p_EventCombiner_Module__diagsEnabled ti_sysbios_family_c64p_EventCombiner_Module__diagsEnabled__C;

/* Module__diagsIncluded */
typedef xdc_Bits32 CT__ti_sysbios_family_c64p_EventCombiner_Module__diagsIncluded;
__extern __FAR__ const CT__ti_sysbios_family_c64p_EventCombiner_Module__diagsIncluded ti_sysbios_family_c64p_EventCombiner_Module__diagsIncluded__C;

/* Module__diagsMask */
typedef xdc_Bits16* CT__ti_sysbios_family_c64p_EventCombiner_Module__diagsMask;
__extern __FAR__ const CT__ti_sysbios_family_c64p_EventCombiner_Module__diagsMask ti_sysbios_family_c64p_EventCombiner_Module__diagsMask__C;

/* Module__gateObj */
typedef xdc_Ptr CT__ti_sysbios_family_c64p_EventCombiner_Module__gateObj;
__extern __FAR__ const CT__ti_sysbios_family_c64p_EventCombiner_Module__gateObj ti_sysbios_family_c64p_EventCombiner_Module__gateObj__C;

/* Module__gatePrms */
typedef xdc_Ptr CT__ti_sysbios_family_c64p_EventCombiner_Module__gatePrms;
__extern __FAR__ const CT__ti_sysbios_family_c64p_EventCombiner_Module__gatePrms ti_sysbios_family_c64p_EventCombiner_Module__gatePrms__C;

/* Module__id */
typedef xdc_runtime_Types_ModuleId CT__ti_sysbios_family_c64p_EventCombiner_Module__id;
__extern __FAR__ const CT__ti_sysbios_family_c64p_EventCombiner_Module__id ti_sysbios_family_c64p_EventCombiner_Module__id__C;

/* Module__loggerDefined */
typedef xdc_Bool CT__ti_sysbios_family_c64p_EventCombiner_Module__loggerDefined;
__extern __FAR__ const CT__ti_sysbios_family_c64p_EventCombiner_Module__loggerDefined ti_sysbios_family_c64p_EventCombiner_Module__loggerDefined__C;

/* Module__loggerObj */
typedef xdc_Ptr CT__ti_sysbios_family_c64p_EventCombiner_Module__loggerObj;
__extern __FAR__ const CT__ti_sysbios_family_c64p_EventCombiner_Module__loggerObj ti_sysbios_family_c64p_EventCombiner_Module__loggerObj__C;

/* Module__loggerFxn0 */
typedef xdc_runtime_Types_LoggerFxn0 CT__ti_sysbios_family_c64p_EventCombiner_Module__loggerFxn0;
__extern __FAR__ const CT__ti_sysbios_family_c64p_EventCombiner_Module__loggerFxn0 ti_sysbios_family_c64p_EventCombiner_Module__loggerFxn0__C;

/* Module__loggerFxn1 */
typedef xdc_runtime_Types_LoggerFxn1 CT__ti_sysbios_family_c64p_EventCombiner_Module__loggerFxn1;
__extern __FAR__ const CT__ti_sysbios_family_c64p_EventCombiner_Module__loggerFxn1 ti_sysbios_family_c64p_EventCombiner_Module__loggerFxn1__C;

/* Module__loggerFxn2 */
typedef xdc_runtime_Types_LoggerFxn2 CT__ti_sysbios_family_c64p_EventCombiner_Module__loggerFxn2;
__extern __FAR__ const CT__ti_sysbios_family_c64p_EventCombiner_Module__loggerFxn2 ti_sysbios_family_c64p_EventCombiner_Module__loggerFxn2__C;

/* Module__loggerFxn4 */
typedef xdc_runtime_Types_LoggerFxn4 CT__ti_sysbios_family_c64p_EventCombiner_Module__loggerFxn4;
__extern __FAR__ const CT__ti_sysbios_family_c64p_EventCombiner_Module__loggerFxn4 ti_sysbios_family_c64p_EventCombiner_Module__loggerFxn4__C;

/* Module__loggerFxn8 */
typedef xdc_runtime_Types_LoggerFxn8 CT__ti_sysbios_family_c64p_EventCombiner_Module__loggerFxn8;
__extern __FAR__ const CT__ti_sysbios_family_c64p_EventCombiner_Module__loggerFxn8 ti_sysbios_family_c64p_EventCombiner_Module__loggerFxn8__C;

/* Module__startupDoneFxn */
typedef xdc_Bool (*CT__ti_sysbios_family_c64p_EventCombiner_Module__startupDoneFxn)(void);
__extern __FAR__ const CT__ti_sysbios_family_c64p_EventCombiner_Module__startupDoneFxn ti_sysbios_family_c64p_EventCombiner_Module__startupDoneFxn__C;

/* Object__count */
typedef xdc_Int CT__ti_sysbios_family_c64p_EventCombiner_Object__count;
__extern __FAR__ const CT__ti_sysbios_family_c64p_EventCombiner_Object__count ti_sysbios_family_c64p_EventCombiner_Object__count__C;

/* Object__heap */
typedef xdc_runtime_IHeap_Handle CT__ti_sysbios_family_c64p_EventCombiner_Object__heap;
__extern __FAR__ const CT__ti_sysbios_family_c64p_EventCombiner_Object__heap ti_sysbios_family_c64p_EventCombiner_Object__heap__C;

/* Object__sizeof */
typedef xdc_SizeT CT__ti_sysbios_family_c64p_EventCombiner_Object__sizeof;
__extern __FAR__ const CT__ti_sysbios_family_c64p_EventCombiner_Object__sizeof ti_sysbios_family_c64p_EventCombiner_Object__sizeof__C;

/* Object__table */
typedef xdc_Ptr CT__ti_sysbios_family_c64p_EventCombiner_Object__table;
__extern __FAR__ const CT__ti_sysbios_family_c64p_EventCombiner_Object__table ti_sysbios_family_c64p_EventCombiner_Object__table__C;

/* E_unpluggedEvent */
#define ti_sysbios_family_c64p_EventCombiner_E_unpluggedEvent (ti_sysbios_family_c64p_EventCombiner_E_unpluggedEvent__C)
typedef xdc_runtime_Error_Id CT__ti_sysbios_family_c64p_EventCombiner_E_unpluggedEvent;
__extern __FAR__ const CT__ti_sysbios_family_c64p_EventCombiner_E_unpluggedEvent ti_sysbios_family_c64p_EventCombiner_E_unpluggedEvent__C;

/* EVTMASK */
#define ti_sysbios_family_c64p_EventCombiner_EVTMASK (ti_sysbios_family_c64p_EventCombiner_EVTMASK__C)
typedef xdc_Bits32 __T1_ti_sysbios_family_c64p_EventCombiner_EVTMASK;
typedef xdc_Bits32 __ARRAY1_ti_sysbios_family_c64p_EventCombiner_EVTMASK[4];
typedef __ARRAY1_ti_sysbios_family_c64p_EventCombiner_EVTMASK __TA_ti_sysbios_family_c64p_EventCombiner_EVTMASK;
typedef __TA_ti_sysbios_family_c64p_EventCombiner_EVTMASK CT__ti_sysbios_family_c64p_EventCombiner_EVTMASK;
__extern __FAR__ const CT__ti_sysbios_family_c64p_EventCombiner_EVTMASK ti_sysbios_family_c64p_EventCombiner_EVTMASK__C;


/*
 * ======== FUNCTION DECLARATIONS ========
 */

/* Module_startup */
#define ti_sysbios_family_c64p_EventCombiner_Module_startup ti_sysbios_family_c64p_EventCombiner_Module_startup__E
xdc__CODESECT(ti_sysbios_family_c64p_EventCombiner_Module_startup__E, "ti_sysbios_family_c64p_EventCombiner_Module_startup")
__extern xdc_Int ti_sysbios_family_c64p_EventCombiner_Module_startup__E( xdc_Int state );
xdc__CODESECT(ti_sysbios_family_c64p_EventCombiner_Module_startup__F, "ti_sysbios_family_c64p_EventCombiner_Module_startup")
__extern xdc_Int ti_sysbios_family_c64p_EventCombiner_Module_startup__F( xdc_Int state );
xdc__CODESECT(ti_sysbios_family_c64p_EventCombiner_Module_startup__R, "ti_sysbios_family_c64p_EventCombiner_Module_startup")
__extern xdc_Int ti_sysbios_family_c64p_EventCombiner_Module_startup__R( xdc_Int state );

/* Module__startupDone__S */
xdc__CODESECT(ti_sysbios_family_c64p_EventCombiner_Module__startupDone__S, "ti_sysbios_family_c64p_EventCombiner_Module__startupDone")
__extern xdc_Bool ti_sysbios_family_c64p_EventCombiner_Module__startupDone__S( void );

/* disableEvent__E */
#define ti_sysbios_family_c64p_EventCombiner_disableEvent ti_sysbios_family_c64p_EventCombiner_disableEvent__E
xdc__CODESECT(ti_sysbios_family_c64p_EventCombiner_disableEvent__E, "ti_sysbios_family_c64p_EventCombiner_disableEvent")
__extern xdc_Void ti_sysbios_family_c64p_EventCombiner_disableEvent__E( xdc_UInt evt );
xdc__CODESECT(ti_sysbios_family_c64p_EventCombiner_disableEvent__F, "ti_sysbios_family_c64p_EventCombiner_disableEvent")
__extern xdc_Void ti_sysbios_family_c64p_EventCombiner_disableEvent__F( xdc_UInt evt );
__extern xdc_Void ti_sysbios_family_c64p_EventCombiner_disableEvent__R( xdc_UInt evt );

/* enableEvent__E */
#define ti_sysbios_family_c64p_EventCombiner_enableEvent ti_sysbios_family_c64p_EventCombiner_enableEvent__E
xdc__CODESECT(ti_sysbios_family_c64p_EventCombiner_enableEvent__E, "ti_sysbios_family_c64p_EventCombiner_enableEvent")
__extern xdc_Void ti_sysbios_family_c64p_EventCombiner_enableEvent__E( xdc_UInt evt );
xdc__CODESECT(ti_sysbios_family_c64p_EventCombiner_enableEvent__F, "ti_sysbios_family_c64p_EventCombiner_enableEvent")
__extern xdc_Void ti_sysbios_family_c64p_EventCombiner_enableEvent__F( xdc_UInt evt );
__extern xdc_Void ti_sysbios_family_c64p_EventCombiner_enableEvent__R( xdc_UInt evt );

/* dispatch__E */
#define ti_sysbios_family_c64p_EventCombiner_dispatch ti_sysbios_family_c64p_EventCombiner_dispatch__E
xdc__CODESECT(ti_sysbios_family_c64p_EventCombiner_dispatch__E, "ti_sysbios_family_c64p_EventCombiner_dispatch")
__extern xdc_Void ti_sysbios_family_c64p_EventCombiner_dispatch__E( xdc_UInt evt );
xdc__CODESECT(ti_sysbios_family_c64p_EventCombiner_dispatch__F, "ti_sysbios_family_c64p_EventCombiner_dispatch")
__extern xdc_Void ti_sysbios_family_c64p_EventCombiner_dispatch__F( xdc_UInt evt );
__extern xdc_Void ti_sysbios_family_c64p_EventCombiner_dispatch__R( xdc_UInt evt );

/* dispatchPlug__E */
#define ti_sysbios_family_c64p_EventCombiner_dispatchPlug ti_sysbios_family_c64p_EventCombiner_dispatchPlug__E
xdc__CODESECT(ti_sysbios_family_c64p_EventCombiner_dispatchPlug__E, "ti_sysbios_family_c64p_EventCombiner_dispatchPlug")
__extern xdc_Void ti_sysbios_family_c64p_EventCombiner_dispatchPlug__E( xdc_UInt evt, ti_sysbios_family_c64p_EventCombiner_FuncPtr fxn, xdc_UArg arg, xdc_Bool unmask );
xdc__CODESECT(ti_sysbios_family_c64p_EventCombiner_dispatchPlug__F, "ti_sysbios_family_c64p_EventCombiner_dispatchPlug")
__extern xdc_Void ti_sysbios_family_c64p_EventCombiner_dispatchPlug__F( xdc_UInt evt, ti_sysbios_family_c64p_EventCombiner_FuncPtr fxn, xdc_UArg arg, xdc_Bool unmask );
__extern xdc_Void ti_sysbios_family_c64p_EventCombiner_dispatchPlug__R( xdc_UInt evt, ti_sysbios_family_c64p_EventCombiner_FuncPtr fxn, xdc_UArg arg, xdc_Bool unmask );

/* unused__E */
#define ti_sysbios_family_c64p_EventCombiner_unused ti_sysbios_family_c64p_EventCombiner_unused__E
xdc__CODESECT(ti_sysbios_family_c64p_EventCombiner_unused__E, "ti_sysbios_family_c64p_EventCombiner_unused")
__extern xdc_Void ti_sysbios_family_c64p_EventCombiner_unused__E( xdc_UArg arg );
xdc__CODESECT(ti_sysbios_family_c64p_EventCombiner_unused__F, "ti_sysbios_family_c64p_EventCombiner_unused")
__extern xdc_Void ti_sysbios_family_c64p_EventCombiner_unused__F( xdc_UArg arg );
__extern xdc_Void ti_sysbios_family_c64p_EventCombiner_unused__R( xdc_UArg arg );


/*
 * ======== SYSTEM FUNCTIONS ========
 */

/* Module_startupDone */
#define ti_sysbios_family_c64p_EventCombiner_Module_startupDone() ti_sysbios_family_c64p_EventCombiner_Module__startupDone__S()

/* Object_heap */
#define ti_sysbios_family_c64p_EventCombiner_Object_heap() ti_sysbios_family_c64p_EventCombiner_Object__heap__C

/* Module_heap */
#define ti_sysbios_family_c64p_EventCombiner_Module_heap() ti_sysbios_family_c64p_EventCombiner_Object__heap__C

/* Module_id */
static inline CT__ti_sysbios_family_c64p_EventCombiner_Module__id ti_sysbios_family_c64p_EventCombiner_Module_id( void ) 
{
    return ti_sysbios_family_c64p_EventCombiner_Module__id__C;
}

/* Module_hasMask */
static inline xdc_Bool ti_sysbios_family_c64p_EventCombiner_Module_hasMask( void ) 
{
    return ti_sysbios_family_c64p_EventCombiner_Module__diagsMask__C != NULL;
}

/* Module_getMask */
static inline xdc_Bits16 ti_sysbios_family_c64p_EventCombiner_Module_getMask( void ) 
{
    return ti_sysbios_family_c64p_EventCombiner_Module__diagsMask__C != NULL ? *ti_sysbios_family_c64p_EventCombiner_Module__diagsMask__C : 0;
}

/* Module_setMask */
static inline xdc_Void ti_sysbios_family_c64p_EventCombiner_Module_setMask( xdc_Bits16 mask ) 
{
    if (ti_sysbios_family_c64p_EventCombiner_Module__diagsMask__C != NULL) *ti_sysbios_family_c64p_EventCombiner_Module__diagsMask__C = mask;
}


/*
 * ======== EPILOGUE ========
 */

#ifdef ti_sysbios_family_c64p_EventCombiner__top__
#undef __nested__
#endif

#endif /* ti_sysbios_family_c64p_EventCombiner__include */


/*
 * ======== STATE STRUCTURES ========
 */

#if defined(__config__) || (!defined(__nested__) && defined(ti_sysbios_family_c64p_EventCombiner__internalaccess))

#ifndef ti_sysbios_family_c64p_EventCombiner__include_state
#define ti_sysbios_family_c64p_EventCombiner__include_state

/* Module_State */
struct ti_sysbios_family_c64p_EventCombiner_Module_State {
    __TA_ti_sysbios_family_c64p_EventCombiner_Module_State__dispatchTab dispatchTab;
};

/* Module__state__V */
extern struct ti_sysbios_family_c64p_EventCombiner_Module_State__ ti_sysbios_family_c64p_EventCombiner_Module__state__V;

#endif /* ti_sysbios_family_c64p_EventCombiner__include_state */

#endif


/*
 * ======== PREFIX ALIASES ========
 */

#if !defined(__nested__) && !defined(ti_sysbios_family_c64p_EventCombiner__nolocalnames)

#ifndef ti_sysbios_family_c64p_EventCombiner__localnames__done
#define ti_sysbios_family_c64p_EventCombiner__localnames__done

/* module prefix */
#define EventCombiner_NUM_EVENTS ti_sysbios_family_c64p_EventCombiner_NUM_EVENTS
#define EventCombiner_FuncPtr ti_sysbios_family_c64p_EventCombiner_FuncPtr
#define EventCombiner_DispatchTabElem ti_sysbios_family_c64p_EventCombiner_DispatchTabElem
#define EventCombiner_Module_State ti_sysbios_family_c64p_EventCombiner_Module_State
#define EventCombiner_E_unpluggedEvent ti_sysbios_family_c64p_EventCombiner_E_unpluggedEvent
#define EventCombiner_EVTMASK ti_sysbios_family_c64p_EventCombiner_EVTMASK
#define EventCombiner_disableEvent ti_sysbios_family_c64p_EventCombiner_disableEvent
#define EventCombiner_enableEvent ti_sysbios_family_c64p_EventCombiner_enableEvent
#define EventCombiner_dispatch ti_sysbios_family_c64p_EventCombiner_dispatch
#define EventCombiner_dispatchPlug ti_sysbios_family_c64p_EventCombiner_dispatchPlug
#define EventCombiner_unused ti_sysbios_family_c64p_EventCombiner_unused
#define EventCombiner_Module_name ti_sysbios_family_c64p_EventCombiner_Module_name
#define EventCombiner_Module_id ti_sysbios_family_c64p_EventCombiner_Module_id
#define EventCombiner_Module_startup ti_sysbios_family_c64p_EventCombiner_Module_startup
#define EventCombiner_Module_startupDone ti_sysbios_family_c64p_EventCombiner_Module_startupDone
#define EventCombiner_Module_hasMask ti_sysbios_family_c64p_EventCombiner_Module_hasMask
#define EventCombiner_Module_getMask ti_sysbios_family_c64p_EventCombiner_Module_getMask
#define EventCombiner_Module_setMask ti_sysbios_family_c64p_EventCombiner_Module_setMask
#define EventCombiner_Object_heap ti_sysbios_family_c64p_EventCombiner_Object_heap
#define EventCombiner_Module_heap ti_sysbios_family_c64p_EventCombiner_Module_heap

#endif /* ti_sysbios_family_c64p_EventCombiner__localnames__done */
#endif
