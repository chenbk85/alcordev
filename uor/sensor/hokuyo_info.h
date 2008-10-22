/*
 *  hokuyo_info.h
 *  vf_rbpf_slam2d
 *
 *  Created by andera on 30/09/08.
 *  Copyright 2008 Sapienza, Università di Roma. All rights reserved.
 *
 */

#ifndef UOR_HOKUYO_INFO_H_INCLUDED__
#define UOR_HOKUYO_INFO_H_INCLUDED__

#include <boost/config.hpp>
//#include <boost/assign/list_of.hpp> // for 'map_list_of()'
//#include <boost/assert.hpp> 
#include <map>

//
#include "core/oriented_point2d_t.h"

#include <string>
#include <utility>
#include <map>

///
namespace uor  {
	
	//-----------------------------------------------------------------------

	typedef  std::pair<std::size_t,std::size_t> step_range_t;
	
	struct lrf_240 {};//full scan
	struct lrf_180 {};
	struct lrf_90 {};	
	
	///
	template <typename T>
	struct lrf_mode;
	
#define LRF_MODE_SPECIALIZATION(_NAME, _TAG, _APERTURE, _START, _END, _CC)				 \
template <>																			     \
struct lrf_mode<_TAG>																     \
	{																					 \
		lrf_mode(){};																	 \
		 static std::string name()  {return _NAME;}										\
		 static size_t aperture()   { return _APERTURE; }								\
		 static size_t start_step() {return _START; }									\
		 static size_t end_step()   {return  _END; }									\
		 static step_range_t step_range()   {return std::make_pair(_START, _END);}		\
		 static size_t  cc()  {return _CC;}												\
		 static size_t  nbeams()  {return _END - _START + 1;}							\
	};
	
	//ARGS (name as a string, aperture in degrees, start step, end step, cluster count)
	LRF_MODE_SPECIALIZATION("Full Scan Mode 240 degrees", lrf_240, 240, 44,  724, 1);// [-119.531°, 119.5331°] 681 lines 
	LRF_MODE_SPECIALIZATION("Full Scan Mode 180 degrees", lrf_180, 180, 128, 640, 1);
	LRF_MODE_SPECIALIZATION("Full Scan Mode 90 degrees",  lrf_90,  90 , 256, 512, 1);
	
	
	//-----------------------------------------------------------------------
	///A separate class to set the working parameters for acquisition
	struct hokuyo_info
	{
		///
		hokuyo_info();
		///
		hokuyo_info(const hokuyo_info&);
		///
		bool load_from_ini(const std::string& inifile);
		
		///
		hokuyo_info& operator = (const hokuyo_info& other);
		
		///
		template <typename M>
		void setmode(const lrf_mode<M>& mode);

		///
		std::string name;
		///
		angle_t	aperture;
		///
		size_t start_step;
		///
		size_t end_step;
		
		///
		size_t clustercount;
		///device address
		std::string port;
		///
		pose2d_t pose;
		
		///TODO: (fix) considers only cc ==1
		size_t nbeams;
		
	};
	

	
}//close namespace uor

	//-----------------------------------------------------------------------
	//
	inline std::ostream& operator<<(std::ostream& os, const uor::hokuyo_info& temp)
	{
		//
		os 
		<< "Device Port: "           << temp.port << std::endl
		<< "Pose: "           << temp.pose << std::endl<< std::endl
		<< "Profile Name: "   << temp.name << std::endl
		<< "Aperture: "       << temp.aperture << std::endl
		<< "Start Step: "			 << temp.start_step << std::endl
		<< "End Step: "				<< temp.end_step << std::endl		
		<< "Cluster count: "  << temp.clustercount << std::endl	
		<< "Beams: "	<< temp.nbeams
		<< std::endl;
		
		return os;
	}

#endif //UOR_HOKUYO_INFO_H_INCLUDED__