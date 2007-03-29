#ifndef pololu_ctrl_client_t_H_INCLUDED
#define pololu_ctrl_client_t_H_INCLUDED

#include <alcor/core/client_base_t.hpp>
#include <alcor/core/iniWrapper.h>
#include "pololu_server_data_t.hpp"

namespace all {
	namespace act {

class pololu_ctrl_client_t : public all::core::client_base_t {

public:
	pololu_ctrl_client_t(char* ini_file = "config/pololu_client.ini");
	
	void set_speed(int, int);
	void set_speed_deg(int, double);

	void set_pose(int, double);

	double get_pose(int);
	int get_speed(int);
	

private:

	void get_server_data(core::net_packet_ptr_t);
	void get_num_servo(core::net_packet_ptr_t);

	void connected_cb();

private:

	pololu_server_data_t m_pololu_data;

	iniWrapper m_ini_config;

};

}}

#endif



	