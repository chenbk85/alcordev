#ifndef ipc_mt9_recv_t_H_INCLUDED
#define ipc_mt9_recv_t_H_INCLUDED
//-------------------------------------------------------------------------++
#pragma comment(lib, "libipc_mt9_recv_t.lib")
//-------------------------------------------------------------------------++
#include "alcor/core/core.h"
#include "shared_mt9_data_t.h"
//-------------------------------------------------------------------------++
#pragma comment(lib, "libipc_mt9_recv_t.lib")
//-------------------------------------------------------------------------++
#include <string>
#include <memory>
//-------------------------------------------------------------------------++
namespace all { 
	namespace sense {
	//-------------------------++
	///Forward declared .....
	///
		namespace detail {
			class ipc_mt9_recv_impl;
			}
	class ipc_mt9_recv_t;
	//-------------------------++
	}}
//-------------------------------------------------------------------------++
using namespace all;
///
class sense::ipc_mt9_recv_t
	{
	public:
		///c'tor
		ipc_mt9_recv_t();
		~ipc_mt9_recv_t();

		///
		bool open(const std::string& in_name = "_mt9_B_euler_data_");
		///
		bool get_euler(sense::mt9_data_t& rpy);

	private:
		std::auto_ptr<detail::ipc_mt9_recv_impl> _impl;

	};
//-------------------------------------------------------------------------++
	//}}
//-------------------------------------------------------------------------++
#endif //ipc_camera_recv_t_H_INCLUDED