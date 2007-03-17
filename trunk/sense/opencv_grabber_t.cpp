//-------------------------------------------------------------------------++
#include <iostream>
#include <cstdio>
using std::cout;
using std::endl;
//-------------------------------------------------------------------------++
#include "alcor/sense/opencv_grabber_t.h"
#include "alcor/core/image_utils.h"
#include "alcor/core/iniWrapper.h"
//-------------------------------------------------------------------------++
#include <boost\bind.hpp>
//-------------------------------------------------------------------------++
///
all::sense::opencv_grabber_t::opencv_grabber_t(int cam)
		:	m_w(0)
		,	m_h(0)
		,	m_ch(0)
    , bwantsinterleaved(false)
    , bwantsgray(false)
		,	m_byte_size(0)
		,	m_cam_id(cam)
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
    get_color_buffer = boost::bind(&all::sense::opencv_grabber_t::get_color_buffer_original_,
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
bool all::sense::opencv_grabber_t::open(const std::string& inifile)
{
  iniWrapper ini;
  if (!ini.Load(inifile.c_str()) )return false;
  
  bwantsgray = (ini.GetBool("config:graylevel",0) == 1);
  bwantsinterleaved = (ini.GetBool("config:interleaved",1)== 1);
  m_cam_id = ini.GetInt("config:camid", -1);

  if(bwantsgray)
  {
    get_color_buffer = boost::bind(&all::sense::opencv_grabber_t::get_color_buffer_gray_,
                                   this,
                                   _1);
  }

  int grabmode = ini.GetInt("config:camera",1);

  if(grabmode)
  {
    ///camera
    return open_(core::open_camera, m_cam_id);
  }
  else
  {
    std::string vidname=ini.GetString("config:videofile");
  ///filevideo
    return open_(core::open_video, vidname);
  }

}
//-------------------------------------------------------------------------++
///
bool all::sense::opencv_grabber_t::open_(core::camera_mode_t, int in_cam) 
	{
  // Release any previously allocated resources, just in case
  close();
  m_cam_id = in_cam;

  // Try to open a capture object for the first camera
  m_capture = cvCreateCameraCapture(m_cam_id);
  if (0 == m_capture) 
  {
      cout <<"Unable to open camera " << m_cam_id << " for capture!\n" ;
      return false;
  }
  cvSetCaptureProperty(m_capture, CV_CAP_PROP_DIALOG_SOURCE, 0);
  cvSetCaptureProperty(m_capture, CV_CAP_PROP_DIALOG_FORMAT, 0);

  return internal_open_();

}
//-------------------------------------------------------------------------++
bool all::sense::opencv_grabber_t::open_(core::video_mode_t, const std::string& in_file)
	{
    // Try to open a capture object for the file
    m_capture = cvCaptureFromAVI(in_file.c_str());
    if (0 == m_capture) {
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
	  m_ch    = _iplFrame->nChannels;
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

    if(m_ipl_image)  cvReleaseImage(&m_ipl_image);
    if(m_ipl_image_gray)  cvReleaseImage(&m_ipl_image_gray);

    // Release the capture object, the pointer should be set null
    if (0 != m_capture) cvReleaseCapture((CvCapture**)(&m_capture));
    if (0 != m_capture) {
        m_capture = 0; return false;
    } else return true;
}
//-------------------------------------------------------------------------++
///
bool all::sense::opencv_grabber_t::
get_color_buffer_original_(core::uint8_sarr& user_buffer)
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

    //if( std::strcmp(&iplFrame->channelSeq[0],"B") == 0 )
  cvConvertImage(m_ipl_image, m_ipl_image, CV_CVTIMG_SWAP_RB);

  if(!user_buffer) 
    user_buffer.reset(new core::uint8_t[m_byte_size]);

    //printf("PRE_ RGB _ memcpy(user_buffer.get()...);\n");
  memcpy(user_buffer.get()
  ,(unsigned char*)m_ipl_image->imageData
   ,  m_byte_size);

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
  get_color_buffer_gray_(core::uint8_sarr& user_buffer)
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
//-------------------------------------------------------------------------++
IplImage* all::sense::opencv_grabber_t::get_ipl_image() 
	{
    // Must have a capture object
    if (0 == m_capture) {
		cout << "!**Not a valid Capture Object!" << endl;
        return 0;
    }

	IplImage* iplFrame = 0;

  //// Grab and retrieve a frame, OpenCV owns the returned image
  iplFrame = cvQueryFrame((CvCapture*)m_capture);

  if(0!= iplFrame)
  {
    if(!m_data_origin)
    {
      m_ipl_image = cvCloneImage(iplFrame);
    }
    else
    {
      cvConvertImage(iplFrame, m_ipl_image, CV_CVTIMG_FLIP);
    }
  }
	cvConvertImage(m_ipl_image, m_ipl_image, CV_CVTIMG_SWAP_RB);
  return m_ipl_image;
}
  ///
//-------------------------------------------------------------------------++
///
void all::sense::opencv_grabber_t::log_to_disk(const std::string& avifile)
{

}
//-------------------------------------------------------------------------++