#ifndef glfw_win_HPP_INCLUDED
#define glfw_win_HPP_INCLUDED

#include "alcor/gui/glfw_wrapper.hpp"
#include "alcor.extern/petit/TrackBall.h"
#include "alcor.extern/petit/Matrix.h"

namespace all {
	namespace core {

class glfw_win_t : public all::core::glfw_wrapper {

public:

	glfw_win_t(const char* ini_file="config/glfw_win.ini");

protected:

	virtual void _mouse_btn_cb(int, int);
	virtual void _mouse_pos_cb(int, int);

	virtual void _mouse_wheel_cb(int);

	void _draw_cb();

	void _set_view();

private:

	TrackBall* _trackball;
	Quatf _quat;
	Matrix4f _old_rot_matrix;
	Matrix4f _rot_matrix;
	int _curr_wheel;

	float _cam_x, _cam_y, _cam_z;

};

}}

#endif