#ifndef matlab_engine_t_HPP_INCLUDED
#define matlab_engine_t_HPP_INCLUDED
//--------------------------------------------------------------------------+
#pragma comment(lib, "matlab_engine_t.lib")
//--------------------------------------------------------------------------+
#include "alcor/matlab/matlab_mx_utils.hpp"
//--------------------------------------------------------------------------+
#include "engine.h"
#include <string>
//--------------------------------------------------------------------------+
using std::string;
//--------------------------------------------------------------------------+
namespace all{
	namespace matlab{
///Opens a Local Matlab Context
class matlab_engine_t  
{

public:	
	matlab_engine_t(bool single_mode = false);

public:
	///Execute matlab command.
	void command_line(const string&);
	///Execute matlab command.
	int command_line_blocking(const string&);
public:
	///Access to a named double variable from Workspace.
	double get_scalar_double(const char*);

public:
	///Writes a named variable into workspace
	void put_scalar(const char*, int);
	///Writes a named variable into workspace
	void put_scalar(const char*, float);
	///Writes a named variable into workspace
	void put_scalar(const char*, double);

	///Writes a named (matrix) variable into workspace
	void put_array(const char*, mxArray*);  

public:
	///Clear a Named Var From Workspace.
	void clear(char*);
	///Clears the whole system.
	void clear_all();
  void clear_workspace();

public:
	Engine* eng() const {return m_eng;} ;

private:
	Engine* m_eng;

};

//}
//--------------------------------------------------------------------------+
	}//namespace alcor	
}//namespace matlab
//--------------------------------------------------------------------------+
#endif //matlab_engine_t_HPP_INCLUDED