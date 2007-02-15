#include "matlab_bee_inc.h"
#include "alcor/matlab/matlab_mx_utils.hpp"
#include <boost/shared_array.hpp>
//--------------------------------------------------------------------++
using namespace all;
//--------------------------------------------------------------------++
#define OBJ_HANDLE_ prhs[0]
//--------------------------------------------------------------------++
#define RGB_IMAGE_BUF_ plhs[0]
#define XYZ_IMAGE_BUF_ plhs[1]
//--------------------------------------------------------------------++
void bee_cam_grab_color_and_depth( int nlhs 
					,mxArray *plhs[]
					,int nrhs
					,const mxArray 
					*prhs[])
	{
	//printf("Opening ....\n")
	sense::bumblebee_ipc_recv_t& bee = 
		get_object<sense::bumblebee_ipc_recv_t>(OBJ_HANDLE_);

  //bee.lock();
  
  boost::shared_array<core::uint8_t> buf_ 
      =	bee.get_color_buffer(core::right_img);

  core::single_sarr xyz_sptr 
    = bee.get_depth_buffer();

  //bee.unlock();

  ///
  RGB_IMAGE_BUF_ =  matlab::buffer2array< core::uint8_t >::create_from_planar(
           buf_.get()
	        ,matlab::row_major
	        ,bee.height()
	        ,bee.width());

  XYZ_IMAGE_BUF_ = all::matlab::buffer2array<  core::single_t  >::create_from_planar(
           xyz_sptr.get()
	        ,matlab::row_major
	        ,bee.height()
	        ,bee.width());

	}
//--------------------------------------------------------------------++