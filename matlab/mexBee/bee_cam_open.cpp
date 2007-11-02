#include "matlab_bee_inc.h"

#define OBJ_HANDLE_ prhs[0]
#define CAMERA_ID_  prhs[1]


void bee_cam_open( int nlhs 
					,mxArray *plhs[]
					,int nrhs
					,const mxArray 
					*prhs[])
	{


 // char *input_buf;
 // int   buflen;

 //  //Input must be a string. 
 // if (mxIsChar(CAMERA_ID_) != 1)
 //   mexErrMsgTxt("Input must be a string.");
 // //Input must be a row vector.
 // if (mxGetM(CAMERA_ID_) != 1)
 //   mexErrMsgTxt("Input must be a row vector.");

 // //Get the length of the input string. 
 // buflen = (int)(mxGetM(CAMERA_ID_) * mxGetN(CAMERA_ID_)) + 1;

	all::sense::bumblebee_driver_t& mybee = 
    get_object<all::sense::bumblebee_driver_t>(OBJ_HANDLE_);

 //  //Allocate memory for input and output strings. 
 // input_buf = (char*) mxCalloc(buflen, sizeof(char));
 // mxGetString(CAMERA_ID_, input_buf, buflen);

  std::string ininame = "config/bumblebeeA.ini";
  //ininame += input_buf;

  std::printf("Opening %s\n", ininame.c_str());
  std::printf("Opening Bumblebee \n");

  if(mybee.open(ininame))    
  {
    printf("OK\n");
    printf("Bumblebee  succesfully opened!\n");
    printf("Rows: %d Cols: %d\n", mybee.nrows(), mybee.ncols());
  }
  else
  {
	  printf("Bumblebee  impossible to open!\n");
  }

	}