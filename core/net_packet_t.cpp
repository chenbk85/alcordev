#include "net_packet_t.hpp"
#include <iostream>

namespace all {
	namespace core {

const unsigned short net_packet_header_t::MAX_ID = std::numeric_limits<unsigned short>::max();
const std::string net_packet_t::END_SEQUENCE = "#end#";
unsigned short net_packet_header_t::next_id = 0;

net_packet_header_t::net_packet_header_t() : m_header(new all::core::uint8_t[net_packet_header_t::HEADER_LENGTH]) {

	m_write_ptr = m_header.get();
	m_read_ptr = m_header.get();

	m_packet_type = RAW_PACKET;
	m_packet_size = 0;
}


net_packet_header_t::~net_packet_header_t() {

}

bool net_packet_header_t::build_header() {
	
	//assign an id to packet
	m_packet_id = net_packet_header_t::next_id;
	
	//update next_id for next packet
	net_packet_header_t::next_id = (net_packet_header_t::next_id + 1) % MAX_ID;

	memcpy(m_write_ptr, &m_packet_id, sizeof(m_packet_id));
	m_write_ptr += sizeof(m_packet_id);
	memcpy(m_write_ptr, &m_packet_type, sizeof(m_packet_type));
	m_write_ptr += sizeof(m_packet_type);
	memcpy(m_write_ptr, &m_packet_size, sizeof(m_packet_size));

	return true;
}

bool net_packet_header_t::build_header_from_buffer(const char* header) {
	
	memcpy(m_header.get(), header, net_packet_header_t::HEADER_LENGTH);
	
	memcpy(&m_packet_id, m_read_ptr, sizeof(m_packet_id));
	m_read_ptr+= sizeof(m_packet_id);
	memcpy(&m_packet_type, m_read_ptr, sizeof(m_packet_type));
	m_read_ptr+=sizeof(m_packet_type);
	memcpy(&m_packet_size, m_read_ptr, sizeof(m_packet_size));

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

const all::core::uint8_sarr net_packet_header_t::get_header() const{
	return m_header;
}



const std::size_t net_packet_t::DATA_BUFFER_INCR_SIZE = 100;
std::size_t net_packet_t::INIT_DATA_BUFFER_SIZE = 100;

net_packet_t::net_packet_t() {
	m_data_buffer_size = INIT_DATA_BUFFER_SIZE;
	m_data = new char[m_data_buffer_size];
	m_write_ptr = m_data;
	m_req_freq = 0;
	m_data_size = 0;
}

net_packet_t::net_packet_t(net_packet_header_t header, const char* buffer, std::size_t buffer_size): 
																m_header(header)
																		
{
	net_packet_type packet_type = m_header.get_packet_type();

	m_buffer_size = buffer_size;
	m_data_size = m_buffer_size;

	m_buffer.reset(new char[m_buffer_size]);
	memcpy(m_buffer.get(), buffer, buffer_size);

	m_write_ptr = m_buffer.get();
	
	if (packet_type == COMMAND_PACKET || packet_type == ANSWER_PACKET || packet_type == REQUEST_PACKET) {

		m_command.assign(m_write_ptr);
		m_write_ptr += (m_command.size() +1);
		m_data_size -= (m_command.size() +1);

		if (packet_type == REQUEST_PACKET) {
			memcpy(&m_req_freq, m_write_ptr, sizeof(m_req_freq));
			m_write_ptr += sizeof(m_req_freq);
			m_data_size -= sizeof(m_req_freq);
		}
		
	}
	
	m_data_buffer_size = m_data_size;
	m_data = new char[m_data_buffer_size];
	memcpy(m_data, m_write_ptr, m_data_size);

	m_read_ptr = m_data;

}

net_packet_t::~net_packet_t() {
	if (m_data)
		delete[] m_data;
}

void net_packet_t::set_command(std::string command) {
	m_command = command;
}

void net_packet_t::set_req_freq(int freq) {
	m_req_freq = freq;
}

void net_packet_t::set_packet_type(net_packet_type type) {
	m_header.set_packet_type(type);
}

void net_packet_t::int_to_buf(int i) {
	std::size_t byte_to_write = sizeof(int);
	while ((m_data_size + byte_to_write) > m_data_buffer_size)
		resize_data_buffer();
	memcpy(m_write_ptr, &i, byte_to_write);
	m_write_ptr+=byte_to_write;
	m_data_size += byte_to_write;
}

void net_packet_t::double_to_buf(double d) {
	std::size_t byte_to_write = sizeof(double);
	while ((m_data_size + byte_to_write) > m_data_buffer_size)
		resize_data_buffer();
	memcpy(m_write_ptr, &d, byte_to_write);
	m_write_ptr+=byte_to_write;
	m_data_size += byte_to_write;
}

void net_packet_t::string_to_buf(std::string s) {
	std::size_t byte_to_write = s.size() + 1;
	while ((m_data_size + byte_to_write) > m_data_buffer_size)
		resize_data_buffer();
	strcpy(m_write_ptr, s.c_str());
	m_write_ptr += byte_to_write;
	m_data_size += byte_to_write;
}

void net_packet_t::array_to_buf(all::core::uint8_ptr arr, std::size_t arr_size)  {
	std::size_t byte_to_write = arr_size + sizeof(std::size_t);
	while ((m_data_size + byte_to_write) > m_data_buffer_size)
		resize_data_buffer();
	memcpy(m_write_ptr, &arr_size, sizeof(std::size_t));
	m_write_ptr += sizeof(std::size_t);
	memcpy(m_write_ptr, arr, arr_size);
	m_write_ptr += arr_size;
	m_data_size += byte_to_write;
}


int net_packet_t::buf_to_int() {
	int i;
	memcpy(&i, m_read_ptr, sizeof(int));
	m_read_ptr += sizeof(int);
	return i;
}

double net_packet_t::buf_to_double() {
	double d;
	memcpy(&d, m_read_ptr, sizeof(double));
	m_read_ptr += sizeof(double);
	return d;
}

std::string net_packet_t::buf_to_string() {
	
	std::string s(m_read_ptr);
	m_read_ptr += s.size() + 1;
	return s;

}

std::size_t net_packet_t::buf_to_array(all::core::uint8_ptr& arr) {
	std::size_t arr_size;
	memcpy(&arr_size, m_read_ptr, sizeof(std::size_t));
	m_read_ptr += sizeof(std::size_t);
	arr = new all::core::uint8_t[arr_size];
	memcpy(arr, m_read_ptr, arr_size);
	m_read_ptr += arr_size;
	return arr_size;
}
bool net_packet_t::finalize_packet() {
	
	//fill packet buffer
	net_packet_type packet_type = m_header.get_packet_type();

	if (packet_type == COMMAND_PACKET || packet_type == ANSWER_PACKET) {
        m_buffer_size = m_data_size + m_command.size() + 1 + END_SEQUENCE.size();
		m_buffer.reset(new char[m_buffer_size]);
		
		m_write_ptr = m_buffer.get();
		
		strcpy(m_write_ptr, m_command.c_str());
		m_write_ptr += (m_command.size()+1);
	}
	else if (packet_type == REQUEST_PACKET)  {
		m_buffer_size = m_data_size + m_command.size() + 1 + sizeof(m_req_freq) + END_SEQUENCE.size();
		m_buffer.reset(new char[m_buffer_size]);
		
		m_write_ptr = m_buffer.get();
		
		strcpy(m_write_ptr, m_command.c_str());
		m_write_ptr += (m_command.size()+1);
		
		memcpy(m_write_ptr, &m_req_freq, sizeof(m_req_freq));
		m_write_ptr += sizeof(m_req_freq);

	}
	else  {
		m_buffer_size = m_data_size + END_SEQUENCE.size();
		m_buffer.reset(new char[m_buffer_size]);
		m_write_ptr = m_buffer.get();
	}
	
	memcpy(m_write_ptr, m_data, m_data_size);
	m_write_ptr += m_data_size;
	END_SEQUENCE.copy(m_write_ptr, END_SEQUENCE.size());

	//build packet header
	m_header.set_packet_size(m_buffer_size);

	m_header.build_header();

	return true;
}

std::string net_packet_t::get_command() const {
	return m_command;
}


int net_packet_t::get_req_freq() const {
	return m_req_freq;
}


const boost::shared_array<char> net_packet_t::get_buffer() const {
	return m_buffer;
}

const net_packet_header_t& net_packet_t::get_header() const{
	return m_header;
}


void net_packet_t::resize_data_buffer() {
	m_data_buffer_size += net_packet_t::DATA_BUFFER_INCR_SIZE;
	
	char* new_buffer = new char[m_data_buffer_size];
	memcpy(new_buffer, m_data, m_data_size);
	delete[] m_data;
	m_data = new_buffer;

	net_packet_t::INIT_DATA_BUFFER_SIZE = m_data_buffer_size;
}

}} //namespaces