#ifndef OpenCVGrabber_INC
#define OpenCVGrabber_INC
//-------------------------------------------------------------------------++
#pragma comment(lib, "opencv_grabber_t.lib")
//-------------------------------------------------------------------------++
#include "alcor/core/core.h"
#include "alcor/matlab/opencv_matlab_utils.hpp"
//-------------------------------------------------------------------------++
#include <cv.h>
#include <highgui.h>
//-------------------------------------------------------------------------++
#ifdef CVAPI_EXPORTS
#undef CVAPI_EXPORTS
#endif
//-------------------------------------------------------------------------++
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
//-------------------------------------------------------------------------++
namespace all {
    namespace sense {
        class opencv_grabber_t;
        typedef boost::shared_ptr<opencv_grabber_t> opencv_grabber_ptr;
    }
}
//------------------------- INI -------------------------------------------++
//[config]
//usedialog=1;
//camera=1;camera o videofile?
//videofile=somevideo.avi;
//graylevel=1;0-> originale da telecamera 1->scala di grigio
//camid=-1;misterioso .. � il numero della telecamera ..
//interleaved=1;1 per avere il buffer immagine ilv
//format=640480;320240
//-------------------------------------------------------------------------++
//To set format directly, put width and hight in one integer and use
//CV_CAP_PROP_FRAME_WIDTH_HEIGHT:
//
//cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH_HEIGHT, 640480 )
//or
//cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH_HEIGHT, 320280 )??
//...
//
//Or to use the built in dialogs:
//cvSetCaptureProperty(capture, CV_CAP_PROP_DIALOG_COMPRESSION, 0 );
//cvSetCaptureProperty(capture, CV_CAP_PROP_DIALOG_SOURCE, 0 );
//cvSetCaptureProperty(capture, CV_CAP_PROP_DIALOG_DISPLAY, 0 );
//cvSetCaptureProperty(capture, CV_CAP_PROP_DIALOG_FORMAT, 0 );
//-------------------------------------------------------------------------++
///
class all::sense::opencv_grabber_t 
{
public:

  ///
  opencv_grabber_t(); 

  // Destroy an OpenCV image grabber.
  virtual ~opencv_grabber_t();

  ///
  bool open(const std::string& inifile, bool show_dialog = true);

  ///
  bool open(const std::string& inifile, const std::string& section, bool show_dialog = true);

  ///uhm nothing here .. yet.
  void log_to_disk(const std::string& avifile);

  ///
  bool iplimage_is_interleaved() {return !m_data_order;};

  ///
  bool image_is_interleaved() const {return bwantsinterleaved;};
  ///
  bool image_is_gray() const {return bwantsgray;};

	///
	bool close();

  ///Obsolete!
  bool get_color_buffer(core::uint8_sarr& image_ptr) 
  {
    printf("USE get_image_buffer instead!\n");
    get_image_buffer(image_ptr);
  }

  ///
  boost::function< bool (core::uint8_sarr&) > 
    get_image_buffer; 

  //---------------------------------------------------------------------------
  //this two for fast acquisition....
  ///store the IplImage as is without modifications.
  void       grab_ipl_image();  

  ///get Ipl Image from previously stored buffer.
  IplImage* retrieve_ipl_image() const;
  //---------------------------------------------------------------------------

  ///
  int height() const { return m_h; }
  ///
  int width() const { return m_w; }
  ///
  int channels() const { return m_ch; }

  ///size in bytes della immagine
  std::size_t size() const { return m_byte_size; }

private:
	///
	bool open_(core::camera_mode_t, int cam, bool show_dialog);
	///
	bool open_(core::video_mode_t, const std::string&);
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
  bool get_image_buffer_original_(core::uint8_sarr&);
  ///
  bool get_image_buffer_gray_(core::uint8_sarr&);

  ///
  bool bwantsinterleaved;
  ///
  bool bwantsgray;
  ///
  //bool busedialogs;

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