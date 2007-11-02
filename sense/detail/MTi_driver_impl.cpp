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

  float rpy[3];
  float quat[3];
  float acc[3];
  unsigned short samplecounter;

  unsigned short skip_factor;

  CMTComm mtcomm;
  BOOST_STATIC_CONSTANT(int, SENSOR_BID=BID_MT);
};


}}}///all::sense::deteail