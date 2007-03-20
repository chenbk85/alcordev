#ifndef wx_stream_dest_t_H_INCLUDED
#define wx_stream_dest_t_H_INCLUDED

#include <wx/Window.h>
#include <wx/DCClient.h>
#include <wx/Image.h>

#include <alcor/core/stream_dest_t.hpp>
#include <alcor/core/jpeg_decoder_t.h>


class wx_stream_dest_t : public all::core::stream_dest_t {

public:
	wx_stream_dest_t(wxWindow*);

	//jpeg decoder functions
	void process_data();
	void draw_panel();

private:
	//jpeg decoder members
    all::core::jpeg_decoder_t m_decoder;
	all::core::jpeg_data_t m_image;

	wxWindow* m_dc;
};

typedef boost::shared_ptr<wx_stream_dest_t> wx_stream_dest_ptr;



inline wx_stream_dest_t::wx_stream_dest_t(wxWindow* dc) {
	m_dc = dc;
	m_decoder.set_output_ordering(all::core::interleaved_tag);
}

inline void wx_stream_dest_t::process_data() {
	boost::crc_32_type::value_type enc_crc;
	int enc_data_size = m_data_size - sizeof(enc_crc);
	all::core::uint8_sarr enc_data(new all::core::uint8_t[enc_data_size]);

	memcpy(&enc_crc, m_data.get(), sizeof(enc_crc));

	memcpy(enc_data.get(), m_data.get() + sizeof(enc_crc), enc_data_size);

	if (m_decoder.decode(m_image, enc_data, enc_data_size)) {
		draw_panel();
	}
}

inline void wx_stream_dest_t::draw_panel() {

	wxClientDC dc_client(m_dc);
	
	int w, h;

	wxImage cam_image(m_image.width, m_image.height, reinterpret_cast <unsigned char*> (m_image.data.get()), true);

	m_dc->GetSize(&w, &h);

	if ( (w != m_image.width) || (h != m_image.height)) {
		int x_pos = (w - (int)m_image.width) / 2;
		int y_pos = (h - (int)m_image.height) / 2;

		cam_image.Resize(wxSize(w, h), wxPoint(x_pos, y_pos));
	}

	int depth = m_image.depth * 8;
	wxBitmap cam_bmp(cam_image, m_image.depth);

	m_dc->PrepareDC(dc_client);
	dc_client.DrawBitmap(cam_bmp, 0, 0, false);
    
}

#endif