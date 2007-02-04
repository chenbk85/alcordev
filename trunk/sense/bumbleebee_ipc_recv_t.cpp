#include "bumblebee_ipc_recv_t.h"
////-------------------------------------------------------------------++
#include "alcor/core/image_info_t.h"
#include "alcor/core/ipc_serializable_t.h"
#include "alcor/core/iniWrapper.h"
////-------------------------------------------------------------------++
///impl
#include "detail/bumblebee_ipc_recv_impl.cpp"
//-------------------------------------------------------------------------++
//#include <boost/type_traits/has_trivial_assign.hpp>
////-------------------------------------------------------------------------++
//namespace all { 
//	namespace core { 
//		namespace detail {
////TODO:is this a nice solution?
////-------------------------------------------------------------------------++
//template<typename I1, typename I2, bool b>
//I2 copy_imp(I1 first, I1 last, I2 out, const boost::integral_constant<bool, b>&)
//{
//   while(first != last)
//   {
//      *out = *first;
//      ++out;
//      ++first;
//   }
//   return out;
//}
////-------------------------------------------------------------------------++
//template<typename T>
//T* copy_imp(const T* first, const T* last, T* out, const boost::true_type&)
//{
//   memcpy(out, first, (last-first)*sizeof(T));
//   return out+(last-first);
//}
////-------------------------------------------------------------------------++
////-------------------------------------------------------------------------++
//}//namespace detail
////-------------------------------------------------------------------------++
//template<typename I1, typename I2>
//inline I2 copy(I1 first, I1 last, I2 out)
//{
//   //
//   // We can copy with memcpy if T has a trivial assignment operator,
//   // and if the iterator arguments are actually pointers (this last
//   // requirement we detect with overload resolution):
//   //
//   typedef typename std::iterator_traits<I1>::value_type value_type;
//   return detail::copy_imp(first, last, out, boost::has_trivial_assign<value_type>());
//}
////-------------------------------------------------------------------------++
//}}//namespace all::core
//-------------------------------------------------------------------------++

////-------------------------------------------------------------------++
all::sense::bumblebee_ipc_recv_t::bumblebee_ipc_recv_t()
	//:impl(0)
	{
	impl.reset(new all::sense::detail::bumblebee_ipc_recv_impl);
	}
////-------------------------------------------------------------------++
all::sense::bumblebee_ipc_recv_t::~bumblebee_ipc_recv_t()
	{
	}
////-------------------------------------------------------------------++
////#####################################################################
	///
bool all::sense::bumblebee_ipc_recv_t::open(const std::string & config)
{
  bool openok = true;
  //open ini and get the name
  iniWrapper inifile(config.c_str());
  std::string camname = inifile.GetString("config:name");
  std::string info_name       = camname + "_IPC_bumblebee_info";
  std::string left_rgb_name   = camname + "_IPC_bumblebee_rgb_left";
  std::string right_rgb_name  = camname + "_IPC_bumblebee_rgb_right";
  std::string xyz_name        = camname + "_IPC_bumblebee_xyz";

  //
  openok = openok && ( impl->open_info(info_name) );


  openok = openok && ( impl->open_rgb_right(right_rgb_name ) );
  openok = openok && ( impl->open_rgb_left(left_rgb_name ) );
  openok = openok && ( impl->open_xyz(xyz_name ) );

  if(openok)
  {
    //allocate
    allocate_();
  }

  return openok;
}
////-------------------------------------------------------------------++
void 
all::sense::bumblebee_ipc_recv_t::allocate_()
{
//
  impl->allocate_buffers();
}
////-------------------------------------------------------------------++
///
all::core::uint8_sarr  
  all::sense::bumblebee_ipc_recv_t::get_color_buffer(core::left_img_t ,  bool shared )
{
  return impl->left_image_sptr;
}
////-------------------------------------------------------------------++
///
all::core::uint8_sarr  
  all::sense::bumblebee_ipc_recv_t::get_color_buffer(core::right_img_t , bool shared)
{
  return impl->right_image_sptr;
}
////-------------------------------------------------------------------++
///
all::core::single_sarr 
  all::sense::bumblebee_ipc_recv_t::get_depth_buffer(bool shared)
{
  return impl->depth_image_sptr;
}
////#####################################################################
//void all::sense::bumblebee_ipc_recv_t::use_internal_buffers()
//	{
//
//        std::printf("use_internal_buffers\n");
//    right_image_sptr.reset(new core::uint8_t 
//                [impl->_rgb_region->get_size()]
//    );
//
//     depth_image_sptr.reset(new core::single_t
//                    [impl->_xyz_region->get_size()]
//    );
//	}
//////-------------------------------------------------------------------++
//bool all::sense::bumblebee_ipc_recv_t::get_color(core::uint8_ptr user_buffer)
//	{
//		{
//		try {
//		void * addr       = impl->_rgb_region->get_address();
//		std::size_t size  = impl->_rgb_region->get_size();
//		///Write.....
//    memcpy(user_buffer, (core::uint8_ptr)addr, size);
//		} //try_block
//		catch(ipc::interprocess_exception &ex){
//			std::cout << "Unexpected exception: " << ex.what() << std::endl;
//			return false;
//		}
//
//		}
//	return true;
//	}
//////-------------------------------------------------------------------++
//bool all::sense::bumblebee_ipc_recv_t::get_color
//                    (std::vector<core::uint8_t>& user_buffer)
//{
//	try {
//        //
//		void * addr       = impl->_rgb_region->get_address();
//        //
//		std::size_t size  = impl->_rgb_region->get_size();
//		//
//        if(user_buffer.size() != size)
//            {
//				user_buffer.resize(size);
//            }
//
//        core::copy< all::core::uint8_ptr,
//            all::core::uint8_ptr>(  (core::uint8_ptr)addr
//            ,(core::uint8_ptr)addr + size
//                    ,&user_buffer[0]);
//
//		}//try_block
//	catch(boost::interprocess::interprocess_exception &ex)
//		{
//			std::cout << "Unexpected exception: " << ex.what() << std::endl;
//			return false;
//		}
//
//	return true;
//}
//////-------------------------------------------------------------------++
////
//boost::shared_array<all::core::uint8_t>
//all::sense::bumblebee_ipc_recv_t::get_internal_color(sense::detail::right_cam_t)
//	{
//		try {
//		void * addr       = impl->_rgb_region->get_address();
//		std::size_t size  = impl->_rgb_region->get_size();
//		///Write.....
//        memcpy(&right_image_sptr[0], (core::uint8_ptr)addr, size);
//		} //try_block
//		catch(ipc::interprocess_exception &ex){
//            std::cout << "get_internal_color:Unexpected exception: " 
//                << ex.what() << std::endl;
//			return  boost::shared_array<core::uint8_t>(0);
//		}
//	return right_image_sptr ;
//	}
//////-------------------------------------------------------------------++
//bool all::sense::bumblebee_ipc_recv_t::get_depth(core::single_ptr user_buffer)
//	{
//	return true;
//	}
//    ////-------------------------------------------------------------------++
//boost::shared_array<all::core::single_t>
//all::sense::bumblebee_ipc_recv_t::get_internal_depth()
//    {
//		try {
//		void * addr       = impl->_xyz_region->get_address();
//		std::size_t size  = impl->_xyz_region->get_size();
//		///Write.....
//        memcpy(depth_image_sptr.get(), (core::single_ptr)addr, size);
//		} //try_block
//		catch(ipc::interprocess_exception &ex){
//            std::cout << "get_internal_depth::Unexpected exception: " 
//                << ex.what() << std::endl;
//			return   boost::shared_array<core::single_t>(0);
//		}
//    return depth_image_sptr;
//    }
////-------------------------------------------------------------------++
int all::sense::bumblebee_ipc_recv_t::height()    const
    {
    return impl->image_info_.height;
    }
////-------------------------------------------------------------------++
int all::sense::bumblebee_ipc_recv_t::width()     const
    {
    return impl->image_info_.width;
    }
////-------------------------------------------------------------------++
int all::sense::bumblebee_ipc_recv_t::channels()  const
    {
    return impl->image_info_.channels;
    }
////-------------------------------------------------------------------++
float all::sense::bumblebee_ipc_recv_t::focal() const
{
  return impl->image_info_.focal;
}
//////-------------------------------------------------------------------++
//    std::size_t all::sense::bumblebee_ipc_recv_t::memsize() const
//    {
//    return impl->image_info_.memory_size;
//    }
////-------------------------------------------------------------------++