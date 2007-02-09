#ifndef stream_source_t_H_INCLUDED
#define stream_source_t_H_INCLUDED

#include <alcor/core/core.h>

namespace all {
	namespace core {

class stream_source_t {
public:
	virtual int get_data(all::core::uint8_ptr*) = 0;
};

}}  //namespaces

#endif