#ifndef xsens_mti_driver_t_H_INCLUDED
#define xsens_mti_driver_t_H_INCLUDED
//---------------------------------------------------------
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/timer.hpp>
//---------------------------------------------------------
#include <string>
//---------------------------------------------------------
#include "alcor/sense/xsens_mti_inc.hpp"
//---------------------------------------------------------
namespace all { namespace sense {  
//---------------------------------------------------------
  ///
  namespace detail
  {
    struct MTi_driver_impl;
  } 
//---------------------------------------------------------
  ///
  class xsens_mti_driver_t
  {
  public:
    ///
    xsens_mti_driver_t();
    ///
    ~xsens_mti_driver_t();
    ///
    bool open(std::string& configfile);

    //
    void reset(heading_reset_t);
    //
    void reset(global_reset_t);
    //
    void reset(object_reset_t);
    //
    void reset(align_reset_t);

    //
    void run_mti();
    //
    void stop_mti();

    ///test
    void print_calibdata();

  private:
    ///
    boost::shared_ptr<detail::MTi_driver_impl> impl;
    ///
    boost::timer timestamp_;
    ///
    void mti_loop_();
    ///
    boost::shared_ptr<boost::thread> loop_thread_;
    ///
    volatile bool running_;
  };
//---------------------------------------------------------
}}//all::sense
//---------------------------------------------------------
#endif //xsense_mti_driver_t_H_INCLUDED