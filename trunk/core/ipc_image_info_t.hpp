#ifndef ipc_image_info_t_HPP_INCLUDED
#define ipc_image_info_t_HPP_INCLUDED

#include <boost/interprocess/sync/interprocess_mutex.hpp>

namespace all {
	namespace core {

struct ipc_image_info_t {
	
	int width;
	int height;
	int channel;

	boost::interprocess::interprocess_mutex mutex;
};

}}
#endif
