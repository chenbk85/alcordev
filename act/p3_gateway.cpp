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
  bool p3_gateway::open(std::string inifile)
  {
    iniWrapper ini;
    if ( ini.Load(inifile.c_str()) ) 
    {
      if(ini.GetInt("config:serialmode", 1))
      {
        char* port = ini.GetStringAsChar("config:comport","COM5");
        return (impl->serial_connect(port));
      }
     
    }
    return false;
  }
  //---------------------------------------------------------------------------
  //
  math::pose2d const& p3_gateway::get_odometry()const
  {
    ArPose arpose = impl->m_robot->getPose();
    math::pose2d mypose;

    mypose.set_x1(arpose.getX());
    mypose.set_x2(arpose.getY());
    mypose.set_th(arpose.getTh(),math::deg_tag);
  }
//---------------------------------------------------------------------------
  }}//all::act
//---------------------------------------------------------------------------