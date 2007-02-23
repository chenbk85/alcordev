#ifndef stream_client_t_H_INCLUDED
#define stream_client_t_H_INCLUDED

#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/thread.hpp>

#include <alcor/core/client_base_t.hpp>
#include <alcor/core/core.h>
#include <alcor/core/iniWrapper.h>
#include "detail/in_stream_manager_t.hpp"
#include "stream_dest_t.hpp"

namespace all {
	namespace core {

class stream_client_t : public client_base_t {

public:
	stream_client_t(stream_dest_t&, char* ini_file ="config/stream_client.ini");

	//void set_multicast_address(all::core::ip_address_t);

	void start_receive();
	void stop_receive();

	void get_server_setting();

	void get_stream_setting_cb(net_packet_ptr_t);

	void set_frame_rate(int);

	//void new_frame_cb(boost::shared_array <all::core::uint8_t>, std::size_t);

private:
	boost::asio::io_service m_stream_service;
	boost::asio::ip::udp::socket m_udp_socket;
	
	boost::shared_ptr <boost::thread> m_stream_thread;

	iniWrapper m_ini_config;

	detail::in_stream_manager_t m_stream_manager;

	stream_dest_t& m_stream_dest; 

	all::core::ip_address_t m_multicast_address;

};

}} //namespaces

#endif