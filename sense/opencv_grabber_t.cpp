
//-------------------------------------------------------------------------++
#include <iostream>
#include <cstdio>
using std::cout;
using std::endl;
//-------------------------------------------------------------------------++
#include <alcor/sense/opencv_grabber_t.h>
#include "alcor/core/image_utils.h"
//-------------------------------------------------------------------------++
//-------------------------------------------------------------------------++
///
all::sense::opencv_grabber_t::opencv_grabber_t(int cam)
		:	m_w(0)
		,	m_h(0)
		,	m_ch(0)
		,	m_byte_size(0)
		,	m_cam_id(cam)
		,	m_capture(0) 
		,	snap(0)
    ,m_ipl_image(0)
		{    
		cout<< "Welcome to opencv_grabber" << endl <<"Using OpenCV version "
		<< CV_VERSION 
		<< CV_MAJOR_VERSION << "."
		<< CV_MINOR_VERSION << "."
		<< CV_SUBMINOR_VERSION
		<< endl; 
		};
//-------------------------------------------------------------------------++
all::sense::opencv_grabber_t::~opencv_grabber_t()
	{
	close();
	}
//-------------------------------------------------------------------------++
///
bool all::sense::opencv_grabber_t::open(core::camera_mode_t, int in_cam) 
	{
  // Release any previously allocated resources, just in case
  close();

  m_cam_id = in_cam;

  // Try to open a capture object for the first camera
  m_capture = cvCreateCameraCapture(m_cam_id);
  if (0 == m_capture) {
      cout <<"Unable to open camera for capture!\n" ;
      return false;
  }

  return internal_open_();

}
//-------------------------------------------------------------------------++
bool all::sense::opencv_grabber_t::open(core::video_mode_t, const std::string& in_file)
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

	//
	m_w     = _iplFrame->width;
	m_h     = _iplFrame->height;
	m_ch    = _iplFrame->nChannels;
  m_data_order = _iplFrame->dataOrder;
  m_data_origin= _iplFrame->origin;
	m_byte_size = _iplFrame->imageSize;

  image_sptr.reset(new core::uint8_t[m_w*m_h*m_ch] );

	m_ipl_image = cvCreateImage(  cvSize(m_w, m_h), 
					                      IPL_DEPTH_8U, 
					                      m_ch);

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

    << "Image Size: " << _iplFrame->imageSize << endl
		<< endl;

    cout << "-->OpenCVGrabber opened\n";
    // Success!
    return true;
	}
//-------------------------------------------------------------------------++
///
bool all::sense::opencv_grabber_t::close() 
{

    if(m_ipl_image)  cvReleaseImage(&m_ipl_image);

    // Release the capture object, the pointer should be set null
    if (0 != m_capture) cvReleaseCapture((CvCapture**)(&m_capture));
    if (0 != m_capture) {
        m_capture = 0; return false;
    } else return true;
}
//-------------------------------------------------------------------------++
///
bool all::sense::opencv_grabber_t::get_color_buffer
(all::core::uint8_sarr user_buffer) 
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

  if(!m_data_origin)
  {
    m_ipl_image = cvCloneImage(iplFrame);
  }
  else
  {
    cvConvertImage(iplFrame, m_ipl_image, CV_CVTIMG_FLIP);
  }


  memcpy(user_buffer.get()
	,(unsigned char*)m_ipl_image->imageData
	 ,  m_byte_size);

  if (is_interleaved() )
  {
    core::change_ordering::to_planar(user_buffer, m_h, m_w, m_ch);
  }

  //// That's it
  return true;
}

IplImage* all::sense::opencv_grabber_t::get_ipl_image() 
	{
    // Must have a capture object
    if (0 == m_capture) {
		cout << "!**Not a valid Capture Object!" << endl;
        return false;
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
	//cvConvertImage(m_ipl_image, m_ipl_image, CV_CVTIMG_SWAP_RB);
  return m_ipl_image;
}
  ///

//-------------------------------------------------------------------------++