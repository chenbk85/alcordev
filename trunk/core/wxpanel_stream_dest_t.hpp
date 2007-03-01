#ifndef wxpanel_stream_dest_t_H_INCLUDED
#define wxpanel_stream_dest_t_H_INCLUDED

#include <wx/panel.h>

#include <alcor/core/stream_dest_t.hpp>
#include <alcor/core/jpeg_decoder_t.h>

namespace all {
	namespace core {

class wxpanel_stream_dest_t : public wxPanel, public all::core::stream_dest_t {

public:
	wxpanel_stream_dest_t() {m_decoder.set_output_ordering(all::core::interleaved_tag);}
	void process_data();

	void draw_panel();

private:
	all::core::jpeg_decoder_t m_decoder;

	all::core::jpeg_data_t m_image;

};

}} //namespaces

#endif