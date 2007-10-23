#ifndef videoinput_simple_grabber_t_HPP_INCLUDED
#define videoinput_simple_grabber_t_HPP_INCLUDED
//--------------------------------------------------------+
#include "alcor.extern/videoinput/videoinput.h"
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

  private:
    ///a videoInput object
	  videoInput VI;

  };
//--------------------------------------------------------+
}}//all::sense
//--------------------------------------------------------+

#endif //videoinput_simple_grabber_t_HPP_INCLUDED