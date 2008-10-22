/*
 *  beam_t.h
 *  vf_rbpf_slam2d
 *
 *  Created by andera on 29/09/08.
 *  Copyright 2008 Sapienza, Università di Roma. All rights reserved.
 *
 */

#ifndef UOR_BEAM_T_H_INCLUDED__
#define UOR_BEAM_T_H_INCLUDED__

#include "core/metric2d_t.h"
#include "core/angle_t.h"

namespace uor  {
	
	///hokuyo LRF beams struct.
	struct beam_info_t
	{
		size_t index;
		angle_t angle;
		angle_t::value_type sine;
		angle_t::value_type cosine;
		range_t range;
	};
	
	
	
	///hokuyo LRF beam struct.
	struct beam_t 
	{
		double  sine;
		
		double  cosine;
		
		range_t range; //measured range
	};
	
	///
	typedef std::vector<beam_t> beam_vec;
	typedef std::vector<beam_info_t> beam_info_vec;
} // close namespace uor

///convert polar to cartesian.
//template < typename T>
//inline uor::basic_metric2d_t<T>& operator << (uor::basic_metric2d_t<T>& target, const uor::beam_t& source)
//	{
//		target.xy(uor::mt, source.range.mt() * source.cosine,  source.range.mt() * source.sine);
//	
//		return target;
//	};

///convert polar to cartesian.
template < typename T>
inline uor::basic_metric2d_t<T>& operator << (uor::basic_metric2d_t<T>& target, const uor::beam_info_t& source)
{
	target.xy(uor::mt, source.range.mt() * source.cosine,  source.range.mt() * source.sine);
	
	return target;
};


///
inline std::ostream&	operator<<(std::ostream& os, const uor::beam_t& temp)
{
	os 
	<< temp.range 
	<< " sine: " << temp.sine
	<< " cosine: " << temp.cosine
	<< std::endl;
	
	return os;
}


#endif //UOR_BEAM_T_H_INCLUDED__