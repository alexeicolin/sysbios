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

#ifndef ti_sysbios_family_arm_a9_Mmu__include
#define ti_sysbios_family_arm_a9_Mmu__include

#ifndef __nested__
#define __nested__
#define ti_sysbios_family_arm_a9_Mmu__top__
#endif

#ifdef __cplusplus
#define __extern extern "C"
#else
#define __extern extern
#endif

#define ti_sysbios_family_arm_a9_Mmu___VERS 150


/*
 * ======== INCLUDES ========
 */

#include <xdc/std.h>

#include <xdc/runtime/xdc.h>
#include <xdc/runtime/Types.h>
#include <ti/sysbios/family/arm/a9/package/package.defs.h>

#include <xdc/runtime/IModule.h>
#include <xdc/runtime/Assert.h>


/*
 * ======== AUXILIARY DEFINITIONS ========
 */

/* FirstLevelDesc */
enum ti_sysbios_family_arm_a9_Mmu_FirstLevelDesc {
    ti_sysbios_family_arm_a9_Mmu_FirstLevelDesc_FAULT = 0,
    ti_sysbios_family_arm_a9_Mmu_FirstLevelDesc_PAGE_TABLE = 1,
    ti_sysbios_family_arm_a9_Mmu_FirstLevelDesc_SECTION = 2,
    ti_sysbios_family_arm_a9_Mmu_FirstLevelDesc_SECTION_PXN = 3
};
typedef enum ti_sysbios_family_arm_a9_Mmu_FirstLevelDesc ti_sysbios_family_arm_a9_Mmu_FirstLevelDesc;

/* FirstLevelDescAttrs */
struct ti_sysbios_family_arm_a9_Mmu_FirstLevelDescAttrs {
    ti_sysbios_family_arm_a9_Mmu_FirstLevelDesc type;
    xdc_UInt8 tex;
    xdc_Bool bufferable;
    xdc_Bool cacheable;
    xdc_Bool shareable;
    xdc_Bool noexecute;
    xdc_UInt8 imp;
    xdc_UInt8 domain;
    xdc_UInt8 accPerm;
};


/*
 * ======== INTERNAL DEFINITIONS ========
 */

/* Module_State */
typedef xdc_UInt32 __T1_ti_sysbios_family_arm_a9_Mmu_Module_State__tableBuf;
typedef xdc_UInt32 *__ARRAY1_ti_sysbios_family_arm_a9_Mmu_Module_State__tableBuf;
typedef __ARRAY1_ti_sysbios_family_arm_a9_Mmu_Module_State__tableBuf __TA_ti_sysbios_family_arm_a9_Mmu_Module_State__tableBuf;


/*
 * ======== MODULE-WIDE CONFIGS ========
 */

/* Module__diagsEnabled */
typedef xdc_Bits32 CT__ti_sysbios_family_arm_a9_Mmu_Module__diagsEnabled;
__extern __FAR__ const CT__ti_sysbios_family_arm_a9_Mmu_Module__diagsEnabled ti_sysbios_family_arm_a9_Mmu_Module__diagsEnabled__C;

/* Module__diagsIncluded */
typedef xdc_Bits32 CT__ti_sysbios_family_arm_a9_Mmu_Module__diagsIncluded;
__extern __FAR__ const CT__ti_sysbios_family_arm_a9_Mmu_Module__diagsIncluded ti_sysbios_family_arm_a9_Mmu_Module__diagsIncluded__C;

/* Module__diagsMask */
typedef xdc_Bits16* CT__ti_sysbios_family_arm_a9_Mmu_Module__diagsMask;
__extern __FAR__ const CT__ti_sysbios_family_arm_a9_Mmu_Module__diagsMask ti_sysbios_family_arm_a9_Mmu_Module__diagsMask__C;

/* Module__gateObj */
typedef xdc_Ptr CT__ti_sysbios_family_arm_a9_Mmu_Module__gateObj;
__extern __FAR__ const CT__ti_sysbios_family_arm_a9_Mmu_Module__gateObj ti_sysbios_family_arm_a9_Mmu_Module__gateObj__C;

/* Module__gatePrms */
typedef xdc_Ptr CT__ti_sysbios_family_arm_a9_Mmu_Module__gatePrms;
__extern __FAR__ const CT__ti_sysbios_family_arm_a9_Mmu_Module__gatePrms ti_sysbios_family_arm_a9_Mmu_Module__gatePrms__C;

/* Module__id */
typedef xdc_runtime_Types_ModuleId CT__ti_sysbios_family_arm_a9_Mmu_Module__id;
__extern __FAR__ const CT__ti_sysbios_family_arm_a9_Mmu_Module__id ti_sysbios_family_arm_a9_Mmu_Module__id__C;

/* Module__loggerDefined */
typedef xdc_Bool CT__ti_sysbios_family_arm_a9_Mmu_Module__loggerDefined;
__extern __FAR__ const CT__ti_sysbios_family_arm_a9_Mmu_Module__loggerDefined ti_sysbios_family_arm_a9_Mmu_Module__loggerDefined__C;

/* Module__loggerObj */
typedef xdc_Ptr CT__ti_sysbios_family_arm_a9_Mmu_Module__loggerObj;
__extern __FAR__ const CT__ti_sysbios_family_arm_a9_Mmu_Module__loggerObj ti_sysbios_family_arm_a9_Mmu_Module__loggerObj__C;

/* Module__loggerFxn0 */
typedef xdc_runtime_Types_LoggerFxn0 CT__ti_sysbios_family_arm_a9_Mmu_Module__loggerFxn0;
__extern __FAR__ const CT__ti_sysbios_family_arm_a9_Mmu_Module__loggerFxn0 ti_sysbios_family_arm_a9_Mmu_Module__loggerFxn0__C;

/* Module__loggerFxn1 */
typedef xdc_runtime_Types_LoggerFxn1 CT__ti_sysbios_family_arm_a9_Mmu_Module__loggerFxn1;
__extern __FAR__ const CT__ti_sysbios_family_arm_a9_Mmu_Module__loggerFxn1 ti_sysbios_family_arm_a9_Mmu_Module__loggerFxn1__C;

/* Module__loggerFxn2 */
typedef xdc_runtime_Types_LoggerFxn2 CT__ti_sysbios_family_arm_a9_Mmu_Module__loggerFxn2;
__extern __FAR__ const CT__ti_sysbios_family_arm_a9_Mmu_Module__loggerFxn2 ti_sysbios_family_arm_a9_Mmu_Module__loggerFxn2__C;

/* Module__loggerFxn4 */
typedef xdc_runtime_Types_LoggerFxn4 CT__ti_sysbios_family_arm_a9_Mmu_Module__loggerFxn4;
__extern __FAR__ const CT__ti_sysbios_family_arm_a9_Mmu_Module__loggerFxn4 ti_sysbios_family_arm_a9_Mmu_Module__loggerFxn4__C;

/* Module__loggerFxn8 */
typedef xdc_runtime_Types_LoggerFxn8 CT__ti_sysbios_family_arm_a9_Mmu_Module__loggerFxn8;
__extern __FAR__ const CT__ti_sysbios_family_arm_a9_Mmu_Module__loggerFxn8 ti_sysbios_family_arm_a9_Mmu_Module__loggerFxn8__C;

/* Module__startupDoneFxn */
typedef xdc_Bool (*CT__ti_sysbios_family_arm_a9_Mmu_Module__startupDoneFxn)(void);
__extern __FAR__ const CT__ti_sysbios_family_arm_a9_Mmu_Module__startupDoneFxn ti_sysbios_family_arm_a9_Mmu_Module__startupDoneFxn__C;

/* Object__count */
typedef xdc_Int CT__ti_sysbios_family_arm_a9_Mmu_Object__count;
__extern __FAR__ const CT__ti_sysbios_family_arm_a9_Mmu_Object__count ti_sysbios_family_arm_a9_Mmu_Object__count__C;

/* Object__heap */
typedef xdc_runtime_IHeap_Handle CT__ti_sysbios_family_arm_a9_Mmu_Object__heap;
__extern __FAR__ const CT__ti_sysbios_family_arm_a9_Mmu_Object__heap ti_sysbios_family_arm_a9_Mmu_Object__heap__C;

/* Object__sizeof */
typedef xdc_SizeT CT__ti_sysbios_family_arm_a9_Mmu_Object__sizeof;
__extern __FAR__ const CT__ti_sysbios_family_arm_a9_Mmu_Object__sizeof ti_sysbios_family_arm_a9_Mmu_Object__sizeof__C;

/* Object__table */
typedef xdc_Ptr CT__ti_sysbios_family_arm_a9_Mmu_Object__table;
__extern __FAR__ const CT__ti_sysbios_family_arm_a9_Mmu_Object__table ti_sysbios_family_arm_a9_Mmu_Object__table__C;

/* A_nullPointer */
#define ti_sysbios_family_arm_a9_Mmu_A_nullPointer (ti_sysbios_family_arm_a9_Mmu_A_nullPointer__C)
typedef xdc_runtime_Assert_Id CT__ti_sysbios_family_arm_a9_Mmu_A_nullPointer;
__extern __FAR__ const CT__ti_sysbios_family_arm_a9_Mmu_A_nullPointer ti_sysbios_family_arm_a9_Mmu_A_nullPointer__C;

/* A_unknownDescType */
#define ti_sysbios_family_arm_a9_Mmu_A_unknownDescType (ti_sysbios_family_arm_a9_Mmu_A_unknownDescType__C)
typedef xdc_runtime_Assert_Id CT__ti_sysbios_family_arm_a9_Mmu_A_unknownDescType;
__extern __FAR__ const CT__ti_sysbios_family_arm_a9_Mmu_A_unknownDescType ti_sysbios_family_arm_a9_Mmu_A_unknownDescType__C;

/* defaultAttrs */
#define ti_sysbios_family_arm_a9_Mmu_defaultAttrs (ti_sysbios_family_arm_a9_Mmu_defaultAttrs__C)
typedef ti_sysbios_family_arm_a9_Mmu_FirstLevelDescAttrs CT__ti_sysbios_family_arm_a9_Mmu_defaultAttrs;
__extern __FAR__ const CT__ti_sysbios_family_arm_a9_Mmu_defaultAttrs ti_sysbios_family_arm_a9_Mmu_defaultAttrs__C;

/* enableMMU */
#ifdef ti_sysbios_family_arm_a9_Mmu_enableMMU__D
#define ti_sysbios_family_arm_a9_Mmu_enableMMU (ti_sysbios_family_arm_a9_Mmu_enableMMU__D)
#else
#define ti_sysbios_family_arm_a9_Mmu_enableMMU (ti_sysbios_family_arm_a9_Mmu_enableMMU__C)
typedef xdc_Bool CT__ti_sysbios_family_arm_a9_Mmu_enableMMU;
__extern __FAR__ const CT__ti_sysbios_family_arm_a9_Mmu_enableMMU ti_sysbios_family_arm_a9_Mmu_enableMMU__C;
#endif


/*
 * ======== FUNCTION DECLARATIONS ========
 */

/* Module_startup */
#define ti_sysbios_family_arm_a9_Mmu_Module_startup ti_sysbios_family_arm_a9_Mmu_Module_startup__E
xdc__CODESECT(ti_sysbios_family_arm_a9_Mmu_Module_startup__E, "ti_sysbios_family_arm_a9_Mmu_Module_startup")
__extern xdc_Int ti_sysbios_family_arm_a9_Mmu_Module_startup__E( xdc_Int state );
xdc__CODESECT(ti_sysbios_family_arm_a9_Mmu_Module_startup__F, "ti_sysbios_family_arm_a9_Mmu_Module_startup")
__extern xdc_Int ti_sysbios_family_arm_a9_Mmu_Module_startup__F( xdc_Int state );
xdc__CODESECT(ti_sysbios_family_arm_a9_Mmu_Module_startup__R, "ti_sysbios_family_arm_a9_Mmu_Module_startup")
__extern xdc_Int ti_sysbios_family_arm_a9_Mmu_Module_startup__R( xdc_Int state );

/* Module__startupDone__S */
xdc__CODESECT(ti_sysbios_family_arm_a9_Mmu_Module__startupDone__S, "ti_sysbios_family_arm_a9_Mmu_Module__startupDone")
__extern xdc_Bool ti_sysbios_family_arm_a9_Mmu_Module__startupDone__S( void );

/* disable__E */
#define ti_sysbios_family_arm_a9_Mmu_disable ti_sysbios_family_arm_a9_Mmu_disable__E
xdc__CODESECT(ti_sysbios_family_arm_a9_Mmu_disable__E, "ti_sysbios_family_arm_a9_Mmu_disable")
__extern xdc_Void ti_sysbios_family_arm_a9_Mmu_disable__E( void );
xdc__CODESECT(ti_sysbios_family_arm_a9_Mmu_disable__F, "ti_sysbios_family_arm_a9_Mmu_disable")
__extern xdc_Void ti_sysbios_family_arm_a9_Mmu_disable__F( void );
__extern xdc_Void ti_sysbios_family_arm_a9_Mmu_disable__R( void );

/* enable__E */
#define ti_sysbios_family_arm_a9_Mmu_enable ti_sysbios_family_arm_a9_Mmu_enable__E
xdc__CODESECT(ti_sysbios_family_arm_a9_Mmu_enable__E, "ti_sysbios_family_arm_a9_Mmu_enable")
__extern xdc_Void ti_sysbios_family_arm_a9_Mmu_enable__E( void );
xdc__CODESECT(ti_sysbios_family_arm_a9_Mmu_enable__F, "ti_sysbios_family_arm_a9_Mmu_enable")
__extern xdc_Void ti_sysbios_family_arm_a9_Mmu_enable__F( void );
__extern xdc_Void ti_sysbios_family_arm_a9_Mmu_enable__R( void );

/* initDescAttrs__E */
#define ti_sysbios_family_arm_a9_Mmu_initDescAttrs ti_sysbios_family_arm_a9_Mmu_initDescAttrs__E
xdc__CODESECT(ti_sysbios_family_arm_a9_Mmu_initDescAttrs__E, "ti_sysbios_family_arm_a9_Mmu_initDescAttrs")
__extern xdc_Void ti_sysbios_family_arm_a9_Mmu_initDescAttrs__E( ti_sysbios_family_arm_a9_Mmu_FirstLevelDescAttrs* attrs );
xdc__CODESECT(ti_sysbios_family_arm_a9_Mmu_initDescAttrs__F, "ti_sysbios_family_arm_a9_Mmu_initDescAttrs")
__extern xdc_Void ti_sysbios_family_arm_a9_Mmu_initDescAttrs__F( ti_sysbios_family_arm_a9_Mmu_FirstLevelDescAttrs* attrs );
__extern xdc_Void ti_sysbios_family_arm_a9_Mmu_initDescAttrs__R( ti_sysbios_family_arm_a9_Mmu_FirstLevelDescAttrs* attrs );

/* isEnabled__E */
#define ti_sysbios_family_arm_a9_Mmu_isEnabled ti_sysbios_family_arm_a9_Mmu_isEnabled__E
xdc__CODESECT(ti_sysbios_family_arm_a9_Mmu_isEnabled__E, "ti_sysbios_family_arm_a9_Mmu_isEnabled")
__extern xdc_Bool ti_sysbios_family_arm_a9_Mmu_isEnabled__E( void );
xdc__CODESECT(ti_sysbios_family_arm_a9_Mmu_isEnabled__F, "ti_sysbios_family_arm_a9_Mmu_isEnabled")
__extern xdc_Bool ti_sysbios_family_arm_a9_Mmu_isEnabled__F( void );
__extern xdc_Bool ti_sysbios_family_arm_a9_Mmu_isEnabled__R( void );

/* setFirstLevelDesc__E */
#define ti_sysbios_family_arm_a9_Mmu_setFirstLevelDesc ti_sysbios_family_arm_a9_Mmu_setFirstLevelDesc__E
xdc__CODESECT(ti_sysbios_family_arm_a9_Mmu_setFirstLevelDesc__E, "ti_sysbios_family_arm_a9_Mmu_setFirstLevelDesc")
__extern xdc_Void ti_sysbios_family_arm_a9_Mmu_setFirstLevelDesc__E( xdc_Ptr virtualAddr, xdc_Ptr phyAddr, ti_sysbios_family_arm_a9_Mmu_FirstLevelDescAttrs* attrs );
xdc__CODESECT(ti_sysbios_family_arm_a9_Mmu_setFirstLevelDesc__F, "ti_sysbios_family_arm_a9_Mmu_setFirstLevelDesc")
__extern xdc_Void ti_sysbios_family_arm_a9_Mmu_setFirstLevelDesc__F( xdc_Ptr virtualAddr, xdc_Ptr phyAddr, ti_sysbios_family_arm_a9_Mmu_FirstLevelDescAttrs* attrs );
__extern xdc_Void ti_sysbios_family_arm_a9_Mmu_setFirstLevelDesc__R( xdc_Ptr virtualAddr, xdc_Ptr phyAddr, ti_sysbios_family_arm_a9_Mmu_FirstLevelDescAttrs* attrs );

/* init__I */
#define ti_sysbios_family_arm_a9_Mmu_init ti_sysbios_family_arm_a9_Mmu_init__I
xdc__CODESECT(ti_sysbios_family_arm_a9_Mmu_init__I, "ti_sysbios_family_arm_a9_Mmu_init")
__extern xdc_Void ti_sysbios_family_arm_a9_Mmu_init__I( void );

/* enableAsm__I */
#define ti_sysbios_family_arm_a9_Mmu_enableAsm ti_sysbios_family_arm_a9_Mmu_enableAsm__I
xdc__CODESECT(ti_sysbios_family_arm_a9_Mmu_enableAsm__I, "ti_sysbios_family_arm_a9_Mmu_enableAsm")
__extern xdc_Void ti_sysbios_family_arm_a9_Mmu_enableAsm__I( void );

/* disableAsm__I */
#define ti_sysbios_family_arm_a9_Mmu_disableAsm ti_sysbios_family_arm_a9_Mmu_disableAsm__I
xdc__CODESECT(ti_sysbios_family_arm_a9_Mmu_disableAsm__I, "ti_sysbios_family_arm_a9_Mmu_disableAsm")
__extern xdc_Void ti_sysbios_family_arm_a9_Mmu_disableAsm__I( void );

/* initTableBuf__I */
#define ti_sysbios_family_arm_a9_Mmu_initTableBuf ti_sysbios_family_arm_a9_Mmu_initTableBuf__I
xdc__CODESECT(ti_sysbios_family_arm_a9_Mmu_initTableBuf__I, "ti_sysbios_family_arm_a9_Mmu_initTableBuf")
__extern xdc_Void ti_sysbios_family_arm_a9_Mmu_initTableBuf__I( xdc_UInt32* mmuTableBuf );


/*
 * ======== SYSTEM FUNCTIONS ========
 */

/* Module_startupDone */
#define ti_sysbios_family_arm_a9_Mmu_Module_startupDone() ti_sysbios_family_arm_a9_Mmu_Module__startupDone__S()

/* Object_heap */
#define ti_sysbios_family_arm_a9_Mmu_Object_heap() ti_sysbios_family_arm_a9_Mmu_Object__heap__C

/* Module_heap */
#define ti_sysbios_family_arm_a9_Mmu_Module_heap() ti_sysbios_family_arm_a9_Mmu_Object__heap__C

/* Module_id */
static inline CT__ti_sysbios_family_arm_a9_Mmu_Module__id ti_sysbios_family_arm_a9_Mmu_Module_id( void ) 
{
    return ti_sysbios_family_arm_a9_Mmu_Module__id__C;
}

/* Module_hasMask */
static inline xdc_Bool ti_sysbios_family_arm_a9_Mmu_Module_hasMask( void ) 
{
    return ti_sysbios_family_arm_a9_Mmu_Module__diagsMask__C != NULL;
}

/* Module_getMask */
static inline xdc_Bits16 ti_sysbios_family_arm_a9_Mmu_Module_getMask( void ) 
{
    return ti_sysbios_family_arm_a9_Mmu_Module__diagsMask__C != NULL ? *ti_sysbios_family_arm_a9_Mmu_Module__diagsMask__C : 0;
}

/* Module_setMask */
static inline xdc_Void ti_sysbios_family_arm_a9_Mmu_Module_setMask( xdc_Bits16 mask ) 
{
    if (ti_sysbios_family_arm_a9_Mmu_Module__diagsMask__C != NULL) *ti_sysbios_family_arm_a9_Mmu_Module__diagsMask__C = mask;
}


/*
 * ======== EPILOGUE ========
 */

#ifdef ti_sysbios_family_arm_a9_Mmu__top__
#undef __nested__
#endif

#endif /* ti_sysbios_family_arm_a9_Mmu__include */


/*
 * ======== STATE STRUCTURES ========
 */

#if defined(__config__) || (!defined(__nested__) && defined(ti_sysbios_family_arm_a9_Mmu__internalaccess))

#ifndef ti_sysbios_family_arm_a9_Mmu__include_state
#define ti_sysbios_family_arm_a9_Mmu__include_state

/* Module_State */
struct ti_sysbios_family_arm_a9_Mmu_Module_State {
    __TA_ti_sysbios_family_arm_a9_Mmu_Module_State__tableBuf tableBuf;
};

/* Module__state__V */
extern struct ti_sysbios_family_arm_a9_Mmu_Module_State__ ti_sysbios_family_arm_a9_Mmu_Module__state__V;

#endif /* ti_sysbios_family_arm_a9_Mmu__include_state */

#endif


/*
 * ======== PREFIX ALIASES ========
 */

#if !defined(__nested__) && !defined(ti_sysbios_family_arm_a9_Mmu__nolocalnames)

#ifndef ti_sysbios_family_arm_a9_Mmu__localnames__done
#define ti_sysbios_family_arm_a9_Mmu__localnames__done

/* module prefix */
#define Mmu_FirstLevelDesc ti_sysbios_family_arm_a9_Mmu_FirstLevelDesc
#define Mmu_FirstLevelDescAttrs ti_sysbios_family_arm_a9_Mmu_FirstLevelDescAttrs
#define Mmu_Module_State ti_sysbios_family_arm_a9_Mmu_Module_State
#define Mmu_FirstLevelDesc_FAULT ti_sysbios_family_arm_a9_Mmu_FirstLevelDesc_FAULT
#define Mmu_FirstLevelDesc_PAGE_TABLE ti_sysbios_family_arm_a9_Mmu_FirstLevelDesc_PAGE_TABLE
#define Mmu_FirstLevelDesc_SECTION ti_sysbios_family_arm_a9_Mmu_FirstLevelDesc_SECTION
#define Mmu_FirstLevelDesc_SECTION_PXN ti_sysbios_family_arm_a9_Mmu_FirstLevelDesc_SECTION_PXN
#define Mmu_A_nullPointer ti_sysbios_family_arm_a9_Mmu_A_nullPointer
#define Mmu_A_unknownDescType ti_sysbios_family_arm_a9_Mmu_A_unknownDescType
#define Mmu_defaultAttrs ti_sysbios_family_arm_a9_Mmu_defaultAttrs
#define Mmu_enableMMU ti_sysbios_family_arm_a9_Mmu_enableMMU
#define Mmu_disable ti_sysbios_family_arm_a9_Mmu_disable
#define Mmu_enable ti_sysbios_family_arm_a9_Mmu_enable
#define Mmu_initDescAttrs ti_sysbios_family_arm_a9_Mmu_initDescAttrs
#define Mmu_isEnabled ti_sysbios_family_arm_a9_Mmu_isEnabled
#define Mmu_setFirstLevelDesc ti_sysbios_family_arm_a9_Mmu_setFirstLevelDesc
#define Mmu_Module_name ti_sysbios_family_arm_a9_Mmu_Module_name
#define Mmu_Module_id ti_sysbios_family_arm_a9_Mmu_Module_id
#define Mmu_Module_startup ti_sysbios_family_arm_a9_Mmu_Module_startup
#define Mmu_Module_startupDone ti_sysbios_family_arm_a9_Mmu_Module_startupDone
#define Mmu_Module_hasMask ti_sysbios_family_arm_a9_Mmu_Module_hasMask
#define Mmu_Module_getMask ti_sysbios_family_arm_a9_Mmu_Module_getMask
#define Mmu_Module_setMask ti_sysbios_family_arm_a9_Mmu_Module_setMask
#define Mmu_Object_heap ti_sysbios_family_arm_a9_Mmu_Object_heap
#define Mmu_Module_heap ti_sysbios_family_arm_a9_Mmu_Module_heap

#endif /* ti_sysbios_family_arm_a9_Mmu__localnames__done */
#endif
