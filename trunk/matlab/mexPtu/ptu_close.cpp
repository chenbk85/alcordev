#include "matlab_ptu_inc.h"

#define OBJ_HANDLE_ prhs[0]

void ptu_close_( int nlhs 
					,mxArray *plhs[]
					,int nrhs
					,const mxArray 
					*prhs[])
	{
	if(nrhs == 1)
	{


    destroy_object<all::act::directed_perception_ptu_t>(OBJ_HANDLE_);
	  printf("Closing Ptu !\n");
	}
	else	
	{
		mexErrMsgTxt("Missing Object Reference\n");
	}	
	}