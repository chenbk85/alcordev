#ifndef stream_pkt_receiver_t_H_INCLUDED
#define stream_pkt_receiver_t_H_INCLUDED

#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include "stream_packet_t.hpp"

namespace all {
	namespace core {
		namespace detail {

class stream_pkt_receiver_t {

public:
	stream_pkt_receiver_t(boost::asio::ip::udp::socket&);

	~stream_pkt_receiver_t();

	void set_packet_size(std::size_t);

	void set_read_callback(boost::function <void (stream_packet_ptr_t)>);
	void set_error_callback(boost::function <void (const boost::system::error_code&)>);

	void start_listen();
	void stop_listen();

	//for stopping a started async read operation
	void interrupt_reading();

	//used by asio::async_read to check if a read operation was aborted (see read_packet)
	bool was_interrupted(const boost::system::error_code&, std::size_t);

	//read a single packet
	void read_packet();

	stream_packet_ptr_t get_last_packet();

private:
	void handle_read_header(const boost::system::error_code&, std::size_t);
	void handle_read_data(const boost::system::error_code&, std::size_t);//, net_packet_header_t&);

private:
	
	typedef boost::function <void (stream_packet_ptr_t)> read_callback_t;
	typedef boost::function <void (const boost::system::error_code&)> error_callback_t;
	
	boost::asio::ip::udp::socket& m_socket;
	boost::asio::ip::udp::endpoint m_remote_endpoint;
	
	error_callback_t m_error_callback;
	read_callback_t m_read_callback;

	stream_packet_ptr_t m_packet_ptr;
	std::size_t m_packet_size;

	char* m_in_data_buffer;

	bool m_listen_f;
	bool m_interrupt_f;
};

}}} //namespaces

#endif