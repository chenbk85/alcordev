#ifndef pantilt_mx_adapter_HPP_INCLUDED 
#define pantilt_mx_adapter_HPP_INCLUDED

#include "alcor/matlab/detail/matlab_tags.hpp"

namespace all { namespace matlab {

struct pantilt_mx_adapter_t
{
  mxArray* operator()(double pan, double tilt)
  {
  mxArray* mx_pantilt = mxCreateDoubleMatrix(1,2,mxREAL);
  double* start_pr = mxGetPr(mx_pantilt);

  start_pr[0] = pan;
  start_pr[1] = tilt;
 
  return mx_pantilt;
  }
};

}}
#endif //pantilt_mx_adapter_HPP_INCLUDED