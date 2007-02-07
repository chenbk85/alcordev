#include "shrimp_gateway_t.hpp"

#include "detail/shrimp_gateway_impl.cpp"



namespace all {
	namespace act {

const double shrimp_gateway_t::SPEED_STEP_TO_MMpS = shrimp_gateway_impl::MAX_SPEED_MMpS / shrimp_gateway_impl::MAX_SPEED_STEP;
const double shrimp_gateway_t::SPEED_MMpS_TO_STEP = 1 / SPEED_STEP_TO_MMpS;
const double shrimp_gateway_t::STEER_STEP_TO_DEG = shrimp_gateway_impl::MAX_STEER_DEG / shrimp_gateway_impl::MAX_STEER_STEP;
const double shrimp_gateway_t::STEER_DEG_TO_STEP = 1 / STEER_STEP_TO_DEG;


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

void shrimp_gateway_t::set_speed(int speed) {
	short step_speed = static_cast <short> (speed * SPEED_MMpS_TO_STEP);
	printf("speed: %i\n", step_speed);
	pimpl->set_speed(step_speed);
}

void shrimp_gateway_t::set_steer(all::math::angle steer) {
	short step_steer = static_cast <short> (steer.deg() * (STEER_DEG_TO_STEP));
	printf("steer: %i\n", step_steer);
	pimpl->set_steer(step_steer);
}

int shrimp_gateway_t::get_speed() {
	return static_cast <int> (pimpl->get_speed() * SPEED_STEP_TO_MMpS);
}

all::math::angle shrimp_gateway_t::get_steer() {
	short step_steer = static_cast <short> (pimpl->get_steer() * (STEER_STEP_TO_DEG));
	all::math::angle deg_steer(step_steer, all::math::deg_tag);
	return deg_steer;
}

double shrimp_gateway_t::get_voltage() {
	return pimpl->get_voltage();
}

bool shrimp_gateway_t::is_on() {
	pimpl->update_power_state();
	return pimpl->m_power_state.test(0);
}

bool shrimp_gateway_t::is_em() {
	pimpl->update_power_state();
	return pimpl->m_power_state.test(1);
}

}} //namespace
