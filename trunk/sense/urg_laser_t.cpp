#include "urg_laser_t.hpp"

#include "detail/urg_laser_impl.cpp"

namespace all {
	namespace sense {

urg_laser_t::urg_laser_t(char* ini_file) {
	pimpl.reset(new detail::urg_laser_impl(ini_file));
}

bool urg_laser_t::connect() {
	return pimpl->connect();
}

void urg_laser_t::disconnect() {
	pimpl->disconnect();
}

urg_scan_data_ptr urg_laser_t::do_scan(int start, int end, int cc) {
	return pimpl->do_scan(start, end, cc);
}

boost::shared_array <urg_scan_data_ptr> urg_laser_t::do_multiple_scan(int start, int end, int cc, int n_scan, int time_interval) {
	return pimpl->do_multiple_scan(start, end, cc, n_scan, time_interval);
}

urg_scan_data_ptr urg_laser_t::do_scan(all::math::angle start, all::math::angle end, int cc) {
	return pimpl->do_scan(angle2step(start), angle2step(end), cc);
}

boost::shared_array <urg_scan_data_ptr> urg_laser_t::do_multiple_scan(all::math::angle start, all::math::angle end, int cc, int n_scan, int time_interval) {
	return pimpl->do_multiple_scan(angle2step(start), angle2step(end), cc, n_scan, time_interval);
}

void urg_laser_t::set_d_mode() {
	pimpl->set_d_mode();
}

void urg_laser_t::set_s_mode() {
	pimpl->set_s_mode();
}

void urg_laser_t::laser_on() {
	pimpl->laser_on();
}

void urg_laser_t::laser_off() {
	pimpl->laser_off();
}

boost::shared_array <urg_scan_data_ptr> urg_laser_t::get_scan_vector() {
	return pimpl->m_scan_vec;
}

all::math::angle urg_laser_t::step2angle(int step) {
	double deg_angle = (double(360)/double(1024))*(step - 384);
	all::math::angle a(deg_angle, all::math::deg_tag);
	return a;
}

int urg_laser_t::angle2step(all::math::angle a) {
	return static_cast<int> ((double(1024)/double(360)) * a.deg())+384;
}

}} //namespaces
