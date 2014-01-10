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

#ifndef ti_sysbios_family_arm_exc_Exception__include
#define ti_sysbios_family_arm_exc_Exception__include

#ifndef __nested__
#define __nested__
#define ti_sysbios_family_arm_exc_Exception__top__
#endif

#ifdef __cplusplus
#define __extern extern "C"
#else
#define __extern extern
#endif

#define ti_sysbios_family_arm_exc_Exception___VERS 150


/*
 * ======== INCLUDES ========
 */

#include <xdc/std.h>

#include <xdc/runtime/xdc.h>
#include <xdc/runtime/Types.h>
#include <ti/sysbios/family/arm/exc/package/package.defs.h>

#include <xdc/runtime/IModule.h>
#include <ti/sysbios/BIOS.h>
#include <xdc/runtime/Error.h>


/*
 * ======== AUXILIARY DEFINITIONS ========
 */

/* ExceptionHookFuncPtr */
typedef xdc_Void (*ti_sysbios_family_arm_exc_Exception_ExceptionHookFuncPtr)(ti_sysbios_family_arm_exc_Exception_ExcContext*);

/* Type */
enum ti_sysbios_family_arm_exc_Exception_Type {
    ti_sysbios_family_arm_exc_Exception_Type_Supervisor = 0x13,
    ti_sysbios_family_arm_exc_Exception_Type_PreAbort = 0x17,
    ti_sysbios_family_arm_exc_Exception_Type_DataAbort = 0x18,
    ti_sysbios_family_arm_exc_Exception_Type_UndefInst = 0x1b
};
typedef enum ti_sysbios_family_arm_exc_Exception_Type ti_sysbios_family_arm_exc_Exception_Type;

/* ExcContext */
struct ti_sysbios_family_arm_exc_Exception_ExcContext {
    ti_sysbios_BIOS_ThreadType threadType;
    xdc_Ptr threadHandle;
    xdc_Ptr threadStack;
    xdc_SizeT threadStackSize;
    ti_sysbios_family_arm_exc_Exception_Type type;
    xdc_Ptr r0;
    xdc_Ptr r1;
    xdc_Ptr r2;
    xdc_Ptr r3;
    xdc_Ptr r4;
    xdc_Ptr r5;
    xdc_Ptr r6;
    xdc_Ptr r7;
    xdc_Ptr r8;
    xdc_Ptr r9;
    xdc_Ptr r10;
    xdc_Ptr r11;
    xdc_Ptr r12;
    xdc_Ptr sp;
    xdc_Ptr lr;
    xdc_Ptr pc;
    xdc_Ptr psr;
};


/*
 * ======== INTERNAL DEFINITIONS ========
 */

/* Module_State */
typedef xdc_Char __T1_ti_sysbios_family_arm_exc_Exception_Module_State__excStack;
typedef xdc_Char *__ARRAY1_ti_sysbios_family_arm_exc_Exception_Module_State__excStack;
typedef __ARRAY1_ti_sysbios_family_arm_exc_Exception_Module_State__excStack __TA_ti_sysbios_family_arm_exc_Exception_Module_State__excStack;


/*
 * ======== MODULE-WIDE CONFIGS ========
 */

/* Module__diagsEnabled */
typedef xdc_Bits32 CT__ti_sysbios_family_arm_exc_Exception_Module__diagsEnabled;
__extern __FAR__ const CT__ti_sysbios_family_arm_exc_Exception_Module__diagsEnabled ti_sysbios_family_arm_exc_Exception_Module__diagsEnabled__C;

/* Module__diagsIncluded */
typedef xdc_Bits32 CT__ti_sysbios_family_arm_exc_Exception_Module__diagsIncluded;
__extern __FAR__ const CT__ti_sysbios_family_arm_exc_Exception_Module__diagsIncluded ti_sysbios_family_arm_exc_Exception_Module__diagsIncluded__C;

/* Module__diagsMask */
typedef xdc_Bits16* CT__ti_sysbios_family_arm_exc_Exception_Module__diagsMask;
__extern __FAR__ const CT__ti_sysbios_family_arm_exc_Exception_Module__diagsMask ti_sysbios_family_arm_exc_Exception_Module__diagsMask__C;

/* Module__gateObj */
typedef xdc_Ptr CT__ti_sysbios_family_arm_exc_Exception_Module__gateObj;
__extern __FAR__ const CT__ti_sysbios_family_arm_exc_Exception_Module__gateObj ti_sysbios_family_arm_exc_Exception_Module__gateObj__C;

/* Module__gatePrms */
typedef xdc_Ptr CT__ti_sysbios_family_arm_exc_Exception_Module__gatePrms;
__extern __FAR__ const CT__ti_sysbios_family_arm_exc_Exception_Module__gatePrms ti_sysbios_family_arm_exc_Exception_Module__gatePrms__C;

/* Module__id */
typedef xdc_runtime_Types_ModuleId CT__ti_sysbios_family_arm_exc_Exception_Module__id;
__extern __FAR__ const CT__ti_sysbios_family_arm_exc_Exception_Module__id ti_sysbios_family_arm_exc_Exception_Module__id__C;

/* Module__loggerDefined */
typedef xdc_Bool CT__ti_sysbios_family_arm_exc_Exception_Module__loggerDefined;
__extern __FAR__ const CT__ti_sysbios_family_arm_exc_Exception_Module__loggerDefined ti_sysbios_family_arm_exc_Exception_Module__loggerDefined__C;

/* Module__loggerObj */
typedef xdc_Ptr CT__ti_sysbios_family_arm_exc_Exception_Module__loggerObj;
__extern __FAR__ const CT__ti_sysbios_family_arm_exc_Exception_Module__loggerObj ti_sysbios_family_arm_exc_Exception_Module__loggerObj__C;

/* Module__loggerFxn0 */
typedef xdc_runtime_Types_LoggerFxn0 CT__ti_sysbios_family_arm_exc_Exception_Module__loggerFxn0;
__extern __FAR__ const CT__ti_sysbios_family_arm_exc_Exception_Module__loggerFxn0 ti_sysbios_family_arm_exc_Exception_Module__loggerFxn0__C;

/* Module__loggerFxn1 */
typedef xdc_runtime_Types_LoggerFxn1 CT__ti_sysbios_family_arm_exc_Exception_Module__loggerFxn1;
__extern __FAR__ const CT__ti_sysbios_family_arm_exc_Exception_Module__loggerFxn1 ti_sysbios_family_arm_exc_Exception_Module__loggerFxn1__C;

/* Module__loggerFxn2 */
typedef xdc_runtime_Types_LoggerFxn2 CT__ti_sysbios_family_arm_exc_Exception_Module__loggerFxn2;
__extern __FAR__ const CT__ti_sysbios_family_arm_exc_Exception_Module__loggerFxn2 ti_sysbios_family_arm_exc_Exception_Module__loggerFxn2__C;

/* Module__loggerFxn4 */
typedef xdc_runtime_Types_LoggerFxn4 CT__ti_sysbios_family_arm_exc_Exception_Module__loggerFxn4;
__extern __FAR__ const CT__ti_sysbios_family_arm_exc_Exception_Module__loggerFxn4 ti_sysbios_family_arm_exc_Exception_Module__loggerFxn4__C;

/* Module__loggerFxn8 */
typedef xdc_runtime_Types_LoggerFxn8 CT__ti_sysbios_family_arm_exc_Exception_Module__loggerFxn8;
__extern __FAR__ const CT__ti_sysbios_family_arm_exc_Exception_Module__loggerFxn8 ti_sysbios_family_arm_exc_Exception_Module__loggerFxn8__C;

/* Module__startupDoneFxn */
typedef xdc_Bool (*CT__ti_sysbios_family_arm_exc_Exception_Module__startupDoneFxn)(void);
__extern __FAR__ const CT__ti_sysbios_family_arm_exc_Exception_Module__startupDoneFxn ti_sysbios_family_arm_exc_Exception_Module__startupDoneFxn__C;

/* Object__count */
typedef xdc_Int CT__ti_sysbios_family_arm_exc_Exception_Object__count;
__extern __FAR__ const CT__ti_sysbios_family_arm_exc_Exception_Object__count ti_sysbios_family_arm_exc_Exception_Object__count__C;

/* Object__heap */
typedef xdc_runtime_IHeap_Handle CT__ti_sysbios_family_arm_exc_Exception_Object__heap;
__extern __FAR__ const CT__ti_sysbios_family_arm_exc_Exception_Object__heap ti_sysbios_family_arm_exc_Exception_Object__heap__C;

/* Object__sizeof */
typedef xdc_SizeT CT__ti_sysbios_family_arm_exc_Exception_Object__sizeof;
__extern __FAR__ const CT__ti_sysbios_family_arm_exc_Exception_Object__sizeof ti_sysbios_family_arm_exc_Exception_Object__sizeof__C;

/* Object__table */
typedef xdc_Ptr CT__ti_sysbios_family_arm_exc_Exception_Object__table;
__extern __FAR__ const CT__ti_sysbios_family_arm_exc_Exception_Object__table ti_sysbios_family_arm_exc_Exception_Object__table__C;

/* E_swi */
#define ti_sysbios_family_arm_exc_Exception_E_swi (ti_sysbios_family_arm_exc_Exception_E_swi__C)
typedef xdc_runtime_Error_Id CT__ti_sysbios_family_arm_exc_Exception_E_swi;
__extern __FAR__ const CT__ti_sysbios_family_arm_exc_Exception_E_swi ti_sysbios_family_arm_exc_Exception_E_swi__C;

/* E_prefetchAbort */
#define ti_sysbios_family_arm_exc_Exception_E_prefetchAbort (ti_sysbios_family_arm_exc_Exception_E_prefetchAbort__C)
typedef xdc_runtime_Error_Id CT__ti_sysbios_family_arm_exc_Exception_E_prefetchAbort;
__extern __FAR__ const CT__ti_sysbios_family_arm_exc_Exception_E_prefetchAbort ti_sysbios_family_arm_exc_Exception_E_prefetchAbort__C;

/* E_dataAbort */
#define ti_sysbios_family_arm_exc_Exception_E_dataAbort (ti_sysbios_family_arm_exc_Exception_E_dataAbort__C)
typedef xdc_runtime_Error_Id CT__ti_sysbios_family_arm_exc_Exception_E_dataAbort;
__extern __FAR__ const CT__ti_sysbios_family_arm_exc_Exception_E_dataAbort ti_sysbios_family_arm_exc_Exception_E_dataAbort__C;

/* E_undefinedInstruction */
#define ti_sysbios_family_arm_exc_Exception_E_undefinedInstruction (ti_sysbios_family_arm_exc_Exception_E_undefinedInstruction__C)
typedef xdc_runtime_Error_Id CT__ti_sysbios_family_arm_exc_Exception_E_undefinedInstruction;
__extern __FAR__ const CT__ti_sysbios_family_arm_exc_Exception_E_undefinedInstruction ti_sysbios_family_arm_exc_Exception_E_undefinedInstruction__C;

/* enableDecode */
#ifdef ti_sysbios_family_arm_exc_Exception_enableDecode__D
#define ti_sysbios_family_arm_exc_Exception_enableDecode (ti_sysbios_family_arm_exc_Exception_enableDecode__D)
#else
#define ti_sysbios_family_arm_exc_Exception_enableDecode (ti_sysbios_family_arm_exc_Exception_enableDecode__C)
typedef xdc_Bool CT__ti_sysbios_family_arm_exc_Exception_enableDecode;
__extern __FAR__ const CT__ti_sysbios_family_arm_exc_Exception_enableDecode ti_sysbios_family_arm_exc_Exception_enableDecode__C;
#endif

/* excStack */
#define ti_sysbios_family_arm_exc_Exception_excStack (ti_sysbios_family_arm_exc_Exception_excStack__C)
typedef xdc_Ptr CT__ti_sysbios_family_arm_exc_Exception_excStack;
__extern __FAR__ const CT__ti_sysbios_family_arm_exc_Exception_excStack ti_sysbios_family_arm_exc_Exception_excStack__C;

/* excHookFunc */
#define ti_sysbios_family_arm_exc_Exception_excHookFunc (ti_sysbios_family_arm_exc_Exception_excHookFunc__C)
typedef ti_sysbios_family_arm_exc_Exception_ExceptionHookFuncPtr CT__ti_sysbios_family_arm_exc_Exception_excHookFunc;
__extern __FAR__ const CT__ti_sysbios_family_arm_exc_Exception_excHookFunc ti_sysbios_family_arm_exc_Exception_excHookFunc__C;


/*
 * ======== FUNCTION DECLARATIONS ========
 */

/* Module_startup */
#define ti_sysbios_family_arm_exc_Exception_Module_startup ti_sysbios_family_arm_exc_Exception_Module_startup__E
xdc__CODESECT(ti_sysbios_family_arm_exc_Exception_Module_startup__E, "ti_sysbios_family_arm_exc_Exception_Module_startup")
__extern xdc_Int ti_sysbios_family_arm_exc_Exception_Module_startup__E( xdc_Int state );
xdc__CODESECT(ti_sysbios_family_arm_exc_Exception_Module_startup__F, "ti_sysbios_family_arm_exc_Exception_Module_startup")
__extern xdc_Int ti_sysbios_family_arm_exc_Exception_Module_startup__F( xdc_Int state );
xdc__CODESECT(ti_sysbios_family_arm_exc_Exception_Module_startup__R, "ti_sysbios_family_arm_exc_Exception_Module_startup")
__extern xdc_Int ti_sysbios_family_arm_exc_Exception_Module_startup__R( xdc_Int state );

/* Module__startupDone__S */
xdc__CODESECT(ti_sysbios_family_arm_exc_Exception_Module__startupDone__S, "ti_sysbios_family_arm_exc_Exception_Module__startupDone")
__extern xdc_Bool ti_sysbios_family_arm_exc_Exception_Module__startupDone__S( void );

/* init__I */
#define ti_sysbios_family_arm_exc_Exception_init ti_sysbios_family_arm_exc_Exception_init__I
xdc__CODESECT(ti_sysbios_family_arm_exc_Exception_init__I, "ti_sysbios_family_arm_exc_Exception_init")
__extern xdc_Void ti_sysbios_family_arm_exc_Exception_init__I( void );

/* excHandlerAsm__I */
#define ti_sysbios_family_arm_exc_Exception_excHandlerAsm ti_sysbios_family_arm_exc_Exception_excHandlerAsm__I
xdc__CODESECT(ti_sysbios_family_arm_exc_Exception_excHandlerAsm__I, "ti_sysbios_family_arm_exc_Exception_excHandlerAsm")
__extern xdc_Void ti_sysbios_family_arm_exc_Exception_excHandlerAsm__I( void );

/* excHandlerDataAsm__I */
#define ti_sysbios_family_arm_exc_Exception_excHandlerDataAsm ti_sysbios_family_arm_exc_Exception_excHandlerDataAsm__I
xdc__CODESECT(ti_sysbios_family_arm_exc_Exception_excHandlerDataAsm__I, "ti_sysbios_family_arm_exc_Exception_excHandlerDataAsm")
__extern xdc_Void ti_sysbios_family_arm_exc_Exception_excHandlerDataAsm__I( void );

/* excHandler__I */
#define ti_sysbios_family_arm_exc_Exception_excHandler ti_sysbios_family_arm_exc_Exception_excHandler__I
xdc__CODESECT(ti_sysbios_family_arm_exc_Exception_excHandler__I, "ti_sysbios_family_arm_exc_Exception_excHandler")
__extern xdc_Void ti_sysbios_family_arm_exc_Exception_excHandler__I( xdc_UInt* excStack, xdc_UInt lr );

/* excDumpContext__I */
#define ti_sysbios_family_arm_exc_Exception_excDumpContext ti_sysbios_family_arm_exc_Exception_excDumpContext__I
xdc__CODESECT(ti_sysbios_family_arm_exc_Exception_excDumpContext__I, "ti_sysbios_family_arm_exc_Exception_excDumpContext")
__extern xdc_Void ti_sysbios_family_arm_exc_Exception_excDumpContext__I( xdc_UInt pc );


/*
 * ======== SYSTEM FUNCTIONS ========
 */

/* Module_startupDone */
#define ti_sysbios_family_arm_exc_Exception_Module_startupDone() ti_sysbios_family_arm_exc_Exception_Module__startupDone__S()

/* Object_heap */
#define ti_sysbios_family_arm_exc_Exception_Object_heap() ti_sysbios_family_arm_exc_Exception_Object__heap__C

/* Module_heap */
#define ti_sysbios_family_arm_exc_Exception_Module_heap() ti_sysbios_family_arm_exc_Exception_Object__heap__C

/* Module_id */
static inline CT__ti_sysbios_family_arm_exc_Exception_Module__id ti_sysbios_family_arm_exc_Exception_Module_id( void ) 
{
    return ti_sysbios_family_arm_exc_Exception_Module__id__C;
}

/* Module_hasMask */
static inline xdc_Bool ti_sysbios_family_arm_exc_Exception_Module_hasMask( void ) 
{
    return ti_sysbios_family_arm_exc_Exception_Module__diagsMask__C != NULL;
}

/* Module_getMask */
static inline xdc_Bits16 ti_sysbios_family_arm_exc_Exception_Module_getMask( void ) 
{
    return ti_sysbios_family_arm_exc_Exception_Module__diagsMask__C != NULL ? *ti_sysbios_family_arm_exc_Exception_Module__diagsMask__C : 0;
}

/* Module_setMask */
static inline xdc_Void ti_sysbios_family_arm_exc_Exception_Module_setMask( xdc_Bits16 mask ) 
{
    if (ti_sysbios_family_arm_exc_Exception_Module__diagsMask__C != NULL) *ti_sysbios_family_arm_exc_Exception_Module__diagsMask__C = mask;
}


/*
 * ======== EPILOGUE ========
 */

#ifdef ti_sysbios_family_arm_exc_Exception__top__
#undef __nested__
#endif

#endif /* ti_sysbios_family_arm_exc_Exception__include */


/*
 * ======== STATE STRUCTURES ========
 */

#if defined(__config__) || (!defined(__nested__) && defined(ti_sysbios_family_arm_exc_Exception__internalaccess))

#ifndef ti_sysbios_family_arm_exc_Exception__include_state
#define ti_sysbios_family_arm_exc_Exception__include_state

/* Module_State */
struct ti_sysbios_family_arm_exc_Exception_Module_State {
    xdc_Bool excActive;
    ti_sysbios_family_arm_exc_Exception_ExcContext* excContext;
    xdc_Ptr excStackBuffer;
    __TA_ti_sysbios_family_arm_exc_Exception_Module_State__excStack excStack;
    xdc_SizeT excStackSize;
};

/* Module__state__V */
extern struct ti_sysbios_family_arm_exc_Exception_Module_State__ ti_sysbios_family_arm_exc_Exception_Module__state__V;

#endif /* ti_sysbios_family_arm_exc_Exception__include_state */

#endif


/*
 * ======== PREFIX ALIASES ========
 */

#if !defined(__nested__) && !defined(ti_sysbios_family_arm_exc_Exception__nolocalnames)

#ifndef ti_sysbios_family_arm_exc_Exception__localnames__done
#define ti_sysbios_family_arm_exc_Exception__localnames__done

/* module prefix */
#define Exception_ExceptionHookFuncPtr ti_sysbios_family_arm_exc_Exception_ExceptionHookFuncPtr
#define Exception_Type ti_sysbios_family_arm_exc_Exception_Type
#define Exception_ExcContext ti_sysbios_family_arm_exc_Exception_ExcContext
#define Exception_Module_State ti_sysbios_family_arm_exc_Exception_Module_State
#define Exception_Type_Supervisor ti_sysbios_family_arm_exc_Exception_Type_Supervisor
#define Exception_Type_PreAbort ti_sysbios_family_arm_exc_Exception_Type_PreAbort
#define Exception_Type_DataAbort ti_sysbios_family_arm_exc_Exception_Type_DataAbort
#define Exception_Type_UndefInst ti_sysbios_family_arm_exc_Exception_Type_UndefInst
#define Exception_E_swi ti_sysbios_family_arm_exc_Exception_E_swi
#define Exception_E_prefetchAbort ti_sysbios_family_arm_exc_Exception_E_prefetchAbort
#define Exception_E_dataAbort ti_sysbios_family_arm_exc_Exception_E_dataAbort
#define Exception_E_undefinedInstruction ti_sysbios_family_arm_exc_Exception_E_undefinedInstruction
#define Exception_enableDecode ti_sysbios_family_arm_exc_Exception_enableDecode
#define Exception_excStack ti_sysbios_family_arm_exc_Exception_excStack
#define Exception_excHookFunc ti_sysbios_family_arm_exc_Exception_excHookFunc
#define Exception_Module_name ti_sysbios_family_arm_exc_Exception_Module_name
#define Exception_Module_id ti_sysbios_family_arm_exc_Exception_Module_id
#define Exception_Module_startup ti_sysbios_family_arm_exc_Exception_Module_startup
#define Exception_Module_startupDone ti_sysbios_family_arm_exc_Exception_Module_startupDone
#define Exception_Module_hasMask ti_sysbios_family_arm_exc_Exception_Module_hasMask
#define Exception_Module_getMask ti_sysbios_family_arm_exc_Exception_Module_getMask
#define Exception_Module_setMask ti_sysbios_family_arm_exc_Exception_Module_setMask
#define Exception_Object_heap ti_sysbios_family_arm_exc_Exception_Object_heap
#define Exception_Module_heap ti_sysbios_family_arm_exc_Exception_Module_heap

#endif /* ti_sysbios_family_arm_exc_Exception__localnames__done */
#endif
