#ifndef metric_utils_H_INCLUDED
#define metric_utils_H_INCLUDED

#include "alcor/math/all_math.h"
#include "alcor/core/core.h"

namespace all { namespace math {

struct metric_utils
{
  static double delta_angle_as_degree(math::pose2d pose_, math::point2d target_)
  {
    double delta1 = target_.get_x1() - pose_.get_x1();
    double delta2 = target_.get_x2() - pose_.get_x2();

    double delta_ang  = ::atan2(-delta2, delta1);
    double delta_finale = delta_ang - pose_.get_th(math::rad_tag);
    
    return core::dconstants::rad_to_deg(delta_finale)  ;

  };

  static double target_tilt_angle_as_degree(math::point2d from, math::point2d to, double height)
  {
    double dist = math::distance(from, to);

    if(dist != 0)
    {
      double theta_rad = ::asin(height/dist);
      return core::dconstants::rad_to_deg(theta_rad);
    }
    else return 0;
  }
};

}} //namespaces
#endif //metric_utils_H_INCLUDED