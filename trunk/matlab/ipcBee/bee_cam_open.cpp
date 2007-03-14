#include "matlab_bee_inc.h"

#define OBJ_HANDLE_ prhs[0]

void bee_cam_open( int nlhs 
					,mxArray *plhs[]
					,int nrhs
					,const mxArray 
					*prhs[])
	{
	printf("Opening ....\n");
	all::sense::bumblebee_ipc_recv_t& mybee = 
	  get_object<all::sense::bumblebee_ipc_recv_t>(OBJ_HANDLE_);

  std::printf("Opening\n");

  if(mybee.open("config/bumblebeeB.ini"))    
  {
    printf("OK\n");
    printf("Bumblebee RGB IPC stream succesfully opened!\n");
    printf("Rows: %d Cols: %d\n", mybee.height(), mybee.width());
  }
  else
  {
    std::printf("not OK\n");
	  printf("Bumblebee RGB IPC stream impossible to open!\n");
  }

	}