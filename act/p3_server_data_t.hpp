#ifndef p3_server_data_t_HPP_INCLUDED
#define p3_server_data_t_HPP_INCLUDED
//-------------------------------------------------------------------
#include <alcor/core/net_packet_t.hpp>
#include "alcor/math/pose2d.h"
//-------------------------------------------------------------------
namespace all { namespace act {
//-------------------------------------------------------------------
struct p3_server_data_t
{
  void import(core::net_packet_ptr_t);
  void pack(core::net_packet_ptr_t);

  math::pose2d pose;
};

//-------------------------------------------------------------------
inline void p3_server_data_t::import(core::net_packet_ptr_t packet)
{
  pose.set_x1(packet->buf_to_double());
  pose.set_x2(packet->buf_to_double());
  pose.set_th(packet->buf_to_double(),all::math::deg_tag);
}
//-------------------------------------------------------------------
inline void p3_server_data_t::pack(core::net_packet_ptr_t packet)
{
  packet->double_to_buf(pose.get_x1());
  packet->double_to_buf(pose.get_x2());
  packet->double_to_buf(pose.get_th(all::math::deg_tag)); 
	packet->finalize_packet();
}
//-------------------------------------------------------------------
}}//all::act
//-------------------------------------------------------------------
#endif //p3_server_data_t_HPP_INCLUDED