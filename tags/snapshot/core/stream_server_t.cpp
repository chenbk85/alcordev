#include "stream_server_t.hpp"

namespace all {
	namespace core {

stream_server_t::stream_server_t(stream_source_t& stream_source, char* ini_file):
									m_stream_service(),
									m_udp_socket(m_stream_service),
									m_stream_manager(m_udp_socket),
									m_ini_config(ini_file),
									m_stream_source(stream_source)
{	
	//set server listen port
	int server_port = (m_ini_config.GetInt("server:port", 33333));

	set_port(server_port);

	//set multicast address
	m_multicast_address.hostname = m_ini_config.GetStringAsChar("multicast:address", "239.255.0.1");
	m_multicast_address.port = m_ini_config.GetInt("multicast:port", 30001);
	m_stream_manager.set_multicast_address(m_multicast_address);

	//set streaming settings
	m_stream_manager.set_packet_size(m_ini_config.GetInt("stream:packet_size", 65000));
	m_stream_manager.set_frame_rate(m_ini_config.GetInt("stream:frame_rate", 5));

	m_stream_manager.set_get_data_callback(boost::bind(&stream_source_t::get_data, &m_stream_source, _1));

	add_command_handler("setFrameRate", boost::bind(&stream_server_t::set_frame_rate_cb, this, _1, _2));
	add_command_handler("getStreamSetting", boost::bind(&stream_server_t::send_stream_setting_cb, this, _1, _2));
}


void stream_server_t::start_streaming() {
	//m_stream_service.reset();
	
	m_udp_socket.open(boost::asio::ip::udp::v4());

	m_stream_manager.start_streaming();
	
	//m_stream_service.run();
	m_stream_thread.reset(new boost::thread(boost::bind(&boost::asio::io_service::run, &m_stream_service)));
}

void stream_server_t::stop_streaming() {
	//m_stream_manager.stop_streaming();
	m_stream_service.post(boost::bind(&detail::out_stream_manager_t::stop_streaming, &m_stream_manager));
	//m_udp_socket.close();
	//m_stream_service.post(boost::bind(&boost::asio::ip::udp::socket::close, &m_udp_socket));

	//try {
	//	m_udp_socket.close();
	//}
	//catch (boost::system::error_code e) {
	//	printf("error closing socket: %s\n", e.message().c_str()); 
	//}

	m_stream_thread->join();
}

void stream_server_t::set_frame_rate_cb(client_connection_ptr_t client, net_packet_ptr_t packet) {
	int frame_rate = packet->buf_to_int();
	m_stream_manager.set_frame_rate(frame_rate);
}

void stream_server_t::send_stream_setting_cb(client_connection_ptr_t client, net_packet_ptr_t packet) {
	packet.reset(new net_packet_t());
	packet->string_to_buf(m_multicast_address.hostname);
	packet->int_to_buf(m_multicast_address.port);
	packet->int_to_buf(m_stream_manager.get_packet_size());
	send_answer_packet("getStreamSetting", client, packet);
}

}} //namespaces