#ifndef opencv_lightweight_grabber_t_H_INCLUDED
#define opencv_lightweight_grabber_t_H_INCLUDED
//--------------------------------------------------------++
#include <cv.h>
#include <highgui.h>
//--------------------------------------------------------++
#include <boost/shared_ptr.hpp>
//--------------------------------------------------------++
#include "alcor/core/parallel_worker_sync_t.hpp"
//--------------------------------------------------------++
//#include <boost/>
//--------------------------------------------------------++
namespace all { namespace core {
///
class opencv_lightweight_grabber_t
{
public:
  ///
  opencv_lightweight_grabber_t(core::parallel_worker_sync_t*);
  ///
  ~opencv_lightweight_grabber_t();

  ///
  bool open(bool show_dialog = true); 

  ///loop
  void grab_loop();

  ///
  void cancel() {running_ = false;};

  ///
  IplImage* get_iplimage() const;

  ///
  int channels() const {return m_ch_;};
  ///
  int width() const {return m_w_;};
  ///
  int height() const {return m_h_;};
  ///image size in bytes
  int image_size() const {return m_byte_size_;};

private:        
  //this two for fast acquisition....
  ///store the IplImage as is without modifications.
  void   grab_ipl_image();  

  ///get Ipl Image from previously stored buffer.
  void retrieve_ipl_image();

  ///executed in the grab_loop
  bool internal_open_();

  ///
  int m_w_ ; 
  ///
  int m_h_;          
  ///
  int m_data_order_;
  ///
  int m_data_origin_;
  ///
  int m_byte_size_;  
  ///
  int m_ch_ ;

  ///Opaque OpenCV structure for image capture.
  CvCapture* m_capture_;

  ///keep the pointer to the retrieved image
  IplImage* image_ptr_;

  ///
  volatile bool running_;
  ///
  bool b_use_dialogs_;

  ///
  core::parallel_worker_sync_t* sync_;
};

typedef boost::shared_ptr<all::core::opencv_lightweight_grabber_t> opencv_lightweight_grabber_ptr;
}}//all::core

#endif //opencv_lightweight_grabber_t_H_INCLUDED