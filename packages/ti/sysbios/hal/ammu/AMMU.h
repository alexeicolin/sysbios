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

#ifndef ti_sysbios_hal_ammu_AMMU__include
#define ti_sysbios_hal_ammu_AMMU__include

#ifndef __nested__
#define __nested__
#define ti_sysbios_hal_ammu_AMMU__top__
#endif

#ifdef __cplusplus
#define __extern extern "C"
#else
#define __extern extern
#endif

#define ti_sysbios_hal_ammu_AMMU___VERS 150


/*
 * ======== INCLUDES ========
 */

#include <xdc/std.h>

#include <xdc/runtime/xdc.h>
#include <xdc/runtime/Types.h>
#include <ti/sysbios/hal/ammu/package/package.defs.h>

#include <xdc/runtime/IModule.h>
#include <xdc/runtime/Types.h>


/*
 * ======== AUXILIARY DEFINITIONS ========
 */

/* MMU */
typedef xdc_Char* __T1_ti_sysbios_hal_ammu_AMMU_MMU__LARGE_ADDR;
typedef xdc_Char* __ARRAY1_ti_sysbios_hal_ammu_AMMU_MMU__LARGE_ADDR[8];
typedef __ARRAY1_ti_sysbios_hal_ammu_AMMU_MMU__LARGE_ADDR __TA_ti_sysbios_hal_ammu_AMMU_MMU__LARGE_ADDR;
typedef xdc_Char* __T1_ti_sysbios_hal_ammu_AMMU_MMU__LARGE_XLTE;
typedef xdc_Char* __ARRAY1_ti_sysbios_hal_ammu_AMMU_MMU__LARGE_XLTE[8];
typedef __ARRAY1_ti_sysbios_hal_ammu_AMMU_MMU__LARGE_XLTE __TA_ti_sysbios_hal_ammu_AMMU_MMU__LARGE_XLTE;
typedef xdc_UInt32 __T1_ti_sysbios_hal_ammu_AMMU_MMU__LARGE_POLICY;
typedef xdc_UInt32 __ARRAY1_ti_sysbios_hal_ammu_AMMU_MMU__LARGE_POLICY[8];
typedef __ARRAY1_ti_sysbios_hal_ammu_AMMU_MMU__LARGE_POLICY __TA_ti_sysbios_hal_ammu_AMMU_MMU__LARGE_POLICY;
typedef xdc_Char* __T1_ti_sysbios_hal_ammu_AMMU_MMU__MEDIUM_ADDR;
typedef xdc_Char* __ARRAY1_ti_sysbios_hal_ammu_AMMU_MMU__MEDIUM_ADDR[16];
typedef __ARRAY1_ti_sysbios_hal_ammu_AMMU_MMU__MEDIUM_ADDR __TA_ti_sysbios_hal_ammu_AMMU_MMU__MEDIUM_ADDR;
typedef xdc_Char* __T1_ti_sysbios_hal_ammu_AMMU_MMU__MEDIUM_XLTE;
typedef xdc_Char* __ARRAY1_ti_sysbios_hal_ammu_AMMU_MMU__MEDIUM_XLTE[16];
typedef __ARRAY1_ti_sysbios_hal_ammu_AMMU_MMU__MEDIUM_XLTE __TA_ti_sysbios_hal_ammu_AMMU_MMU__MEDIUM_XLTE;
typedef xdc_UInt32 __T1_ti_sysbios_hal_ammu_AMMU_MMU__MEDIUM_POLICY;
typedef xdc_UInt32 __ARRAY1_ti_sysbios_hal_ammu_AMMU_MMU__MEDIUM_POLICY[16];
typedef __ARRAY1_ti_sysbios_hal_ammu_AMMU_MMU__MEDIUM_POLICY __TA_ti_sysbios_hal_ammu_AMMU_MMU__MEDIUM_POLICY;
typedef xdc_Char* __T1_ti_sysbios_hal_ammu_AMMU_MMU__SMALL_ADDR;
typedef xdc_Char* __ARRAY1_ti_sysbios_hal_ammu_AMMU_MMU__SMALL_ADDR[32];
typedef __ARRAY1_ti_sysbios_hal_ammu_AMMU_MMU__SMALL_ADDR __TA_ti_sysbios_hal_ammu_AMMU_MMU__SMALL_ADDR;
typedef xdc_Char* __T1_ti_sysbios_hal_ammu_AMMU_MMU__SMALL_XLTE;
typedef xdc_Char* __ARRAY1_ti_sysbios_hal_ammu_AMMU_MMU__SMALL_XLTE[32];
typedef __ARRAY1_ti_sysbios_hal_ammu_AMMU_MMU__SMALL_XLTE __TA_ti_sysbios_hal_ammu_AMMU_MMU__SMALL_XLTE;
typedef xdc_UInt32 __T1_ti_sysbios_hal_ammu_AMMU_MMU__SMALL_POLICY;
typedef xdc_UInt32 __ARRAY1_ti_sysbios_hal_ammu_AMMU_MMU__SMALL_POLICY[32];
typedef __ARRAY1_ti_sysbios_hal_ammu_AMMU_MMU__SMALL_POLICY __TA_ti_sysbios_hal_ammu_AMMU_MMU__SMALL_POLICY;
typedef xdc_UInt32 __T1_ti_sysbios_hal_ammu_AMMU_MMU__SMALL_MAINT;
typedef xdc_UInt32 __ARRAY1_ti_sysbios_hal_ammu_AMMU_MMU__SMALL_MAINT[32];
typedef __ARRAY1_ti_sysbios_hal_ammu_AMMU_MMU__SMALL_MAINT __TA_ti_sysbios_hal_ammu_AMMU_MMU__SMALL_MAINT;
typedef xdc_Char* __T1_ti_sysbios_hal_ammu_AMMU_MMU__LINE_ADDR;
typedef xdc_Char* __ARRAY1_ti_sysbios_hal_ammu_AMMU_MMU__LINE_ADDR[32];
typedef __ARRAY1_ti_sysbios_hal_ammu_AMMU_MMU__LINE_ADDR __TA_ti_sysbios_hal_ammu_AMMU_MMU__LINE_ADDR;
typedef xdc_Char* __T1_ti_sysbios_hal_ammu_AMMU_MMU__LINE_XLTE;
typedef xdc_Char* __ARRAY1_ti_sysbios_hal_ammu_AMMU_MMU__LINE_XLTE[32];
typedef __ARRAY1_ti_sysbios_hal_ammu_AMMU_MMU__LINE_XLTE __TA_ti_sysbios_hal_ammu_AMMU_MMU__LINE_XLTE;
typedef xdc_UInt32 __T1_ti_sysbios_hal_ammu_AMMU_MMU__LINE_POLICY;
typedef xdc_UInt32 __ARRAY1_ti_sysbios_hal_ammu_AMMU_MMU__LINE_POLICY[32];
typedef __ARRAY1_ti_sysbios_hal_ammu_AMMU_MMU__LINE_POLICY __TA_ti_sysbios_hal_ammu_AMMU_MMU__LINE_POLICY;
struct ti_sysbios_hal_ammu_AMMU_MMU {
    __TA_ti_sysbios_hal_ammu_AMMU_MMU__LARGE_ADDR LARGE_ADDR;
    __TA_ti_sysbios_hal_ammu_AMMU_MMU__LARGE_XLTE LARGE_XLTE;
    __TA_ti_sysbios_hal_ammu_AMMU_MMU__LARGE_POLICY LARGE_POLICY;
    __TA_ti_sysbios_hal_ammu_AMMU_MMU__MEDIUM_ADDR MEDIUM_ADDR;
    __TA_ti_sysbios_hal_ammu_AMMU_MMU__MEDIUM_XLTE MEDIUM_XLTE;
    __TA_ti_sysbios_hal_ammu_AMMU_MMU__MEDIUM_POLICY MEDIUM_POLICY;
    __TA_ti_sysbios_hal_ammu_AMMU_MMU__SMALL_ADDR SMALL_ADDR;
    __TA_ti_sysbios_hal_ammu_AMMU_MMU__SMALL_XLTE SMALL_XLTE;
    __TA_ti_sysbios_hal_ammu_AMMU_MMU__SMALL_POLICY SMALL_POLICY;
    __TA_ti_sysbios_hal_ammu_AMMU_MMU__SMALL_MAINT SMALL_MAINT;
    __TA_ti_sysbios_hal_ammu_AMMU_MMU__LINE_ADDR LINE_ADDR;
    __TA_ti_sysbios_hal_ammu_AMMU_MMU__LINE_XLTE LINE_XLTE;
    __TA_ti_sysbios_hal_ammu_AMMU_MMU__LINE_POLICY LINE_POLICY;
    xdc_Char* DEBUG_XLTE;
    xdc_UInt32 DEBUG_POLICY;
    xdc_UInt32 MAINT;
    xdc_Char* MSTART;
    xdc_Char* MEND;
    xdc_UInt32 MSTAT;
};

/* mmu */
#define ti_sysbios_hal_ammu_AMMU_mmu ti_sysbios_hal_ammu_AMMU_mmu
__extern volatile ti_sysbios_hal_ammu_AMMU_MMU ti_sysbios_hal_ammu_AMMU_mmu;


/*
 * ======== INTERNAL DEFINITIONS ========
 */


/*
 * ======== MODULE-WIDE CONFIGS ========
 */

/* Module__diagsEnabled */
typedef xdc_Bits32 CT__ti_sysbios_hal_ammu_AMMU_Module__diagsEnabled;
__extern __FAR__ const CT__ti_sysbios_hal_ammu_AMMU_Module__diagsEnabled ti_sysbios_hal_ammu_AMMU_Module__diagsEnabled__C;

/* Module__diagsIncluded */
typedef xdc_Bits32 CT__ti_sysbios_hal_ammu_AMMU_Module__diagsIncluded;
__extern __FAR__ const CT__ti_sysbios_hal_ammu_AMMU_Module__diagsIncluded ti_sysbios_hal_ammu_AMMU_Module__diagsIncluded__C;

/* Module__diagsMask */
typedef xdc_Bits16* CT__ti_sysbios_hal_ammu_AMMU_Module__diagsMask;
__extern __FAR__ const CT__ti_sysbios_hal_ammu_AMMU_Module__diagsMask ti_sysbios_hal_ammu_AMMU_Module__diagsMask__C;

/* Module__gateObj */
typedef xdc_Ptr CT__ti_sysbios_hal_ammu_AMMU_Module__gateObj;
__extern __FAR__ const CT__ti_sysbios_hal_ammu_AMMU_Module__gateObj ti_sysbios_hal_ammu_AMMU_Module__gateObj__C;

/* Module__gatePrms */
typedef xdc_Ptr CT__ti_sysbios_hal_ammu_AMMU_Module__gatePrms;
__extern __FAR__ const CT__ti_sysbios_hal_ammu_AMMU_Module__gatePrms ti_sysbios_hal_ammu_AMMU_Module__gatePrms__C;

/* Module__id */
typedef xdc_runtime_Types_ModuleId CT__ti_sysbios_hal_ammu_AMMU_Module__id;
__extern __FAR__ const CT__ti_sysbios_hal_ammu_AMMU_Module__id ti_sysbios_hal_ammu_AMMU_Module__id__C;

/* Module__loggerDefined */
typedef xdc_Bool CT__ti_sysbios_hal_ammu_AMMU_Module__loggerDefined;
__extern __FAR__ const CT__ti_sysbios_hal_ammu_AMMU_Module__loggerDefined ti_sysbios_hal_ammu_AMMU_Module__loggerDefined__C;

/* Module__loggerObj */
typedef xdc_Ptr CT__ti_sysbios_hal_ammu_AMMU_Module__loggerObj;
__extern __FAR__ const CT__ti_sysbios_hal_ammu_AMMU_Module__loggerObj ti_sysbios_hal_ammu_AMMU_Module__loggerObj__C;

/* Module__loggerFxn0 */
typedef xdc_runtime_Types_LoggerFxn0 CT__ti_sysbios_hal_ammu_AMMU_Module__loggerFxn0;
__extern __FAR__ const CT__ti_sysbios_hal_ammu_AMMU_Module__loggerFxn0 ti_sysbios_hal_ammu_AMMU_Module__loggerFxn0__C;

/* Module__loggerFxn1 */
typedef xdc_runtime_Types_LoggerFxn1 CT__ti_sysbios_hal_ammu_AMMU_Module__loggerFxn1;
__extern __FAR__ const CT__ti_sysbios_hal_ammu_AMMU_Module__loggerFxn1 ti_sysbios_hal_ammu_AMMU_Module__loggerFxn1__C;

/* Module__loggerFxn2 */
typedef xdc_runtime_Types_LoggerFxn2 CT__ti_sysbios_hal_ammu_AMMU_Module__loggerFxn2;
__extern __FAR__ const CT__ti_sysbios_hal_ammu_AMMU_Module__loggerFxn2 ti_sysbios_hal_ammu_AMMU_Module__loggerFxn2__C;

/* Module__loggerFxn4 */
typedef xdc_runtime_Types_LoggerFxn4 CT__ti_sysbios_hal_ammu_AMMU_Module__loggerFxn4;
__extern __FAR__ const CT__ti_sysbios_hal_ammu_AMMU_Module__loggerFxn4 ti_sysbios_hal_ammu_AMMU_Module__loggerFxn4__C;

/* Module__loggerFxn8 */
typedef xdc_runtime_Types_LoggerFxn8 CT__ti_sysbios_hal_ammu_AMMU_Module__loggerFxn8;
__extern __FAR__ const CT__ti_sysbios_hal_ammu_AMMU_Module__loggerFxn8 ti_sysbios_hal_ammu_AMMU_Module__loggerFxn8__C;

/* Module__startupDoneFxn */
typedef xdc_Bool (*CT__ti_sysbios_hal_ammu_AMMU_Module__startupDoneFxn)(void);
__extern __FAR__ const CT__ti_sysbios_hal_ammu_AMMU_Module__startupDoneFxn ti_sysbios_hal_ammu_AMMU_Module__startupDoneFxn__C;

/* Object__count */
typedef xdc_Int CT__ti_sysbios_hal_ammu_AMMU_Object__count;
__extern __FAR__ const CT__ti_sysbios_hal_ammu_AMMU_Object__count ti_sysbios_hal_ammu_AMMU_Object__count__C;

/* Object__heap */
typedef xdc_runtime_IHeap_Handle CT__ti_sysbios_hal_ammu_AMMU_Object__heap;
__extern __FAR__ const CT__ti_sysbios_hal_ammu_AMMU_Object__heap ti_sysbios_hal_ammu_AMMU_Object__heap__C;

/* Object__sizeof */
typedef xdc_SizeT CT__ti_sysbios_hal_ammu_AMMU_Object__sizeof;
__extern __FAR__ const CT__ti_sysbios_hal_ammu_AMMU_Object__sizeof ti_sysbios_hal_ammu_AMMU_Object__sizeof__C;

/* Object__table */
typedef xdc_Ptr CT__ti_sysbios_hal_ammu_AMMU_Object__table;
__extern __FAR__ const CT__ti_sysbios_hal_ammu_AMMU_Object__table ti_sysbios_hal_ammu_AMMU_Object__table__C;

/* configureAmmu */
#ifdef ti_sysbios_hal_ammu_AMMU_configureAmmu__D
#define ti_sysbios_hal_ammu_AMMU_configureAmmu (ti_sysbios_hal_ammu_AMMU_configureAmmu__D)
#else
#define ti_sysbios_hal_ammu_AMMU_configureAmmu (ti_sysbios_hal_ammu_AMMU_configureAmmu__C)
typedef xdc_Bool CT__ti_sysbios_hal_ammu_AMMU_configureAmmu;
__extern __FAR__ const CT__ti_sysbios_hal_ammu_AMMU_configureAmmu ti_sysbios_hal_ammu_AMMU_configureAmmu__C;
#endif

/* numLargePages */
#ifdef ti_sysbios_hal_ammu_AMMU_numLargePages__D
#define ti_sysbios_hal_ammu_AMMU_numLargePages (ti_sysbios_hal_ammu_AMMU_numLargePages__D)
#else
#define ti_sysbios_hal_ammu_AMMU_numLargePages (ti_sysbios_hal_ammu_AMMU_numLargePages__C)
typedef xdc_UInt CT__ti_sysbios_hal_ammu_AMMU_numLargePages;
__extern __FAR__ const CT__ti_sysbios_hal_ammu_AMMU_numLargePages ti_sysbios_hal_ammu_AMMU_numLargePages__C;
#endif

/* numMediumPages */
#ifdef ti_sysbios_hal_ammu_AMMU_numMediumPages__D
#define ti_sysbios_hal_ammu_AMMU_numMediumPages (ti_sysbios_hal_ammu_AMMU_numMediumPages__D)
#else
#define ti_sysbios_hal_ammu_AMMU_numMediumPages (ti_sysbios_hal_ammu_AMMU_numMediumPages__C)
typedef xdc_UInt CT__ti_sysbios_hal_ammu_AMMU_numMediumPages;
__extern __FAR__ const CT__ti_sysbios_hal_ammu_AMMU_numMediumPages ti_sysbios_hal_ammu_AMMU_numMediumPages__C;
#endif

/* numSmallPages */
#ifdef ti_sysbios_hal_ammu_AMMU_numSmallPages__D
#define ti_sysbios_hal_ammu_AMMU_numSmallPages (ti_sysbios_hal_ammu_AMMU_numSmallPages__D)
#else
#define ti_sysbios_hal_ammu_AMMU_numSmallPages (ti_sysbios_hal_ammu_AMMU_numSmallPages__C)
typedef xdc_UInt CT__ti_sysbios_hal_ammu_AMMU_numSmallPages;
__extern __FAR__ const CT__ti_sysbios_hal_ammu_AMMU_numSmallPages ti_sysbios_hal_ammu_AMMU_numSmallPages__C;
#endif

/* numLinePages */
#ifdef ti_sysbios_hal_ammu_AMMU_numLinePages__D
#define ti_sysbios_hal_ammu_AMMU_numLinePages (ti_sysbios_hal_ammu_AMMU_numLinePages__D)
#else
#define ti_sysbios_hal_ammu_AMMU_numLinePages (ti_sysbios_hal_ammu_AMMU_numLinePages__C)
typedef xdc_UInt CT__ti_sysbios_hal_ammu_AMMU_numLinePages;
__extern __FAR__ const CT__ti_sysbios_hal_ammu_AMMU_numLinePages ti_sysbios_hal_ammu_AMMU_numLinePages__C;
#endif

/* mmuInitConfig */
#define ti_sysbios_hal_ammu_AMMU_mmuInitConfig (ti_sysbios_hal_ammu_AMMU_mmuInitConfig__C)
typedef ti_sysbios_hal_ammu_AMMU_MMU CT__ti_sysbios_hal_ammu_AMMU_mmuInitConfig;
__extern __FAR__ const CT__ti_sysbios_hal_ammu_AMMU_mmuInitConfig ti_sysbios_hal_ammu_AMMU_mmuInitConfig__C;


/*
 * ======== FUNCTION DECLARATIONS ========
 */

/* Module_startup */
#define ti_sysbios_hal_ammu_AMMU_Module_startup( state ) (-1)

/* Module__startupDone__S */
xdc__CODESECT(ti_sysbios_hal_ammu_AMMU_Module__startupDone__S, "ti_sysbios_hal_ammu_AMMU_Module__startupDone")
__extern xdc_Bool ti_sysbios_hal_ammu_AMMU_Module__startupDone__S( void );

/* invAll__E */
#define ti_sysbios_hal_ammu_AMMU_invAll ti_sysbios_hal_ammu_AMMU_invAll__E
xdc__CODESECT(ti_sysbios_hal_ammu_AMMU_invAll__E, "ti_sysbios_hal_ammu_AMMU_invAll")
__extern xdc_Void ti_sysbios_hal_ammu_AMMU_invAll__E( void );
xdc__CODESECT(ti_sysbios_hal_ammu_AMMU_invAll__F, "ti_sysbios_hal_ammu_AMMU_invAll")
__extern xdc_Void ti_sysbios_hal_ammu_AMMU_invAll__F( void );
__extern xdc_Void ti_sysbios_hal_ammu_AMMU_invAll__R( void );

/* dumpAmmu__E */
#define ti_sysbios_hal_ammu_AMMU_dumpAmmu ti_sysbios_hal_ammu_AMMU_dumpAmmu__E
xdc__CODESECT(ti_sysbios_hal_ammu_AMMU_dumpAmmu__E, "ti_sysbios_hal_ammu_AMMU_dumpAmmu")
__extern xdc_Void ti_sysbios_hal_ammu_AMMU_dumpAmmu__E( void );
xdc__CODESECT(ti_sysbios_hal_ammu_AMMU_dumpAmmu__F, "ti_sysbios_hal_ammu_AMMU_dumpAmmu")
__extern xdc_Void ti_sysbios_hal_ammu_AMMU_dumpAmmu__F( void );
__extern xdc_Void ti_sysbios_hal_ammu_AMMU_dumpAmmu__R( void );

/* init__I */
#define ti_sysbios_hal_ammu_AMMU_init ti_sysbios_hal_ammu_AMMU_init__I
xdc__CODESECT(ti_sysbios_hal_ammu_AMMU_init__I, "ti_sysbios_hal_ammu_AMMU_init")
__extern xdc_Void ti_sysbios_hal_ammu_AMMU_init__I( void );


/*
 * ======== SYSTEM FUNCTIONS ========
 */

/* Module_startupDone */
#define ti_sysbios_hal_ammu_AMMU_Module_startupDone() ti_sysbios_hal_ammu_AMMU_Module__startupDone__S()

/* Object_heap */
#define ti_sysbios_hal_ammu_AMMU_Object_heap() ti_sysbios_hal_ammu_AMMU_Object__heap__C

/* Module_heap */
#define ti_sysbios_hal_ammu_AMMU_Module_heap() ti_sysbios_hal_ammu_AMMU_Object__heap__C

/* Module_id */
static inline CT__ti_sysbios_hal_ammu_AMMU_Module__id ti_sysbios_hal_ammu_AMMU_Module_id( void ) 
{
    return ti_sysbios_hal_ammu_AMMU_Module__id__C;
}

/* Module_hasMask */
static inline xdc_Bool ti_sysbios_hal_ammu_AMMU_Module_hasMask( void ) 
{
    return ti_sysbios_hal_ammu_AMMU_Module__diagsMask__C != NULL;
}

/* Module_getMask */
static inline xdc_Bits16 ti_sysbios_hal_ammu_AMMU_Module_getMask( void ) 
{
    return ti_sysbios_hal_ammu_AMMU_Module__diagsMask__C != NULL ? *ti_sysbios_hal_ammu_AMMU_Module__diagsMask__C : 0;
}

/* Module_setMask */
static inline xdc_Void ti_sysbios_hal_ammu_AMMU_Module_setMask( xdc_Bits16 mask ) 
{
    if (ti_sysbios_hal_ammu_AMMU_Module__diagsMask__C != NULL) *ti_sysbios_hal_ammu_AMMU_Module__diagsMask__C = mask;
}


/*
 * ======== EPILOGUE ========
 */

#ifdef ti_sysbios_hal_ammu_AMMU__top__
#undef __nested__
#endif

#endif /* ti_sysbios_hal_ammu_AMMU__include */


/*
 * ======== STATE STRUCTURES ========
 */

#if defined(__config__) || (!defined(__nested__) && defined(ti_sysbios_hal_ammu_AMMU__internalaccess))

#ifndef ti_sysbios_hal_ammu_AMMU__include_state
#define ti_sysbios_hal_ammu_AMMU__include_state


#endif /* ti_sysbios_hal_ammu_AMMU__include_state */

#endif

/*
 * ======== PREFIX ALIASES ========
 */

#if !defined(__nested__) && !defined(ti_sysbios_hal_ammu_AMMU__nolocalnames)

#ifndef ti_sysbios_hal_ammu_AMMU__localnames__done
#define ti_sysbios_hal_ammu_AMMU__localnames__done

/* module prefix */
#define AMMU_MMU ti_sysbios_hal_ammu_AMMU_MMU
#define AMMU_mmu ti_sysbios_hal_ammu_AMMU_mmu
#define AMMU_configureAmmu ti_sysbios_hal_ammu_AMMU_configureAmmu
#define AMMU_numLargePages ti_sysbios_hal_ammu_AMMU_numLargePages
#define AMMU_numMediumPages ti_sysbios_hal_ammu_AMMU_numMediumPages
#define AMMU_numSmallPages ti_sysbios_hal_ammu_AMMU_numSmallPages
#define AMMU_numLinePages ti_sysbios_hal_ammu_AMMU_numLinePages
#define AMMU_mmuInitConfig ti_sysbios_hal_ammu_AMMU_mmuInitConfig
#define AMMU_invAll ti_sysbios_hal_ammu_AMMU_invAll
#define AMMU_dumpAmmu ti_sysbios_hal_ammu_AMMU_dumpAmmu
#define AMMU_Module_name ti_sysbios_hal_ammu_AMMU_Module_name
#define AMMU_Module_id ti_sysbios_hal_ammu_AMMU_Module_id
#define AMMU_Module_startup ti_sysbios_hal_ammu_AMMU_Module_startup
#define AMMU_Module_startupDone ti_sysbios_hal_ammu_AMMU_Module_startupDone
#define AMMU_Module_hasMask ti_sysbios_hal_ammu_AMMU_Module_hasMask
#define AMMU_Module_getMask ti_sysbios_hal_ammu_AMMU_Module_getMask
#define AMMU_Module_setMask ti_sysbios_hal_ammu_AMMU_Module_setMask
#define AMMU_Object_heap ti_sysbios_hal_ammu_AMMU_Object_heap
#define AMMU_Module_heap ti_sysbios_hal_ammu_AMMU_Module_heap

#endif /* ti_sysbios_hal_ammu_AMMU__localnames__done */
#endif
