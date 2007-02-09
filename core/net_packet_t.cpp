#include "net_packet_t.hpp"
#include <iostream>

namespace all {
	namespace core {

const unsigned short net_packet_header_t::MAX_ID = std::numeric_limits<unsigned short>::max();
const std::string net_packet_t::END_SEQUENCE = "#end#";
unsigned short net_packet_header_t::next_id = 0;

net_packet_header_t::net_packet_header_t() {

	m_header.clear();

	m_packet_type = RAW_PACKET;
	m_packet_size = 0;
}


bool net_packet_header_t::build_header_from_packet() {
	
	//assign an id to packet
	m_packet_id = net_packet_header_t::next_id;
	
	//update next_id for next packet
	net_packet_header_t::next_id = (net_packet_header_t::next_id + 1) % MAX_ID;
	
	try {
		m_header += boost::lexical_cast <std::string> (m_packet_id);
		m_header += FIELD_SEPARATOR;
		m_header += boost::lexical_cast <std::string> (static_cast<int>(m_packet_type));
		m_header += FIELD_SEPARATOR;
		m_header += boost::lexical_cast <std::string>(m_packet_size);
		m_header += FIELD_SEPARATOR;
		m_header.resize(HEADER_LENGTH);
	}
	catch (boost::bad_lexical_cast b) {
		printf("Error in build_header_from_packet: \n %s", b.what());
		return false;
	}
	return true;
}

bool net_packet_header_t::build_header_from_string(std::string header) {
	
	m_header = header;
	m_header.resize(HEADER_LENGTH);
	
	try {
		//read packet id
		std::string::size_type start_pos = 0;
		std::string::size_type end_pos = m_header.find(FIELD_SEPARATOR, 0);
		std::string::size_type length = end_pos - start_pos;
		
		std::string value_str = m_header.substr(start_pos, length);

		m_packet_id = boost::lexical_cast <unsigned short> (value_str);
	
		//read packet type
		start_pos += (length+1);
		end_pos = m_header.find(FIELD_SEPARATOR, start_pos);
		length = end_pos - start_pos;

		value_str = m_header.substr(start_pos, length);

		m_packet_type = static_cast <net_packet_type> (boost::lexical_cast <int> (value_str));

		//read packet length
		start_pos += (length + 1);
		end_pos = m_header.find(FIELD_SEPARATOR, start_pos);
		length = end_pos - start_pos;

		value_str = m_header.substr(start_pos, length);

		m_packet_size = boost::lexical_cast <int> (value_str);
	}
	catch (boost::bad_lexical_cast b) {
		printf("Error in build_header_from_string: \n %s", b.what());
		return false;
	}
	return true;
}

void net_packet_header_t::set_packet_type(net_packet_type type) {
	m_packet_type = type;
}

void net_packet_header_t::set_packet_size(std::size_t size) {
	m_packet_size = size;
}

unsigned short int net_packet_header_t::get_packet_id() const{
	return m_packet_id;
}

std::size_t net_packet_header_t::get_packet_size() const{
	return m_packet_size;
}

net_packet_type net_packet_header_t::get_packet_type() const{
	return m_packet_type;
}

std::string net_packet_header_t::get_header() const{
	return m_header;
}


net_packet_t::net_packet_t() {
	finalized = false;
	m_data.clear();
	m_command.clear();
	m_buffer.clear();
}

net_packet_t::net_packet_t(net_packet_header_t header, std::string buffer): 
																m_header(header),
																m_buffer(buffer)
																		
{
	net_packet_type packet_type = m_header.get_packet_type();
	
	if (packet_type == COMMAND_PACKET || packet_type == ANSWER_PACKET || packet_type == REQUEST_PACKET) {
        std::string::size_type end_pos = m_buffer.find(DATA_SEPARATOR, 0);
		std::string::size_type length = end_pos;
		
		m_command = m_buffer.substr(0, length);

		if (packet_type == REQUEST_PACKET) {
			std::string::size_type start_pos = length+1;
			end_pos = m_buffer.find(DATA_SEPARATOR, start_pos);
			length = end_pos - start_pos;
			m_req_freq = m_buffer.substr(start_pos, length);
		}

		m_data = m_buffer.substr(end_pos+1);
	}
	else {
		m_data = m_buffer;
	}
	
	finalized = true;
	m_read_pos = 0;

}

void net_packet_t::set_command(std::string command) {
	m_command = command + DATA_SEPARATOR;
}

void net_packet_t::set_req_freq(int freq) {
	m_req_freq = boost::lexical_cast <std::string> (freq);
	m_req_freq += DATA_SEPARATOR;
	printf("freq: %s\n", m_req_freq.c_str());
}

void net_packet_t::set_packet_type(net_packet_type type) {
	m_header.set_packet_type(type);
}

void net_packet_t::int_to_buf(int i) {
	m_data+=boost::lexical_cast <std::string> (i);
	m_data+=DATA_SEPARATOR;
}

void net_packet_t::double_to_buf(double d) {
	m_data+=boost::lexical_cast <std::string> (d);
	m_data+=DATA_SEPARATOR;
}

void net_packet_t::string_to_buf(std::string s) {
	m_data+=s;
	m_data+=DATA_SEPARATOR;
}

int net_packet_t::buf_to_int() {
	if (finalized) {
		std::string::size_type end_pos = m_data.find(DATA_SEPARATOR, m_read_pos);
		std::string::size_type length = end_pos - m_read_pos;
		
		std::string int_str = m_data.substr(m_read_pos, length);
		try {
			int i = boost::lexical_cast <int> (int_str);
			m_read_pos += (length+1);
			return i;
		}
		catch (boost::bad_lexical_cast b) {
			printf("error in net_packet_t::buf_to_int\n %s\n", b.what());
			return 0;
		}
	}
	else
		return 0;
}

double net_packet_t::buf_to_double() {
	if (finalized) {
		std::string::size_type end_pos = m_data.find(DATA_SEPARATOR, m_read_pos);
		std::string::size_type length = end_pos - m_read_pos;

		std::string double_str = m_data.substr(m_read_pos, length);
		try {
			double d = boost::lexical_cast <double> (double_str);
			m_read_pos += (length+1);
			return d;
		}
		catch (boost::bad_lexical_cast b) {
			printf("error in net_packet_t::buf_to_double\n %s\n", b.what());
			return 0;
		}
	}
	else
		return 0;
}

std::string net_packet_t::buf_to_string() {
	if (finalized) {
		std::string::size_type end_pos = m_data.find(DATA_SEPARATOR, m_read_pos);
		std::string::size_type length = end_pos - m_read_pos;
		

		std::string s = m_data.substr(m_read_pos, length);
		m_read_pos += length+1;

		return s;
	}
	else
		return 0;
}


bool net_packet_t::finalize_packet() {
	
	//fill packet buffer
	net_packet_type packet_type = m_header.get_packet_type();
	if (packet_type == COMMAND_PACKET || packet_type == ANSWER_PACKET)
        m_buffer = m_command;
	else if (packet_type == REQUEST_PACKET)
		m_buffer = m_command + m_req_freq;
	
	m_buffer += m_data;
	m_buffer += END_SEQUENCE;

	//build packet header
	m_header.set_packet_size(m_buffer.size());

	if (m_header.build_header_from_packet()) {
		finalized = true;
		m_read_pos = 0;
	}
	else {
		finalized = false;
	}
	return finalized;
}

std::string net_packet_t::get_command() const {
	return m_command;
}

std::string net_packet_t::get_req() const {
	return m_req_freq;
}

int net_packet_t::get_req_freq() const {
	return boost::lexical_cast <int> (m_req_freq);
}

std::string net_packet_t::get_data() const{
	return m_data;
}

std::string net_packet_t::get_buffer() const {
	return m_buffer;
}

const net_packet_header_t& net_packet_t::get_header() const{
	return m_header;
}

}} //namespaces