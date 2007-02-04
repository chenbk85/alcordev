#include "bumblebee_ipc_recv_t.h"
////-------------------------------------------------------------------++
#include "alcor/core/image_info_t.h"
#include "alcor/core/ipc_serializable_t.h"
////-------------------------------------------------------------------++
///impl
#include "detail/bumblebee_ipc_recv_impl.cpp"
//-------------------------------------------------------------------------++
#include <boost/type_traits/has_trivial_assign.hpp>
//-------------------------------------------------------------------------++
namespace all { 
	namespace core { 
		namespace detail {
//TODO:is this a nice solution?
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

////-------------------------------------------------------------------++
all::sense::bumblebee_ipc_recv_t::bumblebee_ipc_recv_t()
	//:impl_(0)
	{
	impl_.reset(new all::sense::detail::bumblebee_ipc_recv_impl);
	}
////-------------------------------------------------------------------++
all::sense::bumblebee_ipc_recv_t::~bumblebee_ipc_recv_t()
	{
	}
////-------------------------------------------------------------------++
bool all::sense::bumblebee_ipc_recv_t::open_rgb(const std::string& in_name)
	{
	return impl_->open_rgb(in_name);
	}
////-------------------------------------------------------------------++
bool all::sense::bumblebee_ipc_recv_t::open_xyz(const std::string& in_name)
	{
	return impl_->open_xyz(in_name);
	}
////-------------------------------------------------------------------++
void all::sense::bumblebee_ipc_recv_t::use_internal_buffers()
	{
	//right_image_buf.reset(new core::byte_t[impl_->_rgb_region->get_size()]);
        std::printf("use_internal_buffers\n");
    right_image_sptr.reset(new core::uint8_t 
                [impl_->_rgb_region->get_size()]
    );

     depth_image_sptr.reset(new core::single_t
                    [impl_->_xyz_region->get_size()]
    );
	}
////-------------------------------------------------------------------++
bool all::sense::bumblebee_ipc_recv_t::get_color(core::uint8_ptr user_buffer)
	{
		{
		try {
		void * addr       = impl_->_rgb_region->get_address();
		std::size_t size  = impl_->_rgb_region->get_size();
		///Write.....
    memcpy(user_buffer, (core::uint8_ptr)addr, size);
		} //try_block
		catch(ipc::interprocess_exception &ex){
			std::cout << "Unexpected exception: " << ex.what() << std::endl;
			return false;
		}

		}
	return true;
	}
////-------------------------------------------------------------------++
bool all::sense::bumblebee_ipc_recv_t::get_color
                    (std::vector<core::uint8_t>& user_buffer)
{
	try {
        //
		void * addr       = impl_->_rgb_region->get_address();
        //
		std::size_t size  = impl_->_rgb_region->get_size();
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
////-------------------------------------------------------------------++
//
boost::shared_array<all::core::uint8_t>
all::sense::bumblebee_ipc_recv_t::get_internal_color(sense::detail::right_cam_t)
	{
		try {
		void * addr       = impl_->_rgb_region->get_address();
		std::size_t size  = impl_->_rgb_region->get_size();
		///Write.....
        memcpy(&right_image_sptr[0], (core::uint8_ptr)addr, size);
		} //try_block
		catch(ipc::interprocess_exception &ex){
            std::cout << "get_internal_color:Unexpected exception: " 
                << ex.what() << std::endl;
			return  boost::shared_array<core::uint8_t>(0);
		}
	return right_image_sptr ;
	}
////-------------------------------------------------------------------++
bool all::sense::bumblebee_ipc_recv_t::get_depth(core::single_ptr user_buffer)
	{
	return true;
	}
    ////-------------------------------------------------------------------++
boost::shared_array<all::core::single_t>
all::sense::bumblebee_ipc_recv_t::get_internal_depth()
    {
		try {
		void * addr       = impl_->_xyz_region->get_address();
		std::size_t size  = impl_->_xyz_region->get_size();
		///Write.....
        memcpy(depth_image_sptr.get(), (core::single_ptr)addr, size);
		} //try_block
		catch(ipc::interprocess_exception &ex){
            std::cout << "get_internal_depth::Unexpected exception: " 
                << ex.what() << std::endl;
			return   boost::shared_array<core::single_t>(0);
		}
    return depth_image_sptr;
    }
////-------------------------------------------------------------------++
int all::sense::bumblebee_ipc_recv_t::height()    const
    {
    return impl_->image_info_.height;
    }
////-------------------------------------------------------------------++
int all::sense::bumblebee_ipc_recv_t::width()     const
    {
    return impl_->image_info_.width;
    }
////-------------------------------------------------------------------++
int all::sense::bumblebee_ipc_recv_t::channels()  const
    {
    return impl_->image_info_.channels;
    }
////-------------------------------------------------------------------++
double all::sense::bumblebee_ipc_recv_t::focal() const
{
  return impl_->image_info_.focal;
}
////-------------------------------------------------------------------++
    std::size_t all::sense::bumblebee_ipc_recv_t::memsize() const
    {
    return impl_->image_info_.memory_size;
    }
////-------------------------------------------------------------------++