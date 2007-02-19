#ifndef directed_perception_client_t_H_INCLUDED
#define directed_perception_client_t_H_INCLUDED

#include "alcor/core/client_base_t.hpp"
#include "ptu_server_data_t.hpp"
//#include "alcor/math/"

namespace all {
	namespace act {

class directed_perception_client_t : public core::client_base_t

{
public:
  ///
  directed_perception_client_t(const std::string& ini);
  ///
  void set_pantilt(float pan, float tilt);
  ///
  void reset();

  ///
  void get_pantilt(float& pan, float& tilt);

private:
  ///
	void connected_cb();
  ///
  void update_ptu_data(core::net_packet_ptr_t);
  ///
  act::ptu_server_data_t ptu_data;
};


  }}
#endif