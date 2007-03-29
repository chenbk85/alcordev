#ifndef glt_stream_server_t_H_INCLUDED
#define glt_stream_server_t_H_INCLUDED

#include <glutm/window.h>

#include <alcor/gui/trackball.h>
#include <alcor/core/image_utils.h>
#include <alcor/core/stream_server_t.hpp>

class glt_stream_source_t;

class glt_stream_server_t : public GlutWindow{

public:

	glt_stream_server_t(int, int);
	
	void OnOpen();
	void OnClose();
	void OnDisplay();

	void init_gl();
	void build_trans_matrix();
	void reset_projection_mode();

	void update_buffer();

	boost::function <void (void)> draw_data;

public:
	
	float quat[4];              // orientation of object
    float xcam;
    float ycam;
    float zcam;

	int width, height;

	all::core::uint8_sarr image;

	boost::mutex mutex;

protected:

	all::core::uint8_sarr m_frame_buffer;

	boost::shared_ptr<glt_stream_source_t> m_stream_source;
	boost::shared_ptr<all::core::stream_server_t> m_stream_server;

};



#endif