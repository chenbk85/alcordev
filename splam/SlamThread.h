#ifndef SLAMTHREAD_H
#define SLAMTHREAD_H
//-----------------------------------------------------------------------------------------------
#include <iostream>
#include <fstream>
//-----------------------------------------------------------------------------------------------
#include "Aria.h"
#include "ArNetworking.h"
#include "alcor/types/Defs.h"
#include "alcor/slam/SlamWorker.h"
#include "alcor/slam/ScanData.h"
#include "alcor/slam/SlamData.h"
#include "alcor/slam/SlamDataNet.h"
#include "alcor/tools/AriaStarter.h"
#include "alcor/devices/urg/UrgDriver.h"
#include "alcor/extern/iniWrapper/iniWrapper.h"
//#include "alcor/devices/pioneer/p3dx_client.h"
#include "alcor2/act/p3dx_client_t.h"
#include "alcor2/core/i_connection_handler.h"
#include "alcor/extern/pmap/slap.h"
#include "alcor/slam/ValueIteration.h"
//-----------------------------------------------------------------------------------------------
class SlamThread: public ArASyncTask 
{
public:	//typedefs
	typedef ArFunctor2C<SlamThread,ArServerClient*, ArNetPacket*>	Functor;
	typedef	std::vector< std::pair<double,double> >					VectPairDouble;
	typedef	VectPairDouble::iterator								VectPairDoubleIT;

public:	//ctor, dtor, copy ctor, copy assign
	SlamThread(char* name = "./config/slam.ini");
	~SlamThread();
    
public:	//Laser Driver
	int					laserComPort;

public:	//Robot Client
	boost::shared_ptr<all::act::p3dx_client_t>			robot;

public:	//locks
	int					ScanDataLock();
	int					ScanDataUnlock();
	int					SlamDataLock();
	int					SlamDataUnlock();
	int					PmapLock();
	int					PmapUnlock();
private:
	ArMutex				ScanDataMutex;
	ArMutex				SlamDataMutex;
	ArMutex				Pmap;

public:	//thread side
	virtual void*		runThread(void*);
	void				stop(){stopRunning();}

public:	//Data Acquisition
	void				AcquireAll();
	void				Filter();//le precondizioni è che lo ScanData sia già pieno
	std::vector<int>    mask;
	void				SetMask(VectPairDouble);
private:
	void				AcqScan();	//internal
	void				AcqOdomPose();	//internal
	void				EstimateOdometry();
	ScanData			currentScan_;
	ScanData			lastScan_;
	size_t				scanCount_;
	size_t				urg_step_start_;
	size_t				urg_step_end_;
	size_t				urg_cc_;

public:	//Data Processing
	void				Process();
	void				FillMap();
	void				GoalFinding();
private:
	SlamWorker			worker;
	SlamData			slamd;
	ValueIteration		valueIteration_;

public:	//Data Broadcasting
	void				StartServer();
	void				StopServer();
	void				BroadcastData();
	void				UpdatePoseOnRobot();
	void				Mappa(ArServerClient* ,ArNetPacket*);
	void				Others(ArServerClient* ,ArNetPacket*);
	void				Victim(ArServerClient* ,ArNetPacket*);
private:
	Functor				mappaCB;
	Functor				othersCB;
	Functor				victimCB;
	ArServerBase		server_;
	portType			port_;
	SlamDataNet			*slamDataNet_;
	bool				odoPresent_;
	DoublePair			victimPosition_;
	ArTypes::Byte		victimCount_;

public:
	std::ofstream		logFile_;
	iniWrapper			ini_;

};



#endif