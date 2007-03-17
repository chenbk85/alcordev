#include "saliency_server_t.h"
#include "alcor/core/iniWrapper.h"
#include "alcor/core/core.h"

#include "boost/filesystem/path.hpp"
namespace fs = boost::filesystem;
//-------------------------------------------------------------------
namespace all { namespace core {
//-------------------------------------------------------------------
  saliency_server_t::saliency_server_t(const char*  inifile)
  {
  iniWrapper ini;

  core::relative_bin_config_path;

	if (ini.Load(inifile)) 
  {
		int port = ini.GetInt("server:port", 300307);
		set_port(port);
	}
  register_();
  }
//-------------------------------------------------------------------
  void saliency_server_t::register_()
  {
  }
//-------------------------------------------------------------------
}}//all::core
//-------------------------------------------------------------------