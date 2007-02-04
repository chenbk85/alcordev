#include "BumbleBeeDeviceProxy.h"

//Bumblebee Settings
#include "Settings.h"
//---------------------------------------------------------------------------
namespace alcor{
	namespace device {
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
		BumbleBeeDeviceProxy::BumbleBeeDeviceProxy(DigiclopsImageResolution res):
            bRunning(false),
			zlimit(5)
{
	printf("bumblebee Ctor\n");
	switch (res)
		{
		case DIGICLOPS_160x120:
			m_rows = 120;
			m_cols = 160;
			break;

		case  DIGICLOPS_320x240: 
			m_rows = 240;
			m_cols = 320;
			break;

		case DIGICLOPS_640x480:
			m_rows = 480;
			m_cols = 640;
			break;
		}

	//
	_init_context();
}
//---------------------------------------------------------------------------
BumbleBeeDeviceProxy::~BumbleBeeDeviceProxy()
{
    printf("bumblebee Dtor\n");
	_destroy_context();
}
//---------------------------------------------------------------------------
void BumbleBeeDeviceProxy::_destroy_context()
	{
		printf("Stopping the camera\n");
		
		DigiclopsError      de;
		TriclopsError       te; 


		de = digiclopsStop(digiclops); 
	handleDigiclopsError( "digiclopsStop()", de );
	   
		de = digiclopsDestroyContext(digiclops );
	handleDigiclopsError( "digiclopsDestroyContext()", de );

  		te = triclopsDestroyContext(triclops);
	handleTriclopsError("triclopsDestroyContext",te);
   
    printf("CAMERA STOPPED\n");
    bRunning = false;
	}
//---------------------------------------------------------------------------
void BumbleBeeDeviceProxy::start_grabbing()
	{
 /////start_grabbing
 //  // start grabbing
	DigiclopsError      de2;
   de2 = digiclopsStart( digiclops );
   handleDigiclopsError( "digiclopsStart()", de2 );		
   bRunning=true;
	}
//---------------------------------------------------------------------------
///
void BumbleBeeDeviceProxy::stop_grabbing()
	{
	if(bRunning)
		{
		DigiclopsError      de;
		de = digiclopsStop(digiclops); 
		handleDigiclopsError( "digiclopsStop()", de );
		bRunning=false;
		}
	}
//---------------------------------------------------------------------------
void BumbleBeeDeviceProxy::_init_context()
{
	DigiclopsError      de;
	//printf("Creating Context\n");

   // open the Digiclops
   de = digiclopsCreateContext( &digiclops );
   handleDigiclopsError( "digiclopsCreateContext()", de );
   //digiclopsInitializeFromSerialNumber
   de = digiclopsInitialize( digiclops, 0 );
   handleDigiclopsError( "digiclopsInitialize()", de );

   // get the camera module configuration
   de = digiclopsGetTriclopsContextFromCamera( digiclops, &triclops );
   handleDigiclopsError( "digiclopsGetTriclopsContextFromCamera()", de );

}

//---------------------------------------------------------------------------
void BumbleBeeDeviceProxy::init_all()
	{
	DigiclopsError      de;
      // set the digiclops to deliver the stereo image and right (color) image
   de = digiclopsSetImageTypes( digiclops,  ALL_IMAGES );
   handleDigiclopsError( "digiclopsSetImageTypes()", de );

		_digiclops_private_init();
	}
//-------------------------------------------------------------------------
   void BumbleBeeDeviceProxy::init_right_and_stereo()
	   {	
	DigiclopsError      de;

   // set the digiclops to deliver the stereo image and right (color) image
   de = digiclopsSetImageTypes( digiclops, STEREO_IMAGE | RIGHT_IMAGE );
   handleDigiclopsError( "digiclopsSetImageTypes()", de );

   //allocate buffer for depth map
   raw_depth_image.reset(new core::single_t [m_rows*m_cols*3]);

	_digiclops_private_init();

 }
   //-------------------------------------------------------------------------
 void BumbleBeeDeviceProxy::_digiclops_private_init()
	   {
	printf("Setting Resolution and Color Processing\n");
	DigiclopsError      de;

		//     // set the Digiclops resolution
	de = digiclopsSetImageResolution( digiclops,  DIGICLOPS_FULL);
	handleDigiclopsError( "digiclopsSetImageResolution()", de );

		//Framerate
	de= digiclopsSetFrameRate(digiclops, DIGICLOPS_FRAMERATE_050);
		handleDigiclopsError( "digiclopsSetFrameRate()", de );

	de = digiclopsSetColorProcessing(digiclops,  DIGICLOPS_EDGE_SENSING );
		handleDigiclopsError( "digiclopsSetColorProcessing()", de );

	m_settings = new Settings(digiclops);
		m_settings->loadFromFile("config/bumblebee.ini");

	   }
//---------------------------------------------------------------------------
void BumbleBeeDeviceProxy::init_stereo_processing()
	   {
	TriclopsError  te;
	
	te = triclopsGetDefaultContextFromFile(&triclops,"config/bumblebee6213002.cal" );
	handleTriclopsError( "triclopsGetDefaultContextFromFile()", te );

	printf ("Triclops Configuration Loaded.\n");

    te = triclopsGetFocalLength(triclops, &focal);
    handleTriclopsError( "triclopsGetFocalLength()", te );
    
    te = triclopsGetImageCenter(triclops, &centerRow,&centerCol);
    handleTriclopsError( "triclopsGetImageCenter()", te );
        
    te = triclopsGetSerialNumber(triclops, &serialNum);
    handleTriclopsError( "triclopsGetSerialNumber()", te );
    
    te = triclopsGetBaseline(triclops, &base);
    handleTriclopsError( "triclopsGetBaseline()", te );
    
    printf("Serial Number: %d\n",serialNum);
    printf("Focal Lenght: %f\n" ,focal);
    printf("Center: %f %f\n", centerRow, centerCol);
    printf("Baseline: %f\n" , base);    
    
    //better choose another place ..
    bRunning = true;
}

//-------------------------------------------------------------------------
void BumbleBeeDeviceProxy::set_gain(int value)
{
   //GAIN
   DigiclopsError      de;
   de = digiclopsSetCameraProperty(digiclops, DIGICLOPS_GAIN, value, 0, false );
   handleDigiclopsError( "SetCameraProp_Gain()", de );  
}
//-------------------------------------------------------------------------
void BumbleBeeDeviceProxy::set_disparity_range(int min_disparity, int max_disparity)
	{
	TriclopsError te = triclopsSetDisparity( triclops, min_disparity, max_disparity );
	handleTriclopsError( "triclopsSetDisparity()", te );
	}
//-------------------------------------------------------------------------
    ///White Balance
void BumbleBeeDeviceProxy::set_white(int red_val, int blu_val)
{
   //White Balance
   DigiclopsError de;
   de = digiclopsSetCameraProperty(digiclops,
                                    DIGICLOPS_HARDWARE_WHITEBALANCE, 
                                    red_val, blu_val, false );
   handleDigiclopsError( "SetCameraProp_WhiteBalance()", de );
}
//-------------------------------------------------------------------------
void BumbleBeeDeviceProxy::set_frame_rate(DigiclopsFrameRate fr_rate)
	{   
	DigiclopsError de;
	de= digiclopsSetFrameRate(digiclops, fr_rate);
	handleDigiclopsError( "digiclopsSetFrameRate()", de );
	}
//-------------------------------------------------------------------------
void BumbleBeeDeviceProxy::setZlimit(double val)
	{   
		zlimit = val;
	}
//-------------------------------------------------------------------------
double BumbleBeeDeviceProxy::getZlimit() const
	{   
		return zlimit;
	}
//-------------------------------------------------------------------------
void BumbleBeeDeviceProxy::grab_color_left()
{
//if(bRunning){
//	DigiclopsError de;
//
//   // grab the image set
//   de = digiclopsGrabImage( digiclops );
//   handleDigiclopsError( "digiclopsGrabImage()", de );
//
//   de = digiclopsExtractTriclopsInput( digiclops, LEFT_IMAGE, &colorData );
//   handleDigiclopsError( "digiclopsExtractTriclopsInput()", de );
//
//	TriclopsError       te;
//   te = triclopsRectifyColorImage( 
//      triclops, TriCam_REFERENCE, &colorData, &colorImage );
//   handleTriclopsError( "triclopsRectifyColorImage()", te );
//	}

}
//-------------------------------------------------------------------------
void BumbleBeeDeviceProxy::grab_color_right()
{
if(!bRunning)
	{
		start_grabbing();
	}

	DigiclopsError de;

   // grab the image set
   de = digiclopsGrabImage( digiclops );
   handleDigiclopsError( "digiclopsGrabImage()", de );

   de = digiclopsExtractTriclopsInput( digiclops, RIGHT_IMAGE, &colorData );
   handleDigiclopsError( "digiclopsExtractTriclopsInput()", de );

	TriclopsError       te;
   te = triclopsRectifyColorImage( 
      triclops, TriCam_REFERENCE, &colorData, &colorImage );
   handleTriclopsError( "triclopsRectifyColorImage()", te );

}
//-------------------------------------------------------------------------
void BumbleBeeDeviceProxy::grab3d_color_left()
{
//if(bRunning){
//	DigiclopsError de;
//	TriclopsError  te;
//
//   // grab the image set
//   de = digiclopsGrabImage( digiclops );
//   handleDigiclopsError( "digiclopsGrabImage()", de );
//
//   de = digiclopsExtractTriclopsInput( digiclops, LEFT_IMAGE, &colorData );
//   handleDigiclopsError( "digiclopsExtractTriclopsInput()", de );
//
//   te = triclopsRectifyColorImage( 
//      triclops, TriCam_REFERENCE, &colorData, &colorImage );
//   handleTriclopsError( "triclopsRectifyColorImage()", te );
//
//	_do_triclops_process();
//	}
//else
//	{
//		printf("Pgrey is not running..!!\n");
//	}
}
//-------------------------------------------------------------------------
void BumbleBeeDeviceProxy::grab3d_color_right()
{
if(bRunning){
	DigiclopsError de;
	TriclopsError  te;

   // grab the image set
   de = digiclopsGrabImage( digiclops );
   handleDigiclopsError( "digiclopsGrabImage()", de );

   de = digiclopsExtractTriclopsInput( digiclops, RIGHT_IMAGE, &colorData );
   handleDigiclopsError( "digiclopsExtractTriclopsInput()", de );

   te = triclopsRectifyColorImage( 
      triclops, TriCam_REFERENCE, &colorData, &colorImage );
   handleTriclopsError( "triclopsRectifyColorImage()", te );

	_do_triclops_process();
	}
else
	{
		printf("Pgrey is not running..!!\n");
	}
}
//---------------------------------------------------------------------------
void BumbleBeeDeviceProxy::_do_triclops_process()
{

	TriclopsError       te;
	DigiclopsError		de;

   de = digiclopsExtractTriclopsInput( digiclops, STEREO_IMAGE, &stereoData );
   handleDigiclopsError( "digiclopsExtractTriclopsInput()", de );

   // preprocessing the images
   te = triclopsPreprocess( triclops, &stereoData );
	handleTriclopsError( "triclopsPreprocess()", te );
   // stereo processing
   te = triclopsStereo( triclops ) ;
	handleTriclopsError( "triclopsStereo()", te );
   
   // retrieve the interpolated depth image from the context
   te = triclopsGetImage16( 
      triclops, TriImg16_DISPARITY, TriCam_REFERENCE, &depthImage16 );
   handleTriclopsError( "triclopsGetImage16()", te );
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
	////TriclopsError te;
	int planeinc  = depthImage16.ncols *depthImage16.nrows;
    int planeinc_2=planeinc*2;

	int pixelinc    = depthImage16.rowinc/2;
	unsigned short disparity ;
	int		       i, j, k;
	float	       x, y, z; 
	unsigned short*    row ;

    //
    //std::memset(raw_depth_image.get(), 0, size);

	////Outer loop: rows. Extract rows.
	for ( i = 0, k = 0; i < depthImage16.nrows; i++ )
    {
		row     = depthImage16.data + i * pixelinc;
			//Inner Loop: cols. Extract disparities.
		for ( j = 0; j < depthImage16.ncols; j++, k++ )
        {
			disparity = row[j];
		// filter invalid points
		    if ( disparity < 0xFF00 )
            {
			    // convert the 16 bit disparity value to floating point x,y,z
			    triclopsRCD16ToXYZ
				    ( triclops, i, j, disparity, &x, &y, &z ) ;
			    //handleTriclopsError("triclopsRCD16ToXYZ",te);

			    //npoints++;
			    if(z < 15)
                {
                    //printf("Z: %f\n", z);
                    raw_depth_image[k]              = x;
                    raw_depth_image[k+planeinc]     = y;
                    raw_depth_image[k+planeinc_2]   = z;
                    //printf("Z: %f\n", raw_depth_image[k+planeinc_2]);
                }
                else
                {
                    raw_depth_image[k]              = 0;
                    raw_depth_image[k+planeinc]     = 0;
                    raw_depth_image[k+planeinc_2]   = 0;
                }
            }
        }		
    }

}
//---------------------------------------------------------------------------
void BumbleBeeDeviceProxy::handleDigiclopsError(char* function,DigiclopsError error)
{
   if( error != DIGICLOPS_OK ) 
   { 
      printf( 
	 "ERROR: %s reported %s.\n", 
	 function, 
	 digiclopsErrorToString( error ) ); 
     //mexErrMsgTxt("DIGICLOPS Error....");
   } 
}
//---------------------------------------------------------------------------
void BumbleBeeDeviceProxy::handleTriclopsError(char* function, TriclopsError error)
{
   if( error != TriclopsErrorOk ) 
   { 
      printf( 
	 "ERROR: %s reported %s.\n", 
	 function, 
	 triclopsErrorToString( error ) ); 
      //mexErrMsgTxt("TRICLOPS Error...."); 
   } 
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
	}//alcor 
}//device
//---------------------------------------------------------------------------