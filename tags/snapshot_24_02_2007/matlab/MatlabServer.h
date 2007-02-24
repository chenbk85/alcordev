#ifndef MATLAB_SERVER_H
#define MATLAB_SERVER_H
//-----------------------------------------------------------------------------
#include "alcor/def/CommonDef.h"
//-----------------------------------------------------------------------------
#include "alcor/matlab/MatlabEngine.h"
//-----------------------------------------------------------------------------
#include <string>
using std::string;
//-----------------------------------------------------------------------------
//using namespace alcor::matlab;
//-----------------------------------------------------------------------------
/////
class MatlabServer
{
public:
	//[constructor]
	MatlabServer(alcor::types::tServiceAddr);

private:	
	//[direct matlab access]
	//Use in local mode ...
	void doCommandLine	(ArServerClient *, ArNetPacket *);
	void doStopServer	(ArServerClient *, ArNetPacket *);
	void doGetScalar	(ArServerClient *, ArNetPacket *);

public: 
	void init();
	void runServer();


	//[private section]
private:
	alcor::matlab::MatlabEngine matlab;
	ArServerBase mServer;
	alcor::types::tServiceAddr myaddr;
	
//[Callbacks]
protected:
	ArFunctor2C<MatlabServer, ArServerClient *, ArNetPacket *> mCommandLineCB;
	ArFunctor2C<MatlabServer, ArServerClient *, ArNetPacket *> mStopServerCB;
	ArFunctor2C<MatlabServer, ArServerClient *, ArNetPacket *> mGetScalarCB;
};
//-----------------------------------------------------------------------------

#endif //MATLAB_SERVER_H