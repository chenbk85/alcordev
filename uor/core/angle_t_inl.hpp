/*
 *  basic_angle_t_impl.hpp
 *  vf_rbpf_slam2d
 *
 *  Created by andera on 24/09/08.
 *  Copyright 2008 Sapienza, Università di Roma. All rights reserved.
 *
 */

#include <stdexcept>

#include "metric2d_t.h"

namespace uor  {
	
	// CTOR
	
	///default ctor
	template <typename T>
	basic_angle_t<T>::basic_angle_t()
	:angle_(0.0) {}
	
	///ctor radians
	template <typename T>
	basic_angle_t<T>::basic_angle_t(rad_t, T _a)
	: angle_(_a) { normalize_(); }
	
	///ctor degrees
	template <typename T>
	basic_angle_t<T>::basic_angle_t(deg_t, T _a)
	: angle_(cvt<T>::deg2rad(_a)) {normalize_();}
	
	
	template <typename T>
	///ctor from x and Y (used atan2)
	basic_angle_t<T>::basic_angle_t (T x, T y)
	{
		if (x==0 && y==0)
			throw std::runtime_error("ERROR:  atan2(0,0) in basic_angle_t::basic_angle_t(x, y) *_*");
		angle_ = ::atan2(y,x);
	}
	
	template <typename T>
	basic_angle_t<T>::basic_angle_t(const basic_metric2d_t <T>& point)
	{
		if (point.x(mt) == 0 && point.y(mt) == 0)
			throw std::runtime_error("ERROR:  atan2(0,0) in basic_angle_t::basic_angle_t( metric2d_t ) *_*");
		angle_ = ::atan2(point.y(mt), point.x(mt));
	}
	
	
	template <typename T>
	basic_angle_t<T>::basic_angle_t(const basic_angle_t<T>& _ang)
	{
		angle_ = _ang.rad();
		//normalize_(); //not really needed here, should be already normalized.
	}
	
	//
	template<typename T>
	void basic_angle_t<T>::normalize_ ()
	{
		if (angle_ >= -constants<T>::pi() && angle_ < constants<T>::pi())
			return;
		
		int multiplier = static_cast<int> (0.5 * angle_ * constants<T>::inv_pi() );
		//
		angle_ = angle_ - multiplier * constants<T>::two_pi();
		//changed ... removed == part ...
		if (angle_ > constants<T>::pi())
			angle_ -= constants<T>::two_pi();
		if (angle_ < -constants<T>::pi())
			angle_ += constants<T>::two_pi();
	}
	
	template <typename T>
	basic_angle_t<T>& basic_angle_t<T>::operator+=(const basic_angle_t<T>& temp)
	{
		angle_+=temp.rad();
		normalize_();
		return *this;
	}
	
//	//	
//	template <typename T>
//	bool	basic_angle_t<T>::operator==(const basic_angle_t<T>& other)
//	{
//		return this->angle_ == other.rad();
//	}
	
	//assign
	template <typename T>
	basic_angle_t<T>&	basic_angle_t<T>::operator=(const basic_angle_t<T>& other)
	{
		this->angle_ = other.rad();
		return *this;
	}
	
	template <typename T>
	basic_angle_t<T>&	basic_angle_t<T>::operator-=(const basic_angle_t<T>& temp)
	{
		angle_ -= temp.rad();
		normalize_();
		return *this;
	}
	
	template <typename T>
	basic_angle_t<T>&	basic_angle_t<T>::operator*=(T val)
	{
		angle_*=val;
		normalize_();
		return *this;
	}
	
	template <typename T>	
	///cosine
	T	basic_angle_t<T>::cos()	const	{return  ::cos(angle_);}
	
	template <typename T>	
	///sine
	T	basic_angle_t<T>::sin()	const	{return  ::sin(angle_);}
	
	template <typename T>
	///tangent
	T	basic_angle_t<T>::tan()	const   
	{
		
		if (::fabs(angle_ - constants<T>::pi_2()) < 0.00001)
			throw std::runtime_error("ERROR: tan of a 90 degrees angle in basic_angle_t::tan() *_*");
		return  ::tan(angle_);
	}
	
	
} //close namespace uor
