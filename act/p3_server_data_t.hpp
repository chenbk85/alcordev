#ifndef p3_server_data_t_HPP_INCLUDED
#define p3_server_data_t_HPP_INCLUDED.hpp
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
}}//all::act
//-------------------------------------------------------------------
inline p3_server_data_t::import(core::net_packet_ptr_t packet)
{
}
//-------------------------------------------------------------------
inline p3_server_data_t::pack(core::net_packet_ptr_t packet)
{
}
//-------------------------------------------------------------------
#endif //p3_server_data_t_HPP_INCLUDED