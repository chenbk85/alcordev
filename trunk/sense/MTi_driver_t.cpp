#include "MTi_driver_t.h"
#include "alcor/core/iniwrapper.h"
#include "alcor.extern/xsens/MTComm.h"

namespace all { namespace sense {
//
MTi_driver_t::MTi_driver_t()
{
  mtcomm.reset( new CMTComm());
}

//
bool MTi_driver_t::open(std::string& configfile)
{

}

//
void MTi_driver_t::reset(int)
{
}

//
bool MTi_driver_t::euler(all::math::rpy_angle_t& rpy)
{
}










}}