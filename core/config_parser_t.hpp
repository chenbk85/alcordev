//#include "config_parser_t.h"
//----------------------------------------------------------------------++
#include "detail/config_parser_impl.hpp"
//----------------------------------------------------------------------++
namespace all{ 
//----------------------------------------------------------------------++
inline core::config_parser_t::config_parser_t()
    {
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
////----------------------------------------------------------------------++
//void core::xml_config_t::save(const std::string&)
//    {
//    }
//----------------------------------------------------------------------++
//template<typename T> 
//inline T  all::core::config_parser_t::get<T>
//	(const std::string& _tag, const T& _def) const
//    {
//    return impl->pt_.get<T>(_tag.c_str(),_def);
//    }
////----------------------------------------------------------------------++
#define _DOGENSPECIALIZATION(_TYPETAG_) template<> \
    _TYPETAG_ all::core::config_parser_t::get<_TYPETAG_>\
    (const std::string& _tag  ,const _TYPETAG_& _def) const\
    {\
    return impl->pt_.get<_TYPETAG_>(_tag.c_str(), _def);\
    }
//----------------------------------------------------------------------++
_DOGENSPECIALIZATION(short)
_DOGENSPECIALIZATION(int)
_DOGENSPECIALIZATION(long)
_DOGENSPECIALIZATION(double)
_DOGENSPECIALIZATION(float)
_DOGENSPECIALIZATION(std::string)
//----------------------------------------------------------------------++
//----------------------------------------------------------------------++
    }//namespace
//----------------------------------------------------------------------++