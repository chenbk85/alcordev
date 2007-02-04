#include "MatlabClient.h"
//--------------------------------------------------------------------------+
//--------------------------------------------------------------------------+
namespace alcor {
	namespace matlab {
//--------------------------------------------------------------------------+
//--------------------------------------------------------------------------+
//--------------------------------------------------------------------------+
MatlabClient::MatlabClient(char* inifile):

	alcor::net::AbstractConnectionHandler(&m_client, inifile),
	mGetScalarDoubleCB(this, &MatlabClient::getScalarDouble),
	bReceivedScalar(false),
	bNotExist(false)
	{

	}
//--------------------------------------------------------------------------+
void MatlabClient::registerTo()
{
	m_client.addHandler("getScalar", &mGetScalarDoubleCB);
	m_client.request("getScalar", 50);
}
//--------------------------------------------------------------------------+
double  MatlabClient::getScalarValue(char* arg)
{
	if( thisState() == alcor::def::eActiveOpen )
	{	
		printf("Requesting val: %s\n", arg);
		ArNetPacket pkt;
		pkt.strToBufPadded(arg, 50);
		pkt.finalizePacket();
		m_client.requestOnce("getScalar", &pkt);

		while (	thisState() == alcor::def::eActiveOpen 
				&& !bReceivedScalar
				&& !bNotExist
				)
		{

			ArUtil::sleep(15);

		}
		if (bReceivedScalar)
		{
			//reset
			bReceivedScalar = false;
			return dRecvValue;
		}
		else if (bNotExist)
		{
			//reset flag
			bNotExist = false;
			//var does not exist!
			throw alcor::err::WorkspaceError();
		}
	}
	else
	{
		//Server is off-line
		throw alcor::err::ServerOffline();
	}
	//Dummy return value
	return 0;
}
//--------------------------------------------------------------------------+
void MatlabClient::getScalarDouble(ArNetPacket* packet)	
{
	printf("getScalarDouble\n");
	if(packet)
	{
		
		alcor::types::tErrorCode err = packet->bufToByte();
		if(err == alcor::def::op_Ok)
		{
			dRecvValue = packet->bufToDouble();
			printf("Value is: %f\n",dRecvValue);

			//lockReceivedScalar.lock();
				bReceivedScalar = true;
			//lockReceivedScalar.unlock();
		}
		else
		{
			bNotExist = true;
		}
		
	}
}
//--------------------------------------------------------------------------+
void* MatlabClient::runThread(void*)
{
	cout << "Client Thread Created!" << endl;
	while( getRunning() )
	{
//		cout << "RasRemoteTaskInterfece Thread Running.." << endl;
		m_state->handle( dynamic_cast<AbstractConnectionHandler*>(this) );
		ArUtil::sleep(200);
	}

	return NULL;
}
////--------------------------------------------------------------------------+
////--------------------------------------------------------------------------+
void  MatlabClient::commandLine(std::string cmd)
{
	printf("Command is: %s\n",cmd.c_str());

	if(!cmd.empty())
	{
		//char search_string[]= cmd.c_str();
		char* tokens[50];
		int loop;
		//std::string command = "";

		tokens[0]=strtok((char*)cmd.c_str()," ");

		if(tokens[0]!=NULL)
		{
			
			for(loop=1;loop<50;loop++)
			{
				tokens[loop] = strtok(NULL," ");
				if(tokens[loop]==NULL)
					break;
			}

			printf("Token to transmit: %d\n", loop+1);

			ArNetPacket arg;
			arg.byte2ToBuf(loop+1);
			arg.strToBufPadded(cmd.c_str(), alcor::def::kMatlabCommandLength);
			arg.finalizePacket();//non mi ricordo come si chiama esattamente
			m_client.requestOnce("commandLine", &arg);
		}
		else
			printf("No test to search.\n");
	}
}
////--------------------------------------------------------------------------+
void  MatlabClient::stopRemoteServer()
{
	m_client.requestOnce("stopServer", NULL);
}
////--------------------------------------------------------------------------+
void MatlabClient::chHost(alcor::types::tServiceAddr addr)
{
	if(getRunning())
	{
		ArUtil::sleep(200);
		m_client.disconnect();
	}
	m_addr = addr;
}
//--------------------------------------------------------------------------+
//--------------------------------------------------------------------------+
//Closing Namespace
//--------------------------------------------------------------------------+
	}
}
//--------------------------------------------------------------------------+
//--------------------------------------------------------------------------+