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
void p3_client_t::set_vel(double mmpersecs)
{
  core::net_packet_ptr_t packet(new core::net_packet_t());
  packet->double_to_buf(mmpersecs);
  send_command("setVel", packet);
}
//-------------------------------------------------------------------
void p3_client_t::enable_stop_mode()
{
  send_command("enableStop");
}
//-------------------------------------------------------------------
void p3_client_t::enable_wander_mode()
{
    send_command("enableWander");
}
//-------------------------------------------------------------------
void p3_client_t::enable_follow_mode()
{
    send_command("enableFollow");
}
//-------------------------------------------------------------------
///only when in follow_mode.
///distance (mt) e offset angolare dal target (gradi)
void p3_client_t::set_target_to_follow 
  (const math::point2d& target, double mmpersecs)
{
  core::net_packet_ptr_t packet(new core::net_packet_t());

  packet->double_to_buf(target.magnitude());
  packet->double_to_buf(target.orientation().deg());
  packet->double_to_buf(mmpersecs);
    
  send_command("setTarget", packet);
}
//-------------------------------------------------------------------
///
void p3_client_t::connected_callback()
{
send_request("getData", 50);
}
//-------------------------------------------------------------------
}}//all::act
//-------------------------------------------------------------------