///////////////////////////////////////////////
/// file name: wrappImpl.h
/// purpose : Robocup 2006 - Alcor Lab - La Sapienza Italy
/// author: Francesco Roccucci
/// created: 11/05/06
/// last modified: 11/05/06
///////////////////////////////////////////////

#include "wrappImpl.h"
#include <fstream>

wrappImpl::wrappImpl(){

};
/*! 
	iniWrapper try to parse the iniFile f in the dictionary.
	If the file doesn't exist the constructor create an empty file named like f
	\param f ini filename with extension
*/ 
wrappImpl::wrappImpl(const char* f){
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
bool wrappImpl::Load(const char* f){
	dict = iniparser_new(f);
	return (dict != NULL);
};

/*!
	Size returns how many entries are in the dictionary.
*/
int wrappImpl::Size(){
	return dict->n;
};

/*!
	Add insert a new value into the dictionary.
	If the entry exists, Add update the value of that entry.
	\param key name of the entry in the directory of the ini file. Syntax directory:entry
	\param value value to add or update in the ini file
*/

int wrappImpl::Add(char* key,char* value){
	return iniparser_setstr(dict,key,value);
};

/*! 
	Remove cancel an entry in the dictionary.
	\param key name of the entry in the directory of the ini file. Syntax directory:entry
*/

void wrappImpl::Remove(char* key){
	iniparser_unset(dict,key);
}

/*!
	GetString return the string value in the specific entry of the dictionary.
	\param key name of the entry in the directory of the ini file. Syntax directory:entry
*/
std::string wrappImpl::GetString(char* key){
	return iniparser_getstr(dict,key);
};

/*!
	GetString return the string value in the specific entry of the dictionary.
	\param key name of the entry in the directory of the ini file. Syntax directory:entry
	\param not_def value that function returns if there isn't the specified entry
*/
char* wrappImpl::GetStringAsChar(char* key, char* not_def){
	return iniparser_getstring(dict,key,not_def);
};

/*!
	GetString return the integer value in the specific entry of the dictionary.
	\param key name of the entry in the directory of the ini file. Syntax directory:entry
	\param not_def value that function returns if there isn't the specified entry
*/

int wrappImpl::GetInt(char* key, int not_def){
	return iniparser_getint(dict,key,not_def);

}

/*!
	GetString return the double value in the specific entry of the dictionary.
	\param key name of the entry in the directory of the ini file. Syntax directory:entry
	\param not_def value that function returns if there isn't the specified entry

*/

double wrappImpl::GetDouble(char* key,double not_def){
	return iniparser_getdouble(dict,key,not_def);
}

/*!
	GetString return the integer value in the specific entry of the dictionary
	with a boolean mean.
	\param key name of the entry in the directory of the ini file. Syntax directory:entry
	\param not_def value that function returns if there isn't the specified entry
*/

int wrappImpl::GetBool(char* key,int not_def){
	return iniparser_getboolean(dict,key,not_def);
}

/*!
	Save store the dictionary into the ini file specified.
	\param f
*/
void wrappImpl::Save(char* f){
	FILE* file_;
	file_ = fopen(f,"w");
	iniparser_dump_ini(dict,file_);
	fclose(file_);
}