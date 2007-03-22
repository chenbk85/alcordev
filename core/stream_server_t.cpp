#include "stream_server_t.hpp"

namespace all {
	namespace core {

stream_server_t::stream_server_t(stream_source_ptr stream_source, char* ini_file):
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

	m_stream_manager.set_get_data_callback(boost::bind(&stream_source_t::get_data, m_stream_source.get(), _1));

	add_command_handler("setFrameRate", boost::bind(&stream_server_t::set_frame_rate_cb, this, _1, _2));
	add_command_handler("getStreamSetting", boost::bind(&stream_server_t::send_stream_setting_cb, this, _1, _2));

	set_client_connect_cb(boost::bind(&stream_server_t::client_connect_cb, this, _1));
	set_client_disconnect_cb(boost::bind(&stream_server_t::client_disconnect_cb, this, _1));

	//add handler for stream source command
	set_packet_handler(APPL_PACKET, boost::bind(&stream_server_t::process_source_command, this, _1, _2)); 

	m_streaming = false;
}

stream_server_t::~stream_server_t() {
	stop_streaming();
}


void stream_server_t::start_streaming() {
	//m_stream_service.reset();
	
	if (!m_streaming) {
		
		m_streaming = true;
		
		m_udp_socket.open(boost::asio::ip::udp::v4());

		m_stream_manager.start_streaming();
		
		m_stream_thread.reset(new boost::thread(boost::bind(&boost::asio::io_service::run, &m_stream_service)));
	}
}

void stream_server_t::stop_streaming() {

	if (m_streaming) {
		m_stream_service.post(boost::bind(&stream_source_t::stream_stopped, m_stream_source.get()));
		m_stream_service.post(boost::bind(&stream_server_t::handle_stop_streaming, this));
		m_stream_thread->join();
		m_stream_service.reset();
	}

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

void stream_server_t::handle_stop_streaming() {
	m_streaming = false;
	m_stream_manager.stop_streaming();
	m_udp_socket.close();
	m_stream_service.stop();
}

void stream_server_t::stop() {
	stop_streaming();
	server_base_t::stop();
}

void stream_server_t::client_connect_cb(client_connection_ptr_t client) {
	if (!m_streaming)
		start_streaming();
}

void stream_server_t::client_disconnect_cb(int num_client) {
	if (num_client == 0) {
		stop_streaming();
	}
}

void stream_server_t::process_source_command(all::core::client_connection_ptr_t client, all::core::net_packet_ptr_t packet) {
	m_stream_source->process_command(packet);
}


}} //namespaces