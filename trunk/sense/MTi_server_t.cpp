#include "MTi_server_t.h"
#include "alcor/core/config_parser_t.hpp"
//-------------------------------------------------------------------
namespace all { namespace sense {
//-------------------------------------------------------------------
  MTi_server_t::MTi_server_t(const std::string& inifile)

  {
    //
    mti_.reset();
    //
    all::core::config_parser_t config;
    //
    config.load(core::tags::ini, inifile);
    //
    int comport = config.get<int>("config.comport",12144);

    //assegna porta
    set_port(comport);

    //ora definisci le funzioni che rispondono ad un determinato servizio
    //sono solo esempi!
    add_command_handler
      ("rpy_service", boost::bind(&MTi_server_t::rpy_service_, this,_1, _2));
  }
//-------------------------------------------------------------------
    void MTi_server_t::set_device_ptr(all::sense::MTi_driver_ptr device_)
    {
      mti_= device_;
    }
//-------------------------------------------------------------------
  void MTi_server_t::rpy_service_(core::client_connection_ptr_t client_, core::net_packet_ptr_t packet_)
  {
  packet_.reset(new core::net_packet_t);

  if(mti_) 
  {
    all::math::rpy_angle_t current  = mti_->get_euler();
    packet_->double_to_buf(current.roll.deg());
    packet_->double_to_buf(current.pitch.deg());
    packet_->double_to_buf(current.yaw.deg());
  }

  send_answer_packet("rpy_service", client_, packet_);
  }
  //-------------------------------------------------------------------
}}//all::sense
//-------------------------------------------------------------------