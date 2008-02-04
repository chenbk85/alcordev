#ifndef urg_scan_data_t_H_INCLUDED
#define urg_scan_data_t_H_INCLUDED

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <alcor/math/angle.h>

namespace all {
	namespace sense {

class urg_scan_data_t {

public:
	enum
	{
		default_start		=	 44,
		minus_pi			=	128,
		front				=	384,
		plus_pi				=	640,
		default_end			=	725,
		max_scan_points		=	769
	};

	typedef short int value_type;

	static const int MAX_NUM_POINTS = 769;



	static all::math::angle step2angle(int);
	static int angle2step(all::math::angle);
	static all::math::angle resolution(int);

	urg_scan_data_t();
	//return the horizontal angle of the i-th element of scan_points
	all::math::angle get_angle(int) const;
	
	std::vector <all::math::angle> get_deg_vec() const;
    

public:
	int time_stamp;
	
	int start_step;
	int end_step;
	int cc;

	std::vector <value_type> scan_points;
};

typedef boost::shared_ptr <urg_scan_data_t> urg_scan_data_ptr;

typedef struct {
	boost::shared_array <urg_scan_data_ptr> scan_vec;
	int n_scan;
} urg_multi_scan_t;


}} //namespaces


#endif