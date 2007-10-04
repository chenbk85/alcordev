#include "shrimp_server_t.hpp"

namespace all {
	namespace act {


shrimp_server_t::shrimp_server_t(char* ini_file) : m_shrimp(ini_file) {
	if (m_ini_config.Load(ini_file)) {
		int port = m_ini_config.GetInt("server:port", 12345);
		set_port(port);
	}
	
	add_command_handler("setRobotOn", boost::bind(&shrimp_server_t::set_robot_on, this, _1, _2));
	add_command_handler("setRobotOff", boost::bind(&shrimp_server_t::set_robot_standby, this, _1, _2));
	
	add_command_handler("emStop", boost::bind(&shrimp_server_t::em_stop, this, _1, _2));
	add_command_handler("emRelease", boost::bind(&shrimp_server_t::em_release, this, _1, _2));

	add_command_handler("setSpeed", boost::bind(&shrimp_server_t::set_speed, this, _1, _2));
	add_command_handler("setSteer", boost::bind(&shrimp_server_t::set_steer, this, _1, _2));

	add_command_handler("getData", boost::bind(&shrimp_server_t::send_shrimp_data, this, _1, _2));

	set_client_disconnect_cb(boost::bind(&shrimp_server_t::client_disconnected_cb, this, _1));

	if (m_shrimp.connect()) {
		//m_shrimp.set_robot_on();
	}
	else
		printf("Error connecting to robot\n");
}

void shrimp_server_t::stop() {
	server_base_t::stop();
	m_shrimp.set_robot_standby();
	m_shrimp.disconnect();
}

void shrimp_server_t::set_robot_on(client_connection_ptr_t client, net_packet_ptr_t packet) {
	m_shrimp.set_robot_on();
}

void shrimp_server_t::set_robot_standby(client_connection_ptr_t client, net_packet_ptr_t packet) {
	m_shrimp.set_robot_standby();
}

void shrimp_server_t::em_stop(client_connection_ptr_t client, net_packet_ptr_t packet) {
	m_shrimp.em_stop();
}

void shrimp_server_t::em_release(client_connection_ptr_t client, net_packet_ptr_t packet) {
	m_shrimp.em_release();
}

void shrimp_server_t::set_speed(client_connection_ptr_t client, net_packet_ptr_t packet) {
	int speed = packet->buf_to_int();
	m_shrimp.set_speed(speed);
}

void shrimp_server_t::set_steer(client_connection_ptr_t client, net_packet_ptr_t packet) {
	int steer = packet->buf_to_int();
	all::math::angle steer_angle(steer, all::math::deg_tag);

	m_shrimp.set_steer(steer_angle);
}

void shrimp_server_t::update_shrimp_data() {
	m_shrimp_data.m_speed = m_shrimp.get_speed();
	m_shrimp_data.m_steer = m_shrimp.get_steer().deg();
	m_shrimp_data.m_power_status = m_shrimp.is_on();
	m_shrimp_data.m_em_status = m_shrimp.is_em();
	m_shrimp_data.m_voltage = m_shrimp.get_voltage();

	//printf("speed %i\n", m_shrimp_data.m_speed);
	//printf("steer %i\n", m_shrimp_data.m_steer);
	//printf("power %i\n", m_shrimp_data.m_power_status);
	//printf("em %i\n", m_shrimp_data.m_em_status);
	//printf("voltage %f\n", m_shrimp_data.m_voltage);

}

void shrimp_server_t::send_shrimp_data(client_connection_ptr_t client, net_packet_ptr_t packet) {
	update_shrimp_data();
	packet.reset(new net_packet_t());
	m_shrimp_data.pack(packet);
	send_answer_packet("serverData", client, packet);
}

void shrimp_server_t::client_disconnected_cb(int num_client)  {
	if (num_client == 0) {
		m_shrimp.set_robot_standby();
	}
}


}} //namespaces
