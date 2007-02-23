#ifndef distance_H
#define distance_H

/**
 *	file distance.h
 *	this header defines some metric function over geometric features such point, pose, rect, line, etc...
 */
#include <cmath>
#include "line2d.h"
#include "point2d.h"
#include "rect2d.h"

namespace all
{
namespace math
{

//double	distance(const point2d& a, const rect2d& b);

/**
 *	distance between two points: sqrt((x1-x2)^2 + (y1-y2)^2)
 */
inline double	distance(const point2d& a, const point2d& b)
{
	return (a-b).magnitude();
}

/**
 *	distance between a point and a rect: (a*x0 + b*y0 + c)/(a^2 + b^2)
 *		where the rect is (a,b,c) and the point is (x0,y0)
 *	( to do: there's a problem using metric... the "c" coefficient should be a metric to make nemerator meaningful )
 */
inline double	distance(const point2d& a, const rect2d& b)
{
	double aa = b.getA();
	double bb = b.getB();
	double cc = b.getC();
	double num = abs( aa*a.getX() + bb*a.getY() + cc);
	double den = sqrt(aa*aa + bb*bb);
	if(!den)
		throw std::runtime_error("division by ZERO in all::distance(const point2d&, const rect2d&) *_*");

	return (num/den);	
}

/**
 *	same as before :)
 */
inline double	distance(const rect2d& b, const point2d& a)
{
	return distance(a,b);
}

/**	the distance between a point2d P and a line2d S(A,B) is:
 *	1)the distance between P and one of the two vertex of S:
 *		if the angle ABP or BAP are > 90 Degrees
 *	2)the distance between P and the rect passing in A and B
 *		if both ABP and BAP are < 90 Degrees
 */
inline double	distance(const point2d& a, const line2d& b)
{
	point2d b0(b.getP0());
	point2d b1(b.getP1());
	//dot product indicates if the angle between a and the vertex of b is < or > of 90 degrees
	if( dotproduct( (b1-b0) , (a-b0) ) < 0)
		return distance(a, b0);
	if( dotproduct( (b0-b1) , (a-b1) ) < 0)
		return distance(a, b1);
	return distance(a, rect2d(b));		
}

/**
 *	same as before :)
 */
inline double	distance(const line2d& b, const point2d& a)
{
	return distance(a,b);
}


}// namespace math
}// namespace all

#endif //distance_H