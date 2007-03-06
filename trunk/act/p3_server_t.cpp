#include "p3_server_t.h"
#include "alcor/core/iniWrapper.h"
//-------------------------------------------------------------------
namespace all { namespace act{
//-------------------------------------------------------------------
p3_server_t::p3_server_t(char* ini_file)
{
  iniWrapper ini;
	if (ini.Load(ini_file)) 
  {
		int port = ini.GetInt("server:port", 76543);
		set_port(port);
	}
}
//-------------------------------------------------------------------
void p3_server_t::set_gateway_ptr(p3_gateway_ptr_t p3ptr)
{
p3_ = p3ptr;
}
//-------------------------------------------------------------------
void p3_server_t::start_service()
{
	add_command_handler("getData", boost::bind(&p3_server_t::send_p3_data, this, _1, _2));

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
//-------------------------------------------------------------------
}}//all::act
//-------------------------------------------------------------------