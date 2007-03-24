#include <iostream>
#include <bitset>
#include <alcor.extern/wxctb/serport.h>
#include <alcor.extern/wxctb/timer.h>
#include <alcor/core/iniWrapper.h>
#include <alcor/core/core.h>
#include "shrimp_command_t.hpp"



class shrimp_gateway_impl {

public:
	static const int COM_TIMEOUT = 3000;

	static const short MAX_SPEED_STEP = 23;
	static const short MAX_STEER_STEP = 18;
	
	static const int MAX_SPEED_MMpS = 450;
	static const int MAX_STEER_DEG = 90;
	
	shrimp_gateway_impl(const char* ini_file);

	
	bool connect();
	void disconnect();

	void set_robot_on();
	void set_robot_standby();

	void em_stop();
	void em_release();

	void set_speed(short);
	void set_steer(short);

	short get_speed();
	short get_steer();

	double get_voltage();

public:
	void init();

	bool is_feasible_speed(short);
	bool is_feasible_steer(short);
	
	void enable_voltage_reading();

	void update_power_state();
	void update_status_flags();
	void update_control_flags();

	int send_command(const shrimp_command_t&);
	int read_reply();


public:
	wxSerialPort m_shrimp;

	std::string m_com_port;

	timer m_io_timer;
	int m_timer_status;

	all::core::uint8_t m_reply;

	iniWrapper m_ini_config;

	std::bitset<8> m_control_flags;
	std::bitset<8> m_status_flags;
	std::bitset<8> m_power_state;

};

shrimp_gateway_impl::shrimp_gateway_impl(const char* ini_file): m_io_timer(COM_TIMEOUT, &m_timer_status, 0)
{
	if (m_ini_config.Load(ini_file)) {
		m_com_port = m_ini_config.GetStringAsChar("shrimp:port", "COM5");
	}
	else
		m_com_port = wxCOM5;

}

bool shrimp_gateway_impl::connect() {
	if ( m_shrimp.Open(m_com_port.c_str()) != -1) {
		printf("shrimp connected on port %s", m_com_port.c_str());
		init();
		return true;
	}
	else {
		printf("unable to open port %s", m_com_port.c_str());
		return false;
	}
}

void shrimp_gateway_impl::init() {
	m_shrimp.SetBaudRate(wxBAUD_9600);
	m_control_flags.set(0);
	m_control_flags.set(1);

}

void shrimp_gateway_impl::disconnect() {
	set_robot_standby();
	m_shrimp.Close();
}

bool shrimp_gateway_impl::is_feasible_steer(short steer) {
	return (abs(steer) <= MAX_STEER_STEP);
}

bool shrimp_gateway_impl::is_feasible_speed(short speed) {
	return (abs(speed) <= MAX_SPEED_STEP);
}

void shrimp_gateway_impl::enable_voltage_reading() {
	
	//update_control_flags();
	m_control_flags.set(6);

	shrimp_command_t enable_voltage(CONTROL_FLAGS, write_t, static_cast <all::core::uint8_t> (m_control_flags.to_ulong()));
	send_command(enable_voltage);
}

int shrimp_gateway_impl::send_command(const shrimp_command_t& command) {
	
	m_timer_status = 0;
	m_io_timer.start();

	unsigned char* cmd = new unsigned char[3];
	memcpy(cmd, command.to_buf(), command.get_size());
	//printf("%i %i %i", cmd[0], cmd[1], cmd[2]);

	int wd = m_shrimp.Writev(command.to_buf(), command.get_size(), &m_timer_status);
	//printf("bytes sent %i\n", wd);
	return wd;
}

int shrimp_gateway_impl::read_reply() {
	
	m_timer_status = 0;
	m_io_timer.start();
	char* r = new char[10];
	int rd = m_shrimp.Readv(r, 1, &m_timer_status);
	//printf("r: %i\n", r[0]);
	m_reply = r[0];
	//printf("byte read %i\n", rd);
	return rd;
}

void shrimp_gateway_impl::set_robot_on() {
	
	shrimp_command_t command(ROB_ON_OFF, write_t, 0x0f);
	send_command(command);
	
	enable_voltage_reading();

}

void shrimp_gateway_impl::set_robot_standby() {
	shrimp_command_t command(ROB_ON_OFF, write_t, 0x00);
	send_command(command);
}

void shrimp_gateway_impl::em_stop() {
	shrimp_command_t command(EM_STOP, write_t, 0x55);
	send_command(command);
}

void shrimp_gateway_impl::em_release() {
	shrimp_command_t command(EM_STOP, write_t, 0xAA);
	send_command(command);
}

void shrimp_gateway_impl::set_speed(short speed) {
	if (is_feasible_speed(speed)) {
		shrimp_command_t set_speed_command(SPEED, write_t, static_cast <all::core::uint8_t> (speed));
		send_command(set_speed_command);
		//update_control_flags();
		//m_control_flags.set(0);
		//m_control_flags.set(1);
		shrimp_command_t update_speed_command(CONTROL_FLAGS, write_t, static_cast <all::core::uint8_t> (m_control_flags.to_ulong()));
		send_command(update_speed_command);
	}
	else
		printf("Speed out of range\n");
}

void shrimp_gateway_impl::set_steer(short steer) {
	if (is_feasible_steer(steer)) {
		shrimp_command_t set_steer_command(STEER, write_t, static_cast <all::core::uint8_t> (steer));
		send_command(set_steer_command);
		//update_control_flags();
		//m_control_flags.set(0);
		//m_control_flags.set(1);
		shrimp_command_t update_steer_command(CONTROL_FLAGS, write_t, static_cast <all::core::uint8_t> (m_control_flags.to_ulong()));
		send_command(update_steer_command);
	}
	else
		printf("Steer out of range\n");
}

short shrimp_gateway_impl::get_speed() {
	shrimp_command_t command(SPEED, read_t);
	send_command(command);
	if (read_reply()) {
		return m_reply;
	}
	else
		return 0;
}

short shrimp_gateway_impl::get_steer() {
	shrimp_command_t command(STEER, read_t);
	send_command(command);
	if (read_reply()) {
		return m_reply;
	}
	else
		return 0;
}

double shrimp_gateway_impl::get_voltage() {
	
	shrimp_command_t command(V_BATT, read_t);
	send_command(command);
	if (read_reply()) {
		//printf("voltage: %i\n", m_reply);
		double voltage = (m_reply >> 4);
		return voltage;
	}
	else
		return 0;
}

void shrimp_gateway_impl::update_status_flags() {
	shrimp_command_t command(STATUS_FLAGS, read_t);
	send_command(command);
	if (read_reply()) {
		m_status_flags = m_reply;
		std::cout << "status flag: " << m_status_flags << std::endl;
	}
	else
		printf("error getting status flags\n");
}

void shrimp_gateway_impl::update_power_state() {
	shrimp_command_t command(PWR_STATE, read_t);
	send_command(command);
	if (read_reply()) {
		m_power_state = m_reply;
		std::cout << "power state: " << m_power_state << std::endl;
	}
	else
		printf("error getting power state\n");
}

void shrimp_gateway_impl::update_control_flags() {
	shrimp_command_t command(CONTROL_FLAGS, read_t);
	send_command(command);
	if (read_reply()) {
		m_control_flags = m_reply;
		std::cout << "control flags: " << m_control_flags << std::endl;
	}
	else
		printf("error getting control flags\n");
}
