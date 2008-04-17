#include "glfw_wrapper.hpp"

#include "alcor/core/config_parser_t.hpp"


namespace all {
	namespace core {

glfw_wrapper* glfw_wrapper::_instance = NULL;

glfw_wrapper::glfw_wrapper(const char* ini_file) {

	if (_instance != NULL) {
		if (_running == true) {
			stop();
		}
		delete _instance;
	}

	_instance = this;

	_running = false;

	all::core::config_parser_t config;
	config.load(core::ini, ini_file);

	_screen_width = config.get<int>("screen.width", 640);
	_screen_height = config.get<int>("screen.height", 480);
	_full_screen = config.get<bool>("screen.fullscreen", 0);

}

glfw_wrapper::~glfw_wrapper() {
	if (_running == true) {
		stop();
	}
	delete _instance;
	_instance = NULL;
}

void glfw_wrapper::run_async() {

	if (_running == true)
		stop();

	_glfw_thread.reset(new boost::thread(boost::bind(&glfw_wrapper::_main_loop, this)));

}

void glfw_wrapper::stop() {
	_running = false;
	//_glfw_thread->join();
}


void glfw_wrapper::_main_loop() {
	
	if (!glfwInit()) {
		printf("Unable to init glfw...\n");
		return;
	}
    
	if( !glfwOpenWindow( _screen_width, _screen_height, 0,0,0,0, 16,0, GLFW_WINDOW) )
    {
		printf("Unable to open opengl window...\n");
        glfwTerminate();
		return;
    }


	glfwSetKeyCallback( glfw_wrapper::_key_function);
	glfwSetWindowSizeCallback(glfw_wrapper::_resize_function);
	glfwSetMousePosCallback(glfw_wrapper::_mouse_pos_function);
	glfwSetMouseButtonCallback(glfw_wrapper::_mouse_btn_function);
	glfwSetMouseWheelCallback(glfw_wrapper::_mouse_wheel_function);

	_running = true;

	_gl_init_cb();

	while (_running) {

		_draw_cb();
		
		glfwSwapBuffers();

		glfwSleep(0.01);

		_glfw_thread->yield();
	}

	printf("closing...\n");
	glfwTerminate();

}


void GLFWCALL glfw_wrapper::_key_function(int k, int action) {

	_instance->_key_cb(k, action);

}

void GLFWCALL glfw_wrapper::_resize_function(int width, int height) {
	
	_instance->_resize_cb(width, height);

}

void GLFWCALL glfw_wrapper::_mouse_pos_function(int x, int y) {
	
	_instance->_mouse_pos_cb(x, y);

}

void GLFWCALL glfw_wrapper::_mouse_btn_function(int b, int action) {
	
	_instance->_mouse_btn_cb(b, action);

}

void GLFWCALL glfw_wrapper::_mouse_wheel_function(int w) {
	
	_instance->_mouse_wheel_cb(w);

}
}}