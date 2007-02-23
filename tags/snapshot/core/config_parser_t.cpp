#include "config_parser_t.h"
//----------------------------------------------------------------------++
#include "detail/config_parser_impl.hpp"
//----------------------------------------------------------------------++
namespace all{ 
//----------------------------------------------------------------------++
  core::config_parser_t::config_parser_t(std::string& config_)
    {
      config_name = config_;
    impl.reset(new core::detail::config_parser_impl);
    }
//----------------------------------------------------------------------++
core::config_parser_t::config_parser_t()
    {
      config_name = "__noname__";
    impl.reset(new core::detail::config_parser_impl);
    }
//----------------------------------------------------------------------++
core::config_parser_t::~config_parser_t()
    {
    impl.release();
    }
//----------------------------------------------------------------------++
void core::config_parser_t::load(core::detail::xml_t, const std::string& _xmlfile)
    {
    impl->load_xml_(_xmlfile);
    }
//----------------------------------------------------------------------++
void core::config_parser_t::load(core::detail::ini_t, const std::string& _inifile)
    {
    impl->load_ini_(_inifile);
    }
//----------------------------------------------------------------------++
///
int core::config_parser_t::as_int(const std::string& _tag, int _def)
{
	return impl->pt_.get<int>(_tag.c_str(), _def);
}
//----------------------------------------------------------------------++
///
double core::config_parser_t::as_double(const std::string& _tag , double _def)
{
	return impl->pt_.get<double>(_tag.c_str(), _def);
}
//----------------------------------------------------------------------++
///
float core::config_parser_t::as_float (const std::string& _tag, float _def)
{
	return impl->pt_.get<float>(_tag.c_str(), _def);
}
//----------------------------------------------------------------------++
///
long core::config_parser_t::as_long(const std::string& _tag, long _def)
{
	return impl->pt_.get<long>(_tag.c_str(), _def);
}
//----------------------------------------------------------------------++
///
short core::config_parser_t::as_short(const std::string& _tag, short _def)
{
	return impl->pt_.get<short>(_tag.c_str(), _def);
}
//----------------------------------------------------------------------++
///
std::string core::config_parser_t::as_string(const std::string&  _tag, const std::string& _def)
{
	return impl->pt_.get<std::string>(_tag.c_str(), _def);
}

////////----------------------------------------------------------------------++
//#define _DOGENSPECIALIZATION(_TYPETAG_) template<> \
//    _TYPETAG_ all::core::config_parser_t::get<_TYPETAG_>\
//    (const std::string& _tag  ,const _TYPETAG_& _def) const\
//    {\
//    return impl->pt_.get<_TYPETAG_>(_tag.c_str(), _def);\
//    }
////----------------------------------------------------------------------++
//_DOGENSPECIALIZATION(short)
//_DOGENSPECIALIZATION(int)
//_DOGENSPECIALIZATION(long)
//_DOGENSPECIALIZATION(double)
//_DOGENSPECIALIZATION(float)
//_DOGENSPECIALIZATION(std::string)
//----------------------------------------------------------------------++
//----------------------------------------------------------------------++
    }//namespace
//----------------------------------------------------------------------++