#include "tcp_pkt_receiver_t.hpp"

tcp_pkt_receiver_t::tcp_pkt_receiver_t(boost::asio::ip::tcp::socket& socket) : m_socket(socket) {
	m_in_data_buffer = NULL;
	m_listen_f = false;
	m_interrupt_f = false;
}

tcp_pkt_receiver_t::~tcp_pkt_receiver_t() {
	if (m_in_data_buffer != NULL)
		delete [] m_in_data_buffer;
}

void tcp_pkt_receiver_t::set_read_callback(boost::function <void (net_packet_ptr_t)> read_callback) {
	m_read_callback = read_callback;
}

void tcp_pkt_receiver_t::set_error_callback(boost::function <void (const boost::system::error_code&)> error_callback) {
	m_error_callback = error_callback;
}

void tcp_pkt_receiver_t::start_listen() {
	m_listen_f = true;
	read_packet();
}

void tcp_pkt_receiver_t::stop_listen() {
	m_listen_f = false;
	interrupt_reading();
}

void tcp_pkt_receiver_t::interrupt_reading() {
	//m_interrupt_f = true;
	//typedef boost::asio::streambuf::mutable_buffers_type mutable_buffers_type;
	//mutable_buffers_type bufs = m_in_pkt_buffer.prepare(net_packet_t::END_SEQUENCE.size());
	//boost::asio::detail::const_buffers_iterator<mutable_buffers_type> it(bufs,0);
	//*it = net_packet_t::END_SEQUENCE[0];
	//m_in_pkt_buffer.commit(net_packet_t::END_SEQUENCE.size());
}

net_packet_ptr_t tcp_pkt_receiver_t::get_last_packet() {
	return m_packet_ptr;
}


void tcp_pkt_receiver_t::read_packet() {
	printf("waiting packet...\n");
	boost::asio::async_read_until(m_socket, m_in_pkt_buffer, boost::regex(net_packet_t::END_SEQUENCE), 
								  boost::bind (&tcp_pkt_receiver_t::handle_read_data, this,
											   boost::asio::placeholders::error,
											   boost::asio::placeholders::bytes_transferred));
}

void tcp_pkt_receiver_t::handle_read_data(const boost::system::error_code& error, std::size_t bytes_transferred) {
	if (!error) {
		
		printf("read %i bytes\n", bytes_transferred);
		
		std::istream is(&m_in_pkt_buffer);

		//check header size
		if (bytes_transferred < net_packet_header_t::HEADER_LENGTH) {
			
			if (m_listen_f)
				read_packet();

			if (m_error_callback) {
				boost::system::error_code data_error(boost::asio::error::no_data);
				m_error_callback(data_error);
			}

			return;

		}
		
		//read packet header
		is.read(m_in_header_buffer, net_packet_header_t::HEADER_LENGTH);

		net_packet_header_t header;

		//try build packet header
		if (!header.build_header_from_string(m_in_header_buffer)) {
			
			if (m_listen_f)
				read_packet();

			if (m_error_callback) {
				boost::system::error_code data_error(boost::asio::error::no_data);
				m_error_callback(data_error);
			}

			return;
		}
		
		//check packet size consistency
		std::size_t data_size = bytes_transferred - net_packet_header_t::HEADER_LENGTH;
		if (data_size != header.get_packet_size()) {
			
			if (m_listen_f)
				read_packet();

			if (m_error_callback) {
				boost::system::error_code data_error(boost::asio::error::no_data);
				m_error_callback(data_error);
			}

			return;
		}
		
		//read packet data
		if (m_in_data_buffer != NULL)
			delete [] m_in_data_buffer;
		
		m_in_data_buffer = new char[bytes_transferred];
		is.read(m_in_data_buffer, data_size);
		
		//build packet
		m_packet_ptr.reset(new net_packet_t(header, m_in_data_buffer));
		

		//start waiting for next packet
		if (m_listen_f)
			//async->returns immediatly
			read_packet();

		//invoke read callback
		if (m_read_callback) 
			m_read_callback(m_packet_ptr);
	}
	else {
		//printf("error in tcp_pkt_receiver_t::handle_read_data: \n%s", error.what());
		if (m_error_callback)
			m_error_callback(error);
	}
}
