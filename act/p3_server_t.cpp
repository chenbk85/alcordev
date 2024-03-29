#include "p3_server_t.h"
#include "alcor/core/iniWrapper.h"
//-------------------------------------------------------------------
namespace all { namespace act{
//-------------------------------------------------------------------
p3_server_t::p3_server_t(const char* ini_file)
{
  iniWrapper ini;
	if (ini.Load(ini_file)) 
  {
		int port = ini.GetInt("server:port", 76543);
		set_port(port);
	}
  register_();
}
//-------------------------------------------------------------------
void p3_server_t::set_gateway_ptr(p3_gateway_ptr_t p3ptr)
{
p3_ = p3ptr;
}
//-------------------------------------------------------------------
void p3_server_t::register_()
{
  add_command_handler("getData",  boost::bind(&p3_server_t::send_p3_data, this, _1, _2));
  add_command_handler("setSlamPose",  boost::bind(&p3_server_t::set_slam_localized, this, _1, _2));

  add_command_handler("setVel",       boost::bind(&p3_server_t::set_vel, this, _1, _2));
  add_command_handler("setRotVel",    boost::bind(&p3_server_t::set_rot_vel, this, _1, _2));
  add_command_handler("setDHeading",   boost::bind(&p3_server_t::set_dheading, this, _1, _2));


  add_command_handler("enableStop",   boost::bind(&p3_server_t::enable_stop, this, _1, _2));
  add_command_handler("enableWander",   boost::bind(&p3_server_t::enable_wander, this, _1, _2));

  add_command_handler("enableFollow",   boost::bind(&p3_server_t::enable_follow, this, _1, _2));
  add_command_handler("setTarget",   boost::bind(&p3_server_t::set_target, this, _1, _2));

  add_command_handler("enableGoto",   boost::bind(&p3_server_t::enable_goto, this, _1, _2));
  add_command_handler("setGoto",   boost::bind(&p3_server_t::set_goto, this, _1, _2));

  add_command_handler("trigger", boost::bind(&p3_server_t::trigger, this, _1,_2));
}
//-------------------------------------------------------------------
///
void p3_server_t::set_slam_localized(client_connection_ptr_t, net_packet_ptr_t pkt)
{
  p3_slam_data_.import(pkt);
}
//-------------------------------------------------------------------
///
void p3_server_t::set_vel(client_connection_ptr_t, net_packet_ptr_t pkt)
{
  double vel = pkt->buf_to_double();
  p3_->set_vel(vel);
  //printf("setVel: %f", vel);
}
//-------------------------------------------------------------------
///
void p3_server_t::set_rot_vel(client_connection_ptr_t, net_packet_ptr_t pkt)
{
  double vel = pkt->buf_to_double();
  p3_->set_rot_vel(vel);
  //printf("setRotVel: %f", vel);
}
//-------------------------------------------------------------------
///
void p3_server_t::set_dheading(client_connection_ptr_t, net_packet_ptr_t pkt)
{
  double dheading = pkt->buf_to_double();
  p3_->set_delta_heading(math::angle(dheading, math::deg_tag) );
}
//-------------------------------------------------------------------
///
void p3_server_t::enable_wander(client_connection_ptr_t, net_packet_ptr_t)
{
  p3_->enable_wander_mode();
}
//-------------------------------------------------------------------
///
void p3_server_t::enable_stop(client_connection_ptr_t, net_packet_ptr_t)
{
  p3_->enable_stop_mode();
}
//-------------------------------------------------------------------
///
void p3_server_t::enable_follow(client_connection_ptr_t, net_packet_ptr_t)
{
  p3_->enable_follow_mode();
}
//-------------------------------------------------------------------
///
  ///
void p3_server_t::set_target(client_connection_ptr_t, net_packet_ptr_t pkt)
{
  double mod        = pkt->buf_to_double();
  double orient     = pkt->buf_to_double();
  double mmpersecs  = pkt->buf_to_double();

  math::point2d target(mod, math::angle(orient,math::deg_tag) );

  p3_->set_target_to_follow(target, mmpersecs);
}
//-------------------------------------------------------------------
void p3_server_t::send_p3_data(client_connection_ptr_t cl, net_packet_ptr_t pkt)
{
  pkt.reset(new core::net_packet_t());
  //update data ...
  p3_data_.pose = p3_->get_odometry();
  //serialize
  p3_data_.pack(pkt);
  //send data back
	send_answer_packet("serverData", cl, pkt);
}
//-------------------------------------------------------------------
///
void p3_server_t::enable_goto(client_connection_ptr_t, net_packet_ptr_t)
{
  p3_->enable_goto_mode();
}
//-------------------------------------------------------------------
//
void p3_server_t::set_goto(client_connection_ptr_t, net_packet_ptr_t pkt)
{
  double mod        = pkt->buf_to_double();
  double orient     = pkt->buf_to_double();
  double mmpersecs  = pkt->buf_to_double();
  //printf("p3_server_t::set_goto: mod: %f orient: %f mmpersec: %f\n", mod, orient, mmpersecs);
  math::point2d target(mod, math::angle(orient,math::deg_tag) );

  p3_->set_relative_goto(target, mmpersecs);
}
//-------------------------------------------------------------------
void p3_server_t::set_on_trigger_cb(trigger_callback_t user_callback)
{
  on_trigger_ = user_callback;
}
//-------------------------------------------------------------------
void p3_server_t::trigger(client_connection_ptr_t, net_packet_ptr_t pkt)
{
  if(on_trigger_)
    on_trigger_();
}
//-------------------------------------------------------------------
}}//all::act
//-------------------------------------------------------------------