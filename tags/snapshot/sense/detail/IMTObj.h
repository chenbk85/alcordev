/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Mon Feb 24 13:17:23 2003
 */
/* Compiler settings for D:\Patrick\XIMU\C++ code\MTObj\IMTObj.idl:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __IMTObj_h__
#define __IMTObj_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IMotionTracker_FWD_DEFINED__
#define __IMotionTracker_FWD_DEFINED__
typedef interface IMotionTracker IMotionTracker;
#endif 	/* __IMotionTracker_FWD_DEFINED__ */


#ifndef __IMotionTrackerEvents_FWD_DEFINED__
#define __IMotionTrackerEvents_FWD_DEFINED__
typedef interface IMotionTrackerEvents IMotionTrackerEvents;
#endif 	/* __IMotionTrackerEvents_FWD_DEFINED__ */


#ifndef __MotionTracker_FWD_DEFINED__
#define __MotionTracker_FWD_DEFINED__

#ifdef __cplusplus
typedef class MotionTracker MotionTracker;
#else
typedef struct MotionTracker MotionTracker;
#endif /* __cplusplus */

#endif 	/* __MotionTracker_FWD_DEFINED__ */


/* header files for imported files */
#include "unknwn.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IMotionTracker_INTERFACE_DEFINED__
#define __IMotionTracker_INTERFACE_DEFINED__

/* interface IMotionTracker */
/* [oleautomation][dual][unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IMotionTracker;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BB280863-7A0D-11D6-926B-008048B33483")
    IMotionTracker : public IDispatch
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE MT_SetOutputMode( 
            short nMode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_SetSampleFrequency( 
            short nSampleFreq) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_SetCalibratedOutput( 
            short nEnabled) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_SetCOMPort( 
            short nPort) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_SetxmuLocationBSTR( 
            BSTR bstrName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_SetFilterSettings( 
            float fGain,
            short nCorInterval,
            float fRho) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_SetInclination( 
            float fInclination) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_SetDoAMD( 
            short nDoAMD) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_GetOrientationData( 
            /* [out] */ short __RPC_FAR *nRetVal,
            /* [out] */ VARIANT __RPC_FAR *pfOutputArray) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_GetCalibratedData( 
            /* [out] */ short __RPC_FAR *nRetVal,
            /* [out] */ VARIANT __RPC_FAR *pfOutputArray) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_GetFilterSettings( 
            /* [out] */ float __RPC_FAR *fGain,
            /* [out] */ short __RPC_FAR *nCorInterval,
            /* [out] */ float __RPC_FAR *fRho) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_GetNumOrientations( 
            /* [out] */ long __RPC_FAR *nNumOrientations) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_StartProcess( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_StopProcess( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_ResetOrientation( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_SetInputPacket( 
            BSTR bstrInputPacket) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMotionTrackerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IMotionTracker __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IMotionTracker __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IMotionTracker __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IMotionTracker __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IMotionTracker __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IMotionTracker __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IMotionTracker __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_SetOutputMode )( 
            IMotionTracker __RPC_FAR * This,
            short nMode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_SetSampleFrequency )( 
            IMotionTracker __RPC_FAR * This,
            short nSampleFreq);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_SetCalibratedOutput )( 
            IMotionTracker __RPC_FAR * This,
            short nEnabled);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_SetCOMPort )( 
            IMotionTracker __RPC_FAR * This,
            short nPort);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_SetxmuLocationBSTR )( 
            IMotionTracker __RPC_FAR * This,
            BSTR bstrName);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_SetFilterSettings )( 
            IMotionTracker __RPC_FAR * This,
            float fGain,
            short nCorInterval,
            float fRho);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_SetInclination )( 
            IMotionTracker __RPC_FAR * This,
            float fInclination);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_SetDoAMD )( 
            IMotionTracker __RPC_FAR * This,
            short nDoAMD);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_GetOrientationData )( 
            IMotionTracker __RPC_FAR * This,
            /* [out] */ short __RPC_FAR *nRetVal,
            /* [out] */ VARIANT __RPC_FAR *pfOutputArray);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_GetCalibratedData )( 
            IMotionTracker __RPC_FAR * This,
            /* [out] */ short __RPC_FAR *nRetVal,
            /* [out] */ VARIANT __RPC_FAR *pfOutputArray);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_GetFilterSettings )( 
            IMotionTracker __RPC_FAR * This,
            /* [out] */ float __RPC_FAR *fGain,
            /* [out] */ short __RPC_FAR *nCorInterval,
            /* [out] */ float __RPC_FAR *fRho);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_GetNumOrientations )( 
            IMotionTracker __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *nNumOrientations);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_StartProcess )( 
            IMotionTracker __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_StopProcess )( 
            IMotionTracker __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_ResetOrientation )( 
            IMotionTracker __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_SetInputPacket )( 
            IMotionTracker __RPC_FAR * This,
            BSTR bstrInputPacket);
        
        END_INTERFACE
    } IMotionTrackerVtbl;

    interface IMotionTracker
    {
        CONST_VTBL struct IMotionTrackerVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMotionTracker_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMotionTracker_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMotionTracker_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMotionTracker_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IMotionTracker_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IMotionTracker_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IMotionTracker_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IMotionTracker_MT_SetOutputMode(This,nMode)	\
    (This)->lpVtbl -> MT_SetOutputMode(This,nMode)

#define IMotionTracker_MT_SetSampleFrequency(This,nSampleFreq)	\
    (This)->lpVtbl -> MT_SetSampleFrequency(This,nSampleFreq)

#define IMotionTracker_MT_SetCalibratedOutput(This,nEnabled)	\
    (This)->lpVtbl -> MT_SetCalibratedOutput(This,nEnabled)

#define IMotionTracker_MT_SetCOMPort(This,nPort)	\
    (This)->lpVtbl -> MT_SetCOMPort(This,nPort)

#define IMotionTracker_MT_SetxmuLocationBSTR(This,bstrName)	\
    (This)->lpVtbl -> MT_SetxmuLocationBSTR(This,bstrName)

#define IMotionTracker_MT_SetFilterSettings(This,fGain,nCorInterval,fRho)	\
    (This)->lpVtbl -> MT_SetFilterSettings(This,fGain,nCorInterval,fRho)

#define IMotionTracker_MT_SetInclination(This,fInclination)	\
    (This)->lpVtbl -> MT_SetInclination(This,fInclination)

#define IMotionTracker_MT_SetDoAMD(This,nDoAMD)	\
    (This)->lpVtbl -> MT_SetDoAMD(This,nDoAMD)

#define IMotionTracker_MT_GetOrientationData(This,nRetVal,pfOutputArray)	\
    (This)->lpVtbl -> MT_GetOrientationData(This,nRetVal,pfOutputArray)

#define IMotionTracker_MT_GetCalibratedData(This,nRetVal,pfOutputArray)	\
    (This)->lpVtbl -> MT_GetCalibratedData(This,nRetVal,pfOutputArray)

#define IMotionTracker_MT_GetFilterSettings(This,fGain,nCorInterval,fRho)	\
    (This)->lpVtbl -> MT_GetFilterSettings(This,fGain,nCorInterval,fRho)

#define IMotionTracker_MT_GetNumOrientations(This,nNumOrientations)	\
    (This)->lpVtbl -> MT_GetNumOrientations(This,nNumOrientations)

#define IMotionTracker_MT_StartProcess(This)	\
    (This)->lpVtbl -> MT_StartProcess(This)

#define IMotionTracker_MT_StopProcess(This)	\
    (This)->lpVtbl -> MT_StopProcess(This)

#define IMotionTracker_MT_ResetOrientation(This)	\
    (This)->lpVtbl -> MT_ResetOrientation(This)

#define IMotionTracker_MT_SetInputPacket(This,bstrInputPacket)	\
    (This)->lpVtbl -> MT_SetInputPacket(This,bstrInputPacket)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IMotionTracker_MT_SetOutputMode_Proxy( 
    IMotionTracker __RPC_FAR * This,
    short nMode);


void __RPC_STUB IMotionTracker_MT_SetOutputMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_SetSampleFrequency_Proxy( 
    IMotionTracker __RPC_FAR * This,
    short nSampleFreq);


void __RPC_STUB IMotionTracker_MT_SetSampleFrequency_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_SetCalibratedOutput_Proxy( 
    IMotionTracker __RPC_FAR * This,
    short nEnabled);


void __RPC_STUB IMotionTracker_MT_SetCalibratedOutput_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_SetCOMPort_Proxy( 
    IMotionTracker __RPC_FAR * This,
    short nPort);


void __RPC_STUB IMotionTracker_MT_SetCOMPort_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_SetxmuLocationBSTR_Proxy( 
    IMotionTracker __RPC_FAR * This,
    BSTR bstrName);


void __RPC_STUB IMotionTracker_MT_SetxmuLocationBSTR_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_SetFilterSettings_Proxy( 
    IMotionTracker __RPC_FAR * This,
    float fGain,
    short nCorInterval,
    float fRho);


void __RPC_STUB IMotionTracker_MT_SetFilterSettings_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_SetInclination_Proxy( 
    IMotionTracker __RPC_FAR * This,
    float fInclination);


void __RPC_STUB IMotionTracker_MT_SetInclination_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_SetDoAMD_Proxy( 
    IMotionTracker __RPC_FAR * This,
    short nDoAMD);


void __RPC_STUB IMotionTracker_MT_SetDoAMD_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_GetOrientationData_Proxy( 
    IMotionTracker __RPC_FAR * This,
    /* [out] */ short __RPC_FAR *nRetVal,
    /* [out] */ VARIANT __RPC_FAR *pfOutputArray);


void __RPC_STUB IMotionTracker_MT_GetOrientationData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_GetCalibratedData_Proxy( 
    IMotionTracker __RPC_FAR * This,
    /* [out] */ short __RPC_FAR *nRetVal,
    /* [out] */ VARIANT __RPC_FAR *pfOutputArray);


void __RPC_STUB IMotionTracker_MT_GetCalibratedData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_GetFilterSettings_Proxy( 
    IMotionTracker __RPC_FAR * This,
    /* [out] */ float __RPC_FAR *fGain,
    /* [out] */ short __RPC_FAR *nCorInterval,
    /* [out] */ float __RPC_FAR *fRho);


void __RPC_STUB IMotionTracker_MT_GetFilterSettings_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_GetNumOrientations_Proxy( 
    IMotionTracker __RPC_FAR * This,
    /* [out] */ long __RPC_FAR *nNumOrientations);


void __RPC_STUB IMotionTracker_MT_GetNumOrientations_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_StartProcess_Proxy( 
    IMotionTracker __RPC_FAR * This);


void __RPC_STUB IMotionTracker_MT_StartProcess_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_StopProcess_Proxy( 
    IMotionTracker __RPC_FAR * This);


void __RPC_STUB IMotionTracker_MT_StopProcess_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_ResetOrientation_Proxy( 
    IMotionTracker __RPC_FAR * This);


void __RPC_STUB IMotionTracker_MT_ResetOrientation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_SetInputPacket_Proxy( 
    IMotionTracker __RPC_FAR * This,
    BSTR bstrInputPacket);


void __RPC_STUB IMotionTracker_MT_SetInputPacket_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMotionTracker_INTERFACE_DEFINED__ */


#ifndef __IMotionTrackerEvents_INTERFACE_DEFINED__
#define __IMotionTrackerEvents_INTERFACE_DEFINED__

/* interface IMotionTrackerEvents */
/* [oleautomation][dual][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IMotionTrackerEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BB280866-7A0D-11D6-926B-008048B33483")
    IMotionTrackerEvents : public IDispatch
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE MT_OrientationChanged( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMotionTrackerEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IMotionTrackerEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IMotionTrackerEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IMotionTrackerEvents __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IMotionTrackerEvents __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IMotionTrackerEvents __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IMotionTrackerEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IMotionTrackerEvents __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_OrientationChanged )( 
            IMotionTrackerEvents __RPC_FAR * This);
        
        END_INTERFACE
    } IMotionTrackerEventsVtbl;

    interface IMotionTrackerEvents
    {
        CONST_VTBL struct IMotionTrackerEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMotionTrackerEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMotionTrackerEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMotionTrackerEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMotionTrackerEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IMotionTrackerEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IMotionTrackerEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IMotionTrackerEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IMotionTrackerEvents_MT_OrientationChanged(This)	\
    (This)->lpVtbl -> MT_OrientationChanged(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IMotionTrackerEvents_MT_OrientationChanged_Proxy( 
    IMotionTrackerEvents __RPC_FAR * This);


void __RPC_STUB IMotionTrackerEvents_MT_OrientationChanged_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMotionTrackerEvents_INTERFACE_DEFINED__ */



#ifndef __MotionTrackerFilter_LIBRARY_DEFINED__
#define __MotionTrackerFilter_LIBRARY_DEFINED__

/* library MotionTrackerFilter */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_MotionTrackerFilter;

EXTERN_C const CLSID CLSID_MotionTracker;

#ifdef __cplusplus

class DECLSPEC_UUID("BB280865-7A0D-11D6-926B-008048B33483")
MotionTracker;
#endif
#endif /* __MotionTrackerFilter_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long __RPC_FAR *, unsigned long            , VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long __RPC_FAR *, VARIANT __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
