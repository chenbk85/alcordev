#include "matlab_ptu_inc.h"

#define OBJ_HANDLE_   prhs[0]
#define PTU_COM_NUM_  prhs[1]

#define PTU_CLEAN_OPEN plhs[0]

void ptu_open_( int nlhs 
					,mxArray *plhs[]
					,int nrhs
					,const mxArray 
					*prhs[])
          {
            all::act::directed_perception_ptu_t& myptu = 
              get_object<all::act::directed_perception_ptu_t>(OBJ_HANDLE_); 

            if (myptu.open("config/dpptu_conf.ini"))
            {

            }
            else
            {
            }

          }