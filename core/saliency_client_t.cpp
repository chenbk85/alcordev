#include "saliency_client_t.h"
#include "alcor/core/iniWrapper.h"
//-------------------------------------------------------------------
namespace all { namespace core {
//-------------------------------------------------------------------
  saliency_client_t::saliency_client_t(const char* inifile)
  {
    iniWrapper ini;
    ini.Load(inifile);

		all::core::ip_address_t server_address;
		server_address.hostname = ini.GetStringAsChar("server:address", "localhost");
		server_address.port = ini.GetInt("server:port", 30307);
		set_server_addr(server_address);
	  set_connect_callback(boost::bind(&saliency_client_t::connected_callback, this));
  }
//-------------------------------------------------------------------
///
  void saliency_client_t::send_saliency_data()
  {
    //core::net_packet_ptr_t packet( new core::net_packet_t);
    core::net_packet_ptr_t packet = core::net_packet_t::create();
    saliency_data_.pack(packet);
    send_command("update_saliency",packet);
  }
//-------------------------------------------------------------------
///
  void saliency_client_t::reset_data()
  {
    saliency_data_.saliency_data_.observations_.clear();
  }
//-------------------------------------------------------------------
///
  void saliency_client_t::set_scancount(size_t scancount)
  {
    saliency_data_.saliency_data_.scan_count_ = scancount;
  }
//-------------------------------------------------------------------
    ///
  void saliency_client_t::set_bearing(const all::core::pantilt_angle_t& bearing)
  {
    saliency_data_.saliency_data_.bearing_ = bearing;
  }
//-------------------------------------------------------------------
///
  void saliency_client_t::add_observation(core::observation_data_t& odata)
  {
    saliency_data_.saliency_data_.observations_.push_back(odata);
  }
//-------------------------------------------------------------------

//-------------------------------------------------------------------
}}//all::core
//-------------------------------------------------------------------