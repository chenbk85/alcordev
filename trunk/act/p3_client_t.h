#ifndef p3_client_t_H_INCLUDED
#define p3_client_t_H_INCLUDED
//-------------------------------------------------------------------
#include <alcor/core/client_base_t.hpp>
#include <boost/shared_ptr.hpp>
//-------------------------------------------------------------------
#include "alcor/act/p3_server_data_t.hpp"
//-------------------------------------------------------------------
namespace all { namespace act{
//-------------------------------------------------------------------
///
  class p3_client_t : public core::client_base_t 
  {
  public:
    ///
    p3_client_t(const char* ini);
    
    ///
    const math::pose2d& 
      get_odometry() const;

    ///
    typedef boost::function<void (const math::pose2d&)> 
      user_callback_t;

    ///the actual method invoked
    user_callback_t user_callback;
  
    ///
    void set_user_callback(user_callback_t);
  
    ///
     //[remote methods]
  public:
    ///
    void set_slam_localized(const math::pose2d&);
    //[direct]
    ///
    void set_vel(double mmpersecs);
    ///
	void set_rot_vel(double mmpersecs);
	///
    void set_dheading(double offset);

    ///
    void enable_stop_mode();
    ///
    void enable_wander_mode();
    ///
    void enable_follow_mode();
    ///only when in follow_mode.
    ///distance (mt) e offset angolare dal target (gradi)
    void set_target_to_follow 
      (const math::point2d& target, double speed = 100);

    ///
    void enable_goto_mode();
    ///only when in follow_mode.
    ///distance (mt) e offset angolare dal target (gradi)
    void set_relative_goto
      (const math::point2d& target, double mmpersecs = 100);

    ///Generic remote trigger
    void trigger();

  private:
    ///
	  void update_p3_data(net_packet_ptr_t);
    ///
	  void connected_callback();
    ///
    p3_server_data_t p3_data_;
  };

//-------------------------------------------------------------------
typedef boost::shared_ptr<p3_client_t>  p3_client_ptr_t;
//-------------------------------------------------------------------
}}
//-------------------------------------------------------------------
#endif//p3_client_t_H_INCLUDED