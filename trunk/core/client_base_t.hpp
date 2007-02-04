#ifndef client_base_t_H_INCLUDED
#define client_base_t_H_INCLUDED

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <alcor/core/core.h>
#include <boost/bind.hpp>
#include "detail/net_packet_t.hpp"
#include "detail/tcp_pkt_sender_t.hpp"
#include "detail/tcp_pkt_receiver_t.hpp"

class client_base_t {
	
public:

	enum client_state {
		STATE_CONNECTED,
		STATE_DISCONNECTED,
		STATE_LOST_CONNECTION
	};

	client_base_t();

	//run client and connect to the server
	void run();

	//run client in it's own thread
	void run_async();

	//stop client
	void stop();

	void set_server_addr(all::core::ip_address_t);

	//send a command to the server (no answer)
	void send_command(std::string);
	void send_command(std::string, net_packet_ptr_t);

	//send a request to the server
	void send_request(std::string, int req_freq = 0);
	void send_request(std::string, net_packet_ptr_t, int req_freq = 0);

	void stop_request(std::string);
	
	//set handler for incoming packet (based on its type)
	void set_packet_handler (net_packet_type, boost::function <void (net_packet_ptr_t) >);
	
	
	//add a command handler
	void add_command_handler(std::string, boost::function <void (net_packet_ptr_t)> );
	

private:

	void handle_stop();

	void handle_resolve(const boost::system::error_code&, boost::asio::ip::tcp::resolver::iterator);
	void handle_connect(const boost::system::error_code&, boost::asio::ip::tcp::resolver::iterator);

	//invoked by m_tcp_receiver when receive a new packet
	void in_packet_handler (net_packet_ptr_t);
	void in_packet_error_cb (const boost::system::error_code&);

	//
	void command_packet_handler (net_packet_ptr_t);

	void shutdown_handler(net_packet_ptr_t);


private:
	
	client_state m_state;

	boost::asio::io_service m_io_service;
	boost::asio::ip::tcp::socket m_tcp_socket;
	boost::asio::ip::tcp::resolver m_resolver;

	all::core::ip_address_t m_server_addr;

	tcp_pkt_sender_t m_tcp_sender;
	tcp_pkt_receiver_t m_tcp_receiver;

	typedef boost::function <void (net_packet_ptr_t)> in_packet_handler_t;

	std::map <net_packet_type, in_packet_handler_t > m_packet_handler_list;
	std::map <std::string, in_packet_handler_t> m_command_handler_list;

};

#endif