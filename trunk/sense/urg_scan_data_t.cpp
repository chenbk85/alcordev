#include "urg_scan_data_t.hpp"

namespace all {
	namespace sense {

all::math::angle urg_scan_data_t::step2angle(int step) {
	return all::math::angle((360.0/1024.0)*(static_cast<double>(step) - 384.0), all::math::deg_tag);
}

int urg_scan_data_t::angle2step(all::math::angle a) {
	return static_cast<int> ((double(1024)/double(360)) * a.deg())+384;
}

all::math::angle urg_scan_data_t::resolution (int cc) {
	return all::math::angle(static_cast<double>(cc)*(360.0/1024.0) , all::math::deg_tag);
}

urg_scan_data_t::urg_scan_data_t() {
	scan_points.reserve(MAX_NUM_POINTS);
}

all::math::angle urg_scan_data_t::get_angle(int index) const {
	return urg_scan_data_t::step2angle(start_step + (index * cc));
}

std::vector <all::math::angle> urg_scan_data_t::get_deg_vec() const {
	
	std::vector <all::math::angle> deg_vec;
	
	for (int i=0; i<scan_points.size(); i++) {
		deg_vec.push_back(get_angle(i));
	}
	
	return deg_vec;
}


}}