#ifndef parallel_worker_sync_t_HPP_INCLUDED
#define parallel_worker_sync_t_HPP_INCLUDED

#include <boost/thread.hpp>
#include <boost/thread/barrier.hpp>

namespace all { namespace core {
///
struct parallel_worker_sync_t {
	parallel_worker_sync_t(int count) : grab_barrier(count+1), query_barrier(count), retrieve_barrier(count+1) {}
	boost::barrier grab_barrier;
	boost::barrier query_barrier;
	boost::barrier retrieve_barrier;
};

}}//all::core

#endif //parallel_worker_sync_t_HPP_INCLUDED