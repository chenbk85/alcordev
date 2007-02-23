#ifndef in_stream_manager_t_H_INCLUDED
#define in_stream_manager_t_H_INCLUDED

#include <vector>
#include <boost/asio.hpp>
#include <boost/shared_array.hpp>
#include <boost/function.hpp>
#include <alcor/core/core.h>
#include "stream_pkt_receiver_t.hpp"
#include "stream_packet_t.hpp"

namespace all {
	namespace core {
		namespace detail {


class in_stream_manager_t {

public:

	static const std::size_t DATA_BUFFER_INCR_SIZE;
	static const int DEFAULT_FRAME_BUFFER_SIZE = 5;
	
	in_stream_manager_t(boost::asio::ip::udp::socket&);

	void start_receive();
	void stop_receive();

	void in_packet_cb(stream_packet_ptr_t);
	void in_packet_error_cb(const boost::system::error_code&);

	void set_frame_buffer(std::size_t buffer_size = DEFAULT_FRAME_BUFFER_SIZE);
	void set_packet_size(std::size_t);

	void set_data_ready_callback(boost::function <void (all::core::uint8_sarr, std::size_t)>);

private:
	//called if in data is bigger than m_max_data_size
	void resize_data_buffer();

private:

	boost::asio::ip::udp::socket& m_socket;
	std::size_t m_packet_data_size;

	stream_pkt_receiver_t m_packet_receiver;

	std::vector<all::core::uint8_ptr> m_frame_buffer;
	std::size_t m_frame_buffer_size;

	std::vector<int> m_frame_size;

	std::vector<int> m_frame_buffer_map;

	std::size_t m_max_data_size;

	int m_last_frame_num;
	int m_last_frame_pos;

	boost::function <void (all::core::uint8_sarr, std::size_t)> m_data_ready_cb;

};

}}} //namespaces

#endif