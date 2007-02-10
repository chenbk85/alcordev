#ifndef OpenCVGrabber_INC
#define OpenCVGrabber_INC
//-------------------------------------------------------------------------++
#pragma comment(lib, "opencv_grabber_t.lib")
//-------------------------------------------------------------------------++
#include "alcor/core/core.h"
#include "alcor/core/detail/grabber_mode_tags.hpp"
#include <cv.h>
#include <highgui.h>
//-------------------------------------------------------------------------++
namespace all {
    namespace sense {
        class opencv_grabber_t;
    }
}

//-------------------------------------------------------------------------++
///
class all::sense::opencv_grabber_t 
{
public:

///
  opencv_grabber_t(int cam = -1); 

  // Destroy an OpenCV image grabber.
  virtual ~opencv_grabber_t();

	///
	bool open(core::detail::camera_mode_t, int);
	///
	bool open(core::detail::video_mode_t, const std::string&);
	///
	bool close();
	///
  bool get_color_buffer(core::uint8_sarr);
  
	///
    int height() const { return m_h; }
	///
    int width() const { return m_w; }
	///
    int channels() const { return m_ch; }
	///
	std::size_t size() const { return m_byte_size; }

private:
	bool internal_open_();

protected:
  
  ///Width of the images a grabber produces. 
  int m_w;
  ///Height of the images a grabber produces. 
  int m_h;
	///Height of the images a grabber produces. 
	int m_ch;
	///
	int m_cam_id;
	///
	unsigned int snap;
	///
	std::size_t m_byte_size;
  ///
  all::core::uint8_sarr image_sptr;
  ///
  IplImage* m_ipl_image;

  ///Opaque OpenCV structure for image capture.
  void * m_capture;

};

//-------------------------------------------------------------------------++

#endif