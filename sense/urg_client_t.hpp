#ifndef urg_client_t_H_INCLUDED
#define urg_client_t_H_INCLUDED

#include <alcor/core/client_base_t.hpp>
#include <alcor/core/iniWrapper.h>

//#include "urg_server_data_t.hpp"
#include "alcor/sense/urg_server_data_t.hpp"

namespace all {
	namespace sense {

class urg_client_t:public client_base_t {

public:

	urg_client_t(char* ini_file = "config/urg_client.ini");

	void set_laser_on();
	void set_laser_off();

	void set_d_mode();
	void set_s_mode();

	void do_scan(int, int, int);
	void do_multiple_scan(int, int, int, int, int);

	void start_continous_scan(int, int, int, int);
	void stop_continous_scan();

	urg_scan_data_ptr get_last_scan();
	urg_multi_scan_t get_last_multi_scan();

	void set_scan_ready_cb(boost::function <void (urg_scan_data_ptr)>);
	void set_multi_scan_ready_cb(boost::function <void (urg_multi_scan_t)>);

private:

	void save_scan_data(net_packet_ptr_t);
	void save_multi_scan_data(net_packet_ptr_t);

	boost::shared_ptr<boost::thread> _ready_cb_thread;
	boost::function <void (urg_scan_data_ptr)> _scan_ready_cb;
	boost::function <void (urg_multi_scan_t)> _multi_scan_ready_cb;

private:

	iniWrapper _ini_config;
	urg_server_data_t _urg_data;

	int _multi_scan_left;
	urg_multi_scan_t _multi_scan;
};

}}
#endif