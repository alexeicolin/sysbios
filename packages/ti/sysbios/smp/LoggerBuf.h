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
 *     VIRTUAL FUNCTIONS
 *     FUNCTION DECLARATIONS
 *     FUNCTION SELECTORS
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

#ifndef ti_sysbios_smp_LoggerBuf__include
#define ti_sysbios_smp_LoggerBuf__include

#ifndef __nested__
#define __nested__
#define ti_sysbios_smp_LoggerBuf__top__
#endif

#ifdef __cplusplus
#define __extern extern "C"
#else
#define __extern extern
#endif

#define ti_sysbios_smp_LoggerBuf___VERS 150


/*
 * ======== INCLUDES ========
 */

#include <xdc/std.h>

#include <xdc/runtime/xdc.h>
#include <xdc/runtime/Types.h>
#include <xdc/runtime/IInstance.h>
#include <ti/sysbios/smp/package/package.defs.h>

#include <xdc/runtime/Assert.h>
#include <xdc/runtime/IGateProvider.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Types.h>
#include <xdc/runtime/ILogger.h>
#include <xdc/runtime/ITimestampClient.h>
#include <xdc/runtime/IHeap.h>
#include <xdc/runtime/Log.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/IFilterLogger.h>
#include <ti/sysbios/smp/package/LoggerBuf_TimestampProxy.h>
#include <ti/sysbios/smp/package/LoggerBuf_Module_GateProxy.h>


/*
 * ======== AUXILIARY DEFINITIONS ========
 */

/* BufType */
enum ti_sysbios_smp_LoggerBuf_BufType {
    ti_sysbios_smp_LoggerBuf_BufType_CIRCULAR,
    ti_sysbios_smp_LoggerBuf_BufType_FIXED
};
typedef enum ti_sysbios_smp_LoggerBuf_BufType ti_sysbios_smp_LoggerBuf_BufType;


/*
 * ======== INTERNAL DEFINITIONS ========
 */

/* FULL */
#define ti_sysbios_smp_LoggerBuf_FULL (-1)

/* WRAP */
#define ti_sysbios_smp_LoggerBuf_WRAP (0)

/* NEXT */
#define ti_sysbios_smp_LoggerBuf_NEXT (1)

/* Entry */
struct ti_sysbios_smp_LoggerBuf_Entry {
    xdc_runtime_Types_Timestamp64 tstamp;
    xdc_Bits32 serial;
    xdc_runtime_Types_Event evt;
    xdc_IArg arg1;
    xdc_IArg arg2;
    xdc_IArg arg3;
    xdc_IArg arg4;
};

/* Instance_State */
typedef ti_sysbios_smp_LoggerBuf_Entry __T1_ti_sysbios_smp_LoggerBuf_Instance_State__entryArr;
typedef ti_sysbios_smp_LoggerBuf_Entry *__ARRAY1_ti_sysbios_smp_LoggerBuf_Instance_State__entryArr;
typedef __ARRAY1_ti_sysbios_smp_LoggerBuf_Instance_State__entryArr __TA_ti_sysbios_smp_LoggerBuf_Instance_State__entryArr;


/*
 * ======== MODULE-WIDE CONFIGS ========
 */

/* Module__diagsEnabled */
typedef xdc_Bits32 CT__ti_sysbios_smp_LoggerBuf_Module__diagsEnabled;
__extern __FAR__ const CT__ti_sysbios_smp_LoggerBuf_Module__diagsEnabled ti_sysbios_smp_LoggerBuf_Module__diagsEnabled__C;

/* Module__diagsIncluded */
typedef xdc_Bits32 CT__ti_sysbios_smp_LoggerBuf_Module__diagsIncluded;
__extern __FAR__ const CT__ti_sysbios_smp_LoggerBuf_Module__diagsIncluded ti_sysbios_smp_LoggerBuf_Module__diagsIncluded__C;

/* Module__diagsMask */
typedef xdc_Bits16* CT__ti_sysbios_smp_LoggerBuf_Module__diagsMask;
__extern __FAR__ const CT__ti_sysbios_smp_LoggerBuf_Module__diagsMask ti_sysbios_smp_LoggerBuf_Module__diagsMask__C;

/* Module__gateObj */
typedef xdc_Ptr CT__ti_sysbios_smp_LoggerBuf_Module__gateObj;
__extern __FAR__ const CT__ti_sysbios_smp_LoggerBuf_Module__gateObj ti_sysbios_smp_LoggerBuf_Module__gateObj__C;

/* Module__gatePrms */
typedef xdc_Ptr CT__ti_sysbios_smp_LoggerBuf_Module__gatePrms;
__extern __FAR__ const CT__ti_sysbios_smp_LoggerBuf_Module__gatePrms ti_sysbios_smp_LoggerBuf_Module__gatePrms__C;

/* Module__id */
typedef xdc_runtime_Types_ModuleId CT__ti_sysbios_smp_LoggerBuf_Module__id;
__extern __FAR__ const CT__ti_sysbios_smp_LoggerBuf_Module__id ti_sysbios_smp_LoggerBuf_Module__id__C;

/* Module__loggerDefined */
typedef xdc_Bool CT__ti_sysbios_smp_LoggerBuf_Module__loggerDefined;
__extern __FAR__ const CT__ti_sysbios_smp_LoggerBuf_Module__loggerDefined ti_sysbios_smp_LoggerBuf_Module__loggerDefined__C;

/* Module__loggerObj */
typedef xdc_Ptr CT__ti_sysbios_smp_LoggerBuf_Module__loggerObj;
__extern __FAR__ const CT__ti_sysbios_smp_LoggerBuf_Module__loggerObj ti_sysbios_smp_LoggerBuf_Module__loggerObj__C;

/* Module__loggerFxn0 */
typedef xdc_runtime_Types_LoggerFxn0 CT__ti_sysbios_smp_LoggerBuf_Module__loggerFxn0;
__extern __FAR__ const CT__ti_sysbios_smp_LoggerBuf_Module__loggerFxn0 ti_sysbios_smp_LoggerBuf_Module__loggerFxn0__C;

/* Module__loggerFxn1 */
typedef xdc_runtime_Types_LoggerFxn1 CT__ti_sysbios_smp_LoggerBuf_Module__loggerFxn1;
__extern __FAR__ const CT__ti_sysbios_smp_LoggerBuf_Module__loggerFxn1 ti_sysbios_smp_LoggerBuf_Module__loggerFxn1__C;

/* Module__loggerFxn2 */
typedef xdc_runtime_Types_LoggerFxn2 CT__ti_sysbios_smp_LoggerBuf_Module__loggerFxn2;
__extern __FAR__ const CT__ti_sysbios_smp_LoggerBuf_Module__loggerFxn2 ti_sysbios_smp_LoggerBuf_Module__loggerFxn2__C;

/* Module__loggerFxn4 */
typedef xdc_runtime_Types_LoggerFxn4 CT__ti_sysbios_smp_LoggerBuf_Module__loggerFxn4;
__extern __FAR__ const CT__ti_sysbios_smp_LoggerBuf_Module__loggerFxn4 ti_sysbios_smp_LoggerBuf_Module__loggerFxn4__C;

/* Module__loggerFxn8 */
typedef xdc_runtime_Types_LoggerFxn8 CT__ti_sysbios_smp_LoggerBuf_Module__loggerFxn8;
__extern __FAR__ const CT__ti_sysbios_smp_LoggerBuf_Module__loggerFxn8 ti_sysbios_smp_LoggerBuf_Module__loggerFxn8__C;

/* Module__startupDoneFxn */
typedef xdc_Bool (*CT__ti_sysbios_smp_LoggerBuf_Module__startupDoneFxn)(void);
__extern __FAR__ const CT__ti_sysbios_smp_LoggerBuf_Module__startupDoneFxn ti_sysbios_smp_LoggerBuf_Module__startupDoneFxn__C;

/* Object__count */
typedef xdc_Int CT__ti_sysbios_smp_LoggerBuf_Object__count;
__extern __FAR__ const CT__ti_sysbios_smp_LoggerBuf_Object__count ti_sysbios_smp_LoggerBuf_Object__count__C;

/* Object__heap */
typedef xdc_runtime_IHeap_Handle CT__ti_sysbios_smp_LoggerBuf_Object__heap;
__extern __FAR__ const CT__ti_sysbios_smp_LoggerBuf_Object__heap ti_sysbios_smp_LoggerBuf_Object__heap__C;

/* Object__sizeof */
typedef xdc_SizeT CT__ti_sysbios_smp_LoggerBuf_Object__sizeof;
__extern __FAR__ const CT__ti_sysbios_smp_LoggerBuf_Object__sizeof ti_sysbios_smp_LoggerBuf_Object__sizeof__C;

/* Object__table */
typedef xdc_Ptr CT__ti_sysbios_smp_LoggerBuf_Object__table;
__extern __FAR__ const CT__ti_sysbios_smp_LoggerBuf_Object__table ti_sysbios_smp_LoggerBuf_Object__table__C;

/* filterByLevel */
#ifdef ti_sysbios_smp_LoggerBuf_filterByLevel__D
#define ti_sysbios_smp_LoggerBuf_filterByLevel (ti_sysbios_smp_LoggerBuf_filterByLevel__D)
#else
#define ti_sysbios_smp_LoggerBuf_filterByLevel (ti_sysbios_smp_LoggerBuf_filterByLevel__C)
typedef xdc_Bool CT__ti_sysbios_smp_LoggerBuf_filterByLevel;
__extern __FAR__ const CT__ti_sysbios_smp_LoggerBuf_filterByLevel ti_sysbios_smp_LoggerBuf_filterByLevel__C;
#endif

/* E_badLevel */
#define ti_sysbios_smp_LoggerBuf_E_badLevel (ti_sysbios_smp_LoggerBuf_E_badLevel__C)
typedef xdc_runtime_Error_Id CT__ti_sysbios_smp_LoggerBuf_E_badLevel;
__extern __FAR__ const CT__ti_sysbios_smp_LoggerBuf_E_badLevel ti_sysbios_smp_LoggerBuf_E_badLevel__C;

/* enableFlush */
#ifdef ti_sysbios_smp_LoggerBuf_enableFlush__D
#define ti_sysbios_smp_LoggerBuf_enableFlush (ti_sysbios_smp_LoggerBuf_enableFlush__D)
#else
#define ti_sysbios_smp_LoggerBuf_enableFlush (ti_sysbios_smp_LoggerBuf_enableFlush__C)
typedef xdc_Bool CT__ti_sysbios_smp_LoggerBuf_enableFlush;
__extern __FAR__ const CT__ti_sysbios_smp_LoggerBuf_enableFlush ti_sysbios_smp_LoggerBuf_enableFlush__C;
#endif

/* statusLogger */
#define ti_sysbios_smp_LoggerBuf_statusLogger (ti_sysbios_smp_LoggerBuf_statusLogger__C)
typedef xdc_runtime_ILogger_Handle CT__ti_sysbios_smp_LoggerBuf_statusLogger;
__extern __FAR__ const CT__ti_sysbios_smp_LoggerBuf_statusLogger ti_sysbios_smp_LoggerBuf_statusLogger__C;

/* level1Mask */
#ifdef ti_sysbios_smp_LoggerBuf_level1Mask__D
#define ti_sysbios_smp_LoggerBuf_level1Mask (ti_sysbios_smp_LoggerBuf_level1Mask__D)
#else
#define ti_sysbios_smp_LoggerBuf_level1Mask (ti_sysbios_smp_LoggerBuf_level1Mask__C)
typedef xdc_runtime_Diags_Mask CT__ti_sysbios_smp_LoggerBuf_level1Mask;
__extern __FAR__ const CT__ti_sysbios_smp_LoggerBuf_level1Mask ti_sysbios_smp_LoggerBuf_level1Mask__C;
#endif

/* level2Mask */
#ifdef ti_sysbios_smp_LoggerBuf_level2Mask__D
#define ti_sysbios_smp_LoggerBuf_level2Mask (ti_sysbios_smp_LoggerBuf_level2Mask__D)
#else
#define ti_sysbios_smp_LoggerBuf_level2Mask (ti_sysbios_smp_LoggerBuf_level2Mask__C)
typedef xdc_runtime_Diags_Mask CT__ti_sysbios_smp_LoggerBuf_level2Mask;
__extern __FAR__ const CT__ti_sysbios_smp_LoggerBuf_level2Mask ti_sysbios_smp_LoggerBuf_level2Mask__C;
#endif

/* level3Mask */
#ifdef ti_sysbios_smp_LoggerBuf_level3Mask__D
#define ti_sysbios_smp_LoggerBuf_level3Mask (ti_sysbios_smp_LoggerBuf_level3Mask__D)
#else
#define ti_sysbios_smp_LoggerBuf_level3Mask (ti_sysbios_smp_LoggerBuf_level3Mask__C)
typedef xdc_runtime_Diags_Mask CT__ti_sysbios_smp_LoggerBuf_level3Mask;
__extern __FAR__ const CT__ti_sysbios_smp_LoggerBuf_level3Mask ti_sysbios_smp_LoggerBuf_level3Mask__C;
#endif

/* level4Mask */
#ifdef ti_sysbios_smp_LoggerBuf_level4Mask__D
#define ti_sysbios_smp_LoggerBuf_level4Mask (ti_sysbios_smp_LoggerBuf_level4Mask__D)
#else
#define ti_sysbios_smp_LoggerBuf_level4Mask (ti_sysbios_smp_LoggerBuf_level4Mask__C)
typedef xdc_runtime_Diags_Mask CT__ti_sysbios_smp_LoggerBuf_level4Mask;
__extern __FAR__ const CT__ti_sysbios_smp_LoggerBuf_level4Mask ti_sysbios_smp_LoggerBuf_level4Mask__C;
#endif


/*
 * ======== PER-INSTANCE TYPES ========
 */

/* Params */
struct ti_sysbios_smp_LoggerBuf_Params {
    size_t __size;
    const void* __self;
    void* __fxns;
    xdc_runtime_IInstance_Params* instance;
    xdc_Int numEntries;
    ti_sysbios_smp_LoggerBuf_BufType bufType;
    xdc_Bool exitFlush;
    xdc_runtime_IHeap_Handle bufHeap;
    xdc_runtime_IInstance_Params __iprms;
};

/* Struct */
struct ti_sysbios_smp_LoggerBuf_Struct {
    const ti_sysbios_smp_LoggerBuf_Fxns__* __fxns;
    xdc_runtime_IHeap_Handle __f0;
    __TA_ti_sysbios_smp_LoggerBuf_Instance_State__entryArr __f1;
    ti_sysbios_smp_LoggerBuf_Entry* __f2;
    ti_sysbios_smp_LoggerBuf_Entry* __f3;
    ti_sysbios_smp_LoggerBuf_Entry* __f4;
    xdc_Bits32 __f5;
    xdc_Int16 __f6;
    xdc_Int8 __f7;
    xdc_Bool __f8;
    xdc_Bool __f9;
    xdc_runtime_Types_CordAddr __name;
};


/*
 * ======== VIRTUAL FUNCTIONS ========
 */

/* Fxns__ */
struct ti_sysbios_smp_LoggerBuf_Fxns__ {
    xdc_runtime_Types_Base* __base;
    const xdc_runtime_Types_SysFxns2* __sysp;
    xdc_Bool (*enable)(ti_sysbios_smp_LoggerBuf_Handle);
    xdc_Bool (*disable)(ti_sysbios_smp_LoggerBuf_Handle);
    xdc_Void (*write0)(ti_sysbios_smp_LoggerBuf_Handle, xdc_runtime_Log_Event, xdc_runtime_Types_ModuleId);
    xdc_Void (*write1)(ti_sysbios_smp_LoggerBuf_Handle, xdc_runtime_Log_Event, xdc_runtime_Types_ModuleId, xdc_IArg);
    xdc_Void (*write2)(ti_sysbios_smp_LoggerBuf_Handle, xdc_runtime_Log_Event, xdc_runtime_Types_ModuleId, xdc_IArg, xdc_IArg);
    xdc_Void (*write4)(ti_sysbios_smp_LoggerBuf_Handle, xdc_runtime_Log_Event, xdc_runtime_Types_ModuleId, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg);
    xdc_Void (*write8)(ti_sysbios_smp_LoggerBuf_Handle, xdc_runtime_Log_Event, xdc_runtime_Types_ModuleId, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg);
    xdc_Void (*setFilterLevel)(ti_sysbios_smp_LoggerBuf_Handle, xdc_runtime_Diags_Mask, xdc_runtime_Diags_EventLevel);
    xdc_runtime_Diags_Mask (*getFilterLevel)(ti_sysbios_smp_LoggerBuf_Handle, xdc_runtime_Diags_EventLevel);
    xdc_runtime_Types_SysFxns2 __sfxns;
};

/* Module__FXNS__C */
__extern const ti_sysbios_smp_LoggerBuf_Fxns__ ti_sysbios_smp_LoggerBuf_Module__FXNS__C;


/*
 * ======== FUNCTION DECLARATIONS ========
 */

/* Module_startup */
#define ti_sysbios_smp_LoggerBuf_Module_startup ti_sysbios_smp_LoggerBuf_Module_startup__E
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_Module_startup__E, "ti_sysbios_smp_LoggerBuf_Module_startup")
__extern xdc_Int ti_sysbios_smp_LoggerBuf_Module_startup__E( xdc_Int state );
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_Module_startup__F, "ti_sysbios_smp_LoggerBuf_Module_startup")
__extern xdc_Int ti_sysbios_smp_LoggerBuf_Module_startup__F( xdc_Int state );
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_Module_startup__R, "ti_sysbios_smp_LoggerBuf_Module_startup")
__extern xdc_Int ti_sysbios_smp_LoggerBuf_Module_startup__R( xdc_Int state );

/* Instance_init__F */
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_Instance_init__F, "ti_sysbios_smp_LoggerBuf_Instance_init")
__extern int ti_sysbios_smp_LoggerBuf_Instance_init__F( ti_sysbios_smp_LoggerBuf_Object*, const ti_sysbios_smp_LoggerBuf_Params*, xdc_runtime_Error_Block* );

/* Instance_finalize__F */
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_Instance_finalize__F, "ti_sysbios_smp_LoggerBuf_Instance_finalize")
__extern void ti_sysbios_smp_LoggerBuf_Instance_finalize__F( ti_sysbios_smp_LoggerBuf_Object* , int );

/* Instance_init__R */
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_Instance_init__R, "ti_sysbios_smp_LoggerBuf_Instance_init")
__extern int ti_sysbios_smp_LoggerBuf_Instance_init__R( ti_sysbios_smp_LoggerBuf_Object*, const ti_sysbios_smp_LoggerBuf_Params*, xdc_runtime_Error_Block* );

/* Instance_finalize__R */
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_Instance_finalize__R, "ti_sysbios_smp_LoggerBuf_Instance_finalize")
__extern void ti_sysbios_smp_LoggerBuf_Instance_finalize__R( ti_sysbios_smp_LoggerBuf_Object* , int );

/* Handle__label__S */
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_Handle__label__S, "ti_sysbios_smp_LoggerBuf_Handle__label")
__extern xdc_runtime_Types_Label* ti_sysbios_smp_LoggerBuf_Handle__label__S( xdc_Ptr obj, xdc_runtime_Types_Label* lab );

/* Module__startupDone__S */
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_Module__startupDone__S, "ti_sysbios_smp_LoggerBuf_Module__startupDone")
__extern xdc_Bool ti_sysbios_smp_LoggerBuf_Module__startupDone__S( void );

/* Object__create__S */
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_Object__create__S, "ti_sysbios_smp_LoggerBuf_Object__create")
__extern xdc_Ptr ti_sysbios_smp_LoggerBuf_Object__create__S( xdc_Ptr __oa, xdc_SizeT __osz, xdc_Ptr __aa, const xdc_UChar* __pa, xdc_SizeT __psz, xdc_runtime_Error_Block* __eb );

/* Object__delete__S */
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_Object__delete__S, "ti_sysbios_smp_LoggerBuf_Object__delete")
__extern xdc_Void ti_sysbios_smp_LoggerBuf_Object__delete__S( xdc_Ptr instp );

/* Object__destruct__S */
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_Object__destruct__S, "ti_sysbios_smp_LoggerBuf_Object__destruct")
__extern xdc_Void ti_sysbios_smp_LoggerBuf_Object__destruct__S( xdc_Ptr objp );

/* Object__get__S */
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_Object__get__S, "ti_sysbios_smp_LoggerBuf_Object__get")
__extern xdc_Ptr ti_sysbios_smp_LoggerBuf_Object__get__S( xdc_Ptr oarr, xdc_Int i );

/* Object__first__S */
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_Object__first__S, "ti_sysbios_smp_LoggerBuf_Object__first")
__extern xdc_Ptr ti_sysbios_smp_LoggerBuf_Object__first__S( void );

/* Object__next__S */
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_Object__next__S, "ti_sysbios_smp_LoggerBuf_Object__next")
__extern xdc_Ptr ti_sysbios_smp_LoggerBuf_Object__next__S( xdc_Ptr obj );

/* Params__init__S */
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_Params__init__S, "ti_sysbios_smp_LoggerBuf_Params__init")
__extern xdc_Void ti_sysbios_smp_LoggerBuf_Params__init__S( xdc_Ptr dst, xdc_Ptr src, xdc_SizeT psz, xdc_SizeT isz );

/* enable__E */
#define ti_sysbios_smp_LoggerBuf_enable ti_sysbios_smp_LoggerBuf_enable__E
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_enable__E, "ti_sysbios_smp_LoggerBuf_enable")
__extern xdc_Bool ti_sysbios_smp_LoggerBuf_enable__E( ti_sysbios_smp_LoggerBuf_Handle __inst );
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_enable__F, "ti_sysbios_smp_LoggerBuf_enable")
__extern xdc_Bool ti_sysbios_smp_LoggerBuf_enable__F( ti_sysbios_smp_LoggerBuf_Object* __inst );
__extern xdc_Bool ti_sysbios_smp_LoggerBuf_enable__R( ti_sysbios_smp_LoggerBuf_Handle __inst );

/* disable__E */
#define ti_sysbios_smp_LoggerBuf_disable ti_sysbios_smp_LoggerBuf_disable__E
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_disable__E, "ti_sysbios_smp_LoggerBuf_disable")
__extern xdc_Bool ti_sysbios_smp_LoggerBuf_disable__E( ti_sysbios_smp_LoggerBuf_Handle __inst );
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_disable__F, "ti_sysbios_smp_LoggerBuf_disable")
__extern xdc_Bool ti_sysbios_smp_LoggerBuf_disable__F( ti_sysbios_smp_LoggerBuf_Object* __inst );
__extern xdc_Bool ti_sysbios_smp_LoggerBuf_disable__R( ti_sysbios_smp_LoggerBuf_Handle __inst );

/* write0__E */
#define ti_sysbios_smp_LoggerBuf_write0 ti_sysbios_smp_LoggerBuf_write0__E
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_write0__E, "ti_sysbios_smp_LoggerBuf_write0")
__extern xdc_Void ti_sysbios_smp_LoggerBuf_write0__E( ti_sysbios_smp_LoggerBuf_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid );
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_write0__F, "ti_sysbios_smp_LoggerBuf_write0")
__extern xdc_Void ti_sysbios_smp_LoggerBuf_write0__F( ti_sysbios_smp_LoggerBuf_Object* __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid );
__extern xdc_Void ti_sysbios_smp_LoggerBuf_write0__R( ti_sysbios_smp_LoggerBuf_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid );

/* write1__E */
#define ti_sysbios_smp_LoggerBuf_write1 ti_sysbios_smp_LoggerBuf_write1__E
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_write1__E, "ti_sysbios_smp_LoggerBuf_write1")
__extern xdc_Void ti_sysbios_smp_LoggerBuf_write1__E( ti_sysbios_smp_LoggerBuf_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1 );
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_write1__F, "ti_sysbios_smp_LoggerBuf_write1")
__extern xdc_Void ti_sysbios_smp_LoggerBuf_write1__F( ti_sysbios_smp_LoggerBuf_Object* __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1 );
__extern xdc_Void ti_sysbios_smp_LoggerBuf_write1__R( ti_sysbios_smp_LoggerBuf_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1 );

/* write2__E */
#define ti_sysbios_smp_LoggerBuf_write2 ti_sysbios_smp_LoggerBuf_write2__E
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_write2__E, "ti_sysbios_smp_LoggerBuf_write2")
__extern xdc_Void ti_sysbios_smp_LoggerBuf_write2__E( ti_sysbios_smp_LoggerBuf_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1, xdc_IArg a2 );
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_write2__F, "ti_sysbios_smp_LoggerBuf_write2")
__extern xdc_Void ti_sysbios_smp_LoggerBuf_write2__F( ti_sysbios_smp_LoggerBuf_Object* __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1, xdc_IArg a2 );
__extern xdc_Void ti_sysbios_smp_LoggerBuf_write2__R( ti_sysbios_smp_LoggerBuf_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1, xdc_IArg a2 );

/* write4__E */
#define ti_sysbios_smp_LoggerBuf_write4 ti_sysbios_smp_LoggerBuf_write4__E
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_write4__E, "ti_sysbios_smp_LoggerBuf_write4")
__extern xdc_Void ti_sysbios_smp_LoggerBuf_write4__E( ti_sysbios_smp_LoggerBuf_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1, xdc_IArg a2, xdc_IArg a3, xdc_IArg a4 );
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_write4__F, "ti_sysbios_smp_LoggerBuf_write4")
__extern xdc_Void ti_sysbios_smp_LoggerBuf_write4__F( ti_sysbios_smp_LoggerBuf_Object* __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1, xdc_IArg a2, xdc_IArg a3, xdc_IArg a4 );
__extern xdc_Void ti_sysbios_smp_LoggerBuf_write4__R( ti_sysbios_smp_LoggerBuf_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1, xdc_IArg a2, xdc_IArg a3, xdc_IArg a4 );

/* write8__E */
#define ti_sysbios_smp_LoggerBuf_write8 ti_sysbios_smp_LoggerBuf_write8__E
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_write8__E, "ti_sysbios_smp_LoggerBuf_write8")
__extern xdc_Void ti_sysbios_smp_LoggerBuf_write8__E( ti_sysbios_smp_LoggerBuf_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1, xdc_IArg a2, xdc_IArg a3, xdc_IArg a4, xdc_IArg a5, xdc_IArg a6, xdc_IArg a7, xdc_IArg a8 );
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_write8__F, "ti_sysbios_smp_LoggerBuf_write8")
__extern xdc_Void ti_sysbios_smp_LoggerBuf_write8__F( ti_sysbios_smp_LoggerBuf_Object* __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1, xdc_IArg a2, xdc_IArg a3, xdc_IArg a4, xdc_IArg a5, xdc_IArg a6, xdc_IArg a7, xdc_IArg a8 );
__extern xdc_Void ti_sysbios_smp_LoggerBuf_write8__R( ti_sysbios_smp_LoggerBuf_Handle __inst, xdc_runtime_Log_Event evt, xdc_runtime_Types_ModuleId mid, xdc_IArg a1, xdc_IArg a2, xdc_IArg a3, xdc_IArg a4, xdc_IArg a5, xdc_IArg a6, xdc_IArg a7, xdc_IArg a8 );

/* setFilterLevel__E */
#define ti_sysbios_smp_LoggerBuf_setFilterLevel ti_sysbios_smp_LoggerBuf_setFilterLevel__E
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_setFilterLevel__E, "ti_sysbios_smp_LoggerBuf_setFilterLevel")
__extern xdc_Void ti_sysbios_smp_LoggerBuf_setFilterLevel__E( ti_sysbios_smp_LoggerBuf_Handle __inst, xdc_runtime_Diags_Mask mask, xdc_runtime_Diags_EventLevel filterLevel );
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_setFilterLevel__F, "ti_sysbios_smp_LoggerBuf_setFilterLevel")
__extern xdc_Void ti_sysbios_smp_LoggerBuf_setFilterLevel__F( ti_sysbios_smp_LoggerBuf_Object* __inst, xdc_runtime_Diags_Mask mask, xdc_runtime_Diags_EventLevel filterLevel );
__extern xdc_Void ti_sysbios_smp_LoggerBuf_setFilterLevel__R( ti_sysbios_smp_LoggerBuf_Handle __inst, xdc_runtime_Diags_Mask mask, xdc_runtime_Diags_EventLevel filterLevel );

/* getFilterLevel__E */
#define ti_sysbios_smp_LoggerBuf_getFilterLevel ti_sysbios_smp_LoggerBuf_getFilterLevel__E
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_getFilterLevel__E, "ti_sysbios_smp_LoggerBuf_getFilterLevel")
__extern xdc_runtime_Diags_Mask ti_sysbios_smp_LoggerBuf_getFilterLevel__E( ti_sysbios_smp_LoggerBuf_Handle __inst, xdc_runtime_Diags_EventLevel level );
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_getFilterLevel__F, "ti_sysbios_smp_LoggerBuf_getFilterLevel")
__extern xdc_runtime_Diags_Mask ti_sysbios_smp_LoggerBuf_getFilterLevel__F( ti_sysbios_smp_LoggerBuf_Object* __inst, xdc_runtime_Diags_EventLevel level );
__extern xdc_runtime_Diags_Mask ti_sysbios_smp_LoggerBuf_getFilterLevel__R( ti_sysbios_smp_LoggerBuf_Handle __inst, xdc_runtime_Diags_EventLevel level );

/* flushAll__E */
#define ti_sysbios_smp_LoggerBuf_flushAll ti_sysbios_smp_LoggerBuf_flushAll__E
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_flushAll__E, "ti_sysbios_smp_LoggerBuf_flushAll")
__extern xdc_Void ti_sysbios_smp_LoggerBuf_flushAll__E( void );
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_flushAll__F, "ti_sysbios_smp_LoggerBuf_flushAll")
__extern xdc_Void ti_sysbios_smp_LoggerBuf_flushAll__F( void );
__extern xdc_Void ti_sysbios_smp_LoggerBuf_flushAll__R( void );

/* flushAllInternal__E */
#define ti_sysbios_smp_LoggerBuf_flushAllInternal ti_sysbios_smp_LoggerBuf_flushAllInternal__E
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_flushAllInternal__E, "ti_sysbios_smp_LoggerBuf_flushAllInternal")
__extern xdc_Void ti_sysbios_smp_LoggerBuf_flushAllInternal__E( xdc_Int stat );
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_flushAllInternal__F, "ti_sysbios_smp_LoggerBuf_flushAllInternal")
__extern xdc_Void ti_sysbios_smp_LoggerBuf_flushAllInternal__F( xdc_Int stat );
__extern xdc_Void ti_sysbios_smp_LoggerBuf_flushAllInternal__R( xdc_Int stat );

/* reset__E */
#define ti_sysbios_smp_LoggerBuf_reset ti_sysbios_smp_LoggerBuf_reset__E
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_reset__E, "ti_sysbios_smp_LoggerBuf_reset")
__extern xdc_Void ti_sysbios_smp_LoggerBuf_reset__E( ti_sysbios_smp_LoggerBuf_Handle __inst );
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_reset__F, "ti_sysbios_smp_LoggerBuf_reset")
__extern xdc_Void ti_sysbios_smp_LoggerBuf_reset__F( ti_sysbios_smp_LoggerBuf_Object* __inst );
__extern xdc_Void ti_sysbios_smp_LoggerBuf_reset__R( ti_sysbios_smp_LoggerBuf_Handle __inst );

/* flush__E */
#define ti_sysbios_smp_LoggerBuf_flush ti_sysbios_smp_LoggerBuf_flush__E
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_flush__E, "ti_sysbios_smp_LoggerBuf_flush")
__extern xdc_Void ti_sysbios_smp_LoggerBuf_flush__E( ti_sysbios_smp_LoggerBuf_Handle __inst );
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_flush__F, "ti_sysbios_smp_LoggerBuf_flush")
__extern xdc_Void ti_sysbios_smp_LoggerBuf_flush__F( ti_sysbios_smp_LoggerBuf_Object* __inst );
__extern xdc_Void ti_sysbios_smp_LoggerBuf_flush__R( ti_sysbios_smp_LoggerBuf_Handle __inst );

/* getNextEntry__E */
#define ti_sysbios_smp_LoggerBuf_getNextEntry ti_sysbios_smp_LoggerBuf_getNextEntry__E
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_getNextEntry__E, "ti_sysbios_smp_LoggerBuf_getNextEntry")
__extern xdc_Int ti_sysbios_smp_LoggerBuf_getNextEntry__E( ti_sysbios_smp_LoggerBuf_Handle __inst, xdc_runtime_Log_EventRec* evtRec );
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_getNextEntry__F, "ti_sysbios_smp_LoggerBuf_getNextEntry")
__extern xdc_Int ti_sysbios_smp_LoggerBuf_getNextEntry__F( ti_sysbios_smp_LoggerBuf_Object* __inst, xdc_runtime_Log_EventRec* evtRec );
__extern xdc_Int ti_sysbios_smp_LoggerBuf_getNextEntry__R( ti_sysbios_smp_LoggerBuf_Handle __inst, xdc_runtime_Log_EventRec* evtRec );

/* filterOutEvent__I */
#define ti_sysbios_smp_LoggerBuf_filterOutEvent ti_sysbios_smp_LoggerBuf_filterOutEvent__I
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_filterOutEvent__I, "ti_sysbios_smp_LoggerBuf_filterOutEvent")
__extern xdc_Bool ti_sysbios_smp_LoggerBuf_filterOutEvent__I( xdc_runtime_Diags_Mask mask );

/* instanceStartup__I */
#define ti_sysbios_smp_LoggerBuf_instanceStartup ti_sysbios_smp_LoggerBuf_instanceStartup__I
xdc__CODESECT(ti_sysbios_smp_LoggerBuf_instanceStartup__I, "ti_sysbios_smp_LoggerBuf_instanceStartup")
__extern xdc_Void ti_sysbios_smp_LoggerBuf_instanceStartup__I( ti_sysbios_smp_LoggerBuf_Object* obj );


/*
 * ======== FUNCTION SELECTORS ========
 */

/* enable_{FxnT,fxnP} */
typedef xdc_Bool (*ti_sysbios_smp_LoggerBuf_enable_FxnT)(void*);
static inline ti_sysbios_smp_LoggerBuf_enable_FxnT ti_sysbios_smp_LoggerBuf_enable_fxnP( void )
{
    return (ti_sysbios_smp_LoggerBuf_enable_FxnT)ti_sysbios_smp_LoggerBuf_enable; 
}

/* disable_{FxnT,fxnP} */
typedef xdc_Bool (*ti_sysbios_smp_LoggerBuf_disable_FxnT)(void*);
static inline ti_sysbios_smp_LoggerBuf_disable_FxnT ti_sysbios_smp_LoggerBuf_disable_fxnP( void )
{
    return (ti_sysbios_smp_LoggerBuf_disable_FxnT)ti_sysbios_smp_LoggerBuf_disable; 
}

/* write0_{FxnT,fxnP} */
typedef xdc_Void (*ti_sysbios_smp_LoggerBuf_write0_FxnT)(void*, xdc_runtime_Log_Event, xdc_runtime_Types_ModuleId);
static inline ti_sysbios_smp_LoggerBuf_write0_FxnT ti_sysbios_smp_LoggerBuf_write0_fxnP( void )
{
    return (ti_sysbios_smp_LoggerBuf_write0_FxnT)ti_sysbios_smp_LoggerBuf_write0; 
}

/* write1_{FxnT,fxnP} */
typedef xdc_Void (*ti_sysbios_smp_LoggerBuf_write1_FxnT)(void*, xdc_runtime_Log_Event, xdc_runtime_Types_ModuleId, xdc_IArg);
static inline ti_sysbios_smp_LoggerBuf_write1_FxnT ti_sysbios_smp_LoggerBuf_write1_fxnP( void )
{
    return (ti_sysbios_smp_LoggerBuf_write1_FxnT)ti_sysbios_smp_LoggerBuf_write1; 
}

/* write2_{FxnT,fxnP} */
typedef xdc_Void (*ti_sysbios_smp_LoggerBuf_write2_FxnT)(void*, xdc_runtime_Log_Event, xdc_runtime_Types_ModuleId, xdc_IArg, xdc_IArg);
static inline ti_sysbios_smp_LoggerBuf_write2_FxnT ti_sysbios_smp_LoggerBuf_write2_fxnP( void )
{
    return (ti_sysbios_smp_LoggerBuf_write2_FxnT)ti_sysbios_smp_LoggerBuf_write2; 
}

/* write4_{FxnT,fxnP} */
typedef xdc_Void (*ti_sysbios_smp_LoggerBuf_write4_FxnT)(void*, xdc_runtime_Log_Event, xdc_runtime_Types_ModuleId, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg);
static inline ti_sysbios_smp_LoggerBuf_write4_FxnT ti_sysbios_smp_LoggerBuf_write4_fxnP( void )
{
    return (ti_sysbios_smp_LoggerBuf_write4_FxnT)ti_sysbios_smp_LoggerBuf_write4; 
}

/* write8_{FxnT,fxnP} */
typedef xdc_Void (*ti_sysbios_smp_LoggerBuf_write8_FxnT)(void*, xdc_runtime_Log_Event, xdc_runtime_Types_ModuleId, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg, xdc_IArg);
static inline ti_sysbios_smp_LoggerBuf_write8_FxnT ti_sysbios_smp_LoggerBuf_write8_fxnP( void )
{
    return (ti_sysbios_smp_LoggerBuf_write8_FxnT)ti_sysbios_smp_LoggerBuf_write8; 
}

/* setFilterLevel_{FxnT,fxnP} */
typedef xdc_Void (*ti_sysbios_smp_LoggerBuf_setFilterLevel_FxnT)(void*, xdc_runtime_Diags_Mask, xdc_runtime_Diags_EventLevel);
static inline ti_sysbios_smp_LoggerBuf_setFilterLevel_FxnT ti_sysbios_smp_LoggerBuf_setFilterLevel_fxnP( void )
{
    return (ti_sysbios_smp_LoggerBuf_setFilterLevel_FxnT)ti_sysbios_smp_LoggerBuf_setFilterLevel; 
}

/* getFilterLevel_{FxnT,fxnP} */
typedef xdc_runtime_Diags_Mask (*ti_sysbios_smp_LoggerBuf_getFilterLevel_FxnT)(void*, xdc_runtime_Diags_EventLevel);
static inline ti_sysbios_smp_LoggerBuf_getFilterLevel_FxnT ti_sysbios_smp_LoggerBuf_getFilterLevel_fxnP( void )
{
    return (ti_sysbios_smp_LoggerBuf_getFilterLevel_FxnT)ti_sysbios_smp_LoggerBuf_getFilterLevel; 
}

/* reset_{FxnT,fxnP} */
typedef xdc_Void (*ti_sysbios_smp_LoggerBuf_reset_FxnT)(void*);
static inline ti_sysbios_smp_LoggerBuf_reset_FxnT ti_sysbios_smp_LoggerBuf_reset_fxnP( void )
{
    return (ti_sysbios_smp_LoggerBuf_reset_FxnT)ti_sysbios_smp_LoggerBuf_reset; 
}

/* flush_{FxnT,fxnP} */
typedef xdc_Void (*ti_sysbios_smp_LoggerBuf_flush_FxnT)(void*);
static inline ti_sysbios_smp_LoggerBuf_flush_FxnT ti_sysbios_smp_LoggerBuf_flush_fxnP( void )
{
    return (ti_sysbios_smp_LoggerBuf_flush_FxnT)ti_sysbios_smp_LoggerBuf_flush; 
}

/* getNextEntry_{FxnT,fxnP} */
typedef xdc_Int (*ti_sysbios_smp_LoggerBuf_getNextEntry_FxnT)(void*, xdc_runtime_Log_EventRec*);
static inline ti_sysbios_smp_LoggerBuf_getNextEntry_FxnT ti_sysbios_smp_LoggerBuf_getNextEntry_fxnP( void )
{
    return (ti_sysbios_smp_LoggerBuf_getNextEntry_FxnT)ti_sysbios_smp_LoggerBuf_getNextEntry; 
}


/*
 * ======== CONVERTORS ========
 */

/* Module_upCast */
static inline xdc_runtime_IFilterLogger_Module ti_sysbios_smp_LoggerBuf_Module_upCast( void )
{
    return (xdc_runtime_IFilterLogger_Module)&ti_sysbios_smp_LoggerBuf_Module__FXNS__C;
}

/* Module_to_xdc_runtime_IFilterLogger */
#define ti_sysbios_smp_LoggerBuf_Module_to_xdc_runtime_IFilterLogger ti_sysbios_smp_LoggerBuf_Module_upCast

/* Handle_upCast */
static inline xdc_runtime_IFilterLogger_Handle ti_sysbios_smp_LoggerBuf_Handle_upCast( ti_sysbios_smp_LoggerBuf_Handle i )
{
    return (xdc_runtime_IFilterLogger_Handle)i;
}

/* Handle_to_xdc_runtime_IFilterLogger */
#define ti_sysbios_smp_LoggerBuf_Handle_to_xdc_runtime_IFilterLogger ti_sysbios_smp_LoggerBuf_Handle_upCast

/* Handle_downCast */
static inline ti_sysbios_smp_LoggerBuf_Handle ti_sysbios_smp_LoggerBuf_Handle_downCast( xdc_runtime_IFilterLogger_Handle i )
{
    xdc_runtime_IFilterLogger_Handle i2 = (xdc_runtime_IFilterLogger_Handle)i;
    return (void*)i2->__fxns == (void*)&ti_sysbios_smp_LoggerBuf_Module__FXNS__C ? (ti_sysbios_smp_LoggerBuf_Handle)i : 0;
}

/* Handle_from_xdc_runtime_IFilterLogger */
#define ti_sysbios_smp_LoggerBuf_Handle_from_xdc_runtime_IFilterLogger ti_sysbios_smp_LoggerBuf_Handle_downCast

/* Module_upCast2 */
static inline xdc_runtime_ILogger_Module ti_sysbios_smp_LoggerBuf_Module_upCast2( void )
{
    return (xdc_runtime_ILogger_Module)&ti_sysbios_smp_LoggerBuf_Module__FXNS__C;
}

/* Module_to_xdc_runtime_ILogger */
#define ti_sysbios_smp_LoggerBuf_Module_to_xdc_runtime_ILogger ti_sysbios_smp_LoggerBuf_Module_upCast2

/* Handle_upCast2 */
static inline xdc_runtime_ILogger_Handle ti_sysbios_smp_LoggerBuf_Handle_upCast2( ti_sysbios_smp_LoggerBuf_Handle i )
{
    return (xdc_runtime_ILogger_Handle)i;
}

/* Handle_to_xdc_runtime_ILogger */
#define ti_sysbios_smp_LoggerBuf_Handle_to_xdc_runtime_ILogger ti_sysbios_smp_LoggerBuf_Handle_upCast2

/* Handle_downCast2 */
static inline ti_sysbios_smp_LoggerBuf_Handle ti_sysbios_smp_LoggerBuf_Handle_downCast2( xdc_runtime_ILogger_Handle i )
{
    xdc_runtime_ILogger_Handle i2 = (xdc_runtime_ILogger_Handle)i;
    return (void*)i2->__fxns == (void*)&ti_sysbios_smp_LoggerBuf_Module__FXNS__C ? (ti_sysbios_smp_LoggerBuf_Handle)i : 0;
}

/* Handle_from_xdc_runtime_ILogger */
#define ti_sysbios_smp_LoggerBuf_Handle_from_xdc_runtime_ILogger ti_sysbios_smp_LoggerBuf_Handle_downCast2


/*
 * ======== SYSTEM FUNCTIONS ========
 */

/* Module_startupDone */
#define ti_sysbios_smp_LoggerBuf_Module_startupDone() ti_sysbios_smp_LoggerBuf_Module__startupDone__S()

/* Object_heap */
#define ti_sysbios_smp_LoggerBuf_Object_heap() ti_sysbios_smp_LoggerBuf_Object__heap__C

/* Module_heap */
#define ti_sysbios_smp_LoggerBuf_Module_heap() ti_sysbios_smp_LoggerBuf_Object__heap__C

/* Module_id */
static inline CT__ti_sysbios_smp_LoggerBuf_Module__id ti_sysbios_smp_LoggerBuf_Module_id( void ) 
{
    return ti_sysbios_smp_LoggerBuf_Module__id__C;
}

/* Module_hasMask */
static inline xdc_Bool ti_sysbios_smp_LoggerBuf_Module_hasMask( void ) 
{
    return ti_sysbios_smp_LoggerBuf_Module__diagsMask__C != NULL;
}

/* Module_getMask */
static inline xdc_Bits16 ti_sysbios_smp_LoggerBuf_Module_getMask( void ) 
{
    return ti_sysbios_smp_LoggerBuf_Module__diagsMask__C != NULL ? *ti_sysbios_smp_LoggerBuf_Module__diagsMask__C : 0;
}

/* Module_setMask */
static inline xdc_Void ti_sysbios_smp_LoggerBuf_Module_setMask( xdc_Bits16 mask ) 
{
    if (ti_sysbios_smp_LoggerBuf_Module__diagsMask__C != NULL) *ti_sysbios_smp_LoggerBuf_Module__diagsMask__C = mask;
}

/* Params_init */
static inline void ti_sysbios_smp_LoggerBuf_Params_init( ti_sysbios_smp_LoggerBuf_Params* prms ) 
{
    if (prms) {
        ti_sysbios_smp_LoggerBuf_Params__init__S(prms, 0, sizeof(ti_sysbios_smp_LoggerBuf_Params), sizeof(xdc_runtime_IInstance_Params));
    }
}

/* Params_copy */
static inline void ti_sysbios_smp_LoggerBuf_Params_copy( ti_sysbios_smp_LoggerBuf_Params* dst, const ti_sysbios_smp_LoggerBuf_Params* src ) 
{
    if (dst) {
        ti_sysbios_smp_LoggerBuf_Params__init__S(dst, (xdc_Ptr)src, sizeof(ti_sysbios_smp_LoggerBuf_Params), sizeof(xdc_runtime_IInstance_Params));
    }
}

/* Object_count */
#define ti_sysbios_smp_LoggerBuf_Object_count() ti_sysbios_smp_LoggerBuf_Object__count__C

/* Object_sizeof */
#define ti_sysbios_smp_LoggerBuf_Object_sizeof() ti_sysbios_smp_LoggerBuf_Object__sizeof__C

/* Object_get */
static inline ti_sysbios_smp_LoggerBuf_Handle ti_sysbios_smp_LoggerBuf_Object_get( ti_sysbios_smp_LoggerBuf_Instance_State* oarr, int i ) 
{
    return (ti_sysbios_smp_LoggerBuf_Handle)ti_sysbios_smp_LoggerBuf_Object__get__S(oarr, i);
}

/* Object_first */
static inline ti_sysbios_smp_LoggerBuf_Handle ti_sysbios_smp_LoggerBuf_Object_first( void )
{
    return (ti_sysbios_smp_LoggerBuf_Handle)ti_sysbios_smp_LoggerBuf_Object__first__S();
}

/* Object_next */
static inline ti_sysbios_smp_LoggerBuf_Handle ti_sysbios_smp_LoggerBuf_Object_next( ti_sysbios_smp_LoggerBuf_Object* obj )
{
    return (ti_sysbios_smp_LoggerBuf_Handle)ti_sysbios_smp_LoggerBuf_Object__next__S(obj);
}

/* Handle_label */
static inline xdc_runtime_Types_Label* ti_sysbios_smp_LoggerBuf_Handle_label( ti_sysbios_smp_LoggerBuf_Handle inst, xdc_runtime_Types_Label* lab )
{
    return ti_sysbios_smp_LoggerBuf_Handle__label__S(inst, lab);
}

/* Handle_name */
static inline String ti_sysbios_smp_LoggerBuf_Handle_name( ti_sysbios_smp_LoggerBuf_Handle inst )
{
    xdc_runtime_Types_Label lab;
    return ti_sysbios_smp_LoggerBuf_Handle__label__S(inst, &lab)->iname;
}

/* create */
static inline ti_sysbios_smp_LoggerBuf_Handle ti_sysbios_smp_LoggerBuf_create( const ti_sysbios_smp_LoggerBuf_Params* __prms, xdc_runtime_Error_Block* __eb )
{
    return (ti_sysbios_smp_LoggerBuf_Handle)ti_sysbios_smp_LoggerBuf_Object__create__S(0, 0, 0, (const xdc_UChar*)__prms, sizeof(ti_sysbios_smp_LoggerBuf_Params), __eb);
}

/* construct */
static inline void ti_sysbios_smp_LoggerBuf_construct( ti_sysbios_smp_LoggerBuf_Struct* __obj, const ti_sysbios_smp_LoggerBuf_Params* __prms, xdc_runtime_Error_Block* __eb )
{
    ti_sysbios_smp_LoggerBuf_Object__create__S(__obj, sizeof (ti_sysbios_smp_LoggerBuf_Struct), 0, (const xdc_UChar*)__prms, sizeof(ti_sysbios_smp_LoggerBuf_Params), __eb);
}

/* delete */
static inline void ti_sysbios_smp_LoggerBuf_delete( ti_sysbios_smp_LoggerBuf_Handle* instp )
{
    ti_sysbios_smp_LoggerBuf_Object__delete__S(instp);
}

/* destruct */
static inline void ti_sysbios_smp_LoggerBuf_destruct( ti_sysbios_smp_LoggerBuf_Struct* obj )
{
    ti_sysbios_smp_LoggerBuf_Object__destruct__S(obj);
}

/* handle */
static inline ti_sysbios_smp_LoggerBuf_Handle ti_sysbios_smp_LoggerBuf_handle( ti_sysbios_smp_LoggerBuf_Struct* str )
{
    return (ti_sysbios_smp_LoggerBuf_Handle)str;
}

/* struct */
static inline ti_sysbios_smp_LoggerBuf_Struct* ti_sysbios_smp_LoggerBuf_struct( ti_sysbios_smp_LoggerBuf_Handle inst )
{
    return (ti_sysbios_smp_LoggerBuf_Struct*)inst;
}


/*
 * ======== EPILOGUE ========
 */

#ifdef ti_sysbios_smp_LoggerBuf__top__
#undef __nested__
#endif

#endif /* ti_sysbios_smp_LoggerBuf__include */


/*
 * ======== STATE STRUCTURES ========
 */

#if defined(__config__) || (!defined(__nested__) && defined(ti_sysbios_smp_LoggerBuf__internalaccess))

#ifndef ti_sysbios_smp_LoggerBuf__include_state
#define ti_sysbios_smp_LoggerBuf__include_state

/* Module_State */
struct ti_sysbios_smp_LoggerBuf_Module_State {
    xdc_runtime_Diags_Mask level1;
    xdc_runtime_Diags_Mask level2;
    xdc_runtime_Diags_Mask level3;
};

/* Module__state__V */
extern struct ti_sysbios_smp_LoggerBuf_Module_State__ ti_sysbios_smp_LoggerBuf_Module__state__V;

/* Object */
struct ti_sysbios_smp_LoggerBuf_Object {
    const ti_sysbios_smp_LoggerBuf_Fxns__* __fxns;
    xdc_runtime_IHeap_Handle bufHeap;
    __TA_ti_sysbios_smp_LoggerBuf_Instance_State__entryArr entryArr;
    ti_sysbios_smp_LoggerBuf_Entry* curEntry;
    ti_sysbios_smp_LoggerBuf_Entry* endEntry;
    ti_sysbios_smp_LoggerBuf_Entry* readEntry;
    xdc_Bits32 serial;
    xdc_Int16 numEntries;
    xdc_Int8 advance;
    xdc_Bool enabled;
    xdc_Bool flush;
};

#endif /* ti_sysbios_smp_LoggerBuf__include_state */

#endif


/*
 * ======== PREFIX ALIASES ========
 */

#if !defined(__nested__) && !defined(ti_sysbios_smp_LoggerBuf__nolocalnames)

#ifndef ti_sysbios_smp_LoggerBuf__localnames__done
#define ti_sysbios_smp_LoggerBuf__localnames__done

/* module prefix */
#define LoggerBuf_Instance ti_sysbios_smp_LoggerBuf_Instance
#define LoggerBuf_Handle ti_sysbios_smp_LoggerBuf_Handle
#define LoggerBuf_Module ti_sysbios_smp_LoggerBuf_Module
#define LoggerBuf_Object ti_sysbios_smp_LoggerBuf_Object
#define LoggerBuf_Struct ti_sysbios_smp_LoggerBuf_Struct
#define LoggerBuf_BufType ti_sysbios_smp_LoggerBuf_BufType
#define LoggerBuf_FULL ti_sysbios_smp_LoggerBuf_FULL
#define LoggerBuf_WRAP ti_sysbios_smp_LoggerBuf_WRAP
#define LoggerBuf_NEXT ti_sysbios_smp_LoggerBuf_NEXT
#define LoggerBuf_Entry ti_sysbios_smp_LoggerBuf_Entry
#define LoggerBuf_Module_State ti_sysbios_smp_LoggerBuf_Module_State
#define LoggerBuf_Instance_State ti_sysbios_smp_LoggerBuf_Instance_State
#define LoggerBuf_BufType_CIRCULAR ti_sysbios_smp_LoggerBuf_BufType_CIRCULAR
#define LoggerBuf_BufType_FIXED ti_sysbios_smp_LoggerBuf_BufType_FIXED
#define LoggerBuf_filterByLevel ti_sysbios_smp_LoggerBuf_filterByLevel
#define LoggerBuf_E_badLevel ti_sysbios_smp_LoggerBuf_E_badLevel
#define LoggerBuf_enableFlush ti_sysbios_smp_LoggerBuf_enableFlush
#define LoggerBuf_statusLogger ti_sysbios_smp_LoggerBuf_statusLogger
#define LoggerBuf_level1Mask ti_sysbios_smp_LoggerBuf_level1Mask
#define LoggerBuf_level2Mask ti_sysbios_smp_LoggerBuf_level2Mask
#define LoggerBuf_level3Mask ti_sysbios_smp_LoggerBuf_level3Mask
#define LoggerBuf_level4Mask ti_sysbios_smp_LoggerBuf_level4Mask
#define LoggerBuf_Params ti_sysbios_smp_LoggerBuf_Params
#define LoggerBuf_enable ti_sysbios_smp_LoggerBuf_enable
#define LoggerBuf_enable_fxnP ti_sysbios_smp_LoggerBuf_enable_fxnP
#define LoggerBuf_enable_FxnT ti_sysbios_smp_LoggerBuf_enable_FxnT
#define LoggerBuf_disable ti_sysbios_smp_LoggerBuf_disable
#define LoggerBuf_disable_fxnP ti_sysbios_smp_LoggerBuf_disable_fxnP
#define LoggerBuf_disable_FxnT ti_sysbios_smp_LoggerBuf_disable_FxnT
#define LoggerBuf_write0 ti_sysbios_smp_LoggerBuf_write0
#define LoggerBuf_write0_fxnP ti_sysbios_smp_LoggerBuf_write0_fxnP
#define LoggerBuf_write0_FxnT ti_sysbios_smp_LoggerBuf_write0_FxnT
#define LoggerBuf_write1 ti_sysbios_smp_LoggerBuf_write1
#define LoggerBuf_write1_fxnP ti_sysbios_smp_LoggerBuf_write1_fxnP
#define LoggerBuf_write1_FxnT ti_sysbios_smp_LoggerBuf_write1_FxnT
#define LoggerBuf_write2 ti_sysbios_smp_LoggerBuf_write2
#define LoggerBuf_write2_fxnP ti_sysbios_smp_LoggerBuf_write2_fxnP
#define LoggerBuf_write2_FxnT ti_sysbios_smp_LoggerBuf_write2_FxnT
#define LoggerBuf_write4 ti_sysbios_smp_LoggerBuf_write4
#define LoggerBuf_write4_fxnP ti_sysbios_smp_LoggerBuf_write4_fxnP
#define LoggerBuf_write4_FxnT ti_sysbios_smp_LoggerBuf_write4_FxnT
#define LoggerBuf_write8 ti_sysbios_smp_LoggerBuf_write8
#define LoggerBuf_write8_fxnP ti_sysbios_smp_LoggerBuf_write8_fxnP
#define LoggerBuf_write8_FxnT ti_sysbios_smp_LoggerBuf_write8_FxnT
#define LoggerBuf_setFilterLevel ti_sysbios_smp_LoggerBuf_setFilterLevel
#define LoggerBuf_setFilterLevel_fxnP ti_sysbios_smp_LoggerBuf_setFilterLevel_fxnP
#define LoggerBuf_setFilterLevel_FxnT ti_sysbios_smp_LoggerBuf_setFilterLevel_FxnT
#define LoggerBuf_getFilterLevel ti_sysbios_smp_LoggerBuf_getFilterLevel
#define LoggerBuf_getFilterLevel_fxnP ti_sysbios_smp_LoggerBuf_getFilterLevel_fxnP
#define LoggerBuf_getFilterLevel_FxnT ti_sysbios_smp_LoggerBuf_getFilterLevel_FxnT
#define LoggerBuf_flushAll ti_sysbios_smp_LoggerBuf_flushAll
#define LoggerBuf_flushAllInternal ti_sysbios_smp_LoggerBuf_flushAllInternal
#define LoggerBuf_reset ti_sysbios_smp_LoggerBuf_reset
#define LoggerBuf_reset_fxnP ti_sysbios_smp_LoggerBuf_reset_fxnP
#define LoggerBuf_reset_FxnT ti_sysbios_smp_LoggerBuf_reset_FxnT
#define LoggerBuf_flush ti_sysbios_smp_LoggerBuf_flush
#define LoggerBuf_flush_fxnP ti_sysbios_smp_LoggerBuf_flush_fxnP
#define LoggerBuf_flush_FxnT ti_sysbios_smp_LoggerBuf_flush_FxnT
#define LoggerBuf_getNextEntry ti_sysbios_smp_LoggerBuf_getNextEntry
#define LoggerBuf_getNextEntry_fxnP ti_sysbios_smp_LoggerBuf_getNextEntry_fxnP
#define LoggerBuf_getNextEntry_FxnT ti_sysbios_smp_LoggerBuf_getNextEntry_FxnT
#define LoggerBuf_Module_name ti_sysbios_smp_LoggerBuf_Module_name
#define LoggerBuf_Module_id ti_sysbios_smp_LoggerBuf_Module_id
#define LoggerBuf_Module_startup ti_sysbios_smp_LoggerBuf_Module_startup
#define LoggerBuf_Module_startupDone ti_sysbios_smp_LoggerBuf_Module_startupDone
#define LoggerBuf_Module_hasMask ti_sysbios_smp_LoggerBuf_Module_hasMask
#define LoggerBuf_Module_getMask ti_sysbios_smp_LoggerBuf_Module_getMask
#define LoggerBuf_Module_setMask ti_sysbios_smp_LoggerBuf_Module_setMask
#define LoggerBuf_Object_heap ti_sysbios_smp_LoggerBuf_Object_heap
#define LoggerBuf_Module_heap ti_sysbios_smp_LoggerBuf_Module_heap
#define LoggerBuf_construct ti_sysbios_smp_LoggerBuf_construct
#define LoggerBuf_create ti_sysbios_smp_LoggerBuf_create
#define LoggerBuf_handle ti_sysbios_smp_LoggerBuf_handle
#define LoggerBuf_struct ti_sysbios_smp_LoggerBuf_struct
#define LoggerBuf_Handle_label ti_sysbios_smp_LoggerBuf_Handle_label
#define LoggerBuf_Handle_name ti_sysbios_smp_LoggerBuf_Handle_name
#define LoggerBuf_Instance_init ti_sysbios_smp_LoggerBuf_Instance_init
#define LoggerBuf_Object_count ti_sysbios_smp_LoggerBuf_Object_count
#define LoggerBuf_Object_get ti_sysbios_smp_LoggerBuf_Object_get
#define LoggerBuf_Object_first ti_sysbios_smp_LoggerBuf_Object_first
#define LoggerBuf_Object_next ti_sysbios_smp_LoggerBuf_Object_next
#define LoggerBuf_Object_sizeof ti_sysbios_smp_LoggerBuf_Object_sizeof
#define LoggerBuf_Params_copy ti_sysbios_smp_LoggerBuf_Params_copy
#define LoggerBuf_Params_init ti_sysbios_smp_LoggerBuf_Params_init
#define LoggerBuf_Instance_finalize ti_sysbios_smp_LoggerBuf_Instance_finalize
#define LoggerBuf_delete ti_sysbios_smp_LoggerBuf_delete
#define LoggerBuf_destruct ti_sysbios_smp_LoggerBuf_destruct
#define LoggerBuf_Module_upCast ti_sysbios_smp_LoggerBuf_Module_upCast
#define LoggerBuf_Module_to_xdc_runtime_IFilterLogger ti_sysbios_smp_LoggerBuf_Module_to_xdc_runtime_IFilterLogger
#define LoggerBuf_Handle_upCast ti_sysbios_smp_LoggerBuf_Handle_upCast
#define LoggerBuf_Handle_to_xdc_runtime_IFilterLogger ti_sysbios_smp_LoggerBuf_Handle_to_xdc_runtime_IFilterLogger
#define LoggerBuf_Handle_downCast ti_sysbios_smp_LoggerBuf_Handle_downCast
#define LoggerBuf_Handle_from_xdc_runtime_IFilterLogger ti_sysbios_smp_LoggerBuf_Handle_from_xdc_runtime_IFilterLogger
#define LoggerBuf_Module_upCast2 ti_sysbios_smp_LoggerBuf_Module_upCast2
#define LoggerBuf_Module_to_xdc_runtime_ILogger ti_sysbios_smp_LoggerBuf_Module_to_xdc_runtime_ILogger
#define LoggerBuf_Handle_upCast2 ti_sysbios_smp_LoggerBuf_Handle_upCast2
#define LoggerBuf_Handle_to_xdc_runtime_ILogger ti_sysbios_smp_LoggerBuf_Handle_to_xdc_runtime_ILogger
#define LoggerBuf_Handle_downCast2 ti_sysbios_smp_LoggerBuf_Handle_downCast2
#define LoggerBuf_Handle_from_xdc_runtime_ILogger ti_sysbios_smp_LoggerBuf_Handle_from_xdc_runtime_ILogger

/* proxies */
#include <ti/sysbios/smp/package/LoggerBuf_TimestampProxy.h>

/* proxies */
#include <ti/sysbios/smp/package/LoggerBuf_Module_GateProxy.h>

#endif /* ti_sysbios_smp_LoggerBuf__localnames__done */
#endif
