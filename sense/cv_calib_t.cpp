#include "cv_calib_t.hpp"
#include <alcor/core/core.h>

namespace all {
	namespace sense {


cv_calib_t::cv_calib_t() {
}

bool cv_calib_t::open(char* config_file) {
	
	VI = new videoInput();

	videoInput::listDevices();
	
	getchar();
	
	all::core::config_parser_t config;
	config.load(core::tags::ini, config_file);

	_n_cam = config.get<int>("config.num_cam", 1);

	_etalon_params[0] = config.get<double>("config.chess_rows", 5);
	_etalon_params[1] = config.get<double>("config.chess_cols", 7);
	_etalon_params[2] = config.get<double>("config.chess_unit", 1);

	_calib_pose = config.get<int>("config.calib_pose", 30);

	_cam_image = new IplImage*[_n_cam];
	_image_size = new all::math::size3d_t[_n_cam];
	_cam_id = new int[_n_cam];
	_window_name = new std::string[_n_cam];

	std::string ini_base;
	std::string ini_section;
	for (int i=0; i < _n_cam; i++) {
		
		//read ini section corresponding to i-th camera
		ini_base = "cam" + all::core::make_string(i);
		ini_section = ini_base + ".id";
		_cam_id[i] = config.get<int>(ini_section, i);
		ini_section = ini_base + ".width";
		_image_size[i].col_ = config.get<int>(ini_section, 320);
		ini_section = ini_base + ".height";
		_image_size[i].row_ = config.get<int>(ini_section, 240);
		_image_size[i].depth_ = 3;

		//open camera
		if (!VI->setupDevice(_cam_id[i], _image_size[i].col_, _image_size[i].row_))
			return false;

		//create camera image
		_cam_image[i] = cvCreateImage(cvSize(_image_size[i].col_, _image_size[i].row_), IPL_DEPTH_8U, _image_size[i].depth_);
		_cam_image[i]->origin = IPL_ORIGIN_BL;

		//create window for camera visualization
		_window_name[i] = ini_base;
		cvNamedWindow(_window_name[i].c_str());

	}

	_calib_filter.SetCameraCount(_n_cam);
	_calib_filter.SetEtalon(CV_CALIB_ETALON_CHESSBOARD, _etalon_params);
	_calib_filter.SetFrames(_calib_pose);

	return true;
}

void cv_calib_t::close() {

	for (int i=0; i<_n_cam; i++) {
		VI->stopDevice(_cam_id[i]);
		cvReleaseImage(&_cam_image[i]);
	}

}

void cv_calib_t::grab_images_() {

	for (int i=0; i<_n_cam; i++) {
		VI->getPixels(_cam_id[i], reinterpret_cast <unsigned char*> (_cam_image[i]->imageData), false);
	}
}


bool cv_calib_t::calibrate() {

	while (!_calib_filter.IsCalibrated()) {
		
		grab_images_();

		bool found = _calib_filter.FindEtalon(_cam_image);

		if (found) {
			_calib_filter.DrawPoints(_cam_image);
			_calib_filter.Push();
		}
		else
			printf("unable to find etalon points\n");

		for (int i=0; i<_n_cam; i++) {
			cvShowImage(_window_name[i].c_str(), _cam_image[i]);
		}

		if (cvWaitKey(500) != -1)
			return false;

	}

	//float trans_vector[3];
	//memcpy(trans_vector, _calib_filter.GetStereoParams()->transVector, sizeof(float)*3);
	//printf("Translation vector: %f %f %f\n", trans_vector[0], trans_vector[1], trans_vector[2]);
	return _calib_filter.IsCalibrated();

}

void cv_calib_t::save_calib_data(char* calib_file) {
	_calib_filter.SaveCameraParams(calib_file);
}

	
}}