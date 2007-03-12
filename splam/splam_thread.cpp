#include "splam_thread.h"
#include "detail/splam_thread_impl.cpp"
//-----------------------------------------------------------------------------------------------
namespace all{
namespace splam{

splam_thread::splam_thread(const char* name)
{
	pImpl_.reset(new splam_thread_impl(name));
}

splam_thread::~splam_thread()
{
	pImpl_->stop_server();
	pImpl_->stop();
}

void	splam_thread::run()
{
	pImpl_->runAsync();
}

void	splam_thread::stop()
{
	pImpl_->stop();
}

void	splam_thread::start_server()
{
	pImpl_->start_server();
}

void	splam_thread::stop_server()
{
	pImpl_->stop_server();
}

}//namespace splam
}//namespace all
//-----------------------------------------------------------------------------------------------
