/*
 *  range_sensor_t.cpp
 *  vf_rbpf_slam2d
 *
 *  Created by andera on 29/09/08.
 *  Copyright 2008 Sapienza, Università di Roma. All rights reserved.
 *
 */

//	all::math::angle urg_scan_data_t::step2angle(int step) {
//		return all::math::angle((360.0/1024.0)*(static_cast<double>(step) - 384.0), all::math::deg_tag);
//	}
//	
//	int urg_scan_data_t::angle2step(all::math::angle a) {
//		return static_cast<int> ((double(1024)/double(360)) * a.deg())+384;
//	}
//	
//	all::math::angle urg_scan_data_t::resolution (int cc) {
//		return all::math::angle(static_cast<double>(cc)*(360.0/1024.0) , all::math::deg_tag);
//	}
//	
//	urg_scan_data_t::urg_scan_data_t() {
//		scan_points.reserve(MAX_NUM_POINTS);
//	}
//	
//	all::math::angle urg_scan_data_t::get_angle(int index) const {
//		return urg_scan_data_t::step2angle(start_step + (index * cc));
//	}

#include "hokuyo_lrf.h"

#include <cassert>
#include <boost/foreach.hpp>
#include <boost/random/random_number_generator.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/linear_congruential.hpp>

#include <boost/random/uniform_int.hpp>

//
#include "hokuyo_impl.cpp"

namespace uor {
	
	//
	hokuyo_lrf::hokuyo_lrf()
	{
		impl_.reset(new hokuyo_impl);
		//
		beam_info_.reserve(parameters_.nbeams);
		
		std:: cout 
		<< "Hokuyo LRF (CTOR) :: Default Parameters set!" 
		<< std::endl 
		<< parameters_ 
		<< std::endl;
		
		updatebeamlookups_();
	}
	
	///
	hokuyo_lrf::hokuyo_lrf(const hokuyo_info& params)
	{
		impl_.reset(new hokuyo_impl);
		
		parameters_ = params;
		
		beam_info_.reserve(parameters_.nbeams);
		
		//
		std:: cout 
		<< "Hokuyo LRF (CTOR) :: Parameters set!" 
		<< std::endl 
		<< parameters_ 
		<< std::endl;
		
		updatebeamlookups_();
	}
	
	///
	hokuyo_lrf::~hokuyo_lrf()
	{
		impl_->close();
	}
	
	//	///
	void hokuyo_lrf::set_params(const hokuyo_info& params)
	{
		parameters_ = params;
		
		if (!beam_info_.empty()) beam_info_.clear();
		beam_info_.reserve(parameters_.nbeams);
		
		std:: cout 
		<< "Hokuyo LRF :: Parameters set!" 
		<< std::endl 
		<< parameters_ 
		<< std::endl;	
		
		updatebeamlookups_();
	}
	
	//*************************************************************************
	//
	bool hokuyo_lrf::enable()
	{
		if ( !impl_->connect(parameters_.port) )
		{
			std::cout
			<< "LASER CANNOT CONNECT ON "
			<< parameters_.port << std::endl
			<< "Please Verify that serial port is correct" << std::endl
			<< std::endl;
			return false;
		}
		else
		{
			Sleep(300);
			return impl_->initialize ();
		}
	}
	
	///
	void hokuyo_lrf::disable()
	{
		impl_->close();
	}
	
	//
	range_reading_ptr hokuyo_lrf::get()
	{
		//
		range_reading_ptr scanptr= range_reading_t::create(parameters_.nbeams);
		//
		assert(scanptr);
		assert(scanptr->nbeams() == parameters_.nbeams);
		
		//		
		//		//fill it with random values
		boost::rand48 rng;                 // produces randomness out of thin air
		//		// see pseudo-random number generators
		boost::uniform_int<> ranges(100, 5000);       // distribution that maps to 1..6
		boost::variate_generator<boost::rand48&, boost::uniform_int<> > die(rng, ranges);             
		//
		int ir = 0;
		
		//assert(scanptr->beams_.size() == beam_info_.size());
		assert(scanptr->cartesian_.size() == beam_info_.size());
		
		BOOST_FOREACH( point2d_t b, scanptr->cartesian_)
		{
			
			beam_info_[ir].range.mm(die()) ;
			scanptr->cartesian_[ir] <<  beam_info_[ir]; //polar to cartesian

//			std::cout 
//			<< "Beam " <<  beam_info_[ir].range <<   " at angle: " << beam_info_[ir].angle  << std::endl
//			<< "Projected in: " << scanptr->cartesian_[ir] << std::endl
//			<< std::endl;
			
			ir++;
		}
		
		return scanptr;
	}
	
	///
	angle_t hokuyo_lrf::resolution ()
	{
		return angle_t(rad, constants<double>::two_pi()/1024.0);
	}
	
	///
	angle_t hokuyo_lrf::step2angle(size_t step)
	{
		return angle_t(rad, (constants<double>::two_pi()/1024.0)*(static_cast<double>(step) - 384.0));
	}
	
	///
	size_t hokuyo_lrf::angle2step(const angle_t& angle) const {
 
		return static_cast<size_t> ((double(1024) / constants< >::two_pi() ) * angle.rad() ) + 384;
 
	}
	
	///
	void hokuyo_lrf::updatebeamlookups_()
	{
		beam_info_t beam;
		for(size_t it = parameters_.start_step; it <= parameters_.end_step; it++)
		{
			beam.index = it;
			beam.angle  = step2angle(it);
			beam.sine   = beam.angle.sin();
			beam.cosine = beam.angle.cos();
			beam_info_.push_back(beam);
		}
		
		assert(beam_info_.size() == parameters_.nbeams);
		
		//
		std::cout << std::endl << parameters_.name << std::endl ;
		//		BOOST_FOREACH( beam_info_t b, beam_info_ )
		//		{
		//			std::cout
		//			<< "Beam #: "  << b.index << std::endl
		//			<< "angle: "   << b.angle << std::endl
		//			<<  "sine: "   << b.sine  << std::endl
		//			<<  "cosine: " << b.cosine  << std::endl
		//			<< std::endl;
		//		}
		
	}
	
}//close namespace uor
