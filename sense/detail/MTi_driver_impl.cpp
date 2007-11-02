#include "alcor.extern/xsens/MTComm.h"
//-------------------------------------------------------------------------++
#include <boost/config.hpp>
//-------------------------------------------------------------------------++
namespace all { namespace sense { namespace detail {
///
struct MTi_driver_impl
{
  ///
  MTi_driver_impl(){};
  ///
	unsigned char data[MAXMSGLEN];
  ///
	short datalen;

  ///
	float fdata[18];

  ///[roll pitch yaw] (xyz)degrees
  float rpy[3];

  ///[q0  q1  q2  q3] Qgs
  ///[q0 -q1 -q2 -q3] Qsg
  float quat[4];

  //[a b c d e f g h i]
  //GS [a d g; b e h; c f i] //MTi(S) to global(G)
  //SG [a b c;d e f; g h i]  //global(G) to MTi(S)
  float rot[9];
  ///
  float acc[3];
  ///
  unsigned short samplecounter;
  ///
  int output_settings;
  int output_mode;
  ///
  unsigned short skip_factor;
  ///
  CMTComm mtcomm;
  ///
  BOOST_STATIC_CONSTANT(int, SENSOR_BID=BID_MT);
};


}}}///all::sense::deteail