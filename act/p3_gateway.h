#ifndef p3_gateway_H_INCLUDED
#define p3_gateway_H_INCLUDED
//---------------------------------------------------------------------------
#include "alcor/core/core.h"
#include "alcor/math/angle.h"
#include "alcor/math/pose2d.h"
//---------------------------------------------------------------------------
namespace all{namespace act{
  namespace detail
  {
    struct p3_gateway_impl;
  }
//---------------------------------------------------------------------------
//[config]
//serialmode=1
//tcpmode=0
//comport=COM5
//---------------------------------------------------------------------------

  class p3_gateway
  {
  public:
    ///
    p3_gateway();

    ///
    bool open(std::string inifile);

    ///Get Odometry (not localised) in meters!!.
    math::pose2d const& get_odometry()const;

    //[DIRECT COMMANDS]
    ///Move forward.
    void move(double);
    ///
    void set_heading(math::angle);
    ///
    void set_delta_heading(math::angle);
    ///
    void set_rot_vel(double);
    ///
    void set_vel(double);
    ///
    void set_vel2(double, double);
    ///
    void stop();

    //[ACTIONS]
    ///Enable Wandering Action
    void wander_mode();
    ///Enable Stop Action
    void stop_mode();
    ///
    void follow_mode();
    ///only when in follow_mode.
    ///distance (mt) e offset angolare dal target (gradi)
    void set_target_to_follow 
        (double distance, double offset,double speed = 100);

  private:
    boost::shared_ptr<detail::p3_gateway_impl> impl;
  };
//---------------------------------------------------------------------------
  typedef boost::shared_ptr<p3_gateway>  p3_gateway_ptr;
//---------------------------------------------------------------------------
  }}//all::act
//---------------------------------------------------------------------------


#endif //p3_gateway_H_INCLUDED