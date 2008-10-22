/*
 *  main.cpp
 *  boost_ptu_control
 *
 *  Created by Matia Pizzoli on 9/22/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "ptu46.h"

int main(){	
	boost::asio::io_service ios;
	UOR::PTU46 ptu(ios);
	//ios.run();
	ptu.connect("/dev/tty.usbserial", 9600);
	ptu.setup();
	std::cout << ptu.get_limit(PTU46_PAN, PTU46_MIN) << std::endl;
	std::cout << ptu.get_limit(PTU46_PAN, PTU46_MAX) << std::endl;
	std::cout << ptu.get_limit(PTU46_TILT, PTU46_MIN) << std::endl;
	std::cout << ptu.get_limit(PTU46_TILT, PTU46_MAX) << std::endl;
	std::cout << ptu.get_limit(PTU46_PAN, PTU46_MIN_SPEED) << std::endl;
	std::cout << ptu.get_limit(PTU46_PAN, PTU46_MAX_SPEED) << std::endl;
	std::cout << ptu.get_limit(PTU46_TILT, PTU46_MIN_SPEED) << std::endl;
	std::cout << ptu.get_limit(PTU46_TILT, PTU46_MAX_SPEED) << std::endl;
	std::cout << ptu.get_res(PTU46_TILT) << std::endl;
	std::cout << ptu.get_res(PTU46_PAN) << std::endl;
	
	if(ptu.reset())
		std::cout << "PTU resetted." << std::endl;
	
	if(ptu.set_pantilt(53, 30, 0))
		std::cout << "changed PTU position: P " << ptu.get_pos(PTU46_PAN) << " T "<< ptu.get_pos(PTU46_TILT) << std::endl;
	if(ptu.set_mode(PTU46_POSITION))
		std::cout << "changed PTU mode: " << ptu.get_mode() << std::endl;
	if(ptu.set_speed(PTU46_PAN, 200))
		std::cout << "changed PTU speed: " << ptu.get_speed(PTU46_PAN) << std::endl;
	if(ptu.reset())
		std::cout << "PTU resetted." << std::endl;
}
