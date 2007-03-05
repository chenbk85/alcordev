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
}
//-------------------------------------------------------------------
inline void p3_server_data_t::pack(core::net_packet_ptr_t packet)
{
}
//-------------------------------------------------------------------
}}//all::act
//-------------------------------------------------------------------
#endif //p3_server_data_t_HPP_INCLUDED