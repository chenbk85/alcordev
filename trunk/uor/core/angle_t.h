/*
 *  angle_t.h
 *  vf_rbpf_slam2d
 *
 *  Created by andera on 22/09/08.
 *  Copyright 2008 Sapienza, Università di Roma. All rights reserved.
 *
 */

#ifndef UOR_ANGLE_T_H_INCLUDED__
#define UOR_ANGLE_T_H_INCLUDED__

#include "math_utils.h"
#include <iostream>


namespace uor  {
	///-------------------------------------------------------------
	struct rad_t{};
	struct deg_t{};
	///-------------------------------------------------------------
	static const rad_t rad = rad_t();
	static const deg_t deg = deg_t();
	///-------------------------------------------------------------
	//	//forward
	template <typename T>
	class basic_metric2d_t;
	
	///
	template <typename T = double>
	class basic_angle_t
	{
	
public:
	///default ctor
	basic_angle_t();
	
	///ctor radians
	basic_angle_t(rad_t, T _a);
	
	///ctor degrees
	basic_angle_t(deg_t, T _a);
	
	///ctor from x and Y (uses atan2)
	basic_angle_t (T x, T y); //throws std::runtime_error
	
	///ctor from point
	basic_angle_t(const basic_metric2d_t<T>& point); 
	///
	basic_angle_t(const basic_angle_t<T>&);
	///
	typedef T value_type;
	
	//setters & getters	
public:
	///set radians
	void rad(T newval)   { angle_ = newval;}
	///set degrees
	void deg(T newval)   {angle_ = cvt<T>::deg2rad(newval);}
	
	//getters
	///get radians
	inline T rad() const {return angle_;}
	///get degrees
	inline T deg() const {return cvt<T>::rad2deg(angle_);}
	
	//standard operations
public:
	//			//ops
	basic_angle_t&	operator+=(const basic_angle_t<T>& other);
	basic_angle_t&	operator-=(const basic_angle_t<T>& other);
	///assign
	basic_angle_t<T>&	operator=(const basic_angle_t<T>& other);
	///
	//bool	operator==(const basic_angle_t<T>& other);	
	
	//not really important these.
	basic_angle_t<T>&	operator*=(T val);
	
	//trigonometric operations	
public:	//trigonometric operations
	///cosine
	T	cos()	const;
	///sine
	T	sin()	const;
	///tangent
	T	tan()	const; //throws std::runtime_error 
	
	//functions
private:	
	///reset value to [-pi:pi]
	void normalize_ (); 
	//[data]
private:
	T angle_;
};


typedef basic_angle_t<> angle_t;

}//close namespace uor

//
template <typename T>
inline std::ostream&	operator<<(std::ostream& os, const uor::basic_angle_t<T>& temp)
{
	os<< "radians/degrees: " << temp.rad() << "/" << temp.deg(); 
	return os;
}

template <typename T>
const inline uor::basic_angle_t<T>	operator+(const uor::basic_angle_t<T>& a, const uor::basic_angle_t<T>& b)
{
	uor::basic_angle_t<T> c = a;
	c += b;
	return c;
}

template <typename T>
inline uor::basic_angle_t<T>	operator-(const uor::basic_angle_t<T>& a, const uor::basic_angle_t<T>& b)
{
	uor::basic_angle_t<T> c = a;
	c -= b;
	return c;
}

//template <typename T>
//bool operator==(const uor::basic_angle_t<T>& a, const uor::basic_angle_t<T>& b)
//{
//
//	return (a.rad() == b.rad());
//}


//TODO
//const inline angle	operator*(const angle& a, double b)
//{
//	angle c = a;
//	c *= b;
//	return c;
//}
//
//const inline angle	operator*(double b, const angle& a)
//{
//	angle c = a;
//	c *= b;
//	return c;
//}
//
//const inline angle	operator/(const angle& a, double b)
//{
//	angle c = a;
//	c /= b;
//	return c;
//}
//
//bool	inline operator<(const angle& a, const angle& b){return a.deg() < b.deg();}
//bool	inline operator<=(const angle& a, const angle& b){return a.deg() <= b.deg();}
//bool	inline operator>(const angle& a, const angle& b){return a.deg() > b.deg();}
//bool	inline operator>=(const angle& a, const angle& b){return a.deg() >= b.deg();}

//implementation
#include "angle_t_inl.hpp"

#endif //UOR_ANGLE_T_H_INCLUDED__