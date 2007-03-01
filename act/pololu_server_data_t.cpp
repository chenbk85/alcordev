#include "pololu_server_data_t.hpp"

namespace all {
	namespace act {

void pololu_server_data_t::init(int num_servo) {
	n_servo = num_servo;
	servo = new servo_data_t[num_servo];
}

void pololu_server_data_t::pack(core::net_packet_ptr_t packet) {
	for (int i = 0; i< n_servo; i++) {
		packet->double_to_buf(servo[i].pose);
		packet->int_to_buf(servo[i].speed);
	}
}

void pololu_server_data_t::import(core::net_packet_ptr_t packet) {
	for (int i = 0; i < n_servo; i++) {
		servo[i].pose = packet->buf_to_double();
		servo[i].speed = packet->buf_to_int();
	}
}

}}

