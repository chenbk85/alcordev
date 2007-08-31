#ifndef urg_server_data_t_H_INCLUDED
#define urg_server_data_t_H_INCLUDED

#include <alcor/core/net_packet_t.hpp>
#include <alcor/sense/urg_scan_data_t.hpp>

namespace all {
	namespace sense {

class urg_server_data_t {

public:
	
	void import(core::net_packet_ptr_t);
	void pack(core::net_packet_ptr_t);

public:

	urg_scan_data_ptr _scan_data;

};

}} //namespaces

#endif