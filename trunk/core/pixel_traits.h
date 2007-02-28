#ifndef pixel_traits_H_INCLUDED
#define pixel_traits_H_INCLUDED
//---------------------------------------------------------------------------+
#include "alcor/core/core.h"
#include <boost/shared_ptr.hpp>
//---------------------------------------------------------------------------+
namespace all { namespace core {
//---------------------------------------------------------------------------+
struct iRGB{};
struct pRGB{};
struct iBGR{};
struct GRAY{};
struct DEPTH{};
//---------------------------------------------------------------------------+
template <class T>
class pixel_traits;
//---------------------------------------------------------------------------+
template< >
class pixel_traits<iRGB>
{
public:
  typedef all::core::uint8_t     value_type;
  typedef all::core::traits<value_type>::ptr ptr;
  typedef const ptr*             const_ptr;

    //: Is signed
  static bool is_signed(){return false;} 
  ///
  BOOST_STATIC_CONSTANT(std::size_t, bytes_per_channel=1);
  BOOST_STATIC_CONSTANT(std::size_t, nchannels=3);
  BOOST_STATIC_CONSTANT(std::size_t, size=sizeof(value_type));
  ///
  //static size_t row_stride(size_t width)
  //{
  //  return (width*nchannels);
  //}

  /////
  //static size_t column_stride(size_t height)
  //{
  //  return (nchannels);
  //}

  /////
  //static size_t channel_stride (size_t height, size_t width)
  //{
  //  return (1);
  //}

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
  typedef all::core::uint8_t     value_type;
  typedef value_type* ptr;
  typedef const ptr*  const_ptr;

    //: Is signed
  static bool is_signed()   {return false;}

  BOOST_STATIC_CONSTANT(std::size_t, bytes_per_channel=1);
  BOOST_STATIC_CONSTANT(std::size_t, nchannels=1);
  BOOST_STATIC_CONSTANT(std::size_t, size=sizeof(value_type));
};
//---------------------------------------------------------------------------+
template< >
class pixel_traits<DEPTH>
{
public:
  typedef all::core::single_t value_type;
  typedef value_type*         ptr;
  typedef const ptr*          const_ptr;

    //: Is signed
  static bool  is_signed() {return true;}
 
  BOOST_STATIC_CONSTANT(std::size_t, bytes_per_channel=sizeof(value_type));
  BOOST_STATIC_CONSTANT(std::size_t, nchannels=3);
  BOOST_STATIC_CONSTANT(std::size_t, size=sizeof(value_type));
};

//---------------------------------------------------------------------------+
template <typename PIXELTYPE>
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
    channels_ = pixel_traits<PIXELTYPE>::nchannels;

    allocate_(height_, width_);
  }

  ///
  image_of(   size_t height 
            , size_t width 
            , typename pixel_traits<PIXELTYPE>::ptr p
            , bool shared_ = false)
  {
    assign(height,width,p,shared);
  }
  
  typedef boost::shared_array<typename pixel_traits<PIXELTYPE>::value_type>
    buffer_type;

  ///
  void assign(size_t height 
            , size_t width 
            , typename pixel_traits<PIXELTYPE>::ptr p
            , bool shared  = false)
  {
      //
    height_     = height; 
    width_      = width;
    channels_   = pixel_traits<PIXELTYPE>::nchannels;

    //vale solo nel caso planare ... da considerare nei traits.
    row_stride_  = width_;
    column_stride_ = 1;
    channel_stride_ = height_*width_;

    if(shared)
    {
      data_.reset(p);
    }
    else
    {
    //
    allocate_(height_, width_);
    //
    memcpy(
            data_.get(),//Dst
            p, //Src
            height_ * width_ * channels_
            * pixel_traits<PIXELTYPE>::bytes_per_channel //size
            );
    }
  }

  buffer_type get_buffer_sptr() const
  {
    return data_;
  }

  size_t height()   const {return height_;}
  size_t width()    const {return width_;}
  size_t channels() const {return channels_;}



  ///
  typename pixel_traits<PIXELTYPE>::value_type
    get(size_t row, size_t col, size_t ch = 0) const
  {
       return data_[( row * row_stride_ )
      +( col* column_stride_  )
      +( ch * channel_stride_ ) ];
  }

private:
  ///
  void allocate_(size_t height, size_t width)
  {
    data_.reset( new pixel_traits<PIXELTYPE>::value_type[height*width*pixel_traits<PIXELTYPE>::nchannels] );
  }

  ///
  boost::shared_array<typename pixel_traits<PIXELTYPE>::value_type> data_;
  ///
  size_t height_; 
  size_t width_;
  size_t channels_;
  size_t row_stride_;
  size_t column_stride_;
  size_t channel_stride_;
};

//---------------------------------------------------------------------------+
typedef image_of<DEPTH> depth_image_t;
typedef boost::shared_ptr<depth_image_t> depth_image_ptr_t;
//---------------------------------------------------------------------------+
}}//all::core
#endif//pixel_traits_H_INCLUDED