#include "matlab_bee_inc.h"
#include "alcor\matlab\matlab_mx_utils.hpp"
//--------------------------------------------------------------------++
using namespace all;
//--------------------------------------------------------------------++
#define OBJ_HANDLE_ prhs[0]
#define RET_IMAGE_BUF_ plhs[0]
//--------------------------------------------------------------------++
void bee_cam_grab_color( int nlhs 
					,mxArray *plhs[]
					,int nrhs
					,const mxArray 
					*prhs[])
	{
    ///
    sense::bumblebee_driver_t& bee 
    = get_object<sense::bumblebee_driver_t>(OBJ_HANDLE_);

  all::core::uint8_sarr image_ptr 
    =	bee.get_color_buffer(core::right_img);

  ///
  RET_IMAGE_BUF_ = matlab::buffer2array<all::core::uint8_t>::create_from_planar(
            image_ptr.get()
	        ,matlab::row_major
	        ,bee.nrows()
	        ,bee.ncols());
	}
//--------------------------------------------------------------------++