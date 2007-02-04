//########################################################################
#include "alcor/core/core.h"
#include "alcor.extern/jpeg-c++/jpegCompress.h"
#include "alcor/core/image_utils.h"
#include <boost/shared_ptr.hpp>
//########################################################################
namespace all  { namespace core { namespace detail {
//########################################################################
  //Forward declared
//-------------------------------------------------------------------
///jpeg_memory_dest_t
struct jpeg_memory_dest_t : public jpeg::OStream
{
    ///
    jpeg_memory_dest_t();
    /**
     * writes nb_to_write bytes.
     */
    void writeBytes(char* src_buf,int nb_to_write);
    ///
    void init_(int height, int width, int depth);
    ///
    size_t datacount;
    ///
    all::core::uint8_sarr jpeg_arr;

};
//########################################################################
//-.-----------------------------------------------------------------
///jpeg_encoder_impl
struct jpeg_encoder_impl
{
  ///
  jpeg_encoder_impl(int height, int width, int depth);

  ///Assumes interleaved RGB
  void encode_interleaved_impl_ (core::jpeg_data_t& res, core::uint8_sarr, int quality);
  ///Assumes planar RGB
  void encode_planar_impl_ (core::jpeg_data_t& res, core::uint8_sarr, int quality);

  ///
  jpeg_memory_dest_t jpeg_sink;
  ///
  jpeg::Compress encoder;
  ///
  //boost::shared_ptr<all::core::change_ordering_cached> change_ordering_sptr;
  ///
  int w_;
  ///
  int h_;
  ///
  int d_;
};
//########################################################################
//          jpeg_encoder_impl
//########################################################################
inline jpeg_encoder_impl::jpeg_encoder_impl(int height, int width, int depth)
{
  w_ = width;
  h_ = height;
  d_ = depth;

  jpeg_sink.init_(height, width, depth);
  encoder.setOutputStream(&jpeg_sink);

  //change_ordering_sptr.reset( 
  //  new all::core::change_ordering_cached(height, width, depth) );
}
//------------------------------------------------------------------------
//
inline void jpeg_encoder_impl::encode_interleaved_impl_(core::jpeg_data_t& ret, core::uint8_sarr arr, int quality)
{
  jpeg_sink.datacount = 0;

  encoder.writeImage(w_, h_, d_, arr.get(), quality);
  
  core::uint8_ptr temp = new all::core::uint8_t[jpeg_sink.datacount];

  memcpy (temp, jpeg_sink.jpeg_arr.get(), jpeg_sink.datacount);

  ret.data.reset(temp);
  ret.size    = jpeg_sink.datacount;
  ret.height  =   h_;
  ret.width   =   w_;
  ret.depth   =   d_;

}
//------------------------------------------------------------------------
//
inline void jpeg_encoder_impl::encode_planar_impl_(core::jpeg_data_t& ret, 
                                                   core::uint8_sarr arr, 
                                                   int quality)
{
  jpeg_sink.datacount = 0;

  //change_ordering_sptr->to_interleaved(arr);

  core::change_ordering::to_interleaved(arr, h_,w_,d_);

  encoder.writeImage(w_, h_, d_, arr.get(), quality);
  
  core::uint8_ptr temp = new all::core::uint8_t[jpeg_sink.datacount];

  memcpy (temp, jpeg_sink.jpeg_arr.get(), jpeg_sink.datacount);

  ret.data.reset(temp);
  ret.size    =   jpeg_sink.datacount;
  ret.height  =   h_;
  ret.width   =   w_;
  ret.depth   =   d_;

}
//########################################################################
//                        jpeg_memory_dest_t
//########################################################################
//-------------------------------------------------------------------
inline jpeg_memory_dest_t::jpeg_memory_dest_t():datacount(0)
{

}
//-------------------------------------------------------------------
inline void jpeg_memory_dest_t::writeBytes(char *src_buf, int nb_to_write)
{
  memcpy(jpeg_arr.get()+ datacount, src_buf, nb_to_write);
  datacount+=nb_to_write;
}
//-------------------------------------------------------------------
inline void jpeg_memory_dest_t::init_(int height, int width, int depth)
{
  printf("Initializing Jpeg Encoder with H: %d W: %d D: %d\n", height,width, depth);
  ///oversized ...
  jpeg_arr.reset(new all::core::uint8_t[height * width * depth]);
}
//-------------------------------------------------------------------
}}}//all::core::detail