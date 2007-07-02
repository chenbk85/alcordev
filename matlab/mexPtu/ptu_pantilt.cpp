#include "matlab_ptu_inc.h"

#define OBJ_HANDLE_ prhs[0]

#define PAN_VALUE_  prhs[1]
#define TILT_VALUE_ prhs[2]

void ptu_pantilt_( int nlhs 
					,mxArray *plhs[]
					,int nrhs
					,const mxArray 
					*prhs[])
          {
          all::act::directed_perception_ptu_t& myptu = 
            get_object<all::act::directed_perception_ptu_t>(OBJ_HANDLE_); 

          double panangle   = mxGetScalar(PAN_VALUE_);
          double tiltangle  = mxGetScalar(TILT_VALUE_);
          printf("Setting Pan %f Tilt %f\n", panangle, tiltangle);
          myptu.set_pantilt(panangle, tiltangle);

          }