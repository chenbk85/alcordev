#ifndef COMMONDEFH
#define COMMONDEFH
//--------------------------------------------------------------------------+
#include <iostream>
#include <Aria.h>
#include <ArNetworking.h>
//--------------------------------------------------------------------------+
#include "alcor/deprecated/types.h"
//--------------------------------------------------------------------------+
using std::cout;
using std::endl;
//--------------------------------------------------------------------------+
 namespace alcor
 {
	 namespace def
	 {
//---------------------------------------------------------------------------+
//static const alcor::types::tServiceAddr MatlabServiceAddr = {"localhost",11111};
//---------------------------------------------------------------------------+
static const int kMatlabCommandLength = 100;
//---------------------------------------------------------------------------+
//---------------------------------------------------------------------------+
static const alcor::types::tErrorCode		op_Ok		= 0x00;
static const alcor::types::tErrorCode		op_Failed	= 0x01;
static const alcor::types::tErrorCode		op_Unknown	= 0x02;
//---------------------------------------------------------------------------+
//---------------------------------------------------------------------------+
static const alcor::types::tStateId     eEstablished	= 0x00;
static const alcor::types::tStateId		eActiveOpen		= 0x01;
static const alcor::types::tStateId		eLinkDown		= 0x02;
//---------------------------------------------------------------------------+
	 }//namesapce def
 }//na
//---------------------------------------------------------------------------+
#endif //COMMONDEFH
