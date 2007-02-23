#include "MatlabServer.h"
//-----------------------------------------------------------------------------
using namespace alcor::matlab;
//[IMPLEMENT]
//-----------------------------------------------------------------------------
MatlabServer::MatlabServer(alcor::types::tServiceAddr addr):
			myaddr(addr),
			mCommandLineCB(this, &MatlabServer::doCommandLine),
			mStopServerCB(this, &MatlabServer::doStopServer),
			mGetScalarCB(this,  &MatlabServer::doGetScalar)  
{

}
//-----------------------------------------------------------------------------
void MatlabServer::runServer()
{
	mServer.run();
}
//-----------------------------------------------------------------------------
void MatlabServer::init()
{
	// first open the server up
	if (!mServer.open(myaddr.port))
		{
			printf("Could not open server port: %d\n", myaddr.port );
			exit(1);
		}

	//[Assign Callbacks]
	mServer.addData("commandLine", "Command Line Service", 
						&mCommandLineCB,"String", "none");
	mServer.addData("stopServer", "Close Server Thread", 
						&mStopServerCB,"none", "none");	
	mServer.addData("getScalar", "Get a Named Scalar Value From Workspace",
						&mGetScalarCB, "char", "Double");
}
//-----------------------------------------------------------------------------
void MatlabServer::doStopServer(ArServerClient *client, ArNetPacket *packet)
{
	printf("doStopServer\n");
	mServer.stopRunning();

}
//-----------------------------------------------------------------------------
void MatlabServer::doCommandLine(ArServerClient *client, ArNetPacket *packet)
{
	//printf("doCommandLine\n");
	char cmd[alcor::def::kMatlabCommandLength];
	int ntokens = packet->bufToByte2();
	printf("%d Tokens\n", ntokens);
	packet->bufToStr(cmd, sizeof(cmd));
	//printf("Calling matlab gw to handle: %s\n", cmd);
	matlab.commandLine(cmd);
}
////-----------------------------------------------------------------------------
void MatlabServer::doGetScalar(ArServerClient* client, ArNetPacket* packet)
{

	if(packet->getDataLength() > packet->getHeaderLength())
	{
		char var[50];
		packet->bufToStr(var, sizeof(var));

		ArNetPacket answ;

		try{
			double value = matlab.getScalarDouble(var);
			answ.byteToBuf(alcor::def::op_Ok);
			answ.doubleToBuf(value);
		}
		catch (alcor::err::WorkspaceError e)
		{
			answ.byteToBuf(alcor::def::op_Failed);
			printf(e.what());
		}
		answ.finalizePacket();
		client->sendPacketTcp(&answ);
	}
}
////-----------------------------------------------------------------------------

