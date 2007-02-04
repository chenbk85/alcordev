#ifndef mt9_grabber_t_H_INCLUDED
#define mt9_grabber_t_H_INCLUDED
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#ifdef MT9_DEBUG_INFO
	#include <iostream>
	#include <fstream>
#endif
//---------------------------------------------------------------------------
#include <boost/timer.hpp>
#include <boost/shared_ptr.hpp>
//---------------------------------------------------------------------------
//#include "alcor/sense/PiGateway.h"
#pragma warning (disable : 4018 4244 4819 4996 4099 936)
class PiGateway;
//---------------------------------------------------------------------------
namespace all { namespace sense{
class mt9_grabber_t;
	}}
//---------------------------------------------------------------------------
class all::sense::mt9_grabber_t
	{
	public:
		mt9_grabber_t();
		~mt9_grabber_t();

		void cancel();

		void reset();

		//void get_euler(float* rpy)  const;

		float get_yaw() const;

		float get_pitch() const;

		float get_roll() const;

		void run_thread();

#ifdef MT9_DEBUG_INFO
	private:
		std::ofstream myfile;
#endif

	private:
		boost::timer m_timer;
		bool _running;
		unsigned int m_count;
		///MT9
		boost::shared_ptr<PiGateway> mt9_;
	};
//---------------------------------------------------------------------------
#endif //mt9_grabber_t_H_INCLUDED