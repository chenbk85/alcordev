#ifndef urg_laser_t_H_INCLUDED
#define urg_laser_t_H_INCLUDED

#include <memory>
#include <vector>
#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>

#include <alcor/math/angle.h>

#include "urg_scan_data_t.hpp"

namespace all {
	namespace sense {
		
		namespace detail {
			class urg_laser_impl;
		}

class urg_laser_t {

public:
	static all::math::angle step2angle(int);
	static int angle2step(all::math::angle);

	urg_laser_t();
	
	bool connect();
	void disconnect();

	void set_d_mode();
	void set_s_mode();

	void laser_on();
	void laser_off();

	urg_scan_data_ptr do_scan(int, int, int);
	boost::shared_array <urg_scan_data_ptr> do_multiple_scan(int, int, int, int, int);

	urg_scan_data_ptr do_scan(all::math::angle, all::math::angle, int);
	boost::shared_array <urg_scan_data_ptr> do_multiple_scan(all::math::angle, all::math::angle, int, int, int);

	boost::shared_array <urg_scan_data_ptr> get_scan_vector();

protected:
	std::auto_ptr <detail::urg_laser_impl> pimpl;
};

}} //namespaces

#endif