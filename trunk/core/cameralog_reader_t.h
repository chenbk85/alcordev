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
  void main_loop_();

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
thread_ptr_.reset( new boost::thread(boost::bind(&cameralog_reader_t::main_loop_, this) ) );
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
  binreader_.reset(new all::core::imagestream_reader_t<all::core::uint8_t> );
  binreader_->open();
  //
  imag_sptr_.reset( new all::core::uint8_t[binreader_->width()*binreader_->height()*binreader_->depth()] );
}
//-------------------------------------------------------------------
  ///
void cameralog_reader_t::deinit_()
{

}
//-------------------------------------------------------------------
//MAIN LOOP
void cameralog_reader_t::main_loop_()
{
  //
  init_();
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
  if (binreader_->sample(imag_sptr_ , timestamp_))
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
  //double elapsed = timer.elapsed();
  //
  deinit_();
  //
  //printf("Acquired %d samples in %f seconds\n", binlogger_->nsamples(), elapsed);
  //printf("Frame Rate: %f\n",binlogger_->nsamples()/elapsed);
}
//-------------------------------------------------------------------
}}//all::core
//-------------------------------------------------------------------
#endif //cameralog_reader_t_H_INCLUDED