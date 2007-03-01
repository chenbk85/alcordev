#ifndef pololu_ctrl_server_t_H_INCLUDED
#define pololu_ctrl_server_t_H_INCLUDED

#include <alcor/core/server_base_t.hpp>
#include "pololu_server_data_t.hpp"

#include "PololuServoController.hpp"

#include <alcor/core/iniWrapper.h>

namespace all {
	namespace act {


class pololu_ctrl_server_t : public all::core::server_base_t {

public:
	pololu_ctrl_server_t(char* ini_file = "config/pololu_server.ini");


private:
	
	void update_pololu_data();

private:

	void set_pose(all::core::client_connection_ptr_t, all::core::net_packet_ptr_t);
	
	void set_speed(all::core::client_connection_ptr_t, all::core::net_packet_ptr_t);

	void get_pololu_data(all::core::client_connection_ptr_t, all::core::net_packet_ptr_t);

	void send_pololu_data(all::core::client_connection_ptr_t, all::core::net_packet_ptr_t);
	void send_num_servo(all::core::client_connection_ptr_t, all::core::net_packet_ptr_t);


private:

	PololuServoController m_pololu;

	pololu_server_data_t m_pololu_data;

	iniWrapper m_ini_config;
};

}} //namespaces

#endif



