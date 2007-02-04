#ifndef pantilt_control_loop_t_H_INCLUDED
#define pantilt_control_loop_t_H_INCLUDED
//---------------------------------------------------------------------------
#include "alcor/core/core.h"
#include "alcor/act/directed_perception_ptu_t.h"
#include "alcor/core/slam_data_adapter_i.h"
#include <boost\shared_ptr.hpp>
#include <boost\thread.hpp>
//---------------------------------------------------------------------------
namespace all { namespace act {
  //---------------------------------------------------------------------------
///class pantilt_control_loop_t.
///needs a ptr to the pantilt and to the robot.
class pantilt_control_loop_t
{
public:
  pantilt_control_loop_t(  
    boost::shared_ptr<directed_perception_ptu_t>,
    boost::shared_ptr<core::slam_data_adapter_i>);
  ~pantilt_control_loop_t();
  typedef enum {
    idle,
    enabled
  }loop_state;

  void set_polar_reference(math::deg_t, double);
  void set_polar_reference(math::rad_t, double);

  void clear_polar_reference();

  double get_polar_reference(math::deg_t) const ;
  double get_polar_reference(math::rad_t) const ;

  void run_loop();

  void enable(bool bflag)
  { (bflag) ? (state_= enabled) : (state_ = idle);};

  void esc(){running_ = false;};

private:
  void adjust_();

private:
  volatile bool running_;
  volatile loop_state state_;
  double reference_;

private:
  ///
  boost::shared_ptr<boost::thread> loop_thr;

  boost::shared_ptr<directed_perception_ptu_t>  ptu_;
  boost::shared_ptr<slam_data_adapter_i>        slam_;
};

//---------------------------------------------------------------------------
}}//namespaces
//---------------------------------------------------------------------------
#endif //pantilt_control_loop_t_H_INCLUDED