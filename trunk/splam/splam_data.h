#ifndef SPLAM_DATA_H
#define SPLAM_DATA_H
//-----------------------------------------------------------------------------------------------
#include <vector>
#include <boost/shared_ptr.hpp>
#include "alcor/splam/scan_data.h"
#include "alcor/math/size2d.h"
#include "alcor/math/geom2dtypes.h"
//-----------------------------------------------------------------------------------------------
using namespace all::math;
//-----------------------------------------------------------------------------------------------
namespace all{
namespace splam{

struct	goal_t
{
	bool		is_valid_;
	pose2d		goal_far_;
	pose2d		goal_near_;
	angle		head_direction_;
	angle		relative_head_direction_;
};

typedef	boost::shared_ptr<goal_t>	goal_ptr_t;

class splam_data
{
	
public:
	splam_data();
	~splam_data();

public:	// occupancy grid data
	map_values		og_cells_;		///< occupancy grid map occupancy value
	size_t			og_row_;		///< number of occupancy grid map raw... old mapYdimension
	size_t			og_col_;		///< number of occupancy grid map column... old mapXdimension
	double			og_resolution_;	///< dimesion of occupancy grid map cells... in meters

public:	// path
	pose2d_vect		path_;

public:	// laser
	scan_data		last_scan_;		///< last laser scan

public:	// goal
	goal_t			goal_;

public:	// services
	/**	
	 *	given a cell's coord (row,col), it returns the metric coord of the cell's center
	 */
	point2d	get_position_of(const size2d& temp) const 
	{
		return point2d((temp.row_ - og_row_/2)*og_resolution_, (temp.col_ - og_col_/2)*og_resolution_);
	}	

	/**	
	 *	given a metric position over the map, it returns the related cell's coord (row,col)
	 */
	size2d	get_coord_of(const point2d& temp) const
	{
		int temp1 = static_cast<int>((static_cast<double>(og_row_)/2.0) + (temp.getY()/og_resolution_));
		int temp2 = static_cast<int>((static_cast<double>(og_col_)/2.0) + (temp.getX()/og_resolution_));
		if((temp1>=0) && (temp2>=0))
			return size2d(temp1,temp2);
		return size2d_max;
	}
	/**	
	 *	given a metric position over the map, it returns "true" iff that position is inside the allocated area
	 */	
	bool	in_map(const point2d& temp) const {return in_map(get_coord_of(temp));}	
	
	/**	
	 *	given a cell's coord (raw,col), it returns "true" iff that position is inside the allocated area
	 *	i.e. 0 <= raw <= RAW and 0 <= col <= COL 
	 */	
	bool	in_map(const size2d& temp) const {return temp.row_<og_row_ && temp.col_<og_col_;}	

	/**
	 *	that function estimates the nearest metric goal, based only on Occupancy Values and the Frontier-Based
	 *  approach.
	 */
	void	metric_goal_finding(goal_ptr_t);

	/**
	 *	that function calculates next saliency goal and return TRUE if in that position the Recognition activity
	 *	should be launched
	 */
	bool	saliency_goal_finding(goal_ptr_t);

	/**
	 *	
	 */
	void	build_saliency_map();
};

typedef	boost::shared_ptr<splam_data>	splam_data_ptr;

}//namespace splam
}//namespace all
//-----------------------------------------------------------------------------------------------
#endif
