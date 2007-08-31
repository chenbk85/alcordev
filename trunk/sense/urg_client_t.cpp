#include "urg_client_t.hpp"

namespace all {
	namespace sense {

urg_client_t::urg_client_t(char* ini_file) {
	if (_ini_config.Load(ini_file)) {
		all::core::ip_address_t server_address;
		server_address.hostname = _ini_config.GetStringAsChar("server:address", "127.0.0.1");
		server_address.port = _ini_config.GetInt("server:port", 12345);
		set_server_addr(server_address);
	}

	add_command_handler("scanData", boost::bind(&urg_client_t::save_scan_data, this, _1));
	add_command_handler("multiScanData", boost::bind(&urg_client_t::save_multi_scan_data, this, _1));

}

void urg_client_t::set_laser_on() {
	send_command("setLaserOn");
}

void urg_client_t::set_laser_off() {
	send_command("setLaserOff");
}

void urg_client_t::set_d_mode() {
	send_command("setDMode");
}

void urg_client_t::set_s_mode() {
	send_command("setSMode");
}

void urg_client_t::do_scan(int start_step, int end_step, int cc) {
	net_packet_ptr_t packet(new net_packet_t());
	packet->int_to_buf(start_step);
	packet->int_to_buf(end_step);
	packet->int_to_buf(cc);

	send_command("doScan", packet);
}

void urg_client_t::do_multiple_scan(int start_step, int end_step, int cc, int n_scan, int time_interval) {
	net_packet_ptr_t packet(new net_packet_t());
	packet->int_to_buf(start_step);
	packet->int_to_buf(end_step);
	packet->int_to_buf(cc);
	packet->int_to_buf(n_scan);
	packet->int_to_buf(time_interval);

	_multi_scan_left = n_scan;
	_multi_scan.scan_vec.reset(new urg_scan_data_ptr[n_scan]);
	_multi_scan.n_scan = n_scan;

	send_command("doMultipleScan", packet);
	
}

void urg_client_t::start_continous_scan(int start, int end, int cc, int time_interval) {
	net_packet_ptr_t packet(new net_packet_t());
	packet->int_to_buf(start);
	packet->int_to_buf(end);
	packet->int_to_buf(cc);
	packet->int_to_buf(time_interval);

	send_command("startContinousScan", packet);
	
}

void urg_client_t::stop_continous_scan() {
	send_command("stopContinousScan");
}

void urg_client_t::save_scan_data(all::core::net_packet_ptr_t packet) {
	_urg_data.import(packet);
	if (_scan_ready_cb)
		_ready_cb_thread.reset(new boost::thread(boost::bind(urg_client_t::_scan_ready_cb, _urg_data._scan_data)));
		//_scan_ready_cb(_urg_data._scan_data);
}

void urg_client_t::save_multi_scan_data(all::core::net_packet_ptr_t packet) {
	_urg_data.import(packet);
	int curr_index = _multi_scan.n_scan - _multi_scan_left;
	_multi_scan.scan_vec[curr_index] = _urg_data._scan_data;
	_multi_scan_left--;
	if (_multi_scan_left == 0 && _multi_scan_ready_cb) {
		_ready_cb_thread.reset(new boost::thread(boost::bind(urg_client_t::_multi_scan_ready_cb, _multi_scan)));
		//_multi_scan_ready_cb(_multi_scan);
	}
}

all::sense::urg_scan_data_ptr urg_client_t::get_last_scan() {
	return _urg_data._scan_data;
}

all::sense::urg_multi_scan_t urg_client_t::get_last_multi_scan() {
	return _multi_scan;
}

void urg_client_t::set_scan_ready_cb(boost::function <void (urg_scan_data_ptr)> scan_ready_cb) {
	_scan_ready_cb = scan_ready_cb;
}

void urg_client_t::set_multi_scan_ready_cb(boost::function <void (urg_multi_scan_t)> multi_scan_ready_cb) {
	_multi_scan_ready_cb = multi_scan_ready_cb;
}

}}