#include "urg_server_t.hpp"

namespace all {
	namespace sense {


urg_server_t::urg_server_t(char* ini_file) : _urg(ini_file) {
	if (_ini_config.Load(ini_file)) {
		int port = _ini_config.GetInt("server:port", 12345);
		set_port(port);
	}
	
	add_command_handler("setLaserOn", boost::bind(&urg_server_t::set_laser_on, this, _1, _2));
	add_command_handler("setLaserOff", boost::bind(&urg_server_t::set_laser_off, this, _1, _2));
	
	add_command_handler("setDMode", boost::bind(&urg_server_t::set_d_mode, this, _1, _2));
	add_command_handler("setSMode", boost::bind(&urg_server_t::set_s_mode, this, _1, _2));

	add_command_handler("doScan", boost::bind(&urg_server_t::do_scan, this, _1, _2));
	add_command_handler("doMultipleScan", boost::bind(&urg_server_t::do_multiple_scan, this, _1, _2));

	add_command_handler("startContinousScan", boost::bind(&urg_server_t::start_continous_scan, this, _1, _2));
	add_command_handler("stopContinousScan", boost::bind(&urg_server_t::stop_continous_scan, this, _1, _2));



	if (_urg.connect()) {
		//m_shrimp.set_robot_on();
	}
	else
		printf("Error connecting to laser\n");
}

void urg_server_t::set_laser_on(client_connection_ptr_t client, net_packet_ptr_t packet) {
	_urg.laser_on();
}

void urg_server_t::set_laser_off(client_connection_ptr_t client, net_packet_ptr_t packet) {
	_urg.laser_off();
}

void urg_server_t::set_d_mode(client_connection_ptr_t client, net_packet_ptr_t packet) {
	_urg.set_d_mode();
}

void urg_server_t::set_s_mode(client_connection_ptr_t client, net_packet_ptr_t packet) {
	_urg.set_s_mode();
}

void urg_server_t::do_scan(client_connection_ptr_t client, net_packet_ptr_t packet) {
	int start = packet->buf_to_int();
	int end = packet->buf_to_int();
	int cc = packet->buf_to_int();

	_urg_data._scan_data = _urg.do_scan(start, end, cc);

	packet.reset(new net_packet_t());

	_urg_data.pack(packet);

	send_answer_packet("scanData", client, packet);
}

void urg_server_t::do_multiple_scan(client_connection_ptr_t client, net_packet_ptr_t packet) {
	
	int start = packet->buf_to_int();
	int end = packet->buf_to_int();
	int cc = packet->buf_to_int();
	int n_scan = packet->buf_to_int();
	int time_interval = packet->buf_to_int();

	_urg.set_line_ready_cb(boost::bind(&urg_server_t::send_multiple_scan_data, this, _1, client));
	
	_urg.do_multiple_scan(start, end, cc, n_scan, time_interval);
}

void urg_server_t::start_continous_scan(client_connection_ptr_t client, net_packet_ptr_t packet) {
	
	int start = packet->buf_to_int();
	int end = packet->buf_to_int();
	int cc = packet->buf_to_int();
	int time_interval = packet->buf_to_int();

	_urg.set_line_ready_cb(boost::bind(&urg_server_t::send_scan_data, this, _1, client));

	_urg.start_continous_scan(start, end, cc, time_interval);
}

void urg_server_t::stop_continous_scan(client_connection_ptr_t client, net_packet_ptr_t packet) {
	_urg.stop_continous_scan();
}

void urg_server_t::send_scan_data(all::sense::urg_scan_data_ptr scan_data, client_connection_ptr_t client) {

	net_packet_ptr_t packet;
	packet.reset(new net_packet_t());

	_urg_data._scan_data = scan_data;
	_urg_data.pack(packet);

	send_answer_packet("scanData", client, packet);
	
}

void urg_server_t::send_multiple_scan_data(all::sense::urg_scan_data_ptr scan_data, client_connection_ptr_t client) {
	net_packet_ptr_t packet;
	packet.reset(new net_packet_t());

	_urg_data._scan_data = scan_data;
	_urg_data.pack(packet);

	send_answer_packet("multiScanData", client, packet);
}

}}