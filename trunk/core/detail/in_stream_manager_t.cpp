#include "in_stream_manager_t.hpp"

namespace all {
	namespace core {
		namespace detail {

const std::size_t in_stream_manager_t::DATA_BUFFER_INCR_SIZE = 50000;

in_stream_manager_t::in_stream_manager_t(boost::asio::ip::udp::socket& socket): 
															m_socket(socket),
															m_packet_receiver(m_socket)
															
																				
{
	m_last_frame_num = 0;
	m_last_frame_pos = 0;
	
	m_packet_receiver.set_read_callback(boost::bind(&in_stream_manager_t::in_packet_cb, this,_1));
	m_packet_receiver.set_error_callback(boost::bind(&in_stream_manager_t::in_packet_error_cb, this, _1));
	m_max_data_size = DATA_BUFFER_INCR_SIZE;
}

void in_stream_manager_t::start_receive() {
	m_packet_receiver.start_listen();
}

void in_stream_manager_t::stop_receive() {
	m_packet_receiver.stop_listen();
}

void in_stream_manager_t::in_packet_cb(stream_packet_ptr_t packet) {
	
	int frame_num = packet->get_frame_number();
	
	int frame_buffer_pos = -1;
	for (int i=0; i<m_frame_buffer_size; i++) {
		if (m_frame_buffer_map[i] == frame_num)
			frame_buffer_pos = i;
	}
	
	//frame it's not in the frame buffer
	if (frame_buffer_pos == -1) {
		
		//printf("Frame %i not in buffer...\n", frame_num);
	
		//if it's the first packet of a new frame...
		if ( (frame_num > m_last_frame_num) || ( (frame_num == 1) && (frame_num != m_last_frame_num) )) {
		
			//printf("First packet of a new frame...\n");

			int ready_frame_pos = (m_last_frame_pos + 1) % m_frame_buffer_size;
			int ready_frame_size = m_frame_size[ready_frame_pos];
		
			if (ready_frame_size > 0) {
				
				printf("Frame ready...\n");
				printf("Frame size: %i\n", ready_frame_size);
				printf("Frame pos: %i\n", ready_frame_pos);

				all::core::uint8_ptr new_frame = new all::core::uint8_t[ready_frame_size];
				memcpy(new_frame, m_frame_buffer[ready_frame_pos], ready_frame_size);
				all::core::uint8_sarr new_frame_sarr(new_frame);
				
				m_data_ready_cb(new_frame_sarr, ready_frame_size);
			}

			m_last_frame_num = frame_num;
			m_last_frame_pos = ready_frame_pos;
			
			frame_buffer_pos = m_last_frame_pos;

			m_frame_size[frame_buffer_pos] = 0;

			m_frame_buffer_map[frame_buffer_pos] = frame_num;
		}
	}

	if (frame_buffer_pos != -1) {
        
		int packet_num = packet->get_packet_number();
		int packet_data_size = packet->get_data_size();

		//printf("Packet number %i\n", packet_num);
		
		//find last byte position of current data in the frame
		int last_frame_byte = (m_packet_data_size * (packet_num-1)) + packet_data_size;

		//resize frame if necessary
		while (last_frame_byte > m_max_data_size) {
			resize_data_buffer();
		}

		//update frame size
		if (last_frame_byte > m_frame_size[frame_buffer_pos]) 
			m_frame_size[frame_buffer_pos] = last_frame_byte;
			
		//find the position of the current data in the frame
		all::core::uint8_ptr frame_pos = m_frame_buffer[frame_buffer_pos] + (m_packet_data_size * (packet_num-1));

		//copy data in the frame
		memcpy(frame_pos, packet->get_data_ptr(), packet_data_size);
	}
	else {
		printf("Packet dropped\n");
	}

}

void in_stream_manager_t::in_packet_error_cb(const boost::system::error_code& error) {

}

void in_stream_manager_t::set_frame_buffer(std::size_t buffer_size) {
	m_frame_buffer_size = buffer_size;
	m_frame_buffer.clear();
	for (int i=0; i < buffer_size; i++) {
		m_frame_buffer.push_back(new all::core::uint8_t[m_max_data_size]);
		m_frame_size.push_back(0);
		m_frame_buffer_map.push_back(0);
	}
}


void in_stream_manager_t::resize_data_buffer() {
	
	printf("Resizing data buffer...\n");
	
	std::size_t old_data_size = m_max_data_size;
	m_max_data_size += DATA_BUFFER_INCR_SIZE;
	
	for (int i=0; i<m_frame_buffer_size; i++) {
		//create a new frame bigger!
		all::core::uint8_t* new_frame = new all::core::uint8_t[m_max_data_size];
		
		memcpy(new_frame, m_frame_buffer[i], old_data_size);
		
		delete[] m_frame_buffer[i];
		
		m_frame_buffer[i] = new_frame;

	}
}

void in_stream_manager_t::set_packet_size(std::size_t packet_size) {
	m_packet_data_size = packet_size - stream_packet_t::HEADER_LENGTH;
	m_packet_receiver.set_packet_size(packet_size);
}

void in_stream_manager_t::set_data_ready_callback(boost::function <void (all::core::uint8_sarr, std::size_t )> data_ready_callback) {
	m_data_ready_cb = data_ready_callback;
}

}}} //namespaces