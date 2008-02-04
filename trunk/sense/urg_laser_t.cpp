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

void urg_laser_t::set_line_ready_cb(boost::function <void (urg_scan_data_ptr)> line_ready_cb) {
	pimpl->m_line_ready_cb = line_ready_cb;
}

urg_scan_data_ptr urg_laser_t::do_scan(int start, int end, int cc) {
	return pimpl->do_scan(start, end, cc);
}

urg_multi_scan_t urg_laser_t::do_multiple_scan(int start, int end, int cc, int n_scan, int time_interval) {
	return pimpl->do_multiple_scan(start, end, cc, n_scan, time_interval);
}

urg_scan_data_ptr urg_laser_t::do_scan(all::math::angle start, all::math::angle end, int cc) {
	return pimpl->do_scan(urg_scan_data_t::angle2step(start), urg_scan_data_t::angle2step(end), cc);
}

urg_multi_scan_t urg_laser_t::do_multiple_scan(all::math::angle start, all::math::angle end, int cc, int n_scan, int time_interval) {
	return pimpl->do_multiple_scan(urg_scan_data_t::angle2step(start), urg_scan_data_t::angle2step(end), cc, n_scan, time_interval);
}

void urg_laser_t::start_continous_scan(int start, int end, int cc, int time_interval) {
	pimpl->start_continous_scan(start, end, cc, time_interval);
}

void urg_laser_t::stop_continous_scan() {
	pimpl->stop_continous_scan();
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

urg_multi_scan_t urg_laser_t::get_scan_vector() {
	return pimpl->m_last_scan;
}

all::math::angle urg_laser_t::step2angle(int step) {
	return all::math::angle((360.0/1024.0)*(static_cast<double>(step) - 384.0), all::math::deg_tag);
}

int urg_laser_t::angle2step(all::math::angle a) {
	return static_cast<int> ((double(1024)/double(360)) * a.deg())+384;
}

all::math::angle urg_laser_t::resolution (int cc) {
	return all::math::angle(static_cast<double>(cc)*(360.0/1024.0) , all::math::deg_tag);
}

} //namespace sense
} //namespace all
