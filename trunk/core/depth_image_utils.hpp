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
  template<typename T, size_t DIM=3>
  struct pointnd_t
  {
    typedef T value_type;
    BOOST_STATIC_CONSTANT(size_t

    pointnd_t(core::traits<T>::const_ptr ptr)
    {
      memcpy(&p[0], ptr, core::traits<T>::size*DIM);
    }

    T[DIM] p;
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

  //template <typename T, size_t DIM>
  //T squared_distance()
  ///
  inline point3d16_t::value_type squared_distance(const point3d16_t& a, const point3d16_t& b)
  {

    return (    ((a.x - b.x)*(a.x - b.x)) 
              + ((a.y - b.y)*(a.y - b.y)) 
              + ((a.z - b.z)*(a.z - b.z)) 
              );
  }
  ///
  inline point3d16_t::value_type euclidean_distance(const point3d16_t& a, const point3d16_t& b)
  {
    return std::sqrt(squared_distance(a,b));
  }
  ///
  inline point3d16_t::value_type squared_distance_from_origin(const point3d16_t& p )
  {

    return (    ((p.x )*(p.x)) 
              + ((p.y )*(p.y)) 
              + ((p.z )*(p.z)) 
              );
  }
  ///
  inline point3d16_t::value_type euclidean_distance_from_origin(const point3d16_t& p)
  {
    return std::sqrt(squared_distance_from_origin(p));
  }

  ///
  inline  point3d16_t::value_type 
    estimate_depth(core::depth_image_t& depth, pixelcoord_t center, size_t hsize)
  {
    roi_2d_t roi (center, hsize);
    roi.clip_to(depth.height(), depth.width());

    using namespace boost::accumulators;

    accumulator_set<point3d16_t::value_type, stats<tag::mean, tag::moment<2> > > acc;

    core::depth_image_t::buffer_type data = depth.get_buffer_sptr();

    point3d16_t::value_type dist;
    point3d16_t p;

    for(size_t it_r = roi.topleft.row;  it_r < roi.bottomright.row; it_r++ )
    {
      for(size_t it_c = roi.topleft.col; it_c < roi.bottomright.col; it_c++)
      {
        //calc norm
        p.x = depth.get(it_r, it_c, 0);
        p.y = depth.get(it_r, it_c, 1);
        p.z = depth.get(it_r, it_c, 2);

        dist = squared_distance_from_origin(p);
        acc(dist);
      }
    }

    return (mean(acc));
  }


}}


#endif //depth_image_utils_HPP_INCLUDED