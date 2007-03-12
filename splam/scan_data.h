#ifndef SCAN_DATA_H
#define SCAN_DATA_H
//-----------------------------------------------------------------------------------------------
#include <vector>
#include "alcor/math/angle.h"
#include "alcor/math/pose2d.h"
//-----------------------------------------------------------------------------------------------
using namespace all::math;
//-----------------------------------------------------------------------------------------------
namespace all{
namespace splam{

typedef double						scan_value;
typedef	std::vector<scan_value>		scan_values;
typedef	scan_values::iterator		scan_values_it;

class scan_data
{
	
public:
	scan_data();
	~scan_data();

public:	// data
	// laser
	scan_values		ranges_;		///< scan data vector
	angle			start_angle_;	///< starting scan angle
	angle			angle_step_;	///< angle step

	// robot
	pose2d			odo_pose_;		///< odometric position of the scan
	size_t			scan_step_;		///< 

	// time
	time_t			tempo_;			///< istante di tempo in cui i dati vengono acquisiti

public:	// services

};

}//namespace splam
}//namespace all

//-----------------------------------------------------------------------------------------------
#endif
