#include "directed_perception_ptu_server_t.h"
#include "alcor/core/iniWrapper.h"

namespace all {
	namespace act {

directed_perception_ptu_server_t::directed_perception_ptu_server_t(const std::string& ini_file)
{
  iniWrapper ini;
  if ( ini.Load(ini_file.c_str()) ) 
  {
		int port = ini.GetInt("server:port", 99999);
		set_port(port);
	}

  add_command_handler
    ("resetptu", boost::bind(&directed_perception_ptu_server_t::reset, this, _1, _2));
  add_command_handler
    ("pantilt",  boost::bind(&directed_perception_ptu_server_t::pantilt, this, _1, _2));
  add_command_handler
    ("getserverdata",  boost::bind(&directed_perception_ptu_server_t::send_pantilt_data, this, _1, _2));

}

void directed_perception_ptu_server_t::assign_ptu(act::directed_perception_sptr ptu)
{
  ptu_sp = ptu;
}

void directed_perception_ptu_server_t::reset(client_connection_ptr_t, net_packet_ptr_t)
{
  printf("Reset\n");
  if(ptu_sp.get())
    ptu_sp->reset();
}

void directed_perception_ptu_server_t::pantilt(client_connection_ptr_t, net_packet_ptr_t packet)
{
  //printf("Set\n");
  if(ptu_sp.get())
  {
      act::ptu_server_data_t clientdata;
      clientdata.import(packet);
      ptu_sp->set_pantilt(clientdata.pan,clientdata.tilt);
  } 
}

void directed_perception_ptu_server_t::send_pantilt_data(client_connection_ptr_t cl, net_packet_ptr_t packet)
{
  //printf("Update\n");
  if(ptu_sp.get())
  {
    //printf("get_pantilt\n");
    ptu_sp->get_current_pantilt((float&)ptu_data.pan, (float&)ptu_data.tilt);
    ptu_data.pan = ptu_sp->get_pan();
    ptu_data.tilt = ptu_sp->get_tilt();
    //printf("get_pantilt\n");
    packet.reset(new core::net_packet_t);
    //printf(" ptu_data.pack(ans);\n");
    ptu_data.pack(packet);
    //printf(" send_answer_packet\n");
    send_answer_packet("updatedata", cl, packet);
  }

}



  }}//all::act