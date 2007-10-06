#ifndef MTi_client_t_H_INCLUDED
#define MTi_client_t_H_INCLUDED
//-------------------------------------------------------------------
//-------------------------------------------------------------------
#include "alcor/core/client_base_t.hpp"
#include "alcor/math/rpy_angle_t.h"
//-------------------------------------------------------------------
namespace all { namespace sense {
//-------------------------------------------------------------------
///
class MTi_client_t  : public all::core::client_base_t
{
public:
  MTi_client_t(const std::string& inifile);

private:
  ///
  void rpy_service_(core::net_packet_ptr_t);
  ///
  all::math::rpy_angle_t rpy_;
};
//-------------------------------------------------------------------
}}//all::sense
//-------------------------------------------------------------------


#endif //MTi_client_t_H_INCLUDED