#include "PiServer.h"

//[IMPLEMENT]
//-----------------------------------------------------------------------------
PiServer::PiServer(int pi_port_):
			mPi(pi_port_),
			mResetCB(this, &PiServer::Reset),
			mGetDataCB(this, &PiServer::GetData)
			
{
	
	// connessione alla piattaforma
	mPi.Init();
   	mPi.Start();//fai controlli
	// apertura porta
	if (!mServer.open(PiAddr.port))
	{
		cout <<"Could not open server port: " << PiAddr.port << endl;
		exit(1);
	}
	//[Assign Callbacks]
	mServer.addData(	"Reset", "reset zero", 
						&mResetCB,"none", "none");

	mServer.addData(   "GetData", "gets  data",
						&mGetDataCB,"none", "none");	
	mServer.runAsync();
}
//-----------------------------------------------------------------------------
void PiServer::run()
{
	//mServer.runAsync();
}
//-----------------------------------------------------------------------------
void PiServer::Reset(ArServerClient *client, ArNetPacket *packet)
{
	mPi.Reset();
}
//-----------------------------------------------------------------------------


void PiServer::GetData(ArServerClient *client, ArNetPacket *p)
{
	
	ArNetPacket packet;
 
	mDataPi.mroll = mPi.GetRoll(); 
	mDataPi.mpitch = mPi.GetPitch();
	mDataPi.myaw = mPi.GetYaw();
			

	mDataPi.pack(&packet);
	client->sendPacketTcp(&packet);
}
////-----------------------------------------------------------------------------
//
//



