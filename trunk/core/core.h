#ifndef core_H_INCLUDED
#define core_H_INCLUDED
//-------------------------------------------------------------------------++
#include <boost/config.hpp>
#include <boost/crc.hpp>  // for boost::crc_32_type
//-------------------------------------------------------------------------++
#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/xtime.hpp>
//-------------------------------------------------------------------------++
#include <string>
#include <string>
#include <sstream>
#include <vector>
//-------------------------------------------------------------------------++
namespace all { 
	namespace core {
//-----------------------------------------------------------------------++
const char VERSION_INFORMATION[] = "alcor library 0.1.0";
const char AUTHOR_INFORMATION[]  = "Andrea Carbone, Giorgio Ugazio, Stefano Marra";
//-----------------------------------------------------------------------++
const std::string relative_bin_config_path = "config";
//-----------------------------------------------------------------------++
typedef enum {
eEstablished,
eActiveOpen,
eLinkDown
}net_state_t;
//-----------------------------------------------------------------------++
struct p3dx_t{};
struct p3at_t{};

static const p3dx_t p3dx_tag = p3dx_t();
static const p3at_t p3at_tag = p3at_t();
//-----------------------------------------------------------------------++
///random tags
struct interleaved_t {};
struct planar_t {};
struct rgb_t {};
struct gray_t {gray_t(){};};
//-----------------------------------------------------------------------++
static const interleaved_t interleaved_tag = interleaved_t();
static const planar_t planar_tag = planar_t();
//-----------------------------------------------------------------------++
static const rgb_t rgb_tag = rgb_t();
static const gray_t gray_tag = gray_t();
//-----------------------------------------------------------------------++
	struct left_img_t {};
	struct right_img_t{};
	struct depth_img_t{};

  static const left_img_t   left_img    = left_img_t ();
  static const right_img_t  right_img   = right_img_t ();
  static const depth_img_t  depth_img   = depth_img_t ();

//-------------------------------------------------------------------------++
struct video_mode_t {};
struct camera_mode_t {};

static const core::video_mode_t   open_video  = 
										core::video_mode_t();
static const core::camera_mode_t  open_camera = 
										core::camera_mode_t();
//---------------------------------------------------------------------------+
//ANDREA:andrebbero in math ... effettivamente ...
template<class T>
  class constants 
  {
  public:
    /**
     * Pi
     */
    static inline T pi()   {
      return static_cast<T>(3.1415926535897932384626433832795);
    };

    /**
     * Twice Pi (2*Pi)
     */
    static inline T two_pi()   {
      return static_cast<T>(3.1415926535897932384626433832795*2.0);
    };

    /**
     * Half Pi (Pi/2)
     */
    static inline T half_pi()   {
    return static_cast<T>(1.5707963267948966192313216916398);
      //return static_cast<T>(3.1415926535897932384626433832795/2.0);
    };

    /**
     * rad_to_deg  
     */
    static inline T rad_to_deg(T val)  {
    return static_cast<T>(val * 180.0/3.1415926535897932384626433832795);
    };

    /**
     * deg_to_rad
     */
    static inline T deg_to_rad(T val)   {
    return static_cast<T>(val * 3.1415926535897932384626433832795/180.0);
    };
    /**
     * e (basis of natural logarithm) = 2.7182818284590452353602874713526...
     */
    static inline T e() throw() {
      return static_cast<T>(2.7182818284590452353602874713526);
      // continuing with ... 62497757247093699959574966967627724076630353547594571382178525166427427466391932003059921817413596629043572900334295260
      // more at http://sources.wikipedia.org/wiki/E_to_10%2C000_places
    };

    /**
     * square root of 2 = 1.4142135623730950488016887242097...
     */
    static inline T sqrt2() throw() {
      return static_cast<T>(1.4142135623730950488016887242097);
    };

  protected:
    /**
     * Protected constructor avoids the creation of
     * an instance of this class.
     */
    constants() {};
  };
//-------------------------------------------------------------------------++
  typedef all::core::constants<double> dconstants;
  typedef all::core::constants<float>  fconstants;
//-------------------------------------------------------------------------++
    ///
template <typename T>
static void truncate_2(T& val)
{
  int temp = static_cast<int>(val*100);
  val = static_cast<T>(temp)/static_cast<T>(100.0);
}
//---------------------------------------------------------
///
template <typename T>
static void truncate_1(T& val)
{
  int temp = static_cast<int>(val*10);
  val = static_cast<T>(temp)/static_cast<T>(10.0);
}
//-------------------------------------------------------------------------++
/**	costruisce una stringa di almeno 'number' caratteri. se la sorgente non riempie i caratteri, 
	vengono introdotti tanti '0' (ALL'INIZIO) quanti sono i caratteri mancanti*/
template <typename Source> std::string make_string(Source s, size_t number=0, char fill = '0')
{
	std::ostringstream buf,ret;
	buf << s;
	if(buf.str().size()>=number)
		return buf.str();
	for(size_t i=0;i<number-buf.str().size();++i)
		ret << fill;
	ret << s;
	return ret.str();
}
//-------------------------------------------------------------------------++

//-------------------------------------------------------------------------++
struct null_deleter
{
    void operator()(void const *) const
    {
    }
};
//-------------------------------------------------------------------------++
//struct i_service_handler
//{
//		///Implement this with handlers
//		virtual void register_to() = 0;
//		///Called when server goes down
//		virtual void lost_connection()=0;
//};
//-------------------------------------------------------------------------++
//-------------------------------------------------------------------------++
//namespace sandbox {
  typedef unsigned char  uint8_t;
  typedef uint8_t* uint8_ptr;
  typedef boost::shared_array<uint8_t> uint8_sptr;
  ///better name
   typedef boost::shared_array<uint8_t> uint8_sarr; 

  typedef  char  int8_t;
  typedef  int8_t* int8_ptr;

  typedef short  int16_t;
  typedef int16_t* int16_ptr;

  typedef long int  int32_t;
  typedef int32_t* int32_ptr; 

  typedef float single_t;
  typedef single_t* single_ptr;
  typedef boost::shared_array<single_t> single_sptr;
  typedef boost::shared_array<single_t> single_sarr;

  typedef double double_t;
  typedef double_t* double_ptr;
  typedef boost::shared_array<double_t> double_sptr;
  typedef boost::shared_array<double_t> double_sarr;

//---------------------------------------------------------------------------+
struct jpeg_data_t{
  all::core::uint8_sarr data;
  size_t                size; 
  size_t                height;
  size_t                width;
  size_t                depth;
  boost::crc_32_type::value_type    crc;
};
//---------------------------------------------------------------------------+
template <class T>
class traits;
//-------------------------------------------------------------------------++
///Unsinged integer value [0,255]
///One byte lenght
template< >
class traits<uint8_t>
{
public:
  typedef uint8_t value_type;
  typedef uint8_t* ptr;
  typedef const uint8_t* const_ptr;
    //: Is signed
  static bool is_signed() {return false;}
  static unsigned num_bits() {return 8;}
  BOOST_STATIC_CONSTANT(std::size_t, size=sizeof(uint8_t));
};
//-------------------------------------------------------------------------++
template< >
class traits<int8_t>
{
public:
  typedef int8_t value_type;
  typedef int8_t* ptr;
  typedef const int8_t* const_ptr;
    //: Is signed
  static bool is_signed() {return true;}
  static unsigned num_bits() {return 8;}
  BOOST_STATIC_CONSTANT(std::size_t, size=sizeof(int8_t));
};
//-------------------------------------------------------------------------++
template< >
class traits<int16_t>
{
public:
  typedef int16_t value_type;
  typedef int16_t* ptr;
  typedef const int16_t* const_ptr;
    //: Is signed
  static bool is_signed() {return true;}
  static unsigned num_bits() {return 16;}
  BOOST_STATIC_CONSTANT(std::size_t, size=sizeof(int16_t));
};
//-------------------------------------------------------------------------++
template< >
class traits<int32_t>
{
public:
  typedef int32_t value_type;
  typedef int32_t* ptr;
  typedef const int32_t* const_ptr;
    //: Is signed
  static bool is_signed() {return true;}
  static unsigned num_bits() {return 8*sizeof(int32_t);}
  BOOST_STATIC_CONSTANT(std::size_t, size=sizeof(int32_t));
};
//-------------------------------------------------------------------------++
template< >
class traits<single_t>
{
public:
  typedef single_t value_type;
  typedef single_t* ptr;
  typedef const single_t* const_ptr;
  static bool is_signed() {return true;}
  static unsigned num_bits() {return 8*sizeof(single_t);}
  BOOST_STATIC_CONSTANT(std::size_t, size=sizeof(single_t));
};
//-------------------------------------------------------------------------++
template< >
class traits<double_t>
{
public:
  typedef double_t  value_type;
  typedef double_t* ptr;
  typedef const double_t* const_ptr;
  static bool is_signed() {return true;}
  static unsigned num_bits() {return 8*sizeof(double_t);}
  BOOST_STATIC_CONSTANT(std::size_t, size=sizeof(double_t));
};
//-------------------------------------------------------------------------++
//-------------------------------------------------------------------------++
inline void BOOST_SLEEP(unsigned int _millisecs_)
{
   boost::xtime xt; 
   unsigned int secs = static_cast<unsigned int>(_millisecs_*0.001);  
   unsigned int nsecs = (1000000*_millisecs_)-(secs*1000000000);
   boost::xtime_get(&xt, boost::TIME_UTC);
   xt.sec += secs;
   xt.nsec += nsecs;
   boost::thread::sleep(xt);
}
//-------------------------------------------------------------------------++
inline void do_sleep(int ms) {
    using namespace boost;
    boost::xtime next;
    boost::xtime_get( &next, boost::TIME_UTC);
    next.nsec += (ms % 1000) * 1000000;

    int nano_per_sec = 1000000000;
    next.sec += next.nsec / nano_per_sec;
    next.sec += ms / 1000;
    next.nsec %= nano_per_sec;
    boost::thread::sleep( next);
}
//-------------------------------------------------------------------------++
inline void SLEEP_MSECS(unsigned int _millisecs_)
{
   boost::xtime xt; 
   unsigned int secs = static_cast<unsigned int>(_millisecs_*0.001);  
   unsigned int nsecs = (1000000*_millisecs_)-(secs*1000000000);
   boost::xtime_get(&xt, boost::TIME_UTC);
   xt.sec += secs;
   xt.nsec += nsecs;
   boost::thread::sleep(xt);
}
//-------------------------------------------------------------------------++
//inline void SLEEP_SECS(unsigned int _seconds_)
//{
//   boost::xtime xt; 
//   //unsigned int secs = static_cast<unsigned int>(_millisecs_*0.001);  
//   unsigned int nsecs = (1000000*_millisecs_)-(secs*1000000000);
//   boost::xtime_get(&xt, boost::TIME_UTC);
//   xt.sec += secs;
//   xt.nsec += nsecs;
//   boost::thread::sleep(xt);
//}
//-------------------------------------------------------------------------++
///
struct ip_address_t
{
  std::string		 hostname;
  unsigned int port;
};
//-------------------------------------------------------------------------++
}}
//-------------------------------------------------------------------------++
#endif //core_H_INCLUDED
