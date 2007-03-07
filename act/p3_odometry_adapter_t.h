#ifndef p3_odometry_adapter_t_H_INC
#define p3_odometry_adapter_t_H_INC
//-------------------------------------------------------------------
#include <boost/shared_ptr.hpp>
//-------------------------------------------------------------------
#include "alcor/core/slam_data_adapter_i.h"
#include "alcor/act/p3_gateway.h"
//-------------------------------------------------------------------
namespace all { namespace act {

  class p3_odometry_adapter_t : public all::core::slam_data_adapter_i
  {
  public:
    ///
    p3_odometry_adapter_t(act::p3_gateway_ptr_t);

  public:
      ///(Not)Localized rotation angle in degrees.
    double get_current_rot(math::deg_t);
    ///(Not)Localized rotation angle in radians.
    double get_current_rot(math::rad_t);

  private:
    act::p3_gateway_ptr_t p3;
  };

//-------------------------------------------------------------------
  inline p3_odometry_adapter_t::p3_odometry_adapter_t(act::p3_gateway_ptr_t p3ptr)
  {
    p3 = p3ptr;
  }
//-------------------------------------------------------------------
      ///(Not)Localized rotation angle in degrees.
  inline double p3_odometry_adapter_t::get_current_rot(math::deg_t)
  {
    return p3->get_odometry().get_th(math::deg_tag);
  }
//-------------------------------------------------------------------
  ///(Not)Localized rotation angle in radians.
  inline double p3_odometry_adapter_t::get_current_rot(math::rad_t)
  {
    return p3->get_odometry().get_th(math::rad_tag);
  }
//-------------------------------------------------------------------
  typedef boost::shared_ptr<p3_odometry_adapter_t> p3_odometry_adapter_ptr;
//-------------------------------------------------------------------
}}
#endif //p3_odometry_adapter_t_H_INC