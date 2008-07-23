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

		server_address.hostname = ini.GetStringAsChar("server:hostname", "localhost");
		server_address.port = ini.GetInt("server:port", 700504);

    printf("Server: %s:%d\n", server_address.hostname.c_str(), server_address.port);
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
void p3_client_t::set_slam_localized(const math::pose2d& slam_pose)
{
  p3_server_data_t temp;
  temp.pose = slam_pose;
  core::net_packet_ptr_t packet(new core::net_packet_t());
  temp.pack(packet);
  send_command("setSlamPose",packet);
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
///
void p3_client_t::set_rot_vel(double mmpersecs)
{
  core::net_packet_ptr_t packet(new core::net_packet_t());
  packet->double_to_buf(mmpersecs);
  send_command("setRotVel", packet);
}
//-------------------------------------------------------------------
//
void p3_client_t::set_dheading(double offset)
{
  core::net_packet_ptr_t packet(new core::net_packet_t());
  packet->double_to_buf(offset);
  send_command("setDHeading", packet);
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
void p3_client_t::enable_goto_mode()
{
    send_command("enableGoto");
}
//-------------------------------------------------------------------
///only when in goto_mode.
///distance (mt) e offset angolare dal target (gradi)
void p3_client_t::set_relative_goto
  (const math::point2d& target, double mmpersecs)
{
  core::net_packet_ptr_t packet(new core::net_packet_t());

  packet->double_to_buf(target.magnitude());
  packet->double_to_buf(target.orientation().deg());
  packet->double_to_buf(mmpersecs);
    
  send_command("setGoto", packet);
}
//-------------------------------------------------------------------
void p3_client_t::trigger()
{
send_command("trigger");
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