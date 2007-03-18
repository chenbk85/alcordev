#ifndef saliency_data_t_H_INCLUDED
#define saliency_data_t_H_INCLUDED
//---------------------------------------------------------------------------
#include "alcor/math/pose2d.h"
#include "alcor/core/pantilt_angle_t.hpp"
//---------------------------------------------------------------------------
using namespace all::math;
//---------------------------------------------------------------------------
namespace all{
namespace core{

/**
 *	this struct should be initializated in a constructor by an ini file.
 */

struct attention_fov_t
{
	angle	horizontal_fov_;	// symmetric assumption
	double	near_;				// closest attention distance 
	double	far_;				// farthest  attention distance
};

struct observation_data_t
{
	pose2d	relative_position_;
	double	radius_;	// in METERS
	double	weight_;	// probability of being 
};

struct saliency_data_t
{
	size_t				scan_count_;
	pantilt_angle_t		bearing_;
	std::vector<observation_data_t>	observations_;
};

}//namespace core
}//namespace all
//---------------------------------------------------------------------------
#endif