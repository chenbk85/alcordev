#include <boost/interprocess/sync/named_mutex.hpp>
//-----------------------------------------------------------------------++
//boost.interprocess
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <memory>
//-------------------------------------------------------------------------++
namespace ipc=boost::interprocess;
//-------------------------------------------------------------------------++
namespace all{ namespace sense{ namespace detail{
//-------------------------------------------------------------------------++
class bumblebee_ipc_recv_impl
	{
	public:
		///
		bumblebee_ipc_recv_impl()
			:
			_rgb_shm(0)
			,_rgb_region(0)
			//,_mutex_ptr(0)
			{
			//_mutex_ptr.reset(
			//	new ipc::named_mutex(ipc::open_only, 
			//	"_bumblebee_data_mutex_"));
			};
		///
		~bumblebee_ipc_recv_impl()
			{
			};
	public:
		///
		bool open_rgb(const std::string& in_name)
			{
                core::ipc_serializable_t<core::image_info_t>
                    image_info(core::open_read,"_ipc_bumblebee_rgb_info_");
                image_info_ = image_info.get_const_reference();
                //image_info.get_reference().focal;
        //////////////////////////////////////////////////////////////////
        //RGB map
			try {
			//Open already created shared memory object.
				_rgb_shm.reset(
					new ipc::shared_memory_object 
					(ipc::open_only
					,in_name.c_str()         
					,ipc::read_only 
				)
				);

			//Map the whole shared memory in this process
				_rgb_region.reset(
					new ipc::mapped_region(
					*_rgb_shm.get()                       
					,ipc::read_only )
				);

				} //try_block

		catch(ipc::interprocess_exception &ex){
			std::cout << "Unexpected exception: " << ex.what() << std::endl;
			return false;
		}//catch block
		return true;
        };
        		///
		bool open_xyz(const std::string& in_name)
        {
        //////////////////////////////////////////////////////////////////
        //Same for the XYZ map
			try {
			//Open already created shared memory object.
                _xyz_shm.reset(
					new ipc::shared_memory_object 
					(ipc::open_only
					,in_name.c_str()         
					,ipc::read_only 
				)
				);

			//Map the whole shared memory in this process
                _xyz_region.reset(
					new ipc::mapped_region(
					*_xyz_shm.get()                       
					,ipc::read_only )
				);

				} //try_block

		catch(ipc::interprocess_exception &ex){
			std::cout << "Unexpected exception: " << ex.what() << std::endl;
			return false;
		}//catch block
        return true;
        }

//////////////////////////////////////////////////////////////////
    public:
	///
	std::auto_ptr<ipc::shared_memory_object>	_rgb_shm;
	///
	std::auto_ptr<ipc::mapped_region>			_rgb_region;
    ///
    std::auto_ptr<ipc::shared_memory_object>    _xyz_shm;
    ///
	std::auto_ptr<ipc::mapped_region>			_xyz_region;
	///
	//std::auto_ptr<ipc::named_mutex>				_mutex_ptr;	
    ///
    core::image_info_t                          image_info_;
	};
//-------------------------------------------------------------------------++
	}}}
//-------------------------------------------------------------------------++