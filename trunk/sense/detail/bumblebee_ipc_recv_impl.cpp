#include <boost/interprocess/sync/named_mutex.hpp>
//-----------------------------------------------------------------------++
//boost.interprocess
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <memory>
//-------------------------------------------------------------------------++
#include "alcor/core/image_info_t.h"
#include "alcor/core/ipc_serializable_t.h"
#include "alcor/core/core.h"
//-------------------------------------------------------------------------++
namespace ipc=boost::interprocess;
//-------------------------------------------------------------------------++
namespace all{ namespace sense{ namespace detail{
//-------------------------------------------------------------------------++
struct bumblebee_ipc_recv_impl
	{
	///
	bumblebee_ipc_recv_impl()
		{
		};

	///
	~bumblebee_ipc_recv_impl()
		{
		};

  void allocate_buffers()
  {
    //TODO:
    right_image_sptr.reset( 
        new core::uint8_t[image_info_.height*image_info_.width*image_info_.channels] 
    );

    left_image_sptr.reset( 
        new core::uint8_t[image_info_.height*image_info_.width*image_info_.channels] 
    );

    depth_image_sptr.reset( 
      new core::single_t[image_info_.height*image_info_.width*image_info_.channels] 
    );

  }

  void create_mutex(std::string& mutexname)
  {
    mutex_sptr.reset( new ipc::named_mutex(ipc::open_or_create, mutexname.c_str()) );
  }
  /////////////////////////////////////////////////////////////////////////
  //OPEN
  ///////////////////////////////////////////////////////////////////////////
  bool open_info(const std::string& in_name)
  {     
    try {
    all::core::ipc_serializable_t<core::image_info_t>
      image_info(core::open_read,in_name);
    image_info_ = image_info.get_reference();
      } //try_block

    catch(ipc::interprocess_exception &ex){
	    std::cout << "Unexpected exception: " << ex.what() << std::endl;
	    return false;
    }//catch block

    return true;

  }

		///OPENS RIGHT //////////////////////////////////////
  bool open_rgb_right(const std::string& in_name)
			{
        //RGB map
			try {
			//Open already created shared memory object.
				right_rgb_shm.reset(
					new ipc::shared_memory_object 
					(ipc::open_only
					,in_name.c_str()         
					,ipc::read_only 
				)
				);

			//Map the whole shared memory in this process
				right_rgb_region.reset(
					new ipc::mapped_region(
					*right_rgb_shm.get()                       
					,ipc::read_only )
				);

				} //try_block

		catch(ipc::interprocess_exception &ex){
			std::cout << "Unexpected exception: " << ex.what() << std::endl;
			return false;
		}//catch block
		return true;
        };

  ///OPENS LEFT //////////////////////////////////////
  bool open_rgb_left(const std::string& in_name)
			{
			try {
			//Open already created shared memory object.
				left_rgb_shm.reset(
					new ipc::shared_memory_object 
					(ipc::open_only
					,in_name.c_str()         
					,ipc::read_only 
				)
				);

			//Map the whole shared memory in this process
				left_rgb_region.reset(
					new ipc::mapped_region(
					*left_rgb_shm.get()                       
					,ipc::read_only )
				);

				} //try_block

		catch(ipc::interprocess_exception &ex){
			std::cout << "Unexpected exception: " << ex.what() << std::endl;
			return false;
		}//catch block
		return true;
        };


    /// OPENS XYZ //////////////////////////////////////
		bool open_xyz(const std::string& in_name)
        {
        
        //Same for the XYZ map
			try {
			//Open already created shared memory object.
          xyz_shm.reset(
					new ipc::shared_memory_object 
					(ipc::open_only
					,in_name.c_str()         
					,ipc::read_only 
				)
				);

			//Map the whole shared memory in this process
                xyz_region.reset(
					new ipc::mapped_region(
					*xyz_shm.get()                       
					,ipc::read_only )
				);

				} //try_block

		catch(ipc::interprocess_exception &ex){
			std::cout << "Unexpected exception: " << ex.what() << std::endl;
			return false;
		}//catch block
        return true;
        }

  /////////////////////////////////////////////////////////////////////////
  //GETTERS
  ///////////////////////////////////////////////////////////////////////////  
    bool get_color_(all::core::left_img_t, all::core::uint8_sarr arr)
    {
  		try 
      {
		  void * addr       = left_rgb_region->get_address();
		  std::size_t size  = left_rgb_region->get_size();
		  ///Write.....
      memcpy(left_image_sptr.get(), (core::uint8_ptr)addr, size);
      }//try_block

		  catch(ipc::interprocess_exception &ex)
      {
			  std::cout << "Unexpected exception: " << ex.what() << std::endl;
			  return false;
		  }
      arr = left_image_sptr;
	    return true;
    }


    bool get_color_(all::core::right_img_t, all::core::uint8_sarr arr)
    {
  		try 
      {
		  void * addr       = right_rgb_region->get_address();
		  std::size_t size  = right_rgb_region->get_size();
		  ///Write.....
      memcpy(right_image_sptr.get(), (core::uint8_ptr)addr, size);
      }//try_block

		  catch(ipc::interprocess_exception &ex)
      {
			  std::cout << "Unexpected exception: " << ex.what() << std::endl;
			  return false;
		  }
      arr = right_image_sptr;
	    return true;
    }

    bool get_depth_(all::core::single_sarr arr)
    {
		  try 
      {
	    void * addr       = xyz_region->get_address();
	    std::size_t size  = xyz_region->get_size();
      memcpy(depth_image_sptr.get(), (core::single_ptr)addr, size);
		  } //try_block
		  catch(ipc::interprocess_exception &ex)
      {
        std::cout << "get_internal_depth::Unexpected exception: " 
            << ex.what() << std::endl;
			  return   false;
		  }
      arr=depth_image_sptr;
    return true;
    }

		///
  all::core::uint8_sarr   left_image_sptr;		
	/////
  all::core::uint8_sarr   right_image_sptr;
	///
  all::core::single_sarr  depth_image_sptr;

  ///
  std::auto_ptr<ipc::named_mutex> mutex_sptr;//(open_or_create, "cout_named_mutex");

	///
	std::auto_ptr<ipc::shared_memory_object>	    right_rgb_shm;
	///
	std::auto_ptr<ipc::mapped_region>			        right_rgb_region;
	///
	std::auto_ptr<ipc::shared_memory_object>	    left_rgb_shm;
	///
	std::auto_ptr<ipc::mapped_region>			        left_rgb_region;
  ///
  std::auto_ptr<ipc::shared_memory_object>      xyz_shm;
    ///
	std::auto_ptr<ipc::mapped_region>			        xyz_region;
    ///
  core::image_info_t                            image_info_;
	};
//-------------------------------------------------------------------------++
	}}}
//-------------------------------------------------------------------------++