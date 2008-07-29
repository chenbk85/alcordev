#define WIN32_LEAN_AND_MEAN

#include "urg_3d_scanner_t.hpp"


namespace all {
	namespace sense {


urg_3d_scanner_t::urg_3d_scanner_t(all::sense::urg_laser_t* urg, int laser_servo) {
	m_urg = urg;
	m_pololu.reset(new all::act::pololu_ctrl_client_t("config/pololu_scan_client.ini"));
	m_pololu->run_async();
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
	//m_scan->cartesian_buffer.reset(new urg_pcd_t::Point_3*[m_scan->ncols * m_scan->nrows]);

	m_scan->th_index.reset(new all::core::single_t[m_scan->ncols]);
	m_scan->ph_index.reset(new all::core::single_t[m_scan->nrows]);

	m_scan->x_buffer_ptr = m_scan->cartesian_buffer.get();
	m_scan->y_buffer_ptr = m_scan->x_buffer_ptr + (m_scan->nrows * m_scan->ncols);
	m_scan->z_buffer_ptr = m_scan->y_buffer_ptr + (m_scan->nrows * m_scan->ncols);

	
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
	//double ph = m_v_start_angle.deg();
	//double delta_ph = (m_v_end_angle.deg() - m_v_start_angle.deg()) / (double)m_scan->nrows;
	
	all::math::angle ph = m_v_start_angle;
	all::math::angle delta_ph = (m_v_end_angle - m_v_start_angle) / (m_scan->nrows);

	for (int i = 0; i < m_scan->nrows; i++) {
		
		m_scan->ph_index[i] = ph.deg();
		ph_rad_index[i] = ph.rad();

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

	m_pololu->set_speed(m_laser_servo, 20);
	m_pololu->set_pose(m_laser_servo, m_v_start_angle.deg());

	Sleep(1000);

	double delta_angle = m_v_end_angle.deg() - m_v_start_angle.deg();

	//angle between scanlines
	double delta_scan = delta_angle / (double)m_scan->nrows;

	//speed in deg per second (100ms per scan)
	double speed = delta_scan * 10;

	m_pololu->set_speed_deg(m_laser_servo, speed);

	m_pololu->set_pose(m_laser_servo, m_v_end_angle.deg());

	printf("rows: %i\n", m_scan->nrows);

	all::sense::urg_multi_scan_t scan;
	scan = m_urg->do_multiple_scan(m_h_start_angle, m_h_end_angle, 1, m_scan->nrows, 0);
	
	//vertical angle between single scan points
	double delta_ph = all::core::constants<double>::deg_to_rad (delta_scan / (double) m_scan->ncols);
	
	int buf_index = 0;
	double ph;
	double th;

	all::core::single_t x,y,z;

	m_scan->m_point_cloud.clear();

	for (int i=0; i < m_scan->nrows; i++) {
		
		ph = ph_rad_index[i];
		
		for (int j=0; j < m_scan->ncols; j++) {

			th = th_rad_index[j];

			m_scan->polar_buffer[buf_index] = scan.scan_vec[i]->scan_points[j];

			
			//planar buffer
			if (get_cartesian(m_scan->polar_buffer[buf_index], th, ph, x, y, z)) {
				//cgal point vector
				m_scan->m_point_cloud.push_back(urg_pcd_t::Point_3(x, y, z));


				//m_scan->cartesian_buffer[buf_index] = &(*(m_scan->m_point_cloud.end()-1));
			}
			
			m_scan->x_buffer_ptr[buf_index] = x;
			m_scan->y_buffer_ptr[buf_index] = y;
			m_scan->z_buffer_ptr[buf_index] = z;

			buf_index++;

		}

	}

	printf ("valid point: %i\n", m_scan->m_point_cloud.size());
	return m_scan;
}


urg_pcd_ptr urg_3d_scanner_t::do_step_scan() {
	
	m_pololu->set_speed(m_laser_servo, 40);
	m_pololu->set_pose(m_laser_servo, m_v_start_angle.deg());

	Sleep(1000);

	all::sense::urg_scan_data_ptr scan;

	m_scan->m_point_cloud.clear();

	int buf_index = 0;

	all::core::single_t x,y,z;

	for (int i=0; i < m_scan->nrows; i++) {

		m_pololu->set_pose(m_laser_servo, m_scan->ph_index[i]);

		Sleep(100);
		
		scan = m_urg->do_scan(m_h_start_angle, m_h_end_angle, 1);

		for (int j=0; j < m_scan->ncols; j++) {
			
			m_scan->polar_buffer[buf_index] = scan->scan_points[j];

			if (get_cartesian(m_scan->polar_buffer[buf_index], th_rad_index[j], ph_rad_index[i], x, y, z))
				//cgal point vector
				m_scan->m_point_cloud.push_back(urg_pcd_t::Point_3(x, y, z));

			buf_index++;
		}

		Sleep(300);
	}

	return m_scan;
}


bool urg_3d_scanner_t::get_cartesian(double rho, double theta, double phi, all::core::single_t& x, all::core::single_t& y, all::core::single_t& z) {
	if (rho > 19) {
		//rho = rho / 1000.0f;
		z = rho * cos(phi) * cos(theta);
		x = rho * sin(theta);
		y = rho * sin(phi) * cos(theta);
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










