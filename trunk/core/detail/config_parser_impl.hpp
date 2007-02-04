#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ini_parser.hpp>
//----------------------------------------------------------------------++
namespace all { namespace core { namespace detail{
//----------------------------------------------------------------------++
///
class  config_parser_impl
    {
    public:
    config_parser_impl();
    ~config_parser_impl(){};

    ///
    void load_xml_(const std::string&);
    ///
    void load_ini_(const std::string&);

    boost::property_tree::ptree pt_;

    };
//----------------------------------------------------------------------++
inline config_parser_impl::config_parser_impl()
    {
    }
//----------------------------------------------------------------------++
inline void config_parser_impl::load_xml_(const std::string& _name)
    {

    read_xml(_name, pt_);
    }
//----------------------------------------------------------------------++
inline void config_parser_impl::load_ini_(const std::string& _name)
    {
    read_ini(_name, pt_);
    }

//----------------------------------------------------------------------++
    }}}
//----------------------------------------------------------------------++
