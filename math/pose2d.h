#ifndef	pose2d_H
#define	pose2d_H

#include <iostream>
#include "angle.h"
#include "point2d.h"

namespace all
{
namespace math
{

/**
 *	class pose2d:
 *	no copy assignment, copy constructor e destructor... cause this is a value-like class
 *	no automatic conversion FROM and TO any builtin...
 *	pose2d HAS A point2d (instead of IS A) 
 *	internal data are ALL::POINT2D and ALL::ANGLE... there has (to do) another version (pose2d_)
 *		that uses (double,double,angle) instead...
 *	operator += and -= are members
 *	operator + and - are not members
 *	operator /= and *= are members and we intend point /= (or *=) scalar value
 *	operator * is not member cause you can have point2d*double or vice versa
 *	operator / is member because you could have only point2d/double...
 */

class pose2d
{
public:	//ctors
	pose2d():p_(),theta_(){}
	explicit	pose2d(point2d q):p_(q),theta_(0.0, deg_tag){}
	pose2d(point2d q, angle a):p_(q),theta_(a){}
	pose2d(double a, double b, angle c):p_(a,b),theta_(c){}
	pose2d(double a, double b, double c, rad_t d):p_(a,b),theta_(c,d){}
	pose2d(double a, double b, double c, deg_t d):p_(a,b),theta_(c,d){}

public:	//binary operators
	// to do... do we need operator + and - for pose2d??
	//pose2d&			operator+=(const pose2d& temp){p_+=temp.p_; theta_+=temp.theta_; return *this;}
	//pose2d&			operator-=(const pose2d& temp){p_-=temp.p_; theta_-=temp.theta_; return *this;}

public:	//unary operators
	pose2d	operator-(){return pose2d(-p_,-theta_);}

public:	//setters and getters
	point2d	getP() const {return p_;}
	angle	getTh() const {return theta_;}
	double	getX() const {return p_.getX();}
	double	getY() const {return p_.getY();}

	double	get_x1() const {return p_.get_x1();}
	double	get_x2() const {return p_.get_x2();}
	double	get_th(deg_t) const {return theta_.deg();};
	double	get_th(rad_t) const {return theta_.rad();};

	void	setP(const point2d& temp){p_ = temp;}
	void	setTh(const angle& temp){theta_ = temp;}
	void	setX(const double& temp){p_.setX(temp);}
	void	setY(const double& temp){p_.setY(temp);}

	void	set_x1(double val)  {p_.set_x1(val); };
	void	set_x2(double val)  {p_.set_x2(val);};
	void	set_th(double val, deg_t) {theta_.set_deg(val);};
	void	set_th(double val, rad_t) {theta_.set_rad(val);};

public:	//tools
	/**
	 *	function overload
	 *	this function returns a pose2d representing a move forward of "delta".
	 *	invariant: 
	 *		distance(*this, this->move(K)) == K
	 *		(this->move(K)).theta_ = this->theta_
	 */
	pose2d	move(double delta) const {return pose2d( p_ + point2d(delta,theta_) , theta_);}

	/**
	 *	function overload
	 *	this function returns a pose2d representing a move of "deltaP" in relative direction "deltaTh"
	 *	invariant: 
	 *		distance(*this, this->move(K,H)) == K
	 *		distance(*this, this->move(K,0.0)) == distance(*this, this->move(K))
	 *		(this->move(K,H)).p_ - p_ == point2d(K,H)
	 *		(this->move(K,H)).theta_ = this->theta_
	 */
	pose2d	move(double deltaP, const angle& deltaTh) const {return pose2d( (rotate(deltaTh).move(deltaP)).p_ , theta_);}

	/**
	 *	function overload
	 *	this function change the *this object, moving forward by "delta"
	 *	returns a reference to the modified *this object
	 *	(to do) can be improved for efficiency ;)
	 */
	pose2d&			moveApply(double delta){*this = move(delta); return *this;}

	/**
	 *	function overload
	 *	this function change the *this object, moving of "deltaP" in relative direction "deltaTh"
	 *	returns a reference to the modified *this object
	 *	(to do) can be improved for efficiency ;)
	 */
	pose2d&			moveApply(double deltaP, const angle& deltaTh){*this = move(deltaP,deltaTh); return *this;}

	/**
	 *	this function returns a pose2d representing a rotation counterclockwise of "delta".
	 *	invariant: p_ remains the same
	 */
	pose2d	rotate(const angle& delta) const {return pose2d(p_,theta_+delta);}

	/**
	 *	this function change the *this object, rotating the pose by "delta"
	 *	returns a reference to the modified *this object
	 */
	pose2d&			rotateApply(const angle& delta){theta_ += delta;return *this;}

	/**
	 *	this function returns a point2d representing the position of the point "absolute" relative to the
	 *		frame defined by the *this object... 
	 *	a pose2d object can be a description of a 2D-Frame: p_ indicates the Origin and theta_ the xx' angle
	 */
  //mentre relative ipotizza che il point sia espresso 
  //  in coordinate del frame assoluto e che si vogliano le coordinate in un frame
  //centrato nella pose2d
	point2d	relative(const point2d& absolute) const {return (absolute - p_).rotate(-theta_);}

	/**
	 *	this function returns a point2d representing the absolute position of the point "relative", 
	 *		that's expressed in a local coordinate frame represented by the *this object
	 *	invariant: pose.relative(pose.absolute(point)) == point
	 *		(where pose is a pose2d and point is a point2d)
	 */

  //comunque... absolute ipotizza che il point sia espresso in un frame solidale 
  //  con la pose2d e che si vogliano le sue coordinate nel frame (0,0,0)
	point2d	absolute(const point2d& relative) const {return relative.rotate(theta_) + p_;}

public:	//friends
	friend	std::ostream&	operator<<(std::ostream& os, const pose2d& temp);
	friend	std::istream&	operator>>(std::istream& is, pose2d& temp);

private:	//data stored as a point and an angle
	point2d	p_;
	angle	theta_;

};//class pose2d

inline std::ostream&	operator<<(std::ostream& os, const pose2d& temp)
{
	os<< temp.p_ << " "<< temp.theta_;
	return os;
}

inline std::istream&	operator>>(std::istream& is, pose2d& temp)
{
	is >> temp.p_ >> temp.theta_; 
	return is;
}

}//namespace math
}//namespace all

#endif //pose2d_H