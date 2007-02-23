#ifndef	segment2d_H
#define	segment2d_H

#include "point2d.h"

namespace all
{
namespace math
{

/**
 *	class line2d:
 *	a segment in two dimension is a pair of points...
 *	this class is also a base class for class segmentOriented2d...
 */

class line2d
{
public:	//ctors
	line2d():p0_(0,0),p1_(0,0){}
	line2d(const point2d& start, const point2d& end):p0_(start),p1_(end){}

public:	//setters and getters
	const point2d	getP0() const {return p0_;}
	const point2d	getP1() const {return p1_;}
	void			setP0(const point2d& temp) {p0_ = temp;}
	void			setP1(const point2d& temp) {p1_ = temp;}

public:	//tools
	/**
	 *	this function returns the lenght of the segment...
	 */
	double	lenght() const {return (p1_ - p0_).magnitude();}

	/**
	 *	this functions returns the orientation of the segment...
	 *	be careful: 
	 *		orientation of a NOT ORIENTED segment (as *this is...) is always between -90 and +90
	 *		while orientation of a ORIENTED segment is between -180 and +180
	 */
	virtual const angle	orientation() const 
	{
		angle temp = (p1_ - p0_).orientation();
    if( (temp<angle(90, math::deg_tag) )&& (temp>angle(-90, math::deg_tag)) )
			return temp;
    return temp + angle(180, math::deg_tag);
	}

/*	da rivedere... rendere esterni, rinominare refactorizzare (generic, usare gli iterator invece che tutti sti overload)

	//	Il segmento risultante sarà la diagonale di un rettangolo p0(min-min)--p1(max-max) che contiene
	//	tutti i punti del vettore.
	//	il parametro \a originIncluded, se \a true, indica se va incluso l'origine (0,0)
	void		PointRegion(std::vector<point2d> *vect, bool originIncluded=false);///<Restituisce il più piccolo "rettangolo" che contiene tutti i punti
	void		PointRegion(std::list<point2d> *list, bool originIncluded=false);///<Restituisce il più piccolo "rettangolo" che contiene tutti i punti
	void		PointRegion(std::list<pose2d> *list, bool originIncluded=false);///<Restituisce il più piccolo "rettangolo" che contiene tutti i punti
	//bool		operator/(line2d&) const;//no buono... l'intersezione la facciamo esterna alla classe ;) e non la si chiama operator/
*/

public:	//friends
	friend	std::ostream&	operator<<(std::ostream& os, const line2d& temp);
	friend	std::istream&	operator>>(std::istream& is, line2d& temp);

private:	//data stored as a pair of point2d
	point2d		p0_;
	point2d		p1_;

};//class line2d
inline std::ostream&	operator<<(std::ostream& os, const line2d& temp)
{
	os<< temp.p0_ << " "<< temp.p1_;
	return os;
}

inline std::istream&	operator>>(std::istream& is, line2d& temp)
{
	is >> temp.p0_ >> temp.p1_; 
	return is;
}

}//namespace math
}//namespace all

#endif //segment2d_H