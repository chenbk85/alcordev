#include "pololu_ctrl_client_t.hpp"

namespace all {
	namespace act {

pololu_ctrl_client_t::pololu_ctrl_client_t(char* ini_file) {
	if (m_ini_config.Load(ini_file)) {
		all::core::ip_address_t server_addr;
		server_addr.hostname = m_ini_config.GetStringAsChar("server:address", "127.0.0.1");
		server_addr.port = m_ini_config.GetInt("server:port", 30000);
		set_server_addr(server_addr);
	}

	add_command_handler("server_data", boost::bind(&pololu_ctrl_client_t::get_server_data, this, _1));
	add_command_handler("n_servo", boost::bind(&pololu_ctrl_client_t::get_num_servo, this, _1));

	set_connect_callback(boost::bind(&pololu_ctrl_client_t::connected_cb, this));
}

void pololu_ctrl_client_t::set_pose(int servo_number, double pose) {
	core::net_packet_ptr_t packet(new core::net_packet_t());
	packet->int_to_buf(servo_number);
	packet->double_to_buf(pose);
	send_command("set_pose", packet);
}

void pololu_ctrl_client_t::set_speed(int servo_number, int speed) {
	core::net_packet_ptr_t packet(new core::net_packet_t());
	packet->int_to_buf(servo_number);
	packet->int_to_buf(speed);
	send_command("set_speed", packet);
}

void pololu_ctrl_client_t::set_speed_deg(int servo_number, double speed) {
	core::net_packet_ptr_t packet(new core::net_packet_t());
	packet->int_to_buf(servo_number);
	packet->double_to_buf(speed);
	send_command("set_speed_deg", packet);
}

double pololu_ctrl_client_t::get_pose(int servo_number) {
	return m_pololu_data.servo[servo_number].pose;
}

int pololu_ctrl_client_t::get_speed(int servo_number) {
	return m_pololu_data.servo[servo_number].speed;
}

void pololu_ctrl_client_t::get_num_servo(core::net_packet_ptr_t packet) {
	int num_servo = packet->buf_to_int();
	m_pololu_data.init(num_servo);
	send_request("get_data", 100); 
}

void pololu_ctrl_client_t::get_server_data(core::net_packet_ptr_t packet) {
	m_pololu_data.import(packet);
}

void pololu_ctrl_client_t::connected_cb() {
	send_command("get_num_servo");
}

}} //namespaces