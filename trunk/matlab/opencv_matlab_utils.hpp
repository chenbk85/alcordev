#ifndef opencv_matlab_utils_HPP_INCLUDED
#define opencv_matlab_utils_HPP_INCLUDED
//-----------------------------------------------------------------------++
#include "matrix.h" //for matlab mxArray
#include "cxcore.h"
//-----------------------------------------------------------------------++
//#include "alcor/matlab/detail/matlab_tags.hpp"
#include "alcor/matlab/matlab_mx_utils.hpp"
//-----------------------------------------------------------------------++
#include "boost/shared_ptr.hpp"
//-----------------------------------------------------------------------++
namespace all { namespace matlab {
//-----------------------------------------------------------------------++
//void release_iplimage(IplImage* p_image)
//{
//  cvReleaseImage(&p_image);
//}
//-----------------------------------------------------------------------++
struct mxcv {
//=============================================================================
static void release_iplimage(IplImage* p_image)
{
  cvReleaseImage(&p_image);
}
//=============================================================================
//
typedef boost::shared_ptr<IplImage> iplimage_ptr;
//=============================================================================
template <typename T>
static IplImage* mxarray_to_iplimage(const mxArray* p_array)
{  
  //wants (width, height)
  CvSize size;//(ncols,nrows);
  //
  int depth = IPL_DEPTH_8U;

  //assign correct type id
  switch (matlab::traits<T>::tag)
  {
  case mxUINT8_CLASS:
    depth = IPL_DEPTH_8U;
    break;


  case mxSINGLE_CLASS:
    depth = IPL_DEPTH_32F;
    break;

  case mxINT16_CLASS:
    depth = IPL_DEPTH_16S;
    break;

  case mxUINT16_CLASS:
    depth =  IPL_DEPTH_16U;
    break;

  case mxDOUBLE_CLASS:
    depth = IPL_DEPTH_64F;
    break;

  default:
    depth = IPL_DEPTH_8U;
    break;
  }
  
  //
  const mwSize ndims = mxGetNumberOfDimensions(p_array);
  //
  const mwSize* dims = mxGetDimensions(p_array);
  //
  int channels = 0;

  if(ndims == 3)
  //channels
    channels=static_cast<int>(dims[2]);
  else
    channels = 1 ;

  //
  size.height = static_cast<int>(dims[0]);
  size.width  = static_cast<int>(dims[1]);

  //printf("Height: %d Width: %d Channels: %d\n",size.height , size.width , channels);

  //
  //boost::shared_ptr<IplImage> p_iplimage(cvCreateImage(size,depth,channels), mxcv::release_iplimage );

  IplImage* p_iplimage = cvCreateImage(size,depth,channels);

  //get pointer to array/image data
  matlab::traits<T>::ptr p_data = static_cast<matlab::traits<T>::ptr >(mxGetData(p_array));

  //here cycle to convert ...
  const size_t n_pixel = size.height*size.width;
  //
  size_t start_pixel_index = 0;
  //
  const size_t channel_offset = n_pixel;
  //

  for (int row_iter = 0; row_iter < size.height; row_iter++)
  {
    for (int col_iter = 0; col_iter < size.width; col_iter++)
    {
      for (int channel_index = 0; channel_index < channels; channel_index++)
      {
        //
        size_t bgr_channel_index = (channels - channel_index - 1);

       
        (CV_IMAGE_ELEM( p_iplimage, T, row_iter, (col_iter*channels) + bgr_channel_index ) )= 
          static_cast<T>(p_data[(col_iter*size.height)+ row_iter+(channel_index*channel_offset)]);

      }
    }
  }

  //done
  return p_iplimage;
}

//=============================================================================
template < typename T >
static mxArray* iplimage_to_mxarray(IplImage* p_iplimage)
{
  ////
  //int ipl_depth = p_iplimage->depth;

  //mxClassID class_id;

  ////assign correct type id
  //switch (ipl_depth)
  //{
  //case IPL_DEPTH_8U:
  //  class_id = mxUINT8_CLASS;
  //  break;

  //case IPL_DEPTH_16S:
  //  class_id = mxINT16_CLASS;
  //  break;

  //case IPL_DEPTH_16U:
  //  class_id = mxUINT16_CLASS;
  //  break;

  //case IPL_DEPTH_32F:
  //  class_id = mxSINGLE_CLASS;
  //  break;

  //case IPL_DEPTH_64F:
  //  class_id = mxDOUBLE_CLASS;
  //  break;

  //default:
  //  class_id = mxUINT8_CLASS;
  //  break;
  //}

  //
  //mwSize 
  //azz.... la avevo già fatta
  mxArray* p_array = all::matlab::buffer2array<T>::create_from_interleaved(reinterpret_cast<matlab::traits<T>::const_ptr>(p_iplimage->imageData), 
    all::matlab::row_major, p_iplimage->height, p_iplimage->width, p_iplimage->nChannels);

  return p_array;
}
//----------------------------------------------------------------------++
};
//-----------------------------------------------------------------------++
}}
//-----------------------------------------------------------------------++
#endif //opencv_matlab_utils_HPP_INCLUDED