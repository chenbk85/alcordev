#include "alcor/core/jpeg_decoder_t.h"
#include "alcor/core/detail/jpeg_decoder_impl.cpp"

//---------------------------------------------------------------------------
namespace all { namespace core {
//---------------------------------------------------------------------------
jpeg_decoder_t::jpeg_decoder_t()
{
  impl.reset(new detail::jpeg_decoder_impl);
}
//---------------------------------------------------------------------------
bool jpeg_decoder_t::decode(  all::core::jpeg_data_t& decoded,
                              all::core::uint8_sarr   todecode, 
                              size_t                  lenght)  
{
  try {
  decoded = impl->decode_(todecode, lenght);
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
void jpeg_decoder_t::restore_()
{
  printf("\nRestoring Decoder......\n");
  impl.reset(new detail::jpeg_decoder_impl);
}
//---------------------------------------------------------------------------
}}//namespace all::core
//---------------------------------------------------------------------------