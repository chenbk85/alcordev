#include "stereo_camera_t.hpp"



namespace all {
	namespace sense {

		
stereo_camera_t::stereo_camera_t() {

}

bool stereo_camera_t::open(char* config_file) {
	
	VI = new videoInput();
	videoInput::listDevices();
	
	getchar();

	all::core::config_parser_t config;
	config.load(core::ini, config_file);

	_cam_id[left] = config.get<int>("left.id", 1);
	_cam_id[right] = config.get<int>("right.id", 2);

	std::string stereo_proc_ini = config.get<std::string>("config.stereo_process_conf", "config/stereo_process.ini");

	stereo_process.init(stereo_proc_ini.c_str());

	_image_dim = stereo_process.get_dims();

	_cam_image[left] = cvCreateImage(cvSize(_image_dim.col_, _image_dim.row_), IPL_DEPTH_8U, _image_dim.depth_);
	_cam_image[right] = cvCreateImage(cvSize(_image_dim.col_, _image_dim.row_), IPL_DEPTH_8U, _image_dim.depth_);

	_cam_image[left]->origin = IPL_ORIGIN_BL;
	_cam_image[right]->origin = IPL_ORIGIN_BL;

	//_depth_image = cvCreateImage(cvSize(_image_dim.col_, _image_dim.row_), IPL_DEPTH_32F, 3);
	//open cameras
	if (!VI->setupDevice(_cam_id[left], _image_dim.col_, _image_dim.row_))
		return false;
	if (!VI->setupDevice(_cam_id[right], _image_dim.col_, _image_dim.row_))
		return false;

	return true;

}

void stereo_camera_t::close() {
	VI->stopDevice(_cam_id[left]);
	VI->stopDevice(_cam_id[right]);
	cvReleaseImage(&_cam_image[left]);
	cvReleaseImage(&_cam_image[right]);
}

void stereo_camera_t::grab_stereo() {
	VI->getPixels(_cam_id[left], reinterpret_cast <unsigned char*> (_cam_image[left]->imageData), false);
	VI->getPixels(_cam_id[right], reinterpret_cast <unsigned char*> (_cam_image[right]->imageData), false);

	_depth_image = stereo_process.do_stereo(_cam_image[left], _cam_image[right]);
	_disp_image = stereo_process.get_disparity();
}

}} //namespaces