#ifndef directed_perception_ptu_t_H_INCLUDED
#define directed_perception_ptu_t_H_INCLUDED

#pragma comment (lib, "directed_perception_ptu.lib")

#include <boost\shared_ptr.hpp>

///forward
namespace lti{
class directedPerceptionPTU;
}

#include "alcor/math/angle.h"
#include "alcor/core/pantilt_angle_t.h"

//tags
struct immidiate_t {};
struct slaved_t {};

static const immidiate_t  immidiate = immidiate_t();
static const slaved_t     slaved    = slaved_t();

namespace all { namespace act {

///
//typedef std::pair<double, double> pantilt_angle_t;



class directed_perception_ptu_t
{
public:
  directed_perception_ptu_t();

  bool open();

  bool set_angle_format(math::deg_t);
  bool set_angle_format(math::rad_t);

  bool set_exec_mode(immidiate_t);
  bool set_exec_mode(slaved_t);

  bool reset();

  ///se wait > 0 è il tempo massimo di attesa in secondi per 
  ///completare il movimento
  bool set_pantilt(float pan, float tilt, float wait = -1);

  bool set_pan(float pan, float wait = -1);

  bool set_tilt(float tilt, float wait = -1);

	bool get_pantilt(float& pan, float& tilt) const;

	bool get_current_pantilt(float& pan, float& tilt) const;

  ///just a quick guess
  double get_pan() const;
  ///just a quiack guess
  double get_tilt() const;

	bool await_command_completion();

  bool update_pantilt();

private:
  boost::shared_ptr<lti::directedPerceptionPTU> impl;
  mutable core::pantilt_angle_t ptangle_;

};

}}

#endif //directed_perception_ptu_t_H_INCLUDED