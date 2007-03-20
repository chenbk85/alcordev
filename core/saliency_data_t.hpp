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
	point2d	relative_position_;	// changed... point2d NON pose2d
	double	radius_;			// in METERS
	double	weight_;			// probability of being 
};

typedef std::vector<observation_data_t>		observation_data_vect_t;
typedef observation_data_vect_t::iterator	observation_data_vect_it_t;

struct saliency_data_t
{
	size_t						scan_count_;
	pantilt_angle_t				bearing_;
	observation_data_vect_t		observations_;
	bool						recognition_;
};

typedef std::vector<saliency_data_t>	saliency_path_t;
typedef saliency_path_t::iterator		saliency_path_it_t;

}//namespace core
}//namespace all
//---------------------------------------------------------------------------
#endif