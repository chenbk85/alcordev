#include "matlab_bee_inc.h"
//--------------------------------------------------------------------++
#include <boost/function.hpp>
#include <map>
#include <boost/assign/list_inserter.hpp>
////--------------------------------------------------------------------++
////External Functions (they do all the serious work)
extern void bee_cam_create( int nlhs, mxArray *plhs[], int nrhs, const mxArray 
 *prhs[]); 
////--------------------------------------------------------------------++
extern void bee_cam_open( int nlhs, mxArray *plhs[], int nrhs, const mxArray 
 *prhs[]); 
////--------------------------------------------------------------------++
extern void bee_cam_close( int nlhs, mxArray *plhs[], int nrhs, const mxArray 
 *prhs[]); 
//--------------------------------------------------------------------++
extern void bee_cam_grab_color( int nlhs, mxArray *plhs[], int nrhs, const mxArray 
 *prhs[]); 
//--------------------------------------------------------------------++
extern void bee_cam_grab_color_and_depth( int nlhs, mxArray *plhs[], int nrhs, 
								  const mxArray *prhs[]); 
//--------------------------------------------------------------------++
extern void bee_grab_all( int nlhs, mxArray *plhs[], int nrhs, 
								  const mxArray *prhs[]); 
//--------------------------------------------------------------------++
typedef boost::function<void (int nlhs, 
                        mxArray* plhs[], 
                        int nrhs, 
                        const mxArray* prhs[])> func_t;

typedef std::map<int,func_t> function_table_t;

static		function_table_t*	function_table	= NULL;
//--------------------------------------------------------------------++
static	int	myStaticDataInitialized	= 0;
static  std::size_t myFuncTableSize	= 0;
//--------------------------------------------------------------------++
void exitFcn() 
{
	if(myStaticDataInitialized)
	{
		printf("exitFun Invoked ...Cleaning\n");
		if(function_table !=  NULL)
		{
			delete function_table;
			function_table = NULL;
		}	
		myStaticDataInitialized	= 0;
		myFuncTableSize			= 0;
	}

}

//--------------------------------------------------------------------++
static void init_function_table()
{
	if(myStaticDataInitialized == 0)
	{
		//
		function_table = new function_table_t;

		//self.CREATE         = 0;
		//function_table->push_back(&bee_cam_create);
    boost::assign::insert(*function_table) 
      (0, &bee_cam_create)//self.CREATE = 0;
      (1, &bee_cam_open)//self.OPEN =1
      (2, &bee_cam_close)//self.CLOSE =2
      (3, &bee_cam_grab_color)//self.GRABCOLOR =3
      (4, &bee_cam_grab_color_and_depth)//self.GRABRGBXYZ =4
      (5, &bee_grab_all);//self.GRABALL =5
       
		////self.OPEN           = 1;
		//function_table->push_back(&bee_cam_open);
		////self.CLOSE           = 2;
		//function_table->push_back(&bee_cam_close);
		////self.GRABCOLOR       = 3;
		//function_table->push_back(&bee_cam_grab_color);
		////self.GRABRGBXYZ       = 4;
		//function_table->push_back(&bee_cam_grab_color_and_depth);
		////self.GRABALL       = 5;
		//function_table->push_back(&bee_grab_all);

		myStaticDataInitialized = 1;
		myFuncTableSize = static_cast<int> (function_table->size());

		mexMakeMemoryPersistent(&myStaticDataInitialized);
		mexMakeMemoryPersistent(&myFuncTableSize);
		mexMakeMemoryPersistent(function_table);

		mexAtExit(exitFcn);
	}
};

//--------------------------------------------------------------------++
/// The "real" mex function.
/// Basically 'routes' the control to a specific external routine.
//--------------------------------------------------------------------++
#define COMMAND_IN prhs[0]
//--------------------------------------------------------------------++
void mexFunction( int nlhs, mxArray* plhs[], int nrhs, const mxArray
*prhs[] )
{
	if(nrhs >0)
	{
		if(myStaticDataInitialized==0)
		{
			printf("Initializing  mex file\n");
			init_function_table();
		}
		
		int cmd = static_cast<int>(mxGetScalar(COMMAND_IN));
    
		if( cmd >= 0 && cmd < myFuncTableSize )
			{
				//Ok we are in the range...
				//func_t func = function_table->at(cmd);
        func_t func = function_table->operator [](cmd);
				//Check ... you'll never know...
				if(func)
					// N.B. the -1 and +1
					//That's why we have already read 
					//from the std arguments arrays
					func(nlhs, plhs, nrhs-1, prhs+1);
			}
		else
	     	mexErrMsgTxt("Something is bad .. null function reference.\n");		
    }
    else
    {
    	printf("Specify Command ID please.\n");
    }
}
//--------------------------------------------------------------------++