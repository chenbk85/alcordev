#ifndef PI_SERVER_H
#define PI_SERVER_H
//-----------------------------------------------------------------------------
#include "PiInterface.h"
#include "PiGateway.h" //this must be the last include 
//-----------------------------------------------------------------------------
///
class PiServer{
public:
	//[constructor]
	PiServer(int);
	//[setters]
	void Reset(ArServerClient *, ArNetPacket *);
public:
	//[getters]
	//su richiesta del client il server 
	//preleva informazioni per poi spedirle al client
	void GetData(ArServerClient *,ArNetPacket *);
public: 
	void run();

	//[private section]
private:
	ArServerBase mServer;
	
public:
	PiGateway mPi;
	PiData mDataPi;	

//[Functor Callbacks]
protected:
	ArFunctor2C<PiServer, ArServerClient *, ArNetPacket *> mResetCB;	
protected:
	ArFunctor2C<PiServer, ArServerClient *, ArNetPacket *> mGetDataCB;
};
//-----------------------------------------------------------------------------

#endif //ShrimpServerH