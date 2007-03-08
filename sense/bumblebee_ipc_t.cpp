#include "bumblebee_ipc_t.h"
#include "alcor.extern/CImg/CImg.h"
////-------------------------------------------------------------------++
#include <boost/bind.hpp>
#include "alcor/core/image_info_t.h"
#include "alcor/core/ipc_serializable_t.h"
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
//-------------------------------------------------------------------++
namespace ipc = boost::interprocess;
//-------------------------------------------------------------------++
all::sense::bumblebee_ipc_t::bumblebee_ipc_t():
		_running(true)
	{
	}
//-------------------------------------------------------------------++
bool  all::sense::bumblebee_ipc_t::open()
{
  //bee.reset(new     bumblebee_driver_t());

  //printf( "\nOpening: %s\n\n", config_file.c_str() );

  //if( bee->open(config_file) )
  //{
  //  printf("\nBumblebee camera: %s Opened!\n", bee->name().c_str());

    printf("Starting Thread!\n");
    thisthread.reset(
          new boost::thread 
          (
          boost::bind(
          &all::sense::bumblebee_ipc_t::run_thread
          , this) 
          )
      );
    return true;
  //}
  //else
  //{
  //  printf("ERROR: Bumblebee camera Not Opened!\n");
  //  return false;
  //}
  
}     
//-------------------------------------------------------------------++
all::sense::bumblebee_ipc_t::~bumblebee_ipc_t()
	{
	_running = false;
	Sleep(50);
	}
//-------------------------------------------------------------------++

void all::sense::bumblebee_ipc_t::assign_bumblebee(boost::shared_ptr<bumblebee_driver_t> bptr)
{
bee = bptr;
}
////-------------------------------------------------------------------++
void all::sense::bumblebee_ipc_t::run_thread()
	{

  //--------------------------------------------------------------------
  std::string camname = bee->name();
//////////////////////////////////////////////////////////////////////
//names: hardcoded this class serves only bumblebees...
  std::string info_name       = camname + "_IPC_bumblebee_info";
  std::string left_rgb_name   = camname + "_IPC_bumblebee_rgb_left";
  std::string right_rgb_name  = camname + "_IPC_bumblebee_rgb_right";
  std::string xyz_name        = camname + "_IPC_bumblebee_xyz";
  std::string shmutex         = camname + "_IPC_bumblebee_mutex";
//////////////////////////////////////////////////////////////////////
  ipc::shared_memory_object::remove(right_rgb_name.c_str());
  ipc::shared_memory_object::remove(xyz_name.c_str());
  ipc::shared_memory_object::remove(left_rgb_name.c_str());
  ipc::named_mutex::remove(shmutex.c_str());
//////////////////////////////////////////////////////////////////////
  //RGB INFO
  core::ipc_serializable_t<core::image_info_t> image_info(core::open_write,
                                                          info_name);
  //
  image_info.get_reference().height       =  bee->nrows();
  image_info.get_reference().width        =  bee->ncols();
  image_info.get_reference().channels     =  3;
  image_info.get_reference().focal        =  bee->focal();
  printf("Focal is: %f\n", bee->focal() );
  //////////////////////////////////////////////////////////////////////
  //RIGHT RGB IMAGE
  ipc::shared_memory_object::remove( right_rgb_name.c_str() );
  //Create a shared memory object.
  ipc::shared_memory_object right_shm_rgb
       (ipc::open_or_create                  //only open
        ,right_rgb_name.c_str()             //name
        ,ipc::read_write  //read-write mode
       );
  //Set the size of the shared memory segment
  right_shm_rgb.truncate(bee->ncols()*bee->nrows()*3);

  //Map the whole shared memory in this process
  ipc::mapped_region right_rgb_region
       (right_shm_rgb                       //What to map
       ,ipc::read_write //Map it as read-write
       );
  //////////////////////////////////////////////////////////////////////
  //LEFT RGB IMAGE
  ipc::shared_memory_object::remove( left_rgb_name.c_str() );
  //Create a shared memory object.
  ipc::shared_memory_object left_shm_rgb
       (ipc::open_or_create                  //only open
        ,left_rgb_name.c_str()             //name
        ,ipc::read_write  //read-write mode
       );
  //Set the size of the shared memory segment
  left_shm_rgb.truncate(bee->ncols()*bee->nrows()*3);

  //Map the whole shared memory in this process
  ipc::mapped_region left_rgb_region
       (left_shm_rgb                       //What to map
       ,ipc::read_write //Map it as read-write
       );
  //////////////////////////////////////////////////////////////////////
  //XYZ IMAGE
  ipc::shared_memory_object::remove(xyz_name.c_str());
  //Create a shared memory object.
  ipc::shared_memory_object shm_xyz
       (ipc::open_or_create                  //only open
        ,xyz_name.c_str()              //name
	      ,ipc::read_write  //read-write mode
       );
  //Set the size of the shared memory segment
  shm_xyz.truncate(bee->ncols()*bee->nrows()*core::traits<core::single_t>::size*3);

  //Map the whole shared memory in this process
  ipc::mapped_region xyz_region
       (shm_xyz                      //What to map
       ,ipc::read_write //Map it as read-write
       );

  //Get the address of the mapped region
  core::uint8_ptr  left_rgb_addr    = (core::uint8_ptr)  left_rgb_region.get_address();
  core::uint8_ptr  right_rgb_addr   = (core::uint8_ptr)  right_rgb_region.get_address();
  core::single_ptr xyz_addr  = (core::single_ptr) xyz_region.get_address();
    ///////////////////////////////////////////////////////////////////////
  const std::size_t    r_rgb_imag_size      =  right_rgb_region.get_size();
  const std::size_t    l_rgb_imag_size      =  left_rgb_region.get_size();
  const std::size_t    xyz_imag_size        =  xyz_region.get_size();
	///////////////////////////////////////////////////////////////////////
  ipc::named_mutex mutex(ipc::open_or_create, shmutex.c_str());
  //printf("IPC Mutex: %s\n",shmutex.c_str());
  ///////////////////////////////////////////////////////////////////////
  cimg_library::CImgDisplay dispr(bee->ncols(), bee->nrows(), "Bumblebee ipc /Right");
  cimg_library::CImg<core::uint8_t> right;
   //in the thread_loop
	while (_running)
		{
      //printf("Run\n");
      ///////////////////////////////////////////////////////////////////
     if (bee->grab())//acquisition
     {
       //ipc::scoped_lock<ipc::named_mutex> lock(mutex);
       //boost::scoped_lock< > lock(mutex);
       //RIGHT
		    {
        memcpy(right_rgb_addr, 
          bee->get_color_buffer(core::right_img).get(), 
          r_rgb_imag_size );
          right.assign(right_rgb_addr, bee->ncols(),bee->nrows(),1,3);
          right.display(dispr);
		    }
         //LEFT
		    {
        memcpy(left_rgb_addr, 
          bee->get_color_buffer(core::left_img).get(), 
          l_rgb_imag_size );
		    }
        //DMAP
        {
        memcpy( xyz_addr, 
                bee->get_depth_buffer().get() ,
                xyz_imag_size );
        }

       //mutex.unlock();

     }//grab
        //
        ///////////////////////////////////////////////////////////////////
		boost::thread::yield();
    core::BOOST_SLEEP(50);
		}//_running ... out of thread loop...
	///////////////////////////////////////////////////////////////////////
    ipc::shared_memory_object::remove(right_rgb_name.c_str());
    ipc::shared_memory_object::remove(xyz_name.c_str());
    ipc::shared_memory_object::remove(left_rgb_name.c_str());
    ipc::named_mutex::remove(shmutex.c_str());
    thisthread->join();
	}
//-------------------------------------------------------------------++
