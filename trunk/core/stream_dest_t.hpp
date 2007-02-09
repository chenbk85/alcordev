#ifndef stream_dest_t_H_INCLUDED
#define stream_dest_t_H_INCLUDED

#include <alcor/core/core.h>
#include <boost/shared_array.hpp>

namespace all {
	namespace core {

class stream_dest_t {

public:
	stream_dest_t(){m_data_size = 0;}

	void import_data(all::core::uint8_sarr data, std::size_t data_size) {m_data = data; m_data_size = data_size; process_data();}

	virtual void process_data() = 0;

	all::core::uint8_sarr get_data() {return m_data;}
	std::size_t get_data_size() {return m_data_size;}

protected:
	
	all::core::uint8_sarr m_data;
	std::size_t m_data_size;
};

}} //namespaces

#endif