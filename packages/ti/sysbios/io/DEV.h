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
 *     CREATE ARGS
 *     INTERNAL DEFINITIONS
 *     MODULE-WIDE CONFIGS
 *     PER-INSTANCE TYPES
 *     FUNCTION DECLARATIONS
 *     FUNCTION SELECTORS
 *     SYSTEM FUNCTIONS
 *     
 *     EPILOGUE
 *     STATE STRUCTURES
 *     PREFIX ALIASES
 */


/*
 * ======== PROLOGUE ========
 */

#ifndef ti_sysbios_io_DEV__include
#define ti_sysbios_io_DEV__include

#ifndef __nested__
#define __nested__
#define ti_sysbios_io_DEV__top__
#endif

#ifdef __cplusplus
#define __extern extern "C"
#else
#define __extern extern
#endif

#define ti_sysbios_io_DEV___VERS 150


/*
 * ======== INCLUDES ========
 */

#include <xdc/std.h>

#include <xdc/runtime/xdc.h>
#include <xdc/runtime/Types.h>
#include <xdc/runtime/IInstance.h>
#include <ti/sysbios/io/package/package.defs.h>

#include <xdc/runtime/IModule.h>
#include <xdc/runtime/Error.h>


/*
 * ======== AUXILIARY DEFINITIONS ========
 */

/* InitFxn */
typedef xdc_Void (*ti_sysbios_io_DEV_InitFxn)(void);

/* Fxns */
struct ti_sysbios_io_DEV_Fxns {
    xdc_Fxn mdBindDev;
    xdc_Fxn mdUnBindDev;
    xdc_Fxn mdControlChan;
    xdc_Fxn mdCreateChan;
    xdc_Fxn mdDeleteChan;
    xdc_Fxn mdSubmitChan;
};

/* DeviceParams */
struct ti_sysbios_io_DEV_DeviceParams {
    xdc_Int dummy;
};


/*
 * ======== CREATE ARGS ========
 */

/* Args__create */
typedef struct ti_sysbios_io_DEV_Args__create {
    xdc_String name;
    xdc_Ptr fxns;
} ti_sysbios_io_DEV_Args__create;


/*
 * ======== INTERNAL DEFINITIONS ========
 */

/* Module_State */
typedef ti_sysbios_io_DEV_Handle __T1_ti_sysbios_io_DEV_Module_State__table;
typedef ti_sysbios_io_DEV_Handle *__ARRAY1_ti_sysbios_io_DEV_Module_State__table;
typedef __ARRAY1_ti_sysbios_io_DEV_Module_State__table __TA_ti_sysbios_io_DEV_Module_State__table;


/*
 * ======== MODULE-WIDE CONFIGS ========
 */

/* Module__diagsEnabled */
typedef xdc_Bits32 CT__ti_sysbios_io_DEV_Module__diagsEnabled;
__extern __FAR__ const CT__ti_sysbios_io_DEV_Module__diagsEnabled ti_sysbios_io_DEV_Module__diagsEnabled__C;

/* Module__diagsIncluded */
typedef xdc_Bits32 CT__ti_sysbios_io_DEV_Module__diagsIncluded;
__extern __FAR__ const CT__ti_sysbios_io_DEV_Module__diagsIncluded ti_sysbios_io_DEV_Module__diagsIncluded__C;

/* Module__diagsMask */
typedef xdc_Bits16* CT__ti_sysbios_io_DEV_Module__diagsMask;
__extern __FAR__ const CT__ti_sysbios_io_DEV_Module__diagsMask ti_sysbios_io_DEV_Module__diagsMask__C;

/* Module__gateObj */
typedef xdc_Ptr CT__ti_sysbios_io_DEV_Module__gateObj;
__extern __FAR__ const CT__ti_sysbios_io_DEV_Module__gateObj ti_sysbios_io_DEV_Module__gateObj__C;

/* Module__gatePrms */
typedef xdc_Ptr CT__ti_sysbios_io_DEV_Module__gatePrms;
__extern __FAR__ const CT__ti_sysbios_io_DEV_Module__gatePrms ti_sysbios_io_DEV_Module__gatePrms__C;

/* Module__id */
typedef xdc_runtime_Types_ModuleId CT__ti_sysbios_io_DEV_Module__id;
__extern __FAR__ const CT__ti_sysbios_io_DEV_Module__id ti_sysbios_io_DEV_Module__id__C;

/* Module__loggerDefined */
typedef xdc_Bool CT__ti_sysbios_io_DEV_Module__loggerDefined;
__extern __FAR__ const CT__ti_sysbios_io_DEV_Module__loggerDefined ti_sysbios_io_DEV_Module__loggerDefined__C;

/* Module__loggerObj */
typedef xdc_Ptr CT__ti_sysbios_io_DEV_Module__loggerObj;
__extern __FAR__ const CT__ti_sysbios_io_DEV_Module__loggerObj ti_sysbios_io_DEV_Module__loggerObj__C;

/* Module__loggerFxn0 */
typedef xdc_runtime_Types_LoggerFxn0 CT__ti_sysbios_io_DEV_Module__loggerFxn0;
__extern __FAR__ const CT__ti_sysbios_io_DEV_Module__loggerFxn0 ti_sysbios_io_DEV_Module__loggerFxn0__C;

/* Module__loggerFxn1 */
typedef xdc_runtime_Types_LoggerFxn1 CT__ti_sysbios_io_DEV_Module__loggerFxn1;
__extern __FAR__ const CT__ti_sysbios_io_DEV_Module__loggerFxn1 ti_sysbios_io_DEV_Module__loggerFxn1__C;

/* Module__loggerFxn2 */
typedef xdc_runtime_Types_LoggerFxn2 CT__ti_sysbios_io_DEV_Module__loggerFxn2;
__extern __FAR__ const CT__ti_sysbios_io_DEV_Module__loggerFxn2 ti_sysbios_io_DEV_Module__loggerFxn2__C;

/* Module__loggerFxn4 */
typedef xdc_runtime_Types_LoggerFxn4 CT__ti_sysbios_io_DEV_Module__loggerFxn4;
__extern __FAR__ const CT__ti_sysbios_io_DEV_Module__loggerFxn4 ti_sysbios_io_DEV_Module__loggerFxn4__C;

/* Module__loggerFxn8 */
typedef xdc_runtime_Types_LoggerFxn8 CT__ti_sysbios_io_DEV_Module__loggerFxn8;
__extern __FAR__ const CT__ti_sysbios_io_DEV_Module__loggerFxn8 ti_sysbios_io_DEV_Module__loggerFxn8__C;

/* Module__startupDoneFxn */
typedef xdc_Bool (*CT__ti_sysbios_io_DEV_Module__startupDoneFxn)(void);
__extern __FAR__ const CT__ti_sysbios_io_DEV_Module__startupDoneFxn ti_sysbios_io_DEV_Module__startupDoneFxn__C;

/* Object__count */
typedef xdc_Int CT__ti_sysbios_io_DEV_Object__count;
__extern __FAR__ const CT__ti_sysbios_io_DEV_Object__count ti_sysbios_io_DEV_Object__count__C;

/* Object__heap */
typedef xdc_runtime_IHeap_Handle CT__ti_sysbios_io_DEV_Object__heap;
__extern __FAR__ const CT__ti_sysbios_io_DEV_Object__heap ti_sysbios_io_DEV_Object__heap__C;

/* Object__sizeof */
typedef xdc_SizeT CT__ti_sysbios_io_DEV_Object__sizeof;
__extern __FAR__ const CT__ti_sysbios_io_DEV_Object__sizeof ti_sysbios_io_DEV_Object__sizeof__C;

/* Object__table */
typedef xdc_Ptr CT__ti_sysbios_io_DEV_Object__table;
__extern __FAR__ const CT__ti_sysbios_io_DEV_Object__table ti_sysbios_io_DEV_Object__table__C;

/* tableSize */
#ifdef ti_sysbios_io_DEV_tableSize__D
#define ti_sysbios_io_DEV_tableSize (ti_sysbios_io_DEV_tableSize__D)
#else
#define ti_sysbios_io_DEV_tableSize (ti_sysbios_io_DEV_tableSize__C)
typedef xdc_UInt CT__ti_sysbios_io_DEV_tableSize;
__extern __FAR__ const CT__ti_sysbios_io_DEV_tableSize ti_sysbios_io_DEV_tableSize__C;
#endif


/*
 * ======== PER-INSTANCE TYPES ========
 */

/* Params */
struct ti_sysbios_io_DEV_Params {
    size_t __size;
    const void* __self;
    void* __fxns;
    xdc_runtime_IInstance_Params* instance;
    xdc_Ptr deviceParams;
    ti_sysbios_io_DEV_InitFxn initFxn;
    xdc_Int devid;
    xdc_runtime_IInstance_Params __iprms;
};

/* Struct */
struct ti_sysbios_io_DEV_Struct {
    xdc_String __f0;
    ti_sysbios_io_DEV_Fxns* __f1;
    ti_sysbios_io_DEV_InitFxn __f2;
    xdc_Int __f3;
    ti_sysbios_io_DEV_DeviceParams* __f4;
    xdc_Ptr __f5;
    xdc_runtime_Types_CordAddr __name;
};


/*
 * ======== FUNCTION DECLARATIONS ========
 */

/* Module_startup */
#define ti_sysbios_io_DEV_Module_startup ti_sysbios_io_DEV_Module_startup__E
xdc__CODESECT(ti_sysbios_io_DEV_Module_startup__E, "ti_sysbios_io_DEV_Module_startup")
__extern xdc_Int ti_sysbios_io_DEV_Module_startup__E( xdc_Int state );
xdc__CODESECT(ti_sysbios_io_DEV_Module_startup__F, "ti_sysbios_io_DEV_Module_startup")
__extern xdc_Int ti_sysbios_io_DEV_Module_startup__F( xdc_Int state );
xdc__CODESECT(ti_sysbios_io_DEV_Module_startup__R, "ti_sysbios_io_DEV_Module_startup")
__extern xdc_Int ti_sysbios_io_DEV_Module_startup__R( xdc_Int state );

/* Instance_init__F */
xdc__CODESECT(ti_sysbios_io_DEV_Instance_init__F, "ti_sysbios_io_DEV_Instance_init")
__extern int ti_sysbios_io_DEV_Instance_init__F( ti_sysbios_io_DEV_Object*, xdc_String name, xdc_Ptr fxns, const ti_sysbios_io_DEV_Params*, xdc_runtime_Error_Block* );

/* Instance_finalize__F */
xdc__CODESECT(ti_sysbios_io_DEV_Instance_finalize__F, "ti_sysbios_io_DEV_Instance_finalize")
__extern void ti_sysbios_io_DEV_Instance_finalize__F( ti_sysbios_io_DEV_Object* , int );

/* Instance_init__R */
xdc__CODESECT(ti_sysbios_io_DEV_Instance_init__R, "ti_sysbios_io_DEV_Instance_init")
__extern int ti_sysbios_io_DEV_Instance_init__R( ti_sysbios_io_DEV_Object*, xdc_String name, xdc_Ptr fxns, const ti_sysbios_io_DEV_Params*, xdc_runtime_Error_Block* );

/* Instance_finalize__R */
xdc__CODESECT(ti_sysbios_io_DEV_Instance_finalize__R, "ti_sysbios_io_DEV_Instance_finalize")
__extern void ti_sysbios_io_DEV_Instance_finalize__R( ti_sysbios_io_DEV_Object* , int );

/* Handle__label__S */
xdc__CODESECT(ti_sysbios_io_DEV_Handle__label__S, "ti_sysbios_io_DEV_Handle__label")
__extern xdc_runtime_Types_Label* ti_sysbios_io_DEV_Handle__label__S( xdc_Ptr obj, xdc_runtime_Types_Label* lab );

/* Module__startupDone__S */
xdc__CODESECT(ti_sysbios_io_DEV_Module__startupDone__S, "ti_sysbios_io_DEV_Module__startupDone")
__extern xdc_Bool ti_sysbios_io_DEV_Module__startupDone__S( void );

/* Object__create__S */
xdc__CODESECT(ti_sysbios_io_DEV_Object__create__S, "ti_sysbios_io_DEV_Object__create")
__extern xdc_Ptr ti_sysbios_io_DEV_Object__create__S( xdc_Ptr __oa, xdc_SizeT __osz, xdc_Ptr __aa, const xdc_UChar* __pa, xdc_SizeT __psz, xdc_runtime_Error_Block* __eb );

/* Object__delete__S */
xdc__CODESECT(ti_sysbios_io_DEV_Object__delete__S, "ti_sysbios_io_DEV_Object__delete")
__extern xdc_Void ti_sysbios_io_DEV_Object__delete__S( xdc_Ptr instp );

/* Object__destruct__S */
xdc__CODESECT(ti_sysbios_io_DEV_Object__destruct__S, "ti_sysbios_io_DEV_Object__destruct")
__extern xdc_Void ti_sysbios_io_DEV_Object__destruct__S( xdc_Ptr objp );

/* Object__get__S */
xdc__CODESECT(ti_sysbios_io_DEV_Object__get__S, "ti_sysbios_io_DEV_Object__get")
__extern xdc_Ptr ti_sysbios_io_DEV_Object__get__S( xdc_Ptr oarr, xdc_Int i );

/* Object__first__S */
xdc__CODESECT(ti_sysbios_io_DEV_Object__first__S, "ti_sysbios_io_DEV_Object__first")
__extern xdc_Ptr ti_sysbios_io_DEV_Object__first__S( void );

/* Object__next__S */
xdc__CODESECT(ti_sysbios_io_DEV_Object__next__S, "ti_sysbios_io_DEV_Object__next")
__extern xdc_Ptr ti_sysbios_io_DEV_Object__next__S( xdc_Ptr obj );

/* Params__init__S */
xdc__CODESECT(ti_sysbios_io_DEV_Params__init__S, "ti_sysbios_io_DEV_Params__init")
__extern xdc_Void ti_sysbios_io_DEV_Params__init__S( xdc_Ptr dst, xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz );

/* match__E */
#define ti_sysbios_io_DEV_match ti_sysbios_io_DEV_match__E
xdc__CODESECT(ti_sysbios_io_DEV_match__E, "ti_sysbios_io_DEV_match")
__extern xdc_String ti_sysbios_io_DEV_match__E( xdc_String name, ti_sysbios_io_DEV_Handle* device );
xdc__CODESECT(ti_sysbios_io_DEV_match__F, "ti_sysbios_io_DEV_match")
__extern xdc_String ti_sysbios_io_DEV_match__F( xdc_String name, ti_sysbios_io_DEV_Handle* device );
__extern xdc_String ti_sysbios_io_DEV_match__R( xdc_String name, ti_sysbios_io_DEV_Handle* device );

/* getName__E */
#define ti_sysbios_io_DEV_getName ti_sysbios_io_DEV_getName__E
xdc__CODESECT(ti_sysbios_io_DEV_getName__E, "ti_sysbios_io_DEV_getName")
__extern xdc_String ti_sysbios_io_DEV_getName__E( ti_sysbios_io_DEV_Handle __inst );
xdc__CODESECT(ti_sysbios_io_DEV_getName__F, "ti_sysbios_io_DEV_getName")
__extern xdc_String ti_sysbios_io_DEV_getName__F( ti_sysbios_io_DEV_Object* __inst );
__extern xdc_String ti_sysbios_io_DEV_getName__R( ti_sysbios_io_DEV_Handle __inst );

/* getFxns__E */
#define ti_sysbios_io_DEV_getFxns ti_sysbios_io_DEV_getFxns__E
xdc__CODESECT(ti_sysbios_io_DEV_getFxns__E, "ti_sysbios_io_DEV_getFxns")
__extern xdc_Ptr ti_sysbios_io_DEV_getFxns__E( ti_sysbios_io_DEV_Handle __inst );
xdc__CODESECT(ti_sysbios_io_DEV_getFxns__F, "ti_sysbios_io_DEV_getFxns")
__extern xdc_Ptr ti_sysbios_io_DEV_getFxns__F( ti_sysbios_io_DEV_Object* __inst );
__extern xdc_Ptr ti_sysbios_io_DEV_getFxns__R( ti_sysbios_io_DEV_Handle __inst );

/* getInitFxn__E */
#define ti_sysbios_io_DEV_getInitFxn ti_sysbios_io_DEV_getInitFxn__E
xdc__CODESECT(ti_sysbios_io_DEV_getInitFxn__E, "ti_sysbios_io_DEV_getInitFxn")
__extern ti_sysbios_io_DEV_InitFxn ti_sysbios_io_DEV_getInitFxn__E( ti_sysbios_io_DEV_Handle __inst );
xdc__CODESECT(ti_sysbios_io_DEV_getInitFxn__F, "ti_sysbios_io_DEV_getInitFxn")
__extern ti_sysbios_io_DEV_InitFxn ti_sysbios_io_DEV_getInitFxn__F( ti_sysbios_io_DEV_Object* __inst );
__extern ti_sysbios_io_DEV_InitFxn ti_sysbios_io_DEV_getInitFxn__R( ti_sysbios_io_DEV_Handle __inst );

/* getDevid__E */
#define ti_sysbios_io_DEV_getDevid ti_sysbios_io_DEV_getDevid__E
xdc__CODESECT(ti_sysbios_io_DEV_getDevid__E, "ti_sysbios_io_DEV_getDevid")
__extern xdc_Int ti_sysbios_io_DEV_getDevid__E( ti_sysbios_io_DEV_Handle __inst );
xdc__CODESECT(ti_sysbios_io_DEV_getDevid__F, "ti_sysbios_io_DEV_getDevid")
__extern xdc_Int ti_sysbios_io_DEV_getDevid__F( ti_sysbios_io_DEV_Object* __inst );
__extern xdc_Int ti_sysbios_io_DEV_getDevid__R( ti_sysbios_io_DEV_Handle __inst );

/* getDeviceParams__E */
#define ti_sysbios_io_DEV_getDeviceParams ti_sysbios_io_DEV_getDeviceParams__E
xdc__CODESECT(ti_sysbios_io_DEV_getDeviceParams__E, "ti_sysbios_io_DEV_getDeviceParams")
__extern xdc_Ptr ti_sysbios_io_DEV_getDeviceParams__E( ti_sysbios_io_DEV_Handle __inst );
xdc__CODESECT(ti_sysbios_io_DEV_getDeviceParams__F, "ti_sysbios_io_DEV_getDeviceParams")
__extern xdc_Ptr ti_sysbios_io_DEV_getDeviceParams__F( ti_sysbios_io_DEV_Object* __inst );
__extern xdc_Ptr ti_sysbios_io_DEV_getDeviceParams__R( ti_sysbios_io_DEV_Handle __inst );

/* getDevp__E */
#define ti_sysbios_io_DEV_getDevp ti_sysbios_io_DEV_getDevp__E
xdc__CODESECT(ti_sysbios_io_DEV_getDevp__E, "ti_sysbios_io_DEV_getDevp")
__extern xdc_Ptr ti_sysbios_io_DEV_getDevp__E( ti_sysbios_io_DEV_Handle __inst );
xdc__CODESECT(ti_sysbios_io_DEV_getDevp__F, "ti_sysbios_io_DEV_getDevp")
__extern xdc_Ptr ti_sysbios_io_DEV_getDevp__F( ti_sysbios_io_DEV_Object* __inst );
__extern xdc_Ptr ti_sysbios_io_DEV_getDevp__R( ti_sysbios_io_DEV_Handle __inst );

/* postInit__I */
#define ti_sysbios_io_DEV_postInit ti_sysbios_io_DEV_postInit__I
xdc__CODESECT(ti_sysbios_io_DEV_postInit__I, "ti_sysbios_io_DEV_postInit")
__extern xdc_Int ti_sysbios_io_DEV_postInit__I( ti_sysbios_io_DEV_Object* obj, xdc_runtime_Error_Block* eb );


/*
 * ======== FUNCTION SELECTORS ========
 */

/* getName_{FxnT,fxnP} */
typedef xdc_String (*ti_sysbios_io_DEV_getName_FxnT)(void*);
static inline ti_sysbios_io_DEV_getName_FxnT ti_sysbios_io_DEV_getName_fxnP( void )
{
    return (ti_sysbios_io_DEV_getName_FxnT)ti_sysbios_io_DEV_getName; 
}

/* getFxns_{FxnT,fxnP} */
typedef xdc_Ptr (*ti_sysbios_io_DEV_getFxns_FxnT)(void*);
static inline ti_sysbios_io_DEV_getFxns_FxnT ti_sysbios_io_DEV_getFxns_fxnP( void )
{
    return (ti_sysbios_io_DEV_getFxns_FxnT)ti_sysbios_io_DEV_getFxns; 
}

/* getInitFxn_{FxnT,fxnP} */
typedef ti_sysbios_io_DEV_InitFxn (*ti_sysbios_io_DEV_getInitFxn_FxnT)(void*);
static inline ti_sysbios_io_DEV_getInitFxn_FxnT ti_sysbios_io_DEV_getInitFxn_fxnP( void )
{
    return (ti_sysbios_io_DEV_getInitFxn_FxnT)ti_sysbios_io_DEV_getInitFxn; 
}

/* getDevid_{FxnT,fxnP} */
typedef xdc_Int (*ti_sysbios_io_DEV_getDevid_FxnT)(void*);
static inline ti_sysbios_io_DEV_getDevid_FxnT ti_sysbios_io_DEV_getDevid_fxnP( void )
{
    return (ti_sysbios_io_DEV_getDevid_FxnT)ti_sysbios_io_DEV_getDevid; 
}

/* getDeviceParams_{FxnT,fxnP} */
typedef xdc_Ptr (*ti_sysbios_io_DEV_getDeviceParams_FxnT)(void*);
static inline ti_sysbios_io_DEV_getDeviceParams_FxnT ti_sysbios_io_DEV_getDeviceParams_fxnP( void )
{
    return (ti_sysbios_io_DEV_getDeviceParams_FxnT)ti_sysbios_io_DEV_getDeviceParams; 
}

/* getDevp_{FxnT,fxnP} */
typedef xdc_Ptr (*ti_sysbios_io_DEV_getDevp_FxnT)(void*);
static inline ti_sysbios_io_DEV_getDevp_FxnT ti_sysbios_io_DEV_getDevp_fxnP( void )
{
    return (ti_sysbios_io_DEV_getDevp_FxnT)ti_sysbios_io_DEV_getDevp; 
}


/*
 * ======== SYSTEM FUNCTIONS ========
 */

/* Module_startupDone */
#define ti_sysbios_io_DEV_Module_startupDone() ti_sysbios_io_DEV_Module__startupDone__S()

/* Object_heap */
#define ti_sysbios_io_DEV_Object_heap() ti_sysbios_io_DEV_Object__heap__C

/* Module_heap */
#define ti_sysbios_io_DEV_Module_heap() ti_sysbios_io_DEV_Object__heap__C

/* Module_id */
static inline CT__ti_sysbios_io_DEV_Module__id ti_sysbios_io_DEV_Module_id( void ) 
{
    return ti_sysbios_io_DEV_Module__id__C;
}

/* Module_hasMask */
static inline xdc_Bool ti_sysbios_io_DEV_Module_hasMask( void ) 
{
    return ti_sysbios_io_DEV_Module__diagsMask__C != NULL;
}

/* Module_getMask */
static inline xdc_Bits16 ti_sysbios_io_DEV_Module_getMask( void ) 
{
    return ti_sysbios_io_DEV_Module__diagsMask__C != NULL ? *ti_sysbios_io_DEV_Module__diagsMask__C : 0;
}

/* Module_setMask */
static inline xdc_Void ti_sysbios_io_DEV_Module_setMask( xdc_Bits16 mask ) 
{
    if (ti_sysbios_io_DEV_Module__diagsMask__C != NULL) *ti_sysbios_io_DEV_Module__diagsMask__C = mask;
}

/* Params_init */
static inline void ti_sysbios_io_DEV_Params_init( ti_sysbios_io_DEV_Params* prms ) 
{
    if (prms) {
        ti_sysbios_io_DEV_Params__init__S(prms, 0, sizeof(ti_sysbios_io_DEV_Params), sizeof(xdc_runtime_IInstance_Params));
    }
}

/* Params_copy */
static inline void ti_sysbios_io_DEV_Params_copy( ti_sysbios_io_DEV_Params* dst, const ti_sysbios_io_DEV_Params* src ) 
{
    if (dst) {
        ti_sysbios_io_DEV_Params__init__S(dst, (xdc_Ptr)src, sizeof(ti_sysbios_io_DEV_Params), sizeof(xdc_runtime_IInstance_Params));
    }
}

/* Object_count */
#define ti_sysbios_io_DEV_Object_count() ti_sysbios_io_DEV_Object__count__C

/* Object_sizeof */
#define ti_sysbios_io_DEV_Object_sizeof() ti_sysbios_io_DEV_Object__sizeof__C

/* Object_get */
static inline ti_sysbios_io_DEV_Handle ti_sysbios_io_DEV_Object_get( ti_sysbios_io_DEV_Instance_State* oarr, int i ) 
{
    return (ti_sysbios_io_DEV_Handle)ti_sysbios_io_DEV_Object__get__S(oarr, i);
}

/* Object_first */
static inline ti_sysbios_io_DEV_Handle ti_sysbios_io_DEV_Object_first( void )
{
    return (ti_sysbios_io_DEV_Handle)ti_sysbios_io_DEV_Object__first__S();
}

/* Object_next */
static inline ti_sysbios_io_DEV_Handle ti_sysbios_io_DEV_Object_next( ti_sysbios_io_DEV_Object* obj )
{
    return (ti_sysbios_io_DEV_Handle)ti_sysbios_io_DEV_Object__next__S(obj);
}

/* Handle_label */
static inline xdc_runtime_Types_Label* ti_sysbios_io_DEV_Handle_label( ti_sysbios_io_DEV_Handle inst, xdc_runtime_Types_Label* lab )
{
    return ti_sysbios_io_DEV_Handle__label__S(inst, lab);
}

/* Handle_name */
static inline String ti_sysbios_io_DEV_Handle_name( ti_sysbios_io_DEV_Handle inst )
{
    xdc_runtime_Types_Label lab;
    return ti_sysbios_io_DEV_Handle__label__S(inst, &lab)->iname;
}

/* create */
static inline ti_sysbios_io_DEV_Handle ti_sysbios_io_DEV_create( xdc_String name, xdc_Ptr fxns, const ti_sysbios_io_DEV_Params* __prms, xdc_runtime_Error_Block* __eb )
{
    ti_sysbios_io_DEV_Args__create __args;
    __args.name = name;
    __args.fxns = fxns;
    return (ti_sysbios_io_DEV_Handle)ti_sysbios_io_DEV_Object__create__S(0, 0, &__args, (const xdc_UChar*)__prms, sizeof(ti_sysbios_io_DEV_Params), __eb);
}

/* construct */
static inline void ti_sysbios_io_DEV_construct( ti_sysbios_io_DEV_Struct* __obj, xdc_String name, xdc_Ptr fxns, const ti_sysbios_io_DEV_Params* __prms, xdc_runtime_Error_Block* __eb )
{
    ti_sysbios_io_DEV_Args__create __args;
    __args.name = name;
    __args.fxns = fxns;
    ti_sysbios_io_DEV_Object__create__S(__obj, sizeof (ti_sysbios_io_DEV_Struct), &__args, (const xdc_UChar*)__prms, sizeof(ti_sysbios_io_DEV_Params), __eb);
}

/* delete */
static inline void ti_sysbios_io_DEV_delete( ti_sysbios_io_DEV_Handle* instp )
{
    ti_sysbios_io_DEV_Object__delete__S(instp);
}

/* destruct */
static inline void ti_sysbios_io_DEV_destruct( ti_sysbios_io_DEV_Struct* obj )
{
    ti_sysbios_io_DEV_Object__destruct__S(obj);
}

/* handle */
static inline ti_sysbios_io_DEV_Handle ti_sysbios_io_DEV_handle( ti_sysbios_io_DEV_Struct* str )
{
    return (ti_sysbios_io_DEV_Handle)str;
}

/* struct */
static inline ti_sysbios_io_DEV_Struct* ti_sysbios_io_DEV_struct( ti_sysbios_io_DEV_Handle inst )
{
    return (ti_sysbios_io_DEV_Struct*)inst;
}


/*
 * ======== EPILOGUE ========
 */

#ifdef ti_sysbios_io_DEV__top__
#undef __nested__
#endif

#endif /* ti_sysbios_io_DEV__include */


/*
 * ======== STATE STRUCTURES ========
 */

#if defined(__config__) || (!defined(__nested__) && defined(ti_sysbios_io_DEV__internalaccess))

#ifndef ti_sysbios_io_DEV__include_state
#define ti_sysbios_io_DEV__include_state

/* Module_State */
struct ti_sysbios_io_DEV_Module_State {
    __TA_ti_sysbios_io_DEV_Module_State__table table;
};

/* Module__state__V */
extern struct ti_sysbios_io_DEV_Module_State__ ti_sysbios_io_DEV_Module__state__V;

/* Object */
struct ti_sysbios_io_DEV_Object {
    xdc_String name;
    ti_sysbios_io_DEV_Fxns* fxns;
    ti_sysbios_io_DEV_InitFxn initFxn;
    xdc_Int devid;
    ti_sysbios_io_DEV_DeviceParams* deviceParams;
    xdc_Ptr devp;
};

#endif /* ti_sysbios_io_DEV__include_state */

#endif


/*
 * ======== PREFIX ALIASES ========
 */

#if !defined(__nested__) && !defined(ti_sysbios_io_DEV__nolocalnames)

#ifndef ti_sysbios_io_DEV__localnames__done
#define ti_sysbios_io_DEV__localnames__done

/* module prefix */
#define DEV_Instance ti_sysbios_io_DEV_Instance
#define DEV_Handle ti_sysbios_io_DEV_Handle
#define DEV_Module ti_sysbios_io_DEV_Module
#define DEV_Object ti_sysbios_io_DEV_Object
#define DEV_Struct ti_sysbios_io_DEV_Struct
#define DEV_InitFxn ti_sysbios_io_DEV_InitFxn
#define DEV_Fxns ti_sysbios_io_DEV_Fxns
#define DEV_DeviceParams ti_sysbios_io_DEV_DeviceParams
#define DEV_Instance_State ti_sysbios_io_DEV_Instance_State
#define DEV_Module_State ti_sysbios_io_DEV_Module_State
#define DEV_tableSize ti_sysbios_io_DEV_tableSize
#define DEV_Params ti_sysbios_io_DEV_Params
#define DEV_match ti_sysbios_io_DEV_match
#define DEV_getName ti_sysbios_io_DEV_getName
#define DEV_getName_fxnP ti_sysbios_io_DEV_getName_fxnP
#define DEV_getName_FxnT ti_sysbios_io_DEV_getName_FxnT
#define DEV_getFxns ti_sysbios_io_DEV_getFxns
#define DEV_getFxns_fxnP ti_sysbios_io_DEV_getFxns_fxnP
#define DEV_getFxns_FxnT ti_sysbios_io_DEV_getFxns_FxnT
#define DEV_getInitFxn ti_sysbios_io_DEV_getInitFxn
#define DEV_getInitFxn_fxnP ti_sysbios_io_DEV_getInitFxn_fxnP
#define DEV_getInitFxn_FxnT ti_sysbios_io_DEV_getInitFxn_FxnT
#define DEV_getDevid ti_sysbios_io_DEV_getDevid
#define DEV_getDevid_fxnP ti_sysbios_io_DEV_getDevid_fxnP
#define DEV_getDevid_FxnT ti_sysbios_io_DEV_getDevid_FxnT
#define DEV_getDeviceParams ti_sysbios_io_DEV_getDeviceParams
#define DEV_getDeviceParams_fxnP ti_sysbios_io_DEV_getDeviceParams_fxnP
#define DEV_getDeviceParams_FxnT ti_sysbios_io_DEV_getDeviceParams_FxnT
#define DEV_getDevp ti_sysbios_io_DEV_getDevp
#define DEV_getDevp_fxnP ti_sysbios_io_DEV_getDevp_fxnP
#define DEV_getDevp_FxnT ti_sysbios_io_DEV_getDevp_FxnT
#define DEV_Module_name ti_sysbios_io_DEV_Module_name
#define DEV_Module_id ti_sysbios_io_DEV_Module_id
#define DEV_Module_startup ti_sysbios_io_DEV_Module_startup
#define DEV_Module_startupDone ti_sysbios_io_DEV_Module_startupDone
#define DEV_Module_hasMask ti_sysbios_io_DEV_Module_hasMask
#define DEV_Module_getMask ti_sysbios_io_DEV_Module_getMask
#define DEV_Module_setMask ti_sysbios_io_DEV_Module_setMask
#define DEV_Object_heap ti_sysbios_io_DEV_Object_heap
#define DEV_Module_heap ti_sysbios_io_DEV_Module_heap
#define DEV_construct ti_sysbios_io_DEV_construct
#define DEV_create ti_sysbios_io_DEV_create
#define DEV_handle ti_sysbios_io_DEV_handle
#define DEV_struct ti_sysbios_io_DEV_struct
#define DEV_Handle_label ti_sysbios_io_DEV_Handle_label
#define DEV_Handle_name ti_sysbios_io_DEV_Handle_name
#define DEV_Instance_init ti_sysbios_io_DEV_Instance_init
#define DEV_Object_count ti_sysbios_io_DEV_Object_count
#define DEV_Object_get ti_sysbios_io_DEV_Object_get
#define DEV_Object_first ti_sysbios_io_DEV_Object_first
#define DEV_Object_next ti_sysbios_io_DEV_Object_next
#define DEV_Object_sizeof ti_sysbios_io_DEV_Object_sizeof
#define DEV_Params_copy ti_sysbios_io_DEV_Params_copy
#define DEV_Params_init ti_sysbios_io_DEV_Params_init
#define DEV_Instance_finalize ti_sysbios_io_DEV_Instance_finalize
#define DEV_delete ti_sysbios_io_DEV_delete
#define DEV_destruct ti_sysbios_io_DEV_destruct

#endif /* ti_sysbios_io_DEV__localnames__done */
#endif
