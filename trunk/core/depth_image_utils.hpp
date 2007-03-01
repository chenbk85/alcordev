#ifndef depth_image_utils_HPP_INCLUDED
#define depth_image_utils_HPP_INCLUDED
//---------------------------------------------------------------------------+
#include "alcor/core/core.h"
#include "alcor/core/pixel_traits.h"
//---------------------------------------------------------------------------+
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>
#include <boost/range/iterator.hpp>
#include <boost/timer.hpp>
//---------------------------------------------------------------------------+
#include <boost/numeric/ublas/vector.hpp>
//---------------------------------------------------------------------------+
#include <utility>
//---------------------------------------------------------------------------+
//namespace boost::numeric::ublas = ublas
//---------------------------------------------------------------------------+
namespace all { namespace core {
//---------------------------------------------------------------------------+
  ///Pixel Coordinates
  struct pixelcoord_t
  {
    size_t row,col;
  };
//---------------------------------------------------------------------------+
  ///
  template<typename T, size_t DIM=3>
  struct pointnd_t
  {
    ///
    typedef T value_type;
    ///
    typedef T& reference;
    typedef const T& const_reference;

    ///
    BOOST_STATIC_CONSTANT(size_t, dim = DIM);
    ///
    pointnd_t()
    {
      vdata = boost::numeric::ublas::zero_vector<T>(DIM); 
    }
    ///
    pointnd_t(const pointnd_t<T,DIM>& point)
    {
      vdata=point.vdata;
    }

    inline reference       operator()(const std::size_t& index)       { return vdata[index]; }

    inline const_reference operator()(const std::size_t& index) const { return vdata[index]; }

    inline void assign(const_reference val, const std::size_t& index)
          {if (index < DIM ) vdata[index]= val; }

    ///
    inline pointnd_t<T,DIM>& operator=(const pointnd_t<T,DIM>& point)
    {
      vdata = point.vdata;
      return *this;
    }

    boost::numeric::ublas::vector<T> vdata;
  };
//////---------------------------------------------------------------------------+
  ///
  typedef struct pointnd_t<float,3> 
          point3df_t;

  typedef struct pointnd_t<float,3> 
          point3df_t;
//---------------------------------------------------------------------------+
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

    size_t minrow() const {return topleft.row;}
    size_t maxrow() const {return bottomright.row;}
    size_t mincol() const {return topleft.col;}
    size_t maxcol() const {return bottomright.col;}

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
//---------------------------------------------------------------------------+
  ///
  inline point3df_t::value_type euclidean_distance(const point3df_t& point)
  {
    return(boost::numeric::ublas::norm_2(point.vdata));
  }
//---------------------------------------------------------------------------+
  ///
  inline point3df_t::value_type squared_distance(const point3df_t& point)
  {
    return(boost::numeric::ublas::norm_1(point.vdata));
  }
//---------------------------------------------------------------------------+
//---------------------------------------------------------------------------+
//---------------------------------------------------------------------------+
  struct mystat
  {
  core::single_t          mean;
  core::single_t          stddev;
  };
//---------------------------------------------------------------------------+
  ///
  inline  mystat 
    estimate_depth(core::depth_image_t& depth, pixelcoord_t center, size_t hsize)
  {
    boost::timer timer;
    timer.restart();

    roi_2d_t roi (center, hsize);
    roi.clip_to(depth.height(), depth.width());

    using namespace boost::accumulators;
    
    typedef accumulator_set<point3df_t::value_type, stats<tag::p_square_cumulative_distribution> > 
      psqr_accumulator_t;

    accumulator_set <point3df_t::value_type, stats < 
                                tag::mean(immediate)
                                , tag::error_of<tag::immediate_mean> 
                                                    > > acc;

    accumulator_set<float, stats<tag::density> > 
      hist(tag::density::cache_size = 10,tag::density::num_bins = 50);

    psqr_accumulator_t 
      chist(tag::p_square_cumulative_distribution::num_cells = 50);

    core::depth_image_t::buffer_type data = 
      depth.get_buffer_sptr();

    point3df_t::value_type  dist;
    point3df_t              point;

    typedef boost::iterator_range<std::vector<std::pair<single_t, single_t> >::iterator >  
      histogram_type;

    for (size_t it_r = roi.minrow()  ;  it_r < roi.maxrow(); it_r++)
    {
      for (size_t it_c = roi.mincol(); it_c < roi.maxcol() ; it_c++)
      {
        //calc norm
        //weird per-channel pixel access....
        //prima vedo se la zeta è valida....
        point.assign( depth.get(it_r, it_c, 2),  2);

        if(point(2) > 0.1 && point(2) < 10.0)
        {
        point.assign( depth.get(it_r, it_c, 0), 0);
        point.assign( depth.get(it_r, it_c, 1), 1);

        dist = euclidean_distance(point);
        acc(dist);
        chist(dist);  
        hist(dist);
        }//if

      }//inner for
    }//outer for

    mystat outstat;
    outstat.mean     =   mean(acc);
    //outstat.stddev  =    extract_result<tag::error_of<tag::mean> >(acc);
    outstat.stddev  =    error_of<tag::immediate_mean>(acc);

    histogram_type 
      chistogram = p_square_cumulative_distribution(chist);

    histogram_type 
      histogram = density(hist); 

    double elapsed = timer.elapsed();

    //log istogramma cumulativo ...
    if (1)
    {
      printf("Elapsed:  %f\n", elapsed);
      printf("Mean:     %f\n", mean(acc));
      printf("StdDev    %f\n", error_of<tag::immediate_mean>(acc));

      for (std::size_t i = 0; i < histogram.size(); ++i)
      {   
      // problem with small results: epsilon is relative (in percent), not absolute!
      if ( histogram[i].second > 0.001 )  //non capito ...  
        printf("%d --> first: %f second: %f\n", i, histogram[i].first, histogram[i].second);
      }
    }

    return outstat;
  }
//---------------------------------------------------------------------------+

}}

//---------------------------------------------------------------------------+
#endif //depth_image_utils_HPP_INCLUDED
