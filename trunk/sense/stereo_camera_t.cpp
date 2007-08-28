#include "stereo_camera_t.hpp"

namespace all {
	namespace sense {

		
stereo_camera_t::stereo_camera_t() {
	
	m_left_ipl_image = 0;
	m_right_ipl_image = 0;
	m_gray_left = 0;
	m_gray_right = 0;

	m_max_disparity = 30;
}

bool stereo_camera_t::open() {
	
	m_left_cam = cvCreateCameraCapture(-1);
	m_right_cam = cvCreateCameraCapture(-1);

	if (0 == m_left_cam) {
		printf("Unable to open left camera for capture!\n") ;
		return false;
	}
	else if (0 == m_right_cam) {
		printf("Unable to open right camera for capture!\n") ;
		return false;
	}

	m_left_ipl_image = cvQueryFrame(m_left_cam);
	if (m_left_ipl_image == 0) {
		printf("Unable to grab image...\n");
		return false;
	}

	m_right_ipl_image = cvQueryFrame(m_left_cam);
	
	if (m_right_ipl_image == 0) {
		printf("Unable to grab image...\n");
		return false;
	}

	m_width = m_left_ipl_image->width;
	m_height = m_left_ipl_image->height;
	m_channel = m_left_ipl_image->nChannels;
	m_data_origin = m_left_ipl_image->origin;
	m_data_order = m_left_ipl_image->dataOrder;

	if (m_channel > 1) {
		m_gray_left = cvCreateImage(cvSize( m_width, m_height), IPL_DEPTH_8U, 1 );
		m_gray_right = cvCreateImage(cvSize( m_width, m_height), IPL_DEPTH_8U, 1 );
		m_gray_left->origin = m_gray_right->origin = m_data_origin;
	}

	m_disp_image = cvCreateImage(cvSize(m_width, m_height), IPL_DEPTH_8U, 1);
	
	m_left_disp = cvCreateImage(cvSize(m_width, m_height),IPL_DEPTH_64F,1);
	m_right_disp = cvCreateImage(cvSize(m_width, m_height),IPL_DEPTH_64F,1);
	m_left_occl = cvCreateImage(cvSize(m_width, m_height),IPL_DEPTH_64F,1);
	m_right_occl = cvCreateImage(cvSize(m_width, m_height),IPL_DEPTH_64F,1);

	m_left_img_ad = new OpenCVImageAdapter(m_left_ipl_image);
	m_right_img_ad = new OpenCVImageAdapter(m_right_ipl_image);
	m_left_disp_ad = new OpenCVImageAdapter(m_left_disp);
	m_right_img_ad = new OpenCVImageAdapter(m_right_disp);
	m_left_occl_ad = new OpenCVImageAdapter(m_left_occl);
	m_right_occl_ad = new OpenCVImageAdapter(m_right_occl);

	double alpha = 20;
	bt_matcher.setParams(1,&alpha);

	stereo_manager.setLocalImageMatcher(bt_matcher); 
    stereo_manager.setGlobalMatcher(stereo_diffuse_matcher); 

	cvNamedWindow("left camera", 1);
    cvNamedWindow("right camera", 1);

	return true;
}

void stereo_camera_t::close() {

	//cvReleaseImage(&m_gray_left);
	//cvReleaseImage(&m_gray_right);

	cvReleaseCapture(&m_left_cam);
	cvReleaseCapture(&m_right_cam);

}


bool stereo_camera_t::calibrate(int chess_rows, int chess_cols, int unit) {
	
	double etalon_params[3] = {chess_rows, chess_cols, unit};
	
	m_calib_filter.SetEtalon( CV_CALIB_ETALON_CHESSBOARD, etalon_params );
    m_calib_filter.SetCameraCount(2);
    m_calib_filter.SetFrames(30);

	
	while (!m_calib_filter.IsCalibrated()) {
		
		grab_stereo_();

		IplImage* stereo_pair[] = { m_gray_left, m_gray_right };
	
		bool found = m_calib_filter.FindEtalon(stereo_pair);

		m_calib_filter.DrawPoints(stereo_pair);

	
		//int cmd = cvvWaitKeyEx( 0, 1 );
		//if( cmd == '\x1b' )
  //          break;
		cvShowImage( "left camera", m_gray_left);
        cvShowImage( "right camera", m_gray_right);

		if (found) {
			m_calib_filter.Push();
		}
		else
			printf("unable to find etalon points\n");

		cvWaitKey(1000);


	}

	//cvDestroyWindow("left camera");
	//cvDestroyWindow("right camera");

	printf("calibration ok!\n");
	return true;

}

bool stereo_camera_t::is_calibrated() {
	return m_calib_filter.IsCalibrated();
}

void stereo_camera_t::save_calib_data() {
	m_calib_filter.SaveCameraParams("config/stereo_calib.ini");
}

void stereo_camera_t::load_calib_data() {
	m_calib_filter.LoadCameraParams("config/stereo_calib.ini");
}

void stereo_camera_t::grab_stereo_() {

	cvGrabFrame(m_left_cam);
	cvGrabFrame(m_right_cam);

	m_left_ipl_image = cvRetrieveFrame(m_left_cam);
    m_right_ipl_image = cvRetrieveFrame(m_right_cam);

	if (m_channel > 1) {
		cvCvtColor(m_left_ipl_image, m_gray_left, CV_BGR2GRAY );
		cvCvtColor(m_right_ipl_image, m_gray_right, CV_BGR2GRAY );
	}
	else {
		m_gray_left = m_left_ipl_image;
		m_gray_right = m_right_ipl_image;
	}
}

void stereo_camera_t::do_stereo_process_() {

	cvNamedWindow("Disparity map",1);

	while(1) {
	grab_stereo_();

	//IplImage* stereo_pair[] = { m_gray_left, m_gray_right };

	//m_calib_filter.Rectify(stereo_pair, stereo_pair);

	//m_disp_image->origin = m_gray_left->origin;

	//cvFindStereoCorrespondence(m_gray_left, m_gray_right,
 //                  CV_DISPARITY_BIRCHFIELD,
 //                  m_disp_image,
 //                  m_max_disparity, 25, 5, 12, 12, 25);

	//cvConvertScale(m_disp_image, m_disp_image, 255.f/m_max_disparity);

	IplImage* stereo_pair[] = {m_left_ipl_image, m_right_ipl_image};

	m_calib_filter.Rectify(stereo_pair, stereo_pair);

	stereo_manager.doStereoMatching(*m_left_img_ad, *m_right_img_ad, 0, 50, *m_left_disp_ad, *m_right_disp_ad, *m_left_occl_ad, *m_right_occl_ad);

	OvImageT<double> ovtleftDisp, ovtrightDisp;
    ovtleftDisp.copyFromAdapter(*m_left_disp_ad);
    ovtrightDisp.copyFromAdapter(*m_right_disp_ad);

    ovtleftDisp = (ovtleftDisp)/(50);
    ovtrightDisp = (ovtrightDisp+50)/(50);

    ovtleftDisp.copyToAdapter(*m_left_disp_ad);
    ovtrightDisp.copyToAdapter(*m_right_disp_ad);

    cvShowImage("Disparity map", m_disp_image);

	cvShowImage( "left camera", m_gray_left);
    cvShowImage( "right camera", m_gray_right);

	cvWaitKey(1);

	}
}

}} //namespaces