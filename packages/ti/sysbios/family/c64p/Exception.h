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

#ifndef ti_sysbios_family_c64p_Exception__include
#define ti_sysbios_family_c64p_Exception__include

#ifndef __nested__
#define __nested__
#define ti_sysbios_family_c64p_Exception__top__
#endif

#ifdef __cplusplus
#define __extern extern "C"
#else
#define __extern extern
#endif

#define ti_sysbios_family_c64p_Exception___VERS 150


/*
 * ======== INCLUDES ========
 */

#include <xdc/std.h>

#include <xdc/runtime/xdc.h>
#include <xdc/runtime/Types.h>
#include <ti/sysbios/family/c64p/package/package.defs.h>

#include <xdc/runtime/IModule.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Log.h>


/*
 * ======== AUXILIARY DEFINITIONS ========
 */

/* FuncPtr */
typedef xdc_Void (*ti_sysbios_family_c64p_Exception_FuncPtr)(xdc_Void);

/* Context */
struct ti_sysbios_family_c64p_Exception_Context {
    xdc_Ptr ILC;
    xdc_Ptr RILC;
    xdc_Ptr AMR;
    xdc_Ptr SSR;
    xdc_Ptr IRP;
    xdc_Ptr NRP;
    xdc_Ptr ITSR;
    xdc_Ptr NTSR;
    xdc_Ptr EFR;
    xdc_Ptr IERR;
    xdc_Ptr B30;
    xdc_Ptr B31;
    xdc_Ptr B28;
    xdc_Ptr B29;
    xdc_Ptr B26;
    xdc_Ptr B27;
    xdc_Ptr B24;
    xdc_Ptr B25;
    xdc_Ptr B22;
    xdc_Ptr B23;
    xdc_Ptr B20;
    xdc_Ptr B21;
    xdc_Ptr B18;
    xdc_Ptr B19;
    xdc_Ptr B16;
    xdc_Ptr B17;
    xdc_Ptr B14;
    xdc_Ptr B15;
    xdc_Ptr B12;
    xdc_Ptr B13;
    xdc_Ptr B10;
    xdc_Ptr B11;
    xdc_Ptr B8;
    xdc_Ptr B9;
    xdc_Ptr B6;
    xdc_Ptr B7;
    xdc_Ptr B4;
    xdc_Ptr B5;
    xdc_Ptr B2;
    xdc_Ptr B3;
    xdc_Ptr B0;
    xdc_Ptr B1;
    xdc_Ptr A30;
    xdc_Ptr A31;
    xdc_Ptr A28;
    xdc_Ptr A29;
    xdc_Ptr A26;
    xdc_Ptr A27;
    xdc_Ptr A24;
    xdc_Ptr A25;
    xdc_Ptr A22;
    xdc_Ptr A23;
    xdc_Ptr A20;
    xdc_Ptr A21;
    xdc_Ptr A18;
    xdc_Ptr A19;
    xdc_Ptr A16;
    xdc_Ptr A17;
    xdc_Ptr A14;
    xdc_Ptr A15;
    xdc_Ptr A12;
    xdc_Ptr A13;
    xdc_Ptr A10;
    xdc_Ptr A11;
    xdc_Ptr A8;
    xdc_Ptr A9;
    xdc_Ptr A6;
    xdc_Ptr A7;
    xdc_Ptr A4;
    xdc_Ptr A5;
    xdc_Ptr A2;
    xdc_Ptr A3;
    xdc_Ptr A0;
    xdc_Ptr A1;
};

/* Status */
struct ti_sysbios_family_c64p_Exception_Status {
    xdc_Bits32 efr;
    xdc_Bits32 nrp;
    xdc_Bits32 ntsr;
    xdc_Bits32 ierr;
    ti_sysbios_family_c64p_Exception_Context* excContext;
};

/* EFRSXF */
#define ti_sysbios_family_c64p_Exception_EFRSXF (0x00000001)

/* EFRIXF */
#define ti_sysbios_family_c64p_Exception_EFRIXF (0x00000002)

/* EFREXF */
#define ti_sysbios_family_c64p_Exception_EFREXF (0x40000000)

/* EFRNXF */
#define ti_sysbios_family_c64p_Exception_EFRNXF (0x80000000)

/* ECRSXF */
#define ti_sysbios_family_c64p_Exception_ECRSXF (0x00000001)

/* ECRIXF */
#define ti_sysbios_family_c64p_Exception_ECRIXF (0x00000002)

/* ECREXF */
#define ti_sysbios_family_c64p_Exception_ECREXF (0x40000000)

/* ECRNXF */
#define ti_sysbios_family_c64p_Exception_ECRNXF (0x80000000)

/* IERRIFX */
#define ti_sysbios_family_c64p_Exception_IERRIFX (0x00000001)

/* IERRFPX */
#define ti_sysbios_family_c64p_Exception_IERRFPX (0x00000002)

/* IERREPX */
#define ti_sysbios_family_c64p_Exception_IERREPX (0x00000004)

/* IERROPX */
#define ti_sysbios_family_c64p_Exception_IERROPX (0x00000008)

/* IERRRCX */
#define ti_sysbios_family_c64p_Exception_IERRRCX (0x00000010)

/* IERRRAX */
#define ti_sysbios_family_c64p_Exception_IERRRAX (0x00000020)

/* IERRPRX */
#define ti_sysbios_family_c64p_Exception_IERRPRX (0x00000040)

/* IERRLBX */
#define ti_sysbios_family_c64p_Exception_IERRLBX (0x00000080)

/* IERRMSX */
#define ti_sysbios_family_c64p_Exception_IERRMSX (0x00000100)

/* TSRGEE */
#define ti_sysbios_family_c64p_Exception_TSRGEE (0x00000004)

/* TSRXEN */
#define ti_sysbios_family_c64p_Exception_TSRXEN (0x00000008)

/* TSREXC */
#define ti_sysbios_family_c64p_Exception_TSREXC (0x00000400)

/* TSRCXM */
#define ti_sysbios_family_c64p_Exception_TSRCXM (0x000000C0)

/* TSRCXMSHIFT */
#define ti_sysbios_family_c64p_Exception_TSRCXMSHIFT (6)

/* TSRCXMSV */
#define ti_sysbios_family_c64p_Exception_TSRCXMSV (0x00000000)

/* TSRCXMUS */
#define ti_sysbios_family_c64p_Exception_TSRCXMUS (0x00000040)

/* sizeContextBuf */
#define ti_sysbios_family_c64p_Exception_sizeContextBuf (320)


/*
 * ======== INTERNAL DEFINITIONS ========
 */

/* EVTPMCCMPA */
#define ti_sysbios_family_c64p_Exception_EVTPMCCMPA (120)

/* EVTDMCCMPA */
#define ti_sysbios_family_c64p_Exception_EVTDMCCMPA (122)

/* EVTUMCCMPA */
#define ti_sysbios_family_c64p_Exception_EVTUMCCMPA (124)

/* EVTEMCCMPA */
#define ti_sysbios_family_c64p_Exception_EVTEMCCMPA (126)

/* Module_State */
typedef xdc_Char __T1_ti_sysbios_family_c64p_Exception_Module_State__scratch;
typedef xdc_Char __ARRAY1_ti_sysbios_family_c64p_Exception_Module_State__scratch[16];
typedef __ARRAY1_ti_sysbios_family_c64p_Exception_Module_State__scratch __TA_ti_sysbios_family_c64p_Exception_Module_State__scratch;
typedef xdc_Char __T1_ti_sysbios_family_c64p_Exception_Module_State__contextBuf;
typedef xdc_Char *__ARRAY1_ti_sysbios_family_c64p_Exception_Module_State__contextBuf;
typedef __ARRAY1_ti_sysbios_family_c64p_Exception_Module_State__contextBuf __TA_ti_sysbios_family_c64p_Exception_Module_State__contextBuf;


/*
 * ======== MODULE-WIDE CONFIGS ========
 */

/* Module__diagsEnabled */
typedef xdc_Bits32 CT__ti_sysbios_family_c64p_Exception_Module__diagsEnabled;
__extern __FAR__ const CT__ti_sysbios_family_c64p_Exception_Module__diagsEnabled ti_sysbios_family_c64p_Exception_Module__diagsEnabled__C;

/* Module__diagsIncluded */
typedef xdc_Bits32 CT__ti_sysbios_family_c64p_Exception_Module__diagsIncluded;
__extern __FAR__ const CT__ti_sysbios_family_c64p_Exception_Module__diagsIncluded ti_sysbios_family_c64p_Exception_Module__diagsIncluded__C;

/* Module__diagsMask */
typedef xdc_Bits16* CT__ti_sysbios_family_c64p_Exception_Module__diagsMask;
__extern __FAR__ const CT__ti_sysbios_family_c64p_Exception_Module__diagsMask ti_sysbios_family_c64p_Exception_Module__diagsMask__C;

/* Module__gateObj */
typedef xdc_Ptr CT__ti_sysbios_family_c64p_Exception_Module__gateObj;
__extern __FAR__ const CT__ti_sysbios_family_c64p_Exception_Module__gateObj ti_sysbios_family_c64p_Exception_Module__gateObj__C;

/* Module__gatePrms */
typedef xdc_Ptr CT__ti_sysbios_family_c64p_Exception_Module__gatePrms;
__extern __FAR__ const CT__ti_sysbios_family_c64p_Exception_Module__gatePrms ti_sysbios_family_c64p_Exception_Module__gatePrms__C;

/* Module__id */
typedef xdc_runtime_Types_ModuleId CT__ti_sysbios_family_c64p_Exception_Module__id;
__extern __FAR__ const CT__ti_sysbios_family_c64p_Exception_Module__id ti_sysbios_family_c64p_Exception_Module__id__C;

/* Module__loggerDefined */
typedef xdc_Bool CT__ti_sysbios_family_c64p_Exception_Module__loggerDefined;
__extern __FAR__ const CT__ti_sysbios_family_c64p_Exception_Module__loggerDefined ti_sysbios_family_c64p_Exception_Module__loggerDefined__C;

/* Module__loggerObj */
typedef xdc_Ptr CT__ti_sysbios_family_c64p_Exception_Module__loggerObj;
__extern __FAR__ const CT__ti_sysbios_family_c64p_Exception_Module__loggerObj ti_sysbios_family_c64p_Exception_Module__loggerObj__C;

/* Module__loggerFxn0 */
typedef xdc_runtime_Types_LoggerFxn0 CT__ti_sysbios_family_c64p_Exception_Module__loggerFxn0;
__extern __FAR__ const CT__ti_sysbios_family_c64p_Exception_Module__loggerFxn0 ti_sysbios_family_c64p_Exception_Module__loggerFxn0__C;

/* Module__loggerFxn1 */
typedef xdc_runtime_Types_LoggerFxn1 CT__ti_sysbios_family_c64p_Exception_Module__loggerFxn1;
__extern __FAR__ const CT__ti_sysbios_family_c64p_Exception_Module__loggerFxn1 ti_sysbios_family_c64p_Exception_Module__loggerFxn1__C;

/* Module__loggerFxn2 */
typedef xdc_runtime_Types_LoggerFxn2 CT__ti_sysbios_family_c64p_Exception_Module__loggerFxn2;
__extern __FAR__ const CT__ti_sysbios_family_c64p_Exception_Module__loggerFxn2 ti_sysbios_family_c64p_Exception_Module__loggerFxn2__C;

/* Module__loggerFxn4 */
typedef xdc_runtime_Types_LoggerFxn4 CT__ti_sysbios_family_c64p_Exception_Module__loggerFxn4;
__extern __FAR__ const CT__ti_sysbios_family_c64p_Exception_Module__loggerFxn4 ti_sysbios_family_c64p_Exception_Module__loggerFxn4__C;

/* Module__loggerFxn8 */
typedef xdc_runtime_Types_LoggerFxn8 CT__ti_sysbios_family_c64p_Exception_Module__loggerFxn8;
__extern __FAR__ const CT__ti_sysbios_family_c64p_Exception_Module__loggerFxn8 ti_sysbios_family_c64p_Exception_Module__loggerFxn8__C;

/* Module__startupDoneFxn */
typedef xdc_Bool (*CT__ti_sysbios_family_c64p_Exception_Module__startupDoneFxn)(void);
__extern __FAR__ const CT__ti_sysbios_family_c64p_Exception_Module__startupDoneFxn ti_sysbios_family_c64p_Exception_Module__startupDoneFxn__C;

/* Object__count */
typedef xdc_Int CT__ti_sysbios_family_c64p_Exception_Object__count;
__extern __FAR__ const CT__ti_sysbios_family_c64p_Exception_Object__count ti_sysbios_family_c64p_Exception_Object__count__C;

/* Object__heap */
typedef xdc_runtime_IHeap_Handle CT__ti_sysbios_family_c64p_Exception_Object__heap;
__extern __FAR__ const CT__ti_sysbios_family_c64p_Exception_Object__heap ti_sysbios_family_c64p_Exception_Object__heap__C;

/* Object__sizeof */
typedef xdc_SizeT CT__ti_sysbios_family_c64p_Exception_Object__sizeof;
__extern __FAR__ const CT__ti_sysbios_family_c64p_Exception_Object__sizeof ti_sysbios_family_c64p_Exception_Object__sizeof__C;

/* Object__table */
typedef xdc_Ptr CT__ti_sysbios_family_c64p_Exception_Object__table;
__extern __FAR__ const CT__ti_sysbios_family_c64p_Exception_Object__table ti_sysbios_family_c64p_Exception_Object__table__C;

/* E_exceptionMin */
#define ti_sysbios_family_c64p_Exception_E_exceptionMin (ti_sysbios_family_c64p_Exception_E_exceptionMin__C)
typedef xdc_runtime_Error_Id CT__ti_sysbios_family_c64p_Exception_E_exceptionMin;
__extern __FAR__ const CT__ti_sysbios_family_c64p_Exception_E_exceptionMin ti_sysbios_family_c64p_Exception_E_exceptionMin__C;

/* E_exceptionMax */
#define ti_sysbios_family_c64p_Exception_E_exceptionMax (ti_sysbios_family_c64p_Exception_E_exceptionMax__C)
typedef xdc_runtime_Error_Id CT__ti_sysbios_family_c64p_Exception_E_exceptionMax;
__extern __FAR__ const CT__ti_sysbios_family_c64p_Exception_E_exceptionMax ti_sysbios_family_c64p_Exception_E_exceptionMax__C;

/* useInternalBuffer */
#ifdef ti_sysbios_family_c64p_Exception_useInternalBuffer__D
#define ti_sysbios_family_c64p_Exception_useInternalBuffer (ti_sysbios_family_c64p_Exception_useInternalBuffer__D)
#else
#define ti_sysbios_family_c64p_Exception_useInternalBuffer (ti_sysbios_family_c64p_Exception_useInternalBuffer__C)
typedef xdc_Bool CT__ti_sysbios_family_c64p_Exception_useInternalBuffer;
__extern __FAR__ const CT__ti_sysbios_family_c64p_Exception_useInternalBuffer ti_sysbios_family_c64p_Exception_useInternalBuffer__C;
#endif

/* enableExternalMPC */
#ifdef ti_sysbios_family_c64p_Exception_enableExternalMPC__D
#define ti_sysbios_family_c64p_Exception_enableExternalMPC (ti_sysbios_family_c64p_Exception_enableExternalMPC__D)
#else
#define ti_sysbios_family_c64p_Exception_enableExternalMPC (ti_sysbios_family_c64p_Exception_enableExternalMPC__C)
typedef xdc_Bool CT__ti_sysbios_family_c64p_Exception_enableExternalMPC;
__extern __FAR__ const CT__ti_sysbios_family_c64p_Exception_enableExternalMPC ti_sysbios_family_c64p_Exception_enableExternalMPC__C;
#endif

/* enablePrint */
#ifdef ti_sysbios_family_c64p_Exception_enablePrint__D
#define ti_sysbios_family_c64p_Exception_enablePrint (ti_sysbios_family_c64p_Exception_enablePrint__D)
#else
#define ti_sysbios_family_c64p_Exception_enablePrint (ti_sysbios_family_c64p_Exception_enablePrint__C)
typedef xdc_Bool CT__ti_sysbios_family_c64p_Exception_enablePrint;
__extern __FAR__ const CT__ti_sysbios_family_c64p_Exception_enablePrint ti_sysbios_family_c64p_Exception_enablePrint__C;
#endif

/* exceptionHook */
#define ti_sysbios_family_c64p_Exception_exceptionHook (ti_sysbios_family_c64p_Exception_exceptionHook__C)
typedef ti_sysbios_family_c64p_Exception_FuncPtr CT__ti_sysbios_family_c64p_Exception_exceptionHook;
__extern __FAR__ const CT__ti_sysbios_family_c64p_Exception_exceptionHook ti_sysbios_family_c64p_Exception_exceptionHook__C;

/* internalHook */
#define ti_sysbios_family_c64p_Exception_internalHook (ti_sysbios_family_c64p_Exception_internalHook__C)
typedef ti_sysbios_family_c64p_Exception_FuncPtr CT__ti_sysbios_family_c64p_Exception_internalHook;
__extern __FAR__ const CT__ti_sysbios_family_c64p_Exception_internalHook ti_sysbios_family_c64p_Exception_internalHook__C;

/* externalHook */
#define ti_sysbios_family_c64p_Exception_externalHook (ti_sysbios_family_c64p_Exception_externalHook__C)
typedef ti_sysbios_family_c64p_Exception_FuncPtr CT__ti_sysbios_family_c64p_Exception_externalHook;
__extern __FAR__ const CT__ti_sysbios_family_c64p_Exception_externalHook ti_sysbios_family_c64p_Exception_externalHook__C;

/* nmiHook */
#define ti_sysbios_family_c64p_Exception_nmiHook (ti_sysbios_family_c64p_Exception_nmiHook__C)
typedef ti_sysbios_family_c64p_Exception_FuncPtr CT__ti_sysbios_family_c64p_Exception_nmiHook;
__extern __FAR__ const CT__ti_sysbios_family_c64p_Exception_nmiHook ti_sysbios_family_c64p_Exception_nmiHook__C;

/* returnHook */
#define ti_sysbios_family_c64p_Exception_returnHook (ti_sysbios_family_c64p_Exception_returnHook__C)
typedef ti_sysbios_family_c64p_Exception_FuncPtr CT__ti_sysbios_family_c64p_Exception_returnHook;
__extern __FAR__ const CT__ti_sysbios_family_c64p_Exception_returnHook ti_sysbios_family_c64p_Exception_returnHook__C;


/*
 * ======== FUNCTION DECLARATIONS ========
 */

/* Module_startup */
#define ti_sysbios_family_c64p_Exception_Module_startup ti_sysbios_family_c64p_Exception_Module_startup__E
xdc__CODESECT(ti_sysbios_family_c64p_Exception_Module_startup__E, "ti_sysbios_family_c64p_Exception_Module_startup")
__extern xdc_Int ti_sysbios_family_c64p_Exception_Module_startup__E( xdc_Int state );
xdc__CODESECT(ti_sysbios_family_c64p_Exception_Module_startup__F, "ti_sysbios_family_c64p_Exception_Module_startup")
__extern xdc_Int ti_sysbios_family_c64p_Exception_Module_startup__F( xdc_Int state );
xdc__CODESECT(ti_sysbios_family_c64p_Exception_Module_startup__R, "ti_sysbios_family_c64p_Exception_Module_startup")
__extern xdc_Int ti_sysbios_family_c64p_Exception_Module_startup__R( xdc_Int state );

/* Module__startupDone__S */
xdc__CODESECT(ti_sysbios_family_c64p_Exception_Module__startupDone__S, "ti_sysbios_family_c64p_Exception_Module__startupDone")
__extern xdc_Bool ti_sysbios_family_c64p_Exception_Module__startupDone__S( void );

/* getLastStatus__E */
#define ti_sysbios_family_c64p_Exception_getLastStatus ti_sysbios_family_c64p_Exception_getLastStatus__E
xdc__CODESECT(ti_sysbios_family_c64p_Exception_getLastStatus__E, "ti_sysbios_family_c64p_Exception_getLastStatus")
__extern xdc_Void ti_sysbios_family_c64p_Exception_getLastStatus__E( ti_sysbios_family_c64p_Exception_Status* status );
xdc__CODESECT(ti_sysbios_family_c64p_Exception_getLastStatus__F, "ti_sysbios_family_c64p_Exception_getLastStatus")
__extern xdc_Void ti_sysbios_family_c64p_Exception_getLastStatus__F( ti_sysbios_family_c64p_Exception_Status* status );
__extern xdc_Void ti_sysbios_family_c64p_Exception_getLastStatus__R( ti_sysbios_family_c64p_Exception_Status* status );

/* clearLastStatus__E */
#define ti_sysbios_family_c64p_Exception_clearLastStatus ti_sysbios_family_c64p_Exception_clearLastStatus__E
xdc__CODESECT(ti_sysbios_family_c64p_Exception_clearLastStatus__E, "ti_sysbios_family_c64p_Exception_clearLastStatus")
__extern xdc_Void ti_sysbios_family_c64p_Exception_clearLastStatus__E( void );
xdc__CODESECT(ti_sysbios_family_c64p_Exception_clearLastStatus__F, "ti_sysbios_family_c64p_Exception_clearLastStatus")
__extern xdc_Void ti_sysbios_family_c64p_Exception_clearLastStatus__F( void );
__extern xdc_Void ti_sysbios_family_c64p_Exception_clearLastStatus__R( void );

/* setReturnPtr__E */
#define ti_sysbios_family_c64p_Exception_setReturnPtr ti_sysbios_family_c64p_Exception_setReturnPtr__E
xdc__CODESECT(ti_sysbios_family_c64p_Exception_setReturnPtr__E, "ti_sysbios_family_c64p_Exception_setReturnPtr")
__extern ti_sysbios_family_c64p_Exception_FuncPtr ti_sysbios_family_c64p_Exception_setReturnPtr__E( ti_sysbios_family_c64p_Exception_FuncPtr ptr );
xdc__CODESECT(ti_sysbios_family_c64p_Exception_setReturnPtr__F, "ti_sysbios_family_c64p_Exception_setReturnPtr")
__extern ti_sysbios_family_c64p_Exception_FuncPtr ti_sysbios_family_c64p_Exception_setReturnPtr__F( ti_sysbios_family_c64p_Exception_FuncPtr ptr );
__extern ti_sysbios_family_c64p_Exception_FuncPtr ti_sysbios_family_c64p_Exception_setReturnPtr__R( ti_sysbios_family_c64p_Exception_FuncPtr ptr );

/* evtEvtClear__E */
#define ti_sysbios_family_c64p_Exception_evtEvtClear ti_sysbios_family_c64p_Exception_evtEvtClear__E
xdc__CODESECT(ti_sysbios_family_c64p_Exception_evtEvtClear__E, "ti_sysbios_family_c64p_Exception_evtEvtClear")
__extern xdc_Void ti_sysbios_family_c64p_Exception_evtEvtClear__E( xdc_UInt event );
xdc__CODESECT(ti_sysbios_family_c64p_Exception_evtEvtClear__F, "ti_sysbios_family_c64p_Exception_evtEvtClear")
__extern xdc_Void ti_sysbios_family_c64p_Exception_evtEvtClear__F( xdc_UInt event );
__extern xdc_Void ti_sysbios_family_c64p_Exception_evtEvtClear__R( xdc_UInt event );

/* evtExpMaskEnable__E */
#define ti_sysbios_family_c64p_Exception_evtExpMaskEnable ti_sysbios_family_c64p_Exception_evtExpMaskEnable__E
xdc__CODESECT(ti_sysbios_family_c64p_Exception_evtExpMaskEnable__E, "ti_sysbios_family_c64p_Exception_evtExpMaskEnable")
__extern xdc_Void ti_sysbios_family_c64p_Exception_evtExpMaskEnable__E( xdc_UInt event );
xdc__CODESECT(ti_sysbios_family_c64p_Exception_evtExpMaskEnable__F, "ti_sysbios_family_c64p_Exception_evtExpMaskEnable")
__extern xdc_Void ti_sysbios_family_c64p_Exception_evtExpMaskEnable__F( xdc_UInt event );
__extern xdc_Void ti_sysbios_family_c64p_Exception_evtExpMaskEnable__R( xdc_UInt event );

/* dispatch__E */
#define ti_sysbios_family_c64p_Exception_dispatch ti_sysbios_family_c64p_Exception_dispatch__E
xdc__CODESECT(ti_sysbios_family_c64p_Exception_dispatch__E, "ti_sysbios_family_c64p_Exception_dispatch")
__extern xdc_Void ti_sysbios_family_c64p_Exception_dispatch__E( void );
xdc__CODESECT(ti_sysbios_family_c64p_Exception_dispatch__F, "ti_sysbios_family_c64p_Exception_dispatch")
__extern xdc_Void ti_sysbios_family_c64p_Exception_dispatch__F( void );
__extern xdc_Void ti_sysbios_family_c64p_Exception_dispatch__R( void );

/* handler__I */
#define ti_sysbios_family_c64p_Exception_handler ti_sysbios_family_c64p_Exception_handler__I
xdc__CODESECT(ti_sysbios_family_c64p_Exception_handler__I, "ti_sysbios_family_c64p_Exception_handler")
__extern xdc_Void ti_sysbios_family_c64p_Exception_handler__I( xdc_Bool abortFlag );

/* internalHandler__I */
#define ti_sysbios_family_c64p_Exception_internalHandler ti_sysbios_family_c64p_Exception_internalHandler__I
xdc__CODESECT(ti_sysbios_family_c64p_Exception_internalHandler__I, "ti_sysbios_family_c64p_Exception_internalHandler")
__extern xdc_Void ti_sysbios_family_c64p_Exception_internalHandler__I( void );

/* externalHandler__I */
#define ti_sysbios_family_c64p_Exception_externalHandler ti_sysbios_family_c64p_Exception_externalHandler__I
xdc__CODESECT(ti_sysbios_family_c64p_Exception_externalHandler__I, "ti_sysbios_family_c64p_Exception_externalHandler")
__extern xdc_Void ti_sysbios_family_c64p_Exception_externalHandler__I( void );

/* nmiHandler__I */
#define ti_sysbios_family_c64p_Exception_nmiHandler ti_sysbios_family_c64p_Exception_nmiHandler__I
xdc__CODESECT(ti_sysbios_family_c64p_Exception_nmiHandler__I, "ti_sysbios_family_c64p_Exception_nmiHandler")
__extern xdc_Void ti_sysbios_family_c64p_Exception_nmiHandler__I( void );

/* decodeMpfsr__I */
#define ti_sysbios_family_c64p_Exception_decodeMpfsr ti_sysbios_family_c64p_Exception_decodeMpfsr__I
xdc__CODESECT(ti_sysbios_family_c64p_Exception_decodeMpfsr__I, "ti_sysbios_family_c64p_Exception_decodeMpfsr")
__extern xdc_Void ti_sysbios_family_c64p_Exception_decodeMpfsr__I( xdc_UInt mpfsr );


/*
 * ======== SYSTEM FUNCTIONS ========
 */

/* Module_startupDone */
#define ti_sysbios_family_c64p_Exception_Module_startupDone() ti_sysbios_family_c64p_Exception_Module__startupDone__S()

/* Object_heap */
#define ti_sysbios_family_c64p_Exception_Object_heap() ti_sysbios_family_c64p_Exception_Object__heap__C

/* Module_heap */
#define ti_sysbios_family_c64p_Exception_Module_heap() ti_sysbios_family_c64p_Exception_Object__heap__C

/* Module_id */
static inline CT__ti_sysbios_family_c64p_Exception_Module__id ti_sysbios_family_c64p_Exception_Module_id( void ) 
{
    return ti_sysbios_family_c64p_Exception_Module__id__C;
}

/* Module_hasMask */
static inline xdc_Bool ti_sysbios_family_c64p_Exception_Module_hasMask( void ) 
{
    return ti_sysbios_family_c64p_Exception_Module__diagsMask__C != NULL;
}

/* Module_getMask */
static inline xdc_Bits16 ti_sysbios_family_c64p_Exception_Module_getMask( void ) 
{
    return ti_sysbios_family_c64p_Exception_Module__diagsMask__C != NULL ? *ti_sysbios_family_c64p_Exception_Module__diagsMask__C : 0;
}

/* Module_setMask */
static inline xdc_Void ti_sysbios_family_c64p_Exception_Module_setMask( xdc_Bits16 mask ) 
{
    if (ti_sysbios_family_c64p_Exception_Module__diagsMask__C != NULL) *ti_sysbios_family_c64p_Exception_Module__diagsMask__C = mask;
}


/*
 * ======== EPILOGUE ========
 */

#ifdef ti_sysbios_family_c64p_Exception__top__
#undef __nested__
#endif

#endif /* ti_sysbios_family_c64p_Exception__include */


/*
 * ======== STATE STRUCTURES ========
 */

#if defined(__config__) || (!defined(__nested__) && defined(ti_sysbios_family_c64p_Exception__internalaccess))

#ifndef ti_sysbios_family_c64p_Exception__include_state
#define ti_sysbios_family_c64p_Exception__include_state

/* Module_State */
struct ti_sysbios_family_c64p_Exception_Module_State {
    xdc_Bits32 efr;
    xdc_Bits32 nrp;
    xdc_Bits32 ntsr;
    xdc_Bits32 ierr;
    ti_sysbios_family_c64p_Exception_FuncPtr returnHook;
    ti_sysbios_family_c64p_Exception_Context* excContext;
    __TA_ti_sysbios_family_c64p_Exception_Module_State__scratch scratch;
    xdc_Char* excPtr;
    __TA_ti_sysbios_family_c64p_Exception_Module_State__contextBuf contextBuf;
};

/* Module__state__V */
extern struct ti_sysbios_family_c64p_Exception_Module_State__ ti_sysbios_family_c64p_Exception_Module__state__V;

#endif /* ti_sysbios_family_c64p_Exception__include_state */

#endif


/*
 * ======== PREFIX ALIASES ========
 */

#if !defined(__nested__) && !defined(ti_sysbios_family_c64p_Exception__nolocalnames)

#ifndef ti_sysbios_family_c64p_Exception__localnames__done
#define ti_sysbios_family_c64p_Exception__localnames__done

/* module prefix */
#define Exception_FuncPtr ti_sysbios_family_c64p_Exception_FuncPtr
#define Exception_Context ti_sysbios_family_c64p_Exception_Context
#define Exception_Status ti_sysbios_family_c64p_Exception_Status
#define Exception_EFRSXF ti_sysbios_family_c64p_Exception_EFRSXF
#define Exception_EFRIXF ti_sysbios_family_c64p_Exception_EFRIXF
#define Exception_EFREXF ti_sysbios_family_c64p_Exception_EFREXF
#define Exception_EFRNXF ti_sysbios_family_c64p_Exception_EFRNXF
#define Exception_ECRSXF ti_sysbios_family_c64p_Exception_ECRSXF
#define Exception_ECRIXF ti_sysbios_family_c64p_Exception_ECRIXF
#define Exception_ECREXF ti_sysbios_family_c64p_Exception_ECREXF
#define Exception_ECRNXF ti_sysbios_family_c64p_Exception_ECRNXF
#define Exception_IERRIFX ti_sysbios_family_c64p_Exception_IERRIFX
#define Exception_IERRFPX ti_sysbios_family_c64p_Exception_IERRFPX
#define Exception_IERREPX ti_sysbios_family_c64p_Exception_IERREPX
#define Exception_IERROPX ti_sysbios_family_c64p_Exception_IERROPX
#define Exception_IERRRCX ti_sysbios_family_c64p_Exception_IERRRCX
#define Exception_IERRRAX ti_sysbios_family_c64p_Exception_IERRRAX
#define Exception_IERRPRX ti_sysbios_family_c64p_Exception_IERRPRX
#define Exception_IERRLBX ti_sysbios_family_c64p_Exception_IERRLBX
#define Exception_IERRMSX ti_sysbios_family_c64p_Exception_IERRMSX
#define Exception_TSRGEE ti_sysbios_family_c64p_Exception_TSRGEE
#define Exception_TSRXEN ti_sysbios_family_c64p_Exception_TSRXEN
#define Exception_TSREXC ti_sysbios_family_c64p_Exception_TSREXC
#define Exception_TSRCXM ti_sysbios_family_c64p_Exception_TSRCXM
#define Exception_TSRCXMSHIFT ti_sysbios_family_c64p_Exception_TSRCXMSHIFT
#define Exception_TSRCXMSV ti_sysbios_family_c64p_Exception_TSRCXMSV
#define Exception_TSRCXMUS ti_sysbios_family_c64p_Exception_TSRCXMUS
#define Exception_sizeContextBuf ti_sysbios_family_c64p_Exception_sizeContextBuf
#define Exception_EVTPMCCMPA ti_sysbios_family_c64p_Exception_EVTPMCCMPA
#define Exception_EVTDMCCMPA ti_sysbios_family_c64p_Exception_EVTDMCCMPA
#define Exception_EVTUMCCMPA ti_sysbios_family_c64p_Exception_EVTUMCCMPA
#define Exception_EVTEMCCMPA ti_sysbios_family_c64p_Exception_EVTEMCCMPA
#define Exception_Module_State ti_sysbios_family_c64p_Exception_Module_State
#define Exception_E_exceptionMin ti_sysbios_family_c64p_Exception_E_exceptionMin
#define Exception_E_exceptionMax ti_sysbios_family_c64p_Exception_E_exceptionMax
#define Exception_useInternalBuffer ti_sysbios_family_c64p_Exception_useInternalBuffer
#define Exception_enableExternalMPC ti_sysbios_family_c64p_Exception_enableExternalMPC
#define Exception_enablePrint ti_sysbios_family_c64p_Exception_enablePrint
#define Exception_exceptionHook ti_sysbios_family_c64p_Exception_exceptionHook
#define Exception_internalHook ti_sysbios_family_c64p_Exception_internalHook
#define Exception_externalHook ti_sysbios_family_c64p_Exception_externalHook
#define Exception_nmiHook ti_sysbios_family_c64p_Exception_nmiHook
#define Exception_returnHook ti_sysbios_family_c64p_Exception_returnHook
#define Exception_getLastStatus ti_sysbios_family_c64p_Exception_getLastStatus
#define Exception_clearLastStatus ti_sysbios_family_c64p_Exception_clearLastStatus
#define Exception_setReturnPtr ti_sysbios_family_c64p_Exception_setReturnPtr
#define Exception_evtEvtClear ti_sysbios_family_c64p_Exception_evtEvtClear
#define Exception_evtExpMaskEnable ti_sysbios_family_c64p_Exception_evtExpMaskEnable
#define Exception_dispatch ti_sysbios_family_c64p_Exception_dispatch
#define Exception_Module_name ti_sysbios_family_c64p_Exception_Module_name
#define Exception_Module_id ti_sysbios_family_c64p_Exception_Module_id
#define Exception_Module_startup ti_sysbios_family_c64p_Exception_Module_startup
#define Exception_Module_startupDone ti_sysbios_family_c64p_Exception_Module_startupDone
#define Exception_Module_hasMask ti_sysbios_family_c64p_Exception_Module_hasMask
#define Exception_Module_getMask ti_sysbios_family_c64p_Exception_Module_getMask
#define Exception_Module_setMask ti_sysbios_family_c64p_Exception_Module_setMask
#define Exception_Object_heap ti_sysbios_family_c64p_Exception_Object_heap
#define Exception_Module_heap ti_sysbios_family_c64p_Exception_Module_heap

#endif /* ti_sysbios_family_c64p_Exception__localnames__done */
#endif
