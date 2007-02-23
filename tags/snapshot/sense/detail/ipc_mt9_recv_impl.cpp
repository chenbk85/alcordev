//-------------------------------------------------------------------------++
#include "alcor/core/config_parser_t.h"
#include "alcor/core/ipc_serializable_t.h"
//-------------------------------------------------------------------------++
using namespace boost::interprocess;
//-------------------------------------------------------------------------++
#include "alcor/sense/shared_mt9_data_t.h"
#include <memory>
//-------------------------------------------------------------------------++
namespace all { namespace sense { namespace detail {
//-------------------------------------------------------------------------++
///
class ipc_mt9_recv_impl
	{
	public:
	ipc_mt9_recv_impl()
		{
			ipc_mt9.reset(new core::ipc_serializable_t<sense::mt9_data_t>
					(core::open_read,"_mt9_B_euler_data_") );

			//std::cout << "Pimpl Allocated!" << std::endl;
		};

	///
	bool _open(const std::string& in_name);

	public:
	///
		std::auto_ptr<core::ipc_serializable_t<sense::mt9_data_t> > ipc_mt9;

	};
//-------------------------------------------------------------------------++
//-------------------------------------------------------------------------++
inline bool ipc_mt9_recv_impl::_open(const std::string& in_name)
	{
		return true;
	}
//-------------------------------------------------------------------------++
	}}}
//-------------------------------------------------------------------------++