#ifndef net_packet_t_H_INCLUDED
#define net_packet_t_H_INCLUDED

#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <limits>

enum net_packet_type {
		COMMAND_PACKET,
		REQUEST_PACKET,
		ANSWER_PACKET,
		ALIVE_PACKET,
		RAW_PACKET
};

class net_packet_header_t {
public:
	
	static const int HEADER_LENGTH = 16;
	static const unsigned short MAX_ID;
	static const char FIELD_SEPARATOR = '_';

	static unsigned short next_id;
	
	net_packet_header_t();

	void set_packet_type(net_packet_type);
	void set_packet_size(std::size_t);
	
	bool build_header_from_packet();
	bool build_header_from_string(std::string);
	
	unsigned short int get_packet_id() const;
	net_packet_type get_packet_type() const;
	std::size_t get_packet_size() const;
	
	std::string get_header() const;
	

private:
	std::size_t m_packet_size;
	net_packet_type m_packet_type;
	unsigned short int m_packet_id;
	
	std::string m_header;
};

class net_packet_t {

public:

	static const char DATA_SEPARATOR = '_';
	static const std::string END_SEQUENCE;
	
	net_packet_t();

	//for packet construction from socket data...
	net_packet_t(net_packet_header_t, std::string);

	void set_command(std::string);
	void set_req_freq(int);

	void set_packet_type(net_packet_type);
	void set_packet_size();
	
	void int_to_buf(int);
	void double_to_buf(double);
	void string_to_buf(std::string);

	int buf_to_int();
	double buf_to_double();
	std::string buf_to_string();
	
	bool finalize_packet();

	int get_req_freq() const;
	std::string get_req() const;

	std::string get_command() const;
	std::string get_data() const;
	std::string get_buffer() const;
	const net_packet_header_t& get_header() const;

protected:

	net_packet_header_t m_header;
	
	std::string m_command;
	std::string m_req_freq;
	
	std::string m_data;

	std::string m_footer;

	std::string m_buffer;

	bool finalized;
	std::string::size_type m_read_pos;
};

typedef boost::shared_ptr <net_packet_t> net_packet_ptr_t;

#endif
