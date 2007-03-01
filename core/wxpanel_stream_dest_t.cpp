#include "wxpanel_stream_dest_t.hpp"
#include <alcor/core/image_utils.h>
#include <wx/image.h>
#include <wx/dcclient.h>

namespace all {
	namespace core {

void wxpanel_stream_dest_t::process_data() {
	boost::crc_32_type::value_type enc_crc;
	int enc_data_size = m_data_size - sizeof(enc_crc);
	all::core::uint8_sarr enc_data(new all::core::uint8_t[enc_data_size]);

	memcpy(&enc_crc, m_data.get(), sizeof(enc_crc));

	memcpy(enc_data.get(), m_data.get() + sizeof(enc_crc), enc_data_size);

	if (m_decoder.decode(m_image, enc_data, enc_data_size)) {
		draw_panel();
	}
}

void wxpanel_stream_dest_t::draw_panel() {

	wxClientDC dc_client(this);
	
	wxImage cam_image(m_image.width, m_image.height, reinterpret_cast <unsigned char*> (m_image.data.get()), true);

	wxBitmap cam_bmp(cam_image, 24);

	PrepareDC(dc_client);
	dc_client.DrawBitmap(cam_bmp, 0, 0, false);

}

}}