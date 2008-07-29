#ifndef urg_pcd_t_H_INCLUDED
#define urg_pcd_t_H_INCLUDED

#include <boost/shared_array.hpp>
#include <alcor/core/core.h>
#include <cgal/Cartesian.h>


namespace all {
	namespace sense {

class urg_pcd_t {

public:
	typedef CGAL::Cartesian<float> K;
	typedef K::Point_3 Point_3;

public:

	//all::core::single_sarr get_x_img();
	//all::core::single_sarr get_y_img();
	//all::core::single_sarr get_z_img();

public:

	int nrows;
	int ncols;
	
	all::core::single_sarr cartesian_buffer;
	boost::shared_array <all::core::int16_t> polar_buffer;

	//boost::shared_array<Point_3*> cartesian_buffer;

	all::core::single_ptr x_buffer_ptr;
	all::core::single_ptr y_buffer_ptr;
	all::core::single_ptr z_buffer_ptr;

	all::core::single_sarr th_index;
	all::core::single_sarr ph_index;

	std::vector<Point_3> m_point_cloud;
	
};

typedef boost::shared_ptr<urg_pcd_t> urg_pcd_ptr;

}} //namespaces

#endif