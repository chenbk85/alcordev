//---------------------------------------------------------------------------
#ifndef p3dx_client_t_H_INCLUDED
#define p3dx_client_t_H_INCLUDED
//---------------------------------------------------------------------------
#pragma comment (lib, "p3dx_client_t.lib")
//---------------------------------------------------------------------------
#include "alcor/math/all_math.h"
#include "alcor/core/core.h"
//---------------------------------------------------------------------------
#include <boost/shared_ptr.hpp>
//---------------------------------------------------------------------------
namespace all{
	namespace act{
//---------------------------------------------------------------------------
  class p3dx_client_t 
	{
  protected:
    ///
    p3dx_client_t(){};

  public:
    ///deprecated .. unuseful
    //virtual void start() = 0;
    ///
		virtual void stop()=0;
    ///
    virtual core::net_state_t net_state() const = 0;
    ///
		virtual void set_localized(double, double, double)=0;
    ///
    virtual void set_localized(const math::pose2d&)=0;
    ///
    virtual void set_vel(double)=0;
    ///
		virtual void set_rot_vel(double)=0;
    ///
		virtual void stop_robot()=0;
    //TODO: aggiungere i servizi!
    ///
    virtual void move(double distance)=0;
    ///
    virtual void move_diff(const math::point2d&) = 0;

    ///
    virtual void set_heading(const math::angle&) = 0;
    ///
    virtual void set_delta_heading(const math::angle&) = 0;

    ///
    virtual void wander() = 0;
    ///
    virtual void set_local_dir (double delta_degree) = 0;
    ///
    virtual math::pose2d const& get_odometry()  const = 0;
    ///
    virtual math::pose2d const& get_localized() const = 0;

	};
////---------------------------------------------------------------------------
  typedef   boost::shared_ptr<p3dx_client_t> p3dx_client_sptr;
////---------------------------------------------------------------------------
  boost::shared_ptr<p3dx_client_t> create_p3dx_client();
//---------------------------------------------------------------------------
		}//alcor
	}//act
//---------------------------------------------------------------------------
#endif //p3dx_client_H_INCLUDED
//---------------------------------------------------------------------------
