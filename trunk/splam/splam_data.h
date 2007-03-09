#ifndef SPLAM_DATA_H
#define SPLAM_DATA_H
//-----------------------------------------------------------------------------------------------
#include "alcor/math/point2d.h"
#include "alcor/math/size2d.h"
//-----------------------------------------------------------------------------------------------
using namespace all::math;
//-----------------------------------------------------------------------------------------------
namespace all{
namespace splam{

class splam_data
{
	
public:
	splam_data();
	~splam_data();

public:
	/**	
	 *	given a cell's coord (raw,col), it returns the metric coord of the cell's center
	 */
	point2d	get_position_of(size2d);	

	/**	
	 *	given a metric position over the map, it returns the related cell's coord (raw,col)
	 */
	size2d	get_coord_of(point2d);	

	/**	
	 *	given a metric position over the map, it returns "true" iff that position is inside the allocated area
	 */	
	bool	in_map(point2d);	
	
	/**	
	 *	given a cell's coord (raw,col), it returns "true" iff that position is inside the allocated area
	 *	i.e. 0 <= raw <= RAW and 0 <= col <= COL 
	 */	
	bool	in_map(size2d);
};

}//namespace splam
}//namespace all

//-----------------------------------------------------------------------------------------------
#endif
