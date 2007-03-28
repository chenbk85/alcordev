#ifndef SPLAM_DATA_H
#define SPLAM_DATA_H
//-----------------------------------------------------------------------------------------------
#include <vector>
#include <boost/shared_ptr.hpp>
#include "alcor/core/saliency_data_t.hpp"
#include "alcor/math/size2d.h"
#include "alcor/math/pose2d.h"
#include "alcor/math/point2d.h"
#include "alcor/math/geom2dtypes.h"
#include "alcor/splam/scan_data.h"
//-----------------------------------------------------------------------------------------------
using namespace all::math;
using namespace all::core;
//-----------------------------------------------------------------------------------------------
namespace all{
namespace splam{

struct	goal_t
{
public:
	bool			is_valid_;
	pose2d			goal_far_;
	pose2d			goal_near_;
	angle			head_direction_;
	angle			relative_head_direction_;
	points2d	path_;
	bool			recognition_;

public:
	goal_t()		{reset();}
	void	reset()	{is_valid_=false; goal_far_.reset(); goal_near_.reset(); head_direction_.reset(); relative_head_direction_.reset(); path_.clear(); recognition_=false;}
};

typedef	boost::shared_ptr<goal_t>	goal_ptr_t;

class splam_data
{
	
public:
	splam_data(){}
	~splam_data(){}

public:	// occupancy grid data
	map_values		og_cells_;		///< occupancy grid map occupancy value
	size_t			og_row_;		///< number of occupancy grid map raw... old mapYdimension
	size_t			og_col_;		///< number of occupancy grid map column... old mapXdimension
	double			og_resolution_;	///< dimension of occupancy grid map cells... in meters

public:	// paths
	poses2d		path_;
	saliency_path_t	observations_;
	attention_fov_t	attention_;

public:	// laser
	scan_data		last_scan_;		///< last laser scan

public:	// saliency grid data
	map_values		sg_cells_;		///< saliency grid: spatial saliency value
	size_t			sg_row_;		
	size_t			sg_col_;		
	double			sg_resolution_;	///< dimension of saliency grid map cells... in meters

public:	// goal
	goal_t			goal_;


public:	// services
	pose2d	get_current_position() const
	{
		if(path_.empty())
			return pose2d();
		return path_.back();
	}

	size2d	get_current_coord()	const
	{
		return get_coord_of(get_current_position().getP());
	}

	/**	
	 *	given a cell's coord (row,col), it returns the metric coord of the cell's center
	 */
	point2d	get_position_of(const size2d& temp) const 
	{
		return point2d((static_cast<int>(temp.col_) - static_cast<int>(og_col_)/2)*og_resolution_, (static_cast<int>(temp.row_) - static_cast<int>(og_row_)/2)*og_resolution_);
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

	bool	in_map(const point2d& temp) const {return in_map(get_coord_of(temp));}	
	bool	in_map(const size2d& temp) const {return temp.row_<og_row_ && temp.col_<og_col_;}	
	bool	in_map(size_t row, size_t col) const {return row<og_row_ && col<og_col_;}	
	void	metric_goal_finding();		///< find the goal of Metric-Based Exploration ...TOTEST...
	bool	saliency_goal_finding();	///< find the goal of Saliency-Based Exploration ...TODO...
	void	build_saliency_map(){sg_cells_.push_back('a');}	// build the SG map from observations, path and OG map ...TODO...
	void	save_og_pgm(const char*) const;		///< saves a pgm OG file ...TOTEST...
	void	save_sg_pgm(const char*) const;		///< saves a pgm SG file ...TOTEST...
	void	save_maps_png(const char*) const;	///< saves a png MAPS file ...TODO...
};

typedef	boost::shared_ptr<splam_data>	splam_data_ptr;

}//namespace splam
}//namespace all
//-----------------------------------------------------------------------------------------------
#endif
