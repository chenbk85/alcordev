#ifndef	size2d_H
#define	size2d_H
//-----------------------------------------------------------------------------------------------
#include<iostream>
//-----------------------------------------------------------------------------------------------
namespace all{
namespace math{

/**
 *	class size2d
 *	a useful class for pair of size_t objects... for example a couple of matrix indices!
 *	(todo: maybe std::pair<size_t,size_t> can do it better... )
 */
class size2d
{
public:	//ctors
			size2d():x1_(0),x2_(0){}
			size2d(size_t a):x1_(a),x2_(a){}
			size2d(size_t a, size_t b):x1_(a),x2_(b){}

public:	//setters and getters
	size_t	get_x1() const {return x1_;}
	size_t	get_x2() const {return x2_;}
	void	set_x1(size_t	temp){x1_ = temp;}
	void	set_x2(size_t	temp){x2_ = temp;}

public:	//friends
	friend	std::ostream&	operator<<(std::ostream& os, size2d temp);
	friend	std::istream&	operator>>(std::istream& is, size2d& temp);
	friend	bool operator==(const size2d& a, const size2d& b);

private: //data stored as a pair of size_t
	size_t	x1_;
	size_t	x2_;

};//class size2d

inline	std::ostream&	operator<<(std::ostream& os, size2d temp)
{
	os<< temp.x1_ << " "<< temp.x2_;
	return os;
}

inline	std::istream&	operator>>(std::istream& is, size2d& temp)
{
	is >> temp.x1_ >> temp.x2_; 
	return is;
}

bool inline	operator==(const size2d& a, const size2d& b)
{
	return (a.x1_ == b.x1_) && (a.x2_ == b.x2_);
}

const size_t size_max = static_cast<size_t>(-1);
const size2d size2d_max(size_max,size_max);

}//namespace math
}//namespace all
//-----------------------------------------------------------------------------------------------
#endif //size2d_H
