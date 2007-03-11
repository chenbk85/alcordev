#include "stream_pkt_receiver_t.hpp"

namespace all {
	namespace core {
		namespace detail {


stream_pkt_receiver_t::stream_pkt_receiver_t(boost::asio::ip::udp::socket& socket) : m_socket(socket)
{
	m_in_data_buffer = NULL;
	m_listen_f = false;
	m_interrupt_f = false;
}

stream_pkt_receiver_t::~stream_pkt_receiver_t() {
	if (m_in_data_buffer != NULL)
		delete [] m_in_data_buffer;
}

void stream_pkt_receiver_t::set_packet_size(std::size_t packet_size) {
	m_packet_size = packet_size;
	
	if (m_in_data_buffer)
		delete[] m_in_data_buffer;

	m_in_data_buffer = new char[m_packet_size];
}


void stream_pkt_receiver_t::set_read_callback(boost::function <void (stream_packet_ptr_t)> read_callback) {
	m_read_callback = read_callback;
}

void stream_pkt_receiver_t::set_error_callback(boost::function <void (const boost::system::error_code&)> error_callback) {
	m_error_callback = error_callback;
}

void stream_pkt_receiver_t::start_listen() {
	m_listen_f = true;
	read_packet();
}

void stream_pkt_receiver_t::stop_listen() {
	m_listen_f = false;
	interrupt_reading();
}

void stream_pkt_receiver_t::interrupt_reading() {

}

stream_packet_ptr_t stream_pkt_receiver_t::get_last_packet() {
	return m_packet_ptr;
}


void stream_pkt_receiver_t::read_packet() {
	//printf("waiting packet...\n");
	//m_socket.async_receive_from(boost::asio::buffer(m_in_data_buffer, m_packet_size), m_remote_endpoint,  
	//							  boost::bind (&stream_pkt_receiver_t::handle_read_data, this,
	//										   boost::asio::placeholders::error,
	//										   boost::asio::placeholders::bytes_transferred));
	m_socket.async_receive(boost::asio::buffer(m_in_data_buffer, m_packet_size),  
								  boost::bind (&stream_pkt_receiver_t::handle_read_data, this,
											   boost::asio::placeholders::error,
											   boost::asio::placeholders::bytes_transferred));
}

void stream_pkt_receiver_t::handle_read_data(const boost::system::error_code& error, std::size_t bytes_transferred) {
	if (!error) {
		
		//printf("read %i bytes\n", bytes_transferred);
		
		m_packet_ptr.reset(new stream_packet_t());

		//build packet
		if (!m_packet_ptr->build_packet_from_string(m_in_data_buffer, bytes_transferred)) {
			
			printf("Error building packet from socket\n");
			
			//start waiting for next packet
			if (m_listen_f)
			//async->returns immediatly
				read_packet();
			
			if (m_error_callback)
				m_error_callback(boost::asio::error::no_data);

		}
		//invoke read callback
		else { 
			//start waiting for next packet
			if (m_listen_f)
				//async->returns immediatly
				read_packet();
			
			if (m_read_callback) 
				m_read_callback(m_packet_ptr);
		}
	}
	else {
		if (m_error_callback)
			m_error_callback(error);
	}
}

}}} //namespaces