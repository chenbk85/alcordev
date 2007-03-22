#ifndef wx_stream_dest_T_HPP_INCLUDED
#define wx_stream_dest_T_HPP_INCLUDED

#include <alcor/core/stream_dest_t.hpp>
#include <alcor/core/jpeg_decoder_t.h>

using namespace all;


class  wx_stream_dest_t : public core::stream_dest_t 
{
public:
  //
	wx_stream_dest_t() 
    {m_decoder.set_output_ordering(core::interleaved_tag);}

  ///
	void process_data();

  ///
  typedef boost::function< void (const core::jpeg_data_t&)> update_callback_t;

  ///
  void set_update_callback(update_callback_t client_update_cb)
  {
    update_ = client_update_cb;
  }

  ///
  boost::mutex mutex;

private:
  ///
	core::jpeg_decoder_t m_decoder;
  ///
	core::jpeg_data_t    m_image;
  ///
  //drawing_callback_t  draw_image_;
  ///
  update_callback_t   update_;

};

//###################################################################

inline void wx_stream_dest_t::process_data() 
{


  boost::crc_32_type::value_type enc_crc;

  std::ptrdiff_t crc_offset = sizeof(boost::crc_32_type::value_type);

  size_t enc_data_size = m_data_size - crc_offset;

  memcpy(&enc_crc, m_data.get(), sizeof(enc_crc));

  boost::mutex::scoped_lock lock(mutex);

  if (m_decoder.decode(m_image, (m_data.get() + crc_offset), enc_data_size, enc_crc)) 
  {
  if(update_)
    update_(m_image);
  }
}




#endif //wx_stream_dest_T_HPP_INCLUDED