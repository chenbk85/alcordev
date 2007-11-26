#include "stereo_process_t.hpp"
#include "alcor/core/core.h"

namespace all {
	namespace sense {

stereo_process_t::stereo_process_t() {

}

bool stereo_process_t::init(const char* config_file) {
	
	all::core::config_parser_t config;
	config.load(all::core::ini, config_file);

	std::string calib_file = config.get<std::string>("config.calib_file");

	_calib_filter.LoadCameraParams(calib_file.c_str());

	_image_size.col_ = _calib_filter.GetCameraParams(0)->imgSize[0];
	_image_size.row_ = _calib_filter.GetCameraParams(0)->imgSize[1];
	_image_size.depth_ = 3;

	int max_disp = config.get<int>("config.max_disparity", 10);
    
	int corr_win_w = config.get<int>("config.corr_win_w", 9);
	int corr_win_h = config.get<int>("config.corr_win_h", 9);

	float disp_th = config.get<float>("config.disparity_threshold",1);
	bool multi_scale = config.get<bool>("config.multi_scale", 0);

	int horopter = config.get<int>("config.horopter", 0);

	_estereo_image = new StereoImage();

	_estereo_matcher.setInputImageSize(_image_size.col_, _image_size.row_); 
	_estereo_matcher.setNumDepth(max_disp);
    _estereo_matcher.setCorrelationWindowSize(corr_win_w, corr_win_h);
    _estereo_matcher.setAcceptDisparityThreshold(disp_th);
	_estereo_matcher.setHoropter(horopter);
	_estereo_matcher.setScale(0);
	if (multi_scale)
		_estereo_matcher.setScaleProcessing(StereoMatching::MULTISCALE);
	else
	    _estereo_matcher.setScaleProcessing(StereoMatching::MONOSCALE);
	
	_estereo_matcher.initializeContext();

	_gray_left = cvCreateImage(cvSize(_image_size.col_, _image_size.row_), IPL_DEPTH_8U, 1);
	_gray_right = cvCreateImage(cvSize(_image_size.col_, _image_size.row_), IPL_DEPTH_8U, 1);

	_depth_image = cvCreateImage(cvSize(_image_size.col_, _image_size.row_), IPL_DEPTH_32F, 3);
	_depth_image->origin = IPL_ORIGIN_BL;

	float P[9];
	memcpy(P, _calib_filter.GetCameraParams(1)->matrix, sizeof(float) * 9);

	_focal_length[0] = P[0];
	_focal_length[1] = P[4];
	_u0 = P[2];
	_v0 = P[5];

	float trans_vec[3];
	memcpy(trans_vec, _calib_filter.GetStereoParams()->transVector, sizeof(float) *3);

	_baseline = sqrt(pow(trans_vec[0],2) + pow(trans_vec[1],2) + pow(trans_vec[2],2));

	_disp_image = cvCreateImage(cvSize(_image_size.col_, _image_size.row_), IPL_DEPTH_8U, 1);
	_disp_image->origin = IPL_ORIGIN_BL;

	printf("\nStereo camera parameters:\n");
	printf("baseline: %f\n", _baseline);
	printf("focal length: %f\n", _focal_length[0]);
	printf("image center: %f %f\n\n", _u0, _v0);

	return true;

}

void stereo_process_t::stop() {
	_estereo_matcher.destroyContext();
	cvReleaseImage(&_gray_left);
	cvReleaseImage(&_gray_right);
}

void stereo_process_t::rectify(IplImage** stereo_pair, IplImage** rect_pair) {
	_calib_filter.Rectify(stereo_pair, rect_pair);
}

IplImage* stereo_process_t::do_stereo(IplImage* left_image, IplImage* right_image) {
	
	//rectify stereo pair (side effect on input images)
	IplImage* stereo_pair[] = {left_image, right_image};
	rectify(stereo_pair, stereo_pair);

	//get BW image as required by estereo
	if (left_image->nChannels > 1) {
		cvCvtColor(left_image, _gray_left, CV_BGR2GRAY );
		cvCvtColor(right_image, _gray_right, CV_BGR2GRAY );
	}else {
		_gray_left = left_image;
		_gray_right = right_image;
	}

	//stereo matching
	_estereo_matcher.doStereo(_estereo_image, (unsigned char*) _gray_left->imageData, (unsigned char*) _gray_right->imageData);

	do_3d_();

	return _depth_image;

}

void stereo_process_t::do_3d_() {


	float* disp_ptr = _estereo_image->imDepth32f;
	float* depth_ptr = (float*) _depth_image->imageData;

	float b_over_d;
	for (int row = 0; row < _image_size.row_; row++) {
		for (int col = 0; col < _image_size.col_; col++) {
			if (*disp_ptr > 0) {
				b_over_d = _baseline / *disp_ptr;
				//X
				*depth_ptr = (col - _u0) * b_over_d;
				depth_ptr++;
				//Y
				*depth_ptr = (row - _v0) * b_over_d;
				depth_ptr++;
				//Z
				*depth_ptr = _focal_length[0] * b_over_d;
				depth_ptr++;
			}
			else {
				*depth_ptr = 0;
				depth_ptr++;
				*depth_ptr = 0;
				depth_ptr++;
				*depth_ptr = 0;
				depth_ptr++;
			}

			disp_ptr++;
		}
	}

}

all::math::size3d_t stereo_process_t::get_dims() {
	return _image_size;
}

IplImage* stereo_process_t::get_disparity() {

	memcpy(_disp_image->imageData, _estereo_image->imDepth8u, _image_size.col_ * _image_size.row_);

	return _disp_image;
}

}}