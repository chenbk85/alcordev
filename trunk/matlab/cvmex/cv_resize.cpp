//--------------------------------------------------------------------++
#include "mex.h"
//--------------------------------------------------------------------++
#include "alcor/matlab/opencv_matlab_utils.hpp"
//--------------------------------------------------------------------++
#include "cv.h"
#include <cmath>
//--------------------------------------------------------------------++
#define _HEIGHT_FIELD_  prhs[1]
#define _WIDTH_FIELD_   prhs[2]
//--------------------------------------------------------------------++
#define _OUT_MXARRAY_  plhs[0]
//--------------------------------------------------------------------++
using namespace all;
//--------------------------------------------------------------------++
void cv_resize( int nlhs 
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
        if(mxGetClassID(prhs[0]) == mxSINGLE_CLASS)
        {
          //
          IplImage* p_image   = matlab::mxcv::mxarray_to_iplimage<core::single_t>(prhs[0]);

          //
          //if(p_image->nChannels > 1)
          //{
          //  cvCvtColor(p_image,p_image,CV_RGB2GRAY);

          //}

          //
          int height = 0;
          int width  = 0;

          if (nrhs == 3)
          {
          //downsampled size
          height  = static_cast<int>(mxGetScalar(_HEIGHT_FIELD_) );
          width   = static_cast<int>(mxGetScalar(_WIDTH_FIELD_) );
          }
          else
          {
           height = static_cast<int>(std::floor(static_cast<double>( (p_image->height/2.0) ) ));
           width  = static_cast<int>(std::floor(static_cast<double>( (p_image->width/2.0)  ) ));
          }

          //
          IplImage* p_resized= cvCreateImage(  cvSize(width,height),
                                                    IPL_DEPTH_32F, 
                                                    1);
          //
          cvResize( p_image, p_resized);
          //
          if(nlhs > 0)
          {
            //
            _OUT_MXARRAY_ = matlab::mxcv::iplimage_to_mxarray<core::single_t>(p_resized);
          }
          core::cvutils::release_iplimage(p_image);
          core::cvutils::release_iplimage(p_resized); 
       }
      }
      else
      {
        printf("USE ONLY DOUBLE image\n");
      }
  }

//----------------------------------------I----------------------------++