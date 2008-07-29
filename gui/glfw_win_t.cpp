#define __MK_GEOMETRY_TRACKBALL_INLINE__
//#define __MK_GEOMETRY_MATRIX_INLINE__

#include "glfw_win_t.hpp"

namespace all {
	namespace core {

	glfw_win_t::glfw_win_t(const char* ini_file): glfw_wrapper(ini_file) {
	
	
	_rot_matrix = Matrix4d::identityMatrix();
	_old_rot_matrix = Matrix4d::identityMatrix();

	_cam_x = 0;
	_cam_y = 0;
	_cam_z = 1000;

	_curr_wheel = 0;

	_trackball = new TrackBall(_screen_height/2, Point2f(_screen_width/2, _screen_height/2));

}

void glfw_win_t::_mouse_btn_cb(int btn, int action) {
	
	//rotation
	if (btn == GLFW_MOUSE_BUTTON_LEFT){
		if(action == GLFW_PRESS) {
			int x,y;
			glfwGetMousePos(&x, &y);
			_trackball->beginTracking(Point2f(x,y));
		}
		else if (action == GLFW_RELEASE) {
			_old_rot_matrix = _rot_matrix * _old_rot_matrix;
			_rot_matrix = Matrix4d::identityMatrix();
		}
	}
	//panning
	else if (btn == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		int x,y;
		glfwGetMousePos(&x, &y);

		_trackball->beginPanning(Point2f(x + _cam_x, y + _cam_y));
	}
}
	
void glfw_win_t::_mouse_pos_cb(int x, int y) {
	
	//rotation
	if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT == GLFW_PRESS)) {
		_quat = _trackball->track(Point2f(x,y));
		_quat.to4x4Matrix(&_rot_matrix);
	}
	//panning
	else if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT == GLFW_PRESS)) {
		Point2f pan = _trackball->pan(Point2f(x,y));
		_cam_x = pan.x;
		_cam_y = pan.y;
	}
}
	

void glfw_win_t::_mouse_wheel_cb(int wheel) {

	//zooming
	_cam_z -= 10*(wheel - _curr_wheel);
	_curr_wheel = wheel;
}

void glfw_win_t::_set_view() {
		
	// Setup projection matrix
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	//glOrtho(-3000, 3000, -3000, 3000, 0, 10000);
	gluPerspective(
		90.0f,                            // Angle of view
		(GLfloat)_screen_width/(GLfloat)_screen_height,   // Aspect
		1,                             // Near Z
		10000                            // Far Z
	);

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	gluLookAt(0, 0, 0, 0, 0, 1, 0, 1, 0);

	glTranslatef(_cam_x, _cam_y, _cam_z);
	glMultMatrixf(_rot_matrix.transposed().ptr());
	glMultMatrixf(_old_rot_matrix.transposed().ptr());
}

void glfw_win_t::_draw_cb() {
	_set_view();
}

}}