#include "directed_perception_ptu_t.h"
//impl
#include "detail\lti\ltiDirectedPerceptionPTU.h"
#include <boost\timer.hpp>
#include "alcor\core\config_parser_t.h"
//---------------------------------------------------------------------------
namespace all { namespace act {
//---------------------------------------------------------------------------
directed_perception_ptu_t::directed_perception_ptu_t()
{
  impl.reset(new lti::directedPerceptionPTU);
}
//---------------------------------------------------------------------------
bool directed_perception_ptu_t::open()
{
  using namespace lti;

  core::config_parser_t config;

  //
  config.load(core::ini,"config/dpptu_conf.ini");

  //
  int com = config.as_int("ptu.port",8);  
  serial::parameters port_params;
  port_params.receiveTimeout = 125;

  //Opening port
  printf("Opening Port %d\n", com);

  //
  switch(com)
  {  

  case 4:
    port_params.port = serial::parameters::Com4;
    break;
  case 5:
    port_params.port = serial::parameters::Com5;
    break;
  case 6:
    port_params.port = serial::parameters::Com6;
    break;
  case 7:
    port_params.port = serial::parameters::Com7;
    break;
  case 8:
    port_params.port = serial::parameters::Com8;
    break;

  default:
    port_params.port = serial::parameters::Com7;
    break;
  }

  //
  int panspeed  = config.as_int("ptu.panvelstep",1000);
  int tiltspeed = config.as_int("ptu.titlvelstep",1000);

  //
  directedPerceptionPTU::parameters par;
  par.connectionToPTU.setParameters(port_params);

  impl->setParameters(par);

  if(impl->initialize())
    {
      impl->setAngleFormat(panTiltUnit::parameters::Degrees);
      impl->setPanSpeed((float)panspeed);
      impl->setTiltSpeed((float)tiltspeed);  

    }
  else
    return false;

  return true;

}
//---------------------------------------------------------------------------
bool directed_perception_ptu_t::set_angle_format(math::deg_t)
{
  return impl->setAngleFormat(lti::panTiltUnit::parameters::Degrees);
}
bool directed_perception_ptu_t::set_angle_format(math::rad_t)
{  
  return impl->setAngleFormat(lti::panTiltUnit::parameters::Radiant);
}
//---------------------------------------------------------------------------
bool directed_perception_ptu_t::set_exec_mode(immidiate_t)
{
  return impl->setExecMode(lti::directedPerceptionPTU::parameters::immidiate);
}
//---------------------------------------------------------------------------
bool directed_perception_ptu_t::set_exec_mode(slaved_t)
{
  return impl->setExecMode(lti::directedPerceptionPTU::parameters::slaved);
}
//---------------------------------------------------------------------------
bool directed_perception_ptu_t::reset()
{
  ptangle_.pan = 0;
  ptangle_.tilt = 0;
  return impl->reset();
}
//---------------------------------------------------------------------------
bool directed_perception_ptu_t::set_pantilt(float pan, float tilt, float waitsec)
{
  //
  impl->setPanTilt(pan, tilt);
  //
	impl->awaitPosCommandCompletion();
  //  
  if(waitsec>0)
  {
  boost::timer quit_timer;
  quit_timer.restart();
  while ( !impl->isPTUidle())
    {if(quit_timer.elapsed() > waitsec) break;}
  }
  ptangle_.pan = pan;
  ptangle_.tilt = tilt;

  return true;
}
//---------------------------------------------------------------------------
bool directed_perception_ptu_t::set_pan(float pan, float wait  )
{
  //
  impl->setPan(pan);
  //
	impl->awaitPosCommandCompletion();
  //  
  if(wait>0)
  {
  boost::timer quit_timer;
  quit_timer.restart();
  while ( !impl->isPTUidle())
    {if(quit_timer.elapsed() > wait) break;}
  }
  ptangle_.pan = pan;
  return true;
}
//---------------------------------------------------------------------------
bool directed_perception_ptu_t::set_tilt(float tilt, float wait  )
{
  //
  impl->setTilt(tilt);
  //
	impl->awaitPosCommandCompletion();
  //  
  if(wait>0)
  {
  boost::timer quit_timer;
  quit_timer.restart();
  while ( !impl->isPTUidle())
    {if(quit_timer.elapsed() > wait) break;}
  }
  ptangle_.tilt = tilt;
  return true;
}
//---------------------------------------------------------------------------
bool directed_perception_ptu_t::get_pantilt(float& pan_, float& tilt_) const
{  
  bool ans = impl->getPanTilt(pan_, tilt_);
  if(ans)
  {
    ptangle_.pan  = pan_;
    ptangle_.tilt = tilt_;
  }
  return ans;
}
//---------------------------------------------------------------------------
bool directed_perception_ptu_t::get_current_pantilt(float& pan, float& tilt) const
{  
  bool ans = impl->getCurrentPanTilt(pan, tilt);
  if(ans)
  {
  ptangle_.pan = pan;
  ptangle_.tilt = tilt;
  }
  return ans;
}
//---------------------------------------------------------------------------
double directed_perception_ptu_t::get_pan() const
{
    return ptangle_.pan;
}
//---------------------------------------------------------------------------
double directed_perception_ptu_t::get_tilt() const
{
    return ptangle_.tilt;
}
//---------------------------------------------------------------------------
bool directed_perception_ptu_t::await_command_completion()
{
  return impl->awaitPosCommandCompletion();
}
//---------------------------------------------------------------------------
bool directed_perception_ptu_t::update_pantilt()
{
  return impl->updatePanTilt();
}
//---------------------------------------------------------------------------
}}//namespaces
//---------------------------------------------------------------------------