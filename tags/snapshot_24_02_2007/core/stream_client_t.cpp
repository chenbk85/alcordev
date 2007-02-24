#include "stream_client_t.hpp"

namespace all {
	namespace core {

stream_client_t::stream_client_t(stream_dest_t& stream_dest, char* ini_file) :
									m_stream_service(),
									m_udp_socket(m_stream_service),
									m_stream_manager(m_udp_socket),
									m_ini_config(ini_file),
									m_stream_dest(stream_dest)
{
	
	//m_multicast_address.hostname = "239.255.0.1";
	//m_multicast_address.port = 30001;

	all::core::ip_address_t server_addr;
	server_addr.hostname = m_ini_config.GetStringAsChar("server:address", "127.0.0.1");
	server_addr.port = m_ini_config.GetInt("server:port", 33333);

	set_server_addr(server_addr);

	add_command_handler("getStreamSetting", boost::bind(&stream_client_t::get_stream_setting_cb, this, _1));

	set_connect_callback(boost::bind(&stream_client_t::get_server_setting, this));
}

void stream_client_t::get_stream_setting_cb(net_packet_ptr_t packet) {
	m_multicast_address.hostname = packet->buf_to_string();
	printf("hostname: %s\n", m_multicast_address.hostname.c_str());
	m_multicast_address.port = packet->buf_to_int();
	int packet_size = packet->buf_to_int();
	m_stream_manager.set_packet_size(packet_size);
}

void stream_client_t::get_server_setting() {
	send_request("getStreamSetting");
}

void stream_client_t::set_frame_rate(int frame_rate) {
	net_packet_ptr_t packet(new net_packet_t());
	packet->int_to_buf(frame_rate);
	send_command("setFrameRate", packet);
}

void stream_client_t::start_receive() {
	
	//m_stream_service.reset();

	boost::asio::ip::address listen_address = boost::asio::ip::address::from_string("0.0.0.0");
	boost::asio::ip::address multicast_address = boost::asio::ip::address::from_string(m_multicast_address.hostname);

	boost::asio::ip::udp::endpoint listen_endpoint(listen_address, m_multicast_address.port);
    m_udp_socket.open(listen_endpoint.protocol());
    m_udp_socket.set_option(boost::asio::ip::udp::socket::reuse_address(true));
    m_udp_socket.bind(listen_endpoint);

    // join the multicast group
    m_udp_socket.set_option(
		boost::asio::ip::multicast::join_group(multicast_address));


	int frame_buffer_size = m_ini_config.GetInt("stream:frame_buffer_size", 5);
	m_stream_manager.set_frame_buffer(frame_buffer_size);

	//m_stream_manager.set_data_ready_callback(boost::bind(&stream_client_t::new_frame_cb, this, _1, _2));
	m_stream_manager.set_data_ready_callback(boost::bind(&stream_dest_t::import_data, &m_stream_dest, _1, _2));
	m_stream_manager.start_receive();
	m_stream_thread.reset(new boost::thread(boost::bind(&boost::asio::io_service::run, &m_stream_service)));

}

void stream_client_t::stop_receive() {
	//m_stream_service.post(boost::bind(&boost::asio::io_service::interrupt, &m_stream_service));
	printf("Stopping stream...\n");
	m_udp_socket.close();
	m_stream_thread->join();
}


//void stream_client_t::new_frame_cb(boost::shared_array <all::core::uint8_t> data, std::size_t data_size) {
//	printf("Frame callback\n");
//}

}} //namespaces