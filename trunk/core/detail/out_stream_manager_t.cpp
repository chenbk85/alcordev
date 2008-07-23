#define WIN32_LEAN_AND_MEAN
#include "out_stream_manager_t.hpp"

namespace all {
	namespace core {
		namespace detail {

out_stream_manager_t::out_stream_manager_t(boost::asio::ip::udp::socket& socket):
													m_socket(socket),
													m_packet_sender(m_socket),
													m_timer(m_socket.io_service())
{
	m_frame_number = 0;
	m_packet_number = 0;
	m_curr_data = NULL;
}

out_stream_manager_t::~out_stream_manager_t() {
	if (m_curr_data) 
		delete [] m_curr_data;
}

void out_stream_manager_t::set_packet_size(int packet_size) {
	m_packet_size = packet_size;
	max_packet_data_size = packet_size - stream_packet_t::HEADER_LENGTH;
}

void out_stream_manager_t::set_multicast_address(all::core::ip_address_t multicast_addr) {
	boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string(multicast_addr.hostname), multicast_addr.port);
	m_packet_sender.set_endpoint(endpoint);

}

void out_stream_manager_t::set_get_data_callback(boost::function <int (all::core::uint8_ptr*)> get_data_callback) {
	m_get_data_cb = get_data_callback;
}

void out_stream_manager_t::set_frame_rate(int frame_rate) {
	m_timer_freq = 1000 / frame_rate;
}

int out_stream_manager_t::get_packet_size() {
	return m_packet_size;
}

void out_stream_manager_t::start_streaming() {

	m_timer.expires_from_now(boost::posix_time::millisec(500));
	m_timer.async_wait(boost::bind(&out_stream_manager_t::next_frame_handler, this,
								   boost::asio::placeholders::error));
}

void out_stream_manager_t::stop_streaming() {
	m_timer.cancel();
}

void out_stream_manager_t::send_data() {
	
	
	int packet_number = 1;
	int frame_number = m_frame_number;
	
	stream_packet_ptr_t packet;

	all::core::uint8_t *data, *data_pos;
	data = new all::core::uint8_t[m_curr_data_size];
	memcpy(data, m_curr_data, m_curr_data_size);
	
	data_pos = data;
	int remaining_bytes = m_curr_data_size;
	
	int bytes_to_send;
	while (remaining_bytes > 0) {
		packet.reset(new stream_packet_t());
		
		if (remaining_bytes > max_packet_data_size)
			bytes_to_send = max_packet_data_size;
		else
			bytes_to_send = remaining_bytes;

		packet->set_frame_number(frame_number);
		packet->set_packet_number(packet_number);
		packet->set_data(data_pos, bytes_to_send);
		packet->finalize_packet();

		//m_packet_sender.send_packet(packet);
		
		m_packet_sender.async_send_packet(packet);
		//printf("send packet n %i, size %i\n", packet_number, bytes_to_send);
		data_pos += bytes_to_send;
		remaining_bytes -= bytes_to_send;
		packet_number++;
		
	}

	delete[] data;

}

void out_stream_manager_t::next_frame_handler(const boost::system::error_code& error) {
	
	if (!error) {

		m_frame_number = (m_frame_number+1) % MAX_FRAME_NUMBER;

		if (m_curr_data) 
			delete[] m_curr_data;
		
		m_curr_data_size = m_get_data_cb(&m_curr_data);

		//printf("data_size: %i\n", m_curr_data_size);
		
		//m_send_data_thread.reset(new boost::thread(boost::bind(&out_stream_manager_t::send_data, this)));
		send_data();

		m_timer.expires_from_now(boost::posix_time::millisec(m_timer_freq));
		m_timer.async_wait(boost::bind(&out_stream_manager_t::next_frame_handler, this,
									   boost::asio::placeholders::error));

	}
}

}}} //namespaces

