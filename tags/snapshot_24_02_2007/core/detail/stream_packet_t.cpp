#include "stream_packet_t.hpp"

namespace all {
	namespace core {
		namespace detail {


stream_packet_t::stream_packet_t() {
	m_header.clear();
	m_buffer.clear();
	m_data = NULL;
}

stream_packet_t::~stream_packet_t() {
	if (m_data)
		delete m_data;
}


bool stream_packet_t::build_packet_from_string(char* data, int size) {
	try {
		//read data size
		std::string buffer(data, size);

		std::string::size_type start_pos = 0;
		std::string::size_type end_pos = buffer.find(FIELD_SEPARATOR, 0);
		std::string::size_type length = end_pos - start_pos;
		
		std::string value_str = buffer.substr(start_pos, length);

		m_data_size = boost::lexical_cast <std::size_t> (value_str);
	
		//read frame number
		start_pos += (length+1);
		end_pos = buffer.find(FIELD_SEPARATOR, start_pos);
		length = end_pos - start_pos;

		value_str = buffer.substr(start_pos, length);

		m_frame_number = boost::lexical_cast <int> (value_str);

		//read packet number
		start_pos += (length + 1);
		end_pos = buffer.find(FIELD_SEPARATOR, start_pos);
		length = end_pos - start_pos;

		value_str = buffer.substr(start_pos, length);

		m_packet_number = boost::lexical_cast <int> (value_str);

		//read data
		value_str = buffer.substr(HEADER_LENGTH);

		if (value_str.size() != m_data_size) {
			printf("%i %i\n", buffer.size(), m_data_size);
			printf("packet length error\n");
			return false;
		}
		
		if (m_data)
			delete[] m_data;

		m_data = new all::core::uint8_t[m_data_size];
		//m_data = reinterpret_cast <all::core::uint8_t*> (boost::lexical_cast <char*> (value_str));
		value_str.copy(reinterpret_cast <char*> (m_data), m_data_size);


	}
	catch (boost::bad_lexical_cast b) {
		printf("Error in build_packet_from_string: \n %s", b.what());
		return false;
	}
	return true;
}


void stream_packet_t::set_frame_number(int frame) {
	m_frame_number = frame;
}

void stream_packet_t::set_packet_number(int packet) {
	m_packet_number = packet;
}

void stream_packet_t::set_data(all::core::uint8_t* data, std::size_t size) {
	m_data = new all::core::uint8_t[size];
	memcpy(m_data, data, size);
	m_data_size = size;
}

bool stream_packet_t::finalize_packet() {
	m_header += boost::lexical_cast <std::string> (m_data_size);
	m_header += FIELD_SEPARATOR;
	m_header += boost::lexical_cast <std::string> (m_frame_number);
	m_header += FIELD_SEPARATOR;
	m_header += boost::lexical_cast <std::string> (m_packet_number);
	m_header += FIELD_SEPARATOR;

	m_header.resize(HEADER_LENGTH);

	std::string data_string(reinterpret_cast <char*> (m_data), m_data_size);

	m_buffer = m_header + data_string;

	return true;
}

int stream_packet_t::get_frame_number() const {
	return m_frame_number;
}

int stream_packet_t::get_packet_number() const {
	return m_packet_number;
}

all::core::uint8_t* stream_packet_t::get_data() const {
	all::core::uint8_t* return_data = new all::core::uint8_t[m_data_size];
	memcpy(return_data, m_data, m_data_size);
	return return_data;
}

const all::core::uint8_t* stream_packet_t::get_data_ptr() const {
	return m_data;
}

std::size_t stream_packet_t::get_data_size() const {
	return m_data_size;
}

std::string stream_packet_t::get_buffer() const {
	return m_buffer;
}

}}} //namespaces