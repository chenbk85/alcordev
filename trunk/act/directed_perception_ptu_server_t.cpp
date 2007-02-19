#include "directed_perception_ptu_server_t.h"
#include "alcor/core/iniWrapper.h"

namespace all {
	namespace act {

directed_perception_ptu_server_t::directed_perception_ptu_server_t(std::string& ini_file)
{
  iniWrapper ini;
  if ( ini.Load(ini_file.c_str()) ) 
  {
		int port = ini.GetInt("server:port", 99999);
		set_port(port);
	}

  add_command_handler("resetptu", boost::bind(&directed_perception_ptu_server_t::reset, this, _1, _2));
	
}

void directed_perception_ptu_server_t::assign_ptu(boost::shared_ptr<act::directed_perception_ptu_t> ptu)
{
  ptu_sptr = ptu;
}

void directed_perception_ptu_server_t::reset(client_connection_ptr_t, net_packet_ptr_t)
{
  if(ptu_sptr.get())
    ptu_sptr->reset();
}


  }}//all::act