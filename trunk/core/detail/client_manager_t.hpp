#ifndef client_manager_t_H_INCLUDED
#define client_manager_t_H_INCLUDED

#include <set>
#include <boost/function.hpp>
#include "net_packet_t.hpp"
#include "client_connection_t.hpp"

class client_manager_t {

public:
	
	//add a new client to the list
	void add_client(client_connection_ptr_t);
	
	void stop_client(client_connection_ptr_t);
	
	void stop_all_client();

	int get_num_client();

private:
	//the client list
	std::set <client_connection_ptr_t> m_client_list;

};

#endif