#pragma once//oggi sono pigro
#include <utility>
#include "alcor/core/core.h"
#include "alcor/core/pantilt_angle_t.h"
#include <cmath>
#include <boost/config.hpp>

namespace all { namespace math {

struct pinhole_t
{

 ///---------------------------------------------------------------------
  void pantilt_from_pixel(double row, double col, pantilt_angle_t& ptangle) 
  {
	//TODO
  double	nrows_2 = static_cast<double>(nrows/2.0);
	double	ncols_2 = static_cast<double>(ncols/2.0);

	double delta_row = row - nrows_2;
	double delta_col = col - ncols_2;

	double delta_pan =
      core::dconstants::rad_to_deg(std::asin(delta_col / focal));

	double delta_pitch =
      core::dconstants::rad_to_deg(std::asin(delta_row / focal));


  ptangle.pan  = delta_pan;
  ptangle.tilt = delta_pitch;
  };

  ///---------------------------------------------------------------------
  float delta_tilt_from_pixel(double row ) 
  {
	//TODO
  double	nrows_2 = static_cast<double>(nrows/2.0);

	double delta_row = row - nrows_2;

	float delta_pitch = static_cast<float> (
    core::dconstants::rad_to_deg(std::asin(delta_row / focal))
    );

  return delta_pitch;
  };
  ///---------------------------------------------------------------------
   double delta_pan_from_pixel(double col) 
    {
    //TODO:
    double	ncols_2 = static_cast<double>(ncols/2);
    double delta_col = col - ncols_2;

    double delta_pan =
      std::asin(delta_col / focal);

    return  -(  core::dconstants::rad_to_deg(delta_pan));
    };
  ///---------------------------------------------------------------------
   int delta_pixel_from_deg(double deg)
   {
     return static_cast<int>(focal * ::tan( core::dconstants::deg_to_rad(deg) ));
   };

  double focal;
  size_t nrows;
  size_t ncols;

};

}}

