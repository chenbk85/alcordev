#ifndef stream_server_t_H_INCLUDED
#define stream_server_t_H_INCLUDED

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <alcor/core/core.h>
#include <alcor/core/iniWrapper.h>
#include <alcor/core/server_base_t.hpp>

#include "detail/out_stream_manager_t.hpp"
#include "stream_source_t.hpp"

namespace all {
	namespace core {

class stream_server_t : public server_base_t {

public:
	stream_server_t(stream_source_ptr, char* ini_file = "config/stream_server.ini");

	void start_streaming();
	void stop_streaming();

	void set_frame_rate_cb(client_connection_ptr_t, net_packet_ptr_t);

	virtual void stop();

private:

	void send_stream_setting_cb(client_connection_ptr_t, net_packet_ptr_t);

	void handle_stop_streaming();

	void client_connect_cb(client_connection_ptr_t);
	void client_disconnect_cb(int);

private:
	
	boost::asio::io_service m_stream_service;
	boost::shared_ptr <boost::thread> m_stream_thread; 
	
	boost::asio::ip::udp::socket m_udp_socket;
	boost::asio::ip::udp::endpoint m_endpoint;

	iniWrapper m_ini_config;

	stream_source_ptr m_stream_source;

	all::core::ip_address_t m_multicast_address;

	detail::out_stream_manager_t m_stream_manager;

	volatile bool m_streaming;

};

}} //namespaces

#endif