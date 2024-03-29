#define WIN32_LEAN_AND_MEAN

#include "client_manager_t.hpp"
#include <boost/bind.hpp>



namespace all {
	namespace core {
		namespace detail {


void client_manager_t::add_client(client_connection_ptr_t new_connection) {
	m_client_list.insert(new_connection);
	new_connection->start();
	if (connect_cb)
		connect_cb(new_connection);
}

void client_manager_t::stop_client(client_connection_ptr_t connection) {
	connection->stop();
	m_client_list.erase(connection);
	if (disconnect_cb)
		disconnect_cb(get_num_client());
}

void client_manager_t::stop_all_client() {
	std::for_each(m_client_list.begin(), m_client_list.end(), 
				boost::bind(&client_connection_t::stop, _1));
	m_client_list.clear();
}

int client_manager_t::get_num_client() {
	return static_cast <int> (m_client_list.size());
}

}}} //namespaces