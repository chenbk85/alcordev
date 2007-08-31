#ifndef urg_server_t_H_INCLUDED
#define urg_server_t_H_INCLUDED

#include <alcor/core/server_base_t.hpp>
#include <alcor/core/iniWrapper.h>

#include "urg_laser_t.hpp"
#include "urg_server_data_t.hpp"

namespace all {
	namespace sense {

class urg_server_t:public server_base_t {

public:
	urg_server_t(char* ini_file="config/urg_server.ini");

	void set_laser_on(client_connection_ptr_t, net_packet_ptr_t);
	void set_laser_off(client_connection_ptr_t, net_packet_ptr_t);

	void set_d_mode(client_connection_ptr_t, net_packet_ptr_t);
	void set_s_mode(client_connection_ptr_t, net_packet_ptr_t);

	void do_scan(client_connection_ptr_t, net_packet_ptr_t);
	void do_multiple_scan(client_connection_ptr_t, net_packet_ptr_t);

	void start_continous_scan(client_connection_ptr_t, net_packet_ptr_t);
	void stop_continous_scan(client_connection_ptr_t, net_packet_ptr_t);

private:
	
	void send_scan_data(all::sense::urg_scan_data_ptr, client_connection_ptr_t);
	void send_multiple_scan_data(all::sense::urg_scan_data_ptr, client_connection_ptr_t);

private:

	urg_laser_t _urg;
	iniWrapper _ini_config;

	urg_server_data_t _urg_data;

};

}}
#endif
