#pragma once

#include <alcor/core/jpeg_encoder_t.h>
#include <alcor/core/stream_source_t.hpp>
#include <alcor/sense/opencv_grabber_t.h>

namespace all{ namespace sense {

class opencv_data_source_t : public all::core::stream_source_t 
{
public:	
	opencv_data_source_t();
	
	void set_quality(unsigned int);

	int get_data(all::core::uint8_ptr*);

private:
	boost::shared_ptr<all::core::jpeg_encoder_t>     m_encoder;
  boost::shared_ptr<all::sense::opencv_grabber_t>  m_cam;
  void init_();
  core::uint8_sarr image;
	int m_jpeg_quality;
};

}}