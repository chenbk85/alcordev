#include "stream_pkt_sender_t.hpp"

namespace all {
	namespace core {
		namespace detail {


stream_pkt_sender_t::stream_pkt_sender_t(boost::asio::ip::udp::socket& socket) : m_socket(socket) 
{
}

void stream_pkt_sender_t::set_error_callback(boost::function <void (const boost::system::error_code&)> error_callback) {
    m_error_callback = error_callback;
}

void stream_pkt_sender_t::set_endpoint(const boost::asio::ip::udp::endpoint& endpoint) {
	m_endpoint = endpoint;
}

void stream_pkt_sender_t::send_packet(stream_packet_ptr_t packet) {
	
	m_out_data_buffer = packet->get_buffer();
	try {
		m_socket.send_to(boost::asio::buffer(m_out_data_buffer), m_endpoint);
	}
	catch (boost::system::error_code e) {
		if (m_error_callback)
			m_error_callback(e);
	}
}

}}} //namespaces