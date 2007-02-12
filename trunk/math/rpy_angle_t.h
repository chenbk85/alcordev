#ifndef  rpy_angle_t_H_INC
#define  rpy_angle_t_H_INC

#include "angle.h"

namespace all { namespace math {
///
struct rpy_angle_t
{
  angle roll;
  angle pitch;
  angle yaw;
};

}}//namespace all::math

#endif //rpy_angle_t_H_INC