#ifndef SCAN_DATA_H
#define SCAN_DATA_H
//-----------------------------------------------------------------------------------------------
#include <vector>
#include <ctime>
#include <boost/shared_ptr.hpp>
#include "alcor/math/pose2d.h"
#include "typedefs.hpp"
//-----------------------------------------------------------------------------------------------
using namespace all::math;
using namespace all::util;
//-----------------------------------------------------------------------------------------------
namespace all{
namespace splam{

class scan_data
{
public:	// data
	// laser
	scan_values		ranges_;		///< scan data vector
	angle			start_angle_;	///< starting scan angle
	angle			angle_step_;	///< angle step

	// robot
	pose2d			odo_pose_;		///< odometric position of the scan
	size_t			scan_step_;		///< 

	// time
	clock_t			time_stamp_;	///< istante di tempo in cui i dati vengono acquisiti

public:	// services
};

typedef boost::shared_ptr<scan_data> scan_data_ptr;

}//namespace splam
}//namespace all

//-----------------------------------------------------------------------------------------------
#endif
