#ifndef camera_logger_t_H_INCLUDED
#define camera_logger_t_H_INCLUDED
//-------------------------------------------------------------------
#include "alcor/sense/opencv_grabber_t.h"
#include "alcor/core/imagestream_logger_t.h"
//-------------------------------------------------------------------
#include <boost/timer.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
//-------------------------------------------------------------------
#pragma comment (lib, "opencv_grabber_t.lib")
//-------------------------------------------------------------------
namespace all { namespace core {
//-------------------------------------------------------------------
class camera_logger_t
{
public:
  ///
  camera_logger_t();
  ///
  ~camera_logger_t();
  ///
  void begin_loop();
  ///
  void end_loop();

private:
  ///
  void init_();
  ///
  void deinit_();
  ///
  void main_loop_();

  ///
  boost::shared_ptr<boost::thread> 
    thread_ptr_;

  ///
  boost::shared_ptr<all::core::image_stream_logger_t<all::core::uint8_t> > 
    binlogger_;

  ///
  all::sense::opencv_grabber_ptr camera_;

  ///
  all::core::uint8_sarr imag_sptr_;

  ///loop control
  volatile bool running_;

  ///
  boost::timer timer_;
};
//-------------------------------------------------------------------
//*******************************************************************
//-------------------------------------------------------------------
  ///
camera_logger_t::camera_logger_t():
running_(true)
{

}
//-------------------------------------------------------------------
camera_logger_t::~camera_logger_t()
{

}
//-------------------------------------------------------------------
  ///
void camera_logger_t::begin_loop()
{
thread_ptr_.reset( new boost::thread(boost::bind(&camera_logger_t::main_loop_, this) ) );
}
//-------------------------------------------------------------------
  ///
void camera_logger_t::end_loop()
{
  running_ = false;
}
//-------------------------------------------------------------------
  ///
void camera_logger_t::init_()
{
  //
  camera_.reset(new all::sense::opencv_grabber_t);
  camera_->open("config/opencvcam.ini");
  //
  imag_sptr_.reset( new core::uint8_t[camera_->width()*camera_->height()*camera_->channels()] );
  //
  binlogger_.reset(new all::core::image_stream_logger_t<all::core::uint8_t>);
  binlogger_->open(camera_->height(),camera_->width(),camera_->channels());
}
//-------------------------------------------------------------------
  ///
void camera_logger_t::deinit_()
{
  //
  camera_->close();
}
//-------------------------------------------------------------------
  ///
void camera_logger_t::main_loop_()
{
  //
  init_();
  timer_.restart();
  double timestamp;
  while(running_)
  {
    if (camera_->get_color_buffer(imag_sptr_) )
    {
      timestamp = timer_.elapsed();
      binlogger_->add(imag_sptr_, timestamp);
    }

  boost::thread::yield();
  all::core::BOOST_SLEEP(1);
  }
  //
  double elapsed = timer_.elapsed();
  //
  deinit_();
  //
  printf("Acquired %d samples in %f seconds\n", binlogger_->nsamples(), elapsed);
  printf("Frame Rate: %f\n",binlogger_->nsamples()/elapsed);
}
//-------------------------------------------------------------------
}}//all::core
//-------------------------------------------------------------------
#endif //camera_logger_t_H_INCLUDED