#ifndef server_base_t_H_INCLUDED
#define server_base_t_H_INCLUDED

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <alcor/core/core.h>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include "detail/client_manager_t.hpp"
#include "client_connection_t.hpp"
#include "net_packet_t.hpp"

namespace all {
	namespace core {

class server_base_t {

public:
	
	server_base_t();

	//virtual ~server_base_t();

	//run server
	void run();
	
	//run server in its own thread
	void run_async();

	//stop the server
	void stop();

	void set_port(int);
	

	void set_packet_handler(net_packet_type, boost::function <void (client_connection_ptr_t, net_packet_ptr_t) >);

	void add_command_handler(std::string, boost::function <void (client_connection_ptr_t, net_packet_ptr_t) >);

	
	void send_finalized_packet(client_connection_ptr_t, net_packet_ptr_t);

	void send_command_packet(std::string, client_connection_ptr_t, net_packet_ptr_t);

	void send_answer_packet(std::string, client_connection_ptr_t, net_packet_ptr_t);


private:
	
	void start_listen();

	//handler for new client connection 
	void handle_accept(const boost::system::error_code&);

	//handler for stop request
	void handle_stop();

	//when a client_connection_t receive a packet invoke this function
	void in_packet_cb(client_connection_ptr_t, net_packet_ptr_t);
	
	//default callback for COMMAND_PACKET type.
	//it search for a corresponding command entry in m_command_list
	//can be changed by invoking set_packet_type_callback
	void in_command_pkt_cb(client_connection_ptr_t, net_packet_ptr_t);

	void in_request_pkt_cb(client_connection_ptr_t, net_packet_ptr_t);

	//default callback when receiving a disconnect command from a client
	void client_disconnect_cb(client_connection_ptr_t, net_packet_ptr_t);

	void pkt_send_error_cb(const boost::system::error_code&);

	void pkt_receive_error_cb(const boost::system::error_code&);

private:
	boost::asio::io_service m_io_service;

	//acceptor for client connections
	boost::asio::ip::tcp::acceptor m_client_acceptor;

	//address of the server
	all::core::ip_address_t m_addr;

	//for run_async
	typedef boost::shared_ptr< boost::thread > thread_spr;
	thread_spr m_execution_thread;

	//manage clients currently connected on the server
	detail::client_manager_t m_client_manager;

	//handler of new incoming connection to the server
	client_connection_ptr_t m_new_client_connection_ptr;

	typedef boost::function <void (client_connection_ptr_t, net_packet_ptr_t) > in_pkt_callback_t;
	
	std::map <std::string, in_pkt_callback_t> m_command_handler_list;

	std::map <net_packet_type, in_pkt_callback_t> m_packet_handler_list;
};

}} //namespaces

#endif

