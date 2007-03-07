#ifndef pantilt_control_loop_t_H_INCLUDED
#define pantilt_control_loop_t_H_INCLUDED
//---------------------------------------------------------------------------
#include "alcor/core/core.h"
#include "alcor/act/directed_perception_ptu_t.h"
#include "alcor/core/slam_data_adapter_i.h"
//---------------------------------------------------------------------------
#include <boost/thread/mutex.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
//---------------------------------------------------------------------------
#include <boost\bind.hpp>
//---------------------------------------------------------------------------
namespace all { namespace act {
  //---------------------------------------------------------------------------
///class pantilt_control_loop_t.
///needs a ptr to the pantilt and to the robot.
class pantilt_control_loop_t
{
public:
  pantilt_control_loop_t();
  ~pantilt_control_loop_t();    

  void set_ptu(boost::shared_ptr<directed_perception_ptu_t>);
  void set_slam(boost::shared_ptr<core::slam_data_adapter_i>);

  typedef enum {
    idled,
    enabled
  }loop_state;

  void set_polar_reference(math::deg_t, double);
  void set_polar_reference(math::rad_t, double);

  void clear_polar_reference();

  double get_polar_reference(math::deg_t) const ;
  double get_polar_reference(math::rad_t) const ;

  void run_loop();

  boost::function <void (void)> loop_action;

  void idle_action();
  void enabled_action();

  bool is_enabled() const {return state_==enabled;}
  bool is_idled()   const {return state_==idled;}

  void enable(bool bflag)
  { 
    boost::mutex::scoped_lock lock(mutex);

    (bflag) ? (state_= enabled) : (state_ = idled);
    if(bflag)
      loop_action = 
        boost::bind(&pantilt_control_loop_t::enabled_action, this);
    else
      loop_action = 
        boost::bind(&pantilt_control_loop_t::idle_action, this);
  };

  void esc(){running_ = false;};

private:
  volatile bool running_;
  volatile loop_state state_;
  double reference_;

private:
  ///
  boost::shared_ptr<boost::thread> loop_thr;

  boost::shared_ptr<directed_perception_ptu_t>  ptu_;
  boost::shared_ptr<slam_data_adapter_i>        slam_;
  boost::mutex mutex;
};
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
inline pantilt_control_loop_t::pantilt_control_loop_t():

running_(true),
state_(idled),
reference_(0)
{
  loop_action = 
    boost::bind(&pantilt_control_loop_t::idle_action, this);
    ///The MACHINE
  loop_thr.reset(
    new boost::thread 
    (
    boost::bind(
    &pantilt_control_loop_t::run_loop
    , this) 
    )
    );
}
//---------------------------------------------------------------------------
inline void pantilt_control_loop_t::set_ptu(boost::shared_ptr<directed_perception_ptu_t> ptu)
{
  ptu_ = ptu;
}
//---------------------------------------------------------------------------
inline void pantilt_control_loop_t::set_slam(boost::shared_ptr<core::slam_data_adapter_i> slam)
{
  slam_ = slam;
}
//---------------------------------------------------------------------------
inline pantilt_control_loop_t::~pantilt_control_loop_t()
{

}
//---------------------------------------------------------------------------
inline void pantilt_control_loop_t::set_polar_reference(math::deg_t, double val)
{
  reference_ = val;
}
//---------------------------------------------------------------------------
inline void pantilt_control_loop_t::set_polar_reference(math::rad_t, double val)
{
  reference_ = core::dconstants::rad_to_deg(val);
}
//---------------------------------------------------------------------------
inline void pantilt_control_loop_t::clear_polar_reference()
{
  reference_ = 0;
}
//---------------------------------------------------------------------------
inline double pantilt_control_loop_t::get_polar_reference(math::deg_t) const
{
  return reference_;
}
//---------------------------------------------------------------------------
inline double pantilt_control_loop_t::get_polar_reference(math::rad_t) const
{
  return core::dconstants::deg_to_rad(reference_);
}
//---------------------------------------------------------------------------
inline   void pantilt_control_loop_t::idle_action()
{
  BOOST_SLEEP(50);
}

inline   void pantilt_control_loop_t::enabled_action()
{
  boost::mutex::scoped_lock lock(mutex);
  if(slam_ && ptu_)
  {
  printf("adjust\n");
  double theta_rob = slam_->get_current_rot(math::deg_tag);
  printf("reference: %f\n", reference_);
  double pan_setpoint =  reference_ - theta_rob;
  //printf("theta_rob: %f \n",theta_rob);
  ptu_->set_pan(static_cast<float>(pan_setpoint), 0.1f);
  }
  BOOST_SLEEP(50);
}
//---------------------------------------------------------------------------
inline void pantilt_control_loop_t::run_loop()
{
  while (running_)
  {
    loop_action();
    boost::thread::yield();
  }
  state_ = idled;
}
//---------------------------------------------------------------------------
}}//namespaces
//---------------------------------------------------------------------------
#endif //pantilt_control_loop_t_H_INCLUDED