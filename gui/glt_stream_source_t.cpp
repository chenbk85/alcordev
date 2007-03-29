#define WIN32_LEAN_AND_MEAN

#include "glt_stream_source_t.hpp"
#include "glt_stream_server_t.hpp"

glt_stream_source_t::glt_stream_source_t(glt_stream_server_t* source) {

	m_gl_source = source;

	m_encoder.reset(all::core::rgb_tag, all::core::planar_tag, m_gl_source->height, m_gl_source->width);

}


void glt_stream_source_t::process_command(all::core::net_packet_ptr_t packet) {
	
	m_gl_source->xcam = packet->buf_to_double();
	m_gl_source->ycam = packet->buf_to_double();
	m_gl_source->zcam = packet->buf_to_double();
	m_gl_source->quat[0] = packet->buf_to_double();
	m_gl_source->quat[1] = packet->buf_to_double();
	m_gl_source->quat[2] = packet->buf_to_double();
	m_gl_source->quat[3] = packet->buf_to_double();
	
}

int glt_stream_source_t::get_data(all::core::uint8_ptr* data) {
	
	boost::mutex::scoped_lock lock (m_gl_source->mutex);

	all::core::jpeg_data_t jpeg_data;

	jpeg_data = m_encoder.encode(m_gl_source->image, 75);

	size_t buffer_size = jpeg_data.size + sizeof(jpeg_data.crc);

	all::core::uint8_ptr return_data = new all::core::uint8_t[buffer_size];

	all::core::uint8_ptr write_ptr = return_data;

	memcpy(write_ptr, &(jpeg_data.crc), sizeof(jpeg_data.crc));
	//
	write_ptr+=sizeof(jpeg_data.crc);

	memcpy(write_ptr, jpeg_data.data.get(), jpeg_data.size);

	*data = return_data;

	return buffer_size;
	
}