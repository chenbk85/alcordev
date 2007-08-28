#ifndef stereo_camera_t_H_INCLUDED
#define stereo_camera_t_H_INCLUDED

#include <Openvis3d.h>
#include <OpenCVImageAdapter.h>

#include <StereoMatching.h>
#include <Reconst3D.h>

#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

#include <boost/shared_ptr.hpp>
#include <alcor/sense/opencv_grabber_t.h>
#include <alcor/core/iniWrapper.h>

namespace all {
	namespace sense {

class stereo_camera_t {

public:

	stereo_camera_t();

	bool open();
	void close();

	bool calibrate(int chess_rows, int chess_cols, int unit = 1);

	void save_calib_data();
	void load_calib_data();

	bool is_calibrated();

public:

	void grab_stereo_();

	void do_stereo_process_();

	void do_3d_reconstruction_();

public:

	CvCapture*  m_left_cam;
	CvCapture*  m_right_cam;

	IplImage* m_left_ipl_image;
	IplImage* m_right_ipl_image;

	IplImage* m_gray_left;
	IplImage* m_gray_right;

	IplImage* m_disp_image;

	//openvis3d
	IplImage* m_left_disp;
	IplImage* m_right_disp;
	IplImage* m_left_occl;
	IplImage* m_right_occl;
	OpenCVImageAdapter* m_left_img_ad;
	OpenCVImageAdapter* m_right_img_ad;
	OpenCVImageAdapter* m_left_disp_ad;
	OpenCVImageAdapter* m_right_disp_ad;
	OpenCVImageAdapter* m_left_occl_ad;
	OpenCVImageAdapter* m_right_occl_ad;
	BTLocalMatcherT<double> bt_matcher;
	OvStereoDiffuseMatcherT<double> stereo_diffuse_matcher;
	OvStereoT<double> stereo_manager;

	//Estereo
	StereoMatching* estereo_matcher;
	StereoImage* estereo_image;
	Reconst3D* estereo_reconst;
	ReconstPoints3D * estereo_points;

	CvCalibFilter m_calib_filter;

	//image parameters
	int m_width;
	int m_height;
	int m_channel;
	int m_data_order;
	int m_data_origin;

	int m_max_disparity;

};

		
stereo_camera_t::stereo_camera_t() {
	
	m_left_ipl_image = 0;
	m_right_ipl_image = 0;
	m_gray_left = 0;
	m_gray_right = 0;

	m_max_disparity = 64;
}

bool stereo_camera_t::open() {
	
	m_left_cam = cvCreateCameraCapture(CV_CAP_ANY);
	m_right_cam = cvCreateCameraCapture(CV_CAP_ANY);


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

	//opencv disparity
	m_disp_image = cvCreateImage(cvSize(m_width, m_height), IPL_DEPTH_8U, 1);
	
	//openvis3d
	m_left_disp = cvCreateImage(cvSize(m_width, m_height),IPL_DEPTH_64F,1);
	m_right_disp = cvCreateImage(cvSize(m_width, m_height),IPL_DEPTH_64F,1);
	m_left_occl = cvCreateImage(cvSize(m_width, m_height),IPL_DEPTH_64F,1);
	m_right_occl = cvCreateImage(cvSize(m_width, m_height),IPL_DEPTH_64F,1);

	m_left_img_ad = new OpenCVImageAdapter(m_gray_left);
	m_right_img_ad = new OpenCVImageAdapter(m_gray_right);
	m_left_disp_ad = new OpenCVImageAdapter(m_left_disp);
	m_right_disp_ad = new OpenCVImageAdapter(m_right_disp);
	m_left_occl_ad = new OpenCVImageAdapter(m_left_occl);
	m_right_occl_ad = new OpenCVImageAdapter(m_right_occl);

	double alpha[] = {20.0};
	bt_matcher.setParams(1,alpha);

	stereo_manager.setLocalImageMatcher(bt_matcher); 
    stereo_manager.setGlobalMatcher(stereo_diffuse_matcher); 

	//Estereo
	estereo_matcher = new StereoMatching();
	estereo_image = new StereoImage();
	estereo_reconst = new Reconst3D();
	estereo_points = new ReconstPoints3D();

	estereo_matcher->setInputImageSize(m_width, m_height); 
    estereo_matcher->setNumDepth(m_max_disparity);
    estereo_matcher->setCorrelationWindowSize(9,9);
    estereo_matcher->setAcceptDisparityThreshold(1);
	estereo_matcher->setScale(0);
    estereo_matcher->setScaleProcessing(StereoMatching::MONOSCALE);
	estereo_matcher->initializeContext();
	
	estereo_reconst->setCameraParameters(605, 14, 175, 120);

	
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
	cvNamedWindow("Disparity map",1);
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


	grab_stereo_();

	
	//=============================OPENCV=======================================
	//IplImage* stereo_pair[] = { m_gray_left, m_gray_right };

	//m_calib_filter.Rectify(stereo_pair, stereo_pair);

	//m_disp_image->origin = m_gray_left->origin;

	//cvFindStereoCorrespondence(m_gray_left, m_gray_right,
 //                  CV_DISPARITY_BIRCHFIELD,
 //                  m_disp_image,
 //                  m_max_disparity, 25, 5, 12, 12, 25);

	//cvConvertScale(m_disp_image, m_disp_image, 255.f/m_max_disparity);

	//   cvShowImage("Disparity map", m_disp_image);

	//cvShowImage( "left camera", m_gray_left);
    //cvShowImage( "right camera", m_gray_right);
	//==========================END OPENCV======================================

	
	//==========================OPENVIS3D=======================================
	
	//IplImage* stereo_pair[] = {m_left_ipl_image, m_right_ipl_image};

	//m_calib_filter.Rectify(stereo_pair, stereo_pair);

	//stereo_manager.doStereoMatching(*m_left_img_ad, *m_right_img_ad, 1, 50, *m_left_disp_ad, *m_right_disp_ad, *m_left_occl_ad, *m_right_occl_ad);

	//OvImageT<double> ovtleftDisp, ovtrightDisp;
    //ovtleftDisp.copyFromAdapter(*m_left_disp_ad);
    //ovtrightDisp.copyFromAdapter(*m_right_disp_ad);

    //ovtleftDisp = (ovtleftDisp)/(50);
    //ovtrightDisp = (ovtrightDisp+50)/(50);

    //ovtleftDisp.copyToAdapter(*m_left_disp_ad);
    //ovtrightDisp.copyToAdapter(*m_right_disp_ad);

    //cvShowImage("Disparity map", m_left_disp);

	//cvShowImage( "left camera", m_left_ipl_image);
    //cvShowImage( "right camera", m_right_ipl_image);

	//=========================END OPENVIS3D==========================================

	//============================ESTEREO=============================================

	IplImage* stereo_pair[] = {m_gray_left, m_gray_right};
	m_calib_filter.Rectify(stereo_pair, stereo_pair);

	//cvSaveImage("left.bmp", m_gray_left);
	//cvSaveImage("right.bmp", m_gray_right);

	estereo_matcher->doStereo(estereo_image, (unsigned char*) m_gray_left->imageData, (unsigned char*) m_gray_right->imageData);
	estereo_reconst->doReconstruction(estereo_points, estereo_image);
	//estereo_points->save("points.txt");

	//estereo_image->generateDepth8uFromDepth32f();
	memcpy(m_disp_image->imageData, estereo_image->imDepth8u, m_width*m_height);
	m_disp_image->origin = m_gray_left->origin;

	cvConvertScale(m_disp_image, m_disp_image, 255.f/m_max_disparity);

	cvShowImage("Disparity map", m_disp_image);

	cvShowImage( "left camera", m_gray_left);
    cvShowImage( "right camera", m_gray_right);

	//===========================END ESTEREO==========================================


}

void stereo_camera_t::do_3d_reconstruction_() {
}

}}//namespaces

#endif