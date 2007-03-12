#ifndef SPLAM_DATA_H
#define SPLAM_DATA_H
//-----------------------------------------------------------------------------------------------
#include <vector>
#include "alcor/splam/scan_data.h"
#include "alcor/math/pose2d.h"
#include "alcor/math/point2d.h"
#include "alcor/math/size2d.h"
//-----------------------------------------------------------------------------------------------
using namespace all::math;
//-----------------------------------------------------------------------------------------------
namespace all{
namespace splam{

typedef	signed char					map_value;
typedef std::vector<map_value>		map_value_vect;
typedef map_value_vect::iterator	map_value_vect_it;
typedef	std::vector<pose2d>			pose_vect;
typedef pose_vect::iterator			pose_vect_it;

class splam_data
{
	
public:
	splam_data();
	~splam_data();

public:	// occupancy grid data
	map_value_vect	og_cells_;	///< occupancy grid map occupancy value
	size_t			og_row_;	///< number of occupancy grid map raw... old mapYdimension
	size_t			og_col_;	///< number of occupancy grid map column... old mapXdimension
	double			og_resolution_;	///< dimesion of occupancy grid map cells... in meters

public:	// path
	pose_vect		path_;

public:	// laser


public:	// goal


public:	// services
	/**	
	 *	given a cell's coord (raw,col), it returns the metric coord of the cell's center
	 */
	point2d	get_position_of(const size2d& temp) const 
	{
		return point2d((temp.get_x1() - og_row_/2)*og_resolution_, (temp.get_x2() - og_col_/2)*og_resolution_);
	}	

	/**	
	 *	given a metric position over the map, it returns the related cell's coord (raw,col)
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
	bool	in_map(const size2d& temp) const {return temp.get_x1()<og_row_ && temp.get_x2()<og_col_;}	

};

}//namespace splam
}//namespace all

//-----------------------------------------------------------------------------------------------
#endif
