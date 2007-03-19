#include "alcor/core/jpeg_decoder_t.h"
#include "alcor/core/detail/jpeg_decoder_impl.cpp"

//---------------------------------------------------------------------------
namespace all { namespace core {
//---------------------------------------------------------------------------
  jpeg_decoder_t::jpeg_decoder_t():toplanar_(true)
{
  impl.reset(new detail::jpeg_decoder_impl);
}
//---------------------------------------------------------------------------
  ///
void jpeg_decoder_t::set_output_ordering(core::interleaved_t)
{
  toplanar_=false;
}
//---------------------------------------------------------------------------
///
void jpeg_decoder_t::set_output_ordering(core::planar_t)
{
  toplanar_ = true;
}
//---------------------------------------------------------------------------
bool jpeg_decoder_t::decode(  all::core::jpeg_data_t& decoded,
                              all::core::uint8_sarr   todecode, 
                              size_t                  lenght)  
{
  try {
  decoded = impl->decode_(todecode, lenght, toplanar_);
  }
  catch (std::runtime_error& e)
  {  
    printf(e.what());
    restore_(); 
    decoded.size    = -1;
    decoded.height  = -1;
    decoded.width   = -1;
    decoded.depth   = -1;
    return false;

  }
  return true;

}
//---------------------------------------------------------------------------
bool jpeg_decoder_t:: decode(  all::core::jpeg_data_t& decoded, 
                               all::core::uint8_sarr  todecode,
                               size_t lenght,
                               boost::crc_32_type::value_type  crc) 
{
  //verify crc first
  if(impl->verify_crc_(todecode,lenght,crc))
  {
    //printf("CRC test passed!\n");
    try {
    decoded = impl->decode_(todecode, lenght, toplanar_);
    }
    catch (std::runtime_error& e)
    {  
      printf(e.what());
      restore_(); 
      decoded.size    = -1;
      decoded.height  = -1;
      decoded.width   = -1;
      decoded.depth   = -1;
      return false;

    }
    return true;
  }//verify_crc_
    printf("CRC test failed!\n");
  //TODO:redundant ...  
  decoded.size    = -1;
  decoded.height  = -1;
  decoded.width   = -1;
  decoded.depth   = -1;
  return false;

}
//---------------------------------------------------------------------------
  ///crc variant decode overload. pass a raw pointer and the size.
bool jpeg_decoder_t::decode(  all::core::jpeg_data_t& decoded, 
                all::core::uint8_ptr todecode_ptr, 
                size_t lenght,
                boost::crc_32_type::value_type  crc
                )
{
  //verify crc first
  if(impl->verify_crc_(todecode_ptr, lenght, crc))
  {
    //printf("CRC test passed!\n");
    try {
    decoded = impl->decode_(todecode_ptr, lenght, toplanar_);
    }
    catch (std::runtime_error& e)
    {  
      printf(e.what());
      restore_(); 
      decoded.size    = -1;
      decoded.height  = -1;
      decoded.width   = -1;
      decoded.depth   = -1;
      return false;

    }
    return true;
  }//verify_crc_
    printf("CRC test failed!\n");
  //TODO:redundant ...  
  decoded.size    = -1;
  decoded.height  = -1;
  decoded.width   = -1;
  decoded.depth   = -1;
  return false;
}
//---------------------------------------------------------------------------
void jpeg_decoder_t::restore_()
{
  printf("\nRestoring Decoder......\n");
  impl.reset(new detail::jpeg_decoder_impl);
}
//---------------------------------------------------------------------------
}}//namespace all::core
//---------------------------------------------------------------------------