#ifndef OpenCVGrabber_INC
#define OpenCVGrabber_INC
//-------------------------------------------------------------------------++
#pragma comment(lib, "opencv_grabber_t.lib")
//-------------------------------------------------------------------------++
#include "alcor/core/core.h"
#include <cv.h>
#include <highgui.h>
//-------------------------------------------------------------------------++
#include <boost\shared_ptr.hpp>
#include <boost\function.hpp>
//-------------------------------------------------------------------------++
namespace all {
    namespace sense {
        class opencv_grabber_t;
        typedef boost::shared_ptr<opencv_grabber_t> opencv_grabber_ptr;
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
  void set_output_ordering(core::interleaved_t);
  ///
  void set_output_ordering(core::planar_t);
  ///
  //void set_output_format(core::rgb_t);
  ///
  void set_graylevel_output();

  ///
  void log_to_disk(const std::string& avifile);

  ///
  bool ipl_is_interleaved() {return !m_data_order;};
	///
	bool close();

  ///
  boost::function< bool (core::uint8_sarr&) > 
    get_color_buffer; 

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

private:
  
  ///Width of the images a grabber produces. 
  int m_w;
  ///Height of the images a grabber produces. 
  int m_h;
	///Depth of the images a grabber produces. 
	int m_ch;

	///
  bool get_color_buffer_original_(core::uint8_sarr&);
  ///
  bool get_color_buffer_gray_(core::uint8_sarr&);

  ///
  bool bwantsinterleaved;
  ///
  bool bwantsgray;

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
  ///
  IplImage* m_ipl_image_gray;

  ///Opaque OpenCV structure for image capture.
  CvCapture* m_capture;

};

//-------------------------------------------------------------------------++

#endif