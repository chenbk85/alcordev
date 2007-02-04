#ifndef	rect2d_H
#define	rect2d_H

#include "angle.h"
#include "point2d.h"
#include "line2d.h"

namespace all
{
namespace math
{

/**	
 *	class rect2d:
 *	rect equation: ax+by+c=0
 */

class rect2d
{
public:	//ctors
	/**
	 *	default ctor: (a,b,c) = (0,1,0)  ==>  y=0  ==>  x axis!! 
	 */
	rect2d():a_(0.0),b_(1.0),c_(0.0){}

	/**
	 *	rect2d(double,double) means y = mx+q ==> mx -y +q = 0 ==> (a,b,c) = (m,-1,q)
	 */
	rect2d(double m, double q):a_(m),b_(-1.0),c_(q){}

	/**
	 *	rect2d(double,double,double) means ax+by+c=0 :)
	 */
	rect2d(double a, double b, double c):a_(a),b_(b),c_(c){}

	/**
	 *	rect2d(point2d,point2d) means rect passing through 2 point... (y-y1)*(x2-x1) = (x-x1)*(y2-y1) ...
	 */
	rect2d(const point2d& p0, const point2d& p1)
		:a_((p1.getY()-p0.getY()))
		,b_((p0.getX()-p1.getX()))
		,c_(p1.getX()*p0.getY()-p1.getY()*p0.getX())
	{}
	
	/**
	 *	rect2d(point2d,angle) means rect passing through p with direction a ...
	 */
	rect2d(const point2d& p, const angle& a)
		:a_(-a.sin())
		,b_(a.cos())
		,c_(p.getX()*a.sin() - p.getY()*a.cos())
	{}

	/**
	 *	rect2d(segment) means rect passing through the two vertex of the segment s
	 *	( to do ... could be optimized... a lot of temporaries...)
	 */
	explicit rect2d(const line2d& s){(*this)=rect2d(s.getP0(),s.getP1());}

public:	//setters and getters
	double	getA() const {return a_;}
	double	getB() const {return b_;}
	double	getC() const {return c_;}
	void	setA(double temp) {a_ = temp;}
	void	setB(double temp) {b_ = temp;}
	void	setC(double temp) {c_ = temp;}

public:	//tools
	/**
	 *	functions that evaluate the linear function in a particular point (a*x0+b*y0+c)
	 *	invariant:
	 *		r.evaluate(x0,y0) == 0 <==> (x0,y0) in r
	 */
	double	evaluate(double x0, double y0) const{return a_*x0 + b_*y0 + c_;}
	double	evaluate(const point2d& p) const {return evaluate(p.getX(),p.getY());}
	bool	isValid() const {return ( (a_!=0.0) || (b_!=0.0) );}


public:	//friends
	friend	std::ostream&	operator<<(std::ostream& os, const rect2d& temp);
	friend	std::istream&	operator>>(std::istream& is, rect2d& temp);
	//friend	const point2d	intersection(const rect2d&, const rect2d&);

private:	//data stored ad a set of three doubles
	double	a_;
	double	b_;
	double	c_;

};//class rect2d

inline std::ostream&	operator<<(std::ostream& os, const rect2d& temp)
{
	os<< temp.a_ <<" "<< temp.b_ << " "<< temp.c_;
	return os;
}

inline std::istream&	operator>>(std::istream& is, rect2d& temp)
{
	is >> temp.a_>>temp.b_>> temp.c_; 
	return is;
}

}//namespace math
}//namespace all

#endif //rect2d_H