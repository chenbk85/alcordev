#ifndef capture_cam_info_t_H_INCLUDED
#define capture_cam_info_t_H_INCLUDED
//-----------------------------------------------------------------------++
#include "alcor/core/image_size_t.h"
//-----------------------------------------------------------------------++
namespace all { namespace sense {
	///
	struct  shared_cam_info_t;
	}}
//-----------------------------------------------------------------------++
#include <boost/interprocess/sync/interprocess_mutex.hpp>
//-----------------------------------------------------------------------++
///
struct all::sense::shared_cam_info_t
	{
	///
	std::string memory_tag;
	///
	std::size_t height;
	///
	std::size_t width;
	///
	std::size_t channels;
	///
	std::size_t memory_size;
	///
	core::image_size_t	image;
	///
	boost::interprocess::interprocess_mutex mutex;
	};
//-----------------------------------------------------------------------++
//-----------------------------------------------------------------------++
#endif //capture_cam_info_t_H_INCLUDED