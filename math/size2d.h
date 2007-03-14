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
			size2d():row_(0),col_(0){}
			size2d(size_t a):row_(a),col_(a){}
			size2d(size_t a, size_t b):row_(a),col_(b){}

public: //data stored as a pair of size_t
	size_t	row_;
	size_t	col_;

public:	//friends
	friend	std::ostream&	operator<<(std::ostream& os, size2d temp);
	friend	std::istream&	operator>>(std::istream& is, size2d& temp);
	friend	bool operator==(const size2d& a, const size2d& b);

};//class size2d

inline	std::ostream&	operator<<(std::ostream& os, size2d temp)
{
	os<< temp.row_ << " "<< temp.col_;
	return os;
}

inline	std::istream&	operator>>(std::istream& is, size2d& temp)
{
	is >> temp.row_ >> temp.col_; 
	return is;
}

bool inline	operator==(const size2d& a, const size2d& b)
{
	return (a.row_ == b.row_) && (a.col_ == b.col_);
}

const size_t size_max = static_cast<size_t>(-1);
const size2d size2d_max(size_max,size_max);

}//namespace math
}//namespace all
//-----------------------------------------------------------------------------------------------
#endif //size2d_H
