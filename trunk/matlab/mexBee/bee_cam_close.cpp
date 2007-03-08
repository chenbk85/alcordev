#include "matlab_bee_inc.h"

#define OBJ_HANDLE_ prhs[0]

void bee_cam_close( int nlhs 
					,mxArray *plhs[]
					,int nrhs
					,const mxArray 
					*prhs[])
	{
	if(nrhs == 1)
	{
    destroy_object<all::sense::bumblebee_driver_t>(OBJ_HANDLE_);
	printf("Closing Bumblebee  stream!\n");
	}
	else	
	{
		mexErrMsgTxt("Missing Object Reference\n");
	}	
	}