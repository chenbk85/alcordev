/*
 *  pose_t.h
 *  vf_rbpf_slam2d
 *
 *  Created by andera on 22/09/08.
 *  Copyright 2008 Sapienza, Università di Roma. All rights reserved.
 *
 */

#ifndef UOR_POSE2D_T_H_INCLUDED__
#define UOR_POSE2D_T_H_INCLUDED__

#include "core/metric2d_t.h"
#include "core/angle_t.h"

#include <boost/ref.hpp>

namespace uor  {
	
	template <typename T=double, typename A=double>
	class oriented_point2d_t
	{
	//CTOR
public:
	///default ctor
	oriented_point2d_t() {};
	///
	oriented_point2d_t(const basic_metric2d_t<T>& _pos) 
	: position_(_pos) {}
	///
	oriented_point2d_t(const basic_metric2d_t<T>& _pos, const basic_angle_t<A>& _ang)
	: position_(_pos), angle_(_ang){}
	
	///si ctor (mt, mt, rad)
	oriented_point2d_t(T _x , T _y , A _ang)
	: position_(mt, _x, _y),  angle_(rad,_ang){}
	
	//GETTERS
public:
	///
	inline const basic_metric2d_t<T>& position() const 
		{return boost::cref(position_);};
	///
	inline   basic_metric2d_t<T>& position()  
		{return boost::ref(position_);};
	
//	///
	const basic_angle_t<A>& angle()  const 
		{return boost::cref(angle_);}
	
	///
	basic_angle_t<A>& angle() 
		{return boost::ref(angle_);}
	
	//		pose2d(double a, double b, angle c):p_(a,b),theta_(c){}
	//		pose2d(double a, double b, double c, rad_t):p_(a,b),theta_(c,rad_tag){}
	//		pose2d(double a, double b, double c, deg_t):p_(a,b),theta_(c,deg_tag){}
	
	//OPERATIONS
public:
	///move(point).
	///Translate only the position component by an offset givent by the argument
	inline oriented_point2d_t<T,A>& move_rel(const basic_metric2d_t<T> _delta)
	{
		position_ += _delta;
		return boost::ref(*this);
	}
	///
	inline oriented_point2d_t<T,A>& move_to (const basic_metric2d_t<T>& newposition)
	{
		position_ = newposition;
		return boost::ref(*this);
	}
	
	
	///rotate(alfa)
	///applies a rotation of alfa around the position.
	///position remains the same, angle is increased by alpha.
	oriented_point2d_t<T,A>& rotate_around_p(const basic_angle_t<A>& _delta)
	{
		angle_ += _delta;
		return boost::ref(*this);
	}
	

private:
	basic_metric2d_t<T> position_;
	basic_angle_t<A> angle_;
};

typedef oriented_point2d_t<> pose2d_t;
typedef std::vector< pose2d_t > pose2d_vec;

//typedef oriented_point2d_t<> oriented_point2d;

}//close namespace uor

///
template <typename T, typename A>
inline std::ostream&	operator<<(std::ostream& os, const uor::oriented_point2d_t<T,A>& temp)
{
	os << "Point: " 
	<< temp.position()
	<< " Angle: "  
	<< temp.angle(); 
	return os;
}

#endif //UOR_POSE2D_T_H_INCLUDED__