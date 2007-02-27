#ifndef depth_image_utils_HPP_INCLUDED
#define depth_image_utils_HPP_INCLUDED

#include "alcor/core/core.h"
#include "alcor/core/pixel_traits.h"

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/io.hpp>

namespace boost::numeric::ublas = ublas;

namespace all { namespace core {

  template<typename T>
  struct point3d_t
  {
    T value_type;

    point3d_t(T x_ = T(), T y_ = T(), T z_= T())
      :x(x_),y(y_),z(z_)
    {
    }

    T x,y,z;
  };

  typedef struct point3d_t<float> point3df_t;

  //point3d_t< pixel_traits<DEPTH>::value_type > 
  //  estimate_depth(const image_of<DEPTH>& depth_)
  //{
  //  
  //  return point3d_t< pixel_traits<DEPTH>::value_type > ();
  //}

  point3df_t 
      estimate_depth(ublas::matrix& depth_)
  {
    using namespace boost::numeric::ublas;

    return point3df_t();
  }


}}


#endif //depth_image_utils_HPP_INCLUDED