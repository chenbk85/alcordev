#ifndef wx_gl_stream_source_t_H_INCLUDED
#define wx_gl_stream_source_t_H_INCLUDED

#include <wx/GLCanvas.h>

#include <alcor/core/stream_source_t.hpp>
#include <alcor/core/jpeg_encoder_t.h>
#include <boost/thread/mutex.hpp>
#include <alcor/core/image_utils.h>


class wx_gl_stream_source_t : public all::core::stream_source_t {

public:

	wx_gl_stream_source_t(wxGLCanvas*);

	void reset_size();

	int get_data(all::core::uint8_ptr*);

	void process_command(all::core::net_packet_ptr_t);

	void update_buffer();

public:

    float quat[4];              // orientation of object
    float xcam;
    float ycam;
    float zcam;

private:

	wxGLCanvas* m_gl_canvas;

	int m_width, m_height;

	all::core::jpeg_encoder_t m_encoder;
	
	all::core::uint8_sarr m_frame_buffer;
	all::core::uint8_sarr m_image;

	boost::mutex mutex;

};

typedef boost::shared_ptr<wx_gl_stream_source_t> wx_gl_stream_source_ptr;



inline wx_gl_stream_source_t::wx_gl_stream_source_t(wxGLCanvas* gl_canvas) {

	m_gl_canvas = gl_canvas;
	
	m_gl_canvas->GetClientSize(&m_width, &m_height);
	m_encoder.reset(all::core::rgb_tag, all::core::planar_tag, m_height, m_width);
	
	m_frame_buffer.reset(new all::core::uint8_t[m_width * m_height * 4]);
	m_image.reset(new all::core::uint8_t[m_width * m_height * 3]);

	xcam = 0.0f;
    ycam = 0.0f;
    zcam = -3.0f;
    
}

inline void wx_gl_stream_source_t::reset_size() {

	boost::mutex::scoped_lock lock (mutex);
	m_gl_canvas->GetClientSize(&m_width, &m_height);
	m_encoder.reset(all::core::rgb_tag, all::core::planar_tag, m_height, m_width);
	
	m_frame_buffer.reset(new all::core::uint8_t[m_width * m_height * 4]);
	m_image.reset(new all::core::uint8_t[m_width * m_height * 3]);
}

inline int wx_gl_stream_source_t::get_data(all::core::uint8_ptr* data) {

	all::core::jpeg_data_t jpeg_data;

	boost::mutex::scoped_lock lock (mutex);

	jpeg_data = m_encoder.encode(m_image, 75);

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


inline void wx_gl_stream_source_t::process_command(all::core::net_packet_ptr_t packet) {
	xcam = packet->buf_to_double();
	ycam = packet->buf_to_double();
	zcam = packet->buf_to_double();
	quat[0] = packet->buf_to_double();
	quat[1] = packet->buf_to_double();
	quat[2] = packet->buf_to_double();
	quat[3] = packet->buf_to_double();

	m_gl_canvas->Refresh(false);
}

inline void wx_gl_stream_source_t::update_buffer() {

	boost::mutex::scoped_lock lock (mutex);

	glReadBuffer(GL_FRONT);
	glReadPixels( 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE,(void*) m_frame_buffer.get() );

	all::core::change_ordering::from_rgba_opengl_to_planar(m_frame_buffer, m_image, m_height, m_width);
    all::core::change_ordering::to_topleft(m_image, m_height, m_width, 3);

}

#endif