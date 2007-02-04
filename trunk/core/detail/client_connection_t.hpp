#ifndef client_connection_t_H_INCLUDED
#define client_connection_t_H_INCLUDED

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <alcor/core/core.h>
#include "net_packet_t.hpp"
#include "tcp_pkt_receiver_t.hpp"
#include "tcp_pkt_sender_t.hpp"

class client_manager_t;

//handler for client connection

class client_connection_t : public boost::enable_shared_from_this <client_connection_t> {
public:
	client_connection_t(boost::asio::io_service&, client_manager_t&);
	
	boost::asio::ip::tcp::socket& get_socket();

	void start();
	
	void stop();

	void handle_shutdown();

	all::core::ip_address_t get_client_address();

	//invoked by m_tcp_receiver when receive a new packet
	void read_packet_cb(net_packet_ptr_t);

	void read_packet_error_cb(const boost::system::error_code&);

	typedef boost::shared_ptr <client_connection_t> client_connection_ptr_t;
	
	//set by server
	void set_in_pkt_callback(boost::function <void (client_connection_ptr_t, net_packet_ptr_t)>);

	void send_packet(net_packet_ptr_t);

	void add_req_timer(std::string, boost::shared_ptr <boost::asio::deadline_timer>);
	void rem_req_timer(std::string);

	void stop_all_req();

	typedef boost::function <void (void)> timer_callback_t;
	typedef boost::shared_ptr <boost::asio::deadline_timer> timer_ptr_t;
	void handle_timer_req (const boost::system::error_code&, timer_ptr_t, timer_callback_t, int);

private:
	boost::asio::ip::tcp::socket m_socket;

	tcp_pkt_receiver_t m_tcp_receiver;

	tcp_pkt_sender_t m_tcp_sender;

	client_manager_t& m_client_manager_ref;

	all::core::ip_address_t m_remote_addr;

	std::map <std::string, boost::shared_ptr <boost::asio::deadline_timer> > m_req_list;

	boost::function <void (client_connection_ptr_t, net_packet_ptr_t)> m_server_in_pkt_callback;
};

typedef boost::shared_ptr <client_connection_t> client_connection_ptr_t;

#endif