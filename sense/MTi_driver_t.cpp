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
bool MTi_driver_t::open(std::string& configfile)
{
  //
  iniWrapper mticonfig(configfile);
  char* port = mticonfig.GetStringAsChar("config:comport","COM10");
  //
  if (impl->mtcomm.openPort(port)!= MTRV_OK)
  {    
    printf("No device connected\n");
    return false;
  }

  // Put MTi/MTx in Config State
  if(impl->mtcomm.writeMessage(MID_GOTOCONFIG) != MTRV_OK)
  {
    printf("No device connected\n");
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
all::math::rpy_angle_t&   MTi_driver_t::get_euler()
{
  if(impl->mtcomm.readDataMessage(impl->data, impl->datalen) == MTRV_OK)
  {
		impl->mtcomm.getValue(VALUE_SAMPLECNT, impl->samplecounter, impl->data, BID_MASTER);

	  // Output: Euler
    impl->mtcomm.getValue(VALUE_ORIENT_EULER, impl->fdata, impl->data, detail::MTi_driver_impl::SENSOR_BID);
		printf("%6.1f\t%6.1f\t%6.1f\n",
				impl->fdata[0],
				impl->fdata[1], 
				impl->fdata[2]);

  }

}
//-----------------------------------------------------------------
}}//all::sense
//-----------------------------------------------------------------
//#define RESETORIENTATION_STORE		0
//#define RESETORIENTATION_HEADING	1
//#define RESETORIENTATION_GLOBAL		2
//#define RESETORIENTATION_OBJECT		3
//#define RESETORIENTATION_ALIGN		4