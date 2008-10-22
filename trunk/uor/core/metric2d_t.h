/*
 *  position_t.h
 *  vf_rbpf_slam2d
 *
 *  Created by andera on 22/09/08.
 *  Copyright 2008 Sapienza, Università di Roma. All rights reserved.
 *
 */

#ifndef UOR_POSITION_T_INCLUDED__
#define UOR_POSITION_T_INCLUDED__

#include <iostream>
#include <vector>
#include "math_utils.h"

namespace uor {
	
	//
	struct mt_t {};
	struct cm_t {};
	struct mm_t {};
	//
	static const mt_t  mt = mt_t();
	static const cm_t	cm = cm_t();
	static const mm_t	mm = mm_t();
	
	
	///
	template  <typename T=double>
	class basic_metric_coord_t 
	{
public:
	///
	typedef T value_type;
	
	///ctor default
	basic_metric_coord_t() : val_(0) {}
	///
	basic_metric_coord_t(mt_t, T _val) 
	: val_(_val) {}
	///
	basic_metric_coord_t(cm_t, T _val) 
	: val_( cvt<T>::cm2mt(_val) ) {}
	///
	basic_metric_coord_t(mm_t, T _val) 
	: val_( cvt <T>::mm2mt(_val) ) {}
	///
	basic_metric_coord_t(const basic_metric_coord_t<T>& other) 
	: val_( other.mt() ) {}	
	
	
	//setters and getters
public:
	///setter
	void mt  (T _val)   { val_ = _val;}
	void cm (T _val)   { val_ = cvt<T>::cm2mt(_val) ;}
	void mm (T _val)   { val_ = cvt<T>::mm2mt(_val) ;}
	
	///getter
	T mt() const {return val_;}
	///
	T cm() const {return cvt<T>::mt2cm(val_); }
	///
	T mm() const {return cvt<T>::mt2mm(val_); }
	
	//operation overloads
public:
	///op += overload
	basic_metric_coord_t<T>& operator += (const basic_metric_coord_t<T>& other)
	{
		val_ += other.mt();return *this;
	}
	
	///op = overload
	basic_metric_coord_t<T>& operator = (const basic_metric_coord_t<T>& other)
	{
		val_ = other.mt(); return *this;
	}
	//[member data]
private:
	T val_;
};

//basic_metric2d_t
template <typename T=double>
class basic_metric2d_t
{
public:
///
typedef T value_type;
//ctor
inline basic_metric2d_t()
:x_(mt, 0.0),y_(mt, 0.0) {}

///ctor meters
inline basic_metric2d_t(mt_t, T _x, T _y)
:x_(mt, _x), y_(mt, _y){}

///ctor centimeters
inline basic_metric2d_t(cm_t, T _x, T _y)
:x_(cm, _x ), y_(cm, _y ) {}

///ctor millimeters
inline basic_metric2d_t(mm_t, T _x, T _y)
:x_(mm, _x),  y_(mm, _y) {}

///ctor copy
inline basic_metric2d_t(const basic_metric2d_t<T>& other)
:x_(mt, other.x(mt)),  y_(mt, other.y(mt)) {}

///
inline basic_metric_coord_t<T>& x()   {return x_;}
///
inline basic_metric_coord_t<T>& y()   {return y_;}	

///
inline const basic_metric_coord_t<T>& x() const   {return x_;}
///
inline const basic_metric_coord_t<T>& y()   const {return y_;}	

//
inline void xy(mt_t, T _x, T _y) { x_.mt(_x); y_.mt(_y); };
//
inline void xy(cm_t, T _x, T _y) { x_.cm(_x); y_.cm(_y); };
//
inline void xy(mm_t, T _x, T _y) { x_.mm(_x); y_.mm(_y); };

//getters overloads
inline T x(mt_t) const {return x_.mt();}
inline T x(cm_t) const {return x_.cm(); }
inline T x(mm_t) const {return x_.mm(); }

inline T y(mt_t) const {return y_.mt();}
inline T y(cm_t) const {return y_.cm(); }
inline T y(mm_t) const {return y_.mm(); }

//operation overloads
public:
///
basic_metric2d_t<T>& operator +=(const basic_metric2d_t<T>& other)
{
	x_ += other.x(); y_ += other.y(); return *this;
}

///copy overload
basic_metric2d_t<T>& operator= (const basic_metric2d_t<T>& other)
{
	x_ = other.x(); y_ = other.y(); return *this;
}


private:
///
basic_metric_coord_t<T> x_;
///
basic_metric_coord_t<T> y_;
};



///op+
template <class T>
inline basic_metric2d_t<T> operator+ (const basic_metric2d_t<T>& p1, const basic_metric2d_t<T>& p2){
	return basic_metric2d_t<T>(mt, p1.x(mt) + p2.x(mt), p1.y(mt) + p2.y(mt));
}

///op-
template <class T>
inline basic_metric2d_t<T> operator- (const basic_metric2d_t<T>& p1, const basic_metric2d_t<T>& p2){
	return basic_metric2d_t<T>(mt, p1.x(mt) - p2.x(mt), p1.y(mt) - p2.y(mt));
}

///
template <class T>
inline basic_metric2d_t<T> operator * (const basic_metric2d_t<T>& p, const T& v){
	return basic_metric2d_t<T>(mt, p.x(mt)*v, p.y(mt)*v);
}

///
template <class T>
inline basic_metric2d_t<T> operator * (const T& v, const basic_metric2d_t<T>& p){
	return basic_metric2d_t<T>(mt, p.x(mt)*v, p.y(mt)*v);
}

///?
template <class T>
inline T operator * (const basic_metric2d_t<T>& p1, const basic_metric2d_t<T>& p2){
	return p1.x(mt)*p2.x(mt)+p1.y(mt)*p2.y(mt);
}

//_________________________________________________________________
///
typedef basic_metric2d_t     < >  metric2d_t;
///
typedef basic_metric2d_t <double>  point2d_t;
typedef std::vector < basic_metric2d_t <double> >  point2d_vec;

///
typedef basic_metric_coord_t < >  metric_coord_t;

///
typedef basic_metric_coord_t < double >  range_t;
typedef std::vector < basic_metric_coord_t < double > > range_vec;
//_________________________________________________________________
}//close namespace uor

///
template <typename T>
inline T euclidian_dist(const uor::basic_metric2d_t<T>& p1, const uor::basic_metric2d_t<T>& p2)
{
#if defined (WIN32) || defined(_WIN32_)
	return ::_hypot(p1.x(uor::mt) - p2.x(uor::mt), p1.y(uor::mt)-p2.y(uor::mt));
#else
	return ::hypot(p1.x(uor::mt) - p2.x(uor::mt), p1.y(uor::mt)-p2.y(uor::mt));
#endif
}


///
template <typename T>
inline std::ostream&	operator<<(std::ostream& os, const uor::basic_metric2d_t<T>& temp)
{
	os << "X: " 
	<< temp.x(uor::mt)  
	<< " Y: " << temp.y(uor::mt) 
	<< " meters"; 
	return os;
}

///
template <typename T>
inline std::ostream&	operator<<(std::ostream& os, const uor::basic_metric_coord_t<T> & source)
{
	os 
	<< "Range: " 
	<<  source.mt() << " meters"
	<< std::endl;
	
	return os;
}

#endif //UOR_POSITION_T_INCLUDED__