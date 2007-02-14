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
////#####################################################################
//
void all::sense::bumblebee_ipc_recv_t::lock()
{
impl->mutex_sptr->lock();
}
////-------------------------------------------------------------------++
//
void all::sense::bumblebee_ipc_recv_t::unlock()
{
impl->mutex_sptr->unlock();
}
////-------------------------------------------------------------------++
////#####################################################################
	///
bool all::sense::bumblebee_ipc_recv_t::open(const std::string & config)
{
  bool openok = true;
  //open ini and get the name
  printf("\nOpening Camera Ini: %s\n\n", config.c_str());
  iniWrapper inifile(config.c_str());
  char* tempname   = inifile.GetStringAsChar("config:name","bubbola");
  std::string camname(tempname);         //= inifile.GetStringAsChar("config:name");

  printf("Opening Camera %s\n\n", camname.c_str());
  //
  std::string info_name       = camname + "_IPC_bumblebee_info";
  std::string left_rgb_name   = camname + "_IPC_bumblebee_rgb_left";
  std::string right_rgb_name  = camname + "_IPC_bumblebee_rgb_right";
  std::string xyz_name        = camname + "_IPC_bumblebee_xyz";
  //
  std::string shmutex         = camname + "_IPC_bumblebee_mutex";
  //
  openok = openok && ( impl->open_info(info_name) );

  //
  openok = openok && ( impl->open_rgb_right( right_rgb_name ) );
  openok = openok && ( impl->open_rgb_left( left_rgb_name ) );
  openok = openok && ( impl->open_xyz( xyz_name ) );

  if(openok)
  {
    printf("Allocating memory buffer\n\n");
    impl->allocate_buffers();
    printf("Allocating mutex \n\n");
    impl->create_mutex(shmutex);
  }

  return openok;
}
////-------------------------------------------------------------------++
///
all::core::uint8_sarr  
  all::sense::bumblebee_ipc_recv_t::get_color_buffer(core::left_img_t ,  bool shared )
{

  all::core::uint8_sarr return_sptr =
    impl->get_color_(all::core::left_img);  

  if(shared)
    return return_sptr;
  else
  {
    size_t a_size = impl->image_info_.height 
                    * impl->image_info_.width 
                    * 3;

    all::core::uint8_sarr a_copy(new all::core::uint8_t[a_size]);
  
    memcpy(a_copy.get(), return_sptr.get(), a_size);

    return a_copy;
  }

}
////-------------------------------------------------------------------++
///
all::core::uint8_sarr  
  all::sense::bumblebee_ipc_recv_t::get_color_buffer(core::right_img_t , bool shared)
{
  all::core::uint8_sarr temp =
    impl->get_color_(all::core::right_img) ;
  return temp;
}
////-------------------------------------------------------------------++
///
all::core::single_sarr 
  all::sense::bumblebee_ipc_recv_t::get_depth_buffer(bool shared)
{
  all::core::single_sarr temp =
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
