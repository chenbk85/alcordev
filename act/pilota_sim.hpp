#ifndef pilota_sim_HPP_INCLUDED
#define pilota_sim_HPP_INCLUDED
//-----------------------------------------------
#include "alcor/act/p3_gateway.h"
#include "alcor/act/p3_server_t.h"
//-----------------------------------------------
//#include "alcor/core/memory_stream_source_t.hpp"
//#include <alcor/core/stream_server_t.hpp>
//-----------------------------------------------
//#define _USE_MATLAB_
//-----------------------------------------------
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#pragma comment (lib,"AriaStatic.lib")
#pragma comment (lib,"ArNetworkingStatic.lib")
//-----------------------------------------------
#pragma comment (lib,"p3_gateway.lib")
#pragma comment (lib,"p3_client_server.lib")
//-----------------------------------------------
//#pragma  comment (lib, "data_streaming.lib")
//-----------------------------------------------
//#pragma comment (lib, "jpeg_codec.lib")
//-----------------------------------------------
#endif
//-----------------------------------------------
#endif
//-----------------------------------------------
#include <boost/lexical_cast.hpp>
//-----------------------------------------------
#define ARIA_STATIC
//-----------------------------------------------
using namespace all;
//-----------------------------------------------

class pilota_sim
{
public:
	pilota_sim(); // ctor
    void cancel() {running_ = false;};

	void wandering();
	void stop();


private:
	//p3
	act::p3_server_ptr_t  p3_server_;
	act::p3_gateway_ptr_t p3;

    volatile bool running_;

};

inline pilota_sim::pilota_sim(){
	
	//p3 gateway
	 std::string configname   = "config/p3_conf.ini";
	 std::cout << "Carico il file : " << configname << std::endl;

	 p3.reset(new act::p3_gateway());
	 p3->open(configname);

	 //p3 server
	 std::cout << "Start p3 server" << std::endl;
	 p3_server_.reset(new act::p3_server_t(configname.c_str()));
	 p3_server_->set_gateway_ptr(p3);
	 p3_server_->run_async();

}

inline void pilota_sim::wandering(){
	p3->enable_wander_mode();
}

inline void pilota_sim::stop(){
	p3->enable_stop_mode();
}