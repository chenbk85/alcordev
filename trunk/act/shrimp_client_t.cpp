#include "shrimp_client_t.hpp"

namespace all {
	namespace act {

shrimp_client_t::shrimp_client_t(char* ini_file) {
	if (m_ini_config.Load(ini_file)) {
		all::core::ip_address_t server_address;
		server_address.hostname = m_ini_config.GetStringAsChar("server:address", "127.0.0.1");
		server_address.port = m_ini_config.GetInt("server:port", 12345);
		set_server_addr(server_address);
	}

	add_command_handler("serverData", boost::bind(&shrimp_client_t::update_shrimp_data, this, _1));
	set_connect_callback(boost::bind(&shrimp_client_t::connected_cb, this));
}

void shrimp_client_t::set_robot_on() {
	send_command("setRobotOn");
}

void shrimp_client_t::set_robot_standby() {
	send_command("setRobotOff");
}

void shrimp_client_t::em_stop() {
	send_command("emStop");
}

void shrimp_client_t::em_release() {
	send_command("emRelease");
}

void shrimp_client_t::set_speed(int speed) {
	net_packet_ptr_t packet(new net_packet_t());
	packet->int_to_buf(speed);
	send_command("setSpeed", packet);
}

void shrimp_client_t::set_steer(all::math::angle angle) {
	net_packet_ptr_t packet(new net_packet_t());
	packet->int_to_buf(angle.deg());
	send_command("setSteer", packet);
}

void shrimp_client_t::update_shrimp_data(net_packet_ptr_t packet) {
	m_shrimp_data.import(packet);
}

void shrimp_client_t::connected_cb() {
	//send_request("getData", 100);
}

int shrimp_client_t::get_speed() {
	return m_shrimp_data.m_speed;
}

all::math::angle shrimp_client_t::get_steer() {
	all::math::angle steer(m_shrimp_data.m_steer, all::math::deg_tag);
	return steer;
}

bool shrimp_client_t::is_on() {
	return m_shrimp_data.m_power_status;
}

bool shrimp_client_t::is_em() {
	return m_shrimp_data.m_em_status;
}

double shrimp_client_t::get_voltage() {
	return m_shrimp_data.m_voltage;
}

}} //namespaces