#ifndef shrimp_gateway_t_H_INCLUDED
#define shrimp_gateway_t_H_INCLUDED

#include <memory>

class shrimp_gateway_impl;

namespace all {
	namespace act {

class shrimp_gateway_t {
public:
	shrimp_gateway_t(char* ini_file = "config/shrimp_config.ini");

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

	bool is_on();
	bool is_em();


private:
	std::auto_ptr<shrimp_gateway_impl> pimpl;
};

}} //namespace

#endif