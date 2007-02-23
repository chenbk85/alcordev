#ifndef OpenCVGrabber_INC
#define OpenCVGrabber_INC
//-------------------------------------------------------------------------++
#pragma comment(lib, "opencv_grabber_t.lib")
//-------------------------------------------------------------------------++
#include "alcor/core/core.h"
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
	bool open(core::camera_mode_t, int cam=-1);
	///
	bool open(core::video_mode_t, const std::string&);

  ///
  bool is_interleaved() {return !m_data_order;};
  ///
  bool is_topleft()     {return !m_data_origin;};
	///
	bool close();
	///
  bool get_color_buffer(core::uint8_sarr&);
  ///
  IplImage* get_ipl_image();
  
  ///
  int height() const { return m_h; }
  ///
  int width() const { return m_w; }
  ///
  int channels() const { return m_ch; }
  ///
  std::size_t size() const { return m_byte_size; }

private:
  ///
	bool internal_open_();

protected:
  
  ///Width of the images a grabber produces. 
  int m_w;
  ///Height of the images a grabber produces. 
  int m_h;
	///Depth of the images a grabber produces. 
	int m_ch;
  ///
  int m_data_order;
  ///
  int m_data_origin;
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
  CvCapture* m_capture;

};

//-------------------------------------------------------------------------++

#endif