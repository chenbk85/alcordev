#include "alcor.extern/xsens/MTComm.h"
//-------------------------------------------------------------------------++
#include <boost/config.hpp>
//-------------------------------------------------------------------------++
namespace all { namespace sense { namespace detail {

struct MTi_driver_impl
{
  MTi_driver_impl(){};

	unsigned char data[MAXMSGLEN];
	short datalen;
	float fdata[18];
  unsigned short samplecounter;

  CMTComm mtcomm;
  BOOST_STATIC_CONSTANT(int, SENSOR_BID=1);
};


}}}///all::sense::deteail