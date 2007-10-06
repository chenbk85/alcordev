#ifndef MTi_server_t_H_INCLUDED
#define MTi_server_t_H_INCLUDED
//-------------------------------------------------------------------
#include "alcor/sense/MTi_driver_t.h"
//-------------------------------------------------------------------
#include "alcor/core/server_base_t.hpp"
//-------------------------------------------------------------------
namespace all { namespace sense {
//-------------------------------------------------------------------
///
class MTi_server_t:public core::server_base_t
{
public:
  ///ctor
  MTi_server_t(const std::string& ini = "mti_service.ini");
  ///Mandatory
  void set_device_ptr(all::sense::MTi_driver_ptr);

private:
  ///RPY service
  void rpy_service_(core::client_connection_ptr_t, core::net_packet_ptr_t);

private:
  ///MTi device
  all::sense::MTi_driver_ptr mti_;
};
//-------------------------------------------------------------------
}}//all::sense
//-------------------------------------------------------------------
#endif //MTi_server_t_H_INCLUDED