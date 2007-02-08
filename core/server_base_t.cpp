#include "server_base_t.hpp"

server_base_t::server_base_t():
							m_io_service(),
							m_client_acceptor(m_io_service),
							m_client_manager(),
							m_new_client_connection_ptr(new client_connection_t(m_io_service, m_client_manager))
{
	m_addr.hostname = "localhost";
	m_addr.port = 33333;

	m_new_client_connection_ptr->set_in_pkt_callback(boost::bind(&server_base_t::in_packet_cb, this, _1, _2));
    
	//add predefinited commands
	add_command_handler("disconnect", boost::bind(&server_base_t::client_disconnect_cb, this, _1, _2));

	set_packet_handler(COMMAND_PACKET, boost::bind(&server_base_t::in_command_pkt_cb, this, _1, _2));
	set_packet_handler(REQUEST_PACKET, boost::bind(&server_base_t::in_request_pkt_cb, this, _1, _2));

}

void server_base_t::run() {
	
	printf("Server started: listening on port %i\n", m_addr.port);

	//costruct a tcp endpoint
	boost::asio::ip::tcp::endpoint server_endpoint (boost::asio::ip::tcp::v4(), m_addr.port);
	
	//associate the connection acceptor with the endpoint and start listen for new connection
	m_client_acceptor.open(server_endpoint.protocol());

	//set option to reuse address
	m_client_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));

	m_client_acceptor.bind(server_endpoint);
	
	//start listening for new connection
	m_client_acceptor.listen();

	m_client_acceptor.async_accept(m_new_client_connection_ptr->get_socket(),
							boost::bind(&server_base_t::handle_accept,
							this,
							boost::asio::placeholders::error));

	m_io_service.run();
}

void server_base_t::run_async() {
	//create a new thread and run server in this new thread
	thread_t server_thread;
	server_thread.reset(new boost::thread(boost::bind(&server_base_t::run, this)));
}

void server_base_t::stop() {
	
	//safe to call from any thread
	m_io_service.post(boost::bind(&server_base_t::handle_stop, this));
}

void server_base_t::set_port(int port) {
	m_addr.port = port;
}

void server_base_t::add_command_handler(std::string command, boost::function <void (client_connection_ptr_t, net_packet_ptr_t) > callback) {
	//m_command_list.insert(make_pair(command, callback));
	m_command_handler_list[command] = callback;
}

void server_base_t::set_packet_handler(net_packet_type packet_type, boost::function <void (client_connection_ptr_t, net_packet_ptr_t) > callback) {
	m_packet_handler_list[packet_type] = callback;
}

void server_base_t::in_packet_cb(client_connection_ptr_t client, net_packet_ptr_t packet) {
	
	net_packet_type packet_type = packet->get_header().get_packet_type();
	
	std::map <net_packet_type, in_pkt_callback_t>::iterator type_it = m_packet_handler_list.find(packet_type);
	
	if (type_it != m_packet_handler_list.end()) {
        in_pkt_callback_t in_cmd_callback = type_it->second;
		in_cmd_callback(client, packet);
	}
	else {
		printf("No handler found for the received packet type\n");
	}
}

void server_base_t::in_command_pkt_cb(client_connection_ptr_t client, net_packet_ptr_t packet) {
	
	std::string command = packet->get_command();
	
	std::map<std::string, in_pkt_callback_t>::iterator it = m_command_handler_list.find(command);
	if (it != m_command_handler_list.end()) {
		in_pkt_callback_t cmd_callback = it->second;
		cmd_callback(client, packet);
	}
	else {
		printf("%s: command not found\n", command.c_str());
	}
}

void server_base_t::in_request_pkt_cb(client_connection_ptr_t client, net_packet_ptr_t packet) {
	
	std::string request = packet->get_command();

	int req_interval = packet->get_req_freq();
	
	std::map<std::string, in_pkt_callback_t>::iterator it = m_command_handler_list.find(request);
	
	if (it != m_command_handler_list.end()) {
		
		in_pkt_callback_t req_callback = it->second;
		
		if( req_interval > 0) {
			
			boost::function <void (void)> callback;
			callback = boost::bind(req_callback, client, packet);
			boost::shared_ptr <boost::asio::deadline_timer> timer_ptr (new boost::asio::deadline_timer(m_io_service));

			client->add_req_timer(request, timer_ptr);

			timer_ptr->expires_from_now(boost::posix_time::millisec(req_interval));
			timer_ptr->async_wait(boost::bind(&client_connection_t::handle_timer_req, client,
									 boost::asio::placeholders::error, 
									 timer_ptr, callback, req_interval));
		}
		else if (req_interval == 0) {
			req_callback(client, packet);
		}
		else
			client->rem_req_timer(request);
	}
	else {
		printf("%s: command not found\n", request.c_str());
	}
}


void server_base_t::client_disconnect_cb(client_connection_ptr_t client, net_packet_ptr_t packet) {
	printf("Disconnecting client %s\n", client->get_client_address().hostname.c_str());
	m_client_manager.stop_client(client);
}

void server_base_t::handle_accept(const boost::system::error_code& error) {
	if (!error) {

		//handle accepted connection
		m_client_manager.add_client(m_new_client_connection_ptr);
		
		//prepare to accept new connections
		m_new_client_connection_ptr.reset(new client_connection_t(m_io_service, m_client_manager));
		
		m_new_client_connection_ptr->set_in_pkt_callback(boost::bind(&server_base_t::in_packet_cb, this, _1, _2));

		m_client_acceptor.async_accept(m_new_client_connection_ptr->get_socket(), 
									   boost::bind(&server_base_t::handle_accept,
												   this,
												   boost::asio::placeholders::error));
	}
	
	//something wrong...check
	else if (error.value() != 995) {
		printf("Error accepting new connection: %s\n", error.message());
	}
}

void server_base_t::handle_stop() {
	printf("Server shutting down...warning clients\n");
	m_client_acceptor.close();
	m_client_manager.stop_all_client();
}

void server_base_t::send_command_packet(std::string command, client_connection_ptr_t client , net_packet_ptr_t packet) {
	packet->set_packet_type(COMMAND_PACKET);
	packet->set_command(command);
	packet->finalize_packet();
	send_finalized_packet(client, packet);
}

void server_base_t::send_answer_packet(std::string command, client_connection_ptr_t client, net_packet_ptr_t packet) {
	packet->set_packet_type(ANSWER_PACKET);
	packet->set_command(command);
	packet->finalize_packet();
	send_finalized_packet(client, packet);
}

void server_base_t::send_finalized_packet(client_connection_ptr_t client, net_packet_ptr_t packet) {
	client->send_packet(packet);
}