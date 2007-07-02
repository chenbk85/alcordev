#include "matlab_ptu_inc.h"

#define OBJ_HANDLE_ plhs[0]

void ptu_create_( int nlhs 
					,mxArray *plhs[]
					,int nrhs
					,const mxArray 
					*prhs[])
	{

  printf("debug: ptu_create\n");
  OBJ_HANDLE_  = create_handle(new all::act::directed_perception_ptu_t);
	}