#include "MTi_driver_t.h"
#include "alcor/core/iniwrapper.h"
#include "detail/MTi_driver_impl.cpp"
///////////////////////////////////////////////////////////////////
namespace all { namespace sense {
///////////////////////////////////////////////////////////////////
MTi_driver_t::MTi_driver_t()
{
  impl.reset( new detail::MTi_driver_impl() );
}
//-----------------------------------------------------------------
MTi_driver_t::~MTi_driver_t()
{
  if(impl)
    impl->mtcomm.close();
}
//-----------------------------------------------------------------
bool MTi_driver_t::open(std::string configfile)
{
  //
  iniWrapper mticonfig(configfile);
  int port = mticonfig.GetInt("config:comport",10);
  //
  printf("Search MTi on port: %d\n", port);
  //
  if (impl->mtcomm.openPort(port)!= MTRV_OK)
  {    
    printf("No device connected\n");
    return false;
  }

  // Put MTi/MTx in Config State
  if(impl->mtcomm.writeMessage(MID_GOTOCONFIG) != MTRV_OK)
  {
    printf("writeMessage(MID_GOTOCONFIG): No device connected\n");
    return false;
  }

  // Set output mode and output settings for each attached MTi/MTx
  if (impl->mtcomm.setDeviceMode(OUTPUTMODE_ORIENT
                          ,OUTPUTSETTINGS_ORIENTMODE_EULER|OUTPUTSETTINGS_TIMESTAMP_SAMPLECNT, 
                          detail::MTi_driver_impl::SENSOR_BID)
            != MTRV_OK) 
  {
  printf("Could not set (all) device mode(s)\n");
  return false;
	}

  
  printf("Setting skip factor\n");
  if (impl->mtcomm.setSetting(MID_SETOUTPUTSKIPFACTOR
                            ,	0xFFFF
                            , LEN_OUTPUTSKIPFACTOR
                            , impl->SENSOR_BID) != MTRV_OK) {
		printf("Cannot set skip factor\n");
		return false;
  }

  // Put MTi/MTx in Measurement State
	impl->mtcomm.writeMessage(MID_GOTOMEASUREMENT);
  impl->mtcomm.flush();

  Sleep(10);
  return true;
}
//-----------------------------------------------------------------
//
void MTi_driver_t::reset(tags::heading_reset_t)
{
  impl->mtcomm.writeMessage(MID_RESETORIENTATION,RESETORIENTATION_HEADING,LEN_RESETORIENTATION);
}
//-----------------------------------------------------------------
//
void MTi_driver_t::reset(tags::global_reset_t)
{
  impl->mtcomm.writeMessage(MID_RESETORIENTATION,RESETORIENTATION_GLOBAL,LEN_RESETORIENTATION);
}
//-----------------------------------------------------------------
//
void MTi_driver_t::reset(tags::object_reset_t)
{
  impl->mtcomm.writeMessage(MID_RESETORIENTATION,RESETORIENTATION_OBJECT,LEN_RESETORIENTATION);
}
//-----------------------------------------------------------------
//
void MTi_driver_t::reset(tags::align_reset_t)
{
  impl->mtcomm.writeMessage(MID_RESETORIENTATION,RESETORIENTATION_ALIGN,LEN_RESETORIENTATION);
}
//-----------------------------------------------------------------
//
all::math::rpy_angle_t   MTi_driver_t::get_euler()
{    
  math::rpy_angle_t rpy;

  impl->data[IND_PREAMBLE] = PREAMBLE;
	impl->data[IND_BID] = detail::MTi_driver_impl::SENSOR_BID;
	impl->data[IND_MID] = MID_REQDATA;
	impl->data[IND_LEN] = 0;
	impl->data[IND_LEN+1] = - ((unsigned char) (detail::MTi_driver_impl::SENSOR_BID + MID_REQDATA));
	
	impl->mtcomm.writeData(impl->data, IND_LEN+2);

  if(impl->mtcomm.readDataMessage(impl->data, impl->datalen) == MTRV_OK)
  {
		impl->mtcomm.getValue(VALUE_SAMPLECNT, impl->samplecounter, impl->data, BID_MASTER);
	  // Output: Euler
    impl->mtcomm.getValue(VALUE_ORIENT_EULER, impl->fdata, impl->data, detail::MTi_driver_impl::SENSOR_BID);

    rpy.roll.set_deg(impl->fdata[0]);
    rpy.pitch.set_deg(impl->fdata[1]);
    rpy.yaw.set_deg (impl->fdata[2]);
  }
    return rpy;
}
//-----------------------------------------------------------------
}}//all::sense
//-----------------------------------------------------------------
//#define RESETORIENTATION_STORE		0
//#define RESETORIENTATION_HEADING	1
//#define RESETORIENTATION_GLOBAL		2
//#define RESETORIENTATION_OBJECT		3
//#define RESETORIENTATION_ALIGN		4