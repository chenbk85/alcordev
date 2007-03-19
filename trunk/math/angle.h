#ifndef	angle_H_INCLUDED
#define	angle_H_INCLUDED

#include <iostream>
#include <cmath>
#include "alcor/core/core.h"

using namespace all::core;

namespace all
{	
namespace math
{

///-------------------------------------------------------------
struct rad_t{};
struct deg_t{};
///-------------------------------------------------------------
static const rad_t rad_tag = rad_t();
static const deg_t deg_tag = deg_t();
///-------------------------------------------------------------
//fwd declaration
class angle;
const	inline angle	operator+(const angle& a, const angle& b);
const	inline angle	operator-(const angle& a, const angle& b);

/**
 *	class angle:
 *	no copy assignment, copy constructor e destructor... cause this is a value-like class
 *	automatic conversion FROM double (default meaning = degrees)
 *	construction and getters with Radians and Degrees
 */

class angle
{
public:	//ctors
	angle():data_(0.0){}
	angle(double temp, rad_t):data_(dconstants::rad_to_deg(temp)){fixAngle();}
	angle(double temp, deg_t):data_(temp){fixAngle();}

public:	//getters
	double	rad()		const	{return dconstants::deg_to_rad(data_);}
	double	deg()		const	{return data_;}
	angle	getAbs()	const	{return (data_ > 0 ? (*this) : -(*this));}
	void	set_rad (double temp)	{data_ =   dconstants::rad_to_deg(temp);fixAngle();}
	void	set_deg (double temp)	{data_ = temp;fixAngle();}
	void	reset(){data_=0.0;}

public:	//unary operators
	const	angle	operator-()	const {return angle(-data_, deg_tag);}

public: //binary operators
	angle&	operator+=(const angle& temp){data_+=temp.data_;fixAngle();return *this;}
	angle&	operator-=(const angle& temp){data_-=temp.data_;fixAngle();return *this;}
	angle&	operator*=(double temp){data_*=temp;fixAngle();return *this;}
	angle&	operator/=(double temp)
	{
		if(!temp)
			throw std::runtime_error("division by ZERO in all::math::angle::operator/=(double) *_*");
		data_/=temp;
		fixAngle();
		return *this;
	}	

public:	//trigonometric operations
	double	cos()	const	{return ::cos(dconstants::deg_to_rad(data_));}
	double	sin()	const	{return ::sin(dconstants::deg_to_rad(data_));}
	double	tan()	const
	{
		if(abs(data_)==90.0)
			throw std::runtime_error("tan of a 90 degrees angle in all::math::angle::tan() *_*");
		return ::tan(dconstants::deg_to_rad(data_));
	}

public:	//utils
	/**
	 *	fix the angles between -180 and 180 degrees
	 */
	void	fixAngle()
	{		
		while(data_>180.0)
			data_-=360.0;
		while(data_<=-180.0)
			data_+=360.0;
	}

	/**
	 *	returns the angle 90 - *this
	 */
	const angle	complementar() const
	{
		return angle(90, deg_tag) - *this;
	}

	/**
	 *	returns the angle 180 - *this
	 */
	const angle	supplementar() const
	{
		return angle(180, deg_tag) - *this;
	}

public:	//friends
	friend	std::ostream&	operator<<(std::ostream& os, const angle& temp);
	friend	std::istream&	operator>>(std::istream& is, angle& temp);

private:	//data_ stores angle in DEG
	double		data_;

};//class angle

inline std::ostream&	operator<<(std::ostream& os, const angle& temp)
{
	os<< temp.data_; 
	return os;
}

inline std::istream&	operator>>(std::istream& is, angle& temp)
{
	is >> temp.data_; 
	return is;
}

const inline angle	operator+(const angle& a, const angle& b)
{
	angle c = a;
	c += b;
	return c;
}

const inline angle	operator-(const angle& a, const angle& b)
{
	angle c = a;
	c -= b;
	return c;
}

const inline angle	operator*(const angle& a, double b)
{
	angle c = a;
	c *= b;
	return c;
}

const inline angle	operator*(double b, const angle& a)
{
	angle c = a;
	c *= b;
	return c;
}

const inline angle	operator/(const angle& a, double b)
{
	angle c = a;
	c /= b;
	return c;
}

bool	inline operator<(const angle& a, const angle& b){return a.deg() < b.deg();}
bool	inline operator<=(const angle& a, const angle& b){return a.deg() <= b.deg();}
bool	inline operator>(const angle& a, const angle& b){return a.deg() > b.deg();}
bool	inline operator>=(const angle& a, const angle& b){return a.deg() >= b.deg();}


}//namespace math
}//namespace all

#endif //angle_H