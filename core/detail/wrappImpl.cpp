///////////////////////////////////////////////
/// file name: wrappImpl.h
/// purpose : Robocup 2006 - Alcor Lab - La Sapienza Italy
/// author: Francesco Roccucci
/// created: 11/05/06
/// last modified: 11/05/06
///////////////////////////////////////////////
#include <fstream>
//---------------------------------------------------------------------------
#include <iostream>
//---------------------------------------------------------------------------
#include "iniparser.h"
//---------------------------------------------------------------------------
class wrappImpl{
public:
	wrappImpl();
	wrappImpl(const char* f);

  ///Andrea:: d'tor anyone?
  ~wrappImpl();

	int create(const char* f);
	bool Load(const char* f); ///< Load an ini file in the dictionary
	int Size(); ///< returns how many entries there are in the dictionary
	int Add(char* key,char* value); ///< set the value for the key in the directory specified
	void Remove(char* key); ///< delete the value for the specific directory and key
	std::string GetString(char* key);
	///ANDREA:: Overloaded method to handle char*
	char* GetStringAsChar(char* key, char* not_def);

	int GetInt(char* key, int not_def);
	double GetDouble(char* key, double not_def);
	int GetBool(char* key, int not_def);
	void Save(char* f); ///< save the dictionary in the ini file specified

private:
	dictionary* dict;
};

/////////////////////////////////////////////////////////////////////////////
//IMPLEMENTATION
/////////////////////////////////////////////////////////////////////////////

inline wrappImpl::wrappImpl():dict(0)
{
};

/*! 
	DTOR
*/ 
inline wrappImpl::~wrappImpl()
{
  if (dict)
    iniparser_free(dict);
}

/*! 
	iniWrapper try to parse the iniFile f in the dictionary.
	If the file doesn't exist the constructor create an empty file named like f
	\param f ini filename with extension
*/ 
inline wrappImpl::wrappImpl(const char* f){
	FILE* file_;
	if (!Load(f)){
		file_ = fopen(f,"w");
		fclose(file_);
		dict = iniparser_new(f);
	}
};

/*! 
	Load store an iniFile into the dictionary.
	\param f ini filname with extension
*/
inline bool wrappImpl::Load(const char* f){
	dict = iniparser_new(f);
	return (dict != NULL);
};

/*!
	Size returns how many entries are in the dictionary.
*/
inline int wrappImpl::Size(){
	return dict->n;
};

/*!
	Add insert a new value into the dictionary.
	If the entry exists, Add update the value of that entry.
	\param key name of the entry in the directory of the ini file. Syntax directory:entry
	\param value value to add or update in the ini file
*/

inline int wrappImpl::Add(char* key,char* value){
	return iniparser_setstr(dict,key,value);
};

/*! 
	Remove cancel an entry in the dictionary.
	\param key name of the entry in the directory of the ini file. Syntax directory:entry
*/

inline void wrappImpl::Remove(char* key){
	iniparser_unset(dict,key);
}

/*!
	GetString return the string value in the specific entry of the dictionary.
	\param key name of the entry in the directory of the ini file. Syntax directory:entry
*/
inline std::string wrappImpl::GetString(char* key){
	return iniparser_getstr(dict,key);
};

/*!
	GetString return the string value in the specific entry of the dictionary.
	\param key name of the entry in the directory of the ini file. Syntax directory:entry
	\param not_def value that function returns if there isn't the specified entry
*/
inline char* wrappImpl::GetStringAsChar(char* key, char* not_def){
	return iniparser_getstring(dict,key,not_def);
};

/*!
	GetString return the integer value in the specific entry of the dictionary.
	\param key name of the entry in the directory of the ini file. Syntax directory:entry
	\param not_def value that function returns if there isn't the specified entry
*/

inline int wrappImpl::GetInt(char* key, int not_def){
	return iniparser_getint(dict,key,not_def);

}

/*!
	GetString return the double value in the specific entry of the dictionary.
	\param key name of the entry in the directory of the ini file. Syntax directory:entry
	\param not_def value that function returns if there isn't the specified entry

*/

inline double wrappImpl::GetDouble(char* key,double not_def){
	return iniparser_getdouble(dict,key,not_def);
}

/*!
	GetString return the integer value in the specific entry of the dictionary
	with a boolean mean.
	\param key name of the entry in the directory of the ini file. Syntax directory:entry
	\param not_def value that function returns if there isn't the specified entry
*/

inline int wrappImpl::GetBool(char* key,int not_def){
	return iniparser_getboolean(dict,key,not_def);
}

/*!
	Save store the dictionary into the ini file specified.
	\param f
*/
inline void wrappImpl::Save(char* f){
	FILE* file_;
	file_ = fopen(f,"w");
	iniparser_dump_ini(dict,file_);
	fclose(file_);
}