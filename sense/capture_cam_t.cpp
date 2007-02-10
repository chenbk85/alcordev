#include "capture_cam_t.h"
#include "alcor/core/image_info_t.h"
#include "alcor/core/ipc_serializable_t.h"
//-----------------------------------------------------------------------++
#include "opencv_grabber_t.h"
#include "alcor/core/iniwrapper.h"
//-----------------------------------------------------------------------++
#include <boost/thread/thread.hpp>
//-----------------------------------------------------------------------++
#ifdef WIN32
#include <windows.h>
#endif // WIN32
//-----------------------------------------------------------------------++
//boost.interprocess
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
//-----------------------------------------------------------------------++
using boost::interprocess::scoped_lock;
//-----------------------------------------------------------------------++
using namespace boost::interprocess;
//-----------------------------------------------------------------------++
#include <iostream>
using std::cout;
using std::endl;
//-----------------------------------------------------------------------++
all::sense::capture_cam_t::capture_cam_t(const std::string& xml)
				: _running(true)
				,camera_mode(true)
                ,cam_(-1)
				,_buffer(0)
				
	{
	_impl.reset(new all::sense::opencv_grabber_t);
   open_conf_(xml);
   	//boost::thread capture_thr
	//	(
	//	boost::bind(
 //           &sense::capture_cam_t::run_thread
	//	    ,&capture_cam) 
	//	);
	}
//-----------------------------------------------------------------------++
all::sense::capture_cam_t::~capture_cam_t()
	{
	close();
	}
//-----------------------------------------------------------------------++
///
void all::sense::capture_cam_t::open_conf_(const std::string& _xmlfile)
    {
	core::config_parser_t conf;
    conf.load(core::xml,_xmlfile);
    if (conf.as_int("grabber.cameramode",1))
        {
        camera_mode = true;
        cam_ = conf.as_int("grabber.id_cam",-1);
        }
    else
        {
        //open video
        camera_mode = false;
        video_name_ = conf.as_string("grabber.videoname","video.avi");
        }
	info_obj_tag = conf.as_string("grabber.info_mem_tag", "_ipc_capture_info_");
	mem_obj_tag  = conf.as_string("grabber.image_mem_tag","_ipc_capture_buffer_");
    }
//-----------------------------------------------------------------------++
///
bool all::sense::capture_cam_t::_open(core::detail::camera_mode_t, int in_cam)
	{
	if(_impl->open(core::open_camera, in_cam))
		{
		cout << "Allocating buffer" << endl;
    _buffer.reset( new core::uint8_t[_impl->size()]);
		return true;
		}
	else
		{
		//_buffer = 0;
		return false;
		}
	}
//-----------------------------------------------------------------------++
///
bool all::sense::capture_cam_t::_open(core::detail::video_mode_t, 
                                      const std::string& _name)
	{
	if(_impl->open(core::open_video, _name))
		{
		cout << "Allocating buffer" << endl;
		_buffer.reset(
      new core::uint8_t[
				_impl->size()]);
		return true;

		}
	else
		{
		//_buffer = 0;
		return false;
		}
	}
//-----------------------------------------------------------------------++
	///
bool all::sense::capture_cam_t::close()
	{
	return _impl->close();
	}
//---------------------------------------------------------------------++
void all::sense::capture_cam_t::run_thread()
	{
	//Open the grabber ... (In this Thread!!)
	bool is_open;
	if(camera_mode)
		is_open = _open(core::open_camera);
	else
		is_open = _open(core::open_video, video_name_);

	if(is_open)
		{
		//std::string  info_obj_tag = "_ipc_capture_info_";
		//std::string  mem_obj_tag  = "_capture_buffer_";

		shared_memory_object::remove(info_obj_tag.c_str());

		//////////////////////////////////////////////////////////////
		core::ipc_serializable_t<core::image_info_t> image_info(core::open_write,info_obj_tag);
		//
        image_info.get_ref().height			  = _impl->height();
        image_info.get_ref().width			  = _impl->width();
        image_info.get_ref().channels		  = _impl->channels();
        image_info.get_ref().memory_size	= _impl->size();
        //////////////////////////////////////////////////////////////
        //////now allocate the image in memory
		//core::ipc_serializable_t<core::byte_image_640_t> image_data(core::open_write,"_ipc_dummy_image__");
		//////////////////////////////////////////////////////////////
			///Allocate memory for data....
		shared_memory_object::remove(mem_obj_tag.c_str());

		cout << "Creating Shared Memory" << endl;
		//Create a shared memory object.
		shared_memory_object mem_obj
			(open_or_create                  
			,mem_obj_tag.c_str()              //name
			,read_write//read-write mode
			);

		//Set the size of the shared memory segment
		mem_obj.truncate(_impl->size());

		//Map the whole shared memory in this process
		mapped_region mem_region
			(mem_obj                       //What to map
			,read_write //Map it as read-write
			);
		cout << "--> Shared Memory Created" << endl;
		//Get the address of the mapped region
    core::uint8_ptr mem_addr  = (core::uint8_ptr)mem_region.get_address();

			///Thread loop
			cout << "Entering capture loop" << endl;

		while (_running)
			{
			//cout << "Loop" << endl;
			if(_impl->get_color_buffer(_buffer.get()))
				{
				memcpy(mem_addr, _buffer.get(), _impl->size());
				}
			else
				{
				cout << "!** Error ... not able to capture\n";
				_running = false;
				}

				boost::thread::yield();
				#ifdef WIN32
				Sleep(30);
				#endif // WIN32

			}//while

			if(shared_memory_object::remove(mem_obj_tag.c_str()))
				std::cout<< "Memory Successfully Removed" << std::endl;

			if(shared_memory_object::remove(info_obj_tag.c_str()))
				std::cout<< "Memory Successfully Removed" << std::endl;

			close();

			}//_open
		cout << "-->Out of loop.." << endl;
	}	//_open	
//-----------------------------------------------------------------------++

