#ifndef shrimp_server_t_H_INCLUDED
#define shrimp_server_t_H_INCLUDED

#include <alcor/core/server_base_t.hpp>
#include <alcor/core/iniWrapper.h>
#include <alcor/math/angle.h>

#include "shrimp_gateway_t.hpp"
#include "shrimp_server_data_t.hpp"



namespace all {
	namespace act {

class shrimp_server_t : public server_base_t {

public:
	shrimp_server_t(char* ini_file = "config/shrimp_server.ini");

	void stop();

private:

	void set_robot_on(client_connection_ptr_t, net_packet_ptr_t);
	void set_robot_standby(client_connection_ptr_t, net_packet_ptr_t);

	void em_stop(client_connection_ptr_t, net_packet_ptr_t);
	void em_release(client_connection_ptr_t, net_packet_ptr_t);

	void set_speed(client_connection_ptr_t, net_packet_ptr_t);
	void set_steer(client_connection_ptr_t, net_packet_ptr_t);

	void update_shrimp_data();
	void send_shrimp_data(client_connection_ptr_t, net_packet_ptr_t);

	void client_disconnected_cb(int);


private:
	shrimp_gateway_t m_shrimp;
	iniWrapper m_ini_config;

	shrimp_server_data_t m_shrimp_data;
};

}} //namespaces
#endif

