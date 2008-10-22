/*
 *  ptu46.h
 *  boost_ptu_control
 *
 *  Created by Matia Pizzoli on 9/22/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
#include <boost/asio.hpp>
#include <boost/bind.hpp>

// serial port defines
#define PTU46_DEFAULT_BAUD 9600
#define PTU46_BUFFER_LEN 127
#define DEFAULT_PTU_PORT "/dev/tty.usbserial"
#define PTU_TIMER_DELAY_MSEC 100
#define PTU_RESET_DELAY_SEC 100

// command defines
#define PTU46_PAN 'p'
#define PTU46_TILT 't'
#define PTU46_MIN 'n'
#define PTU46_MAX 'x'
#define PTU46_MIN_SPEED 'l'
#define PTU46_MAX_SPEED 'u'
#define PTU46_VELOCITY 'v'
#define PTU46_POSITION 'i'

namespace ba = boost::asio;

namespace UOR{
	
	class PTU46
		{
		public:
			PTU46( ba::io_service &);
			~PTU46();
			bool connect(char *, int rate=PTU46_DEFAULT_BAUD);
			bool setup();
			float get_res(char);
			int get_limit(char, char);
			float get_pos(char);
			float get_speed(char);
			bool set_speed(char, float);
			bool set_mode(char);
			char get_mode();
			bool set_pantilt(float pan, float tilt, uint waitmsec=-1);
			bool reset();
		private:
			// Position Limits
			int TMin_, TMax_, PMin_, PMax_;
			// Speed Limits
			int TSMin_, TSMax_, PSMin_, PSMax_;
			// pan and tilt resolution
			float tr_, pr_;
			// read buffer
			ba::streambuf buffer_;
			// serial port
			ba::serial_port serport_;
			// timer
			ba::deadline_timer timer_;
			// 
			ba::io_service& ioser_;
			// stop read if timer expires
			void stop_read_();
			// writes command to serport
			size_t write_command_(const char * cmd);
			//bool UOR::PTU46::command_ok_();
		};
	
}