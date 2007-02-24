#ifndef MatlabClientH
#define MatlabClientH
//--------------------------------------------------------------------------+
#include "alcor/net/AbstractConnectionHandler.h"
//--------------------------------------------------------------------------+
#include "alcor/exception/SystemException.hpp"
//--------------------------------------------------------------------------+
//#include <exception>
//--------------------------------------------------------------------------+
//--------------------------------------------------------------------------+
namespace alcor {
	namespace matlab {
//--------------------------------------------------------------------------+
//--------------------------------------------------------------------------+
class MatlabClient : public alcor::net::AbstractConnectionHandler
{
public: 
	///Ctor.
	MatlabClient(char*);
	
//[connect to service]
private:
	//La classe client ... si definisce qua ma poi la si passa alla classe genitore...
	//che ha il compito di monitorare la connessione...
	ArClientBase m_client;

	//todo: cambiare accesso ...
public:
	//AbstractConnectionHandler invochera' questo metodo non appena il server remoto
	//risponde
	virtual void registerTo();
	///Non fa altro che chiamare il metodo handle dello stato corrente...
	virtual void* runThread(void*);
	///
	virtual void lostConnection(){};

private:
	///Locks received double flag...
	ArMutex lockReceivedScalar;
	///flag
	bool bReceivedScalar;
	///flag 
	bool bNotExist;
	///Memorize last received scalar double
	double dRecvValue;
	///Internal Callback
	void getScalarDouble(ArNetPacket*);
	
//External Interface
public:
	///CommandLine
	void commandLine(std::string);

	//Changehost 'on the fly'
	void chHost(alcor::types::tServiceAddr);

	///Stop Remote ...
	void  stopRemoteServer();

	///Get a named double value.
	double  getScalarValue(char*);

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//[wrapper functors]
protected:
	///A Dummy Class ...
	ArFunctor1C< MatlabClient, ArNetPacket* > mGetScalarDoubleCB;

};
//--------------------------------------------------------------------------+
//--------------------------------------------------------------------------+
	}
}
//--------------------------------------------------------------------------+
//--------------------------------------------------------------------------+
#endif //RemoteClientConnector
