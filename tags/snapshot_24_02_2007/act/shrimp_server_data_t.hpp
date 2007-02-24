#ifndef shrimp_server_data_t_H_INCLUDED
#define shrimp_server_data_t_H_INCLUDED

#include <alcor/core/net_packet_t.hpp>

namespace all {
	namespace act {

class shrimp_server_data_t {

public:
	void import(core::net_packet_ptr_t);
	void pack(core::net_packet_ptr_t);

public:
	int m_speed;
	int m_steer;
	double m_voltage;
	bool m_power_status;
	bool m_em_status;

};

}} //namespaces

#endif

