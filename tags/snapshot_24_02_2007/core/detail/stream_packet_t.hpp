#ifndef stream_packet_t_H_INCLUDED
#define stream_packet_t_H_INCLUDED

#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <alcor/core/core.h>

namespace all {
	namespace core {
		namespace detail {

class stream_packet_t {

public:
	static const short HEADER_LENGTH = 16;
	static const char FIELD_SEPARATOR = '_';
	
	stream_packet_t();
	~stream_packet_t();

	void set_frame_number(int);
	void set_packet_number(int);
	void set_data(all::core::uint8_t*, std::size_t size);

	bool build_packet_from_string(char*, int);

	bool finalize_packet();

	all::core::uint8_t* get_data() const;
	const all::core::uint8_t* get_data_ptr() const;

	int get_frame_number() const;
	int get_packet_number() const;
	std::size_t get_data_size() const;

	std::string get_buffer() const;

private:
	
	int m_frame_number;
	int m_packet_number;
	all::core::uint8_t* m_data;
	std::size_t m_data_size;

	std::string m_header;

	std::string m_buffer;
};

typedef boost::shared_ptr <stream_packet_t> stream_packet_ptr_t;

}}} //namespaces

#endif