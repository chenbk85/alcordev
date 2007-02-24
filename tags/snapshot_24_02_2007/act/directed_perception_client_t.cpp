#include "directed_perception_client_t.h"
#include "alcor/core/iniWrapper.h"

namespace all {
	namespace act {

    directed_perception_client_t::directed_perception_client_t(const std::string& inifile)
    {
      iniWrapper ini;
      if ( ini.Load(inifile.c_str()) ) 
      {
    		all::core::ip_address_t server_address;
		    server_address.hostname = ini.GetStringAsChar("server:hostname", "127.0.0.1");
		    server_address.port = ini.GetInt("server:port", 99999);
		    set_server_addr(server_address);
	    }
      else
      {
      }

  	  add_command_handler
        ("updatedata", boost::bind(&directed_perception_client_t::update_ptu_data, this, _1));

	    set_connect_callback(boost::bind(&directed_perception_client_t::connected_cb, this));

    }

    ///
    void directed_perception_client_t::set_pantilt(float pan, float tilt)
    {
    	core::net_packet_ptr_t packet(new core::net_packet_t());
      act::ptu_server_data_t data;
      data.pan = (double)pan;data.tilt = (double)tilt;
      data.pack(packet);
	    send_command("pantilt", packet);
    }

      ///
    void directed_perception_client_t::reset()
    {
    	send_command("resetptu");
    }


    void directed_perception_client_t::update_ptu_data(core::net_packet_ptr_t packet) 
    {
	  ptu_data.import(packet);
    }

    void directed_perception_client_t::connected_cb() 
    {
	    send_request("getserverdata", 100);
    }

      ///
    void directed_perception_client_t::get_pantilt(float& pan, float& tilt)
    {
      pan = ptu_data.pan;
      tilt = ptu_data.tilt;
    }

  }}