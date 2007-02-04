///////////////////////////////////////////////
/// file name: wrappImpl.h
/// purpose : Robocup 2006 - Alcor Lab - La Sapienza Italy
/// author: Francesco Roccucci
/// created: 11/05/06
/// last modified: 11/05/06
///////////////////////////////////////////////
#ifndef wrappImpl_H
#define wrappImpl_H
//---------------------------------------------------------------------------
#include <iostream>
//---------------------------------------------------------------------------
#include "iniparser.h"
//---------------------------------------------------------------------------
class wrappImpl{
public:
	wrappImpl();
	wrappImpl(char* f);
	int create(char* f);
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
	dictionary *dict;
};
//---------------------------------------------------------------------------
#endif
