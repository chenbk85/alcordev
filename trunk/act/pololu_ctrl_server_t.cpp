#include "pololu_ctrl_server_t.hpp"

namespace all {
	namespace act {

pololu_ctrl_server_t::pololu_ctrl_server_t(char* ini_file) : m_pololu(ini_file) {
	if (m_ini_config.Load(ini_file)) {
		int port = m_ini_config.GetInt("server:port", 54321);
		set_port(port);
	}

	add_command_handler("set_pose", boost::bind(&pololu_ctrl_server_t::set_pose, this, _1, _2));
	add_command_handler("set_speed", boost::bind(&pololu_ctrl_server_t::set_speed, this, _1, _2));

	add_command_handler("get_data", boost::bind(&pololu_ctrl_server_t::send_pololu_data, this, _1, _2));
	add_command_handler("get_num_servo", boost::bind(&pololu_ctrl_server_t::send_num_servo, this, _1, _2));

	m_pololu.connect();

	m_pololu_data.init(m_pololu.getNServo());

}

void pololu_ctrl_server_t::set_pose(client_connection_ptr_t client, net_packet_ptr_t packet) {
	int servo_number = packet->buf_to_int();
	double angle = packet->buf_to_double();
	m_pololu.setPose(servo_number, angle);
}

void pololu_ctrl_server_t::set_speed(client_connection_ptr_t client, net_packet_ptr_t packet) {
	int servo_number = packet->buf_to_int();
	int speed = packet->buf_to_int();
	m_pololu.setSpeed(servo_number, speed);
}

void pololu_ctrl_server_t::set_speed_deg(client_connection_ptr_t client, net_packet_ptr_t packet) {
	int servo_number = packet->buf_to_int();
	double speed = packet->buf_to_double();
	m_pololu.setSpeedDeg(servo_number, speed);
}

void pololu_ctrl_server_t::send_num_servo(client_connection_ptr_t client, net_packet_ptr_t packet) {
	packet.reset(new net_packet_t());
	packet->int_to_buf(m_pololu_data.n_servo);
	send_answer_packet("n_servo", client, packet);
}

	
void pololu_ctrl_server_t::send_pololu_data(client_connection_ptr_t client, net_packet_ptr_t packet) {
	update_pololu_data();
	packet.reset(new net_packet_t());
	m_pololu_data.pack(packet);
	send_answer_packet("server_data", client, packet);
}


void pololu_ctrl_server_t::update_pololu_data() {
	for (int i = 0; i < m_pololu_data.n_servo; i++) {
		m_pololu_data.servo[i].pose = m_pololu.getPose(i);
		m_pololu_data.servo[i].speed = m_pololu.getSpeed(i);
	}
}

}}