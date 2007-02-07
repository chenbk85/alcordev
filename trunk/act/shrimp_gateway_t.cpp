#include "shrimp_gateway_t.hpp"

#include "detail/shrimp_gateway_impl.cpp"

namespace all {
	namespace act {

shrimp_gateway_t::shrimp_gateway_t(char* ini_file) {
	pimpl.reset(new shrimp_gateway_impl(ini_file));
}

bool shrimp_gateway_t::connect() {
	return pimpl->connect();
}

void shrimp_gateway_t::disconnect() {
	pimpl->disconnect();
}

void shrimp_gateway_t::set_robot_on() {
	pimpl->set_robot_on();
}

void shrimp_gateway_t::set_robot_standby() {
	pimpl->set_robot_standby();
}

void shrimp_gateway_t::em_stop() {
	pimpl->em_stop();
}

void shrimp_gateway_t::em_release() {
	pimpl->em_release();
}

void shrimp_gateway_t::set_speed(short speed) {
	pimpl->set_speed(speed);
}

void shrimp_gateway_t::set_steer(short steer) {
	pimpl->set_steer(steer);
}

short shrimp_gateway_t::get_speed() {
	return pimpl->get_speed();
}

short shrimp_gateway_t::get_steer() {
	return pimpl->get_steer();
}

double shrimp_gateway_t::get_voltage() {
	return pimpl->get_voltage();
}

bool shrimp_gateway_t::is_on() {
	pimpl->get_power_state();
	return pimpl->m_power_state.test(0);
}

bool shrimp_gateway_t::is_em() {
	pimpl->get_power_state();
	return pimpl->m_power_state.test(1);
}

}} //namespace
