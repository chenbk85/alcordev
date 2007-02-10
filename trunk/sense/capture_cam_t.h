#ifndef capture_cam_t_H_INCLUDED
#define capture_cam_t_H_INCLUDED
//-------------------------------------------------------------------------++
namespace all { namespace sense {
class opencv_grabber_t;
class capture_cam_t;
	}}
//-------------------------------------------------------------------------++
#include "alcor/sense/i_device_driver.h"
#include "alcor/core/core.h"
#include "alcor/core/detail/grabber_mode_tags.hpp"
//#define BOOST_DATE_AND_TIME_NO_LIB
//-------------------------------------------------------------------------++
#include <memory>
//-------------------------------------------------------------------------++
///forward
//-------------------------------------------------------------------------++
///
class all::sense::capture_cam_t : public i_device_driver
	{
	public:
		///
    capture_cam_t(const std::string& xml = "xmlconf/grabber.xml");
		///
		~capture_cam_t();
		///
		void set_camera_mode(){camera_mode=true;};
		void set_avi_mode(){camera_mode=false;};
	
	public:

		///
		bool close();

	public:
		///
		void run_thread();
		///
		void cancel() {_running=false;};

	private:	        
    ///
    void open_conf_(const std::string& _xmlfile);	
		///
		bool _open(core::detail::camera_mode_t,int in_cam = -1);
		///
		bool _open(core::detail::video_mode_t,const std::string &);

		///
		std::auto_ptr<all::sense::opencv_grabber_t> _impl; 
		///
    std::auto_ptr<all::core::uint8_t> _buffer;
		///
		bool camera_mode;
        ///
    int cam_;
    ///
    std::string  video_name_;
		std::string  info_obj_tag;
		std::string  mem_obj_tag; 
		///
		volatile bool _running;
	};
//-------------------------------------------------------------------------++
#endif //capture_cam_t_H_INCLUDED
