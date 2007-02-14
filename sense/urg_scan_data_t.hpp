#ifndef urg_scan_data_t_H_INCLUDED
#define urg_scan_data_t_H_INCLUDED

#include <vector>
#include <boost/shared_ptr.hpp>
#include <alcor/math/angle.h>

namespace all {
	namespace sense {

class urg_scan_data_t {

public:
	static const int MAX_NUM_POINTS = 769;

	urg_scan_data_t();
	//return the horizontal angle of the i-th element of scan_points
	all::math::angle get_angle(int) const;
	
	std::vector <all::math::angle> get_deg_vec() const;
    

public:
	int time_stamp;
	
	int start_step;
	int end_step;
	int cc;

	std::vector <short int> scan_points;
};

typedef boost::shared_ptr <urg_scan_data_t> urg_scan_data_ptr;

}} //namespaces


#endif