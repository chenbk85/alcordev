#include "ipc_mt9_recv_t.h"
//-------------------------------------------------------------------------++
#include "detail/ipc_mt9_recv_impl.cpp"
//-------------------------------------------------------------------------++
using namespace all;
//-------------------------------------------------------------------------++
sense::ipc_mt9_recv_t::ipc_mt9_recv_t()
	{
	_impl.reset(new sense::detail::ipc_mt9_recv_impl);
	}
//-------------------------------------------------------------------------++
sense::ipc_mt9_recv_t::~ipc_mt9_recv_t()
	{
	}
//-------------------------------------------------------------------------++
///
bool sense::ipc_mt9_recv_t::open(const std::string& in_name)
	{
	return _impl->_open(in_name);
	}
//-------------------------------------------------------------------------++
///
bool sense::ipc_mt9_recv_t::get_euler(sense::mt9_data_t& rpy)
	{
		//ENTER_CRITICAL(_impl->ipc_mt9)
        rpy = _impl->ipc_mt9->get_const_reference();
		//rpy.roll = _impl->ipc_mt9->get_reference().roll;
		//rpy.elapsed = _impl->ipc_mt9->get_reference().elapsed;
		//LEAVE_CRITICAL

	return true;
	}
//-------------------------------------------------------------------------++
