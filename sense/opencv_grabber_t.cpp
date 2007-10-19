//-------------------------------------------------------------------------++
#include <iostream>
#include <cstdio>
using std::cout;
using std::endl;
//-------------------------------------------------------------------------++
#include "alcor/sense/opencv_grabber_t.h"
#include "alcor/core/image_utils.h"
//-------------------------------------------------------------------------++
#include "alcor/core/iniWrapper.h"//obsolete!
#include "alcor/core/config_parser_t.hpp"
//-------------------------------------------------------------------------++
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
//-------------------------------------------------------------------------++
///
all::sense::opencv_grabber_t::opencv_grabber_t()
		:	m_w(0)
		,	m_h(0)
		,	m_ch(0)
    , bwantsinterleaved(false)
    , bwantsgray(false)
		,	m_byte_size(0)
		,	m_cam_id(0)
		,	m_capture(0) 
		,	snap(0)
    ,m_ipl_image(0)
    ,m_ipl_image_gray(0)
		{    
		cout<< "Welcome to opencv_grabber" << endl <<"Using OpenCV version "
		<< CV_VERSION 
		<< CV_MAJOR_VERSION << "."
		<< CV_MINOR_VERSION << "."
		<< CV_SUBMINOR_VERSION
		<< endl; 


    //default
    get_image_buffer = boost::bind(&all::sense::opencv_grabber_t::get_image_buffer_original_,
                                 this,
                                 _1);

		};
//-------------------------------------------------------------------------++
all::sense::opencv_grabber_t::~opencv_grabber_t()
	{
	close();
	}
//-------------------------------------------------------------------------++
///
bool all::sense::opencv_grabber_t::open(const std::string& inifile, bool show_dialog)
{
  iniWrapper ini;
  if (!ini.Load(inifile.c_str()) )return false;
  
  //busedialogs= (ini.GetBool("config:usedialogs", 1) == 1);
  //
  bwantsgray = (ini.GetBool("config:graylevel",0) == 1);
  //
  bwantsinterleaved = (ini.GetBool("config:interleaved",1)== 1);
  //
  m_cam_id = ini.GetInt("config:camid", CV_CAP_ANY);

  if(bwantsgray)
  {
    get_image_buffer = boost::bind(&all::sense::opencv_grabber_t::get_image_buffer_gray_,
                                   this,
                                   _1);
  }

  int grabmode = ini.GetInt("config:camera",1);

  if(grabmode)
  {
    ///camera
    return open_(core::open_camera, m_cam_id, show_dialog);
  }
  else
  {
    //wants to open a video file.
    std::string vidname=ini.GetString("config:videofile");
  ///filevideo
    return open_(core::open_video, vidname);
  }
}
//-------------------------------------------------------------------------++
  ///
  bool all::sense::opencv_grabber_t::open(  const std::string& inifile
                                          , const std::string& section
                                          , bool show_dialog)
  {
    core::config_parser_t ini;
    if (!ini.load(core::tags::ini,inifile)) return false;
  
  
    //
    bwantsgray        = ini.get<bool>( (section + ":graylevel"), false) ;
    //
    bwantsinterleaved = ini.get<bool>( (section + ":interleaved"), true);
    //
    m_cam_id = ini.get<int>( (section + ":camid") , CV_CAP_ANY);

  if(bwantsgray)
  {
    get_image_buffer = boost::bind(&all::sense::opencv_grabber_t::get_image_buffer_gray_,
                                   this,
                                   _1);
  }

  int grabmode = ini.get<int>( (section + ":camera") ,1);

  if(grabmode)
  {
    ///camera
    return open_(core::open_camera, CV_CAP_ANY, show_dialog);
  }
  else
  {
    //wants to open a video file.
    std::string vidname=ini.get<std::string>( (section + ":videofile") );
  ///filevideo
    return open_(core::open_video, vidname);
  }

  }
//-------------------------------------------------------------------------++
///
bool all::sense::opencv_grabber_t::open_(core::camera_mode_t, int in_cam, bool show_dialog) 
	{
  // Release any previously allocated resources, just in case
  close();
  m_cam_id = in_cam;

  // Try to open a capture object for the first camera
  m_capture = cvCreateCameraCapture(CV_CAP_ANY);
  if (0 == m_capture) 
  {
      cout <<"Unable to open camera " << m_cam_id << " for capture!\n" ;
      return false;
  }
  if (show_dialog) 
  {
	cvSetCaptureProperty(m_capture, CV_CAP_PROP_DIALOG_SOURCE, 0);
	cvSetCaptureProperty(m_capture, CV_CAP_PROP_DIALOG_FORMAT, 0);
  }

  return internal_open_();

}
//-------------------------------------------------------------------------++
bool all::sense::opencv_grabber_t::open_(core::video_mode_t, const std::string& in_file)
	{
    // Try to open a capture object for the file
    m_capture = cvCaptureFromAVI(in_file.c_str());
    if (0 == m_capture) 
    {
        std::printf("Unable to open file '%s' for capture!\n",
               in_file.c_str());
        return false;
    }
		return internal_open_();
	}
//-------------------------------------------------------------------------++
bool all::sense::opencv_grabber_t::internal_open_()
	{
	///TEsting actual image size
	IplImage* _iplFrame = 0;
  //// Grab and retrieve a frame, OpenCV owns the returned image
  _iplFrame = cvQueryFrame((CvCapture*)m_capture);

  //
  if (0 == _iplFrame) {
	cout << "It seeems we are not able to capture images" << endl;
       return false;
  }

  m_w           = _iplFrame->width;
  m_h           = _iplFrame->height;
  m_data_order  = _iplFrame->dataOrder;
  m_data_origin = _iplFrame->origin;

  if(bwantsgray)
  {
    m_byte_size = m_w*m_h ;
    m_ch    = 1;

    m_ipl_image_gray  = cvCreateImage(  cvSize(m_w, m_h), 
					                        IPL_DEPTH_8U, 
					                        m_ch);
  }
  else
  {	
    m_byte_size = _iplFrame->imageSize;
	  m_ch        = _iplFrame->nChannels;
  }

  //
  image_sptr.reset(new core::uint8_t[m_w*m_h*m_ch] );

	m_ipl_image       = cvCloneImage(_iplFrame);

  ////// Ignore capture properties 
  cout << "Capture properties "
		<< m_w 
		<< "x" 
		<< m_h
		<< "x" 
		<< m_ch
		<< " pixels "  
		<< endl << endl

		<< "Data Ordering: " << _iplFrame->dataOrder << endl		
    << "       0 - interleaved color channels, " << endl
		<< "       1 - separate color channels." << endl << endl

		<< "Origin: "  << _iplFrame->origin << endl
		<< "       0 - top-left origin," << endl
    << "       1 - bottom-left origin (Windows bitmaps style)" << endl << endl

    << "Original Image Size: " << _iplFrame->imageSize << endl
		<< endl;

  printf("channelSeq: %c\n", _iplFrame->channelSeq[0]);

    cout << "-->OpenCVGrabber opened\n";
    // Success!
    return true;
	}
//-------------------------------------------------------------------------++
///
bool all::sense::opencv_grabber_t::close() 
{

    if(m_ipl_image)       cvReleaseImage(&m_ipl_image);
    if(m_ipl_image_gray)  cvReleaseImage(&m_ipl_image_gray);

    // Release the capture object, the pointer should be set null
    if (0 != m_capture) 
      cvReleaseCapture((CvCapture**)(&m_capture));

    //uhm...why?...
    if (0 != m_capture) 
    {
        m_capture = 0; 
        return false;
    } 
    else 
      return true;
}
//-------------------------------------------------------------------------++
///
bool all::sense::opencv_grabber_t::
get_image_buffer_original_(core::uint8_sarr& user_buffer)
{
    // Must have a capture object
    if (0 == m_capture) {
		cout << "!**Not a valid Capture Object!" << endl;
        return false;
    }

	IplImage* iplFrame = 0;

  //// Grab and retrieve a frame, OpenCV owns the returned image
  iplFrame = cvQueryFrame((CvCapture*)m_capture);

  if (0 == iplFrame) {
       return false;
  }

  //IplImage format is a mess ...
  //BGR and bottom-left alignement
  //we want topleft .. this is a weird code .. but .. works.
  if(m_data_origin)
  {    
    //bottom-left
    //printf("PRE_ cvConvertImage(m_ipl_image, m_ipl_image, CV_CVTIMG_FLIP);\n");
    cvConvertImage(iplFrame, m_ipl_image, CV_CVTIMG_FLIP);
    //printf("PRE_ cvConvertImage(m_ipl_image, m_ipl_image, CV_CVTIMG_FLIP);\n");
  }
  else
  {
    //top-left
      m_ipl_image = cvCloneImage(iplFrame);
  }

    //if( std::strcmp(&iplFrame->channelSeq[0],"B") == 0 )

  //it seems that everytime the image grabbed is BGR.
  //nocheck here .. also because channelSeq seems unuseful.
  cvConvertImage(m_ipl_image, m_ipl_image, CV_CVTIMG_SWAP_RB);

  //user-buffer was not initialized.
  //we do it here
  if(!user_buffer) 
    user_buffer.reset(new core::uint8_t[m_byte_size]);

  //just a raw memcpy here.
  memcpy(user_buffer.get()
  ,(unsigned char*)m_ipl_image->imageData
   ,  m_byte_size);

  //depending on the inifile we convert things to the desired format.
  if (iplimage_is_interleaved() )
  { 
    if(!bwantsinterleaved)
      core::change_ordering::to_planar(user_buffer, m_h, m_w, m_ch);
  }
  else
  {
    if(bwantsinterleaved)
      core::change_ordering::to_interleaved(user_buffer, m_h, m_w, m_ch);
  }

  return true;
}
//-------------------------------------------------------------------------++
///
bool all::sense::opencv_grabber_t::
  get_image_buffer_gray_(core::uint8_sarr& user_buffer)
{
  
    // Must have a capture object
    if (0 == m_capture) {
		cout << "!**Not a valid Capture Object!" << endl;
        return false;
    }

	IplImage* iplFrame = 0;

  //// Grab and retrieve a frame, OpenCV owns the returned image
  iplFrame = cvQueryFrame((CvCapture*)m_capture);

  if (0 == iplFrame) {
       return false;
  }

  if(m_data_origin)
  {    
    //printf("PRE_ cvConvertImage(m_ipl_image, m_ipl_image, CV_CVTIMG_FLIP);\n");
    cvConvertImage(iplFrame, m_ipl_image, CV_CVTIMG_FLIP);
    //printf("PRE_ cvConvertImage(m_ipl_image, m_ipl_image, CV_CVTIMG_FLIP);\n");
  }
  else
  {
      m_ipl_image = cvCloneImage(iplFrame);
  }

  cvCvtColor(m_ipl_image, m_ipl_image_gray, CV_BGR2GRAY); 

  //printf("POST_cvCvtColor(iplFrame,m_ipl_image, CV_BGR2GRAY);\n");

  if(!user_buffer) 
    user_buffer.reset(new core::uint8_t[m_byte_size]);

    //printf("PRE GRAY memcpy(user_buffer.get()...);\n");
  memcpy(user_buffer.get()
  ,(unsigned char*)m_ipl_image_gray->imageData
   ,  m_byte_size);

    //printf("POST memcpy(user_buffer.get()...);\n");
  return true;
}
////-------------------------------------------------------------------------++
//-------------------------------------------------------------------------++
int all::sense::opencv_grabber_t::grab_ipl_image() 
	{
   	return cvGrabFrame(m_capture); 
  }
//-------------------------------------------------------------------------++
IplImage* all::sense::opencv_grabber_t::retrieve_ipl_image() const
	{

  return cvRetrieveFrame( m_capture);
}
//-------------------------------------------------------------------------++
///
void all::sense::opencv_grabber_t::log_to_disk(const std::string& avifile)
{

}
//-------------------------------------------------------------------------++