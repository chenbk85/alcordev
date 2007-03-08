#include "matlab_bee_inc.h"

#define OBJ_HANDLE_ plhs[0]

void bee_cam_create( int nlhs 
					,mxArray *plhs[]
					,int nrhs
					,const mxArray 
					*prhs[])
	{
	if (nlhs != 1 || nrhs != 0)
		mexErrMsgTxt("Usage: h = bee_cam_create");

  OBJ_HANDLE_  = create_handle(new all::sense::bumblebee_driver_t);
	}