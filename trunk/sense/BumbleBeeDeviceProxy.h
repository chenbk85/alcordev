#ifndef BUMBLEBEEDEVICEPROXY_H
#define BUMBLEBEEDEVICEPROXY_H
//---------------------------------------------------------------------------
//#include <cstdio>
#include <string>
#include "alcor/core/core.h"
//---------------------------------------------------------------------------
#include "triclops.h"
#include "digiclops.h"
#include "pnmutils.h"
//Bumblebee Settings
#include "Settings.h"
#include <memory>
#include <boost/shared_array.hpp>
using std::auto_ptr;
using namespace all;
//---------------------------------------------------------------------------
using std::string;
//---------------------------------------------------------------------------
namespace alcor{
	namespace device {
//---------------------------------------------------------------------------
///The Gateway for the Bumblebee device.
class BumbleBeeDeviceProxy
{
public:	
	///Default Constructor.
	BumbleBeeDeviceProxy(DigiclopsImageResolution res = DIGICLOPS_320x240);
	
	~BumbleBeeDeviceProxy();
	
private:
	///Init. Called in c'tor
	void _init_context();
	///Called from init_all()and init_right_stereo();
	void _digiclops_private_init();
	///Compute depth map. 
	void _do_triclops_process();
	///
	void _destroy_context();

public:
//* ALL_IMAGES which grabs all of the images
//* STEREO_IMAGE which grabs only the stereo image
//* RIGHT_IMAGE | STEREO_IMAGE which grabs both the right
	//Choose one
	///
	void init_all();
	///
	void init_right_and_stereo();
	///
	void init_stereo_processing();

	///
    bool isRunning(){return bRunning;};

//[Setters]    
public:
  ///Gain
  void set_gain(int);
	///Set Disparity Range
	void set_disparity_range(int,int); 

	///Zlimit
	void setZlimit(double);
	///
	double getZlimit() const;

    ///White Balance
    void set_white(int,int);  
	///
	void set_frame_rate(DigiclopsFrameRate);

public:
	///
	int nrows(){return m_rows;};
	///
	int ncols(){return m_cols;};

	///
	void start_grabbing();
	///
	void stop_grabbing();

	///Grabs Stereo Color Image.
	void grab_color_left();
	///
	void grab_color_right();
	///
	void grab3d_color_left();
	///
	void grab3d_color_right();

	///
	TriclopsColorImage  colorImage;

public:
	TriclopsInput       stereoData;
	TriclopsInput       colorData;
	TriclopsImage16     depthImage16;

    boost::shared_array<core::single_t>
                        raw_depth_image;
                        

	TriclopsContext     triclops;
	DigiclopsContext    digiclops;
	Settings*			m_settings;

public:    
    float   focal;
    float   centerRow;
    float   centerCol;
    float   base;
    int     serialNum;
	int		m_rows;
	int		m_cols;

private:
    bool bRunning;
	double zlimit;

private:
    void handleDigiclopsError(char*, DigiclopsError);
    void handleTriclopsError(char*, TriclopsError);

};
//---------------------------------------------------------------------------
	}//alcor 
}//device
//---------------------------------------------------------------------------
//--------------------------------------------------------------------------+
#endif //BUMBLEBEEDEVICEPROXY_H