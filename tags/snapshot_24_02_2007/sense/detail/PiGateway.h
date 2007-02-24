#ifndef PIGATEWAY_H
#define PIGATEWAY_H
//-------------------------------------------------------------------/
//#include "CommonDef.h"
#include "objbase.h"	// Needed for COM functionality
//-------------------------------------------------------------------
//// import functions in MT object
//#import "MTObj.dll"
#include "IMTObj.h"
//-------------------------------------------------------------------
//[Define]
#define MT_NEWDATA			1
#define MT_NODATA			2
#define MT_NOSENSORID		3
#define MT_INCOMPLETE		4
#define MT_CHECKSUMERROR	5
#define MT_NOPORT			6
#define MT_NOCALIBVALUES	7
//-------------------------------------------------------------------/
// output possiblities for MT object
#define MT_LOGQUATERNION	0
#define MT_LOGEULER			1
#define MT_LOGROTMATRIX		2
//-------------------------------------------------------------------/
class PiGateway
{
//[Constructor]
public:
	PiGateway(int);
	~PiGateway();

//[Setters]
public:
	bool Init();    //Create intance of Com Object 
	void SetComPort(short);
	void SetXmuLocation (wchar_t*);
	void Reset();
	bool Start();
	bool Stop();
    
	HRESULT				ConnectMTSink(void);				
	HRESULT				DisconnectMTSink(void);				
	IConnectionPoint*	GetConnectionPoint(REFIID riid);

//[Getters]
public:
   bool  GetData();
   float GetRoll() const;
   float GetPitch()const;
   float GetYaw() const;
   
      
private:
	IMotionTracker *pMT;
	IUnknown*			pSink;	
	DWORD				dSink;	
	float		eulerData[3];
	float		calibratedData[10];
	short		mode;
	short		calibratedOutput;
	short		port;
	wchar_t*	xmuFile;
	float		gain;
	short		corInterval;
	float		rho;
	short		amd;
	short		sampleFreq;

};
#endif

