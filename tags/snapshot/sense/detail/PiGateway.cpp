//#include "Aria.h"
#include "PiGateway.h"
//-------------------------------------------------------------------/
#include "stdio.h"		// Needed for printf etc
//#include "objbase.h"	// Needed for COM functionality
// import functions in MT object
// GUIDs of MT object
#include "IMTObj_i.c"
//-------------------------------------------------------------------/
#include "PiSink.h"
//-------------------------------------------------------------------/
#include <cmath>
//-------------------------------------------------------------------/
#include <iostream>
//-------------------------------------------------------------------/
using namespace std;
//-------------------------------------------------------------------/
//-------------------------------------------------------------------/
PiGateway::PiGateway(int port_):port(port_)
{
	dSink = 0;		
	pSink = NULL;
	pMT=NULL;
	mode=MT_LOGEULER;
	calibratedOutput=1;

    xmuFile= L"config/2096_08052003_001.xmu";  //default setting
	gain=1.0;
	corInterval=1;
	rho=1.0;
	amd=1;
	sampleFreq=100;

	//init rotationMatrix
	for (int i=0; i<2; i++)
		eulerData[i]=0;

	//init calibratedData
    for (int i=0; i<10; i++)
        calibratedData[i]=0;
	
}
//-------------------------------------------------------------------/
PiGateway::~PiGateway()
{   
	if(pSink!=NULL)
	{ pSink->Release();
	  pSink=NULL;
	}

	if (pMT!=NULL)
	{ pMT->Release();
	  pMT=NULL;
	}
    
	CoUninitialize();
}
//-------------------------------------------------------------------/
bool PiGateway::Init()
{    
	//BSTR temp;

	//CoInitialize(NULL);
	if (CoInitialize(NULL) != S_OK)
		printf("Failed to initialize COM library");

	HRESULT hRes=CoCreateInstance (CLSID_MotionTracker, NULL, CLSCTX_SERVER, IID_IMotionTracker, (void **)&pMT);
	if (FAILED(hRes))
	{
		cout <<"Error create Com object"<< hRes;
		return FALSE;
	}
   
	PiSink *pCob = NULL;
	if (FAILED(hRes))
	{	hRes=E_OUTOFMEMORY;
		cout << "Error initialize Sink" << hRes;
		return FALSE;
	}
	
	pCob= new PiSink(NULL,0,this);
	if (NULL!=pCob)
	   { pSink=pCob;
	     pSink->AddRef();
	   }

   pMT->MT_SetOutputMode(mode);
   pMT->MT_SetCalibratedOutput(calibratedOutput);
   pMT->MT_SetCOMPort(port);

   pMT->MT_SetFilterSettings(gain, corInterval, rho);
   pMT->MT_SetDoAMD(amd);
   pMT->MT_SetSampleFrequency(sampleFreq);

   	BSTR bstrIn;
	bstrIn = ::SysAllocString(xmuFile);
   	// Set .XMU file location
	pMT->MT_SetxmuLocationBSTR(bstrIn);	
	// Free allocated string
	::SysFreeString(bstrIn);

   cout<<"Initialization done!\n";
   return TRUE;

}
//-------------------------------------------------------------------/
void PiGateway::SetComPort(short m_port)
{  
   if (pMT==NULL) port=m_port;
   else  pMT->MT_SetCOMPort(port);
}
//-------------------------------------------------------------------/
void PiGateway::Reset()
 { if (pMT!=NULL) pMT->MT_ResetOrientation();
   else cout<< "Do the Init() first" << endl;
 }
//-------------------------------------------------------------------/
IConnectionPoint* PiGateway::GetConnectionPoint(REFIID riid)
{
	// Internal protected method to obtain a connection point interface.

	IConnectionPointContainer* pConnPointContainer = NULL;
	IConnectionPoint* pConnPoint = NULL;
	IConnectionPoint* pConnPt = NULL;
	HRESULT hr;
	
	hr = pMT->QueryInterface(IID_IConnectionPointContainer,(void**)&pConnPointContainer);
	
	if SUCCEEDED(hr)
	{
		hr = pConnPointContainer->FindConnectionPoint(riid, &pConnPt);
		if (SUCCEEDED(hr))
		pConnPoint = pConnPt;
		pConnPointContainer->Release();
		pConnPointContainer = NULL;
	}
	return pConnPoint;
}
//---------------------------------------------------------------------------

HRESULT PiGateway::ConnectMTSink(void)
{
	HRESULT hr = E_FAIL;
	DWORD	dwKey;
	IConnectionPoint* pConnPoint;

	if (!dSink)
	{
		pConnPoint = GetConnectionPoint(IID_IMotionTrackerEvents);
		if (NULL != pConnPoint)
		{
			hr = pConnPoint->Advise(pSink, &dwKey);
			if (SUCCEEDED(hr))
				dSink = dwKey;

			pConnPoint->Release();
			pConnPoint = NULL;
		}
	}
	return hr;
}
//---------------------------------------------------------------------------
HRESULT PiGateway::DisconnectMTSink(void)
{
	HRESULT hr = E_FAIL;
	IConnectionPoint* pConnPoint;
	
	if (dSink)
	{
		pConnPoint = GetConnectionPoint(IID_IMotionTrackerEvents);
		if (NULL != pConnPoint)
		{
			hr = pConnPoint->Unadvise(dSink);
			if (SUCCEEDED(hr))
				dSink = 0;
			
			pConnPoint->Release();
			pConnPoint = NULL;
		}
	}
	return hr;
}
//---------------------------------------------------------------------------	
bool PiGateway::Start()
{   Sleep(5000);    
	cout<<"Connecting with the sink object..."<<endl;
	ConnectMTSink();
    if((pMT->MT_StartProcess()) == E_ACCESSDENIED)
	{
		cout<<"Error starting process!!!"<<endl;
		DisconnectMTSink();
		return FALSE;
	}
	return TRUE;
}
//-------------------------------------------------------------------/
bool PiGateway::Stop()
{ 
	cout<<"Disconnecting from the sink object..."<<endl;
  DisconnectMTSink();
  pMT->MT_StopProcess();
  return TRUE;
}
//-------------------------------------------------------------------/
void PiGateway::SetXmuLocation (wchar_t* location)
{ BSTR temp;

  xmuFile=location;
  temp = ::SysAllocString(xmuFile);
  pMT->MT_SetxmuLocationBSTR(temp);
  ::SysFreeString(xmuFile);
}
//-------------------------------------------------------------------/
bool PiGateway::GetData()
{   VARIANT OrientationBuffer;
    void* pDest;
	short nNew = 0;
    pMT->MT_GetOrientationData(&nNew, &OrientationBuffer);
    if (nNew == MT_NEWDATA) 
	{
		// Check if array is not empty
		if (OrientationBuffer.vt != VT_EMPTY)
		{
			// Retreive pointer to array data
			HRESULT hr = SafeArrayAccessData(OrientationBuffer.parray, &pDest);
			// One dimensional array. Get the bounds for the array.
					         
			if (SUCCEEDED(hr))
			{				
				__try {
					memcpy(eulerData,pDest,(OrientationBuffer.parray->rgsabound->cElements * sizeof(float)));
					
				}
				__except(GetExceptionCode() == STATUS_ACCESS_VIOLATION){
					
				}	
			
				SafeArrayUnaccessData(OrientationBuffer.parray);	// Invalidate pointer
	          
				
			}
			else
				return FALSE;
		}
		else
			return FALSE;
	}
	else if (nNew != 0)
	{		
		// Check if error was reported by MotionTracker object
		switch(nNew) {
		case MT_NODATA:
			printf("No Data On COM Port\n\n");
			printf("trying to reconnect\n\n");
			Start();
			break;
		case MT_NOSENSORID:
			printf("No Sensor ID Received From Sensor\n\n");
            printf("trying to reconnect\n\n");
			Start();
			break;
		case MT_INCOMPLETE:
			printf("Incomplete Data Received (Connection Lost)\n\n");
			printf("trying to reconnect\n\n");
			Start();
			break;
		case MT_CHECKSUMERROR:
			printf("Checksum Error\n\n");
			printf("trying to reconnect\n\n");
			Start();
			break;
		case MT_NOPORT:
			printf("COM Port Could Not Be Opened\n\n");
			printf("trying to reconnect\n\n");
			Start();
			break;
		case MT_NOCALIBVALUES:
			printf("XMU File With Calibration Data Could Not Be Read\n\n");
			printf("trying to reconnect\n\n");
			Start();
			break;						
		}
		
		return FALSE;
	}
	pMT->MT_GetCalibratedData(&nNew, &OrientationBuffer);
	if (nNew == MT_NEWDATA) 
	{
		// Check if array is not empty
		if (OrientationBuffer.vt != VT_EMPTY)
		{
			// Retreive pointer to array data
			HRESULT hr = SafeArrayAccessData(OrientationBuffer.parray, &pDest);
			// One dimensional array. Get the bounds for the array.
					         
			if (SUCCEEDED(hr))
			{				
				__try {
					memcpy(calibratedData,pDest,(OrientationBuffer.parray->rgsabound->cElements * sizeof(float)));
					
				}
				__except(GetExceptionCode() == STATUS_ACCESS_VIOLATION){
					
				}	
			
				SafeArrayUnaccessData(OrientationBuffer.parray);	// Invalidate pointer
	
				
				
			}
			else
				return FALSE;
		}
		else
			return FALSE;
	}
	return FALSE;
}
//-------------------------------------------------------------------/
float PiGateway::GetRoll() const
{   
	return (eulerData[0]);
	//return (atan(rotationMatrix[5]/rotationMatrix[8])*180.0/M_PI);
}         
//-------------------------------------------------------------------/
float PiGateway::GetPitch()const

{ 
	return (-eulerData[1]);
	//return    (asin(rotationMatrix[2]))*180.0/M_PI;
}
//-------------------------------------------------------------------/
float PiGateway::GetYaw()const
{   
	return (eulerData[2]);
	//return(atan(rotationMatrix[1]/rotationMatrix[0])*180.0/M_PI);
}
//-------------------------------------------------------------------/

