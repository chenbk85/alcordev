#include "PiSink.h"
#include <cstdio>
//---------------------------------------------------------------------------
PiSink::PiSink(IUnknown* pUnkOuter,UINT nRefCnt, PiGateway* hand)
{
	// Zero the COM object's reference count.
	m_cRefs = 0;
	// No AddRef necessary if non-NULL, as we're nested.
	m_pUnkOuter = pUnkOuter;
	// Assign reference counter to sink
	m_nRefCnt = nRefCnt;

	piH = hand;
}
//---------------------------------------------------------------------------
PiSink::~PiSink(void)
{
}
//---------------------------------------------------------------------------
STDMETHODIMP PiSink::QueryInterface(REFIID riid,void** ppv)
{
	HRESULT hr = E_NOINTERFACE;
	
	*ppv = NULL;
	
	if (IID_IUnknown == riid)
		*ppv = this;
	else if (IID_IMotionTrackerEvents == riid)
		*ppv = static_cast<IMotionTrackerEvents*>(this);
	
	if (NULL != *ppv)
	{
		((LPUNKNOWN)*ppv)->AddRef();
		hr = NOERROR;
	}
	return (hr);
}
//---------------------------------------------------------------------------
STDMETHODIMP_(ULONG) PiSink::AddRef(void)
{
	ULONG cRefs;
	cRefs = InterlockedIncrement(&m_cRefs);
	return cRefs;
}
//---------------------------------------------------------------------------
STDMETHODIMP_(ULONG) PiSink::Release(void)
{
	ULONG cRefs;
	cRefs = InterlockedDecrement(&m_cRefs);
	
	if (0 == cRefs)
	{
		m_cRefs = InterlockedIncrement(&m_cRefs);;
		delete this;
	}
	
	return cRefs;
}
//---------------------------------------------------------------------------
STDMETHODIMP PiSink::MT_OrientationChanged(void)
{
	//printf("MT_OrientationChanged\n");
	piH->GetData();
	return S_OK;
}
//----------------------------------------------------------