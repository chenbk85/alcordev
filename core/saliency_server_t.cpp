#include "saliency_server_t.h"
#include "alcor/core/iniWrapper.h"
#include "alcor/core/core.h"
//-------------------------------------------------------------------
#include "boost/filesystem/path.hpp"
namespace fs = boost::filesystem;
//-------------------------------------------------------------------
namespace all { namespace core {
//-------------------------------------------------------------------
  saliency_server_t::saliency_server_t(const char*  inifile)
  {
  iniWrapper ini;

  //fs::path  filepath(core::relative_bin_config_path);

	if (ini.Load(inifile)) 
  {
		int port = ini.GetInt("server:port", 30307);
		set_port(port);
	}
  register_();
  }
//-------------------------------------------------------------------
  void saliency_server_t::register_()
  {
    add_command_handler("update_saliency",boost::bind(&saliency_server_t::update_saliency_data_
      ,this,_1,_2));
  }
  //-------------------------------------------------------------------
      ///
  void saliency_server_t::set_user_callback(user_callback_t func)
  {
    user_callback_ = func;
  }
//-------------------------------------------------------------------
//
  void saliency_server_t::update_saliency_data_(client_connection_ptr_t
                                              , net_packet_ptr_t packet)
  {
    saliency_data.import(packet);
    user_callback_();
  }
//-------------------------------------------------------------------
}}//all::core
//-------------------------------------------------------------------