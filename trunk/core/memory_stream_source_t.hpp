#ifndef memory_stream_source_T_HPP_INCLUDED
#define memory_stream_source_T_HPP_INCLUDED
//...................................................................
#include <alcor/core/jpeg_encoder_t.h>
#include <alcor/core/stream_source_t.hpp>
//...................................................................
#include <boost/thread/mutex.hpp>
//...................................................................
namespace all{ namespace core {
//...................................................................
class memory_stream_source_t : public all::core::stream_source_t 
{
public:	
	memory_stream_source_t(size_t h, size_t w, size_t d = 3);
  ///
  void process_command(core::net_packet_ptr_t){};
	//
  void set_quality(unsigned int quality){m_jpeg_quality_ = quality;};
  //
  void update_image(const all::core::uint8_sarr&);
  //
  void update_image(const all::core::uint8_ptr);
  //
	int get_data(all::core::uint8_ptr*);

  ///
  boost::mutex  mutex;

private:
	jpeg_encoder_ptr_t  m_encoder_;
  core::uint8_sarr    m_image_;
	int                 m_jpeg_quality_;
};
//...................................................................
inline memory_stream_source_t::memory_stream_source_t(size_t h, size_t w,size_t d )
{
  m_encoder_.reset(new core::jpeg_encoder_t);
  if(d==1)
    m_encoder_->reset(core::gray_tag,  h, w);
  else
    m_encoder_->reset(core::rgb_tag,  all::core::planar_tag, h, w);

  m_image_.reset(new all::core::uint8_t[w*h*d]);
	m_jpeg_quality_ = 80;
}
//...................................................................
inline void memory_stream_source_t::update_image(const all::core::uint8_ptr pimage)
{
  m_image_.reset(pimage);
}
//...................................................................
inline void memory_stream_source_t::update_image(const all::core::uint8_sarr& image)
{
  m_image_ = image;
}
//...................................................................
inline int memory_stream_source_t::get_data(all::core::uint8_ptr* data)
{
  //
	all::core::jpeg_data_t jpeg_data;

  //lock
  boost::mutex::scoped_lock lock (mutex);
  //encode rgb/gray array  
	jpeg_data = m_encoder_->encode(m_image_, m_jpeg_quality_);
  //byte offset for crc
  std::ptrdiff_t crc_offset = sizeof(jpeg_data.crc);
  //the actual size: image+crc
	size_t buffer_size = jpeg_data.size + crc_offset;
  //keep pointer to the start
	all::core::uint8_ptr start_of_return_data = new all::core::uint8_t[buffer_size];

  all::core::uint8_ptr write_ptr = start_of_return_data;
  //write crc first
	memcpy(write_ptr, &(jpeg_data.crc), crc_offset);

	//offset
	write_ptr+=crc_offset;
  //then, the image
	memcpy(write_ptr, jpeg_data.data.get(), jpeg_data.size);
  //assign argument
	*data = start_of_return_data;

	return buffer_size;
}
//...................................................................
typedef boost::shared_ptr<memory_stream_source_t> memory_stream_source_sptr_t;
//...................................................................
}}//all::core
//...................................................................
#endif//memory_stream_source_T_HPP_INCLUDED