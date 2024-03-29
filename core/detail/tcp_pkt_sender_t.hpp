#ifndef tcp_pkt_sender_t_H_INCLUDED
#define tcp_pkt_sender_t_H_INCLUDED

#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include "alcor/core/net_packet_t.hpp"

namespace all {
	namespace core {
		namespace detail {


class tcp_pkt_sender_t {

public:
	tcp_pkt_sender_t(boost::asio::ip::tcp::socket&);

	void set_error_callback(boost::function <void (const boost::system::error_code&)>);

	void send_packet(net_packet_ptr_t);

	void send_packet_blk(net_packet_ptr_t);

	void handle_write(const boost::system::error_code&, std::size_t, std::string*);

private:
	typedef boost::function <void (const boost::system::error_code&)> error_callback_t;
	error_callback_t m_error_callback;
	
	net_packet_ptr_t m_packet;

	boost::asio::ip::tcp::socket& m_socket;

	std::string m_out_header_buffer;
	std::string m_out_data_buffer;
};

}}} //namespaces

#endif