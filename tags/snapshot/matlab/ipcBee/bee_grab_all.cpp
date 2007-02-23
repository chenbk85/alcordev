#include "matlab_bee_inc.h"
#include "alcor/matlab/matlab_mx_utils.hpp"
#include <boost/shared_array.hpp>
//--------------------------------------------------------------------++
using namespace all;
//--------------------------------------------------------------------++
#define OBJ_HANDLE_ prhs[0]
//--------------------------------------------------------------------++
#define L_RGB_IMAGE_BUF_  plhs[0]
#define R_RGB_IMAGE_BUF_  plhs[1]
#define XYZ_IMAGE_BUF_    plhs[2]
//--------------------------------------------------------------------++
void bee_grab_all( int nlhs 
					,mxArray *plhs[]
					,int nrhs
					,const mxArray 
					*prhs[])
	{
	sense::bumblebee_ipc_recv_t& bee = 
		get_object<sense::bumblebee_ipc_recv_t>(OBJ_HANDLE_);

  bee.lock();
  //printf("IN ACQ\n");

 //printf("LEFT\n");
  boost::shared_array<core::uint8_t> lbuf_ 
    =	bee.get_color_buffer(core::left_img);

 //printf("RIGHT\n");
  boost::shared_array<core::uint8_t> rbuf_ 
      =	bee.get_color_buffer(core::right_img);

 //printf("ZED\n");
 boost::shared_array<core::single_t> xyz_sptr 
    = bee.get_depth_buffer();

  //printf("OUT ACQ\n");
  bee.unlock();

  ///
  L_RGB_IMAGE_BUF_ =  matlab::buffer2array< core::uint8_t >::create_from_planar(
           lbuf_.get()
	        ,matlab::row_major
	        ,bee.height()
	        ,bee.width());
  ///
  R_RGB_IMAGE_BUF_ =  matlab::buffer2array< core::uint8_t >::create_from_planar(
           rbuf_.get()
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