#include "ipc_camera_recv_t.h"
//-------------------------------------------------------------------------++
#include "detail/ipc_camera_recv_impl.cpp"
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/type_traits/has_trivial_assign.hpp>
//-------------------------------------------------------------------------++
//
// opt::copy
// same semantics as std::copy
// calls memcpy where appropriate.
//
//-------------------------------------------------------------------------++
namespace all { 
	namespace core { 
		namespace detail {
//-------------------------------------------------------------------------++
template<typename I1, typename I2, bool b>
I2 copy_imp(I1 first, I1 last, I2 out, const boost::integral_constant<bool, b>&)
{
   while(first != last)
   {
      *out = *first;
      ++out;
      ++first;
   }
   return out;
}
//-------------------------------------------------------------------------++
template<typename T>
T* copy_imp(const T* first, const T* last, T* out, const boost::true_type&)
{
   memcpy(out, first, (last-first)*sizeof(T));
   return out+(last-first);
}
//-------------------------------------------------------------------------++
//-------------------------------------------------------------------------++
}//namespace detail
//-------------------------------------------------------------------------++
template<typename I1, typename I2>
inline I2 copy(I1 first, I1 last, I2 out)
{
   //
   // We can copy with memcpy if T has a trivial assignment operator,
   // and if the iterator arguments are actually pointers (this last
   // requirement we detect with overload resolution):
   //
   typedef typename std::iterator_traits<I1>::value_type value_type;
   return detail::copy_imp(first, last, out, boost::has_trivial_assign<value_type>());
}
//-------------------------------------------------------------------------++
}}//namespace all::core
//-------------------------------------------------------------------------++
#include <algorithm>
//-------------------------------------------------------------------------++
all::sense::ipc_camera_recv_t::ipc_camera_recv_t()
//:image_buffer_(0)
	{
	impl_.reset(new ipc_camera_recv_impl);

	}
//-------------------------------------------------------------------------++
all::sense::ipc_camera_recv_t::~ipc_camera_recv_t()
	{

	}
//-------------------------------------------------------------------------++
bool all::sense::ipc_camera_recv_t::open(const std::string& in_name)
	{
	return impl_->open(in_name);
	}
//-------------------------------------------------------------------------++
bool all::sense::ipc_camera_recv_t::get_image_buffer(all::core::uint8_ptr user_buffer)
	{
	try {
		void * addr  = impl_->_data_region->get_address();
		//
		std::size_t size  = impl_->_data_region->get_size();
        //
    core::copy<all::core::uint8_ptr,all::core::uint8_ptr>(  (core::uint8_ptr)addr
      ,(core::uint8_ptr)addr + size
                    ,&user_buffer[0]);

		}//try_block
	catch(boost::interprocess::interprocess_exception &ex)
		{
			std::cout << "Unexpected exception: " << ex.what() << std::endl;
			return false;
		}

	return true;
	}
//-------------------------------------------------------------------------++
bool all::sense::ipc_camera_recv_t::get_image_buffer2(all::core::uint8_ptr user_buffer)
	{
	try {
		void * addr  = impl_->_data_region->get_address();
		//
		std::size_t size  = impl_->_data_region->get_size();
        //
        core::copy<all::core::uint8_ptr,
            all::core::uint8_ptr>(  (core::uint8_ptr)addr
            ,(core::uint8_ptr)addr + size
                    ,&user_buffer[0]);

		}//try_block
	catch(boost::interprocess::interprocess_exception &ex)
		{
			std::cout << "Unexpected exception: " << ex.what() << std::endl;
			return false;
		}

	return true;
	}
//-------------------------------------------------------------------------++
bool 
all::sense::ipc_camera_recv_t::
get_image_buffer(std::vector<core::uint8_t>& user_buffer)
	{
	try {
		//
		void * addr  = impl_->_data_region->get_address();
		//
		std::size_t size  = impl_->_data_region->get_size();
		//
        if(user_buffer.size() != size)
            {
				user_buffer.resize(size);
            }

        core::copy< all::core::uint8_ptr,
            all::core::uint8_ptr>(  (core::uint8_ptr)addr
            ,(core::uint8_ptr)addr + size
                    ,&user_buffer[0]);

		}//try_block
	catch(boost::interprocess::interprocess_exception &ex)
		{
			std::cout << "Unexpected exception: " << ex.what() << std::endl;
			return false;
		}

	return true;
	}
//-------------------------------------------------------------------------++
///
void all::sense::ipc_camera_recv_t::use_internal_buffer()
	{
    //
        v_image_buffer_.resize(get_size());
	}
//-------------------------------------------------------------------------++
///
all::core::uint8_ptr all::sense::ipc_camera_recv_t::get_internal_buffer() 
	{

	try {
        //
		void* addr  = impl_->_data_region->get_address();
		//
		std::size_t size  = impl_->_data_region->get_size();
        //
        core::copy< all::core::uint8_ptr,
            all::core::uint8_ptr>(  (core::uint8_ptr)addr
            ,(core::uint8_ptr)addr + size
                    ,&v_image_buffer_[0]);
		//
        return &v_image_buffer_[0];

		}//try_block
	catch(boost::interprocess::interprocess_exception &ex)
		{
			std::cout << "Unexpected exception: " << ex.what() << std::endl;
			return 0;
		}
	}
//-------------------------------------------------------------------------++
std::size_t all::sense::ipc_camera_recv_t::get_size() const
	{
		return impl_->image_info_obj.memory_size;
	}
//-------------------------------------------------------------------------++
///
std::size_t all::sense::ipc_camera_recv_t::get_width() const
	{
		return (std::size_t)impl_->image_info_obj.width;
	}
//-------------------------------------------------------------------------++
///
std::size_t all::sense::ipc_camera_recv_t::get_nchannels() const
	{
		return (std::size_t)impl_->image_info_obj.channels;
	}
//-------------------------------------------------------------------------++
///
std::size_t all::sense::ipc_camera_recv_t::get_height() const
	{
		return (std::size_t)impl_->image_info_obj.height;
	}
//-------------------------------------------------------------------------++