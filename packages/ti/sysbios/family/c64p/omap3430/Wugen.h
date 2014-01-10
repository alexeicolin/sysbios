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

#ifndef ti_sysbios_family_c64p_omap3430_Wugen__include
#define ti_sysbios_family_c64p_omap3430_Wugen__include

#ifndef __nested__
#define __nested__
#define ti_sysbios_family_c64p_omap3430_Wugen__top__
#endif

#ifdef __cplusplus
#define __extern extern "C"
#else
#define __extern extern
#endif

#define ti_sysbios_family_c64p_omap3430_Wugen___VERS 150


/*
 * ======== INCLUDES ========
 */

#include <xdc/std.h>

#include <xdc/runtime/xdc.h>
#include <xdc/runtime/Types.h>
#include <ti/sysbios/family/c64p/omap3430/package/package.defs.h>

#include <xdc/runtime/IModule.h>


/*
 * ======== AUXILIARY DEFINITIONS ========
 */


/*
 * ======== INTERNAL DEFINITIONS ========
 */


/*
 * ======== MODULE-WIDE CONFIGS ========
 */

/* Module__diagsEnabled */
typedef xdc_Bits32 CT__ti_sysbios_family_c64p_omap3430_Wugen_Module__diagsEnabled;
__extern __FAR__ const CT__ti_sysbios_family_c64p_omap3430_Wugen_Module__diagsEnabled ti_sysbios_family_c64p_omap3430_Wugen_Module__diagsEnabled__C;

/* Module__diagsIncluded */
typedef xdc_Bits32 CT__ti_sysbios_family_c64p_omap3430_Wugen_Module__diagsIncluded;
__extern __FAR__ const CT__ti_sysbios_family_c64p_omap3430_Wugen_Module__diagsIncluded ti_sysbios_family_c64p_omap3430_Wugen_Module__diagsIncluded__C;

/* Module__diagsMask */
typedef xdc_Bits16* CT__ti_sysbios_family_c64p_omap3430_Wugen_Module__diagsMask;
__extern __FAR__ const CT__ti_sysbios_family_c64p_omap3430_Wugen_Module__diagsMask ti_sysbios_family_c64p_omap3430_Wugen_Module__diagsMask__C;

/* Module__gateObj */
typedef xdc_Ptr CT__ti_sysbios_family_c64p_omap3430_Wugen_Module__gateObj;
__extern __FAR__ const CT__ti_sysbios_family_c64p_omap3430_Wugen_Module__gateObj ti_sysbios_family_c64p_omap3430_Wugen_Module__gateObj__C;

/* Module__gatePrms */
typedef xdc_Ptr CT__ti_sysbios_family_c64p_omap3430_Wugen_Module__gatePrms;
__extern __FAR__ const CT__ti_sysbios_family_c64p_omap3430_Wugen_Module__gatePrms ti_sysbios_family_c64p_omap3430_Wugen_Module__gatePrms__C;

/* Module__id */
typedef xdc_runtime_Types_ModuleId CT__ti_sysbios_family_c64p_omap3430_Wugen_Module__id;
__extern __FAR__ const CT__ti_sysbios_family_c64p_omap3430_Wugen_Module__id ti_sysbios_family_c64p_omap3430_Wugen_Module__id__C;

/* Module__loggerDefined */
typedef xdc_Bool CT__ti_sysbios_family_c64p_omap3430_Wugen_Module__loggerDefined;
__extern __FAR__ const CT__ti_sysbios_family_c64p_omap3430_Wugen_Module__loggerDefined ti_sysbios_family_c64p_omap3430_Wugen_Module__loggerDefined__C;

/* Module__loggerObj */
typedef xdc_Ptr CT__ti_sysbios_family_c64p_omap3430_Wugen_Module__loggerObj;
__extern __FAR__ const CT__ti_sysbios_family_c64p_omap3430_Wugen_Module__loggerObj ti_sysbios_family_c64p_omap3430_Wugen_Module__loggerObj__C;

/* Module__loggerFxn0 */
typedef xdc_runtime_Types_LoggerFxn0 CT__ti_sysbios_family_c64p_omap3430_Wugen_Module__loggerFxn0;
__extern __FAR__ const CT__ti_sysbios_family_c64p_omap3430_Wugen_Module__loggerFxn0 ti_sysbios_family_c64p_omap3430_Wugen_Module__loggerFxn0__C;

/* Module__loggerFxn1 */
typedef xdc_runtime_Types_LoggerFxn1 CT__ti_sysbios_family_c64p_omap3430_Wugen_Module__loggerFxn1;
__extern __FAR__ const CT__ti_sysbios_family_c64p_omap3430_Wugen_Module__loggerFxn1 ti_sysbios_family_c64p_omap3430_Wugen_Module__loggerFxn1__C;

/* Module__loggerFxn2 */
typedef xdc_runtime_Types_LoggerFxn2 CT__ti_sysbios_family_c64p_omap3430_Wugen_Module__loggerFxn2;
__extern __FAR__ const CT__ti_sysbios_family_c64p_omap3430_Wugen_Module__loggerFxn2 ti_sysbios_family_c64p_omap3430_Wugen_Module__loggerFxn2__C;

/* Module__loggerFxn4 */
typedef xdc_runtime_Types_LoggerFxn4 CT__ti_sysbios_family_c64p_omap3430_Wugen_Module__loggerFxn4;
__extern __FAR__ const CT__ti_sysbios_family_c64p_omap3430_Wugen_Module__loggerFxn4 ti_sysbios_family_c64p_omap3430_Wugen_Module__loggerFxn4__C;

/* Module__loggerFxn8 */
typedef xdc_runtime_Types_LoggerFxn8 CT__ti_sysbios_family_c64p_omap3430_Wugen_Module__loggerFxn8;
__extern __FAR__ const CT__ti_sysbios_family_c64p_omap3430_Wugen_Module__loggerFxn8 ti_sysbios_family_c64p_omap3430_Wugen_Module__loggerFxn8__C;

/* Module__startupDoneFxn */
typedef xdc_Bool (*CT__ti_sysbios_family_c64p_omap3430_Wugen_Module__startupDoneFxn)(void);
__extern __FAR__ const CT__ti_sysbios_family_c64p_omap3430_Wugen_Module__startupDoneFxn ti_sysbios_family_c64p_omap3430_Wugen_Module__startupDoneFxn__C;

/* Object__count */
typedef xdc_Int CT__ti_sysbios_family_c64p_omap3430_Wugen_Object__count;
__extern __FAR__ const CT__ti_sysbios_family_c64p_omap3430_Wugen_Object__count ti_sysbios_family_c64p_omap3430_Wugen_Object__count__C;

/* Object__heap */
typedef xdc_runtime_IHeap_Handle CT__ti_sysbios_family_c64p_omap3430_Wugen_Object__heap;
__extern __FAR__ const CT__ti_sysbios_family_c64p_omap3430_Wugen_Object__heap ti_sysbios_family_c64p_omap3430_Wugen_Object__heap__C;

/* Object__sizeof */
typedef xdc_SizeT CT__ti_sysbios_family_c64p_omap3430_Wugen_Object__sizeof;
__extern __FAR__ const CT__ti_sysbios_family_c64p_omap3430_Wugen_Object__sizeof ti_sysbios_family_c64p_omap3430_Wugen_Object__sizeof__C;

/* Object__table */
typedef xdc_Ptr CT__ti_sysbios_family_c64p_omap3430_Wugen_Object__table;
__extern __FAR__ const CT__ti_sysbios_family_c64p_omap3430_Wugen_Object__table ti_sysbios_family_c64p_omap3430_Wugen_Object__table__C;


/*
 * ======== FUNCTION DECLARATIONS ========
 */

/* Module_startup */
#define ti_sysbios_family_c64p_omap3430_Wugen_Module_startup ti_sysbios_family_c64p_omap3430_Wugen_Module_startup__E
xdc__CODESECT(ti_sysbios_family_c64p_omap3430_Wugen_Module_startup__E, "ti_sysbios_family_c64p_omap3430_Wugen_Module_startup")
__extern xdc_Int ti_sysbios_family_c64p_omap3430_Wugen_Module_startup__E( xdc_Int state );
xdc__CODESECT(ti_sysbios_family_c64p_omap3430_Wugen_Module_startup__F, "ti_sysbios_family_c64p_omap3430_Wugen_Module_startup")
__extern xdc_Int ti_sysbios_family_c64p_omap3430_Wugen_Module_startup__F( xdc_Int state );
xdc__CODESECT(ti_sysbios_family_c64p_omap3430_Wugen_Module_startup__R, "ti_sysbios_family_c64p_omap3430_Wugen_Module_startup")
__extern xdc_Int ti_sysbios_family_c64p_omap3430_Wugen_Module_startup__R( xdc_Int state );

/* Module__startupDone__S */
xdc__CODESECT(ti_sysbios_family_c64p_omap3430_Wugen_Module__startupDone__S, "ti_sysbios_family_c64p_omap3430_Wugen_Module__startupDone")
__extern xdc_Bool ti_sysbios_family_c64p_omap3430_Wugen_Module__startupDone__S( void );

/* applyMasks__E */
#define ti_sysbios_family_c64p_omap3430_Wugen_applyMasks ti_sysbios_family_c64p_omap3430_Wugen_applyMasks__E
xdc__CODESECT(ti_sysbios_family_c64p_omap3430_Wugen_applyMasks__E, "ti_sysbios_family_c64p_omap3430_Wugen_applyMasks")
__extern xdc_Void ti_sysbios_family_c64p_omap3430_Wugen_applyMasks__E( xdc_UInt32 mask[] );
xdc__CODESECT(ti_sysbios_family_c64p_omap3430_Wugen_applyMasks__F, "ti_sysbios_family_c64p_omap3430_Wugen_applyMasks")
__extern xdc_Void ti_sysbios_family_c64p_omap3430_Wugen_applyMasks__F( xdc_UInt32 mask[] );
__extern xdc_Void ti_sysbios_family_c64p_omap3430_Wugen_applyMasks__R( xdc_UInt32 mask[] );

/* disableDma__E */
#define ti_sysbios_family_c64p_omap3430_Wugen_disableDma ti_sysbios_family_c64p_omap3430_Wugen_disableDma__E
xdc__CODESECT(ti_sysbios_family_c64p_omap3430_Wugen_disableDma__E, "ti_sysbios_family_c64p_omap3430_Wugen_disableDma")
__extern xdc_Void ti_sysbios_family_c64p_omap3430_Wugen_disableDma__E( xdc_UInt dmaid );
xdc__CODESECT(ti_sysbios_family_c64p_omap3430_Wugen_disableDma__F, "ti_sysbios_family_c64p_omap3430_Wugen_disableDma")
__extern xdc_Void ti_sysbios_family_c64p_omap3430_Wugen_disableDma__F( xdc_UInt dmaid );
__extern xdc_Void ti_sysbios_family_c64p_omap3430_Wugen_disableDma__R( xdc_UInt dmaid );

/* disableEvent__E */
#define ti_sysbios_family_c64p_omap3430_Wugen_disableEvent ti_sysbios_family_c64p_omap3430_Wugen_disableEvent__E
xdc__CODESECT(ti_sysbios_family_c64p_omap3430_Wugen_disableEvent__E, "ti_sysbios_family_c64p_omap3430_Wugen_disableEvent")
__extern xdc_Void ti_sysbios_family_c64p_omap3430_Wugen_disableEvent__E( xdc_UInt eventid );
xdc__CODESECT(ti_sysbios_family_c64p_omap3430_Wugen_disableEvent__F, "ti_sysbios_family_c64p_omap3430_Wugen_disableEvent")
__extern xdc_Void ti_sysbios_family_c64p_omap3430_Wugen_disableEvent__F( xdc_UInt eventid );
__extern xdc_Void ti_sysbios_family_c64p_omap3430_Wugen_disableEvent__R( xdc_UInt eventid );

/* enableDma__E */
#define ti_sysbios_family_c64p_omap3430_Wugen_enableDma ti_sysbios_family_c64p_omap3430_Wugen_enableDma__E
xdc__CODESECT(ti_sysbios_family_c64p_omap3430_Wugen_enableDma__E, "ti_sysbios_family_c64p_omap3430_Wugen_enableDma")
__extern xdc_Void ti_sysbios_family_c64p_omap3430_Wugen_enableDma__E( xdc_UInt dmaid );
xdc__CODESECT(ti_sysbios_family_c64p_omap3430_Wugen_enableDma__F, "ti_sysbios_family_c64p_omap3430_Wugen_enableDma")
__extern xdc_Void ti_sysbios_family_c64p_omap3430_Wugen_enableDma__F( xdc_UInt dmaid );
__extern xdc_Void ti_sysbios_family_c64p_omap3430_Wugen_enableDma__R( xdc_UInt dmaid );

/* enableEvent__E */
#define ti_sysbios_family_c64p_omap3430_Wugen_enableEvent ti_sysbios_family_c64p_omap3430_Wugen_enableEvent__E
xdc__CODESECT(ti_sysbios_family_c64p_omap3430_Wugen_enableEvent__E, "ti_sysbios_family_c64p_omap3430_Wugen_enableEvent")
__extern xdc_Void ti_sysbios_family_c64p_omap3430_Wugen_enableEvent__E( xdc_UInt eventid );
xdc__CODESECT(ti_sysbios_family_c64p_omap3430_Wugen_enableEvent__F, "ti_sysbios_family_c64p_omap3430_Wugen_enableEvent")
__extern xdc_Void ti_sysbios_family_c64p_omap3430_Wugen_enableEvent__F( xdc_UInt eventid );
__extern xdc_Void ti_sysbios_family_c64p_omap3430_Wugen_enableEvent__R( xdc_UInt eventid );

/* getMasks__E */
#define ti_sysbios_family_c64p_omap3430_Wugen_getMasks ti_sysbios_family_c64p_omap3430_Wugen_getMasks__E
xdc__CODESECT(ti_sysbios_family_c64p_omap3430_Wugen_getMasks__E, "ti_sysbios_family_c64p_omap3430_Wugen_getMasks")
__extern xdc_Void ti_sysbios_family_c64p_omap3430_Wugen_getMasks__E( xdc_UInt32 mask[] );
xdc__CODESECT(ti_sysbios_family_c64p_omap3430_Wugen_getMasks__F, "ti_sysbios_family_c64p_omap3430_Wugen_getMasks")
__extern xdc_Void ti_sysbios_family_c64p_omap3430_Wugen_getMasks__F( xdc_UInt32 mask[] );
__extern xdc_Void ti_sysbios_family_c64p_omap3430_Wugen_getMasks__R( xdc_UInt32 mask[] );

/* ierToMasks__E */
#define ti_sysbios_family_c64p_omap3430_Wugen_ierToMasks ti_sysbios_family_c64p_omap3430_Wugen_ierToMasks__E
xdc__CODESECT(ti_sysbios_family_c64p_omap3430_Wugen_ierToMasks__E, "ti_sysbios_family_c64p_omap3430_Wugen_ierToMasks")
__extern xdc_Void ti_sysbios_family_c64p_omap3430_Wugen_ierToMasks__E( xdc_UInt32 mask[] );
xdc__CODESECT(ti_sysbios_family_c64p_omap3430_Wugen_ierToMasks__F, "ti_sysbios_family_c64p_omap3430_Wugen_ierToMasks")
__extern xdc_Void ti_sysbios_family_c64p_omap3430_Wugen_ierToMasks__F( xdc_UInt32 mask[] );
__extern xdc_Void ti_sysbios_family_c64p_omap3430_Wugen_ierToMasks__R( xdc_UInt32 mask[] );


/*
 * ======== SYSTEM FUNCTIONS ========
 */

/* Module_startupDone */
#define ti_sysbios_family_c64p_omap3430_Wugen_Module_startupDone() ti_sysbios_family_c64p_omap3430_Wugen_Module__startupDone__S()

/* Object_heap */
#define ti_sysbios_family_c64p_omap3430_Wugen_Object_heap() ti_sysbios_family_c64p_omap3430_Wugen_Object__heap__C

/* Module_heap */
#define ti_sysbios_family_c64p_omap3430_Wugen_Module_heap() ti_sysbios_family_c64p_omap3430_Wugen_Object__heap__C

/* Module_id */
static inline CT__ti_sysbios_family_c64p_omap3430_Wugen_Module__id ti_sysbios_family_c64p_omap3430_Wugen_Module_id( void ) 
{
    return ti_sysbios_family_c64p_omap3430_Wugen_Module__id__C;
}

/* Module_hasMask */
static inline xdc_Bool ti_sysbios_family_c64p_omap3430_Wugen_Module_hasMask( void ) 
{
    return ti_sysbios_family_c64p_omap3430_Wugen_Module__diagsMask__C != NULL;
}

/* Module_getMask */
static inline xdc_Bits16 ti_sysbios_family_c64p_omap3430_Wugen_Module_getMask( void ) 
{
    return ti_sysbios_family_c64p_omap3430_Wugen_Module__diagsMask__C != NULL ? *ti_sysbios_family_c64p_omap3430_Wugen_Module__diagsMask__C : 0;
}

/* Module_setMask */
static inline xdc_Void ti_sysbios_family_c64p_omap3430_Wugen_Module_setMask( xdc_Bits16 mask ) 
{
    if (ti_sysbios_family_c64p_omap3430_Wugen_Module__diagsMask__C != NULL) *ti_sysbios_family_c64p_omap3430_Wugen_Module__diagsMask__C = mask;
}


/*
 * ======== EPILOGUE ========
 */

#ifdef ti_sysbios_family_c64p_omap3430_Wugen__top__
#undef __nested__
#endif

#endif /* ti_sysbios_family_c64p_omap3430_Wugen__include */


/*
 * ======== STATE STRUCTURES ========
 */

#if defined(__config__) || (!defined(__nested__) && defined(ti_sysbios_family_c64p_omap3430_Wugen__internalaccess))

#ifndef ti_sysbios_family_c64p_omap3430_Wugen__include_state
#define ti_sysbios_family_c64p_omap3430_Wugen__include_state


#endif /* ti_sysbios_family_c64p_omap3430_Wugen__include_state */

#endif

/*
 * ======== PREFIX ALIASES ========
 */

#if !defined(__nested__) && !defined(ti_sysbios_family_c64p_omap3430_Wugen__nolocalnames)

#ifndef ti_sysbios_family_c64p_omap3430_Wugen__localnames__done
#define ti_sysbios_family_c64p_omap3430_Wugen__localnames__done

/* module prefix */
#define Wugen_applyMasks ti_sysbios_family_c64p_omap3430_Wugen_applyMasks
#define Wugen_disableDma ti_sysbios_family_c64p_omap3430_Wugen_disableDma
#define Wugen_disableEvent ti_sysbios_family_c64p_omap3430_Wugen_disableEvent
#define Wugen_enableDma ti_sysbios_family_c64p_omap3430_Wugen_enableDma
#define Wugen_enableEvent ti_sysbios_family_c64p_omap3430_Wugen_enableEvent
#define Wugen_getMasks ti_sysbios_family_c64p_omap3430_Wugen_getMasks
#define Wugen_ierToMasks ti_sysbios_family_c64p_omap3430_Wugen_ierToMasks
#define Wugen_Module_name ti_sysbios_family_c64p_omap3430_Wugen_Module_name
#define Wugen_Module_id ti_sysbios_family_c64p_omap3430_Wugen_Module_id
#define Wugen_Module_startup ti_sysbios_family_c64p_omap3430_Wugen_Module_startup
#define Wugen_Module_startupDone ti_sysbios_family_c64p_omap3430_Wugen_Module_startupDone
#define Wugen_Module_hasMask ti_sysbios_family_c64p_omap3430_Wugen_Module_hasMask
#define Wugen_Module_getMask ti_sysbios_family_c64p_omap3430_Wugen_Module_getMask
#define Wugen_Module_setMask ti_sysbios_family_c64p_omap3430_Wugen_Module_setMask
#define Wugen_Object_heap ti_sysbios_family_c64p_omap3430_Wugen_Object_heap
#define Wugen_Module_heap ti_sysbios_family_c64p_omap3430_Wugen_Module_heap

#endif /* ti_sysbios_family_c64p_omap3430_Wugen__localnames__done */
#endif
