#include "p3_gateway.h"
#include "detail/p3_gateway_impl.cpp"
#include "alcor/core/iniWrapper.h"
//---------------------------------------------------------------------------
namespace all{namespace act{
//---------------------------------------------------------------------------
p3_gateway::p3_gateway()
{
impl.reset(new detail::p3_gateway_impl());
}
//---------------------------------------------------------------------------
p3_gateway::~p3_gateway()
{
	if(impl->m_robot)  
			impl->m_robot->stopRunning();
  Aria::shutdown();
}
//---------------------------------------------------------------------------
bool p3_gateway::open(std::string inifile)
{
  iniWrapper ini;
  if ( ini.Load(inifile.c_str()) ) 
  {
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
  ArPose arpose = impl->m_robot->getPose();
  math::pose2d mypose;

  mypose.set_x1(arpose.getX());
  mypose.set_x2(arpose.getY());
  mypose.set_th(arpose.getTh(),math::deg_tag);

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
    ///Enable Stop Action
void p3_gateway::enable_stop_mode()
{
	impl->m_robot->clearDirectMotion();
	impl->m_stop->activateExclusive();
	printf("The robot will now just stop.\n");
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
  //
  impl->m_ac_follow->setSpeed(speed);
  //
  impl->m_ac_follow->setGoalRel(  
                      target.magnitude()
                    , target.orientation().deg()
                    , false, false
                    );
}
//---------------------------------------------------------------------------
  }}//all::act
//---------------------------------------------------------------------------