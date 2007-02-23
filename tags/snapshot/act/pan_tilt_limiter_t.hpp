#ifndef pan_tilt_limiter_t_H_INCLUDED
#define pan_tilt_limiter_t_H_INCLUDED

#include <utility>
#include "alcor\core\pantilt_angle_t.h"

namespace all { namespace act {

typedef std::pair<double, double> double_pair_t;


class pan_tilt_limiter_t 
{
public:
  pan_tilt_limiter_t()
  {
    //pan
    pan_range_ = std::make_pair(-135.0, 135.0);
    //tilt
    tilt_range_ = std::make_pair(-20.0, 0);

  }

  void set_range(const double_pair_t& pan_range
                ,const double_pair_t& tilt_range)
  {
  pan_range_  = pan_range;
  tilt_range_ = tilt_range;
  }

  core::pantilt_angle_t operator()(double pan, double tilt )
  {

    pan = (pan) >  ( pan_range_.first)  ? (pan) : (pan_range_.first);
    pan = (pan) <  ( pan_range_.second)  ? (pan) : (pan_range_.second);

    tilt = ((tilt) >  ( tilt_range_.first) ) ? (tilt) : (tilt_range_.first);
    tilt = ((tilt) <  ( tilt_range_.second)) ? (tilt) : (tilt_range_.second);

    return pantilt_angle_t(pan, tilt);
  }

private:
  double_pair_t pan_range_;
  double_pair_t tilt_range_;
};

}}
#endif //pan_tilt_limiter_t_H_INCLUDED