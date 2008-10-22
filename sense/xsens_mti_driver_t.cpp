#include "alcor/core/core.h"
#include "detail/MTi_driver_impl.cpp"
///////////////////////////////////////////////////////////////////
#include "alcor/sense/xsens_mti_driver_t.h"
#include <boost/bind.hpp>
//boost includes
#include <boost/program_options.hpp>
#include <fstream>
///////////////////////////////////////////////////////////////////
namespace all { namespace sense {
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
  xsens_mti_driver_t::xsens_mti_driver_t()
    :running_(true)

{
  impl.reset( new detail::MTi_driver_impl() );
}
//-----------------------------------------------------------------
xsens_mti_driver_t::~xsens_mti_driver_t()
{
  if(impl)
  {
    printf("Closing\n");
    impl->mtcomm.flush();
    impl->mtcomm.close();
  }
}
//-----------------------------------------------------------------
bool xsens_mti_driver_t::open(std::string& configfile)
{
	//-------------------------------------------------------
	// Importing settings from the ini
	namespace po = boost::program_options;
	po::variables_map vm;
	po::options_description desc("Allowed options");
	//
	desc.add_options()
	//physical sensor
	("config.comport", po::value<int>() )
	;

	std::ifstream fis(configfile.c_str(), std::ifstream::in);

	po::store(po::parse_config_file(fis, desc, true), vm);
	po::notify(vm);

				///		
	int port = vm["config.comport"].as<int>();

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
  impl->output_mode = OUTPUTMODE_TEMP | OUTPUTMODE_CALIB | OUTPUTMODE_ORIENT;
  //
  // OUTPUTSETTINGS_ORIENTMODE_QUATERNION;
	// OUTPUTSETTINGS_ORIENTMODE_EULER;
	// OUTPUTSETTINGS_ORIENTMODE_MATRIX;
  impl->output_settings = OUTPUTSETTINGS_ORIENTMODE_MATRIX|OUTPUTSETTINGS_TIMESTAMP_SAMPLECNT;

  // Set output mode and output settings for each attached MTi/MTx  - BID_MT
  if (impl->mtcomm.setDeviceMode( impl->output_mode
                                , impl->output_settings
                                , impl->SENSOR_BID) != MTRV_OK) 
  {
			printf("Could not set (all) device mode(s)\n");
			return false;
	}
  
  // Put MTi/MTx in Measurement State
  impl->mtcomm.writeMessage(MID_GOTOMEASUREMENT);
  //impl->mtcomm.flush();

  core::SLEEP_MSECS(10);
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
  int nsamples = 0;
  //reset(sense::heading_reset);
  timestamp_.restart();
  while (running_)
  {
    print_calibdata();
    nsamples++;
    //core::BOOST_SLEEP(1);
    boost::thread::yield();
  }
  printf("\nRATE: %.2f\n", nsamples/timestamp_.elapsed());
  //core::BOOST_SLEEP(10);
}
//-----------------------------------------------------------------
void xsens_mti_driver_t::print_calibdata()
{
  impl->mtcomm.flush();
  //
  if ( impl->mtcomm.readDataMessage(impl->data, impl->datalen) == MTRV_OK && !_kbhit() )
  { 
  //local scope...
  //
  double elapsed = timestamp_.elapsed();
  //  
  unsigned short samplecounter;

  //
  impl->mtcomm.getValue(VALUE_SAMPLECNT, samplecounter, impl->data, BID_MASTER);
  //
  switch((impl->output_settings & OUTPUTSETTINGS_ORIENTMODE_MASK))
  {
    //QUATERNIONS
  case OUTPUTSETTINGS_ORIENTMODE_QUATERNION:
		// Output: quaternions
    impl->mtcomm.getValue(VALUE_ORIENT_QUAT, impl->quat, impl->data, impl->SENSOR_BID);
    break;    
    //ROTATION MATRIX
  case OUTPUTSETTINGS_ORIENTMODE_MATRIX:
		// Output: Cosine Matrix
    impl->mtcomm.getValue(VALUE_ORIENT_MATRIX, impl->rot, impl->data, impl->SENSOR_BID);
    break;
    //EULER
  case OUTPUTSETTINGS_ORIENTMODE_EULER:
    //
    impl->mtcomm.getValue(VALUE_ORIENT_EULER, impl->rpy, impl->data, impl->SENSOR_BID);
    break;

  }

	// Output Calibrated data
  impl->mtcomm.getValue(VALUE_CALIB_ACC, impl->acc, impl->data, impl->SENSOR_BID);

  ////  
  float mti_data[12];
  ////
  mti_data[0] = impl->rot[0]; //a
  mti_data[1] = impl->rot[1]; //b
  mti_data[2] = impl->rot[2]; //c
  mti_data[3] = impl->rot[3]; //d
  mti_data[4] = impl->rot[4]; //e
  mti_data[5] = impl->rot[5]; //f
  mti_data[6] = impl->rot[6]; //g
  mti_data[7] = impl->rot[7]; //h
  mti_data[8] = impl->rot[8]; //i
  mti_data[9] = impl->acc[0]; //accx
  mti_data[10]= impl->acc[1];//accy
  mti_data[11]= impl->acc[2];//accz

  ///
  processor_.update(mti_data, elapsed);

  }
  else
  {
    printf ("Cannot Read DATA!\n");
  }
}
//-----------------------------------------------------------------
//
void xsens_mti_driver_t::reset(tags::heading_reset_t)
{
  impl->mtcomm.writeMessage(MID_RESETORIENTATION,RESETORIENTATION_HEADING,LEN_RESETORIENTATION);
}
//-----------------------------------------------------------------
//
void xsens_mti_driver_t::reset(tags::global_reset_t)
{
  impl->mtcomm.writeMessage(MID_RESETORIENTATION,RESETORIENTATION_GLOBAL,LEN_RESETORIENTATION);
}
//-----------------------------------------------------------------
//
void xsens_mti_driver_t::reset(tags::object_reset_t)
{
  impl->mtcomm.writeMessage(MID_RESETORIENTATION,RESETORIENTATION_OBJECT,LEN_RESETORIENTATION);
}
//-----------------------------------------------------------------
//
void xsens_mti_driver_t::reset(tags::align_reset_t)
{
  impl->mtcomm.writeMessage(MID_RESETORIENTATION,RESETORIENTATION_ALIGN,LEN_RESETORIENTATION);
}
//-----------------------------------------------------------------
///////////////////////////////////////////////////////////////////
}}//all::sense