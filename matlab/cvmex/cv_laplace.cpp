//--------------------------------------------------------------------++
#include "mex.h"
//--------------------------------------------------------------------++
#include "alcor/matlab/opencv_matlab_utils.hpp"
//--------------------------------------------------------------------++
#include "cv.h"
//--------------------------------------------------------------------++
using namespace all;
//--------------------------------------------------------------------++
void cv_laplace( int nlhs 
					,mxArray *plhs[]
					,int nrhs
					,const mxArray 
					*prhs[])
	{
    //next step .. switch through types 
    //now .. only UINT8 
      if(nrhs > 0)
      {
          IplImage* p_image   = matlab::mxcv::mxarray_to_iplimage<core::uint8_t>(prhs[0]);

          IplImage* p_laplace_img16 = cvCreateImage(cvSize(p_image->width,p_image->height),IPL_DEPTH_16S,1);

          cvLaplace( p_image, p_laplace_img16, 5);

          if(nlhs > 0)
          {
            //
            plhs[0] = matlab::mxcv::iplimage_to_mxarray<core::int16_t>(p_laplace_img16);
          }

          core::cvutils::release_iplimage(p_laplace_img16); 
       }
  }

//--------------------------------------------------------------------++