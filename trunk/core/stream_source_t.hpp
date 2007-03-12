#ifndef stream_source_t_H_INCLUDED
#define stream_source_t_H_INCLUDED

#include <alcor/core/core.h>
#include <alcor/core/net_packet_t.hpp>

namespace all {
	namespace core {

class stream_source_t {

public:

	virtual int get_data(all::core::uint8_ptr*) = 0;
	
	virtual void process_command(net_packet_ptr_t) = 0;

private:

};

typedef boost::shared_ptr <stream_source_t> stream_source_ptr;

}}  //namespaces

#endif