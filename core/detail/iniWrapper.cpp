///////////////////////////////////////////////
/// file name: iniWrapper.cpp
/// purpose : Robocup 2006 - Alcor Lab - La Sapienza Italy
/// author: Francesco Roccucci
/// created: 11/05/06
/// last modified: 11/05/06
///////////////////////////////////////////////
#include "../iniWrapper.h"
#include "wrappImpl.cpp"
/// constructors
//:errorFile_("errorFile.log",std::ios::out)

iniWrapper::iniWrapper(){
	wrImpl.reset(new wrappImpl());
};

/*! 
	iniWrapper try to parse the iniFile f in the dictionary.
	If the file doesn't exist the constructor create an empty file named like f
	\param f ini filename with extension
*/ 
iniWrapper::iniWrapper(const char* f){
	wrImpl.reset(new wrappImpl(f) );
};

/*! 
	iniWrapper try to parse the iniFile f in the dictionary.
	If the file doesn't exist the constructor create an empty file named like f
	\param f ini filename with extension
*/ 
iniWrapper::iniWrapper(const std::string& filepath){
  wrImpl.reset( new wrappImpl(filepath.c_str()) );
};
/*! 
	Load store an iniFile into the dictionary.
	\param f ini filname with extension
*/
bool iniWrapper::Load(const char* f){
	return wrImpl->Load(f);
};

/*! 
	Load store an iniFile into the dictionary.
	\param f ini filname with extension
*/
bool iniWrapper::Load(const std::string& filepath){
	return wrImpl->Load(filepath.c_str());
};

/*!
	Size returns how many entries are in the dictionary.
*/
int iniWrapper::Size(){
	return wrImpl->Size();
};

/*!
	Add insert a new value into the dictionary.
	If the entry exists, Add update the value of that entry.
	\param key name of the entry in the directory of the ini file. Syntax directory:entry
	\param value value to add or update in the ini file
*/

int iniWrapper::Add(char* key,char* value){
	return wrImpl->Add(key,value);
};

/*! 
	Remove cancel an entry in the dictionary.
	\param key name of the entry in the directory of the ini file. Syntax directory:entry
*/

void iniWrapper::Remove(char* key){
	wrImpl->Remove(key);
}

/*!
	GetString return the string value in the specific entry of the dictionary.
	\param key name of the entry in the directory of the ini file. Syntax directory:entry
*/
//std::string iniWrapper::GetString(char* key){
//	return wrImpl->GetString(key);
//};

/*!
	GetString return the string value in the specific entry of the dictionary.
	\param key name of the entry in the directory of the ini file. Syntax directory:entry
	\param not_def value that function returns if there isn't the specified entry
*/
char* iniWrapper::GetStringAsChar(char* key, char* not_def){
	return wrImpl->GetStringAsChar(key,not_def);
};

/*!
	GetString return the integer value in the specific entry of the dictionary.
	\param key name of the entry in the directory of the ini file. Syntax directory:entry
	\param not_def value that function returns if there isn't the specified entry
*/

int iniWrapper::GetInt(char* key, int not_def){
	return wrImpl->GetInt(key,not_def);

}

/*!
	GetString return the double value in the specific entry of the dictionary.
	\param key name of the entry in the directory of the ini file. Syntax directory:entry
	\param not_def value that function returns if there isn't the specified entry

*/

double iniWrapper::GetDouble(char* key,double not_def){
	return wrImpl->GetDouble(key,not_def);
}

/*!
	GetString return the integer value in the specific entry of the dictionary
	with a boolean mean.
	\param key name of the entry in the directory of the ini file. Syntax directory:entry
	\param not_def value that function returns if there isn't the specified entry
*/

int iniWrapper::GetBool(char* key,int not_def){
	return wrImpl->GetBool(key,not_def);
}

/*!
	Save store the dictionary into the ini file specified.
	\param f
*/
void iniWrapper::Save(char* f){
	wrImpl->Save(f);
}