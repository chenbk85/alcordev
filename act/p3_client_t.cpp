#include "p3_client_t.h"
#include "alcor/core/iniWrapper.h"
//-------------------------------------------------------------------
namespace all { namespace act{
//-------------------------------------------------------------------
  p3_client_t::p3_client_t(const char* inifile)
  {
    iniWrapper ini;
    ini.Load(inifile);

		all::core::ip_address_t server_address;

		server_address.hostname = ini.GetStringAsChar("server:address", "localhost");
		server_address.port = ini.GetInt("server:port", 76543);
		set_server_addr(server_address);

	  add_command_handler("serverData", boost::bind(&p3_client_t::update_p3_data, this, _1));
	  set_connect_callback(boost::bind(&p3_client_t::connected_callback, this));
  }
//-------------------------------------------------------------------
const math::pose2d& p3_client_t::get_odometry() const
{
  return p3_data_.pose;
}
//-------------------------------------------------------------------
  ///
  void p3_client_t::update_p3_data(net_packet_ptr_t answer_packet)
  {
    p3_data_.import(answer_packet);
    if(user_callback)
      user_callback(p3_data_.pose);
  }
//-------------------------------------------------------------------
void p3_client_t::set_user_callback(user_callback_t callback)
{
  //??
  user_callback = callback;
}
//-------------------------------------------------------------------
  ///
  void p3_client_t::connected_callback()
  {

  }
//-------------------------------------------------------------------
}}//all::act
//-------------------------------------------------------------------