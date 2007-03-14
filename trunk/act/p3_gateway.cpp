#include "p3_gateway.h"
#include "detail/p3_gateway_impl.cpp"
#include "alcor/core/iniWrapper.h"
//---------------------------------------------------------------------------
namespace all{namespace act{
//---------------------------------------------------------------------------
p3_gateway::p3_gateway()
{

}
//---------------------------------------------------------------------------
///
boost::shared_ptr<p3_gateway> p3_gateway::create()
{
boost::shared_ptr<p3_gateway> 
  px(new p3_gateway);
  // use px as 'this_'
  return px;
}
//---------------------------------------------------------------------------
p3_gateway::~p3_gateway()
{

}
//---------------------------------------------------------------------------
bool p3_gateway::open(std::string inifile)
{
  iniWrapper ini;
  if ( ini.Load(inifile.c_str()) ) 
  {
    bool is_p3dx = true;

    if(ini.GetInt("config:p3dx", 1))
    {
      robot_model_ = p3DX;
    }
    else
    {
      is_p3dx = false;
      robot_model_ = p3AT;
    }

    impl.reset(new detail::p3_gateway_impl(is_p3dx));

    if(ini.GetInt("config:serialmode", 1))
    {
      printf("Opening serial connection\n");
      char* port = ini.GetStringAsChar("config:comport","COM5");
      return (impl->serial_connect(port));
    }
  }
  return false;
}
//---------------------------------------------------------------------------
//
math::pose2d  p3_gateway::get_odometry()
{
  math::pose2d mypose;
  ArPose arpose = impl->m_robot->getPose();
  //TODO: dovrebbe essere così
  mypose.set_x1(arpose.getX()/1000.0);
  mypose.set_x2(arpose.getY()/1000.0);
  mypose.set_th(arpose.getTh(),math::deg_tag);

  //printf("Gateway: %.2f\n", arpose.getTh());
  return mypose;
}
//---------------------------------------------------------------------------
void p3_gateway::set_heading(const math::angle& heading)
{
  impl->m_robot->lock();
  impl->m_robot->setHeading(heading.deg());
  impl->m_robot->unlock();	
}
//---------------------------------------------------------------------------
void p3_gateway::set_delta_heading(const math::angle& deltaheading)
{
  impl->m_robot->lock();
  impl->m_robot->setDeltaHeading(deltaheading.deg());
  impl->m_robot->unlock();
}
//---------------------------------------------------------------------------  
///
void p3_gateway::set_rot_vel(double rotvel)
{
  impl->m_robot->lock();
  impl->m_robot->setRotVel(rotvel);
  impl->m_robot->unlock();
}
//---------------------------------------------------------------------------
///
void p3_gateway::set_vel(double vel)
{
  impl->m_robot->lock();
  impl->m_robot->setVel(vel);
  impl->m_robot->unlock();
}
//---------------------------------------------------------------------------
    ///Enable Stop Action
void p3_gateway::enable_stop_mode()
{
	impl->m_robot->clearDirectMotion();
	impl->m_stop->activateExclusive();
	printf("The robot will now just stop.\n");
}
//---------------------------------------------------------------------------
void p3_gateway::enable_wander_mode()
{
  impl->m_robot->clearDirectMotion();
  impl->m_wander->activateExclusive();
  printf("Wandering.\n");
}
//---------------------------------------------------------------------------
void p3_gateway::enable_follow_mode()
{
  impl->m_robot->clearDirectMotion();
  impl->m_follow->activateExclusive();
  printf("The robot will now  follow a target.\n");
}
//---------------------------------------------------------------------------
void p3_gateway::set_target_to_follow 
    (const math::point2d& target, double speed)
{
  impl->m_robot->lock();
  //
  impl->m_ac_follow->setSpeed(speed);
  //setEncoderGoalRel
  impl->m_ac_follow->setGoalRel(  
                      target.magnitude()*1000.0
                    , target.orientation().deg()
                    , false, false
                    );
  impl->m_robot->unlock();
}
//---------------------------------------------------------------------------
  }}//all::act
//---------------------------------------------------------------------------