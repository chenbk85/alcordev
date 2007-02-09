#ifndef stream_pkt_sender_t_H_INCLUDED
#define stream_pkt_sender_t_H_INCLUDED

#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include "stream_packet_t.hpp"

namespace all {
	namespace core {
		namespace detail {

class stream_pkt_sender_t {

public:
	stream_pkt_sender_t(boost::asio::ip::udp::socket&);

	void set_error_callback(boost::function <void (const boost::system::error_code&)>);

	void set_endpoint(const boost::asio::ip::udp::endpoint&);

	void send_packet(stream_packet_ptr_t);

private:
	typedef boost::function <void (const boost::system::error_code&)> error_callback_t;
	error_callback_t m_error_callback;
	
	stream_packet_ptr_t m_packet;

	boost::asio::ip::udp::socket& m_socket;
	boost::asio::ip::udp::endpoint m_endpoint;

	std::string m_out_data_buffer;
};

}}} //namespaces

#endif