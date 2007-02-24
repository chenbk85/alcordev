#ifndef config_parser_t_H_INCLUDED
#define config_parser_t_H_INCLUDED
//---------------------------------------------------------------------
#include <string>
#include <memory>
#pragma comment(lib, "config_parser_t.lib")
//---------------------------------------------------------------------
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
//---------------------------------------------------------------------
//#pragma warning (disable : 4018 4244 4819 4996 4099 936)
//---------------------------------------------------------------------
namespace all { namespace core{

    namespace detail
        { 
        struct xml_t{};
        struct ini_t{};
        }
    static const core::detail::xml_t xml = core::detail::xml_t();
    static const core::detail::ini_t ini = core::detail::ini_t();
    }}
//---------------------------------------------------------------------
namespace all { namespace core{

    namespace detail{class config_parser_impl;}
    class config_parser_t;

    }}
//---------------------------------------------------------------------
class all::core::config_parser_t
{
public:
	///Constructor
  config_parser_t();
  config_parser_t(std::string&);
    ~config_parser_t();
    ///
    void load(core::detail::xml_t, const std::string&);
    ///
    void load(core::detail::ini_t, const std::string&);

public:
	///
	int as_int(const std::string&, int);
	///
	double as_double(const std::string&, double);
	///
	float as_float (const std::string&, float);
	///
	long as_long(const std::string&, long);
	///
	short as_short(const std::string&, short);
	///
	std::string as_string(const std::string&, const std::string&);

    //template <typename T>
    //T get<T>(const std::string&, const T&);

private:
    std::auto_ptr<core::detail::config_parser_impl> impl;
    std::string config_name;
};
//---------------------------------------------------------------------------
#endif //config_parser_t_H_INCLUDED
//---------------------------------------------------------------------------