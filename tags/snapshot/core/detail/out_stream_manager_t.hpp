#ifndef out_stream_manager_t_H_INCLUDED
#define out_stream_manager_t_H_INCLUDED

#include <boost/function.hpp>
#include <alcor/core/core.h>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include "stream_pkt_sender_t.hpp"
#include "stream_packet_t.hpp"

namespace all {
	namespace core {
		namespace detail {

class out_stream_manager_t {

public:
	static const int MAX_FRAME_NUMBER = 999;
	
	out_stream_manager_t(boost::asio::ip::udp::socket&);
	~out_stream_manager_t();

	void set_packet_size(int);
	void set_frame_rate(int);
	void set_multicast_address(all::core::ip_address_t);

	void set_get_data_callback(boost::function <int (all::core::uint8_ptr*)>);

	int get_packet_size();

	void start_streaming();
	void stop_streaming();

private:
	void next_frame_handler(const boost::system::error_code&);
	void send_data();

private:
	boost::asio::ip::udp::socket& m_socket;

	stream_pkt_sender_t m_packet_sender;

	int m_packet_size;
	int max_packet_data_size;

	int m_frame_number;
	int m_packet_number;

	all::core::uint8_ptr m_curr_data;
	int m_curr_data_size;


	boost::shared_ptr<boost::thread> m_send_data_thread;

	boost::asio::deadline_timer m_timer;
	int m_timer_freq;

	boost::function <int (all::core::uint8_ptr*)> m_get_data_cb;

};

}}}  //namespaces

#endif