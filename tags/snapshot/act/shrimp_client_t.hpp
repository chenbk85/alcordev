#ifndef shrimp_client_t_H_INCLUDED
#define shrimp_client_t_H_INCLUDED

#include <alcor/core/client_base_t.hpp>
#include <alcor/core/iniWrapper.h>
#include <alcor/math/angle.h>

#include "shrimp_server_data_t.hpp"

namespace all {
	namespace act {

class shrimp_client_t : public client_base_t {

public:
	shrimp_client_t(char* ini_file = "config/shrimp_client.ini");

	void set_robot_on();
	void set_robot_standby();

	void em_stop();
	void em_release();

	void set_speed(int);
	void set_steer(all::math::angle);

	
	int get_speed();
	all::math::angle get_steer();

	bool is_on();
	bool is_em();

	double get_voltage();


private:
	void update_shrimp_data(net_packet_ptr_t);

	void connected_cb();

private:
	iniWrapper m_ini_config;
	shrimp_server_data_t m_shrimp_data;
};


}} //namespaces

#endif
