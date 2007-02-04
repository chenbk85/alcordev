#include "alcor/act/pantilt_control_loop_t.h"
#include <windows.h>
//---------------------------------------------------------------------------
#include <boost\bind.hpp>
//---------------------------------------------------------------------------
namespace all{ namespace act{

pantilt_control_loop_t::pantilt_control_loop_t(boost::shared_ptr<directed_perception_ptu_t> iptu,
                                               boost::shared_ptr<core::slam_data_adapter_i> islam):
ptu_(iptu),
slam_(islam),
running_(true),
state_(idle),
reference_(0)
{
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
pantilt_control_loop_t::~pantilt_control_loop_t()
{

}
//---------------------------------------------------------------------------
void pantilt_control_loop_t::set_polar_reference(math::deg_t, double val)
{
  reference_ = val;
}
//---------------------------------------------------------------------------
void pantilt_control_loop_t::set_polar_reference(math::rad_t, double val)
{
  reference_ = core::dconstants::rad_to_deg(val);
}
//---------------------------------------------------------------------------
void pantilt_control_loop_t::clear_polar_reference()
{
  reference_ = 0;
}
//---------------------------------------------------------------------------
double pantilt_control_loop_t::get_polar_reference(math::deg_t) const
{
  return reference_;
}
//---------------------------------------------------------------------------
double pantilt_control_loop_t::get_polar_reference(math::rad_t) const
{
  return core::dconstants::deg_to_rad(reference_);
}
//---------------------------------------------------------------------------
void pantilt_control_loop_t::adjust_()
{
  printf("adjust\n");
  double theta_rob = slam_->get_current_rot(math::deg_tag);
  printf("reference: %f\n", reference_);
  double pan_setpoint =  reference_ - theta_rob;
  printf("theta_rob: %f \n",theta_rob);
  ptu_->set_pan(pan_setpoint, 0.2);
}
//---------------------------------------------------------------------------
void pantilt_control_loop_t::run_loop()
{
  while (running_)
  {
    switch(state_)
    {
      case enabled:
        adjust_();
        Sleep(100);
        break;

      case idle:
        Sleep(250);
        break;

      default:
        break;
    }
    boost::thread::yield();
  }
  state_ = idle;
}
//---------------------------------------------------------------------------
}}//namespaces