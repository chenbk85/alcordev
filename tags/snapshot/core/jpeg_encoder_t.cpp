#include "alcor/core/jpeg_encoder_t.h"
#include "alcor/core/core.h"
#include "alcor/core/detail/jpeg_encoder_impl.cpp"

//------------------------------------------------------------------------
#include <boost/bind.hpp>
//------------------------------------------------------------------------
namespace all { namespace core {
//------------------------------------------------------------------------
jpeg_encoder_t::jpeg_encoder_t()
{

}
//------------------------------------------------------------------------
///
void jpeg_encoder_t::reset ( all::core::rgb_t , 
                                      interleaved_t,
                                      int height, int width)
{
printf("Resetting dimensions to: H: %d W: %d\n", height, width);
    printf("Ordering: Interleaved\n");
impl.reset(new detail::jpeg_encoder_impl(height, width, 3));

encode = boost::bind(&jpeg_encoder_t::encode_interleaved, 
                     this,
                     _1,
                     _2);
}
//------------------------------------------------------------------------
///
void jpeg_encoder_t::reset(all::core::rgb_t,  
                           planar_t,
                           int height, int width)
{
printf("Resetting dimensions to: H: %d W: %d\n", height, width);
    printf("Ordering: Planar\n");
impl.reset(new detail::jpeg_encoder_impl(height, width, 3));

encode = boost::bind(&jpeg_encoder_t::encode_planar, 
                     this,
                     _1,
                     _2);

}
//------------------------------------------------------------------------
///
void jpeg_encoder_t::reset(all::core::gray_t, 
                                        int height, int width)
  {
    printf("Resetting dimensions to: H: %d W: %d\n", height, width);

    impl.reset(new detail::jpeg_encoder_impl(height, width, 1));

    encode = boost::bind(&jpeg_encoder_t::encode_interleaved, 
                     this,
                     _1,
                     _2);

  }
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
  core::jpeg_data_t jpeg_encoder_t::encode_interleaved(
                                            all::core::uint8_sarr to_encode,
                                            int quality)
  {
    core::jpeg_data_t ret;
     
    impl->encode_interleaved_impl_(ret, to_encode, quality);

    return ret;
  }
//------------------------------------------------------------------------
  core::jpeg_data_t jpeg_encoder_t::encode_planar( 
                                            all::core::uint8_sarr to_encode,
                                            int quality)
  {    
    core::jpeg_data_t ret;

    impl->encode_planar_impl_(ret, to_encode, quality); 
    return ret;
  }
////------------------------------------------------------------------------
//------------------------------------------------------------------------
}}//namespace all::core