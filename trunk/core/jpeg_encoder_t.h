#ifndef jpeg_encoder_t_H_INCLUDED
#define jpeg_encoder_t_H_INCLUDED
//...................................................................
#include "alcor/core/core.h"
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
//...................................................................
namespace all { namespace core {
//...................................................................
///forward
namespace detail 
{
  struct jpeg_encoder_impl;
}
//...................................................................
///
class jpeg_encoder_t 
{
public:
  ///
  jpeg_encoder_t();


  ///Perform actual encoding.
  ///jpeg_encoder_t::encode([raw image as a shared_array], quality).
  boost::function< core::jpeg_data_t  (all::core::uint8_sarr, int ) > encode;

  ///
  void reset (all::core::rgb_t,  interleaved_t, int height, int width);
  ///
  void reset (all::core::rgb_t,  planar_t, int height, int width);
  ///
  void reset (all::core::gray_t, int height, int width);

private:
  ///
  core::jpeg_data_t encode_interleaved  (all::core::uint8_sarr, int quality = 75);
  ///
  core::jpeg_data_t encode_planar       (all::core::uint8_sarr, int quality = 75);

private:
  ///
  boost::shared_ptr<detail::jpeg_encoder_impl> impl;
};
//...................................................................
typedef boost::shared_ptr<all::core::jpeg_encoder_t> jpeg_encoder_ptr_t;
//...................................................................
}}//namespace all::core
//...................................................................
#endif //jpeg_encoder_t_H_INCLUDED