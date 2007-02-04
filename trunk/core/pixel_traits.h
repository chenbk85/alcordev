#ifndef pixel_traits_H_INCLUDED
#define pixel_traits_H_INCLUDED

typedef struct iRGB{};
typedef struct pRGB{};
typedef struct iBGR{};
typedef struct GRAY{};
typedef struct DEPTH{};
//---------------------------------------------------------------------------+
template <class T>
class pixel_traits;
//---------------------------------------------------------------------------+
template< >
class pixel_traits<iRGB>
{
public:
  typedef core::uint8_t     value_type;
  typedef value_type*       ptr;
  typedef const ptr*        const_ptr;

    //: Is signed
  static bool           is_signed()           {return false;}
  static unsigned       size_t bytes_per_channel()   {return 1;}
  static unsigned int   nchannels()            {return 3; }

  BOOST_STATIC_CONSTANT(std::size_t, size=sizeof(value_type));
  //BOOST_STATIC_CONSTANT(std::size_t, channel_stride = 1);
};
//---------------------------------------------------------------------------+
template< >
class pixel_traits<pRGB>
{
public:
  typedef core::uint8_t     value_type;
  typedef value_type*       ptr;
  typedef const ptr*        const_ptr;

    //: Is signed
  static bool         is_signed()           {return false;}
  static unsigned size_t bytes_per_channel()   {return 1;}
  static unsigned int nchannels()            {return 3; }

  BOOST_STATIC_CONSTANT(std::size_t, size=sizeof(value_type));
  BOOST_STATIC_CONSTANT(std::size_t, channel_stride = 1);
};
//---------------------------------------------------------------------------+
template< >
class pixel_traits<GRAY>
{
public:
  typedef uint8_t     value_type;
  typedef value_type* ptr;
  typedef const ptr*  const_ptr;

    //: Is signed
  static bool         is_signed()           {return false;}
  static unsigned size_t bytes_per_channel()   {return 1;}
  static unsigned int nchannels()            {return 1; }

  BOOST_STATIC_CONSTANT(std::size_t, size=sizeof(value_type));
};
//---------------------------------------------------------------------------+
template< >
class pixel_traits<DEPTH>
{
public:
  typedef core::single_t      value_type;
  typedef value_type*         ptr;
  typedef const ptr*          const_ptr;

    //: Is signed
  static bool         is_signed()           {return true;}
  static unsigned size_t bytes_per_channel()   {return sizeof(value_type);}
  static unsigned int nchannels()            {return 3; }

  BOOST_STATIC_CONSTANT(std::size_t, size=sizeof(value_type));
};
//---------------------------------------------------------------------------+
//template <typename PIXEL, typename ORDERING>

//---------------------------------------------------------------------------+


#endif//pixel_traits_H_INCLUDED