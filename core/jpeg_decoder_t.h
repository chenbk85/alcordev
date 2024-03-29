#ifndef jpeg_decoder_t_H_INCLUDED
#define jpeg_decoder_t_H_INCLUDED
//...................................................................
#include "alcor/core/core.h"
#include "boost/shared_ptr.hpp"
//...................................................................
namespace all { namespace core {
//...................................................................
namespace detail 
{
  struct jpeg_decoder_impl;
}
//...................................................................
class jpeg_decoder_t
{
public:
  ///
  jpeg_decoder_t();

  ///
  void set_output_ordering(core::interleaved_t);
  ///
  void set_output_ordering(core::planar_t);

  ///
  bool decode(  all::core::jpeg_data_t& , 
                all::core::uint8_sarr, 
                size_t lenght);  

  ///crc variant decode overload. takes a shared_array.
  bool decode(  all::core::jpeg_data_t& , 
                all::core::uint8_sarr, 
                size_t lenght,
                boost::crc_32_type::value_type  crc
                ); 

  ///crc variant decode overload. pass a pointer and the size.
  bool decode(  all::core::jpeg_data_t& , 
                all::core::uint8_ptr, 
                size_t lenght,
                boost::crc_32_type::value_type  crc
                ); 

private:  
  ///recover from error..
  void restore_();
  ///
  boost::shared_ptr<detail::jpeg_decoder_impl> impl;
  ///
  bool toplanar_;
};
//...................................................................
}}//all::core
//...................................................................
#endif  //jpeg_decoder_t_H_INCLUDED