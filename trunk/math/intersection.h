#ifndef intersection_H
#define intersection_H

/**
 *	file intersection.h
 *	this header defines some function over geometric features such point, pose, rect, line, etc...
 *	all functions returns a vector of point2d... so you can test if there's no intersectionn!!
 *	and, of course, maybe some geom features intersect more than once (es: when there are polygons...)
 */

#include "geom2dtypes.h"
#include "line2d.h"
#include "rect2d.h"

namespace all
{
namespace math
{

/**
 *	intersection between two rect:
 *	r: ax + by + c = 0
 *	s: Ax + By + C = 0
 *
 *	p = ... result of that system of equation :D
 */
inline point2dVect		intersection(const rect2d& a, const rect2d& b)
{
	point2dVect ret;

	//check for equation validity
	if((!a.isValid()) || (!b.isValid()))
		return ret;

	double a1 = a.getA();
	double b1 = a.getB();
	double c1 = a.getC();
	double a2 = b.getA();
	double b2 = b.getB();
	double c2 = b.getC();

	//check for parallelism
	double den = a1*b2 - b1*a2;
	if(!den)
		return ret;

	point2d p(-c1*b2 + c2*b1 , -c2*a1 + c1*a2);
	p/=den;

	ret.push_back(p);
	return ret;
}

/**
 *	intersection between a rect and a line:
 *	there's an intersection if rect function evaluated on line vertex change in sign.
 *	that point of intersection is the intersection between the original rect and the rect defined by the line.
 */
inline point2dVect		intersection(const rect2d& a, const line2d& b)
{
	point2dVect ret;
	if(a.evaluate(b.getP0()) * a.evaluate(b.getP1()) > 0.0)
		return ret;
	return intersection(a, rect2d(b));
}

/**
 *	same as before...
 */
inline point2dVect		intersection(const line2d& b, const rect2d& a)
{
	return	intersection(a,b);
}

/**	
 *	intersection between two lines:
 *	let's define two rect2d from the lines.
 *	there is an intersection if each line intersect the rect defined by the other line
 *	PROBLEM: what happens if a == b ?? O_O ... in current impl function returns an empty vect...
 *	PROBLEM: what happens if the lines are collinear ?? and if they partially overlap??
 *	(to do: this solution can be optimized using algorithm described in GRAPHICS GEMS 3 pp 199-202)
 */
inline point2dVect		intersection(const line2d& a, const line2d& b)
{
	point2dVect ret;
	if( intersection(rect2d(a),b).empty() )
		return ret;
	return intersection(rect2d(b),a);
}

}// namespace math
}// namespace all

#endif //intersection_H	

