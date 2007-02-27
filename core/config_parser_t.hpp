#ifndef config_parser_t_H_INCLUDED
#define config_parser_t_H_INCLUDED
//---------------------------------------------------------------------
#include <string>
#include <memory>
//---------------------------------------------------------------------
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ini_parser.hpp>
//#pragma comment(lib, "config_parser.lib")
//---------------------------------------------------------------------
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
//---------------------------------------------------------------------
//#pragma warning (disable : 4018 4244 4819 4996 4099 936)
//---------------------------------------------------------------------

namespace all { namespace core{

		namespace tags
		{ 
			struct xml_t{};
			struct ini_t{};
			
			static const xml_t xml = xml_t();
			static const ini_t ini = ini_t();
		}
	}}
//---------------------------------------------------------------------
namespace all { namespace core{
//---------------------------------------------------------------------
///Parses ini and xml config files
class config_parser_t
{
public:
	///
	void load(tags::xml_t, std::string xmlfile)
    {
	  read_xml(xmlfile, pt_);
    }
	///
	void load(tags::ini_t, std::string inifile)
    {
    read_ini(inifile, pt_);
    }
	///
	template <typename T> 
  T get(std::string fname, T defvalue = T())
  {
    return pt_.get<T>(fname.c_str(), defvalue);
  }

private:
	boost::property_tree::ptree pt_;
};
//----------------------------------------------------------------------++
//////----------------------------------------------------------------------++
//#define SPECIALIZECONFIG(_PTYPE_)\
//template <> \
//inline _PTYPE_ config_parser_t::get<_PTYPE_>(std::string fname, _PTYPE_ defvalue)\
//{\
//return pt_.get<_PTYPE_>(fname.c_str(), defvalue);\
//};
//---------------------------------------------------------------------------
}}//namespace
//----------------------------------------------------------------------++
#endif //config_parser_t_H_INCLUDED

