#ifndef	size2d_H
#define	size2d_H

#include<iostream>

namespace all
{
namespace math
{

/**
 *	class size2d
 *	a useful class for pair of size_t objects... for example a couple of matrix indices!
 *	(todo: maybe std::pair<size_t,size_t> can do it better... )
 */
class size2d
{
public:	//ctors
			size2d():x_(0),y_(0){}
			size2d(size_t a):x_(a),y_(a){}
			size2d(size_t a, size_t b):x_(a),y_(b){}

public:	//setters and getters
	size_t	getX() const {return x_;}
	size_t	getY() const {return y_;}
	void	setX(size_t	temp){x_ = temp;}
	void	setY(size_t	temp){y_ = temp;}

public:	//friends
	friend	std::ostream&	operator<<(std::ostream& os, size2d temp);
	friend	std::istream&	operator>>(std::istream& is, size2d& temp);

private: //data stored as a pair of size_t
	size_t	x_;
	size_t	y_;

};//class size2d

inline	std::ostream&	operator<<(std::ostream& os, size2d temp)
{
	os<< temp.x_ << " "<< temp.y_;
	return os;
}

inline	std::istream&	operator>>(std::istream& is, size2d& temp)
{
	is >> temp.x_ >> temp.y_; 
	return is;
}

}//namespace math
}//namespace all

#endif //size2d_H
