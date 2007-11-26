#ifndef stereo_process_t_H_INCLUDED
#define stereo_process_t_H_INCLUDED

#include <cv.h>
#include <cvaux.h>

#include <StereoMatching.h>

#include "alcor/core/config_parser_t.hpp"
#include "alcor/math/size3d_t.hpp"


namespace all {
	namespace sense {

class stereo_process_t {

public:
	stereo_process_t();

	bool init(const char* config_file = "config/stereo_process.ini");
	void stop();

	//rectify stereo pair. 
	void rectify(IplImage**, IplImage**);

	//input: unrectified stereo images
	IplImage* do_stereo(IplImage*, IplImage*);

	all::math::size3d_t get_dims();

	IplImage* get_disparity();

private:

	void do_3d_();

	CvCalibFilter _calib_filter;
	
	StereoMatching _estereo_matcher;
	StereoImage* _estereo_image;

	IplImage* _gray_left;
	IplImage* _gray_right;

	IplImage* _depth_image;

	IplImage* _disp_image;

	all::math::size3d_t _image_size;

	//for 3d reconstruction
	float _focal_length[2];
	float _baseline;
	float _u0;
	float _v0;


};

}}

#endif