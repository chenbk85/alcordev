//boost.interprocess
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
using namespace boost::interprocess;
//-------------------------------------------------------------------------++
#include "alcor/core/image_info_t.h"
#include "alcor/core/ipc_serializable_t.h"
#include "alcor/core/ipc_serializable_image.hpp"
#include "alcor/core/config_parser_t.h"
//-------------------------------------------------------------------------++
class ipc_camera_recv_impl
	{
	public:
	///
		ipc_camera_recv_impl()//:_ptr_info(0)
		{

		};
	///
	~ipc_camera_recv_impl()
		{

		};

	///
	bool open(const std::string& in_name)
		{
			all::core::config_parser_t conf;
			conf.load(all::core::xml,in_name);

		if (conf.as_int("grabber.cameramode", 1))
			{
			printf("Receiving From Camera ...\n");
			}
		else
			{
			printf("Receiving A Video Source ...\n");		
			}

	std::string  info_obj_tag = 
		conf.as_string("grabber.info_mem_tag", "_ipc_capture_info_");
	std::string  mem_obj_tag  = 
		conf.as_string("grabber.image_mem_tag","_ipc_capture_buffer_");

		try {

			all::core::ipc_serializable_t<all::core::image_info_t> 
				image_info(all::core::open_read,info_obj_tag);

			image_info_obj = image_info.get_const_reference();

            //height_ = image_info.get_const().height;
            //width_ = image_info.get_const().width;
            //channels_ = image_info.get_const().channels;
            //memory_size_ = image_info.get_const().memory_size;

		////////////////////////////////////////////////////
			//all::core::ipc_serializable_t<all::core::byte_image_640_t> 
			//	ipc_image(all::core::open_read,"_ipc_dummy_image__");
		////////////////////////////////////////////////////
		///Open DATA
			//Open already created shared memory object.
		 _data_mem.reset( new shared_memory_object 
				(open_only
				,mem_obj_tag.c_str()  //name
				,read_only)
			);
			
		//Map the whole shared memory in this process
		 _data_region.reset( new mapped_region
				(*_data_mem.get()                       //What to map
			,read_only ) //Map it as read-only
			);

			} //try_block

	catch(interprocess_exception &ex){
		std::cout << "Unexpected exception: " << ex.what() << std::endl;
		return false;
	}//catch block
		
		return true;
		};

	///
	all::core::image_info_t image_info_obj;
	///
	std::auto_ptr<shared_memory_object> _data_mem;
	///
	std::auto_ptr<mapped_region>		_data_region;
	};
//-------------------------------------------------------------------------++