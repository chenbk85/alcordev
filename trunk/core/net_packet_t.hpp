#ifndef net_packet_t_H_INCLUDED
#define net_packet_t_H_INCLUDED

#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include <alcor/core/core.h>

#include <vector>
#include <limits>


namespace all {
	namespace core {

enum net_packet_type {
		COMMAND_PACKET,
		REQUEST_PACKET,
		ANSWER_PACKET,
		ALIVE_PACKET,
		RAW_PACKET,
		APPL_PACKET
};

class net_packet_header_t {

public:
	
	static const int HEADER_LENGTH = 16;
	static const unsigned short MAX_ID;

	static unsigned short next_id;
	
	net_packet_header_t();
	~net_packet_header_t();

	void set_packet_type(net_packet_type);
	void set_packet_size(std::size_t);
	
	bool build_header();
	bool build_header_from_buffer(const char*);
	
	unsigned short int get_packet_id() const;
	net_packet_type get_packet_type() const;
	std::size_t get_packet_size() const;
	
	const all::core::uint8_sarr get_header() const;
	

private:

	std::size_t m_packet_size;
	net_packet_type m_packet_type;
	unsigned short int m_packet_id;
	
	all::core::uint8_sarr m_header;

	all::core::uint8_ptr m_write_ptr;
	all::core::uint8_ptr m_read_ptr;
};

class net_packet_t {

public:

	static const std::string END_SEQUENCE;
	static const std::size_t DATA_BUFFER_INCR_SIZE;
	static std::size_t INIT_DATA_BUFFER_SIZE;

	net_packet_t();
	~net_packet_t();

  ///andrea: comodo per creare un net_packet_ptr_t
  static boost::shared_ptr <net_packet_t> create()
  {
    boost::shared_ptr<net_packet_t> 
      px(new net_packet_t);
      return px;
  }

	//for packet construction from socket data...
	net_packet_t(net_packet_header_t, const char*, std::size_t);

	void set_command(std::string);
	void set_req_freq(int);

	void set_packet_type(net_packet_type);
	void set_packet_size();
	
	void int_to_buf(int);
	void double_to_buf(double);
	void string_to_buf(std::string);
	void array_to_buf(all::core::uint8_ptr, std::size_t);

	int buf_to_int();
	double buf_to_double();
	std::string buf_to_string();
	std::size_t buf_to_array(all::core::uint8_ptr&);
	
	bool finalize_packet();

	int get_req_freq() const;

	std::string get_command() const;
	const boost::shared_array<char> get_buffer() const;

	const net_packet_header_t& get_header() const;

protected:
	
	void resize_data_buffer();

protected:

	net_packet_header_t m_header;
	
	std::string m_command;
	int m_req_freq;
	
	char* m_data;
	std::size_t m_data_size;
	std::size_t m_data_buffer_size;

	boost::shared_array <char> m_buffer;
	std::size_t m_buffer_size;

	char* m_read_ptr;
	char* m_write_ptr;
	
	//bool finalized;
};

typedef boost::shared_ptr <net_packet_t> net_packet_ptr_t;

}} //namespaces 

#endif
