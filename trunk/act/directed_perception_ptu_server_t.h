#ifndef directed_perception_ptu_server_t_H_INCLUDED
#define directed_perception_ptu_server_t_H_INCLUDED

#include "alcor/act/directed_perception_ptu_t.h"
#include "alcor/core/server_base_t.hpp"
#include "ptu_server_data_t.hpp"
#include <boost\shared_ptr.hpp>

namespace all {
	namespace act {

class directed_perception_ptu_server_t : public core::server_base_t
{
public:
  ///
  directed_perception_ptu_server_t(std::string& ini_file);
  ///
  void assign_ptu(boost::shared_ptr<act::directed_perception_ptu_t>);

public:
  ///
  void pantilt(client_connection_ptr_t, net_packet_ptr_t);
  ///
	void reset(client_connection_ptr_t, net_packet_ptr_t);
  ///
  void send_pantilt_data(client_connection_ptr_t, net_packet_ptr_t);


private:
  ///
  boost::shared_ptr<act::directed_perception_ptu_t> ptu_sp;
  ///
  act::ptu_server_data_t ptu_data;

};

  }}//all::act

#endif //directed_perception_ptu_server_t_H_INCLUDED