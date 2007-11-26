#ifndef stereo_camera_t_H_INCLUDED
#define stereo_camera_t_H_INCLUDED

#include "stereo_process_t.hpp"

#include "alcor.extern/videoinput/videoinput.h"
#include "alcor/core/config_parser_t.hpp"
#include "alcor/math/size3d_t.hpp"

namespace all {
	namespace sense {

class stereo_camera_t {

public:
	stereo_camera_t();

	bool open(char* config_file  = "config/stereo_camera.ini");
	void close();

	void grab_stereo();

	///left or right
	enum {
    left  = 0,
    right = 1,
	} e_source;

public:

	stereo_process_t stereo_process;

	videoInput* VI;

	int _cam_id[2];
	IplImage* _cam_image[2];
	IplImage* _depth_image;
	IplImage* _disp_image;

	//obtained by stereo_process object to initialize cameras (calibration dimension)
	all::math::size3d_t _image_dim;

};


}}//namespaces

#endif