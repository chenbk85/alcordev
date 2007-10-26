#ifndef camera_logger_t_H_INCLUDED
#define camera_logger_t_H_INCLUDED
//-------------------------------------------------------------------
#include "alcor/sense/opencv_grabber_t.h"
#include "alcor/core/imagestream_logger_t.h"
#include "alcor/core/cameralog_inc.h"
//-------------------------------------------------------------------
#include <boost/timer.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
//-------------------------------------------------------------------
//-------------------------------------------------------------------
#define USE_VIDEOINPUT
//-------------------------------------------------------------------
//-------------------------------------------------------------------
#ifdef USE_VIDEOINPUT
  #include "alcor.extern/videoinput/include/videoinput.h"
  #include <highgui.h>
  #pragma comment (lib, "highgui.lib")
  #pragma comment(lib, "videoInput.lib")
#else 
  #pragma comment (lib, "opencv_grabber_t.lib")
#endif
//-------------------------------------------------------------------
namespace all { namespace core {  
//-------------------------------------------------------------------
//-------------------------------------------------------------------
class camera_logger_t
{
public:
  ///
  camera_logger_t(log_type logtype = e_planar);
  ///
  ~camera_logger_t();
  ///
  void begin_loop();
  ///
  void quit_loop();
  ///


private:
  ///
  void init_();
  ///
  void deinit_();
  ///
  void main_loop_planar_();
  ///
  void main_loop_ipl_();
  ///
  boost::shared_ptr<boost::thread> 
    thread_ptr_;

  ///
  log_type log_type_;

  ///
  boost::shared_ptr<all::core::image_stream_logger_t<all::core::uint8_t> > 
    binlogger_;

#ifdef USE_VIDEOINPUT
  //create a videoInput object
  boost::shared_ptr<videoInput> VI;
#else
  ///
  all::sense::opencv_grabber_ptr camera_;
#endif

	int width; 	
	int height; 	
	int size;	
  int channels;

  ///loop control
  volatile bool running_;

  ///
  boost::timer timer_;
  ///
  CvVideoWriter* avifile_;
};
//-------------------------------------------------------------------
//*******************************************************************
//-------------------------------------------------------------------
  ///
camera_logger_t::camera_logger_t(log_type logtype):
running_(true)
{
  //init_();
log_type_ = logtype;
}
//-------------------------------------------------------------------
camera_logger_t::~camera_logger_t()
{

}
//-------------------------------------------------------------------
///
void camera_logger_t::begin_loop()
{
  thread_ptr_.reset( new boost::thread(boost::bind(&camera_logger_t::main_loop_ipl_, this) ) );
}
//-------------------------------------------------------------------
  ///
void camera_logger_t::quit_loop()
{
  running_ = false;
}
//-------------------------------------------------------------------
  ///
void camera_logger_t::init_()
{

#ifdef USE_VIDEOINPUT
  VI.reset(new videoInput());
 //   //optional static function to list devices
	////for silent listDevices use listDevices(true);
	//int numDevices = videoInput::listDevices();
  //
  VI->setupDevice(0, 640, 480);
	//to get a settings dialog for the device
	VI->showSettingsWindow(0);
  //
	width 	= VI->getWidth(0);
	height 	= VI->getHeight(0);
	size	  = VI->getSize(0);
  channels = 3;

  avifile_ = 
    cvCreateVideoWriter( "videoinput.avi"
                        , -1
                        , 10
                        , cvSize(width, height)
                       );

#else
  //
  camera_.reset(new all::sense::opencv_grabber_t);
  camera_->open("config/opencvcam.ini");

  height = camera_->height();
  width = camera_->width();
  channels = camera_->channels();
#endif
    //
  binlogger_.reset(new all::core::image_stream_logger_t<all::core::uint8_t>);
  binlogger_->open(log_type_, height, width, channels);
}
//-------------------------------------------------------------------
  ///
void camera_logger_t::deinit_()
{
#ifdef USE_VIDEOINPUT
  //Shut down devices properly
	VI->stopDevice(0);
  cvReleaseVideoWriter(&avifile_ );
#else
  //
  camera_->close();
#endif
}
////-------------------------------------------------------------------
//  ///
//void camera_logger_t::main_loop_planar_()
//{
//  //
//  init_();
//  timer_.restart();
//  double timestamp;
//  while(running_)
//  {
//    if (camera_->get_image_buffer(imag_sptr_) )
//    {
//      timestamp = timer_.elapsed();
//      binlogger_->add(imag_sptr_, timestamp);
//    }
//
//  boost::thread::yield();
//  all::core::BOOST_SLEEP(1);
//  }
//  //
//  double elapsed = timer_.elapsed();
//  //
//  deinit_();
//  //
//  printf("Acquired %d samples in %f seconds\n", binlogger_->nsamples(), elapsed);
//  printf("Frame Rate: %f\n",binlogger_->nsamples()/elapsed);
//}
//-------------------------------------------------------------------
  ///
void camera_logger_t::main_loop_ipl_()
{
  ///
  init_();
  timer_.restart();
  double timestamp;
  ///
  IplImage* current_image = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);;

  //    
#ifdef USE_VIDEOINPUT
  unsigned char* rawframe = new unsigned char[VI->getSize(0)];
  //IplImage* my_image      = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
  	//
  cvNamedWindow("videoinput");

#endif
  int nsamples = 0;
  //
  while(running_)
  {

    #ifdef USE_VIDEOINPUT
      VI->getPixels(0, rawframe, false);
      memcpy(current_image->imageData, rawframe, size);
      //cvConvertImage(current_image, current_image, CV_CVTIMG_FLIP);
      cvShowImage("videoinput", current_image);

    #else
      camera_->grab_ipl_image();

      current_image = camera_->retrieve_ipl_image();

    #endif
  
    timestamp = timer_.elapsed();
    cvWriteFrame( avifile_ , current_image );
    nsamples++;
    //binlogger_->add_iplimage(current_image, timestamp);
    cvWaitKey(1);
    boost::thread::yield();
    all::core::BOOST_SLEEP(1);
  }
  //
  double elapsed = timer_.elapsed();

  #ifdef USE_VIDEOINPUT
    delete [] rawframe; 
    cvReleaseImage(&current_image);
  #endif

  //
  deinit_();
  //
  printf("Acquired %d samples in %f seconds\n", nsamples, elapsed);
  printf("Frame Rate: %f\n",nsamples/elapsed);
}
//-------------------------------------------------------------------
}}//all::core
//-------------------------------------------------------------------
#endif //camera_logger_t_H_INCLUDED