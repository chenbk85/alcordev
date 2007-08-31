#include "urg_server_data_t.hpp"

namespace all {
	namespace sense {

void urg_server_data_t::import(all::core::net_packet_ptr_t packet) {
	
	int scan_size = packet->buf_to_int();

	_scan_data.reset(new urg_scan_data_t());

	_scan_data->start_step = packet->buf_to_int();
	_scan_data->end_step = packet->buf_to_int();
	_scan_data->cc = packet->buf_to_int();
	_scan_data->time_stamp = packet->buf_to_int();

	for (int j=0; j < scan_size; j++) {
		_scan_data->scan_points.push_back(packet->buf_to_int());
	}

}

void urg_server_data_t::pack(all::core::net_packet_ptr_t packet) {
	
	packet->int_to_buf(static_cast<int>(_scan_data->scan_points.size()));
	
	packet->int_to_buf(_scan_data->start_step);
	packet->int_to_buf(_scan_data->end_step);
	packet->int_to_buf(_scan_data->cc);
	packet->int_to_buf(_scan_data->time_stamp);

	for (int j=0; j < _scan_data->scan_points.size(); j++) {
		packet->int_to_buf(_scan_data->scan_points[j]);
	}
}

}}