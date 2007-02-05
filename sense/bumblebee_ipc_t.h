#ifndef bumblebee_ipc_t_H_INCLUDED
#define bumblebee_ipc_t_H_INCLUDED 
//-------------------------------------------------------------------++
//boost includes
//#include <boost/interprocess/sync/scoped_lock.hpp>
//#include <boost/interprocess/sync/named_mutex.hpp>
#include <memory>
#include <boost/scoped_ptr.hpp>
#include <boost/thread/thread.hpp>
//-------------------------------------------------------------------++
#include "alcor/sense/bumblebee_driver_t.h"
#include "alcor/core/core.h"
//-------------------------------------------------------------------++
namespace all{ 
	namespace sense{
//-------------------------------------------------------------------++
class bumblebee_ipc_t;
//-------------------------------------------------------------------++
}}
//-------------------------------------------------------------------++
class all::sense::bumblebee_ipc_t
	{
	public:
    ///
		bumblebee_ipc_t();
    ///
		~bumblebee_ipc_t();
    ///
    bool open(std::string& config_file);

	public:
    ///
		void cancel(){_running = false;};

	private:		
    ///
    void run_thread();
		//Mutex to protect access to the queue
		//boost::scoped_ptr<boost::interprocess::named_mutex> _mutex_ptr;
    ///
    boost::scoped_ptr<bumblebee_driver_t> bee;
    ///
    boost::scoped_ptr<boost::thread>      thisthread; 
    ///
		volatile bool _running;
	};
//-------------------------------------------------------------------++
//-------------------------------------------------------------------++
#endif //bumblebee_grabber_t_H_INCLUDED
