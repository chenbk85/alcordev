#ifndef p3_gateway_H_INCLUDED
#define p3_gateway_H_INCLUDED
//---------------------------------------------------------------------------
#include "alcor/core/core.h"
//---------------------------------------------------------------------------
#include "alcor/math/angle.h"
#include "alcor/math/pose2d.h"
#include "alcor/math/point2d.h"
#include "alcor/core/iniWrapper.h"
//---------------------------------------------------------------------------
namespace all{namespace act{
  namespace detail
  {
    struct p3_gateway_impl;
  }
//---------------------------------------------------------------------------
//p3_conf.ini
//[config]
//p3dx = 0
//p3at = 1
//serialmode=1
//tcpmode=0
//comport=COM5
//[server]
//hostname = localhost
//port = 87654
//---------------------------------------------------------------------------
  ///p3_gateway
  class p3_gateway
  {
  public:
    ///
    p3_gateway();
    ~p3_gateway();

    typedef enum 
    {
      p3DX,
      p3AT
    } e_robot_model;

    ///
    static boost::shared_ptr<p3_gateway> create();
    //>+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    ///
    bool open(std::string inifile);

    ///Get Odometry (not localised) in meters!!.
    math::pose2d get_odometry();

    //[DIRECT COMMANDS]*>++++++++++++++++++++++++++++++++++++++++
    ///Move forward.
    void move(double);
    ///
    void set_heading(const math::angle&);
    ///
    void set_delta_heading(const math::angle&);
    ///
    void set_rot_vel(double);
    ///
    void set_vel(double);
    ///
    void set_vel2(double, double);
    ///
    void stop();

    //[ACTIONS] ]*>++++++++++++++++++++++++++++++++++++++++++++++
    ///Enable Wandering Action
    void enable_wander_mode();

    ///Enable Stop Action
    void enable_stop_mode();

    ///Follow Mode
    void enable_follow_mode();
    ///only when in follow_mode.
    ///distance (mt) e offset angolare dal target (gradi)
    void set_target_to_follow 
      (const math::point2d& target, double speed = 100);

    ///
    void enable_goto_mode();
    ///
    void set_goto(const math::point2d& target, double mmpersecs);

  private:
    boost::shared_ptr<detail::p3_gateway_impl> impl;
    e_robot_model robot_model_;

	  iniWrapper ini_;
  };
//---------------------------------------------------------------------------
  typedef boost::shared_ptr<p3_gateway>  p3_gateway_ptr_t;
//---------------------------------------------------------------------------
  }}//all::act
//---------------------------------------------------------------------------


#endif //p3_gateway_H_INCLUDED