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

#ifndef ti_sysbios_family_c66_Cache__include
#define ti_sysbios_family_c66_Cache__include

#ifndef __nested__
#define __nested__
#define ti_sysbios_family_c66_Cache__top__
#endif

#ifdef __cplusplus
#define __extern extern "C"
#else
#define __extern extern
#endif

#define ti_sysbios_family_c66_Cache___VERS 150


/*
 * ======== INCLUDES ========
 */

#include <xdc/std.h>

#include <xdc/runtime/xdc.h>
#include <xdc/runtime/Types.h>
#include <ti/sysbios/family/c66/package/package.defs.h>

#include <ti/sysbios/interfaces/ICache.h>


/*
 * ======== AUXILIARY DEFINITIONS ========
 */

/* Type */
typedef ti_sysbios_interfaces_ICache_Type ti_sysbios_family_c66_Cache_Type;

/* Mode */
enum ti_sysbios_family_c66_Cache_Mode {
    ti_sysbios_family_c66_Cache_Mode_FREEZE,
    ti_sysbios_family_c66_Cache_Mode_BYPASS,
    ti_sysbios_family_c66_Cache_Mode_NORMAL
};
typedef enum ti_sysbios_family_c66_Cache_Mode ti_sysbios_family_c66_Cache_Mode;

/* L1Size */
enum ti_sysbios_family_c66_Cache_L1Size {
    ti_sysbios_family_c66_Cache_L1Size_0K = 0,
    ti_sysbios_family_c66_Cache_L1Size_4K = 1,
    ti_sysbios_family_c66_Cache_L1Size_8K = 2,
    ti_sysbios_family_c66_Cache_L1Size_16K = 3,
    ti_sysbios_family_c66_Cache_L1Size_32K = 4
};
typedef enum ti_sysbios_family_c66_Cache_L1Size ti_sysbios_family_c66_Cache_L1Size;

/* L2Size */
enum ti_sysbios_family_c66_Cache_L2Size {
    ti_sysbios_family_c66_Cache_L2Size_0K = 0,
    ti_sysbios_family_c66_Cache_L2Size_32K = 1,
    ti_sysbios_family_c66_Cache_L2Size_64K = 2,
    ti_sysbios_family_c66_Cache_L2Size_128K = 3,
    ti_sysbios_family_c66_Cache_L2Size_256K = 4,
    ti_sysbios_family_c66_Cache_L2Size_512K = 5,
    ti_sysbios_family_c66_Cache_L2Size_1024K = 6
};
typedef enum ti_sysbios_family_c66_Cache_L2Size ti_sysbios_family_c66_Cache_L2Size;

/* Mar */
enum ti_sysbios_family_c66_Cache_Mar {
    ti_sysbios_family_c66_Cache_Mar_DISABLE = 0,
    ti_sysbios_family_c66_Cache_Mar_ENABLE = 1
};
typedef enum ti_sysbios_family_c66_Cache_Mar ti_sysbios_family_c66_Cache_Mar;

/* PC */
#define ti_sysbios_family_c66_Cache_PC (1)

/* WTE */
#define ti_sysbios_family_c66_Cache_WTE (2)

/* PCX */
#define ti_sysbios_family_c66_Cache_PCX (4)

/* PFX */
#define ti_sysbios_family_c66_Cache_PFX (8)

/* Size */
struct ti_sysbios_family_c66_Cache_Size {
    ti_sysbios_family_c66_Cache_L1Size l1pSize;
    ti_sysbios_family_c66_Cache_L1Size l1dSize;
    ti_sysbios_family_c66_Cache_L2Size l2Size;
};

/* Type_L1P */
#define ti_sysbios_family_c66_Cache_Type_L1P ti_sysbios_interfaces_ICache_Type_L1P

/* Type_L1D */
#define ti_sysbios_family_c66_Cache_Type_L1D ti_sysbios_interfaces_ICache_Type_L1D

/* Type_L1 */
#define ti_sysbios_family_c66_Cache_Type_L1 ti_sysbios_interfaces_ICache_Type_L1

/* Type_L2P */
#define ti_sysbios_family_c66_Cache_Type_L2P ti_sysbios_interfaces_ICache_Type_L2P

/* Type_L2D */
#define ti_sysbios_family_c66_Cache_Type_L2D ti_sysbios_interfaces_ICache_Type_L2D

/* Type_L2 */
#define ti_sysbios_family_c66_Cache_Type_L2 ti_sysbios_interfaces_ICache_Type_L2

/* Type_ALLP */
#define ti_sysbios_family_c66_Cache_Type_ALLP ti_sysbios_interfaces_ICache_Type_ALLP

/* Type_ALLD */
#define ti_sysbios_family_c66_Cache_Type_ALLD ti_sysbios_interfaces_ICache_Type_ALLD

/* Type_ALL */
#define ti_sysbios_family_c66_Cache_Type_ALL ti_sysbios_interfaces_ICache_Type_ALL


/*
 * ======== INTERNAL DEFINITIONS ========
 */

/* L2CFG */
#define ti_sysbios_family_c66_Cache_L2CFG (0x01840000)

/* L1PCFG */
#define ti_sysbios_family_c66_Cache_L1PCFG (0x01840020)

/* L1PCC */
#define ti_sysbios_family_c66_Cache_L1PCC (0x01840024)

/* L1DCFG */
#define ti_sysbios_family_c66_Cache_L1DCFG (0x01840040)

/* L1DCC */
#define ti_sysbios_family_c66_Cache_L1DCC (0x01840044)

/* MAR */
#define ti_sysbios_family_c66_Cache_MAR (0x01848000)


/*
 * ======== MODULE-WIDE CONFIGS ========
 */

/* Module__diagsEnabled */
typedef xdc_Bits32 CT__ti_sysbios_family_c66_Cache_Module__diagsEnabled;
__extern __FAR__ const CT__ti_sysbios_family_c66_Cache_Module__diagsEnabled ti_sysbios_family_c66_Cache_Module__diagsEnabled__C;

/* Module__diagsIncluded */
typedef xdc_Bits32 CT__ti_sysbios_family_c66_Cache_Module__diagsIncluded;
__extern __FAR__ const CT__ti_sysbios_family_c66_Cache_Module__diagsIncluded ti_sysbios_family_c66_Cache_Module__diagsIncluded__C;

/* Module__diagsMask */
typedef xdc_Bits16* CT__ti_sysbios_family_c66_Cache_Module__diagsMask;
__extern __FAR__ const CT__ti_sysbios_family_c66_Cache_Module__diagsMask ti_sysbios_family_c66_Cache_Module__diagsMask__C;

/* Module__gateObj */
typedef xdc_Ptr CT__ti_sysbios_family_c66_Cache_Module__gateObj;
__extern __FAR__ const CT__ti_sysbios_family_c66_Cache_Module__gateObj ti_sysbios_family_c66_Cache_Module__gateObj__C;

/* Module__gatePrms */
typedef xdc_Ptr CT__ti_sysbios_family_c66_Cache_Module__gatePrms;
__extern __FAR__ const CT__ti_sysbios_family_c66_Cache_Module__gatePrms ti_sysbios_family_c66_Cache_Module__gatePrms__C;

/* Module__id */
typedef xdc_runtime_Types_ModuleId CT__ti_sysbios_family_c66_Cache_Module__id;
__extern __FAR__ const CT__ti_sysbios_family_c66_Cache_Module__id ti_sysbios_family_c66_Cache_Module__id__C;

/* Module__loggerDefined */
typedef xdc_Bool CT__ti_sysbios_family_c66_Cache_Module__loggerDefined;
__extern __FAR__ const CT__ti_sysbios_family_c66_Cache_Module__loggerDefined ti_sysbios_family_c66_Cache_Module__loggerDefined__C;

/* Module__loggerObj */
typedef xdc_Ptr CT__ti_sysbios_family_c66_Cache_Module__loggerObj;
__extern __FAR__ const CT__ti_sysbios_family_c66_Cache_Module__loggerObj ti_sysbios_family_c66_Cache_Module__loggerObj__C;

/* Module__loggerFxn0 */
typedef xdc_runtime_Types_LoggerFxn0 CT__ti_sysbios_family_c66_Cache_Module__loggerFxn0;
__extern __FAR__ const CT__ti_sysbios_family_c66_Cache_Module__loggerFxn0 ti_sysbios_family_c66_Cache_Module__loggerFxn0__C;

/* Module__loggerFxn1 */
typedef xdc_runtime_Types_LoggerFxn1 CT__ti_sysbios_family_c66_Cache_Module__loggerFxn1;
__extern __FAR__ const CT__ti_sysbios_family_c66_Cache_Module__loggerFxn1 ti_sysbios_family_c66_Cache_Module__loggerFxn1__C;

/* Module__loggerFxn2 */
typedef xdc_runtime_Types_LoggerFxn2 CT__ti_sysbios_family_c66_Cache_Module__loggerFxn2;
__extern __FAR__ const CT__ti_sysbios_family_c66_Cache_Module__loggerFxn2 ti_sysbios_family_c66_Cache_Module__loggerFxn2__C;

/* Module__loggerFxn4 */
typedef xdc_runtime_Types_LoggerFxn4 CT__ti_sysbios_family_c66_Cache_Module__loggerFxn4;
__extern __FAR__ const CT__ti_sysbios_family_c66_Cache_Module__loggerFxn4 ti_sysbios_family_c66_Cache_Module__loggerFxn4__C;

/* Module__loggerFxn8 */
typedef xdc_runtime_Types_LoggerFxn8 CT__ti_sysbios_family_c66_Cache_Module__loggerFxn8;
__extern __FAR__ const CT__ti_sysbios_family_c66_Cache_Module__loggerFxn8 ti_sysbios_family_c66_Cache_Module__loggerFxn8__C;

/* Module__startupDoneFxn */
typedef xdc_Bool (*CT__ti_sysbios_family_c66_Cache_Module__startupDoneFxn)(void);
__extern __FAR__ const CT__ti_sysbios_family_c66_Cache_Module__startupDoneFxn ti_sysbios_family_c66_Cache_Module__startupDoneFxn__C;

/* Object__count */
typedef xdc_Int CT__ti_sysbios_family_c66_Cache_Object__count;
__extern __FAR__ const CT__ti_sysbios_family_c66_Cache_Object__count ti_sysbios_family_c66_Cache_Object__count__C;

/* Object__heap */
typedef xdc_runtime_IHeap_Handle CT__ti_sysbios_family_c66_Cache_Object__heap;
__extern __FAR__ const CT__ti_sysbios_family_c66_Cache_Object__heap ti_sysbios_family_c66_Cache_Object__heap__C;

/* Object__sizeof */
typedef xdc_SizeT CT__ti_sysbios_family_c66_Cache_Object__sizeof;
__extern __FAR__ const CT__ti_sysbios_family_c66_Cache_Object__sizeof ti_sysbios_family_c66_Cache_Object__sizeof__C;

/* Object__table */
typedef xdc_Ptr CT__ti_sysbios_family_c66_Cache_Object__table;
__extern __FAR__ const CT__ti_sysbios_family_c66_Cache_Object__table ti_sysbios_family_c66_Cache_Object__table__C;

/* initSize */
#define ti_sysbios_family_c66_Cache_initSize (ti_sysbios_family_c66_Cache_initSize__C)
typedef ti_sysbios_family_c66_Cache_Size CT__ti_sysbios_family_c66_Cache_initSize;
__extern __FAR__ const CT__ti_sysbios_family_c66_Cache_initSize ti_sysbios_family_c66_Cache_initSize__C;

/* atomicBlockSize */
#ifdef ti_sysbios_family_c66_Cache_atomicBlockSize__D
#define ti_sysbios_family_c66_Cache_atomicBlockSize (ti_sysbios_family_c66_Cache_atomicBlockSize__D)
#else
#define ti_sysbios_family_c66_Cache_atomicBlockSize (ti_sysbios_family_c66_Cache_atomicBlockSize__C)
typedef xdc_UInt32 CT__ti_sysbios_family_c66_Cache_atomicBlockSize;
__extern __FAR__ const CT__ti_sysbios_family_c66_Cache_atomicBlockSize ti_sysbios_family_c66_Cache_atomicBlockSize__C;
#endif

/* marvalues */
#define ti_sysbios_family_c66_Cache_marvalues (ti_sysbios_family_c66_Cache_marvalues__C)
typedef xdc_UInt32 __T1_ti_sysbios_family_c66_Cache_marvalues;
typedef xdc_UInt32 __ARRAY1_ti_sysbios_family_c66_Cache_marvalues[256];
typedef __ARRAY1_ti_sysbios_family_c66_Cache_marvalues __TA_ti_sysbios_family_c66_Cache_marvalues;
typedef __TA_ti_sysbios_family_c66_Cache_marvalues CT__ti_sysbios_family_c66_Cache_marvalues;
__extern __FAR__ const CT__ti_sysbios_family_c66_Cache_marvalues ti_sysbios_family_c66_Cache_marvalues__C;


/*
 * ======== VIRTUAL FUNCTIONS ========
 */

/* Fxns__ */
struct ti_sysbios_family_c66_Cache_Fxns__ {
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
__extern const ti_sysbios_family_c66_Cache_Fxns__ ti_sysbios_family_c66_Cache_Module__FXNS__C;


/*
 * ======== FUNCTION DECLARATIONS ========
 */

/* Module_startup */
#define ti_sysbios_family_c66_Cache_Module_startup ti_sysbios_family_c66_Cache_Module_startup__E
xdc__CODESECT(ti_sysbios_family_c66_Cache_Module_startup__E, "ti_sysbios_family_c66_Cache_Module_startup")
__extern xdc_Int ti_sysbios_family_c66_Cache_Module_startup__E( xdc_Int state );
xdc__CODESECT(ti_sysbios_family_c66_Cache_Module_startup__F, "ti_sysbios_family_c66_Cache_Module_startup")
__extern xdc_Int ti_sysbios_family_c66_Cache_Module_startup__F( xdc_Int state );
xdc__CODESECT(ti_sysbios_family_c66_Cache_Module_startup__R, "ti_sysbios_family_c66_Cache_Module_startup")
__extern xdc_Int ti_sysbios_family_c66_Cache_Module_startup__R( xdc_Int state );

/* Module__startupDone__S */
xdc__CODESECT(ti_sysbios_family_c66_Cache_Module__startupDone__S, "ti_sysbios_family_c66_Cache_Module__startupDone")
__extern xdc_Bool ti_sysbios_family_c66_Cache_Module__startupDone__S( void );

/* enable__E */
#define ti_sysbios_family_c66_Cache_enable ti_sysbios_family_c66_Cache_enable__E
xdc__CODESECT(ti_sysbios_family_c66_Cache_enable__E, "ti_sysbios_family_c66_Cache_enable")
__extern xdc_Void ti_sysbios_family_c66_Cache_enable__E( xdc_Bits16 type );
xdc__CODESECT(ti_sysbios_family_c66_Cache_enable__F, "ti_sysbios_family_c66_Cache_enable")
__extern xdc_Void ti_sysbios_family_c66_Cache_enable__F( xdc_Bits16 type );
__extern xdc_Void ti_sysbios_family_c66_Cache_enable__R( xdc_Bits16 type );

/* inv__E */
#define ti_sysbios_family_c66_Cache_inv ti_sysbios_family_c66_Cache_inv__E
xdc__CODESECT(ti_sysbios_family_c66_Cache_inv__E, "ti_sysbios_family_c66_Cache_inv")
__extern xdc_Void ti_sysbios_family_c66_Cache_inv__E( xdc_Ptr blockPtr, xdc_SizeT byteCnt, xdc_Bits16 type, xdc_Bool wait );
xdc__CODESECT(ti_sysbios_family_c66_Cache_inv__F, "ti_sysbios_family_c66_Cache_inv")
__extern xdc_Void ti_sysbios_family_c66_Cache_inv__F( xdc_Ptr blockPtr, xdc_SizeT byteCnt, xdc_Bits16 type, xdc_Bool wait );
__extern xdc_Void ti_sysbios_family_c66_Cache_inv__R( xdc_Ptr blockPtr, xdc_SizeT byteCnt, xdc_Bits16 type, xdc_Bool wait );

/* wb__E */
#define ti_sysbios_family_c66_Cache_wb ti_sysbios_family_c66_Cache_wb__E
xdc__CODESECT(ti_sysbios_family_c66_Cache_wb__E, "ti_sysbios_family_c66_Cache_wb")
__extern xdc_Void ti_sysbios_family_c66_Cache_wb__E( xdc_Ptr blockPtr, xdc_SizeT byteCnt, xdc_Bits16 type, xdc_Bool wait );
xdc__CODESECT(ti_sysbios_family_c66_Cache_wb__F, "ti_sysbios_family_c66_Cache_wb")
__extern xdc_Void ti_sysbios_family_c66_Cache_wb__F( xdc_Ptr blockPtr, xdc_SizeT byteCnt, xdc_Bits16 type, xdc_Bool wait );
__extern xdc_Void ti_sysbios_family_c66_Cache_wb__R( xdc_Ptr blockPtr, xdc_SizeT byteCnt, xdc_Bits16 type, xdc_Bool wait );

/* wbInv__E */
#define ti_sysbios_family_c66_Cache_wbInv ti_sysbios_family_c66_Cache_wbInv__E
xdc__CODESECT(ti_sysbios_family_c66_Cache_wbInv__E, "ti_sysbios_family_c66_Cache_wbInv")
__extern xdc_Void ti_sysbios_family_c66_Cache_wbInv__E( xdc_Ptr blockPtr, xdc_SizeT byteCnt, xdc_Bits16 type, xdc_Bool wait );
xdc__CODESECT(ti_sysbios_family_c66_Cache_wbInv__F, "ti_sysbios_family_c66_Cache_wbInv")
__extern xdc_Void ti_sysbios_family_c66_Cache_wbInv__F( xdc_Ptr blockPtr, xdc_SizeT byteCnt, xdc_Bits16 type, xdc_Bool wait );
__extern xdc_Void ti_sysbios_family_c66_Cache_wbInv__R( xdc_Ptr blockPtr, xdc_SizeT byteCnt, xdc_Bits16 type, xdc_Bool wait );

/* wait__E */
#define ti_sysbios_family_c66_Cache_wait ti_sysbios_family_c66_Cache_wait__E
xdc__CODESECT(ti_sysbios_family_c66_Cache_wait__E, "ti_sysbios_family_c66_Cache_wait")
__extern xdc_Void ti_sysbios_family_c66_Cache_wait__E( void );
xdc__CODESECT(ti_sysbios_family_c66_Cache_wait__F, "ti_sysbios_family_c66_Cache_wait")
__extern xdc_Void ti_sysbios_family_c66_Cache_wait__F( void );
__extern xdc_Void ti_sysbios_family_c66_Cache_wait__R( void );

/* disable__E */
#define ti_sysbios_family_c66_Cache_disable ti_sysbios_family_c66_Cache_disable__E
xdc__CODESECT(ti_sysbios_family_c66_Cache_disable__E, "ti_sysbios_family_c66_Cache_disable")
__extern xdc_Void ti_sysbios_family_c66_Cache_disable__E( xdc_Bits16 type );
xdc__CODESECT(ti_sysbios_family_c66_Cache_disable__F, "ti_sysbios_family_c66_Cache_disable")
__extern xdc_Void ti_sysbios_family_c66_Cache_disable__F( xdc_Bits16 type );
__extern xdc_Void ti_sysbios_family_c66_Cache_disable__R( xdc_Bits16 type );

/* getMode__E */
#define ti_sysbios_family_c66_Cache_getMode ti_sysbios_family_c66_Cache_getMode__E
xdc__CODESECT(ti_sysbios_family_c66_Cache_getMode__E, "ti_sysbios_family_c66_Cache_getMode")
__extern ti_sysbios_family_c66_Cache_Mode ti_sysbios_family_c66_Cache_getMode__E( xdc_Bits16 type );
xdc__CODESECT(ti_sysbios_family_c66_Cache_getMode__F, "ti_sysbios_family_c66_Cache_getMode")
__extern ti_sysbios_family_c66_Cache_Mode ti_sysbios_family_c66_Cache_getMode__F( xdc_Bits16 type );
__extern ti_sysbios_family_c66_Cache_Mode ti_sysbios_family_c66_Cache_getMode__R( xdc_Bits16 type );

/* setMode__E */
#define ti_sysbios_family_c66_Cache_setMode ti_sysbios_family_c66_Cache_setMode__E
xdc__CODESECT(ti_sysbios_family_c66_Cache_setMode__E, "ti_sysbios_family_c66_Cache_setMode")
__extern ti_sysbios_family_c66_Cache_Mode ti_sysbios_family_c66_Cache_setMode__E( xdc_Bits16 type, ti_sysbios_family_c66_Cache_Mode mode );
xdc__CODESECT(ti_sysbios_family_c66_Cache_setMode__F, "ti_sysbios_family_c66_Cache_setMode")
__extern ti_sysbios_family_c66_Cache_Mode ti_sysbios_family_c66_Cache_setMode__F( xdc_Bits16 type, ti_sysbios_family_c66_Cache_Mode mode );
__extern ti_sysbios_family_c66_Cache_Mode ti_sysbios_family_c66_Cache_setMode__R( xdc_Bits16 type, ti_sysbios_family_c66_Cache_Mode mode );

/* getSize__E */
#define ti_sysbios_family_c66_Cache_getSize ti_sysbios_family_c66_Cache_getSize__E
xdc__CODESECT(ti_sysbios_family_c66_Cache_getSize__E, "ti_sysbios_family_c66_Cache_getSize")
__extern xdc_Void ti_sysbios_family_c66_Cache_getSize__E( ti_sysbios_family_c66_Cache_Size* size );
xdc__CODESECT(ti_sysbios_family_c66_Cache_getSize__F, "ti_sysbios_family_c66_Cache_getSize")
__extern xdc_Void ti_sysbios_family_c66_Cache_getSize__F( ti_sysbios_family_c66_Cache_Size* size );
__extern xdc_Void ti_sysbios_family_c66_Cache_getSize__R( ti_sysbios_family_c66_Cache_Size* size );

/* setSize__E */
#define ti_sysbios_family_c66_Cache_setSize ti_sysbios_family_c66_Cache_setSize__E
xdc__CODESECT(ti_sysbios_family_c66_Cache_setSize__E, "ti_sysbios_family_c66_Cache_setSize")
__extern xdc_Void ti_sysbios_family_c66_Cache_setSize__E( ti_sysbios_family_c66_Cache_Size* size );
xdc__CODESECT(ti_sysbios_family_c66_Cache_setSize__F, "ti_sysbios_family_c66_Cache_setSize")
__extern xdc_Void ti_sysbios_family_c66_Cache_setSize__F( ti_sysbios_family_c66_Cache_Size* size );
__extern xdc_Void ti_sysbios_family_c66_Cache_setSize__R( ti_sysbios_family_c66_Cache_Size* size );

/* getMar__E */
#define ti_sysbios_family_c66_Cache_getMar ti_sysbios_family_c66_Cache_getMar__E
xdc__CODESECT(ti_sysbios_family_c66_Cache_getMar__E, "ti_sysbios_family_c66_Cache_getMar")
__extern xdc_UInt32 ti_sysbios_family_c66_Cache_getMar__E( xdc_Ptr baseAddr );
xdc__CODESECT(ti_sysbios_family_c66_Cache_getMar__F, "ti_sysbios_family_c66_Cache_getMar")
__extern xdc_UInt32 ti_sysbios_family_c66_Cache_getMar__F( xdc_Ptr baseAddr );
__extern xdc_UInt32 ti_sysbios_family_c66_Cache_getMar__R( xdc_Ptr baseAddr );

/* setMar__E */
#define ti_sysbios_family_c66_Cache_setMar ti_sysbios_family_c66_Cache_setMar__E
xdc__CODESECT(ti_sysbios_family_c66_Cache_setMar__E, "ti_sysbios_family_c66_Cache_setMar")
__extern xdc_Void ti_sysbios_family_c66_Cache_setMar__E( xdc_Ptr baseAddr, xdc_SizeT byteSize, xdc_UInt32 value );
xdc__CODESECT(ti_sysbios_family_c66_Cache_setMar__F, "ti_sysbios_family_c66_Cache_setMar")
__extern xdc_Void ti_sysbios_family_c66_Cache_setMar__F( xdc_Ptr baseAddr, xdc_SizeT byteSize, xdc_UInt32 value );
__extern xdc_Void ti_sysbios_family_c66_Cache_setMar__R( xdc_Ptr baseAddr, xdc_SizeT byteSize, xdc_UInt32 value );

/* invL1pAll__E */
#define ti_sysbios_family_c66_Cache_invL1pAll ti_sysbios_family_c66_Cache_invL1pAll__E
xdc__CODESECT(ti_sysbios_family_c66_Cache_invL1pAll__E, "ti_sysbios_family_c66_Cache_invL1pAll")
__extern xdc_Void ti_sysbios_family_c66_Cache_invL1pAll__E( void );
xdc__CODESECT(ti_sysbios_family_c66_Cache_invL1pAll__F, "ti_sysbios_family_c66_Cache_invL1pAll")
__extern xdc_Void ti_sysbios_family_c66_Cache_invL1pAll__F( void );
__extern xdc_Void ti_sysbios_family_c66_Cache_invL1pAll__R( void );

/* wbAll__E */
#define ti_sysbios_family_c66_Cache_wbAll ti_sysbios_family_c66_Cache_wbAll__E
xdc__CODESECT(ti_sysbios_family_c66_Cache_wbAll__E, "ti_sysbios_family_c66_Cache_wbAll")
__extern xdc_Void ti_sysbios_family_c66_Cache_wbAll__E( void );
xdc__CODESECT(ti_sysbios_family_c66_Cache_wbAll__F, "ti_sysbios_family_c66_Cache_wbAll")
__extern xdc_Void ti_sysbios_family_c66_Cache_wbAll__F( void );
__extern xdc_Void ti_sysbios_family_c66_Cache_wbAll__R( void );

/* wbInvAll__E */
#define ti_sysbios_family_c66_Cache_wbInvAll ti_sysbios_family_c66_Cache_wbInvAll__E
xdc__CODESECT(ti_sysbios_family_c66_Cache_wbInvAll__E, "ti_sysbios_family_c66_Cache_wbInvAll")
__extern xdc_Void ti_sysbios_family_c66_Cache_wbInvAll__E( void );
xdc__CODESECT(ti_sysbios_family_c66_Cache_wbInvAll__F, "ti_sysbios_family_c66_Cache_wbInvAll")
__extern xdc_Void ti_sysbios_family_c66_Cache_wbInvAll__F( void );
__extern xdc_Void ti_sysbios_family_c66_Cache_wbInvAll__R( void );

/* invPrefetchBuffer__I */
#define ti_sysbios_family_c66_Cache_invPrefetchBuffer ti_sysbios_family_c66_Cache_invPrefetchBuffer__I
xdc__CODESECT(ti_sysbios_family_c66_Cache_invPrefetchBuffer__I, "ti_sysbios_family_c66_Cache_invPrefetchBuffer")
__extern xdc_Void ti_sysbios_family_c66_Cache_invPrefetchBuffer__I( void );

/* all__I */
#define ti_sysbios_family_c66_Cache_all ti_sysbios_family_c66_Cache_all__I
xdc__CODESECT(ti_sysbios_family_c66_Cache_all__I, "ti_sysbios_family_c66_Cache_all")
__extern xdc_Void ti_sysbios_family_c66_Cache_all__I( volatile xdc_UInt32* cacheReg );

/* block__I */
#define ti_sysbios_family_c66_Cache_block ti_sysbios_family_c66_Cache_block__I
xdc__CODESECT(ti_sysbios_family_c66_Cache_block__I, "ti_sysbios_family_c66_Cache_block")
__extern xdc_Void ti_sysbios_family_c66_Cache_block__I( xdc_Ptr blockPtr, xdc_SizeT byteCnt, xdc_Bool wait, volatile xdc_UInt32* barReg );


/*
 * ======== CONVERTORS ========
 */

/* Module_upCast */
static inline ti_sysbios_interfaces_ICache_Module ti_sysbios_family_c66_Cache_Module_upCast( void )
{
    return (ti_sysbios_interfaces_ICache_Module)&ti_sysbios_family_c66_Cache_Module__FXNS__C;
}

/* Module_to_ti_sysbios_interfaces_ICache */
#define ti_sysbios_family_c66_Cache_Module_to_ti_sysbios_interfaces_ICache ti_sysbios_family_c66_Cache_Module_upCast


/*
 * ======== SYSTEM FUNCTIONS ========
 */

/* Module_startupDone */
#define ti_sysbios_family_c66_Cache_Module_startupDone() ti_sysbios_family_c66_Cache_Module__startupDone__S()

/* Object_heap */
#define ti_sysbios_family_c66_Cache_Object_heap() ti_sysbios_family_c66_Cache_Object__heap__C

/* Module_heap */
#define ti_sysbios_family_c66_Cache_Module_heap() ti_sysbios_family_c66_Cache_Object__heap__C

/* Module_id */
static inline CT__ti_sysbios_family_c66_Cache_Module__id ti_sysbios_family_c66_Cache_Module_id( void ) 
{
    return ti_sysbios_family_c66_Cache_Module__id__C;
}

/* Module_hasMask */
static inline xdc_Bool ti_sysbios_family_c66_Cache_Module_hasMask( void ) 
{
    return ti_sysbios_family_c66_Cache_Module__diagsMask__C != NULL;
}

/* Module_getMask */
static inline xdc_Bits16 ti_sysbios_family_c66_Cache_Module_getMask( void ) 
{
    return ti_sysbios_family_c66_Cache_Module__diagsMask__C != NULL ? *ti_sysbios_family_c66_Cache_Module__diagsMask__C : 0;
}

/* Module_setMask */
static inline xdc_Void ti_sysbios_family_c66_Cache_Module_setMask( xdc_Bits16 mask ) 
{
    if (ti_sysbios_family_c66_Cache_Module__diagsMask__C != NULL) *ti_sysbios_family_c66_Cache_Module__diagsMask__C = mask;
}


/*
 * ======== EPILOGUE ========
 */

#ifdef ti_sysbios_family_c66_Cache__top__
#undef __nested__
#endif

#endif /* ti_sysbios_family_c66_Cache__include */


/*
 * ======== STATE STRUCTURES ========
 */

#if defined(__config__) || (!defined(__nested__) && defined(ti_sysbios_family_c66_Cache__internalaccess))

#ifndef ti_sysbios_family_c66_Cache__include_state
#define ti_sysbios_family_c66_Cache__include_state


#endif /* ti_sysbios_family_c66_Cache__include_state */

#endif

/*
 * ======== PREFIX ALIASES ========
 */

#if !defined(__nested__) && !defined(ti_sysbios_family_c66_Cache__nolocalnames)

#ifndef ti_sysbios_family_c66_Cache__localnames__done
#define ti_sysbios_family_c66_Cache__localnames__done

/* module prefix */
#define Cache_Type ti_sysbios_family_c66_Cache_Type
#define Cache_Mode ti_sysbios_family_c66_Cache_Mode
#define Cache_L1Size ti_sysbios_family_c66_Cache_L1Size
#define Cache_L2Size ti_sysbios_family_c66_Cache_L2Size
#define Cache_Mar ti_sysbios_family_c66_Cache_Mar
#define Cache_PC ti_sysbios_family_c66_Cache_PC
#define Cache_WTE ti_sysbios_family_c66_Cache_WTE
#define Cache_PCX ti_sysbios_family_c66_Cache_PCX
#define Cache_PFX ti_sysbios_family_c66_Cache_PFX
#define Cache_Size ti_sysbios_family_c66_Cache_Size
#define Cache_L2CFG ti_sysbios_family_c66_Cache_L2CFG
#define Cache_L1PCFG ti_sysbios_family_c66_Cache_L1PCFG
#define Cache_L1PCC ti_sysbios_family_c66_Cache_L1PCC
#define Cache_L1DCFG ti_sysbios_family_c66_Cache_L1DCFG
#define Cache_L1DCC ti_sysbios_family_c66_Cache_L1DCC
#define Cache_MAR ti_sysbios_family_c66_Cache_MAR
#define Cache_Type_L1P ti_sysbios_family_c66_Cache_Type_L1P
#define Cache_Type_L1D ti_sysbios_family_c66_Cache_Type_L1D
#define Cache_Type_L1 ti_sysbios_family_c66_Cache_Type_L1
#define Cache_Type_L2P ti_sysbios_family_c66_Cache_Type_L2P
#define Cache_Type_L2D ti_sysbios_family_c66_Cache_Type_L2D
#define Cache_Type_L2 ti_sysbios_family_c66_Cache_Type_L2
#define Cache_Type_ALLP ti_sysbios_family_c66_Cache_Type_ALLP
#define Cache_Type_ALLD ti_sysbios_family_c66_Cache_Type_ALLD
#define Cache_Type_ALL ti_sysbios_family_c66_Cache_Type_ALL
#define Cache_Mode_FREEZE ti_sysbios_family_c66_Cache_Mode_FREEZE
#define Cache_Mode_BYPASS ti_sysbios_family_c66_Cache_Mode_BYPASS
#define Cache_Mode_NORMAL ti_sysbios_family_c66_Cache_Mode_NORMAL
#define Cache_L1Size_0K ti_sysbios_family_c66_Cache_L1Size_0K
#define Cache_L1Size_4K ti_sysbios_family_c66_Cache_L1Size_4K
#define Cache_L1Size_8K ti_sysbios_family_c66_Cache_L1Size_8K
#define Cache_L1Size_16K ti_sysbios_family_c66_Cache_L1Size_16K
#define Cache_L1Size_32K ti_sysbios_family_c66_Cache_L1Size_32K
#define Cache_L2Size_0K ti_sysbios_family_c66_Cache_L2Size_0K
#define Cache_L2Size_32K ti_sysbios_family_c66_Cache_L2Size_32K
#define Cache_L2Size_64K ti_sysbios_family_c66_Cache_L2Size_64K
#define Cache_L2Size_128K ti_sysbios_family_c66_Cache_L2Size_128K
#define Cache_L2Size_256K ti_sysbios_family_c66_Cache_L2Size_256K
#define Cache_L2Size_512K ti_sysbios_family_c66_Cache_L2Size_512K
#define Cache_L2Size_1024K ti_sysbios_family_c66_Cache_L2Size_1024K
#define Cache_Mar_DISABLE ti_sysbios_family_c66_Cache_Mar_DISABLE
#define Cache_Mar_ENABLE ti_sysbios_family_c66_Cache_Mar_ENABLE
#define Cache_initSize ti_sysbios_family_c66_Cache_initSize
#define Cache_atomicBlockSize ti_sysbios_family_c66_Cache_atomicBlockSize
#define Cache_marvalues ti_sysbios_family_c66_Cache_marvalues
#define Cache_enable ti_sysbios_family_c66_Cache_enable
#define Cache_inv ti_sysbios_family_c66_Cache_inv
#define Cache_wb ti_sysbios_family_c66_Cache_wb
#define Cache_wbInv ti_sysbios_family_c66_Cache_wbInv
#define Cache_wait ti_sysbios_family_c66_Cache_wait
#define Cache_disable ti_sysbios_family_c66_Cache_disable
#define Cache_getMode ti_sysbios_family_c66_Cache_getMode
#define Cache_setMode ti_sysbios_family_c66_Cache_setMode
#define Cache_getSize ti_sysbios_family_c66_Cache_getSize
#define Cache_setSize ti_sysbios_family_c66_Cache_setSize
#define Cache_getMar ti_sysbios_family_c66_Cache_getMar
#define Cache_setMar ti_sysbios_family_c66_Cache_setMar
#define Cache_invL1pAll ti_sysbios_family_c66_Cache_invL1pAll
#define Cache_wbAll ti_sysbios_family_c66_Cache_wbAll
#define Cache_wbInvAll ti_sysbios_family_c66_Cache_wbInvAll
#define Cache_Module_name ti_sysbios_family_c66_Cache_Module_name
#define Cache_Module_id ti_sysbios_family_c66_Cache_Module_id
#define Cache_Module_startup ti_sysbios_family_c66_Cache_Module_startup
#define Cache_Module_startupDone ti_sysbios_family_c66_Cache_Module_startupDone
#define Cache_Module_hasMask ti_sysbios_family_c66_Cache_Module_hasMask
#define Cache_Module_getMask ti_sysbios_family_c66_Cache_Module_getMask
#define Cache_Module_setMask ti_sysbios_family_c66_Cache_Module_setMask
#define Cache_Object_heap ti_sysbios_family_c66_Cache_Object_heap
#define Cache_Module_heap ti_sysbios_family_c66_Cache_Module_heap
#define Cache_Module_upCast ti_sysbios_family_c66_Cache_Module_upCast
#define Cache_Module_to_ti_sysbios_interfaces_ICache ti_sysbios_family_c66_Cache_Module_to_ti_sysbios_interfaces_ICache

#endif /* ti_sysbios_family_c66_Cache__localnames__done */
#endif
