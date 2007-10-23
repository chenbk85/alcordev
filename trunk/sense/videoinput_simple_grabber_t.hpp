#ifndef videoinput_simple_grabber_t_HPP_INCLUDED
#define videoinput_simple_grabber_t_HPP_INCLUDED
//--------------------------------------------------------+
#include "alcor.extern/videoinput/videoinput.h"
#include <cv.h>
//--------------------------------------------------------+
//--------------------------------------------------------+
//--------------------------------------------------------+
namespace all{ namespace sense {
//--------------------------------------------------------+
  ///
  class videoinput_simple_grabber_t
  {
  public:
    ///
    videoinput_simple_grabber_t();
    ///
    bool open(int device = 0);
    ///
    void set_resolution (int width, int height);
    ///
    void get_iplimage(IplImage* image);

  private:
    ///a videoInput object
	  videoInput VI;
    ///
    IplImage* iplimage_;

  };
//--------------------------------------------------------+
//--------------------------------------------------------+
  videoinput_simple_grabber_t::videoinput_simple_grabber_t()
  {

  }
//--------------------------------------------------------+
  videoinput_simple_grabber_t::open(int device)
  {

  }
//--------------------------------------------------------+
}}//all::sense
//--------------------------------------------------------+

#endif //videoinput_simple_grabber_t_HPP_INCLUDED