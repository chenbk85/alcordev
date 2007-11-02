#include "alcor/core/core.h"
#include "alcor/core/config_parser_t.hpp"
#include "detail/MTi_driver_impl.cpp"
///////////////////////////////////////////////////////////////////
#include "alcor/sense/xsens_mti_driver_t.h"
#include <boost/bind.hpp>
///////////////////////////////////////////////////////////////////
namespace all { namespace sense {
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
  xsens_mti_driver_t::xsens_mti_driver_t():running_(true)
{
  impl.reset( new detail::MTi_driver_impl() );
}
//-----------------------------------------------------------------
xsens_mti_driver_t::~xsens_mti_driver_t()
{
  if(impl)
  {
   // printf("Restoring Skip Factor\n");
   // impl->mtcomm.writeMessage(MID_GOTOCONFIG,impl->SENSOR_BID);
	  //impl->mtcomm.setSetting(MID_SETOUTPUTSKIPFACTOR,impl->skip_factor,LEN_OUTPUTSKIPFACTOR,impl->SENSOR_BID);
    // 
    printf("Closing\n");
    impl->mtcomm.flush();
    impl->mtcomm.close();
  }
}
//-----------------------------------------------------------------
bool xsens_mti_driver_t::open(std::string& configfile)
{
  //
  core::config_parser_t mticonfig;
  mticonfig.load(core::tags::ini,configfile);
  //
  int port = mticonfig.get("config.comport",11);
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

  //
  int outputMode = OUTPUTMODE_TEMP | OUTPUTMODE_CALIB | OUTPUTMODE_ORIENT;
  //
  //outputSettings = OUTPUTSETTINGS_ORIENTMODE_QUATERNION;
	//outputSettings = OUTPUTSETTINGS_ORIENTMODE_EULER;
	//outputSettings = OUTPUTSETTINGS_ORIENTMODE_MATRIX;
  int outputSettings = OUTPUTSETTINGS_ORIENTMODE_EULER|OUTPUTSETTINGS_TIMESTAMP_SAMPLECNT;

  // Set output mode and output settings for each attached MTi/MTx
  if (impl->mtcomm.setDeviceMode( outputMode
                                , outputSettings
                                , impl->SENSOR_BID) != MTRV_OK) 
  {
			printf("Could not set (all) device mode(s)\n");
			return false;
	}
  
	//// store current skip factor so we can restore it later
	//printf("Getting skip factor\n");
 // unsigned long value;
	//impl->mtcomm.reqSetting(MID_REQOUTPUTSKIPFACTOR, value, impl->SENSOR_BID);
	//impl->skip_factor = (unsigned short) (value & 0xFFFF);
 // 
	//printf("Setting skip factor\n");
	//if (impl->mtcomm.setSetting(  MID_SETOUTPUTSKIPFACTOR
 //                             ,	0xFFFF
 //                             , LEN_OUTPUTSKIPFACTOR
 //                             , impl->SENSOR_BID) != MTRV_OK) 
 // {
	//	printf("Cannot set skip factor\n");
	//	return false;
	//}

  // Put MTi/MTx in Measurement State
	impl->mtcomm.writeMessage(MID_GOTOMEASUREMENT);
  impl->mtcomm.flush();

  Sleep(10);
  return true;
}
//-----------------------------------------------------------------
void xsens_mti_driver_t::run_mti()
{
  loop_thread_.reset(new boost::thread(boost::bind(&xsens_mti_driver_t::mti_loop_, this)));
}
//-----------------------------------------------------------------
void xsens_mti_driver_t::stop_mti()
{
  running_ = false;
}

//-----------------------------------------------------------------
void xsens_mti_driver_t::mti_loop_()
{
  timestamp_.restart();
  while (running_)
  {
    print_calibdata();
    core::BOOST_SLEEP(1);
    boost::thread::yield();
  }
}
//-----------------------------------------------------------------
void xsens_mti_driver_t::print_calibdata()
{
  impl->mtcomm.flush();
  //
  if ( impl->mtcomm.readDataMessage(impl->data, impl->datalen) == MTRV_OK && !_kbhit() )
  {
  //  
  unsigned short samplecounter;
  //
  double elapsed = timestamp_.elapsed();
  //
  impl->mtcomm.getValue(VALUE_SAMPLECNT, samplecounter, impl->data, BID_MASTER);

	// Output Calibrated data
  impl->mtcomm.getValue(VALUE_CALIB_ACC, impl->fdata, impl->data, impl->SENSOR_BID);
  printf("TSTAMP: %.2f SAMPLE: %d -- % :%6.2f\t%6.2f\t%6.2f\n"
                    , elapsed
                    , samplecounter
                    , impl->fdata[0] 
								    , impl->fdata[1] 
								    , impl->fdata[2]);

	  //mtcomm.getValue(VALUE_CALIB_GYR, fdata, data, BID_MT + i);
	  //printf("%6.2f\t%6.2f\t%6.2f", fdata[0], 
			//					    fdata[1], 
			//					    fdata[2]);
	  //mtcomm.getValue(VALUE_CALIB_MAG, fdata, data, BID_MT + i);
	  //printf("%6.2f\t%6.2f\t%6.2f", fdata[0], 
			//					    fdata[1], 
			//					    fdata[2]);
  }
  else
  {
    printf ("Cannot Read DATA!\n");
  }
}
//-----------------------------------------------------------------
//
void xsens_mti_driver_t::reset(heading_reset_t)
{
  impl->mtcomm.writeMessage(MID_RESETORIENTATION,RESETORIENTATION_HEADING,LEN_RESETORIENTATION);
}
//-----------------------------------------------------------------
//
void xsens_mti_driver_t::reset(global_reset_t)
{
  impl->mtcomm.writeMessage(MID_RESETORIENTATION,RESETORIENTATION_GLOBAL,LEN_RESETORIENTATION);
}
//-----------------------------------------------------------------
//
void xsens_mti_driver_t::reset(object_reset_t)
{
  impl->mtcomm.writeMessage(MID_RESETORIENTATION,RESETORIENTATION_OBJECT,LEN_RESETORIENTATION);
}
//-----------------------------------------------------------------
//
void xsens_mti_driver_t::reset(align_reset_t)
{
  impl->mtcomm.writeMessage(MID_RESETORIENTATION,RESETORIENTATION_ALIGN,LEN_RESETORIENTATION);
}
//-----------------------------------------------------------------
///////////////////////////////////////////////////////////////////
}}//all::sense