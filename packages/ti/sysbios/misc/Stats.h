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

#ifndef ti_sysbios_misc_Stats__include
#define ti_sysbios_misc_Stats__include

#ifndef __nested__
#define __nested__
#define ti_sysbios_misc_Stats__top__
#endif

#ifdef __cplusplus
#define __extern extern "C"
#else
#define __extern extern
#endif

#define ti_sysbios_misc_Stats___VERS 150


/*
 * ======== INCLUDES ========
 */

#include <xdc/std.h>

#include <xdc/runtime/xdc.h>
#include <xdc/runtime/Types.h>
#include <xdc/runtime/IInstance.h>
#include <ti/sysbios/misc/package/package.defs.h>

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
typedef xdc_Bits32 CT__ti_sysbios_misc_Stats_Module__diagsEnabled;
__extern __FAR__ const CT__ti_sysbios_misc_Stats_Module__diagsEnabled ti_sysbios_misc_Stats_Module__diagsEnabled__C;

/* Module__diagsIncluded */
typedef xdc_Bits32 CT__ti_sysbios_misc_Stats_Module__diagsIncluded;
__extern __FAR__ const CT__ti_sysbios_misc_Stats_Module__diagsIncluded ti_sysbios_misc_Stats_Module__diagsIncluded__C;

/* Module__diagsMask */
typedef xdc_Bits16* CT__ti_sysbios_misc_Stats_Module__diagsMask;
__extern __FAR__ const CT__ti_sysbios_misc_Stats_Module__diagsMask ti_sysbios_misc_Stats_Module__diagsMask__C;

/* Module__gateObj */
typedef xdc_Ptr CT__ti_sysbios_misc_Stats_Module__gateObj;
__extern __FAR__ const CT__ti_sysbios_misc_Stats_Module__gateObj ti_sysbios_misc_Stats_Module__gateObj__C;

/* Module__gatePrms */
typedef xdc_Ptr CT__ti_sysbios_misc_Stats_Module__gatePrms;
__extern __FAR__ const CT__ti_sysbios_misc_Stats_Module__gatePrms ti_sysbios_misc_Stats_Module__gatePrms__C;

/* Module__id */
typedef xdc_runtime_Types_ModuleId CT__ti_sysbios_misc_Stats_Module__id;
__extern __FAR__ const CT__ti_sysbios_misc_Stats_Module__id ti_sysbios_misc_Stats_Module__id__C;

/* Module__loggerDefined */
typedef xdc_Bool CT__ti_sysbios_misc_Stats_Module__loggerDefined;
__extern __FAR__ const CT__ti_sysbios_misc_Stats_Module__loggerDefined ti_sysbios_misc_Stats_Module__loggerDefined__C;

/* Module__loggerObj */
typedef xdc_Ptr CT__ti_sysbios_misc_Stats_Module__loggerObj;
__extern __FAR__ const CT__ti_sysbios_misc_Stats_Module__loggerObj ti_sysbios_misc_Stats_Module__loggerObj__C;

/* Module__loggerFxn0 */
typedef xdc_runtime_Types_LoggerFxn0 CT__ti_sysbios_misc_Stats_Module__loggerFxn0;
__extern __FAR__ const CT__ti_sysbios_misc_Stats_Module__loggerFxn0 ti_sysbios_misc_Stats_Module__loggerFxn0__C;

/* Module__loggerFxn1 */
typedef xdc_runtime_Types_LoggerFxn1 CT__ti_sysbios_misc_Stats_Module__loggerFxn1;
__extern __FAR__ const CT__ti_sysbios_misc_Stats_Module__loggerFxn1 ti_sysbios_misc_Stats_Module__loggerFxn1__C;

/* Module__loggerFxn2 */
typedef xdc_runtime_Types_LoggerFxn2 CT__ti_sysbios_misc_Stats_Module__loggerFxn2;
__extern __FAR__ const CT__ti_sysbios_misc_Stats_Module__loggerFxn2 ti_sysbios_misc_Stats_Module__loggerFxn2__C;

/* Module__loggerFxn4 */
typedef xdc_runtime_Types_LoggerFxn4 CT__ti_sysbios_misc_Stats_Module__loggerFxn4;
__extern __FAR__ const CT__ti_sysbios_misc_Stats_Module__loggerFxn4 ti_sysbios_misc_Stats_Module__loggerFxn4__C;

/* Module__loggerFxn8 */
typedef xdc_runtime_Types_LoggerFxn8 CT__ti_sysbios_misc_Stats_Module__loggerFxn8;
__extern __FAR__ const CT__ti_sysbios_misc_Stats_Module__loggerFxn8 ti_sysbios_misc_Stats_Module__loggerFxn8__C;

/* Module__startupDoneFxn */
typedef xdc_Bool (*CT__ti_sysbios_misc_Stats_Module__startupDoneFxn)(void);
__extern __FAR__ const CT__ti_sysbios_misc_Stats_Module__startupDoneFxn ti_sysbios_misc_Stats_Module__startupDoneFxn__C;

/* Object__count */
typedef xdc_Int CT__ti_sysbios_misc_Stats_Object__count;
__extern __FAR__ const CT__ti_sysbios_misc_Stats_Object__count ti_sysbios_misc_Stats_Object__count__C;

/* Object__heap */
typedef xdc_runtime_IHeap_Handle CT__ti_sysbios_misc_Stats_Object__heap;
__extern __FAR__ const CT__ti_sysbios_misc_Stats_Object__heap ti_sysbios_misc_Stats_Object__heap__C;

/* Object__sizeof */
typedef xdc_SizeT CT__ti_sysbios_misc_Stats_Object__sizeof;
__extern __FAR__ const CT__ti_sysbios_misc_Stats_Object__sizeof ti_sysbios_misc_Stats_Object__sizeof__C;

/* Object__table */
typedef xdc_Ptr CT__ti_sysbios_misc_Stats_Object__table;
__extern __FAR__ const CT__ti_sysbios_misc_Stats_Object__table ti_sysbios_misc_Stats_Object__table__C;


/*
 * ======== PER-INSTANCE TYPES ========
 */

/* Params */
struct ti_sysbios_misc_Stats_Params {
    size_t __size;
    const void* __self;
    void* __fxns;
    xdc_runtime_IInstance_Params* instance;
    xdc_Int32 previousVal;
    xdc_runtime_IInstance_Params __iprms;
};

/* Struct */
struct ti_sysbios_misc_Stats_Struct {
    xdc_Int32 __f0;
    xdc_Int32 __f1;
    xdc_Int32 __f2;
    xdc_Int32 __f3;
    xdc_runtime_Types_CordAddr __name;
};


/*
 * ======== FUNCTION DECLARATIONS ========
 */

/* Module_startup */
#define ti_sysbios_misc_Stats_Module_startup( state ) (-1)

/* Instance_init__F */
xdc__CODESECT(ti_sysbios_misc_Stats_Instance_init__F, "ti_sysbios_misc_Stats_Instance_init")
__extern void ti_sysbios_misc_Stats_Instance_init__F( ti_sysbios_misc_Stats_Object*, const ti_sysbios_misc_Stats_Params* );

/* Instance_init__R */
xdc__CODESECT(ti_sysbios_misc_Stats_Instance_init__R, "ti_sysbios_misc_Stats_Instance_init")
__extern void ti_sysbios_misc_Stats_Instance_init__R( ti_sysbios_misc_Stats_Object*, const ti_sysbios_misc_Stats_Params* );

/* Handle__label__S */
xdc__CODESECT(ti_sysbios_misc_Stats_Handle__label__S, "ti_sysbios_misc_Stats_Handle__label")
__extern xdc_runtime_Types_Label* ti_sysbios_misc_Stats_Handle__label__S( xdc_Ptr obj, xdc_runtime_Types_Label* lab );

/* Module__startupDone__S */
xdc__CODESECT(ti_sysbios_misc_Stats_Module__startupDone__S, "ti_sysbios_misc_Stats_Module__startupDone")
__extern xdc_Bool ti_sysbios_misc_Stats_Module__startupDone__S( void );

/* Object__create__S */
xdc__CODESECT(ti_sysbios_misc_Stats_Object__create__S, "ti_sysbios_misc_Stats_Object__create")
__extern xdc_Ptr ti_sysbios_misc_Stats_Object__create__S( xdc_Ptr __oa, xdc_SizeT __osz, xdc_Ptr __aa, const xdc_UChar* __pa, xdc_SizeT __psz, xdc_runtime_Error_Block* __eb );

/* Object__delete__S */
xdc__CODESECT(ti_sysbios_misc_Stats_Object__delete__S, "ti_sysbios_misc_Stats_Object__delete")
__extern xdc_Void ti_sysbios_misc_Stats_Object__delete__S( xdc_Ptr instp );

/* Object__destruct__S */
xdc__CODESECT(ti_sysbios_misc_Stats_Object__destruct__S, "ti_sysbios_misc_Stats_Object__destruct")
__extern xdc_Void ti_sysbios_misc_Stats_Object__destruct__S( xdc_Ptr objp );

/* Object__get__S */
xdc__CODESECT(ti_sysbios_misc_Stats_Object__get__S, "ti_sysbios_misc_Stats_Object__get")
__extern xdc_Ptr ti_sysbios_misc_Stats_Object__get__S( xdc_Ptr oarr, xdc_Int i );

/* Object__first__S */
xdc__CODESECT(ti_sysbios_misc_Stats_Object__first__S, "ti_sysbios_misc_Stats_Object__first")
__extern xdc_Ptr ti_sysbios_misc_Stats_Object__first__S( void );

/* Object__next__S */
xdc__CODESECT(ti_sysbios_misc_Stats_Object__next__S, "ti_sysbios_misc_Stats_Object__next")
__extern xdc_Ptr ti_sysbios_misc_Stats_Object__next__S( xdc_Ptr obj );

/* Params__init__S */
xdc__CODESECT(ti_sysbios_misc_Stats_Params__init__S, "ti_sysbios_misc_Stats_Params__init")
__extern xdc_Void ti_sysbios_misc_Stats_Params__init__S( xdc_Ptr dst, xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz );

/* add__E */
#define ti_sysbios_misc_Stats_add ti_sysbios_misc_Stats_add__E
xdc__CODESECT(ti_sysbios_misc_Stats_add__E, "ti_sysbios_misc_Stats_add")
__extern xdc_Void ti_sysbios_misc_Stats_add__E( ti_sysbios_misc_Stats_Handle __inst, xdc_Int32 value );
xdc__CODESECT(ti_sysbios_misc_Stats_add__F, "ti_sysbios_misc_Stats_add")
__extern xdc_Void ti_sysbios_misc_Stats_add__F( ti_sysbios_misc_Stats_Object* __inst, xdc_Int32 value );
__extern xdc_Void ti_sysbios_misc_Stats_add__R( ti_sysbios_misc_Stats_Handle __inst, xdc_Int32 value );

/* delta__E */
#define ti_sysbios_misc_Stats_delta ti_sysbios_misc_Stats_delta__E
xdc__CODESECT(ti_sysbios_misc_Stats_delta__E, "ti_sysbios_misc_Stats_delta")
__extern xdc_Void ti_sysbios_misc_Stats_delta__E( ti_sysbios_misc_Stats_Handle __inst, xdc_Int32 value );
xdc__CODESECT(ti_sysbios_misc_Stats_delta__F, "ti_sysbios_misc_Stats_delta")
__extern xdc_Void ti_sysbios_misc_Stats_delta__F( ti_sysbios_misc_Stats_Object* __inst, xdc_Int32 value );
__extern xdc_Void ti_sysbios_misc_Stats_delta__R( ti_sysbios_misc_Stats_Handle __inst, xdc_Int32 value );

/* set__E */
#define ti_sysbios_misc_Stats_set ti_sysbios_misc_Stats_set__E
xdc__CODESECT(ti_sysbios_misc_Stats_set__E, "ti_sysbios_misc_Stats_set")
__extern xdc_Void ti_sysbios_misc_Stats_set__E( ti_sysbios_misc_Stats_Handle __inst, xdc_Int32 value );
xdc__CODESECT(ti_sysbios_misc_Stats_set__F, "ti_sysbios_misc_Stats_set")
__extern xdc_Void ti_sysbios_misc_Stats_set__F( ti_sysbios_misc_Stats_Object* __inst, xdc_Int32 value );
__extern xdc_Void ti_sysbios_misc_Stats_set__R( ti_sysbios_misc_Stats_Handle __inst, xdc_Int32 value );

/* reset__E */
#define ti_sysbios_misc_Stats_reset ti_sysbios_misc_Stats_reset__E
xdc__CODESECT(ti_sysbios_misc_Stats_reset__E, "ti_sysbios_misc_Stats_reset")
__extern xdc_Void ti_sysbios_misc_Stats_reset__E( ti_sysbios_misc_Stats_Handle __inst );
xdc__CODESECT(ti_sysbios_misc_Stats_reset__F, "ti_sysbios_misc_Stats_reset")
__extern xdc_Void ti_sysbios_misc_Stats_reset__F( ti_sysbios_misc_Stats_Object* __inst );
__extern xdc_Void ti_sysbios_misc_Stats_reset__R( ti_sysbios_misc_Stats_Handle __inst );

/* getPrev__E */
#define ti_sysbios_misc_Stats_getPrev ti_sysbios_misc_Stats_getPrev__E
xdc__CODESECT(ti_sysbios_misc_Stats_getPrev__E, "ti_sysbios_misc_Stats_getPrev")
__extern xdc_Int32 ti_sysbios_misc_Stats_getPrev__E( ti_sysbios_misc_Stats_Handle __inst );
xdc__CODESECT(ti_sysbios_misc_Stats_getPrev__F, "ti_sysbios_misc_Stats_getPrev")
__extern xdc_Int32 ti_sysbios_misc_Stats_getPrev__F( ti_sysbios_misc_Stats_Object* __inst );
__extern xdc_Int32 ti_sysbios_misc_Stats_getPrev__R( ti_sysbios_misc_Stats_Handle __inst );

/* getCount__E */
#define ti_sysbios_misc_Stats_getCount ti_sysbios_misc_Stats_getCount__E
xdc__CODESECT(ti_sysbios_misc_Stats_getCount__E, "ti_sysbios_misc_Stats_getCount")
__extern xdc_Int32 ti_sysbios_misc_Stats_getCount__E( ti_sysbios_misc_Stats_Handle __inst );
xdc__CODESECT(ti_sysbios_misc_Stats_getCount__F, "ti_sysbios_misc_Stats_getCount")
__extern xdc_Int32 ti_sysbios_misc_Stats_getCount__F( ti_sysbios_misc_Stats_Object* __inst );
__extern xdc_Int32 ti_sysbios_misc_Stats_getCount__R( ti_sysbios_misc_Stats_Handle __inst );

/* getTotal__E */
#define ti_sysbios_misc_Stats_getTotal ti_sysbios_misc_Stats_getTotal__E
xdc__CODESECT(ti_sysbios_misc_Stats_getTotal__E, "ti_sysbios_misc_Stats_getTotal")
__extern xdc_Int32 ti_sysbios_misc_Stats_getTotal__E( ti_sysbios_misc_Stats_Handle __inst );
xdc__CODESECT(ti_sysbios_misc_Stats_getTotal__F, "ti_sysbios_misc_Stats_getTotal")
__extern xdc_Int32 ti_sysbios_misc_Stats_getTotal__F( ti_sysbios_misc_Stats_Object* __inst );
__extern xdc_Int32 ti_sysbios_misc_Stats_getTotal__R( ti_sysbios_misc_Stats_Handle __inst );

/* getMax__E */
#define ti_sysbios_misc_Stats_getMax ti_sysbios_misc_Stats_getMax__E
xdc__CODESECT(ti_sysbios_misc_Stats_getMax__E, "ti_sysbios_misc_Stats_getMax")
__extern xdc_Int32 ti_sysbios_misc_Stats_getMax__E( ti_sysbios_misc_Stats_Handle __inst );
xdc__CODESECT(ti_sysbios_misc_Stats_getMax__F, "ti_sysbios_misc_Stats_getMax")
__extern xdc_Int32 ti_sysbios_misc_Stats_getMax__F( ti_sysbios_misc_Stats_Object* __inst );
__extern xdc_Int32 ti_sysbios_misc_Stats_getMax__R( ti_sysbios_misc_Stats_Handle __inst );

/* print__E */
#define ti_sysbios_misc_Stats_print ti_sysbios_misc_Stats_print__E
xdc__CODESECT(ti_sysbios_misc_Stats_print__E, "ti_sysbios_misc_Stats_print")
__extern xdc_Void ti_sysbios_misc_Stats_print__E( ti_sysbios_misc_Stats_Handle __inst );
xdc__CODESECT(ti_sysbios_misc_Stats_print__F, "ti_sysbios_misc_Stats_print")
__extern xdc_Void ti_sysbios_misc_Stats_print__F( ti_sysbios_misc_Stats_Object* __inst );
__extern xdc_Void ti_sysbios_misc_Stats_print__R( ti_sysbios_misc_Stats_Handle __inst );


/*
 * ======== FUNCTION SELECTORS ========
 */

/* add_{FxnT,fxnP} */
typedef xdc_Void (*ti_sysbios_misc_Stats_add_FxnT)(void*, xdc_Int32);
static inline ti_sysbios_misc_Stats_add_FxnT ti_sysbios_misc_Stats_add_fxnP( void )
{
    return (ti_sysbios_misc_Stats_add_FxnT)ti_sysbios_misc_Stats_add; 
}

/* delta_{FxnT,fxnP} */
typedef xdc_Void (*ti_sysbios_misc_Stats_delta_FxnT)(void*, xdc_Int32);
static inline ti_sysbios_misc_Stats_delta_FxnT ti_sysbios_misc_Stats_delta_fxnP( void )
{
    return (ti_sysbios_misc_Stats_delta_FxnT)ti_sysbios_misc_Stats_delta; 
}

/* set_{FxnT,fxnP} */
typedef xdc_Void (*ti_sysbios_misc_Stats_set_FxnT)(void*, xdc_Int32);
static inline ti_sysbios_misc_Stats_set_FxnT ti_sysbios_misc_Stats_set_fxnP( void )
{
    return (ti_sysbios_misc_Stats_set_FxnT)ti_sysbios_misc_Stats_set; 
}

/* reset_{FxnT,fxnP} */
typedef xdc_Void (*ti_sysbios_misc_Stats_reset_FxnT)(void*);
static inline ti_sysbios_misc_Stats_reset_FxnT ti_sysbios_misc_Stats_reset_fxnP( void )
{
    return (ti_sysbios_misc_Stats_reset_FxnT)ti_sysbios_misc_Stats_reset; 
}

/* getPrev_{FxnT,fxnP} */
typedef xdc_Int32 (*ti_sysbios_misc_Stats_getPrev_FxnT)(void*);
static inline ti_sysbios_misc_Stats_getPrev_FxnT ti_sysbios_misc_Stats_getPrev_fxnP( void )
{
    return (ti_sysbios_misc_Stats_getPrev_FxnT)ti_sysbios_misc_Stats_getPrev; 
}

/* getCount_{FxnT,fxnP} */
typedef xdc_Int32 (*ti_sysbios_misc_Stats_getCount_FxnT)(void*);
static inline ti_sysbios_misc_Stats_getCount_FxnT ti_sysbios_misc_Stats_getCount_fxnP( void )
{
    return (ti_sysbios_misc_Stats_getCount_FxnT)ti_sysbios_misc_Stats_getCount; 
}

/* getTotal_{FxnT,fxnP} */
typedef xdc_Int32 (*ti_sysbios_misc_Stats_getTotal_FxnT)(void*);
static inline ti_sysbios_misc_Stats_getTotal_FxnT ti_sysbios_misc_Stats_getTotal_fxnP( void )
{
    return (ti_sysbios_misc_Stats_getTotal_FxnT)ti_sysbios_misc_Stats_getTotal; 
}

/* getMax_{FxnT,fxnP} */
typedef xdc_Int32 (*ti_sysbios_misc_Stats_getMax_FxnT)(void*);
static inline ti_sysbios_misc_Stats_getMax_FxnT ti_sysbios_misc_Stats_getMax_fxnP( void )
{
    return (ti_sysbios_misc_Stats_getMax_FxnT)ti_sysbios_misc_Stats_getMax; 
}

/* print_{FxnT,fxnP} */
typedef xdc_Void (*ti_sysbios_misc_Stats_print_FxnT)(void*);
static inline ti_sysbios_misc_Stats_print_FxnT ti_sysbios_misc_Stats_print_fxnP( void )
{
    return (ti_sysbios_misc_Stats_print_FxnT)ti_sysbios_misc_Stats_print; 
}


/*
 * ======== SYSTEM FUNCTIONS ========
 */

/* Module_startupDone */
#define ti_sysbios_misc_Stats_Module_startupDone() ti_sysbios_misc_Stats_Module__startupDone__S()

/* Object_heap */
#define ti_sysbios_misc_Stats_Object_heap() ti_sysbios_misc_Stats_Object__heap__C

/* Module_heap */
#define ti_sysbios_misc_Stats_Module_heap() ti_sysbios_misc_Stats_Object__heap__C

/* Module_id */
static inline CT__ti_sysbios_misc_Stats_Module__id ti_sysbios_misc_Stats_Module_id( void ) 
{
    return ti_sysbios_misc_Stats_Module__id__C;
}

/* Module_hasMask */
static inline xdc_Bool ti_sysbios_misc_Stats_Module_hasMask( void ) 
{
    return ti_sysbios_misc_Stats_Module__diagsMask__C != NULL;
}

/* Module_getMask */
static inline xdc_Bits16 ti_sysbios_misc_Stats_Module_getMask( void ) 
{
    return ti_sysbios_misc_Stats_Module__diagsMask__C != NULL ? *ti_sysbios_misc_Stats_Module__diagsMask__C : 0;
}

/* Module_setMask */
static inline xdc_Void ti_sysbios_misc_Stats_Module_setMask( xdc_Bits16 mask ) 
{
    if (ti_sysbios_misc_Stats_Module__diagsMask__C != NULL) *ti_sysbios_misc_Stats_Module__diagsMask__C = mask;
}

/* Params_init */
static inline void ti_sysbios_misc_Stats_Params_init( ti_sysbios_misc_Stats_Params* prms ) 
{
    if (prms) {
        ti_sysbios_misc_Stats_Params__init__S(prms, 0, sizeof(ti_sysbios_misc_Stats_Params), sizeof(xdc_runtime_IInstance_Params));
    }
}

/* Params_copy */
static inline void ti_sysbios_misc_Stats_Params_copy( ti_sysbios_misc_Stats_Params* dst, const ti_sysbios_misc_Stats_Params* src ) 
{
    if (dst) {
        ti_sysbios_misc_Stats_Params__init__S(dst, (xdc_Ptr)src, sizeof(ti_sysbios_misc_Stats_Params), sizeof(xdc_runtime_IInstance_Params));
    }
}

/* Object_count */
#define ti_sysbios_misc_Stats_Object_count() ti_sysbios_misc_Stats_Object__count__C

/* Object_sizeof */
#define ti_sysbios_misc_Stats_Object_sizeof() ti_sysbios_misc_Stats_Object__sizeof__C

/* Object_get */
static inline ti_sysbios_misc_Stats_Handle ti_sysbios_misc_Stats_Object_get( ti_sysbios_misc_Stats_Instance_State* oarr, int i ) 
{
    return (ti_sysbios_misc_Stats_Handle)ti_sysbios_misc_Stats_Object__get__S(oarr, i);
}

/* Object_first */
static inline ti_sysbios_misc_Stats_Handle ti_sysbios_misc_Stats_Object_first( void )
{
    return (ti_sysbios_misc_Stats_Handle)ti_sysbios_misc_Stats_Object__first__S();
}

/* Object_next */
static inline ti_sysbios_misc_Stats_Handle ti_sysbios_misc_Stats_Object_next( ti_sysbios_misc_Stats_Object* obj )
{
    return (ti_sysbios_misc_Stats_Handle)ti_sysbios_misc_Stats_Object__next__S(obj);
}

/* Handle_label */
static inline xdc_runtime_Types_Label* ti_sysbios_misc_Stats_Handle_label( ti_sysbios_misc_Stats_Handle inst, xdc_runtime_Types_Label* lab )
{
    return ti_sysbios_misc_Stats_Handle__label__S(inst, lab);
}

/* Handle_name */
static inline String ti_sysbios_misc_Stats_Handle_name( ti_sysbios_misc_Stats_Handle inst )
{
    xdc_runtime_Types_Label lab;
    return ti_sysbios_misc_Stats_Handle__label__S(inst, &lab)->iname;
}

/* create */
static inline ti_sysbios_misc_Stats_Handle ti_sysbios_misc_Stats_create( const ti_sysbios_misc_Stats_Params* __prms, xdc_runtime_Error_Block* __eb )
{
    return (ti_sysbios_misc_Stats_Handle)ti_sysbios_misc_Stats_Object__create__S(0, 0, 0, (const xdc_UChar*)__prms, sizeof(ti_sysbios_misc_Stats_Params), __eb);
}

/* construct */
static inline void ti_sysbios_misc_Stats_construct( ti_sysbios_misc_Stats_Struct* __obj, const ti_sysbios_misc_Stats_Params* __prms )
{
    ti_sysbios_misc_Stats_Object__create__S(__obj, sizeof (ti_sysbios_misc_Stats_Struct), 0, (const xdc_UChar*)__prms, sizeof(ti_sysbios_misc_Stats_Params), NULL);
}

/* delete */
static inline void ti_sysbios_misc_Stats_delete( ti_sysbios_misc_Stats_Handle* instp )
{
    ti_sysbios_misc_Stats_Object__delete__S(instp);
}

/* destruct */
static inline void ti_sysbios_misc_Stats_destruct( ti_sysbios_misc_Stats_Struct* obj )
{
    ti_sysbios_misc_Stats_Object__destruct__S(obj);
}

/* handle */
static inline ti_sysbios_misc_Stats_Handle ti_sysbios_misc_Stats_handle( ti_sysbios_misc_Stats_Struct* str )
{
    return (ti_sysbios_misc_Stats_Handle)str;
}

/* struct */
static inline ti_sysbios_misc_Stats_Struct* ti_sysbios_misc_Stats_struct( ti_sysbios_misc_Stats_Handle inst )
{
    return (ti_sysbios_misc_Stats_Struct*)inst;
}


/*
 * ======== EPILOGUE ========
 */

#ifdef ti_sysbios_misc_Stats__top__
#undef __nested__
#endif

#endif /* ti_sysbios_misc_Stats__include */


/*
 * ======== STATE STRUCTURES ========
 */

#if defined(__config__) || (!defined(__nested__) && defined(ti_sysbios_misc_Stats__internalaccess))

#ifndef ti_sysbios_misc_Stats__include_state
#define ti_sysbios_misc_Stats__include_state

/* Object */
struct ti_sysbios_misc_Stats_Object {
    xdc_Int32 prev;
    xdc_Int32 count;
    xdc_Int32 total;
    xdc_Int32 max;
};

#endif /* ti_sysbios_misc_Stats__include_state */

#endif


/*
 * ======== PREFIX ALIASES ========
 */

#if !defined(__nested__) && !defined(ti_sysbios_misc_Stats__nolocalnames)

#ifndef ti_sysbios_misc_Stats__localnames__done
#define ti_sysbios_misc_Stats__localnames__done

/* module prefix */
#define Stats_Instance ti_sysbios_misc_Stats_Instance
#define Stats_Handle ti_sysbios_misc_Stats_Handle
#define Stats_Module ti_sysbios_misc_Stats_Module
#define Stats_Object ti_sysbios_misc_Stats_Object
#define Stats_Struct ti_sysbios_misc_Stats_Struct
#define Stats_Instance_State ti_sysbios_misc_Stats_Instance_State
#define Stats_Params ti_sysbios_misc_Stats_Params
#define Stats_add ti_sysbios_misc_Stats_add
#define Stats_add_fxnP ti_sysbios_misc_Stats_add_fxnP
#define Stats_add_FxnT ti_sysbios_misc_Stats_add_FxnT
#define Stats_delta ti_sysbios_misc_Stats_delta
#define Stats_delta_fxnP ti_sysbios_misc_Stats_delta_fxnP
#define Stats_delta_FxnT ti_sysbios_misc_Stats_delta_FxnT
#define Stats_set ti_sysbios_misc_Stats_set
#define Stats_set_fxnP ti_sysbios_misc_Stats_set_fxnP
#define Stats_set_FxnT ti_sysbios_misc_Stats_set_FxnT
#define Stats_reset ti_sysbios_misc_Stats_reset
#define Stats_reset_fxnP ti_sysbios_misc_Stats_reset_fxnP
#define Stats_reset_FxnT ti_sysbios_misc_Stats_reset_FxnT
#define Stats_getPrev ti_sysbios_misc_Stats_getPrev
#define Stats_getPrev_fxnP ti_sysbios_misc_Stats_getPrev_fxnP
#define Stats_getPrev_FxnT ti_sysbios_misc_Stats_getPrev_FxnT
#define Stats_getCount ti_sysbios_misc_Stats_getCount
#define Stats_getCount_fxnP ti_sysbios_misc_Stats_getCount_fxnP
#define Stats_getCount_FxnT ti_sysbios_misc_Stats_getCount_FxnT
#define Stats_getTotal ti_sysbios_misc_Stats_getTotal
#define Stats_getTotal_fxnP ti_sysbios_misc_Stats_getTotal_fxnP
#define Stats_getTotal_FxnT ti_sysbios_misc_Stats_getTotal_FxnT
#define Stats_getMax ti_sysbios_misc_Stats_getMax
#define Stats_getMax_fxnP ti_sysbios_misc_Stats_getMax_fxnP
#define Stats_getMax_FxnT ti_sysbios_misc_Stats_getMax_FxnT
#define Stats_print ti_sysbios_misc_Stats_print
#define Stats_print_fxnP ti_sysbios_misc_Stats_print_fxnP
#define Stats_print_FxnT ti_sysbios_misc_Stats_print_FxnT
#define Stats_Module_name ti_sysbios_misc_Stats_Module_name
#define Stats_Module_id ti_sysbios_misc_Stats_Module_id
#define Stats_Module_startup ti_sysbios_misc_Stats_Module_startup
#define Stats_Module_startupDone ti_sysbios_misc_Stats_Module_startupDone
#define Stats_Module_hasMask ti_sysbios_misc_Stats_Module_hasMask
#define Stats_Module_getMask ti_sysbios_misc_Stats_Module_getMask
#define Stats_Module_setMask ti_sysbios_misc_Stats_Module_setMask
#define Stats_Object_heap ti_sysbios_misc_Stats_Object_heap
#define Stats_Module_heap ti_sysbios_misc_Stats_Module_heap
#define Stats_construct ti_sysbios_misc_Stats_construct
#define Stats_create ti_sysbios_misc_Stats_create
#define Stats_handle ti_sysbios_misc_Stats_handle
#define Stats_struct ti_sysbios_misc_Stats_struct
#define Stats_Handle_label ti_sysbios_misc_Stats_Handle_label
#define Stats_Handle_name ti_sysbios_misc_Stats_Handle_name
#define Stats_Instance_init ti_sysbios_misc_Stats_Instance_init
#define Stats_Object_count ti_sysbios_misc_Stats_Object_count
#define Stats_Object_get ti_sysbios_misc_Stats_Object_get
#define Stats_Object_first ti_sysbios_misc_Stats_Object_first
#define Stats_Object_next ti_sysbios_misc_Stats_Object_next
#define Stats_Object_sizeof ti_sysbios_misc_Stats_Object_sizeof
#define Stats_Params_copy ti_sysbios_misc_Stats_Params_copy
#define Stats_Params_init ti_sysbios_misc_Stats_Params_init
#define Stats_delete ti_sysbios_misc_Stats_delete
#define Stats_destruct ti_sysbios_misc_Stats_destruct

#endif /* ti_sysbios_misc_Stats__localnames__done */
#endif
