#ifndef client_manager_t_H_INCLUDED
#define client_manager_t_H_INCLUDED

#include <set>
#include <boost/function.hpp>
#include "alcor/core/net_packet_t.hpp"
#include "alcor/core/client_connection_t.hpp"

namespace all {
	namespace core {
		namespace detail {


class client_manager_t {

public:
	
	//add a new client to the list
	void add_client(all::core::client_connection_ptr_t);
	
	void stop_client(client_connection_ptr_t);
	
	void stop_all_client();

	int get_num_client();

	boost::function <void (client_connection_ptr_t)> connect_cb;
	boost::function <void (int)> disconnect_cb;

private:
	//the client list
	std::set <client_connection_ptr_t> m_client_list;

};

}}} //namespaces

#endif