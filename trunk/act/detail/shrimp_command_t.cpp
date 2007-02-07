#include "shrimp_command_t.hpp"
	
//const shrimp_register_t MODULE = {0x00, 1};

//const shrimp_register_t CONTROL_FLAGS = {0x35, 0};
//const shrimp_register_t SPEED = {0x36, 0};
//const shrimp_register_t STEER = {0x37, 0};
//const shrimp_register_t STATUS_FLAGS = {0x3A, 1};
//const shrimp_register_t ROB_ON_OFF = {0x3C, 0};
//const shrimp_register_t EM_STOP = {0x3D, 0};
//const shrimp_register_t PWR_STATE = {0x40, 1};
//const shrimp_register_t V_BATT = {0x41, 1};


shrimp_command_t::shrimp_command_t(shrimp_register_t reg, read_tag) {
	m_buf = new char[COMMAND_BUF_SIZE];

	m_buf[0] = MODULE.address;
	m_buf[1] = reg.address;
	
	//set read bit
	m_buf[1] |= 0x80;
	m_size = 2;
}

shrimp_command_t::~shrimp_command_t() {
	delete[] m_buf;
}

shrimp_command_t::shrimp_command_t(shrimp_register_t reg, write_tag, all::core::uint8_t value) {
	m_buf = new char[COMMAND_BUF_SIZE];
	
	if (!reg.read_only) {
		m_buf[0] = MODULE.address;
		m_buf[1] = reg.address;
		m_buf[2] = value;
		m_size = 3;
	}
	else {
		printf("Error: the register is read only!");
		m_size = 0;
	}
}

char* shrimp_command_t::to_buf() const {
	return m_buf;
}