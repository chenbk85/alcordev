#include "urg_scan_data_t.hpp"
#include "urg_laser_t.hpp"

namespace all {
	namespace sense {


urg_scan_data_t::urg_scan_data_t() {
	scan_points.reserve(MAX_NUM_POINTS);
}

all::math::angle urg_scan_data_t::get_angle(int index) const {
	return urg_laser_t::step2angle(start_step + (index * cc));
}

std::vector <all::math::angle> urg_scan_data_t::get_deg_vec() const {
	
	std::vector <all::math::angle> deg_vec;
	
	for (int i=0; i<scan_points.size(); i++) {
		deg_vec.push_back(get_angle(i));
	}
	
	return deg_vec;
}


}}