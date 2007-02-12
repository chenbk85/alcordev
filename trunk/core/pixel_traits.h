#ifndef pixel_traits_H_INCLUDED
#define pixel_traits_H_INCLUDED
//---------------------------------------------------------------------------+
#include "alcor/core/core.h"
//---------------------------------------------------------------------------+
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
  typedef all::core::uint8_t     value_type;
  typedef value_type*       ptr;
  typedef const ptr*        const_ptr;

    //: Is signed
  static bool           is_signed()           {return false;}
 
  BOOST_STATIC_CONSTANT(std::size_t, bytes_per_channel=1);
  BOOST_STATIC_CONSTANT(std::size_t, nchannels=3);
  BOOST_STATIC_CONSTANT(std::size_t, size=sizeof(value_type));
};
//---------------------------------------------------------------------------+
template< >
class pixel_traits<pRGB>
{
public:
  typedef all::core::uint8_t     value_type;
  typedef value_type*       ptr;
  typedef const ptr*        const_ptr;

    //: Is signed
  static bool         is_signed()           {return false;}
 
  BOOST_STATIC_CONSTANT(std::size_t, bytes_per_channel=1);
  BOOST_STATIC_CONSTANT(std::size_t, nchannels=3);
  BOOST_STATIC_CONSTANT(std::size_t, size=sizeof(value_type));
};
//---------------------------------------------------------------------------+
template< >
class pixel_traits<GRAY>
{
public:
  typedef all::uint8_t     value_type;
  typedef value_type* ptr;
  typedef const ptr*  const_ptr;

    //: Is signed
  static bool         is_signed()           {return false;}

  BOOST_STATIC_CONSTANT(std::size_t, bytes_per_channel=1);
  BOOST_STATIC_CONSTANT(std::size_t, nchannels=1);
  BOOST_STATIC_CONSTANT(std::size_t, size=sizeof(value_type));
};
//---------------------------------------------------------------------------+
template< >
class pixel_traits<DEPTH>
{
public:
  typedef all::core::single_t      value_type;
  typedef value_type*         ptr;
  typedef const ptr*          const_ptr;

    //: Is signed
  static bool         is_signed()           {return true;}
 
  BOOST_STATIC_CONSTANT(std::size_t, bytes_per_channel=sizeof(value_type));
  BOOST_STATIC_CONSTANT(std::size_t, nchannels=3);
  BOOST_STATIC_CONSTANT(std::size_t, size=sizeof(value_type));
};
//---------------------------------------------------------------------------+
template <typename PIXEL>
class image_of
{
public:
  ///
  image_of()
  {
  }

  ///
  image_of(size_t height , size_t width)
  {
    //
    height_ = height; 
    width_ = width;
    channels_ = PIXEL::nchannels;

    allocate_(height, width);
  }

  ///
  image_of(size_t height , size_t width, PIXEL::ptr p)
  {
    //
    height_   = height; 
    width_    = width;
    channels_ = PIXEL::nchannels;
    //
    allocate_(height_, width_);
    //
    memcpy(
            data_.get(),//Dst
            p, //Src
            height_ * width_ * channels_
            * PIXEL::bytes_per_channel //size
            );
  }

private:
  ///
  allocate_(size_t height, size_t width)
  {
    data_.reset( new PIXEL::value_type[height*width*PIXEL::nchannels );
  }

  ///
  boost::shared_array<PIXEL::value_type> data_;
  ///
  size_t height_; 
  size_t width_;
  size_t channels_;
};

//---------------------------------------------------------------------------+

#endif//pixel_traits_H_INCLUDED