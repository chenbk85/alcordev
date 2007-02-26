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

void stream_pkt_sender_t::async_send_packet(stream_packet_ptr_t packet) {
	
	std::string* out_data_buffer_ptr = new std::string(packet->get_buffer());
	m_socket.async_send_to(boost::asio::buffer(*out_data_buffer_ptr), m_endpoint,
						   boost::bind(&stream_pkt_sender_t::handle_send_packet, this, 
									   boost::asio::placeholders::error, 
									   boost::asio::placeholders::bytes_transferred,
									   out_data_buffer_ptr));
}

void stream_pkt_sender_t::handle_send_packet(const boost::system::error_code& error, std::size_t bytes_transferred, std::string* buffer_ptr) {
	if (!error) {
		printf("async send ok\n");
		Sleep(1);
	}
	else {
		if (m_error_callback)
			m_error_callback(error);
	}
	delete buffer_ptr;
}

	

}}} //namespaces