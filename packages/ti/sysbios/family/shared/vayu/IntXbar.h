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

#ifndef ti_sysbios_family_shared_vayu_IntXbar__include
#define ti_sysbios_family_shared_vayu_IntXbar__include

#ifndef __nested__
#define __nested__
#define ti_sysbios_family_shared_vayu_IntXbar__top__
#endif

#ifdef __cplusplus
#define __extern extern "C"
#else
#define __extern extern
#endif

#define ti_sysbios_family_shared_vayu_IntXbar___VERS 150


/*
 * ======== INCLUDES ========
 */

#include <xdc/std.h>

#include <xdc/runtime/xdc.h>
#include <xdc/runtime/Types.h>
#include <ti/sysbios/family/shared/vayu/package/package.defs.h>

#include <xdc/runtime/IModule.h>
#include <xdc/runtime/Error.h>
#include <ti/sysbios/interfaces/IHwi.h>
#include <xdc/runtime/Assert.h>


/*
 * ======== AUXILIARY DEFINITIONS ========
 */

/* NUM_EVE_XBAR_INST */
#define ti_sysbios_family_shared_vayu_IntXbar_NUM_EVE_XBAR_INST (8)

/* NUM_DSP_XBAR_INST */
#define ti_sysbios_family_shared_vayu_IntXbar_NUM_DSP_XBAR_INST (64)

/* NUM_BENELLI_XBAR_INST */
#define ti_sysbios_family_shared_vayu_IntXbar_NUM_BENELLI_XBAR_INST (57)

/* NUM_A15_XBAR_INST */
#define ti_sysbios_family_shared_vayu_IntXbar_NUM_A15_XBAR_INST (152)

/* MMR_OFFSET_EVE0 */
#define ti_sysbios_family_shared_vayu_IntXbar_MMR_OFFSET_EVE0 (0x7A0)

/* MMR_OFFSET_EVE1 */
#define ti_sysbios_family_shared_vayu_IntXbar_MMR_OFFSET_EVE1 (0x7B0)

/* MMR_OFFSET_EVE2 */
#define ti_sysbios_family_shared_vayu_IntXbar_MMR_OFFSET_EVE2 (0x7C0)

/* MMR_OFFSET_EVE3 */
#define ti_sysbios_family_shared_vayu_IntXbar_MMR_OFFSET_EVE3 (0x7D0)

/* MMR_OFFSET_DSP0 */
#define ti_sysbios_family_shared_vayu_IntXbar_MMR_OFFSET_DSP0 (0x948)

/* MMR_OFFSET_DSP1 */
#define ti_sysbios_family_shared_vayu_IntXbar_MMR_OFFSET_DSP1 (0x9c8)

/* MMR_OFFSET_BENELLI_IPU1 */
#define ti_sysbios_family_shared_vayu_IntXbar_MMR_OFFSET_BENELLI_IPU1 (0x7E0)

/* MMR_OFFSET_BENELLI_IPU2 */
#define ti_sysbios_family_shared_vayu_IntXbar_MMR_OFFSET_BENELLI_IPU2 (0x854)

/* MMR_OFFSET_A15 */
#define ti_sysbios_family_shared_vayu_IntXbar_MMR_OFFSET_A15 (0xA48)

/* MMR_OFFSET_LOCK2 */
#define ti_sysbios_family_shared_vayu_IntXbar_MMR_OFFSET_LOCK2 (0x544)

/* MMR_LOCK2_LOCK */
#define ti_sysbios_family_shared_vayu_IntXbar_MMR_LOCK2_LOCK (0xFDF45530)

/* MMR_LOCK2_UNLOCK */
#define ti_sysbios_family_shared_vayu_IntXbar_MMR_LOCK2_UNLOCK (0xF757FDC0)


/*
 * ======== INTERNAL DEFINITIONS ========
 */

/* Module_State */
typedef xdc_UInt16 __T1_ti_sysbios_family_shared_vayu_IntXbar_Module_State__intXbar;
typedef xdc_UInt16 *__ARRAY1_ti_sysbios_family_shared_vayu_IntXbar_Module_State__intXbar;
typedef __ARRAY1_ti_sysbios_family_shared_vayu_IntXbar_Module_State__intXbar __TA_ti_sysbios_family_shared_vayu_IntXbar_Module_State__intXbar;


/*
 * ======== MODULE-WIDE CONFIGS ========
 */

/* Module__diagsEnabled */
typedef xdc_Bits32 CT__ti_sysbios_family_shared_vayu_IntXbar_Module__diagsEnabled;
__extern __FAR__ const CT__ti_sysbios_family_shared_vayu_IntXbar_Module__diagsEnabled ti_sysbios_family_shared_vayu_IntXbar_Module__diagsEnabled__C;

/* Module__diagsIncluded */
typedef xdc_Bits32 CT__ti_sysbios_family_shared_vayu_IntXbar_Module__diagsIncluded;
__extern __FAR__ const CT__ti_sysbios_family_shared_vayu_IntXbar_Module__diagsIncluded ti_sysbios_family_shared_vayu_IntXbar_Module__diagsIncluded__C;

/* Module__diagsMask */
typedef xdc_Bits16* CT__ti_sysbios_family_shared_vayu_IntXbar_Module__diagsMask;
__extern __FAR__ const CT__ti_sysbios_family_shared_vayu_IntXbar_Module__diagsMask ti_sysbios_family_shared_vayu_IntXbar_Module__diagsMask__C;

/* Module__gateObj */
typedef xdc_Ptr CT__ti_sysbios_family_shared_vayu_IntXbar_Module__gateObj;
__extern __FAR__ const CT__ti_sysbios_family_shared_vayu_IntXbar_Module__gateObj ti_sysbios_family_shared_vayu_IntXbar_Module__gateObj__C;

/* Module__gatePrms */
typedef xdc_Ptr CT__ti_sysbios_family_shared_vayu_IntXbar_Module__gatePrms;
__extern __FAR__ const CT__ti_sysbios_family_shared_vayu_IntXbar_Module__gatePrms ti_sysbios_family_shared_vayu_IntXbar_Module__gatePrms__C;

/* Module__id */
typedef xdc_runtime_Types_ModuleId CT__ti_sysbios_family_shared_vayu_IntXbar_Module__id;
__extern __FAR__ const CT__ti_sysbios_family_shared_vayu_IntXbar_Module__id ti_sysbios_family_shared_vayu_IntXbar_Module__id__C;

/* Module__loggerDefined */
typedef xdc_Bool CT__ti_sysbios_family_shared_vayu_IntXbar_Module__loggerDefined;
__extern __FAR__ const CT__ti_sysbios_family_shared_vayu_IntXbar_Module__loggerDefined ti_sysbios_family_shared_vayu_IntXbar_Module__loggerDefined__C;

/* Module__loggerObj */
typedef xdc_Ptr CT__ti_sysbios_family_shared_vayu_IntXbar_Module__loggerObj;
__extern __FAR__ const CT__ti_sysbios_family_shared_vayu_IntXbar_Module__loggerObj ti_sysbios_family_shared_vayu_IntXbar_Module__loggerObj__C;

/* Module__loggerFxn0 */
typedef xdc_runtime_Types_LoggerFxn0 CT__ti_sysbios_family_shared_vayu_IntXbar_Module__loggerFxn0;
__extern __FAR__ const CT__ti_sysbios_family_shared_vayu_IntXbar_Module__loggerFxn0 ti_sysbios_family_shared_vayu_IntXbar_Module__loggerFxn0__C;

/* Module__loggerFxn1 */
typedef xdc_runtime_Types_LoggerFxn1 CT__ti_sysbios_family_shared_vayu_IntXbar_Module__loggerFxn1;
__extern __FAR__ const CT__ti_sysbios_family_shared_vayu_IntXbar_Module__loggerFxn1 ti_sysbios_family_shared_vayu_IntXbar_Module__loggerFxn1__C;

/* Module__loggerFxn2 */
typedef xdc_runtime_Types_LoggerFxn2 CT__ti_sysbios_family_shared_vayu_IntXbar_Module__loggerFxn2;
__extern __FAR__ const CT__ti_sysbios_family_shared_vayu_IntXbar_Module__loggerFxn2 ti_sysbios_family_shared_vayu_IntXbar_Module__loggerFxn2__C;

/* Module__loggerFxn4 */
typedef xdc_runtime_Types_LoggerFxn4 CT__ti_sysbios_family_shared_vayu_IntXbar_Module__loggerFxn4;
__extern __FAR__ const CT__ti_sysbios_family_shared_vayu_IntXbar_Module__loggerFxn4 ti_sysbios_family_shared_vayu_IntXbar_Module__loggerFxn4__C;

/* Module__loggerFxn8 */
typedef xdc_runtime_Types_LoggerFxn8 CT__ti_sysbios_family_shared_vayu_IntXbar_Module__loggerFxn8;
__extern __FAR__ const CT__ti_sysbios_family_shared_vayu_IntXbar_Module__loggerFxn8 ti_sysbios_family_shared_vayu_IntXbar_Module__loggerFxn8__C;

/* Module__startupDoneFxn */
typedef xdc_Bool (*CT__ti_sysbios_family_shared_vayu_IntXbar_Module__startupDoneFxn)(void);
__extern __FAR__ const CT__ti_sysbios_family_shared_vayu_IntXbar_Module__startupDoneFxn ti_sysbios_family_shared_vayu_IntXbar_Module__startupDoneFxn__C;

/* Object__count */
typedef xdc_Int CT__ti_sysbios_family_shared_vayu_IntXbar_Object__count;
__extern __FAR__ const CT__ti_sysbios_family_shared_vayu_IntXbar_Object__count ti_sysbios_family_shared_vayu_IntXbar_Object__count__C;

/* Object__heap */
typedef xdc_runtime_IHeap_Handle CT__ti_sysbios_family_shared_vayu_IntXbar_Object__heap;
__extern __FAR__ const CT__ti_sysbios_family_shared_vayu_IntXbar_Object__heap ti_sysbios_family_shared_vayu_IntXbar_Object__heap__C;

/* Object__sizeof */
typedef xdc_SizeT CT__ti_sysbios_family_shared_vayu_IntXbar_Object__sizeof;
__extern __FAR__ const CT__ti_sysbios_family_shared_vayu_IntXbar_Object__sizeof ti_sysbios_family_shared_vayu_IntXbar_Object__sizeof__C;

/* Object__table */
typedef xdc_Ptr CT__ti_sysbios_family_shared_vayu_IntXbar_Object__table;
__extern __FAR__ const CT__ti_sysbios_family_shared_vayu_IntXbar_Object__table ti_sysbios_family_shared_vayu_IntXbar_Object__table__C;

/* A_badEveXbarInstanceNum */
#define ti_sysbios_family_shared_vayu_IntXbar_A_badEveXbarInstanceNum (ti_sysbios_family_shared_vayu_IntXbar_A_badEveXbarInstanceNum__C)
typedef xdc_runtime_Assert_Id CT__ti_sysbios_family_shared_vayu_IntXbar_A_badEveXbarInstanceNum;
__extern __FAR__ const CT__ti_sysbios_family_shared_vayu_IntXbar_A_badEveXbarInstanceNum ti_sysbios_family_shared_vayu_IntXbar_A_badEveXbarInstanceNum__C;

/* A_badDspXbarInstanceNum */
#define ti_sysbios_family_shared_vayu_IntXbar_A_badDspXbarInstanceNum (ti_sysbios_family_shared_vayu_IntXbar_A_badDspXbarInstanceNum__C)
typedef xdc_runtime_Assert_Id CT__ti_sysbios_family_shared_vayu_IntXbar_A_badDspXbarInstanceNum;
__extern __FAR__ const CT__ti_sysbios_family_shared_vayu_IntXbar_A_badDspXbarInstanceNum ti_sysbios_family_shared_vayu_IntXbar_A_badDspXbarInstanceNum__C;

/* A_badBenelliXbarInstanceNum */
#define ti_sysbios_family_shared_vayu_IntXbar_A_badBenelliXbarInstanceNum (ti_sysbios_family_shared_vayu_IntXbar_A_badBenelliXbarInstanceNum__C)
typedef xdc_runtime_Assert_Id CT__ti_sysbios_family_shared_vayu_IntXbar_A_badBenelliXbarInstanceNum;
__extern __FAR__ const CT__ti_sysbios_family_shared_vayu_IntXbar_A_badBenelliXbarInstanceNum ti_sysbios_family_shared_vayu_IntXbar_A_badBenelliXbarInstanceNum__C;

/* A_badA15XbarInstanceNum */
#define ti_sysbios_family_shared_vayu_IntXbar_A_badA15XbarInstanceNum (ti_sysbios_family_shared_vayu_IntXbar_A_badA15XbarInstanceNum__C)
typedef xdc_runtime_Assert_Id CT__ti_sysbios_family_shared_vayu_IntXbar_A_badA15XbarInstanceNum;
__extern __FAR__ const CT__ti_sysbios_family_shared_vayu_IntXbar_A_badA15XbarInstanceNum ti_sysbios_family_shared_vayu_IntXbar_A_badA15XbarInstanceNum__C;

/* A_badEveIRQNum */
#define ti_sysbios_family_shared_vayu_IntXbar_A_badEveIRQNum (ti_sysbios_family_shared_vayu_IntXbar_A_badEveIRQNum__C)
typedef xdc_runtime_Assert_Id CT__ti_sysbios_family_shared_vayu_IntXbar_A_badEveIRQNum;
__extern __FAR__ const CT__ti_sysbios_family_shared_vayu_IntXbar_A_badEveIRQNum ti_sysbios_family_shared_vayu_IntXbar_A_badEveIRQNum__C;

/* A_badDspIRQNum */
#define ti_sysbios_family_shared_vayu_IntXbar_A_badDspIRQNum (ti_sysbios_family_shared_vayu_IntXbar_A_badDspIRQNum__C)
typedef xdc_runtime_Assert_Id CT__ti_sysbios_family_shared_vayu_IntXbar_A_badDspIRQNum;
__extern __FAR__ const CT__ti_sysbios_family_shared_vayu_IntXbar_A_badDspIRQNum ti_sysbios_family_shared_vayu_IntXbar_A_badDspIRQNum__C;

/* A_badBenelliIRQNum */
#define ti_sysbios_family_shared_vayu_IntXbar_A_badBenelliIRQNum (ti_sysbios_family_shared_vayu_IntXbar_A_badBenelliIRQNum__C)
typedef xdc_runtime_Assert_Id CT__ti_sysbios_family_shared_vayu_IntXbar_A_badBenelliIRQNum;
__extern __FAR__ const CT__ti_sysbios_family_shared_vayu_IntXbar_A_badBenelliIRQNum ti_sysbios_family_shared_vayu_IntXbar_A_badBenelliIRQNum__C;

/* A_badA15IRQNum */
#define ti_sysbios_family_shared_vayu_IntXbar_A_badA15IRQNum (ti_sysbios_family_shared_vayu_IntXbar_A_badA15IRQNum__C)
typedef xdc_runtime_Assert_Id CT__ti_sysbios_family_shared_vayu_IntXbar_A_badA15IRQNum;
__extern __FAR__ const CT__ti_sysbios_family_shared_vayu_IntXbar_A_badA15IRQNum ti_sysbios_family_shared_vayu_IntXbar_A_badA15IRQNum__C;

/* A_badIntSourceIdx */
#define ti_sysbios_family_shared_vayu_IntXbar_A_badIntSourceIdx (ti_sysbios_family_shared_vayu_IntXbar_A_badIntSourceIdx__C)
typedef xdc_runtime_Assert_Id CT__ti_sysbios_family_shared_vayu_IntXbar_A_badIntSourceIdx;
__extern __FAR__ const CT__ti_sysbios_family_shared_vayu_IntXbar_A_badIntSourceIdx ti_sysbios_family_shared_vayu_IntXbar_A_badIntSourceIdx__C;

/* A_badEveCpuId */
#define ti_sysbios_family_shared_vayu_IntXbar_A_badEveCpuId (ti_sysbios_family_shared_vayu_IntXbar_A_badEveCpuId__C)
typedef xdc_runtime_Assert_Id CT__ti_sysbios_family_shared_vayu_IntXbar_A_badEveCpuId;
__extern __FAR__ const CT__ti_sysbios_family_shared_vayu_IntXbar_A_badEveCpuId ti_sysbios_family_shared_vayu_IntXbar_A_badEveCpuId__C;

/* A_badDspCpuId */
#define ti_sysbios_family_shared_vayu_IntXbar_A_badDspCpuId (ti_sysbios_family_shared_vayu_IntXbar_A_badDspCpuId__C)
typedef xdc_runtime_Assert_Id CT__ti_sysbios_family_shared_vayu_IntXbar_A_badDspCpuId;
__extern __FAR__ const CT__ti_sysbios_family_shared_vayu_IntXbar_A_badDspCpuId ti_sysbios_family_shared_vayu_IntXbar_A_badDspCpuId__C;

/* A_badBenelliIpuId */
#define ti_sysbios_family_shared_vayu_IntXbar_A_badBenelliIpuId (ti_sysbios_family_shared_vayu_IntXbar_A_badBenelliIpuId__C)
typedef xdc_runtime_Assert_Id CT__ti_sysbios_family_shared_vayu_IntXbar_A_badBenelliIpuId;
__extern __FAR__ const CT__ti_sysbios_family_shared_vayu_IntXbar_A_badBenelliIpuId ti_sysbios_family_shared_vayu_IntXbar_A_badBenelliIpuId__C;

/* mmrBaseAddr */
#ifdef ti_sysbios_family_shared_vayu_IntXbar_mmrBaseAddr__D
#define ti_sysbios_family_shared_vayu_IntXbar_mmrBaseAddr (ti_sysbios_family_shared_vayu_IntXbar_mmrBaseAddr__D)
#else
#define ti_sysbios_family_shared_vayu_IntXbar_mmrBaseAddr (ti_sysbios_family_shared_vayu_IntXbar_mmrBaseAddr__C)
typedef xdc_UInt32 CT__ti_sysbios_family_shared_vayu_IntXbar_mmrBaseAddr;
__extern __FAR__ const CT__ti_sysbios_family_shared_vayu_IntXbar_mmrBaseAddr ti_sysbios_family_shared_vayu_IntXbar_mmrBaseAddr__C;
#endif

/* intXbar */
#define ti_sysbios_family_shared_vayu_IntXbar_intXbar (ti_sysbios_family_shared_vayu_IntXbar_intXbar__C)
typedef xdc_UInt16 __T1_ti_sysbios_family_shared_vayu_IntXbar_intXbar;
typedef xdc_UInt16 *__ARRAY1_ti_sysbios_family_shared_vayu_IntXbar_intXbar;
typedef __ARRAY1_ti_sysbios_family_shared_vayu_IntXbar_intXbar __TA_ti_sysbios_family_shared_vayu_IntXbar_intXbar;
typedef __TA_ti_sysbios_family_shared_vayu_IntXbar_intXbar CT__ti_sysbios_family_shared_vayu_IntXbar_intXbar;
__extern __FAR__ const CT__ti_sysbios_family_shared_vayu_IntXbar_intXbar ti_sysbios_family_shared_vayu_IntXbar_intXbar__C;


/*
 * ======== FUNCTION DECLARATIONS ========
 */

/* Module_startup */
#define ti_sysbios_family_shared_vayu_IntXbar_Module_startup( state ) (-1)

/* Module__startupDone__S */
xdc__CODESECT(ti_sysbios_family_shared_vayu_IntXbar_Module__startupDone__S, "ti_sysbios_family_shared_vayu_IntXbar_Module__startupDone")
__extern xdc_Bool ti_sysbios_family_shared_vayu_IntXbar_Module__startupDone__S( void );

/* connect__E */
#define ti_sysbios_family_shared_vayu_IntXbar_connect ti_sysbios_family_shared_vayu_IntXbar_connect__E
xdc__CODESECT(ti_sysbios_family_shared_vayu_IntXbar_connect__E, "ti_sysbios_family_shared_vayu_IntXbar_connect")
__extern xdc_Void ti_sysbios_family_shared_vayu_IntXbar_connect__E( xdc_UInt xbarInstance, xdc_UInt intSource );
xdc__CODESECT(ti_sysbios_family_shared_vayu_IntXbar_connect__F, "ti_sysbios_family_shared_vayu_IntXbar_connect")
__extern xdc_Void ti_sysbios_family_shared_vayu_IntXbar_connect__F( xdc_UInt xbarInstance, xdc_UInt intSource );
__extern xdc_Void ti_sysbios_family_shared_vayu_IntXbar_connect__R( xdc_UInt xbarInstance, xdc_UInt intSource );

/* disconnect__E */
#define ti_sysbios_family_shared_vayu_IntXbar_disconnect ti_sysbios_family_shared_vayu_IntXbar_disconnect__E
xdc__CODESECT(ti_sysbios_family_shared_vayu_IntXbar_disconnect__E, "ti_sysbios_family_shared_vayu_IntXbar_disconnect")
__extern xdc_Void ti_sysbios_family_shared_vayu_IntXbar_disconnect__E( xdc_UInt xbarInstance );
xdc__CODESECT(ti_sysbios_family_shared_vayu_IntXbar_disconnect__F, "ti_sysbios_family_shared_vayu_IntXbar_disconnect")
__extern xdc_Void ti_sysbios_family_shared_vayu_IntXbar_disconnect__F( xdc_UInt xbarInstance );
__extern xdc_Void ti_sysbios_family_shared_vayu_IntXbar_disconnect__R( xdc_UInt xbarInstance );

/* connectIRQ__E */
#define ti_sysbios_family_shared_vayu_IntXbar_connectIRQ ti_sysbios_family_shared_vayu_IntXbar_connectIRQ__E
xdc__CODESECT(ti_sysbios_family_shared_vayu_IntXbar_connectIRQ__E, "ti_sysbios_family_shared_vayu_IntXbar_connectIRQ")
__extern xdc_Void ti_sysbios_family_shared_vayu_IntXbar_connectIRQ__E( xdc_UInt cpuIRQNum, xdc_UInt intSource );
xdc__CODESECT(ti_sysbios_family_shared_vayu_IntXbar_connectIRQ__F, "ti_sysbios_family_shared_vayu_IntXbar_connectIRQ")
__extern xdc_Void ti_sysbios_family_shared_vayu_IntXbar_connectIRQ__F( xdc_UInt cpuIRQNum, xdc_UInt intSource );
__extern xdc_Void ti_sysbios_family_shared_vayu_IntXbar_connectIRQ__R( xdc_UInt cpuIRQNum, xdc_UInt intSource );

/* disconnectIRQ__E */
#define ti_sysbios_family_shared_vayu_IntXbar_disconnectIRQ ti_sysbios_family_shared_vayu_IntXbar_disconnectIRQ__E
xdc__CODESECT(ti_sysbios_family_shared_vayu_IntXbar_disconnectIRQ__E, "ti_sysbios_family_shared_vayu_IntXbar_disconnectIRQ")
__extern xdc_Void ti_sysbios_family_shared_vayu_IntXbar_disconnectIRQ__E( xdc_UInt cpuIRQNum );
xdc__CODESECT(ti_sysbios_family_shared_vayu_IntXbar_disconnectIRQ__F, "ti_sysbios_family_shared_vayu_IntXbar_disconnectIRQ")
__extern xdc_Void ti_sysbios_family_shared_vayu_IntXbar_disconnectIRQ__F( xdc_UInt cpuIRQNum );
__extern xdc_Void ti_sysbios_family_shared_vayu_IntXbar_disconnectIRQ__R( xdc_UInt cpuIRQNum );

/* initXbarRegs__I */
#define ti_sysbios_family_shared_vayu_IntXbar_initXbarRegs ti_sysbios_family_shared_vayu_IntXbar_initXbarRegs__I
xdc__CODESECT(ti_sysbios_family_shared_vayu_IntXbar_initXbarRegs__I, "ti_sysbios_family_shared_vayu_IntXbar_initXbarRegs")
__extern xdc_Void ti_sysbios_family_shared_vayu_IntXbar_initXbarRegs__I( volatile xdc_UInt32** intXbarReg, xdc_UInt16* intXbarLength );

/* initIntXbar__I */
#define ti_sysbios_family_shared_vayu_IntXbar_initIntXbar ti_sysbios_family_shared_vayu_IntXbar_initIntXbar__I
xdc__CODESECT(ti_sysbios_family_shared_vayu_IntXbar_initIntXbar__I, "ti_sysbios_family_shared_vayu_IntXbar_initIntXbar")
__extern xdc_Void ti_sysbios_family_shared_vayu_IntXbar_initIntXbar__I( void );


/*
 * ======== SYSTEM FUNCTIONS ========
 */

/* Module_startupDone */
#define ti_sysbios_family_shared_vayu_IntXbar_Module_startupDone() ti_sysbios_family_shared_vayu_IntXbar_Module__startupDone__S()

/* Object_heap */
#define ti_sysbios_family_shared_vayu_IntXbar_Object_heap() ti_sysbios_family_shared_vayu_IntXbar_Object__heap__C

/* Module_heap */
#define ti_sysbios_family_shared_vayu_IntXbar_Module_heap() ti_sysbios_family_shared_vayu_IntXbar_Object__heap__C

/* Module_id */
static inline CT__ti_sysbios_family_shared_vayu_IntXbar_Module__id ti_sysbios_family_shared_vayu_IntXbar_Module_id( void ) 
{
    return ti_sysbios_family_shared_vayu_IntXbar_Module__id__C;
}

/* Module_hasMask */
static inline xdc_Bool ti_sysbios_family_shared_vayu_IntXbar_Module_hasMask( void ) 
{
    return ti_sysbios_family_shared_vayu_IntXbar_Module__diagsMask__C != NULL;
}

/* Module_getMask */
static inline xdc_Bits16 ti_sysbios_family_shared_vayu_IntXbar_Module_getMask( void ) 
{
    return ti_sysbios_family_shared_vayu_IntXbar_Module__diagsMask__C != NULL ? *ti_sysbios_family_shared_vayu_IntXbar_Module__diagsMask__C : 0;
}

/* Module_setMask */
static inline xdc_Void ti_sysbios_family_shared_vayu_IntXbar_Module_setMask( xdc_Bits16 mask ) 
{
    if (ti_sysbios_family_shared_vayu_IntXbar_Module__diagsMask__C != NULL) *ti_sysbios_family_shared_vayu_IntXbar_Module__diagsMask__C = mask;
}


/*
 * ======== EPILOGUE ========
 */

#ifdef ti_sysbios_family_shared_vayu_IntXbar__top__
#undef __nested__
#endif

#endif /* ti_sysbios_family_shared_vayu_IntXbar__include */


/*
 * ======== STATE STRUCTURES ========
 */

#if defined(__config__) || (!defined(__nested__) && defined(ti_sysbios_family_shared_vayu_IntXbar__internalaccess))

#ifndef ti_sysbios_family_shared_vayu_IntXbar__include_state
#define ti_sysbios_family_shared_vayu_IntXbar__include_state

/* Module_State */
struct ti_sysbios_family_shared_vayu_IntXbar_Module_State {
    __TA_ti_sysbios_family_shared_vayu_IntXbar_Module_State__intXbar intXbar;
};

/* Module__state__V */
extern struct ti_sysbios_family_shared_vayu_IntXbar_Module_State__ ti_sysbios_family_shared_vayu_IntXbar_Module__state__V;

#endif /* ti_sysbios_family_shared_vayu_IntXbar__include_state */

#endif


/*
 * ======== PREFIX ALIASES ========
 */

#if !defined(__nested__) && !defined(ti_sysbios_family_shared_vayu_IntXbar__nolocalnames)

#ifndef ti_sysbios_family_shared_vayu_IntXbar__localnames__done
#define ti_sysbios_family_shared_vayu_IntXbar__localnames__done

/* module prefix */
#define IntXbar_NUM_EVE_XBAR_INST ti_sysbios_family_shared_vayu_IntXbar_NUM_EVE_XBAR_INST
#define IntXbar_NUM_DSP_XBAR_INST ti_sysbios_family_shared_vayu_IntXbar_NUM_DSP_XBAR_INST
#define IntXbar_NUM_BENELLI_XBAR_INST ti_sysbios_family_shared_vayu_IntXbar_NUM_BENELLI_XBAR_INST
#define IntXbar_NUM_A15_XBAR_INST ti_sysbios_family_shared_vayu_IntXbar_NUM_A15_XBAR_INST
#define IntXbar_MMR_OFFSET_EVE0 ti_sysbios_family_shared_vayu_IntXbar_MMR_OFFSET_EVE0
#define IntXbar_MMR_OFFSET_EVE1 ti_sysbios_family_shared_vayu_IntXbar_MMR_OFFSET_EVE1
#define IntXbar_MMR_OFFSET_EVE2 ti_sysbios_family_shared_vayu_IntXbar_MMR_OFFSET_EVE2
#define IntXbar_MMR_OFFSET_EVE3 ti_sysbios_family_shared_vayu_IntXbar_MMR_OFFSET_EVE3
#define IntXbar_MMR_OFFSET_DSP0 ti_sysbios_family_shared_vayu_IntXbar_MMR_OFFSET_DSP0
#define IntXbar_MMR_OFFSET_DSP1 ti_sysbios_family_shared_vayu_IntXbar_MMR_OFFSET_DSP1
#define IntXbar_MMR_OFFSET_BENELLI_IPU1 ti_sysbios_family_shared_vayu_IntXbar_MMR_OFFSET_BENELLI_IPU1
#define IntXbar_MMR_OFFSET_BENELLI_IPU2 ti_sysbios_family_shared_vayu_IntXbar_MMR_OFFSET_BENELLI_IPU2
#define IntXbar_MMR_OFFSET_A15 ti_sysbios_family_shared_vayu_IntXbar_MMR_OFFSET_A15
#define IntXbar_MMR_OFFSET_LOCK2 ti_sysbios_family_shared_vayu_IntXbar_MMR_OFFSET_LOCK2
#define IntXbar_MMR_LOCK2_LOCK ti_sysbios_family_shared_vayu_IntXbar_MMR_LOCK2_LOCK
#define IntXbar_MMR_LOCK2_UNLOCK ti_sysbios_family_shared_vayu_IntXbar_MMR_LOCK2_UNLOCK
#define IntXbar_Module_State ti_sysbios_family_shared_vayu_IntXbar_Module_State
#define IntXbar_A_badEveXbarInstanceNum ti_sysbios_family_shared_vayu_IntXbar_A_badEveXbarInstanceNum
#define IntXbar_A_badDspXbarInstanceNum ti_sysbios_family_shared_vayu_IntXbar_A_badDspXbarInstanceNum
#define IntXbar_A_badBenelliXbarInstanceNum ti_sysbios_family_shared_vayu_IntXbar_A_badBenelliXbarInstanceNum
#define IntXbar_A_badA15XbarInstanceNum ti_sysbios_family_shared_vayu_IntXbar_A_badA15XbarInstanceNum
#define IntXbar_A_badEveIRQNum ti_sysbios_family_shared_vayu_IntXbar_A_badEveIRQNum
#define IntXbar_A_badDspIRQNum ti_sysbios_family_shared_vayu_IntXbar_A_badDspIRQNum
#define IntXbar_A_badBenelliIRQNum ti_sysbios_family_shared_vayu_IntXbar_A_badBenelliIRQNum
#define IntXbar_A_badA15IRQNum ti_sysbios_family_shared_vayu_IntXbar_A_badA15IRQNum
#define IntXbar_A_badIntSourceIdx ti_sysbios_family_shared_vayu_IntXbar_A_badIntSourceIdx
#define IntXbar_A_badEveCpuId ti_sysbios_family_shared_vayu_IntXbar_A_badEveCpuId
#define IntXbar_A_badDspCpuId ti_sysbios_family_shared_vayu_IntXbar_A_badDspCpuId
#define IntXbar_A_badBenelliIpuId ti_sysbios_family_shared_vayu_IntXbar_A_badBenelliIpuId
#define IntXbar_mmrBaseAddr ti_sysbios_family_shared_vayu_IntXbar_mmrBaseAddr
#define IntXbar_intXbar ti_sysbios_family_shared_vayu_IntXbar_intXbar
#define IntXbar_connect ti_sysbios_family_shared_vayu_IntXbar_connect
#define IntXbar_disconnect ti_sysbios_family_shared_vayu_IntXbar_disconnect
#define IntXbar_connectIRQ ti_sysbios_family_shared_vayu_IntXbar_connectIRQ
#define IntXbar_disconnectIRQ ti_sysbios_family_shared_vayu_IntXbar_disconnectIRQ
#define IntXbar_Module_name ti_sysbios_family_shared_vayu_IntXbar_Module_name
#define IntXbar_Module_id ti_sysbios_family_shared_vayu_IntXbar_Module_id
#define IntXbar_Module_startup ti_sysbios_family_shared_vayu_IntXbar_Module_startup
#define IntXbar_Module_startupDone ti_sysbios_family_shared_vayu_IntXbar_Module_startupDone
#define IntXbar_Module_hasMask ti_sysbios_family_shared_vayu_IntXbar_Module_hasMask
#define IntXbar_Module_getMask ti_sysbios_family_shared_vayu_IntXbar_Module_getMask
#define IntXbar_Module_setMask ti_sysbios_family_shared_vayu_IntXbar_Module_setMask
#define IntXbar_Object_heap ti_sysbios_family_shared_vayu_IntXbar_Object_heap
#define IntXbar_Module_heap ti_sysbios_family_shared_vayu_IntXbar_Module_heap

#endif /* ti_sysbios_family_shared_vayu_IntXbar__localnames__done */
#endif
