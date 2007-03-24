#include "shrimp_server_data_t.hpp"

namespace all {
	namespace act {

void shrimp_server_data_t::import(core::net_packet_ptr_t packet) {
	m_speed = packet->buf_to_int();
	m_steer = packet->buf_to_int();
	m_power_status = packet->buf_to_int();
	m_em_status = packet->buf_to_int();
	m_voltage = packet->buf_to_double();
}

void shrimp_server_data_t::pack(core::net_packet_ptr_t packet) {
	packet->int_to_buf(m_speed);
	packet->int_to_buf(m_steer);
	packet->int_to_buf(m_power_status);
	packet->int_to_buf(m_em_status);
	packet->double_to_buf(m_voltage);
	//packet->finalize_packet();
}

}}  //namespaces