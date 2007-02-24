#ifndef shrimp_gateway_t_H_INCLUDED
#define shrimp_gateway_t_H_INCLUDED

#include <memory>
#include <alcor/math/angle.h>

class shrimp_gateway_impl;

namespace all {
	namespace act {

class shrimp_gateway_t {
public:

	static const double SPEED_STEP_TO_MMpS;
	static const double STEER_STEP_TO_DEG;

	static const double SPEED_MMpS_TO_STEP;
	static const double STEER_DEG_TO_STEP;

	shrimp_gateway_t(char* ini_file = "config/shrimp_config.ini");

	bool connect();
	void disconnect();

	void set_robot_on();
	void set_robot_standby();

	void em_stop();
	void em_release();

	void set_speed(int);
	void set_steer(all::math::angle);

	int get_speed();
	all::math::angle get_steer();

	double get_voltage();

	bool is_on();
	bool is_em();


private:
	std::auto_ptr<shrimp_gateway_impl> pimpl;
};

}} //namespace

#endif