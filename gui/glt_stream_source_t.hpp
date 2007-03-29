#ifndef glt_stream_source_t_H_INCLUDED
#define glt_stream_source_t_H_INCLUDED

#include <alcor/core/stream_source_t.hpp>
#include <alcor/core/jpeg_encoder_t.h>

class glt_stream_server_t;


class glt_stream_source_t : public all::core::stream_source_t {

public:
	glt_stream_source_t(glt_stream_server_t*);

	int get_data(all::core::uint8_ptr*);

	void process_command(all::core::net_packet_ptr_t);


private:

	all::core::jpeg_encoder_t m_encoder;

	glt_stream_server_t* m_gl_source;

};

typedef boost::shared_ptr <glt_stream_source_t> glt_stream_source_ptr;




#endif
