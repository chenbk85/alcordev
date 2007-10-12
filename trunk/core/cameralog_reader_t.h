#ifndef cameralog_reader_t_H_INCLUDED
#define cameralog_reader_t_H_INCLUDED
//-------------------------------------------------------------------
#include "alcor/sense/opencv_grabber_t.h"
#include "alcor/core/imagestream_reader_t.h"
//-------------------------------------------------------------------
#include <boost/timer.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
//-------------------------------------------------------------------
#include "alcor.extern/CImg/CImg.h"
using namespace cimg_library;
//-------------------------------------------------------------------
#include <boost/lexical_cast.hpp>
#include "mat.h"
//-------------------------------------------------------------------
#include "alcor/matlab/matlab_mx_utils.hpp"
#include "alcor/core/image_utils.h"
//-------------------------------------------------------------------
#pragma comment (lib, "libmat.lib")
#pragma comment (lib, "libmx.lib")
//-------------------------------------------------------------------
#define _DO_MATLAB_LOG_
//-------------------------------------------------------------------
namespace all { namespace core {
//-------------------------------------------------------------------
///
class cameralog_reader_t
{
public:
  ///
  cameralog_reader_t(bool b_do_matsave = false);
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
  void main_loop_planar_();
  ///
  void main_loop_iplimage_();

  ///
  boost::shared_ptr<boost::thread> 
    thread_ptr_;

  ///
  boost::shared_ptr<all::core::imagestream_reader_t<all::core::uint8_t> > 
    binreader_;

  ///
  all::core::uint8_sarr imag_sptr_;

  ///loop control
  volatile bool running_;
  ///
  double timestamp_;
  ///
  bool b_matsave_;
  ///
  log_type logtype_;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
cameralog_reader_t::cameralog_reader_t(bool b_do_matsave):
running_(true)
,timestamp_(0)
,b_matsave_(b_do_matsave)
{
}
//-------------------------------------------------------------------
///
void cameralog_reader_t::begin_loop()
{
  //
  init_();
  //
  switch( binreader_->logtype() )
  {
  case e_planar:
    thread_ptr_.reset( new boost::thread(boost::bind(&cameralog_reader_t::main_loop_planar_, this) ) );
    break;
  case e_iplimage:
    thread_ptr_.reset( new boost::thread(boost::bind(&cameralog_reader_t::main_loop_iplimage_, this) ) );
    break;
  }
}
//-------------------------------------------------------------------
///
void cameralog_reader_t::end_loop()
{
  running_ = false;
}
//-------------------------------------------------------------------
  ///
void cameralog_reader_t::init_()
{
  //
  printf("-->cameralog_reader_t::init_\n");
  //
  binreader_.reset(new all::core::imagestream_reader_t<all::core::uint8_t> );
  //
  binreader_->open();
}
//-------------------------------------------------------------------
  ///
void cameralog_reader_t::deinit_()
{

}
//-------------------------------------------------------------------
//MAIN LOOP
void cameralog_reader_t::main_loop_planar_()
{
  //
  imag_sptr_.reset( new all::core::uint8_t[binreader_->width()*binreader_->height()*binreader_->depth()] );
  //graphics
    //
  CImgDisplay view (  binreader_->width(),  binreader_->height(), "Camera");
  CImg<all::core::uint8_t> cimag;

  //
  double prev_timestamp_ = 0;

  //
  while(running_)
  {
  
  //
    if (binreader_->sample_planar(imag_sptr_ , timestamp_))
  {
    //
    cimag.assign(   imag_sptr_.get() 
                  , binreader_->width()
                  , binreader_->height()
                  , 1
                  , binreader_->depth());
    //
    cimag.display(view) ;
    
if (b_matsave_)
    {
        ////////////////////////////////////////////////////////////////////////
        //MATLAB --------------------------------------------------------------+
        //
        MATFile *pmat = 0;
        //MATLAB
        std::string namebase = "cameralog_";
        //
        namebase += boost::lexical_cast<std::string>(binreader_->current_sample());
        namebase += ".mat";
        //
        pmat = matOpen(namebase.c_str(), "w");

        //-----------------
        mxArray* mx_rgb = 
          matlab::buffer2array<core::uint8_t>::create_from_planar(imag_sptr_.get()
                                                          , matlab::row_major
                                                          , binreader_->height()
                                                          , binreader_->width());

        //add to file
        matPutVariable(pmat, "cameralog", mx_rgb);
        
        //  
        matClose(pmat);  

        //
        mxDestroyArray(mx_rgb);
    }//b_matsave_
  }
  else
    running_ = false;

  boost::thread::yield();
  all::core::BOOST_SLEEP((timestamp_ - prev_timestamp_)*1000);
  prev_timestamp_ = timestamp_;
  }
  //
  deinit_();
}
//-------------------------------------------------------------------
//MAIN LOOP
void cameralog_reader_t::main_loop_iplimage_()
{
  //
  printf("-->cameralog_reader_t::main_loop_iplimage_\n");
  ///
  IplImage* current_im = 
    cvCreateImage(cvSize(binreader_->width(),  binreader_->height()), IPL_DEPTH_8U, binreader_->depth());
  /////
  //IplImage* drawing_im = 
  //  cvCreateImage(cvSize(binreader_->width(),  binreader_->height()), IPL_DEPTH_8U, binreader_->depth());

//  //graphics
//    //
  //cvNamedWindow("Stream");

  //    //
  //CImgDisplay view (  binreader_->width(),  binreader_->height(), "Camera");
  //CImg<all::core::uint8_t> cimag;

//  //
  double prev_timestamp_ = 0;
  //
  printf("++ The inner loop\n");
  while(running_)
  {
  //
  if (binreader_->sample_ipl(current_im , timestamp_))
  {
    cvConvertImage(current_im, current_im, CV_CVTIMG_FLIP);
   // all::core::change_ordering
   ////cvShowImage("Stream", current_im);

  //    
  if (b_matsave_)
    {
        ////////////////////////////////////////////////////////////////////////
        //MATLAB --------------------------------------------------------------+
        
        MATFile *pmat = 0;
        //MATLAB
        std::string namebase = "cameralog_";

        //
        namebase += boost::lexical_cast<std::string>(binreader_->current_sample());
        namebase += ".mat";

        ////
        pmat = matOpen(namebase.c_str(), "w");

        ////-----------------
        mxArray* mx_img = matlab::mxcv::iplimage_to_mxarray<core::uint8_t>(current_im);

        ////add to file
        matPutVariable(pmat, "cameralog", mx_img);
        //
        matClose(pmat);  
        //
        mxDestroyArray(mx_img);
    }//b_matsave_
  }
  else
    running_ = false;
//
  boost::thread::yield();
  all::core::BOOST_SLEEP((timestamp_ - prev_timestamp_)*1000);
  prev_timestamp_ = timestamp_;
  }
//  //
  deinit_();

}
//-------------------------------------------------------------------
}}//all::core
//-------------------------------------------------------------------
#endif //cameralog_reader_t_H_INCLUDED