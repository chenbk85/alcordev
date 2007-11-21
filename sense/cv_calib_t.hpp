#ifndef cv_calib_t_HPP_INCLUDED
#define cv_calib_t_HPP_INCLUDED


#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

#include "alcor.extern/videoinput/videoinput.h"
#include "alcor/core/config_parser_t.hpp"
#include "alcor/math/size3d_t.hpp"

namespace all {
	namespace sense {


class cv_calib_t {
public:
	cv_calib_t();

	bool open(char* config_file = "config/cv_calib.ini");
	void close();

	bool calibrate();
	
	void save_calib_data(char*);

	
private:

	void grab_images_();

	//opencv calibration filter
	CvCalibFilter _calib_filter;

	videoInput* VI;

	//camera grabbing stuff
	int _n_cam;
	IplImage** _cam_image;
	int* _cam_id;
	all::math::size3d_t* _image_size;

	std::string* _window_name;

	//calib filter parameters
	double _etalon_params[3];
	int _calib_pose;

	
};


}}

#endif