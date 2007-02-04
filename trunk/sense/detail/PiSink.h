#ifndef PiSinkH
#define PiSinkH
//--------------------------------------------------------------------------------
#include "PiGateway.h"
//--------------------------------------------------------------------------------
class PiSink : public IMotionTrackerEvents
{
public:
    // Main Object Constructor & Destructor.
    PiSink(IUnknown* pUnkOuter, UINT m_nRefCnt, PiGateway *hand);
    ~PiSink(void);
	
    // IUnknown methods. Main object, non-delegating.
    STDMETHODIMP         QueryInterface(REFIID, void**);
    STDMETHODIMP_(ULONG) AddRef(void);
    STDMETHODIMP_(ULONG) Release(void);
	STDMETHODIMP GetTypeInfoCount( 
            /* [out] */ UINT __RPC_FAR *pctinfo)
	{
		return E_NOTIMPL;
	};
        
    STDMETHODIMP GetTypeInfo( 
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo)
	{
		return E_NOTIMPL;
	};
        
    STDMETHODIMP GetIDsOfNames( 
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId)
	{
		return E_NOTIMPL;
	};
        
    STDMETHODIMP Invoke( 
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr)
	{
		return E_NOTIMPL;
	};
	
private:
    // IMotionTrackerSink methods.
	STDMETHODIMP		MT_OrientationChanged(void);
		
     // Main Object reference count.
    long				m_cRefs;
	
    // Outer unknown (aggregation delegation). Used when this COM object
    // is being aggregated.
    IUnknown*			m_pUnkOuter;
	
    // Pointer to the main object that can service the Sink events.
    PiGateway*			piH;
	
	// Indicates which reference belongs to this sink
	UINT m_nRefCnt;
};
//--------------------------------------------------------------------------------
#endif // SINK_H
