#ifndef memory_tags_H_INC
#define memory_tags_H_INC
//-------------------------------------------------------------------------++
//Creation Tags
namespace all { namespace core {
//-------------------------------------------------------------------------++
	namespace detail
		{
struct open_t {};
struct read_t {};
		}//detail
//-------------------------------------------------------------------------++
static const core::detail::open_t	ipc_open   = core::detail::open_t();
static const core::detail::open_t	open_write = core::detail::open_t();
static const core::detail::read_t	ipc_read   = core::detail::read_t();
static const core::detail::read_t	open_read   = core::detail::read_t();
//-------------------------------------------------------------------------++
}}//all::core
//-------------------------------------------------------------------------++
#endif //memory_tags_H_INC