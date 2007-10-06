#include "MTi_client_t.h"
#include "alcor/core/config_parser_t.hpp"
//
namespace all { namespace sense {
//
  MTi_client_t::MTi_client_t(const std::string& inifile)
  {
    //
  all::core::ip_address_t server_address;
  //
  all::core::config_parser_t config;
  config.load(core::tags::ini, inifile);
  //
  server_address.hostname =  config.get<std::string>("config.hostname");
  server_address.port = config.get<int>("config.comport");
  //
  set_server_addr(server_address);

  add_command_handler
    ("rpy_service", boost::bind(&all::sense::MTi_client_t::rpy_service_, this, _1));

  //set_connect_callback(boost::bind(&all::simple_client_t::on_connect, this));
  }

  void MTi_client_t::rpy_service_(all::core::net_packet_ptr_t incoming)
  {
    rpy_.roll.set_deg(incoming->buf_to_double());
    rpy_.pitch.set_deg(incoming->buf_to_double());
    rpy_.yaw.set_deg(incoming->buf_to_double());
  }

//
}}
//