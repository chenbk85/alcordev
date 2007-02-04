#include "client_base_t.hpp"

client_base_t::client_base_t():
						m_io_service(),
						m_tcp_socket(m_io_service),
						m_resolver(m_io_service),
						m_tcp_sender(m_tcp_socket),
						m_tcp_receiver(m_tcp_socket)
{
	m_server_addr.hostname = "localhost";
	m_server_addr.port = 33333;

	m_tcp_receiver.set_read_callback(boost::bind(&client_base_t::in_packet_handler, this, _1));
	m_tcp_receiver.set_error_callback(boost::bind(&client_base_t::in_packet_error_cb, this, _1));

	set_packet_handler(COMMAND_PACKET, boost::bind(&client_base_t::command_packet_handler, this, _1));
	set_packet_handler(ANSWER_PACKET, boost::bind(&client_base_t::command_packet_handler, this, _1));

	add_command_handler("shutdown", boost::bind(&client_base_t::shutdown_handler, this, _1));

	m_state = STATE_DISCONNECTED;

}

void client_base_t::run() {
	boost::asio::ip::tcp::resolver::query query(boost::asio::ip::tcp::v4(), 
												m_server_addr.hostname, 
												boost::lexical_cast<std::string>(m_server_addr.port));
    m_resolver.async_resolve(query,
							 boost::bind(&client_base_t::handle_resolve, 
							             this,
										 boost::asio::placeholders::error,
										 boost::asio::placeholders::iterator));
	m_io_service.run();

}

void client_base_t::run_async() {
	boost::shared_ptr<boost::thread> client_thread;
	client_thread.reset(new boost::thread(boost::bind(&client_base_t::run, this)));
}


void client_base_t::stop() {

	if (m_state == STATE_CONNECTED) {

		send_command("disconnect");

		m_io_service.post(boost::bind(&client_base_t::handle_stop, this));
	}
}

void client_base_t::set_server_addr(all::core::ip_address_t server_address) {
	m_server_addr = server_address;
}

void client_base_t::handle_resolve(const boost::system::error_code& error, boost::asio::ip::tcp::resolver::iterator endpoint_iterator) {
	if (!error) {
		// Attempt a connection to the first endpoint in the list. Each endpoint
		// will be tried until we successfully establish a connection.
		boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;
		m_tcp_socket.async_connect(endpoint,
							   boost::bind(&client_base_t::handle_connect,
										   this,
										   boost::asio::placeholders::error,
										   ++endpoint_iterator));
	}
	else {
		printf("error in client_base_t::handle_resolve \n %s", error.message());
		
    }
}

void client_base_t::handle_stop() {
	
	m_tcp_receiver.stop_listen();

	m_tcp_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
	m_tcp_socket.close();

	m_state = STATE_DISCONNECTED;

	//m_io_service.interrupt();
}


void client_base_t::handle_connect(const boost::system::error_code& error, boost::asio::ip::tcp::resolver::iterator endpoint_iterator) {
	if (!error) {
		// The connection was successful. 
		m_state = STATE_CONNECTED;

		printf("Client connected\n");

		boost::asio::socket_base::keep_alive keep_alive_opt(true);
		m_tcp_socket.set_option(keep_alive_opt);
		
		m_tcp_receiver.start_listen();

    } 
	else if (endpoint_iterator != boost::asio::ip::tcp::resolver::iterator()) {
		// The connection failed. Try the next endpoint in the list.
		m_tcp_socket.close();
		boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;
		m_tcp_socket.async_connect(endpoint,
							   boost::bind(&client_base_t::handle_connect, this,
							     		   boost::asio::placeholders::error, 
										   ++endpoint_iterator));
    } 
	else {
		printf("error in client_base_t::handle_connect \n %s", error.message());
	}
     
}

void client_base_t::send_command(std::string command) {
	net_packet_ptr_t packet(new net_packet_t);
	send_command(command, packet);
}

void client_base_t::send_command(std::string command, net_packet_ptr_t packet) {
	packet->set_packet_type(COMMAND_PACKET);
	packet->set_command(command);
	packet->finalize_packet();
	m_tcp_sender.send_packet(packet);
}

void client_base_t::send_request(std::string request, int req_freq) {
	net_packet_ptr_t packet(new net_packet_t);
	send_request(request, packet, req_freq);
}

void client_base_t::send_request(std::string request, net_packet_ptr_t packet, int req_freq) {
	packet->set_packet_type(REQUEST_PACKET);
	packet->set_command(request);
	packet->set_req_freq(req_freq);
	packet->finalize_packet();
	m_tcp_sender.send_packet(packet);

}

void client_base_t::stop_request(std::string request) {
	send_request(request, -1);
}


void client_base_t::in_packet_handler (net_packet_ptr_t packet) {
	
	net_packet_type packet_type = packet->get_header().get_packet_type();
	std::map <net_packet_type, in_packet_handler_t >::iterator type_it = m_packet_handler_list.find(packet_type);

	if (type_it != m_packet_handler_list.end()) {
		in_packet_handler_t packet_handler = type_it->second;
		packet_handler(packet);
	}
	else {
		printf ("Cannot find an handler for received packet type");
	}

}

void client_base_t::in_packet_error_cb(const boost::system::error_code& error) {
	if (error == boost::asio::error::no_data) {
		printf("Bad packet received\n");
	}
	else if (error == boost::asio::error::eof) {
		printf("Lost connection with server\n");
		m_state = STATE_LOST_CONNECTION;
	}
	else
		printf("Error receiving packet: \n%s", error.message());
}

void client_base_t::command_packet_handler (net_packet_ptr_t packet) {
	std::string command = packet->get_command();
	std::map <std::string, in_packet_handler_t>::iterator cmd_it= m_command_handler_list.find(command);
	if (cmd_it != m_command_handler_list.end()) {
		in_packet_handler_t packet_handler = cmd_it->second;
		packet_handler(packet);
	}
	else {
		printf ("Unknown command received by server: %s\n", command.c_str());
	}
}



void client_base_t::set_packet_handler (net_packet_type packet_type, boost::function <void (net_packet_ptr_t) > handler) {
	m_packet_handler_list[packet_type] = handler;
}

void client_base_t::add_command_handler(std::string command, boost::function <void (net_packet_ptr_t)> handler) {
	//m_command_pkt_handler_list.insert(make_pair(command, handler));
	m_command_handler_list[command] = handler;
}

void client_base_t::shutdown_handler(net_packet_ptr_t packet) {
	printf("Server shutting down...disconnecting\n");
	handle_stop();
}