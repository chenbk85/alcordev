#ifndef glfw_wrapper_HPP_INCLUDED
#define glfw_wrapper_HPP_INCLUDED

#include <GL/glfw.h>

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>


namespace all {
	namespace core {


class glfw_wrapper {

public:
	
	glfw_wrapper(const char* ini_file="config/glfw.ini");
	~glfw_wrapper();

	void run_async();

	void stop();

protected:

	//to override in the derived class
	virtual void _key_cb(int, int) {}
	virtual void _resize_cb(int, int) {}
	virtual void _mouse_pos_cb(int, int) {}
	virtual void _mouse_btn_cb(int, int) {}
	virtual void _mouse_wheel_cb(int) {}
	virtual void _draw_cb() {}
	virtual void _gl_init_cb() {}

protected:

	bool _full_screen;
	int _screen_width;
	int _screen_height;

private:
	
	void _main_loop();

	//static callback...they just call the virtual methods
	static void GLFWCALL _key_function(int, int);
	static void GLFWCALL _resize_function(int, int);
	static void GLFWCALL _mouse_pos_function(int, int);
	static void GLFWCALL _mouse_btn_function(int, int);
	static void GLFWCALL _mouse_wheel_function(int);

private:

	//async running thread
	boost::shared_ptr<boost::thread> _glfw_thread;
	volatile bool _running;

	static glfw_wrapper* _instance;

};

}}

#endif