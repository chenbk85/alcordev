//--------------------------------------------------------------------++
#include "mex.h"
//--------------------------------------------------------------------++
#include "alcor/matlab/opencv_matlab_utils.hpp"
//--------------------------------------------------------------------++
#include "cv.h"
#include <cmath>
//--------------------------------------------------------------------++
using namespace all;
//--------------------------------------------------------------------++
void cv_pyrdown4( int nlhs 
					,mxArray *plhs[]
					,int nrhs
					,const mxArray 
					*prhs[])
	{
    //next step .. switch through types 
    //now .. only UINT8 
      if(nrhs > 0)
      {
        //
        //if(mxGetClassID(prhs[0]) == mxDOUBLE_CLASS)
        //{
        typedef core::single_t value_type ;
          //
          IplImage* p_image   = matlab::mxcv::mxarray_to_iplimage<value_type>(prhs[0]);
          //
          if(p_image->nChannels > 1)
          {
            cvCvtColor(p_image,p_image,CV_RGB2GRAY);

          }

          //downsampled size
          int height = static_cast<int>(std::floor(static_cast<double>( (p_image->height/2.0) ) ));
          int width  = static_cast<int>(std::floor(static_cast<double>( (p_image->width/2.0)  ) ));


          IplImage* p_downsampled64= cvCreateImage(  cvSize(width,height),
            IPL_DEPTH_32F, 
                                                    1);

          cvPyrDown( p_image, p_downsampled64);

          if(nlhs > 0)
          {
            //
            plhs[0] = matlab::mxcv::iplimage_to_mxarray<value_type>(p_downsampled64);
          }

          core::cvutils::release_iplimage(p_downsampled64); 
       }
      
      //else
      //{
      //  printf("USE ONLY DOUBLE image\n");
      //}
  }

//--------------------------------------------------------------------++