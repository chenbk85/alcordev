#include "urg_3d_scanner_t.hpp"


namespace all {
	namespace sense {


urg_3d_scanner_t::urg_3d_scanner_t(all::sense::urg_laser_t* urg, all::act::PololuServoController* pololu, int laser_servo) {
	m_urg = urg;
	m_pololu = pololu;
	m_laser_servo = laser_servo;
	m_scan_mode = LINEAR_SCAN;

}


void urg_3d_scanner_t::reset(all::math::angle h_start_angle, all::math::angle h_end_angle, all::math::angle v_start_angle, all::math::angle v_end_angle, int v_lines) {

	m_scan.reset(new urg_pcd_t);

	m_h_start_angle = h_start_angle;
	m_h_end_angle = h_end_angle;

	m_v_start_angle = v_start_angle;
	m_v_end_angle = v_end_angle;

	all::sense::urg_scan_data_ptr test_scan = m_urg->do_scan(h_start_angle, h_end_angle, 1);

	m_scan->ncols = test_scan->scan_points.size();
	m_scan->nrows = v_lines;

	m_scan->cartesian_buffer.reset(new all::core::single_t[m_scan->ncols * m_scan->nrows * 3]);
	m_scan->polar_buffer.reset(new all::core::int16_t[m_scan->ncols * m_scan->nrows]);

	m_scan->th_index.reset(new all::core::single_t[m_scan->ncols]);
	m_scan->ph_index.reset(new all::core::single_t[m_scan->nrows]);

	//m_scan->x_buffer_ptr = m_scan->cartesian_buffer.get();
	//m_scan->y_buffer_ptr = m_scan->x_buffer_ptr + (m_scan->nrows * m_scan->ncols);
	//m_scan->z_buffer_ptr = m_scan->y_buffer_ptr + (m_scan->nrows * m_scan->ncols);

	
	//temp vector for trigonometric computation
	ph_rad_index.reset(new all::core::double_t[m_scan->nrows]);
	th_rad_index.reset(new all::core::double_t[m_scan->ncols]);

	//init theta index
	for (int i=0; i < m_scan->ncols; i++) {
		all::math::angle th_angle = test_scan->get_angle(i);
		m_scan->th_index[i] = th_angle.deg();
		th_rad_index[i] = th_angle.rad();

		printf("rad theta: %f ", th_rad_index[i]);
	}

	//init phi index
	double ph = m_v_start_angle.deg();
	double delta_ph = (m_v_end_angle.deg() - m_v_start_angle.deg()) / (double)m_scan->nrows;
	
	for (int i = 0; i < m_scan->nrows; i++) {
		
		m_scan->ph_index[i] = ph;
		ph_rad_index[i] = all::core::constants<double>::deg_to_rad (ph);

		printf("rad phi: %f ", ph_rad_index[i]);
		
		ph+= delta_ph;
	}


}

void urg_3d_scanner_t::set_scan_mode(scan_mode mode) {
	m_scan_mode = mode;
}

urg_pcd_ptr urg_3d_scanner_t::do_scan() {

	if (m_scan_mode == STEP_SCAN) 
		return do_step_scan();
	else
		return do_linear_scan();
}

urg_pcd_ptr urg_3d_scanner_t::do_linear_scan() {

	m_pololu->setSpeed(m_laser_servo, 40);
	m_pololu->setPoseBlk(m_laser_servo, m_v_start_angle.deg());

	double delta_angle = m_v_end_angle.deg() - m_v_start_angle.deg();

	//angle between scanlines
	double delta_scan = delta_angle / (double)m_scan->nrows;

	//speed in deg per second (100ms per scan)
	double speed = delta_scan * 10;

	m_pololu->setSpeedDeg(m_laser_servo, speed);

	m_pololu->setPose(m_laser_servo, m_v_end_angle.deg());

	printf("rows: %i\n", m_scan->nrows);

	all::sense::urg_multi_scan_t scan;
	scan = m_urg->do_multiple_scan(m_h_start_angle, m_h_end_angle, 1, m_scan->nrows, 0);
	
	//vertical angle between single scan points
	double delta_ph = all::core::constants<double>::deg_to_rad (delta_scan / (double) m_scan->ncols);
	
	int buf_index = 0;
	double ph;

	m_scan->m_point_cloud.clear();

	for (int i=0; i < m_scan->nrows; i++) {
		
		ph = ph_rad_index[i];
		
		for (int j=0; j < m_scan->ncols; j++) {
			m_scan->polar_buffer[buf_index] = scan.scan_vec[i]->scan_points[j];
			
			////planar buffer
			//get_cartesian(m_scan->polar_buffer[buf_index], th_rad_index[j], ph, 
			//			  m_scan->x_buffer_ptr[buf_index], m_scan->y_buffer_ptr[buf_index],
			//			  m_scan->z_buffer_ptr[buf_index]);

			
			//interleaved buffer
			if (get_cartesian(m_scan->polar_buffer[buf_index], th_rad_index[j], ph, 
							 m_scan->cartesian_buffer[buf_index * 3], m_scan->cartesian_buffer[buf_index * 3 + 1],
							 m_scan->cartesian_buffer[buf_index * 3 + 2]))

				//cgal point vector
				m_scan->m_point_cloud.push_back(urg_pcd_t::Point_3(m_scan->cartesian_buffer[buf_index * 3], m_scan->cartesian_buffer[buf_index * 3 +1],
														 m_scan->cartesian_buffer[buf_index * 3 +2]));


			//ph += delta_ph;
			buf_index++;

		}

	}

	printf ("valid point: %i\n", m_scan->m_point_cloud.size());
	return m_scan;
}


urg_pcd_ptr urg_3d_scanner_t::do_step_scan() {
	return m_scan;
}

bool urg_3d_scanner_t::get_cartesian(double rho, double theta, double phi, all::core::single_t& x, all::core::single_t& y, all::core::single_t& z) {
	if (rho > 19) {
		rho = rho / 1000.0f;
		z = rho * sin(3.14/2-phi) * cos(theta);
		x = rho * sin(3.14/2-phi) * sin(theta);
		y = rho * cos(3.14/2-phi);
		return true;
	}
	else {
		x = 0;
		y = 0;
		z = 0;
		return false;
	}
}

		

}} //namespaces










