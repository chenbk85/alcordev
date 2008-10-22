/*
 *  hokuyo_info.cpp
 *  vf_rbpf_slam2d
 *
 *  Created by andera on 30/09/08.
 *  Copyright 2008 Sapienza, Università di Roma. All rights reserved.
 *
 */

#include "sensor/hokuyo_info.h"

#include <fstream>

//boost includes
#include <boost/program_options.hpp>

namespace uor  {
	//***********************************************************************
	// LRF_SETTINGS
	//
	hokuyo_info::hokuyo_info ()
	{
		//		///		
		this->port = "port";
		//
		setmode( lrf_mode<lrf_240>() );
		
		this->clustercount = 1;
	} 
	
	///
	
	hokuyo_info::hokuyo_info(const hokuyo_info& other)
	{
		(*this) = other;		
	}
	
	///
	hokuyo_info& hokuyo_info::operator = (const hokuyo_info& other)
	{
		//TODO: fix
		this->port = other.port;
		this->pose=  other.pose;
		
		this->name = other.name;
		this->aperture = other.aperture;
		this->start_step = other.start_step;
		this->end_step = other.end_step;
		
		this->clustercount = other.clustercount;
		
		this->nbeams = other.nbeams;
		
		return (*this);
	}
	
	///
	bool hokuyo_info::load_from_ini(const std::string& inifile)
	{
		try {
			
			//-------------------------------------------------------
			// Importing settings from the ini
			namespace po = boost::program_options;
			po::variables_map vm;
			
			
			po::options_description desc("Allowed options");
			//
			desc.add_options()
			//physical sensor
			("device.port", po::value<std::string>() )
			("device.x",  po::value<double>() )
			("device.y",  po::value<double>() )	
			("device.deg", po::value<double>() )	
			//		//acquisition
			("mode.preset", po::value<int>() )
			("mode.clustercount",  po::value<size_t>() )
			;
			
			//
			std::cout 
			<< "Loading Hokuyo configuration from: " 
			<< std::endl
			<< inifile
			<< std::endl << std::endl;
			
			std::ifstream fis(inifile.c_str(), std::ifstream::in);

			po::store(po::parse_config_file(fis, desc, true), vm);
			po::notify(vm);
			
			///		
			port = vm["device.port"].as<std::string>();
			///
			double x	= vm["device.x"].as<double>();
			double y	= vm["device.y"].as<double>();
			double th	= vm["device.deg"].as<double>();
			///
			pose.move_to(metric2d_t(mt, x, y)).angle().deg(th);
			
			///
			int val = vm["mode.preset"].as<int>();
			
			switch (val) {
				case 240:
					setmode(lrf_mode<lrf_240> ());
					break;
				case 180:
					setmode(lrf_mode<lrf_180>() );
					break;
				case 90:
					setmode(lrf_mode<lrf_90>());
					break;
				default:
					setmode(lrf_mode<lrf_240> ());
					break;
			}
					return true;
			
		}
		catch (std::exception& e) {
			
			std::cout 
			<< std::endl
			<< "OUCH :( " << std::endl
			<< e.what() << std::endl
			<< "Means: DONT FIND Config File, or Error during parsing ini file" << std::endl
			<< std::endl;
			return false;
		}
		
	}
	
	template <typename M>
	void hokuyo_info::setmode(const lrf_mode<M>& mode)
	{
		std::cout
		<< std::endl
		<< "Setting Mode: " 
		<< mode.name() 
		<< std::endl << std::endl;
		
		name = mode.name();
		aperture.deg(mode.aperture());
		start_step  = mode.start_step();
		end_step	= mode.end_step();
		clustercount = mode.cc();
		nbeams = mode.nbeams();
	}
	
}//close namespace uor