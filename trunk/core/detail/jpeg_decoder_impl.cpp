//########################################################################
#include "alcor/core/core.h"
#include "alcor/core/image_utils.h"
#include "alcor.extern/jpeg-c++/jpegDecompress.h"
#include <sstream>
using std::istringstream;
//########################################################################
namespace all { namespace core { namespace detail {
//########################################################################
  struct jpeg_decoder_impl
  {
    ///
    jpeg_decoder_impl(){};
    ///
    jpeg::Decompress decoder_;
    ///
    all::core::jpeg_data_t decode_(core::uint8_sarr byte_stream, size_t byte_lenght);
    ///
    bool verify_crc_(core::uint8_sarr byte_stream, 
                    size_t byte_lenght,
                    boost::crc_32_type::value_type  crc);
    ///
    boost::crc_32_type  crc_result;
    ///
    int h_;
    ///
    int w_;
    ///
    int d_;
  };
//########################################################################
  inline all::core::jpeg_data_t jpeg_decoder_impl::decode_(core::uint8_sarr byte_stream, size_t byte_lenght)
  {
    //
    std::string temp(reinterpret_cast<char*>(byte_stream.get()), byte_lenght);
    //
    istringstream jpeg_stream(temp);
    //
    decoder_.setInputStream(jpeg_stream);
    //
    decoder_.readHeader(w_,h_,d_);
    //
    core::uint8_sarr jarr(new core::uint8_t[h_*w_*d_]);
    //
    decoder_.readImage(jarr.get());

    if (d_>1)
    {
          //CONVERT TO PLANAR
          core::change_ordering::to_planar(jarr, h_, w_, d_);
    }
    //
    jpeg_data_t jpeg_data;

    jpeg_data.height  = h_;
    jpeg_data.width   = w_;
    jpeg_data.depth   = d_;

    //
    jpeg_data.size    = h_*w_*d_;

    jpeg_data.data    = jarr;

    //
    return jpeg_data;

  }
  //########################################################################  
  inline bool jpeg_decoder_impl::verify_crc_(core::uint8_sarr byte_stream, 
                                            size_t byte_lenght,
                                            boost::crc_32_type::value_type  crc)
  {
    printf("Verifying CRC %x\n", crc);
    crc_result.process_bytes( byte_stream.get(), byte_lenght);
    return (crc == crc_result.checksum() );
  }
//########################################################################

//########################################################################
}}}//all::core::detail
//########################################################################