#ifndef depth_image_utils_HPP_INCLUDED
#define depth_image_utils_HPP_INCLUDED

#include "alcor/core/core.h"
#include "alcor/core/pixel_traits.h"

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>

namespace all { namespace core {

  ///
  struct pixelcoord_t
  {
    size_t row,col;
  };

  ///
  template<typename T/*, size_t DIM*/>
  struct point3d_t
  {
    typedef T value_type;

    point3d_t(T x_ = T(), T y_ = T(), T z_= T())
      :x(x_),y(y_),z(z_)
    {
    }

    T x,y,z;
  };

  ///
  typedef struct point3d_t<float> 
          point3d16_t;
  ///
  typedef struct point3d_t<double> 
          point3d32_t;


  ///Generates concrete intervals from a ROI (center, halfsize).
  struct roi_2d_t
  {
    roi_2d_t(pixelcoord_t center, size_t half_size)
    {
      //topleft
      topleft.row = center.row - half_size;
      topleft.col = center.col - half_size;
      bottomright.row = center.row + half_size;
      bottomright.col = center.col + half_size;
    }

    void clip_to(size_t nrows, size_t ncols)
    {
      clip(topleft,nrows,ncols);
      clip(bottomright, nrows,ncols);
    }

   size_t ndata() const
   {
     return (bottomright.row - topleft.col)*(bottomright.col - topleft.col);
   }

    void clip(pixelcoord_t& coord, size_t nrows, size_t ncols)
    {
      coord.row = (coord.row >= 0)    ? (coord.row):(0);
      coord.row = (coord.row < nrows) ? (coord.row):(nrows-1);

      coord.col = (coord.col >= 0)    ? (coord.col):(0);
      coord.col = (coord.col < ncols) ? (coord.col):(ncols-1);
    }

    pixelcoord_t topleft;
    pixelcoord_t bottomright;
  };


  ///
 inline  point3d16_t 
    estimate_depth(core::depth_image_t& depth, pixelcoord_t center, size_t hsize)
  {
    roi_2d_t roi (center, hsize);
    roi.clip_to(depth.height(), depth.width());

    using namespace boost::accumulators;

    accumulator_set<double, stats<tag::mean> > acc;

    core::depth_image_t::buffer_type data = depth.get_buffer_sptr();

    for(size_t it_r = roi.topleft.row;  it_r < roi.bottomright.row; it_r++ )
    {
      for(size_t it_c = roi.topleft.col; it_c < roi.bottomright.col; it_c++)
      {
        //calc norm
        //double norm = squared_norm<float,3>(data[]);
        //acc(data[])
      }
    }

    return point3d16_t();
  }


}}


#endif //depth_image_utils_HPP_INCLUDED