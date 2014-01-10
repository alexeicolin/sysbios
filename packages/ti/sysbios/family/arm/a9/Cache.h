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
 *     VIRTUAL FUNCTIONS
 *     FUNCTION DECLARATIONS
 *     CONVERTORS
 *     SYSTEM FUNCTIONS
 *     
 *     EPILOGUE
 *     STATE STRUCTURES
 *     PREFIX ALIASES
 */


/*
 * ======== PROLOGUE ========
 */

#ifndef ti_sysbios_family_arm_a9_Cache__include
#define ti_sysbios_family_arm_a9_Cache__include

#ifndef __nested__
#define __nested__
#define ti_sysbios_family_arm_a9_Cache__top__
#endif

#ifdef __cplusplus
#define __extern extern "C"
#else
#define __extern extern
#endif

#define ti_sysbios_family_arm_a9_Cache___VERS 150


/*
 * ======== INCLUDES ========
 */

#include <xdc/std.h>

#include <xdc/runtime/xdc.h>
#include <xdc/runtime/Types.h>
#include <ti/sysbios/family/arm/a9/package/package.defs.h>

#include <ti/sysbios/interfaces/ICache.h>
#include <xdc/runtime/Assert.h>


/*
 * ======== AUXILIARY DEFINITIONS ========
 */

/* Type */
typedef ti_sysbios_interfaces_ICache_Type ti_sysbios_family_arm_a9_Cache_Type;

/* sizeL1dCacheLine */
#define ti_sysbios_family_arm_a9_Cache_sizeL1dCacheLine (64)

/* sizeL1pCacheLine */
#define ti_sysbios_family_arm_a9_Cache_sizeL1pCacheLine (64)

/* sizeL2CacheLine */
#define ti_sysbios_family_arm_a9_Cache_sizeL2CacheLine (64)

/* Type_L1P */
#define ti_sysbios_family_arm_a9_Cache_Type_L1P ti_sysbios_interfaces_ICache_Type_L1P

/* Type_L1D */
#define ti_sysbios_family_arm_a9_Cache_Type_L1D ti_sysbios_interfaces_ICache_Type_L1D

/* Type_L1 */
#define ti_sysbios_family_arm_a9_Cache_Type_L1 ti_sysbios_interfaces_ICache_Type_L1

/* Type_L2P */
#define ti_sysbios_family_arm_a9_Cache_Type_L2P ti_sysbios_interfaces_ICache_Type_L2P

/* Type_L2D */
#define ti_sysbios_family_arm_a9_Cache_Type_L2D ti_sysbios_interfaces_ICache_Type_L2D

/* Type_L2 */
#define ti_sysbios_family_arm_a9_Cache_Type_L2 ti_sysbios_interfaces_ICache_Type_L2

/* Type_ALLP */
#define ti_sysbios_family_arm_a9_Cache_Type_ALLP ti_sysbios_interfaces_ICache_Type_ALLP

/* Type_ALLD */
#define ti_sysbios_family_arm_a9_Cache_Type_ALLD ti_sysbios_interfaces_ICache_Type_ALLD

/* Type_ALL */
#define ti_sysbios_family_arm_a9_Cache_Type_ALL ti_sysbios_interfaces_ICache_Type_ALL


/*
 * ======== INTERNAL DEFINITIONS ========
 */


/*
 * ======== MODULE-WIDE CONFIGS ========
 */

/* Module__diagsEnabled */
typedef xdc_Bits32 CT__ti_sysbios_family_arm_a9_Cache_Module__diagsEnabled;
__extern __FAR__ const CT__ti_sysbios_family_arm_a9_Cache_Module__diagsEnabled ti_sysbios_family_arm_a9_Cache_Module__diagsEnabled__C;

/* Module__diagsIncluded */
typedef xdc_Bits32 CT__ti_sysbios_family_arm_a9_Cache_Module__diagsIncluded;
__extern __FAR__ const CT__ti_sysbios_family_arm_a9_Cache_Module__diagsIncluded ti_sysbios_family_arm_a9_Cache_Module__diagsIncluded__C;

/* Module__diagsMask */
typedef xdc_Bits16* CT__ti_sysbios_family_arm_a9_Cache_Module__diagsMask;
__extern __FAR__ const CT__ti_sysbios_family_arm_a9_Cache_Module__diagsMask ti_sysbios_family_arm_a9_Cache_Module__diagsMask__C;

/* Module__gateObj */
typedef xdc_Ptr CT__ti_sysbios_family_arm_a9_Cache_Module__gateObj;
__extern __FAR__ const CT__ti_sysbios_family_arm_a9_Cache_Module__gateObj ti_sysbios_family_arm_a9_Cache_Module__gateObj__C;

/* Module__gatePrms */
typedef xdc_Ptr CT__ti_sysbios_family_arm_a9_Cache_Module__gatePrms;
__extern __FAR__ const CT__ti_sysbios_family_arm_a9_Cache_Module__gatePrms ti_sysbios_family_arm_a9_Cache_Module__gatePrms__C;

/* Module__id */
typedef xdc_runtime_Types_ModuleId CT__ti_sysbios_family_arm_a9_Cache_Module__id;
__extern __FAR__ const CT__ti_sysbios_family_arm_a9_Cache_Module__id ti_sysbios_family_arm_a9_Cache_Module__id__C;

/* Module__loggerDefined */
typedef xdc_Bool CT__ti_sysbios_family_arm_a9_Cache_Module__loggerDefined;
__extern __FAR__ const CT__ti_sysbios_family_arm_a9_Cache_Module__loggerDefined ti_sysbios_family_arm_a9_Cache_Module__loggerDefined__C;

/* Module__loggerObj */
typedef xdc_Ptr CT__ti_sysbios_family_arm_a9_Cache_Module__loggerObj;
__extern __FAR__ const CT__ti_sysbios_family_arm_a9_Cache_Module__loggerObj ti_sysbios_family_arm_a9_Cache_Module__loggerObj__C;

/* Module__loggerFxn0 */
typedef xdc_runtime_Types_LoggerFxn0 CT__ti_sysbios_family_arm_a9_Cache_Module__loggerFxn0;
__extern __FAR__ const CT__ti_sysbios_family_arm_a9_Cache_Module__loggerFxn0 ti_sysbios_family_arm_a9_Cache_Module__loggerFxn0__C;

/* Module__loggerFxn1 */
typedef xdc_runtime_Types_LoggerFxn1 CT__ti_sysbios_family_arm_a9_Cache_Module__loggerFxn1;
__extern __FAR__ const CT__ti_sysbios_family_arm_a9_Cache_Module__loggerFxn1 ti_sysbios_family_arm_a9_Cache_Module__loggerFxn1__C;

/* Module__loggerFxn2 */
typedef xdc_runtime_Types_LoggerFxn2 CT__ti_sysbios_family_arm_a9_Cache_Module__loggerFxn2;
__extern __FAR__ const CT__ti_sysbios_family_arm_a9_Cache_Module__loggerFxn2 ti_sysbios_family_arm_a9_Cache_Module__loggerFxn2__C;

/* Module__loggerFxn4 */
typedef xdc_runtime_Types_LoggerFxn4 CT__ti_sysbios_family_arm_a9_Cache_Module__loggerFxn4;
__extern __FAR__ const CT__ti_sysbios_family_arm_a9_Cache_Module__loggerFxn4 ti_sysbios_family_arm_a9_Cache_Module__loggerFxn4__C;

/* Module__loggerFxn8 */
typedef xdc_runtime_Types_LoggerFxn8 CT__ti_sysbios_family_arm_a9_Cache_Module__loggerFxn8;
__extern __FAR__ const CT__ti_sysbios_family_arm_a9_Cache_Module__loggerFxn8 ti_sysbios_family_arm_a9_Cache_Module__loggerFxn8__C;

/* Module__startupDoneFxn */
typedef xdc_Bool (*CT__ti_sysbios_family_arm_a9_Cache_Module__startupDoneFxn)(void);
__extern __FAR__ const CT__ti_sysbios_family_arm_a9_Cache_Module__startupDoneFxn ti_sysbios_family_arm_a9_Cache_Module__startupDoneFxn__C;

/* Object__count */
typedef xdc_Int CT__ti_sysbios_family_arm_a9_Cache_Object__count;
__extern __FAR__ const CT__ti_sysbios_family_arm_a9_Cache_Object__count ti_sysbios_family_arm_a9_Cache_Object__count__C;

/* Object__heap */
typedef xdc_runtime_IHeap_Handle CT__ti_sysbios_family_arm_a9_Cache_Object__heap;
__extern __FAR__ const CT__ti_sysbios_family_arm_a9_Cache_Object__heap ti_sysbios_family_arm_a9_Cache_Object__heap__C;

/* Object__sizeof */
typedef xdc_SizeT CT__ti_sysbios_family_arm_a9_Cache_Object__sizeof;
__extern __FAR__ const CT__ti_sysbios_family_arm_a9_Cache_Object__sizeof ti_sysbios_family_arm_a9_Cache_Object__sizeof__C;

/* Object__table */
typedef xdc_Ptr CT__ti_sysbios_family_arm_a9_Cache_Object__table;
__extern __FAR__ const CT__ti_sysbios_family_arm_a9_Cache_Object__table ti_sysbios_family_arm_a9_Cache_Object__table__C;

/* A_badBlockLength */
#define ti_sysbios_family_arm_a9_Cache_A_badBlockLength (ti_sysbios_family_arm_a9_Cache_A_badBlockLength__C)
typedef xdc_runtime_Assert_Id CT__ti_sysbios_family_arm_a9_Cache_A_badBlockLength;
__extern __FAR__ const CT__ti_sysbios_family_arm_a9_Cache_A_badBlockLength ti_sysbios_family_arm_a9_Cache_A_badBlockLength__C;

/* A_blockCrossesPage */
#define ti_sysbios_family_arm_a9_Cache_A_blockCrossesPage (ti_sysbios_family_arm_a9_Cache_A_blockCrossesPage__C)
typedef xdc_runtime_Assert_Id CT__ti_sysbios_family_arm_a9_Cache_A_blockCrossesPage;
__extern __FAR__ const CT__ti_sysbios_family_arm_a9_Cache_A_blockCrossesPage ti_sysbios_family_arm_a9_Cache_A_blockCrossesPage__C;

/* enableCache */
#ifdef ti_sysbios_family_arm_a9_Cache_enableCache__D
#define ti_sysbios_family_arm_a9_Cache_enableCache (ti_sysbios_family_arm_a9_Cache_enableCache__D)
#else
#define ti_sysbios_family_arm_a9_Cache_enableCache (ti_sysbios_family_arm_a9_Cache_enableCache__C)
typedef xdc_Bool CT__ti_sysbios_family_arm_a9_Cache_enableCache;
__extern __FAR__ const CT__ti_sysbios_family_arm_a9_Cache_enableCache ti_sysbios_family_arm_a9_Cache_enableCache__C;
#endif

/* branchPredictionEnabled */
#ifdef ti_sysbios_family_arm_a9_Cache_branchPredictionEnabled__D
#define ti_sysbios_family_arm_a9_Cache_branchPredictionEnabled (ti_sysbios_family_arm_a9_Cache_branchPredictionEnabled__D)
#else
#define ti_sysbios_family_arm_a9_Cache_branchPredictionEnabled (ti_sysbios_family_arm_a9_Cache_branchPredictionEnabled__C)
typedef xdc_Bool CT__ti_sysbios_family_arm_a9_Cache_branchPredictionEnabled;
__extern __FAR__ const CT__ti_sysbios_family_arm_a9_Cache_branchPredictionEnabled ti_sysbios_family_arm_a9_Cache_branchPredictionEnabled__C;
#endif


/*
 * ======== VIRTUAL FUNCTIONS ========
 */

/* Fxns__ */
struct ti_sysbios_family_arm_a9_Cache_Fxns__ {
    xdc_runtime_Types_Base* __base;
    const xdc_runtime_Types_SysFxns2* __sysp;
    xdc_Void (*enable)(xdc_Bits16);
    xdc_Void (*disable)(xdc_Bits16);
    xdc_Void (*inv)(xdc_Ptr, xdc_SizeT, xdc_Bits16, xdc_Bool);
    xdc_Void (*wb)(xdc_Ptr, xdc_SizeT, xdc_Bits16, xdc_Bool);
    xdc_Void (*wbInv)(xdc_Ptr, xdc_SizeT, xdc_Bits16, xdc_Bool);
    xdc_Void (*wbAll)(void);
    xdc_Void (*wbInvAll)(void);
    xdc_Void (*wait)(void);
    xdc_runtime_Types_SysFxns2 __sfxns;
};

/* Module__FXNS__C */
__extern const ti_sysbios_family_arm_a9_Cache_Fxns__ ti_sysbios_family_arm_a9_Cache_Module__FXNS__C;


/*
 * ======== FUNCTION DECLARATIONS ========
 */

/* Module_startup */
#define ti_sysbios_family_arm_a9_Cache_Module_startup( state ) (-1)

/* Module__startupDone__S */
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_Module__startupDone__S, "ti_sysbios_family_arm_a9_Cache_Module__startupDone")
__extern xdc_Bool ti_sysbios_family_arm_a9_Cache_Module__startupDone__S( void );

/* enable__E */
#define ti_sysbios_family_arm_a9_Cache_enable ti_sysbios_family_arm_a9_Cache_enable__E
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_enable__E, "ti_sysbios_family_arm_a9_Cache_enable")
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_enable__E( xdc_Bits16 type );
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_enable__F, "ti_sysbios_family_arm_a9_Cache_enable")
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_enable__F( xdc_Bits16 type );
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_enable__R( xdc_Bits16 type );

/* disable__E */
#define ti_sysbios_family_arm_a9_Cache_disable ti_sysbios_family_arm_a9_Cache_disable__E
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_disable__E, "ti_sysbios_family_arm_a9_Cache_disable")
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_disable__E( xdc_Bits16 type );
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_disable__F, "ti_sysbios_family_arm_a9_Cache_disable")
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_disable__F( xdc_Bits16 type );
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_disable__R( xdc_Bits16 type );

/* inv__E */
#define ti_sysbios_family_arm_a9_Cache_inv ti_sysbios_family_arm_a9_Cache_inv__E
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_inv__E, "ti_sysbios_family_arm_a9_Cache_inv")
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_inv__E( xdc_Ptr blockPtr, xdc_SizeT byteCnt, xdc_Bits16 type, xdc_Bool wait );
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_inv__F, "ti_sysbios_family_arm_a9_Cache_inv")
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_inv__F( xdc_Ptr blockPtr, xdc_SizeT byteCnt, xdc_Bits16 type, xdc_Bool wait );
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_inv__R( xdc_Ptr blockPtr, xdc_SizeT byteCnt, xdc_Bits16 type, xdc_Bool wait );

/* wb__E */
#define ti_sysbios_family_arm_a9_Cache_wb ti_sysbios_family_arm_a9_Cache_wb__E
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_wb__E, "ti_sysbios_family_arm_a9_Cache_wb")
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_wb__E( xdc_Ptr blockPtr, xdc_SizeT byteCnt, xdc_Bits16 type, xdc_Bool wait );
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_wb__F, "ti_sysbios_family_arm_a9_Cache_wb")
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_wb__F( xdc_Ptr blockPtr, xdc_SizeT byteCnt, xdc_Bits16 type, xdc_Bool wait );
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_wb__R( xdc_Ptr blockPtr, xdc_SizeT byteCnt, xdc_Bits16 type, xdc_Bool wait );

/* wbInv__E */
#define ti_sysbios_family_arm_a9_Cache_wbInv ti_sysbios_family_arm_a9_Cache_wbInv__E
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_wbInv__E, "ti_sysbios_family_arm_a9_Cache_wbInv")
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_wbInv__E( xdc_Ptr blockPtr, xdc_SizeT byteCnt, xdc_Bits16 type, xdc_Bool wait );
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_wbInv__F, "ti_sysbios_family_arm_a9_Cache_wbInv")
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_wbInv__F( xdc_Ptr blockPtr, xdc_SizeT byteCnt, xdc_Bits16 type, xdc_Bool wait );
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_wbInv__R( xdc_Ptr blockPtr, xdc_SizeT byteCnt, xdc_Bits16 type, xdc_Bool wait );

/* wbAll__E */
#define ti_sysbios_family_arm_a9_Cache_wbAll ti_sysbios_family_arm_a9_Cache_wbAll__E
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_wbAll__E, "ti_sysbios_family_arm_a9_Cache_wbAll")
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_wbAll__E( void );
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_wbAll__F, "ti_sysbios_family_arm_a9_Cache_wbAll")
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_wbAll__F( void );
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_wbAll__R( void );

/* wbInvAll__E */
#define ti_sysbios_family_arm_a9_Cache_wbInvAll ti_sysbios_family_arm_a9_Cache_wbInvAll__E
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_wbInvAll__E, "ti_sysbios_family_arm_a9_Cache_wbInvAll")
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_wbInvAll__E( void );
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_wbInvAll__F, "ti_sysbios_family_arm_a9_Cache_wbInvAll")
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_wbInvAll__F( void );
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_wbInvAll__R( void );

/* wait__E */
#define ti_sysbios_family_arm_a9_Cache_wait ti_sysbios_family_arm_a9_Cache_wait__E
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_wait__E, "ti_sysbios_family_arm_a9_Cache_wait")
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_wait__E( void );
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_wait__F, "ti_sysbios_family_arm_a9_Cache_wait")
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_wait__F( void );
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_wait__R( void );

/* getEnabled__E */
#define ti_sysbios_family_arm_a9_Cache_getEnabled ti_sysbios_family_arm_a9_Cache_getEnabled__E
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_getEnabled__E, "ti_sysbios_family_arm_a9_Cache_getEnabled")
__extern xdc_Bits16 ti_sysbios_family_arm_a9_Cache_getEnabled__E( void );
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_getEnabled__F, "ti_sysbios_family_arm_a9_Cache_getEnabled")
__extern xdc_Bits16 ti_sysbios_family_arm_a9_Cache_getEnabled__F( void );
__extern xdc_Bits16 ti_sysbios_family_arm_a9_Cache_getEnabled__R( void );

/* invL1dAll__E */
#define ti_sysbios_family_arm_a9_Cache_invL1dAll ti_sysbios_family_arm_a9_Cache_invL1dAll__E
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_invL1dAll__E, "ti_sysbios_family_arm_a9_Cache_invL1dAll")
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_invL1dAll__E( void );
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_invL1dAll__F, "ti_sysbios_family_arm_a9_Cache_invL1dAll")
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_invL1dAll__F( void );
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_invL1dAll__R( void );

/* invL1pAll__E */
#define ti_sysbios_family_arm_a9_Cache_invL1pAll ti_sysbios_family_arm_a9_Cache_invL1pAll__E
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_invL1pAll__E, "ti_sysbios_family_arm_a9_Cache_invL1pAll")
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_invL1pAll__E( void );
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_invL1pAll__F, "ti_sysbios_family_arm_a9_Cache_invL1pAll")
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_invL1pAll__F( void );
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_invL1pAll__R( void );

/* preLoad__E */
#define ti_sysbios_family_arm_a9_Cache_preLoad ti_sysbios_family_arm_a9_Cache_preLoad__E
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_preLoad__E, "ti_sysbios_family_arm_a9_Cache_preLoad")
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_preLoad__E( xdc_Ptr blockPtr, xdc_SizeT byteCnt );
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_preLoad__F, "ti_sysbios_family_arm_a9_Cache_preLoad")
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_preLoad__F( xdc_Ptr blockPtr, xdc_SizeT byteCnt );
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_preLoad__R( xdc_Ptr blockPtr, xdc_SizeT byteCnt );

/* enableBP__E */
#define ti_sysbios_family_arm_a9_Cache_enableBP ti_sysbios_family_arm_a9_Cache_enableBP__E
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_enableBP__E, "ti_sysbios_family_arm_a9_Cache_enableBP")
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_enableBP__E( void );
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_enableBP__F, "ti_sysbios_family_arm_a9_Cache_enableBP")
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_enableBP__F( void );
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_enableBP__R( void );

/* disableBP__E */
#define ti_sysbios_family_arm_a9_Cache_disableBP ti_sysbios_family_arm_a9_Cache_disableBP__E
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_disableBP__E, "ti_sysbios_family_arm_a9_Cache_disableBP")
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_disableBP__E( void );
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_disableBP__F, "ti_sysbios_family_arm_a9_Cache_disableBP")
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_disableBP__F( void );
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_disableBP__R( void );

/* enablePmc__E */
#define ti_sysbios_family_arm_a9_Cache_enablePmc ti_sysbios_family_arm_a9_Cache_enablePmc__E
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_enablePmc__E, "ti_sysbios_family_arm_a9_Cache_enablePmc")
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_enablePmc__E( void );
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_enablePmc__F, "ti_sysbios_family_arm_a9_Cache_enablePmc")
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_enablePmc__F( void );
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_enablePmc__R( void );

/* resetPmc__E */
#define ti_sysbios_family_arm_a9_Cache_resetPmc ti_sysbios_family_arm_a9_Cache_resetPmc__E
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_resetPmc__E, "ti_sysbios_family_arm_a9_Cache_resetPmc")
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_resetPmc__E( void );
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_resetPmc__F, "ti_sysbios_family_arm_a9_Cache_resetPmc")
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_resetPmc__F( void );
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_resetPmc__R( void );

/* getPmcMiss__E */
#define ti_sysbios_family_arm_a9_Cache_getPmcMiss ti_sysbios_family_arm_a9_Cache_getPmcMiss__E
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_getPmcMiss__E, "ti_sysbios_family_arm_a9_Cache_getPmcMiss")
__extern xdc_UInt ti_sysbios_family_arm_a9_Cache_getPmcMiss__E( void );
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_getPmcMiss__F, "ti_sysbios_family_arm_a9_Cache_getPmcMiss")
__extern xdc_UInt ti_sysbios_family_arm_a9_Cache_getPmcMiss__F( void );
__extern xdc_UInt ti_sysbios_family_arm_a9_Cache_getPmcMiss__R( void );

/* getPmcAxs__E */
#define ti_sysbios_family_arm_a9_Cache_getPmcAxs ti_sysbios_family_arm_a9_Cache_getPmcAxs__E
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_getPmcAxs__E, "ti_sysbios_family_arm_a9_Cache_getPmcAxs")
__extern xdc_UInt ti_sysbios_family_arm_a9_Cache_getPmcAxs__E( void );
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_getPmcAxs__F, "ti_sysbios_family_arm_a9_Cache_getPmcAxs")
__extern xdc_UInt ti_sysbios_family_arm_a9_Cache_getPmcAxs__F( void );
__extern xdc_UInt ti_sysbios_family_arm_a9_Cache_getPmcAxs__R( void );

/* getPmcCycle__E */
#define ti_sysbios_family_arm_a9_Cache_getPmcCycle ti_sysbios_family_arm_a9_Cache_getPmcCycle__E
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_getPmcCycle__E, "ti_sysbios_family_arm_a9_Cache_getPmcCycle")
__extern xdc_UInt ti_sysbios_family_arm_a9_Cache_getPmcCycle__E( void );
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_getPmcCycle__F, "ti_sysbios_family_arm_a9_Cache_getPmcCycle")
__extern xdc_UInt ti_sysbios_family_arm_a9_Cache_getPmcCycle__F( void );
__extern xdc_UInt ti_sysbios_family_arm_a9_Cache_getPmcCycle__R( void );

/* startup__I */
#define ti_sysbios_family_arm_a9_Cache_startup ti_sysbios_family_arm_a9_Cache_startup__I
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_startup__I, "ti_sysbios_family_arm_a9_Cache_startup")
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_startup__I( void );

/* disableD__I */
#define ti_sysbios_family_arm_a9_Cache_disableD ti_sysbios_family_arm_a9_Cache_disableD__I
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_disableD__I, "ti_sysbios_family_arm_a9_Cache_disableD")
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_disableD__I( void );

/* disableP__I */
#define ti_sysbios_family_arm_a9_Cache_disableP ti_sysbios_family_arm_a9_Cache_disableP__I
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_disableP__I, "ti_sysbios_family_arm_a9_Cache_disableP")
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_disableP__I( void );

/* enableD__I */
#define ti_sysbios_family_arm_a9_Cache_enableD ti_sysbios_family_arm_a9_Cache_enableD__I
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_enableD__I, "ti_sysbios_family_arm_a9_Cache_enableD")
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_enableD__I( void );

/* enableP__I */
#define ti_sysbios_family_arm_a9_Cache_enableP ti_sysbios_family_arm_a9_Cache_enableP__I
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_enableP__I, "ti_sysbios_family_arm_a9_Cache_enableP")
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_enableP__I( void );

/* invL1d__I */
#define ti_sysbios_family_arm_a9_Cache_invL1d ti_sysbios_family_arm_a9_Cache_invL1d__I
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_invL1d__I, "ti_sysbios_family_arm_a9_Cache_invL1d")
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_invL1d__I( xdc_Ptr blockPtr, xdc_SizeT byteCnt, xdc_Bool wait );

/* invL1p__I */
#define ti_sysbios_family_arm_a9_Cache_invL1p ti_sysbios_family_arm_a9_Cache_invL1p__I
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_invL1p__I, "ti_sysbios_family_arm_a9_Cache_invL1p")
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_invL1p__I( xdc_Ptr blockPtr, xdc_SizeT byteCnt, xdc_Bool wait );

/* preFetch__I */
#define ti_sysbios_family_arm_a9_Cache_preFetch ti_sysbios_family_arm_a9_Cache_preFetch__I
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_preFetch__I, "ti_sysbios_family_arm_a9_Cache_preFetch")
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_preFetch__I( xdc_Ptr blockPtr, xdc_SizeT byteCnt );

/* getCacheLevelInfo__I */
#define ti_sysbios_family_arm_a9_Cache_getCacheLevelInfo ti_sysbios_family_arm_a9_Cache_getCacheLevelInfo__I
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_getCacheLevelInfo__I, "ti_sysbios_family_arm_a9_Cache_getCacheLevelInfo")
__extern xdc_Bits32 ti_sysbios_family_arm_a9_Cache_getCacheLevelInfo__I( xdc_UInt level );

/* exit__I */
#define ti_sysbios_family_arm_a9_Cache_exit ti_sysbios_family_arm_a9_Cache_exit__I
xdc__CODESECT(ti_sysbios_family_arm_a9_Cache_exit__I, "ti_sysbios_family_arm_a9_Cache_exit")
__extern xdc_Void ti_sysbios_family_arm_a9_Cache_exit__I( xdc_Int status );


/*
 * ======== CONVERTORS ========
 */

/* Module_upCast */
static inline ti_sysbios_interfaces_ICache_Module ti_sysbios_family_arm_a9_Cache_Module_upCast( void )
{
    return (ti_sysbios_interfaces_ICache_Module)&ti_sysbios_family_arm_a9_Cache_Module__FXNS__C;
}

/* Module_to_ti_sysbios_interfaces_ICache */
#define ti_sysbios_family_arm_a9_Cache_Module_to_ti_sysbios_interfaces_ICache ti_sysbios_family_arm_a9_Cache_Module_upCast


/*
 * ======== SYSTEM FUNCTIONS ========
 */

/* Module_startupDone */
#define ti_sysbios_family_arm_a9_Cache_Module_startupDone() ti_sysbios_family_arm_a9_Cache_Module__startupDone__S()

/* Object_heap */
#define ti_sysbios_family_arm_a9_Cache_Object_heap() ti_sysbios_family_arm_a9_Cache_Object__heap__C

/* Module_heap */
#define ti_sysbios_family_arm_a9_Cache_Module_heap() ti_sysbios_family_arm_a9_Cache_Object__heap__C

/* Module_id */
static inline CT__ti_sysbios_family_arm_a9_Cache_Module__id ti_sysbios_family_arm_a9_Cache_Module_id( void ) 
{
    return ti_sysbios_family_arm_a9_Cache_Module__id__C;
}

/* Module_hasMask */
static inline xdc_Bool ti_sysbios_family_arm_a9_Cache_Module_hasMask( void ) 
{
    return ti_sysbios_family_arm_a9_Cache_Module__diagsMask__C != NULL;
}

/* Module_getMask */
static inline xdc_Bits16 ti_sysbios_family_arm_a9_Cache_Module_getMask( void ) 
{
    return ti_sysbios_family_arm_a9_Cache_Module__diagsMask__C != NULL ? *ti_sysbios_family_arm_a9_Cache_Module__diagsMask__C : 0;
}

/* Module_setMask */
static inline xdc_Void ti_sysbios_family_arm_a9_Cache_Module_setMask( xdc_Bits16 mask ) 
{
    if (ti_sysbios_family_arm_a9_Cache_Module__diagsMask__C != NULL) *ti_sysbios_family_arm_a9_Cache_Module__diagsMask__C = mask;
}


/*
 * ======== EPILOGUE ========
 */

#ifdef ti_sysbios_family_arm_a9_Cache__top__
#undef __nested__
#endif

#endif /* ti_sysbios_family_arm_a9_Cache__include */


/*
 * ======== STATE STRUCTURES ========
 */

#if defined(__config__) || (!defined(__nested__) && defined(ti_sysbios_family_arm_a9_Cache__internalaccess))

#ifndef ti_sysbios_family_arm_a9_Cache__include_state
#define ti_sysbios_family_arm_a9_Cache__include_state

/* Module_State */
struct ti_sysbios_family_arm_a9_Cache_Module_State {
    xdc_Bits32 l1dInfo;
    xdc_Bits32 l1pInfo;
    xdc_Bits32 l2Info;
    xdc_SizeT l2WaySize;
};

/* Module__state__V */
extern struct ti_sysbios_family_arm_a9_Cache_Module_State__ ti_sysbios_family_arm_a9_Cache_Module__state__V;

#endif /* ti_sysbios_family_arm_a9_Cache__include_state */

#endif


/*
 * ======== PREFIX ALIASES ========
 */

#if !defined(__nested__) && !defined(ti_sysbios_family_arm_a9_Cache__nolocalnames)

#ifndef ti_sysbios_family_arm_a9_Cache__localnames__done
#define ti_sysbios_family_arm_a9_Cache__localnames__done

/* module prefix */
#define Cache_Type ti_sysbios_family_arm_a9_Cache_Type
#define Cache_sizeL1dCacheLine ti_sysbios_family_arm_a9_Cache_sizeL1dCacheLine
#define Cache_sizeL1pCacheLine ti_sysbios_family_arm_a9_Cache_sizeL1pCacheLine
#define Cache_sizeL2CacheLine ti_sysbios_family_arm_a9_Cache_sizeL2CacheLine
#define Cache_Module_State ti_sysbios_family_arm_a9_Cache_Module_State
#define Cache_Type_L1P ti_sysbios_family_arm_a9_Cache_Type_L1P
#define Cache_Type_L1D ti_sysbios_family_arm_a9_Cache_Type_L1D
#define Cache_Type_L1 ti_sysbios_family_arm_a9_Cache_Type_L1
#define Cache_Type_L2P ti_sysbios_family_arm_a9_Cache_Type_L2P
#define Cache_Type_L2D ti_sysbios_family_arm_a9_Cache_Type_L2D
#define Cache_Type_L2 ti_sysbios_family_arm_a9_Cache_Type_L2
#define Cache_Type_ALLP ti_sysbios_family_arm_a9_Cache_Type_ALLP
#define Cache_Type_ALLD ti_sysbios_family_arm_a9_Cache_Type_ALLD
#define Cache_Type_ALL ti_sysbios_family_arm_a9_Cache_Type_ALL
#define Cache_A_badBlockLength ti_sysbios_family_arm_a9_Cache_A_badBlockLength
#define Cache_A_blockCrossesPage ti_sysbios_family_arm_a9_Cache_A_blockCrossesPage
#define Cache_enableCache ti_sysbios_family_arm_a9_Cache_enableCache
#define Cache_branchPredictionEnabled ti_sysbios_family_arm_a9_Cache_branchPredictionEnabled
#define Cache_enable ti_sysbios_family_arm_a9_Cache_enable
#define Cache_disable ti_sysbios_family_arm_a9_Cache_disable
#define Cache_inv ti_sysbios_family_arm_a9_Cache_inv
#define Cache_wb ti_sysbios_family_arm_a9_Cache_wb
#define Cache_wbInv ti_sysbios_family_arm_a9_Cache_wbInv
#define Cache_wbAll ti_sysbios_family_arm_a9_Cache_wbAll
#define Cache_wbInvAll ti_sysbios_family_arm_a9_Cache_wbInvAll
#define Cache_wait ti_sysbios_family_arm_a9_Cache_wait
#define Cache_getEnabled ti_sysbios_family_arm_a9_Cache_getEnabled
#define Cache_invL1dAll ti_sysbios_family_arm_a9_Cache_invL1dAll
#define Cache_invL1pAll ti_sysbios_family_arm_a9_Cache_invL1pAll
#define Cache_preLoad ti_sysbios_family_arm_a9_Cache_preLoad
#define Cache_enableBP ti_sysbios_family_arm_a9_Cache_enableBP
#define Cache_disableBP ti_sysbios_family_arm_a9_Cache_disableBP
#define Cache_enablePmc ti_sysbios_family_arm_a9_Cache_enablePmc
#define Cache_resetPmc ti_sysbios_family_arm_a9_Cache_resetPmc
#define Cache_getPmcMiss ti_sysbios_family_arm_a9_Cache_getPmcMiss
#define Cache_getPmcAxs ti_sysbios_family_arm_a9_Cache_getPmcAxs
#define Cache_getPmcCycle ti_sysbios_family_arm_a9_Cache_getPmcCycle
#define Cache_Module_name ti_sysbios_family_arm_a9_Cache_Module_name
#define Cache_Module_id ti_sysbios_family_arm_a9_Cache_Module_id
#define Cache_Module_startup ti_sysbios_family_arm_a9_Cache_Module_startup
#define Cache_Module_startupDone ti_sysbios_family_arm_a9_Cache_Module_startupDone
#define Cache_Module_hasMask ti_sysbios_family_arm_a9_Cache_Module_hasMask
#define Cache_Module_getMask ti_sysbios_family_arm_a9_Cache_Module_getMask
#define Cache_Module_setMask ti_sysbios_family_arm_a9_Cache_Module_setMask
#define Cache_Object_heap ti_sysbios_family_arm_a9_Cache_Object_heap
#define Cache_Module_heap ti_sysbios_family_arm_a9_Cache_Module_heap
#define Cache_Module_upCast ti_sysbios_family_arm_a9_Cache_Module_upCast
#define Cache_Module_to_ti_sysbios_interfaces_ICache ti_sysbios_family_arm_a9_Cache_Module_to_ti_sysbios_interfaces_ICache

#endif /* ti_sysbios_family_arm_a9_Cache__localnames__done */
#endif
