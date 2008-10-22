/*
 *  ptu46.cpp
 *  boost_ptu_control
 *
 *  Created by Matia Pizzoli on 9/22/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "ptu46.h"

#include <boost/lexical_cast.hpp>


UOR::PTU46::PTU46 ( ba::io_service & ios )
: ioser_(ios),
  serport_(ios),
  timer_(ios)
{

}

// public functions

UOR::PTU46::~PTU46()
{
	serport_.cancel();
	serport_.close();
}

bool UOR::PTU46::connect(char * port, int rate)
{
	serport_.open(port);
	serport_.set_option(ba::serial_port_base::baud_rate::baud_rate(rate));
	return true;
}

bool UOR::PTU46::setup()
{
	size_t len;
	write_command_(" ");
	usleep(100000);
	write_command_("ft "); // terse feedback
	len = ba::read_until(serport_, buffer_, "\r\n");
	buffer_.consume(len);
	
	write_command_("ed "); // disable echo
	len = ba::read_until(serport_, buffer_, "\r\n");
	buffer_.consume(len);
	
	write_command_("ci "); // position mode
	len = ba::read_until(serport_, buffer_, "\r\n");
	buffer_.consume(len);
	
	tr_ = get_res(PTU46_TILT); 
	pr_ = get_res(PTU46_PAN);

	PMin_ = get_limit(PTU46_PAN, PTU46_MIN);
	PMax_ = get_limit(PTU46_PAN, PTU46_MAX);
	TMin_ = get_limit(PTU46_TILT, PTU46_MIN);
	TMax_ = get_limit(PTU46_TILT, PTU46_MAX);
	PSMin_ = get_limit(PTU46_PAN, PTU46_MIN_SPEED);
	PSMax_ = get_limit(PTU46_PAN, PTU46_MAX_SPEED);
	TSMin_ = get_limit(PTU46_TILT, PTU46_MIN_SPEED);
	TSMax_ = get_limit(PTU46_TILT, PTU46_MAX_SPEED);
	
	return true;
}

void UOR::PTU46::stop_read_()
{
	std::cerr << "aborting read" << std::endl;
	serport_.cancel();
}

float UOR::PTU46::get_res(char type)
{
	float r = -1;
	char cmd[4] = " r ";
	cmd[0] = type;
	
	// get pan res
	write_command_(cmd);

	// setup a timer
	timer_.expires_from_now(boost::posix_time::millisec(PTU_TIMER_DELAY_MSEC));
	// stop the read when timer expires
	timer_.async_wait(boost::bind(&PTU46::stop_read_, this));
	size_t len = ba::read_until(serport_, buffer_, "\r\n");
	
	if ( len < 3 )
		return -1;
	
	timer_.cancel();
	ba::buffers_iterator<ba::streambuf::const_buffers_type, char> i = 
		ba::buffers_begin(buffer_.data());
	if (*i == '*')
	{
		i += 2;
		size_t l = len - 4;
		char res[l];
		for(int ind = 0; ind < l; ind++)
			res[ind] = *(i++);
		r = atof(res)/3600.0f;
	}
	buffer_.consume(len);
	return r;	
}

// get position limit
int UOR::PTU46::get_limit(char type, char LimType)
{
	int r = -1;
	char cmd[4] = "   ";
	cmd[0] = type;
	cmd[1] = LimType;
	
	// get limit
	write_command_(cmd);
	// setup a timer
	timer_.expires_from_now(boost::posix_time::millisec(PTU_TIMER_DELAY_MSEC));
	// stop the read when timer expires
	timer_.async_wait(boost::bind(&PTU46::stop_read_, this));
	size_t len = ba::read_until(serport_, buffer_, "\r\n");

	if ( len < 3 )
		return -1;
	timer_.cancel();
	ba::buffers_iterator<ba::streambuf::const_buffers_type, char> i = 
		ba::buffers_begin(buffer_.data());
	if (*i == '*')
	{
		i += 2;
		size_t l = len - 4;
		char res[l];
		for(int ind = 0; ind < l; ind++)
			res[ind] = *(i++);
		//r = atoi(res);
		r = boost::lexical_cast<int> (res[0]);
		
	}
	buffer_.consume(len);
	return r;
}

// get position in degrees
float UOR::PTU46::get_pos (char type)
{
	int r = -1;
    char cmd[4] = " p ";
    cmd[0] = type;
	
	write_command_(cmd);
	// setup a timer
	timer_.expires_from_now(boost::posix_time::millisec(PTU_TIMER_DELAY_MSEC));
	// stop the read when timer expires
	timer_.async_wait(boost::bind(&PTU46::stop_read_, this));
	size_t len = ba::read_until(serport_, buffer_, "\r\n");
	
	if ( len < 3 )
		return -1;
	timer_.cancel();
	ba::buffers_iterator<ba::streambuf::const_buffers_type, char> iter = 
		ba::buffers_begin(buffer_.data());
	if (*iter == '*')
	{
		iter += 2;
		char res[len-4];
		for(int ind = 0; ind < len - 2 ; ind++)
			res[ind] = *(iter++);
		r = atoi(res) * (type == PTU46_TILT ? tr_ : pr_);
		//i += 2;
//		size_t l = len - 4;
//		char res[l];
//		for(int ind = 0; ind < l; ind++)
//			res[ind] = *(i++);
//		r = atoi(res) * (type == PTU46_TILT ? tr_ : pr_);
		
	}
	buffer_.consume(len);
	return r;
}

// get speed in degrees/sec
float UOR::PTU46::get_speed (char type)
{
	float r = -1;
    char cmd[4] = " s ";
    cmd[0] = type;
	
    // get speed
    write_command_(cmd);
	// setup a timer
	timer_.expires_from_now(boost::posix_time::millisec(PTU_TIMER_DELAY_MSEC));
	// stop the read when timer expires
	timer_.async_wait(boost::bind(&PTU46::stop_read_, this));
	size_t len = ba::read_until(serport_, buffer_, "\r\n");
	
    if ( len < 3 )
		return -1;
	timer_.cancel();
	ba::buffers_iterator<ba::streambuf::const_buffers_type, char> i = 
			ba::buffers_begin(buffer_.data());
	if (*i == '*')
	{
		i += 2;
		size_t l = len - 4;
		char res[l];
		for(int ind = 0; ind < l; ind++)
			res[ind] = *(i++);
		r = atoi(res) * (type == PTU46_TILT ? tr_ : pr_);
		
	}
	buffer_.consume(len);
	return r;
}

// set speed in degrees/sec
bool UOR::PTU46::set_speed (char type, float pos)
{
	bool r;
    // get raw encoder speed to move
    int Count = static_cast<int> (pos/(type == PTU46_TILT ? tr_ : pr_));
    // Check limits
    if (abs(Count) < (type == PTU46_TILT ? TSMin_ : PSMin_) || abs(Count) > (type == PTU46_TILT ? TSMax_ : PSMax_))
    {
		fprintf (stderr,"Pan Tilt Speed Value out of Range: %c %f(%d) (%d-%d)\n", type, pos, Count, (type == PTU46_TILT ? TSMin_ : PSMin_),(type == PTU46_TILT ? TSMax_ : PSMax_));
		return false;
    }
	
    char cmd[16];
    snprintf (cmd,16,"%cs%d ",type,Count);
	
    // set speed
    write_command_(cmd);
	// setup a timer
	timer_.expires_from_now(boost::posix_time::millisec(PTU_TIMER_DELAY_MSEC));
	// stop the read when timer expires
	timer_.async_wait(boost::bind(&PTU46::stop_read_, this));
	size_t len = ba::read_until(serport_, buffer_, "\r\n");
	
    if ( len <= 0 || *(ba::buffers_begin(buffer_.data())) != '*' )
	{
		fprintf (stderr,"Error setting pan-tilt speed\n");
		r = false;
	}
	else
	{
		r = true;
		timer_.cancel();
	}
	buffer_.consume(len);
    return r;
}

// set movement mode (position/velocity)
bool UOR::PTU46::set_mode (char type)
{
	bool r;
    char cmd[4] = "c  ";
    cmd[1] = type;
	
    // set mode
    write_command_(cmd);
	// setup a timer
	timer_.expires_from_now(boost::posix_time::millisec(PTU_TIMER_DELAY_MSEC));
	// stop the read when timer expires
	timer_.async_wait(boost::bind(&PTU46::stop_read_, this));
	size_t len = ba::read_until(serport_, buffer_, "\r\n");
	
    if ( len <= 0 || *(ba::buffers_begin(buffer_.data())) != '*' )
	{
		fprintf (stderr,"Error setting pan-tilt move mode\n");
		r =  false;
	}
	else
	{
		r = true;
		timer_.cancel();
	}
	buffer_.consume(len);
    return r;
}

char UOR::PTU46::get_mode ()
{
    char r;
	// get pan tilt mode
    write_command_("c ");
	// setup a timer
	timer_.expires_from_now(boost::posix_time::millisec(PTU_TIMER_DELAY_MSEC));
	// stop the read when timer expires
	timer_.async_wait(boost::bind(&PTU46::stop_read_, this));
	size_t len = ba::read_until(serport_, buffer_, "\r\n");
	ba::buffers_iterator<ba::streambuf::const_buffers_type, char> i = 
		ba::buffers_begin(buffer_.data());
    if (len < 3 || *i != '*')
    {
    	fprintf (stderr,"Error getting pan-tilt pos\n");
		r = -1;
    }
	timer_.cancel();
    if (i[2] == 'p')
    	r = PTU46_VELOCITY;
    else if (i[2] == 'i')
    	r = PTU46_POSITION;
    else
		r = -1;
	buffer_.consume(len);
    return r;
}


bool UOR::PTU46::set_pantilt(float pan, float tilt, uint waitmsec)
{
	bool r;
	size_t len = 0;
	int pan_steps = static_cast<int>(pan / pr_);
	int tilt_steps = static_cast<int>(tilt / tr_);
	
	if (pan_steps < PMin_ || pan_steps > PMax_ || tilt_steps < TMin_ || tilt_steps > TMax_ )
	{
		std::cerr << "Pan/Tilt value out of range. Tilt: " << tilt_steps <<" Pan: "<< pan_steps << std::endl;
		r = false;
	}
	else
	{
		char cmd[64];
		snprintf(cmd, 64, "S tp%d pp%d A ", tilt_steps, pan_steps);
		write_command_(cmd);
		// setup a timer
		timer_.expires_from_now(boost::posix_time::millisec(PTU_TIMER_DELAY_MSEC));
		// stop the read when timer expires
		timer_.async_wait(boost::bind(&PTU46::stop_read_, this));
		len = ba::read_until(serport_, buffer_,"*\r\n*\r\n*\r\n*\r\n");
		timer_.cancel();
		r = true;
	}
	buffer_.consume(len);
	return r;
}

bool UOR::PTU46::reset()
{
	boost::array<char, 1> temp;
	int len = 0;
	char response[10] = "!T!T!P!P*";
	write_command_ ("r ");
	// setup a timer
	timer_.expires_from_now(boost::posix_time::seconds(PTU_RESET_DELAY_SEC));
	// stop the read when timer expires
	timer_.async_wait(boost::bind(&PTU46::stop_read_, this));
	for (int i = 0; i < 9; ++i)
	{
		len = ba::read(serport_, ba::buffer(temp));
		if ((len != 1) || (temp[0] != response[i]))
		{
			fprintf(stderr,"Error Resetting Pan Tilt unit\n");
			fprintf(stderr,"Stopping access to pan-tilt unit\n");
			return false;
		}
	}
	timer_.cancel();
	return true;
}

// private functions

size_t UOR::PTU46::write_command_(const char * cmd)
{
	boost::system::error_code ignored_error;
	return ba::write(	serport_, 
								ba::buffer(cmd),
								ba::transfer_all(), 
								ignored_error );
}
