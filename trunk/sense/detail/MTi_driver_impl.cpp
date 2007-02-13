#include "alcor.extern/xsens/MTComm.h"

namespace all { namespace sense { namespace detail {

struct MTi_driver_impl
{
  MTi_driver_impl(){};

	unsigned char data[MAXMSGLEN];
	short datalen;
	float fdata[18];

  CMTComm mtcomm;
};


}}}///all::sense::deteail