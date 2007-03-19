#include "stream_client_t.hpp"

namespace all {
	namespace core {

stream_client_t::stream_client_t(stream_dest_ptr stream_dest, char* ini_file) :
									m_stream_service(),
									m_udp_socket(m_stream_service),
									m_stream_manager(m_udp_socket),
									m_ini_config(ini_file),
									m_stream_dest(stream_dest)
{

	all::core::ip_address_t server_addr;
	server_addr.hostname = m_ini_config.GetStringAsChar("server:address", "127.0.0.1");
	server_addr.port = m_ini_config.GetInt("server:port", 33333);

	set_server_addr(server_addr);

	add_command_handler("getStreamSetting", boost::bind(&stream_client_t::get_stream_setting_cb, this, _1));

	set_connect_callback(boost::bind(&stream_client_t::connect_cb, this));
	set_disconnect_callback(boost::bind(&stream_client_t::disconnect_cb, this));

	//set frame buffer
	int frame_buffer_size = m_ini_config.GetInt("stream:frame_buffer_size", 5);
	m_stream_manager.set_frame_buffer(frame_buffer_size);

	m_stream_manager.set_data_ready_callback(boost::bind(&stream_dest_t::import_data, m_stream_dest.get(), _1, _2));

	m_streaming = false;
}

stream_client_t::~stream_client_t() {
	stop_receive();
}

void stream_client_t::get_stream_setting_cb(net_packet_ptr_t packet) {

	//get server setting
	m_multicast_address.hostname = packet->buf_to_string();
	printf("hostname: %s\n", m_multicast_address.hostname.c_str());
	m_multicast_address.port = packet->buf_to_int();
	int packet_size = packet->buf_to_int();
	m_stream_manager.set_packet_size(packet_size);

	
	//open udp socket and bind it to the multicast address
	boost::asio::ip::address listen_address = boost::asio::ip::address::from_string("0.0.0.0");
	m_asio_multicast_address = boost::asio::ip::address::from_string(m_multicast_address.hostname);

	boost::asio::ip::udp::endpoint listen_endpoint(listen_address, m_multicast_address.port);
	printf("opening socket\n");
	m_udp_socket.open(listen_endpoint.protocol());
    m_udp_socket.set_option(boost::asio::ip::udp::socket::reuse_address(true));
	printf("socket opened\n");
    m_udp_socket.bind(listen_endpoint);
	start_receive();
}

void stream_client_t::connect_cb() {
	send_request("getStreamSetting");
}

void stream_client_t::disconnect_cb() {
	if (m_streaming) {
		printf("disconnect cb: stopping stream\n");
		m_stream_service.post(boost::bind(&stream_client_t::handle_disconnect, this));
		m_stream_thread->join();
		m_stream_service.reset();
	}
	else {
		printf("disconnect cb: closing udp socket\n");
		m_udp_socket.close();
	}
}

void stream_client_t::set_frame_rate(int frame_rate) {
	net_packet_ptr_t packet(new net_packet_t());
	packet->int_to_buf(frame_rate);
	send_command("setFrameRate", packet);
}

void stream_client_t::start_receive() {

	if ( (!m_streaming) && (is_connected()) ) {
		
		//start waiting for packet 
		m_stream_manager.start_receive();
		m_stream_thread.reset(new boost::thread(boost::bind(&boost::asio::io_service::run, &m_stream_service)));
    
		// join the multicast group
		m_udp_socket.set_option(
			boost::asio::ip::multicast::join_group(m_asio_multicast_address));

		m_streaming = true;
	}

}

void stream_client_t::stop_receive() {
	if (m_streaming) {
		m_stream_service.post(boost::bind(&stream_client_t::handle_stop_receive, this));
		m_stream_thread->join();
		m_stream_service.reset();
	}
}

void stream_client_t::handle_stop_receive() {
	printf("stream handle stop receive\n");
	m_streaming = false;
	m_stream_manager.stop_receive();
	printf("stream stopped\n");
	m_udp_socket.set_option(boost::asio::ip::multicast::leave_group(m_asio_multicast_address));
	printf("multicast left\n");
	m_stream_service.stop();
}

void stream_client_t::handle_disconnect() {
	printf("stream handle disconnect\n");
	m_streaming = false;
	m_stream_manager.stop_receive();
	printf("stream stopped\n");
	m_udp_socket.close();
	printf("udp socket closed\n");
	m_stream_service.stop();
}

void stream_client_t::stop() {
	stop_receive();
	client_base_t::stop();
}

void stream_client_t::send_source_cmd(std::string command) {
	net_packet_ptr_t packet(new net_packet_t());
	send_source_cmd(command, packet);
}

void stream_client_t::send_source_cmd(std::string command, net_packet_ptr_t packet) {
	if (is_connected()) {
		packet->set_packet_type(APPL_PACKET);
		packet->set_command(command);
		packet->finalize_packet();
		send_finalized_packet(packet);
	}
}



}} //namespaces