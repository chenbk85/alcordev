#include "client_connection_t.hpp"
#include "detail/client_manager_t.hpp"

namespace all {
	namespace core {

client_connection_t::client_connection_t(boost::asio::io_service& io_service,
										 detail::client_manager_t& client_manager):

								m_socket(io_service),
								m_client_manager_ref(client_manager),
								m_tcp_receiver(m_socket),
								m_tcp_sender(m_socket)
{

    //set the function to call when receiving a new packet
	m_tcp_receiver.set_read_callback(boost::bind(&client_connection_t::read_packet_cb, this, _1));
	m_tcp_receiver.set_error_callback(boost::bind(&client_connection_t::read_packet_error_cb, this, _1));

	m_tcp_sender.set_error_callback(boost::bind(&client_connection_t::send_packet_error_cb, this, _1));

}

boost::asio::ip::tcp::socket& client_connection_t::get_socket() {
	return m_socket;
}

all::core::ip_address_t client_connection_t::get_client_address() {
	return m_remote_addr;
}

void client_connection_t::set_in_pkt_callback(boost::function<void(client_connection_ptr_t, net_packet_ptr_t) > in_pkt_callback) {
	m_server_in_pkt_callback = in_pkt_callback;
}

void client_connection_t::start() {
	
	//get the client address
	boost::asio::ip::tcp::endpoint remote_endpoint = m_socket.remote_endpoint();
	
	m_remote_addr.hostname = remote_endpoint.address().to_string();
	
	m_remote_addr.port = remote_endpoint.port();
	printf("Client connected from address %s:%i\n", m_remote_addr.hostname.c_str(), m_remote_addr.port);
	
	//set keep_alive option
	boost::asio::socket_base::keep_alive keep_alive_opt(true);
	m_socket.set_option(keep_alive_opt);

	//start listening for incoming packet from the client
	m_tcp_receiver.start_listen();

}

void client_connection_t::read_packet_cb(net_packet_ptr_t packet) {
	m_server_in_pkt_callback(shared_from_this(), packet);
}

void client_connection_t::read_packet_error_cb(const boost::system::error_code& error) {
	if (error == boost::asio::error::no_data) {
		printf("Bad packet received\n");
	}
	else if (error == boost::asio::error::eof) {
		printf("Lost connection with client %s...\n", m_remote_addr.hostname.c_str());
		//m_client_manager_ref.stop_client(shared_from_this());
		m_socket.io_service().post(boost::bind(&detail::client_manager_t::stop_client, &m_client_manager_ref, shared_from_this()));
	}
	else {
		printf("Error in connection with client %s: %s\n Disconnecting client...\n", m_remote_addr.hostname.c_str(), error.message().c_str());
		//m_client_manager_ref.stop_client(shared_from_this());
		m_socket.io_service().post(boost::bind(&detail::client_manager_t::stop_client, &m_client_manager_ref, shared_from_this()));
	}
}

void client_connection_t::send_packet_error_cb(const boost::system::error_code& error) {
	printf("Error in socket connection with client %s: %s\nDisconnecting client...\n", m_remote_addr.hostname.c_str(), error.message().c_str());
	m_socket.io_service().post(boost::bind(&detail::client_manager_t::stop_client, &m_client_manager_ref, shared_from_this()));
}

void client_connection_t::handle_shutdown() {
	net_packet_ptr_t packet(new net_packet_t);
	packet->set_packet_type(COMMAND_PACKET);
	packet->set_command("shutdown");
	packet->finalize_packet();
	m_tcp_sender.send_packet(packet);
	stop();
}

void client_connection_t::stop() {
	
	stop_all_req();
	m_tcp_receiver.stop_listen();

	m_socket.close();
}

void client_connection_t::send_packet(net_packet_ptr_t packet) {
	m_tcp_sender.send_packet(packet);
}

void client_connection_t::add_req_timer(std::string request, boost::shared_ptr <boost::asio::deadline_timer> timer) {
	m_req_list.insert(std::make_pair(request, timer));//[request] = timer;
}

void client_connection_t::rem_req_timer(std::string request) {

	std::map <std::string, boost::shared_ptr<boost::asio::deadline_timer> >::iterator req_it = m_req_list.find(request);
	
	if (req_it != m_req_list.end()) {
		req_it->second->cancel();
		m_req_list.erase(request);
	}
}

void client_connection_t::stop_all_req() {
	int map_size = m_req_list.size();
	std::map<std::string, boost::shared_ptr<boost::asio::deadline_timer> >::iterator req_it = m_req_list.begin();

	for (int i = 0; i < m_req_list.size(); i++) {
		rem_req_timer(req_it->first);
		req_it++;
	}


	//for (std::map<std::string, boost::shared_ptr<boost::asio::deadline_timer> >::iterator req_it = m_req_list.begin();
	//						   req_it != m_req_list.end(); ++req_it) {
	//	printf("request %s\n", req_it->first.c_str());
	//	rem_req_timer(req_it->first);
	//}
	//printf("done\n");
}


void client_connection_t::handle_timer_req(const boost::system::error_code& error, timer_ptr_t req_timer, timer_callback_t req_callback, int freq) {
	if (!error) {
		req_callback();
		req_timer->expires_from_now(boost::posix_time::millisec(freq));
		req_timer->async_wait(boost::bind(&client_connection_t::handle_timer_req, this,
										  boost::asio::placeholders::error,
										  req_timer, req_callback, freq));
	}
}

}} //namespaces