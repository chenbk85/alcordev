#include "alcor/core/opencv_lightweight_grabber_t.h"
#include <iostream>
using std::cout;
using std::endl;
//--------------------------------------------------------++
namespace all { namespace core {
//--------------------------------------------------------++

  opencv_lightweight_grabber_t::opencv_lightweight_grabber_t(all::core::parallel_worker_sync_t* i_sync):
      running_(true),
      sync_(i_sync)
  {
  }
//--------------------------------------------------------++
  opencv_lightweight_grabber_t::~opencv_lightweight_grabber_t()
  {
    // Release the capture object, the pointer should be set null
    if (0 != m_capture_) 
      cvReleaseCapture((CvCapture**)(&m_capture_));
  }
//--------------------------------------------------------++
  void opencv_lightweight_grabber_t::grab_loop()
  {
    //gotta open capture here!
    internal_open_();
    //
	  while (running_) 
    {

		  //wait that all grabbers are ready to grab and that main asks to grab
		  sync_->grab_barrier.wait();
      
        //grab it!
        cvGrabFrame(m_capture_); 

		  //wait for all grabbers query
		  sync_->query_barrier.wait();

        //import it!!
        image_ptr_ = cvRetrieveFrame( m_capture_);

		  //wait on this so main knows when all data is ready
		  sync_->retrieve_barrier.wait();
	  }
  }
//--------------------------------------------------------++
  bool opencv_lightweight_grabber_t::internal_open_()
  {
    
    // Release the capture object, the pointer should be set null
    if (0 != m_capture_) 
      cvReleaseCapture((CvCapture**)(&m_capture_));

    // Try to open a capture object for the first camera
    m_capture_ = cvCreateCameraCapture(CV_CAP_ANY);

    if (0 == m_capture_) 
    {
      printf("Unable to open camera  for capturing!\n") ;
        return false;
    }
    if (b_use_dialogs_) 
    {
	  cvSetCaptureProperty(m_capture_, CV_CAP_PROP_DIALOG_SOURCE, 0);
	  cvSetCaptureProperty(m_capture_, CV_CAP_PROP_DIALOG_FORMAT, 0);
    }

	  ///TEsting actual image size
	  IplImage* _iplFrame = 0;
    //// Grab and retrieve a frame, OpenCV owns the returned image
    _iplFrame = cvQueryFrame((CvCapture*)m_capture_);

    //
    if (0 == _iplFrame) {
	  printf("It seeems we are not able to capture images\n");
         return false;
    }

    m_w_           = _iplFrame->width;
    m_h_           = _iplFrame->height;
    m_data_order_  = _iplFrame->dataOrder;
    m_data_origin_ = _iplFrame->origin;
    m_byte_size_   = _iplFrame->imageSize;
	  m_ch_          = _iplFrame->nChannels;

    ////// Ignore capture properties 
    cout << "Capture properties "
		  << m_w_ 
		  << "x" 
		  << m_h_
		  << "x" 
		  << m_ch_
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

    printf("channelSeq: %c\n", _iplFrame->channelSeq[0]);

    cout << "-->OpenCVGrabber opened\n";
    // Success!
    return true;
  }
  //--------------------------------------------------------++
  bool opencv_lightweight_grabber_t::open(bool show_dialogs )
  {
    b_use_dialogs_ = show_dialogs;
    return true;
  }
  //--------------------------------------------------------++
  ///store the IplImage as is without modifications.
  void   opencv_lightweight_grabber_t::grab_ipl_image()
  {
    cvGrabFrame(m_capture_); 
  }
  //--------------------------------------------------------++
  ///get Ipl Image from previously stored buffer.
  void opencv_lightweight_grabber_t::retrieve_ipl_image()
  {
    image_ptr_ = cvRetrieveFrame( m_capture_);
  }
//--------------------------------------------------------++
  IplImage* opencv_lightweight_grabber_t::get_iplimage() const
  {
    return image_ptr_;
  }
//--------------------------------------------------------++
//--------------------------------------------------------++
}}//all::core
//--------------------------------------------------------++