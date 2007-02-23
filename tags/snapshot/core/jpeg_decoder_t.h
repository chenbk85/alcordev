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
  void set_ordering(core::interleaved_t);
  ///
  void set_ordering(core::planar_t);

  ///
  bool decode(  all::core::jpeg_data_t& , 
                all::core::uint8_sarr, 
                size_t lenght);  

  ///
  bool decode(  all::core::jpeg_data_t& , 
                all::core::uint8_sarr, 
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