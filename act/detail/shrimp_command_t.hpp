#ifndef shrimp_command_t_H_INCLUDED
#define shrimp_command_t_H_INCLUDED

#include <alcor/core/core.h>

typedef struct {
	char address;
	char read_only;
} shrimp_register_t;

struct read_tag{};
struct write_tag{};

static const read_tag read_t = read_tag();
static const write_tag write_t = write_tag();
	
static const shrimp_register_t MODULE = {0x00, 1};
static const shrimp_register_t CONTROL_FLAGS = {0x35, 0};
static const shrimp_register_t SPEED = {0x36, 0};
static const shrimp_register_t STEER = {0x37, 0};
static const shrimp_register_t STATUS_FLAGS = {0x3A, 1};
static const shrimp_register_t ROB_ON_OFF = {0x3c, 0};
static const shrimp_register_t EM_STOP = {0x3D, 0};
static const shrimp_register_t PWR_STATE = {0x40, 1};
static const shrimp_register_t V_BATT = {0x41, 1};

class shrimp_command_t {

public:

	static const int COMMAND_BUF_SIZE = 3;

	shrimp_command_t(shrimp_register_t, read_tag);
	shrimp_command_t(shrimp_register_t, write_tag, all::core::uint8_t);
	
	~shrimp_command_t();
	
	char* to_buf() const;
	short get_size() const {return m_size;}
		
private:
	char* m_buf;
	unsigned short m_size;

};

#endif