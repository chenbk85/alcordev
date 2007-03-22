#include "directed_perception_ptu_t.h"
//impl
#include "detail\lti\ltiDirectedPerceptionPTU.h"
#include <boost\timer.hpp>

#include "alcor/core/iniWrapper.h"
//---------------------------------------------------------------------------
namespace all { namespace act {
//---------------------------------------------------------------------------
directed_perception_ptu_t::directed_perception_ptu_t()
{
  //impl.reset(new lti::directedPerceptionPTU);
}
//---------------------------------------------------------------------------
directed_perception_ptu_t::~directed_perception_ptu_t()
{
 
}
//---------------------------------------------------------------------------
bool directed_perception_ptu_t::open(const std::string& ini)
{
  using namespace lti;

  iniWrapper config;

  //
 if ( config.Load(ini.c_str()) )
   printf("ini file: %s opened!\n",ini.c_str());
 else
  printf("ini file: %s NOT opened!\n",ini.c_str());

  //
  int com = config.GetInt("ptu:port",8);

  serial::parameters port_params;
  port_params.receiveTimeout = 250;
  port_params.stopBits = serial::parameters::One;
  port_params.baudRate = serial::parameters::Baud9600;
  port_params.parity = serial::parameters::No;
  port_params.characterSize = serial::parameters::Cs8;

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
  int panspeed  = config.GetInt("ptu:panvelstep",1000);
  int tiltspeed = config.GetInt("ptu:titlvelstep",1000);

  //
  directedPerceptionPTU::parameters par;
  par.connectionToPTU.setParameters(port_params);


  //Create
  impl.reset(new lti::directedPerceptionPTU(par) );

  //
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
  ptangle_.set_pan(math::deg_tag,0);
  ptangle_.set_tilt(math::deg_tag,0);
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
  ptangle_.set_pan(math::deg_tag,   pan);
  ptangle_.set_tilt(math::deg_tag,  tilt);

  return true;
}
//---------------------------------------------------------------------------
bool directed_perception_ptu_t::set_pantilt(const core::pantilt_angle_t& pantilt, float waitsec)
{
  //
  impl->setPanTilt(pantilt.get_pan(math::deg_tag), pantilt.get_tilt(math::deg_tag) );
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

  ptangle_=pantilt;

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
  ptangle_.set_pan(math::deg_tag,pan);
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
  ptangle_.set_tilt(math::deg_tag,tilt);
  return true;
}
//---------------------------------------------------------------------------
bool directed_perception_ptu_t::get_pantilt(float& pan_, float& tilt_) const
{  
  bool ans = impl->getPanTilt(pan_, tilt_);
  if(ans)
  {
  ptangle_.set_pan(math::deg_tag, pan_);
  ptangle_.set_tilt(math::deg_tag,tilt_);
  }
  return ans;
}
//---------------------------------------------------------------------------
bool directed_perception_ptu_t::get_current_pantilt(float& pan_, float& tilt_) const
{  
  bool ans = impl->getCurrentPanTilt(pan_, tilt_);
  if(ans)
  {
  ptangle_.set_pan(math::deg_tag, pan_);
  ptangle_.set_tilt(math::deg_tag,tilt_);
  }
  return ans;
}
//---------------------------------------------------------------------------
double directed_perception_ptu_t::get_pan() const
{
  return ptangle_.get_pan(math::deg_tag);
}
//---------------------------------------------------------------------------
double directed_perception_ptu_t::get_tilt() const
{
  return ptangle_.get_tilt(math::deg_tag);
}
//---------------------------------------------------------------------------
core::pantilt_angle_t directed_perception_ptu_t::get_fast_pantilt() const
{
  return ptangle_;
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