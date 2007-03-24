#ifndef CONVERSION_H
#define CONVERSION_H
//------------------------------------------------------------------------------------------------
#include <vector>
#include <cmath>
#include "ariautil.h"
#include "alcor.extern/pmap/slap.h"
#include "alcor/math/pose2d.h"
//------------------------------------------------------------------------------------------------
using namespace all::math;
//------------------------------------------------------------------------------------------------
namespace all{
namespace util{

inline pose2_t		ArPose_to_pose2_t(ArPose pose)
{
	pose2_t	temp;
	temp.pos.x = pose.getX();
	temp.pos.y = pose.getY();
	temp.rot = pose.getThRad();
	return temp;
}

inline ArPose		pose2_t_to_ArPose(pose2_t pose)
{
	ArPose	temp;
	temp.setX(pose.pos.x);
	temp.setY(pose.pos.y);
	temp.setThRad(pose.rot);
	return temp;
}

inline pose2_t		pose2d_to_pose2_t(pose2d pose)
{
	pose2_t	temp;
	temp.pos.x = pose.getX();
	temp.pos.y = pose.getY();
	temp.rot = pose.get_th(rad_tag);
	return temp;
}

inline pose2d		pose2_t_to_pose2d(pose2_t pose)
{
	return pose2d(pose.pos.x,pose.pos.y,pose.rot,rad_tag);
}

inline pose2d	ArPose_to_pose2d(ArPose pose)
{
	return pose2d(pose.getX(), pose.getY(), pose.getThRad(), rad_tag);
}

}//namespace util
}//namespace all
//------------------------------------------------------------------------------------------------
#endif