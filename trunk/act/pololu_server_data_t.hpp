#ifndef pololu_server_data_t_H_INCLUDED
#define pololu_server_data_t_H_INCLUDED

#include <alcor/core/net_packet_t.hpp>

namespace all {
	namespace act {

class pololu_server_data_t {

public:
	void init(int);

	void import(core::net_packet_ptr_t);
	void pack(core::net_packet_ptr_t);

public:
	typedef struct {
		double pose;
		int speed;
	} servo_data_t;

	servo_data_t* servo;
	int n_servo;
};


}}
#endif