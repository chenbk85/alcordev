#include "matlab_engine_t.h"
#include <cstdio>
//---------------------------------------------------------------------------
namespace all{
	namespace matlab{
//---------------------------------------------------------------------------
matlab_engine_t::matlab_engine_t(bool single_mode)
{

	if(!single_mode)
	{
		if (!(m_eng = engOpen("\0"))) 
		{
            std::printf( "Can't start MATLAB engine\n" );
		}
		else
		{
			std::printf( "MATLAB engine has started ....\n" );
      engOutputBuffer(m_eng, NULL, 0);
			command_line("cd 'C:\\Documents and Settings\\ALCORLAB\\Documenti\\matlabworks'");
		}
	}
	else
	{
		int retstatus;
		//SingleUser
		if (!(m_eng = engOpenSingleUse(NULL,NULL, &retstatus) ) ) 
		{
			std::printf( "Can't start MATLAB engine in single user\n" );
		}
		else
		{
			std::printf( "MATLAB engine has started ....\n" );
			command_line("cd 'C:\\Documents and Settings\\ALCORLAB\\Documenti\\matlabworks\\attention_machine'");
		}
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void matlab_engine_t::command_line(const string& arg)
{
	//std::printf("Command: %s\n",arg.c_str() );
	engEvalString(m_eng, arg.c_str());
}
//---------------------------------------------------------------------------
int matlab_engine_t::command_line_blocking(const string& arg)
{
	//std::printf("Command: %s\n",arg.c_str() );
	return engEvalString(m_eng, arg.c_str());
}
////---------------------------------------------------------------------------
///\param name The name of the variable.
///\param value The value we want to assign.
void matlab_engine_t::put_scalar(const char* name, int value)
{
	const mwSize dims [] = {1,1};
  //mwSize dims;
  //dims.
	mxArray* temp = mxCreateNumericArray(
			2
			,dims
			,mxINT32_CLASS
			,mxREAL);
	int* int_ptr = static_cast<int*>(mxGetData(temp));
	*int_ptr = value;
	engPutVariable(m_eng, name, temp);
}
////---------------------------------------------------------------------------
///\param name The name of the variable.
///\param value The value we want to assign.
void matlab_engine_t::put_scalar(const char* name, float value)
{
	const mwSize dims [] = {1,1};
	mxArray* temp = mxCreateNumericArray(
			2
			,dims
			,mxSINGLE_CLASS
			,mxREAL);
	float* float_ptr = static_cast<float*>(mxGetData(temp));
	*float_ptr = value;
	engPutVariable(m_eng, name, temp);
    mxDestroyArray(temp);
}
////---------------------------------------------------------------------------
///\param name The name of the variable.
///\param value The value we want to assign.
void matlab_engine_t::put_scalar(const char* name, double value)
{
	//MESSAGE("Writing Double in WS");
	mxArray* temp = mxCreateDoubleScalar(value);
	engPutVariable(m_eng, name, temp);
  mxDestroyArray(temp);
}
//---------------------------------------------------------------------------
void matlab_engine_t::put_array(const char* name, mxArray* arr)
{
	//MESSAGE("Writing Double Array in WS");
	engPutVariable(m_eng, name, arr);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
double matlab_engine_t::get_scalar_double(const char* var)
{
	double val = 0;

	mxArray* mval = engGetVariable(m_eng, var);

	if(mval != NULL)
	{
		memcpy((void*)&val, (void*)mxGetPr(mval), sizeof(val));
		mxDestroyArray(mval);

	}
	else
	{
		//char* res;
		//sprintf(res,"Var %s does not exist in workspace!\n", var);
		printf("Var %s does not exist in workspace!\n", var);
		//throw alcor::err::WorkspaceError();
	}

	return val;
}
//---------------------------------------------------------------------------
int matlab_engine_t::get_scalar_int(const char* var)
{
	double val = 0;

	mxArray* mval = engGetVariable(m_eng, var);

	if(mval != NULL)
	{
		memcpy((void*)&val, (void*)mxGetPr(mval), sizeof(val));
		mxDestroyArray(mval);

	}
	else
	{
		//char* res;
		//sprintf(res,"Var %s does not exist in workspace!\n", var);
		printf("Var %s does not exist in workspace!\n", var);
		//throw alcor::err::WorkspaceError();
	}

	return static_cast<int>(val);
}
//---------------------------------------------------------------------------
void matlab_engine_t::clear(char* varName)
{
	string cmd = "clear";
	cmd += varName;
	engEvalString(m_eng, cmd.c_str());
}
//---------------------------------------------------------------------------
void matlab_engine_t::clear_all()
{
	engEvalString(m_eng, "clear all");
}
//---------------------------------------------------------------------------
void matlab_engine_t::clear_workspace()
{
	engEvalString(m_eng, "clear");
}
//---------------------------------------------------------------------------
	}
}
//---------------------------------------------------------------------------
//}