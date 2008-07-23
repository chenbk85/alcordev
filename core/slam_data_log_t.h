#include "alcor/sense/urg_scan_data_t.hpp"
#include "alcor/math/pose2d.h"
#include "alcor/math/rpy_angle_t.h"


namespace all{
	namespace core{

struct slam_data_log_t{

	sense::urg_scan_data_ptr laserData; // laser variable
	
	// odometry section
	math::pose2d odo; // pose2d is composed by x,y and th

	// inertial sensor
	math::rpy_angle_t inertialSensor; // roll, pitch, yaw

	double timestamp;

}; //rawdata

	}} //core//all