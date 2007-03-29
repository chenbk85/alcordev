#ifndef urg_3d_scanner_H_INCLUDED
#define urg_3d_scanner_H_INCLUDED

#include <alcor.extern/cgal/config.h>

#include <alcor/sense/urg_laser_t.hpp>
#include <alcor/act/pololu_ctrl_client_t.hpp>

#include <alcor/math/angle.h>
#include <alcor/core/core.h>
#include <boost/shared_array.hpp>
#include <alcor.extern/cgal/basic.h>


#include "urg_pcd_t.hpp"

namespace all {
	namespace sense {


class urg_3d_scanner_t {

public:

	enum scan_mode {
		STEP_SCAN,
		LINEAR_SCAN
	};

	urg_3d_scanner_t(urg_laser_t*, int laser_servo);

	void set_scan_mode(scan_mode);

	urg_pcd_ptr do_scan();

	void reset(all::math::angle h_start_angle, all::math::angle h_end_angle, all::math::angle v_start_angle, all::math::angle v_end_angle, int v_lines);

private:

	urg_pcd_ptr do_linear_scan();
	urg_pcd_ptr do_step_scan();

	bool get_cartesian(double rho, double theta, double phi, all::core::single_t& x, all::core::single_t& y, all::core::single_t& z); 


private:

	all::core::double_sarr th_rad_index;
	all::core::double_sarr ph_rad_index;

private:

	urg_pcd_ptr m_scan;
	
	scan_mode m_scan_mode;
	
	all::math::angle m_h_start_angle, m_h_end_angle;
	all::math::angle m_v_start_angle, m_v_end_angle;

	urg_laser_t* m_urg;

	boost::shared_ptr <all::act::pololu_ctrl_client_t> m_pololu;
	int m_laser_servo;
};

typedef boost::shared_ptr<urg_3d_scanner_t> urg_3d_scanner_ptr;

}} //namespaces

#endif

