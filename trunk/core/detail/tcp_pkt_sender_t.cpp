#include "tcp_pkt_sender_t.hpp"

tcp_pkt_sender_t::tcp_pkt_sender_t(boost::asio::ip::tcp::socket& socket) : m_socket(socket) 
{
}

void tcp_pkt_sender_t::set_error_callback(boost::function <void (const boost::system::error_code&)> error_callback) {
    m_error_callback = error_callback;
}


void tcp_pkt_sender_t::send_packet(net_packet_ptr_t packet) {
	
	const net_packet_header_t packet_header = packet->get_header();
	m_out_header_buffer = packet_header.get_header();

	m_out_data_buffer = packet->get_buffer();

	printf("packet data: %s\n", m_out_data_buffer.c_str());

	std::vector<boost::asio::const_buffer> buffers;
    buffers.push_back(boost::asio::buffer(m_out_header_buffer));
	buffers.push_back(boost::asio::buffer(m_out_data_buffer));

	boost::asio::async_write(m_socket,
							 buffers,
							 boost::bind(&tcp_pkt_sender_t::handle_write,
									     this,
										 boost::asio::placeholders::error,       
										 boost::asio::placeholders::bytes_transferred));
}

void tcp_pkt_sender_t::handle_write(const boost::system::error_code& error, std::size_t bytes_transferred) {
	if (!error) {
		printf("packet sent\n%i bytes", bytes_transferred);
	}
	else {
		printf("Error in tcp_pkt_sender_t::handle_write:\n %s", error.message());
		if (m_error_callback) 
			m_error_callback(error);
	}
}
