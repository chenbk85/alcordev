/////////////////////////////////////	//////////
/// file name: iniWrapper.h
/// purpose : Robocup 2006 - Alcor Lab - La Sapienza Italy
/// author: Francesco Roccucci
/// created: 11/05/06
/// last modified: 11/05/06
///////////////////////////////////////////////
#ifndef INIWRAPPER_H
#define INIWRAPPER_H
//---------------------------------------------------------------------------
#pragma comment (lib,"iniwrapper.lib")
//---------------------------------------------------------------------------
#include <string>
#include <memory>
//#include <boost/scoped_ptr.hpp>
//---------------------------------------------------------------------------
class	wrappImpl; ///< fwd
//---------------------------------------------------------------------------
class	iniWrapper{
public:
	iniWrapper();
	iniWrapper(const char* f);
  iniWrapper(const std::string& filepath );

	int		create(const char* f);

	bool	Load(const char* f); ///< Load an ini file in the dictionary
  bool  Load(const std::string& filepath);//overload

	int		Size(); ///< returns how many entries there are in the dictionary
	int		Add(char* key,char* value); ///< set the value for the key in the directory specified
	void	Remove(char* key); ///< delete the value for the specific directory and key

	std::string		GetString(char* key);

	///Andrea::Overloaded Method to return a char*
	char*	GetStringAsChar(char* key, char* not_def);

	int		  GetInt(char* key, int not_def);
	double	GetDouble(char* key, double not_def);
	int		  GetBool(char* key, int not_def);

	void	  Save(char* f); ///< save the dictionary in the ini file specified

private: 
  std::auto_ptr<wrappImpl> wrImpl;
};
//---------------------------------------------------------------------------
#endif