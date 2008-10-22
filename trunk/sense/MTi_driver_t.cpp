#include "MTi_driver_t.h"
//-----------------------------------------------------------------
#include "detail/MTi_driver_impl.cpp"
//-----------------------------------------------------------------
#include <boost/program_options.hpp>
#include <fstream>
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
	{
		printf("Closing\n");
		  // Put MTi/MTx in Config State
		if(impl->mtcomm.writeMessage(MID_GOTOCONFIG) != MTRV_OK)
		{
		printf("Setting skip factor\n");

		if (impl->mtcomm.setSetting(MID_SETOUTPUTSKIPFACTOR
							,	0x0000
							, LEN_OUTPUTSKIPFACTOR
							, impl->SENSOR_BID) != MTRV_OK) 
		printf("Cannot set skip factor\n");
		}

		impl->mtcomm.flush();
		impl->mtcomm.close();
	}
}

//-----------------------------------------------------------------
bool MTi_driver_t::open(std::string configfile)
{
	//-------------------------------------------------------
	// Importing settings from the ini
	namespace po = boost::program_options;

	int port;
	int mode;

		po::variables_map vm;
		po::options_description desc("Allowed options");

	try{
		//
		desc.add_options()
		//physical sensor
		("config.comport", po::value<int>() )
		("config.mode",    po::value<int>() )
		;

		std::ifstream fis(configfile.c_str(), std::ifstream::in);

		po::store(po::parse_config_file(fis, desc, true), vm);

		//
		po::notify(vm);

		//		
		port  = vm["config.comport"].as<int>();
		mode =  vm["config.mode"].as<int>();
	}
	catch (std::exception& e)
	{
		std::cout 
			<< e.what()
			<< std::endl;
		port = 9;
	}
  //-------------------------------------------------------
  //
  printf("Search MTi on port: COM%d\n", port);
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
  int output_settings;

  switch (mode)
  {
  case 0: //euler
	  output_settings = OUTPUTSETTINGS_ORIENTMODE_EULER
						|OUTPUTSETTINGS_TIMESTAMP_SAMPLECNT;
	  break;
  case 1: //DCM - Direction Cosine
	  	  output_settings = OUTPUTSETTINGS_ORIENTMODE_MATRIX
						|OUTPUTSETTINGS_TIMESTAMP_SAMPLECNT;
	  break;
  case 2: //Quat
	  	  output_settings = OUTPUTSETTINGS_ORIENTMODE_QUATERNION
						|OUTPUTSETTINGS_TIMESTAMP_SAMPLECNT;
	  break;
  default:
	  output_settings = OUTPUTSETTINGS_ORIENTMODE_EULER
						|OUTPUTSETTINGS_TIMESTAMP_SAMPLECNT;
	  break;
  }
  // Set output mode and output settings for each attached MTi/MTx
  if (impl->mtcomm.setDeviceMode(OUTPUTMODE_ORIENT |OUTPUTMODE_CALIB
								,output_settings
								,impl->SENSOR_BID)
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
 //
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
///
bool MTi_driver_t::req_data()
{
	impl->data[IND_PREAMBLE] = PREAMBLE;
	impl->data[IND_BID] = detail::MTi_driver_impl::SENSOR_BID;
	impl->data[IND_MID] = MID_REQDATA;
	impl->data[IND_LEN] = 0;
	impl->data[IND_LEN+1] = - ((unsigned char) (detail::MTi_driver_impl::SENSOR_BID + MID_REQDATA));
	
	impl->mtcomm.writeData(impl->data, IND_LEN+2);

  return (impl->mtcomm.readDataMessage(impl->data, impl->datalen) == MTRV_OK);
}
//-----------------------------------------------------------------
//
all::math::rpy_angle_t   MTi_driver_t::get_euler()
{    
  math::rpy_angle_t rpy;

	impl->mtcomm.getValue(VALUE_SAMPLECNT, impl->samplecounter, impl->data, BID_MASTER);
    impl->mtcomm.getValue(VALUE_ORIENT_EULER, impl->fdata, impl->data, detail::MTi_driver_impl::SENSOR_BID);

    rpy.roll.set_deg(impl->fdata[0]);
    rpy.pitch.set_deg(impl->fdata[1]);
    rpy.yaw.set_deg (impl->fdata[2]);

    return rpy;
}

//-----------------------------------------------------------------
void  MTi_driver_t::get_acc(float acc_[])
{
	impl->mtcomm.getValue(VALUE_CALIB_ACC, acc_, impl->data, impl->SENSOR_BID);

}
//-----------------------------------------------------------------
	///9 floats you get GS
	//[a b c d e f g h i]
	//GS [a d g; b e h; c f i] //MTi(S) to global(G)
	//SG [a b c;d e f; g h i]  //global(G) to MTi(S)
void MTi_driver_t::get_dcm(float dcm_[])
{
	impl->mtcomm.getValue(VALUE_ORIENT_MATRIX, dcm_, impl->data, impl->SENSOR_BID);
}
//-----------------------------------------------------------------
}}//all::sense
//-----------------------------------------------------------------
//#define RESETORIENTATION_STORE		0
//#define RESETORIENTATION_HEADING	1
//#define RESETORIENTATION_GLOBAL		2
//#define RESETORIENTATION_OBJECT		3
//#define RESETORIENTATION_ALIGN		4