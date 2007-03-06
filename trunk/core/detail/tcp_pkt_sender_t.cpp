#include "tcp_pkt_sender_t.hpp"

namespace all {
	namespace core {
		namespace detail {


tcp_pkt_sender_t::tcp_pkt_sender_t(boost::asio::ip::tcp::socket& socket) : m_socket(socket) 
{
}

void tcp_pkt_sender_t::set_error_callback(boost::function <void (const boost::system::error_code&)> error_callback) {
    m_error_callback = error_callback;
}


void tcp_pkt_sender_t::send_packet(net_packet_ptr_t packet) {
	
	const net_packet_header_t packet_header = packet->get_header();
	
	//all::core::uint8_sarr header_buffer = packet_header.get_header();
	
	//m_out_header_buffer.assign(reinterpret_cast <const char*> (header_buffer.get()), net_packet_header_t::HEADER_LENGTH);

	//m_out_data_buffer.assign(packet->get_buffer().get(), packet_header.get_packet_size());

	//printf("packet data: %s\n", m_out_data_buffer.c_str());


	//std::vector<boost::asio::const_buffer> buffers;
 //   buffers.push_back(boost::asio::buffer(m_out_header_buffer));
	//buffers.push_back(boost::asio::buffer(m_out_data_buffer));

	//boost::asio::async_write(m_socket,
	//						 buffers,
	//						 boost::bind(&tcp_pkt_sender_t::handle_write,
	//								     this,
	//									 boost::asio::placeholders::error,       
	//									 boost::asio::placeholders::bytes_transferred));

	
	std::string* out_data_buffer_ptr = new std::string(reinterpret_cast <char*> (packet_header.get_header().get()), net_packet_header_t::HEADER_LENGTH);
	
	*out_data_buffer_ptr += std::string(packet->get_buffer().get(), packet_header.get_packet_size());

	boost::asio::async_write(m_socket, boost::asio::buffer(*out_data_buffer_ptr),
							 boost::bind(&tcp_pkt_sender_t::handle_write,
										 this,
										 boost::asio::placeholders::error,       
										 boost::asio::placeholders::bytes_transferred,
										 out_data_buffer_ptr));


}

void tcp_pkt_sender_t::handle_write(const boost::system::error_code& error, std::size_t bytes_transferred, std::string* buffer_ptr) {
	if (!error) {
		//printf("packet sent\n%i bytes", bytes_transferred);
		Sleep(1);
	}
	else {
    printf("Error in tcp_pkt_sender_t::handle_write:\n %s", error.message().c_str());
		if (m_error_callback) 
			m_error_callback(error);
	}
	delete buffer_ptr;
}

void tcp_pkt_sender_t::send_packet_blk(net_packet_ptr_t packet) {
	
	const net_packet_header_t packet_header = packet->get_header();
	all::core::uint8_sarr header_buffer = packet_header.get_header();
	
	m_out_header_buffer.assign(reinterpret_cast <const char*> (header_buffer.get()), net_packet_header_t::HEADER_LENGTH);

	m_out_data_buffer.assign(packet->get_buffer().get(), packet_header.get_packet_size());

	//printf("packet data: %s\n", m_out_data_buffer.c_str());

	std::vector<boost::asio::const_buffer> buffers;
    buffers.push_back(boost::asio::buffer(m_out_header_buffer));
	buffers.push_back(boost::asio::buffer(m_out_data_buffer));
	boost::asio::write(m_socket, buffers);
}

}}} //namespaces
