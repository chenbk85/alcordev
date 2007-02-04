#include "bumblebee_ipc_recv_t.h"
////-------------------------------------------------------------------++
#include "alcor/core/image_info_t.h"
#include "alcor/core/ipc_serializable_t.h"
#include "alcor/core/iniWrapper.h"
////-------------------------------------------------------------------++
///impl
#include "detail/bumblebee_ipc_recv_impl.cpp"
//-------------------------------------------------------------------------++
////-------------------------------------------------------------------++
all::sense::bumblebee_ipc_recv_t::bumblebee_ipc_recv_t()
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
  std::string camname         = inifile.GetString("config:name");
  std::string info_name       = camname + "_IPC_bumblebee_info";
  std::string left_rgb_name   = camname + "_IPC_bumblebee_rgb_left";
  std::string right_rgb_name  = camname + "_IPC_bumblebee_rgb_right";
  std::string xyz_name        = camname + "_IPC_bumblebee_xyz";

  //
  openok = openok && ( impl->open_info(info_name) );

  //
  openok = openok && ( impl->open_rgb_right( right_rgb_name ) );
  openok = openok && ( impl->open_rgb_left( left_rgb_name ) );
  openok = openok && ( impl->open_xyz( xyz_name ) );

  if(openok)
  {
    impl->allocate_buffers();
  }

  return openok;
}
////-------------------------------------------------------------------++
///
all::core::uint8_sarr  
  all::sense::bumblebee_ipc_recv_t::get_color_buffer(core::left_img_t ,  bool shared )
{
  all::core::uint8_sarr temp;
  impl->get_color_(all::core::left_img,  temp);
  return temp;
}
////-------------------------------------------------------------------++
///
all::core::uint8_sarr  
  all::sense::bumblebee_ipc_recv_t::get_color_buffer(core::right_img_t , bool shared)
{
  all::core::uint8_sarr temp;
  impl->get_color_(all::core::right_img,  temp);
  return temp;
}
////-------------------------------------------------------------------++
///
all::core::single_sarr 
  all::sense::bumblebee_ipc_recv_t::get_depth_buffer(bool shared)
{
  all::core::single_sarr temp;
  impl->get_depth_(temp);
  return temp;
}
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
