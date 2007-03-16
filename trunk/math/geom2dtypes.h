#ifndef	geom2dtypes_H
#define	geom2dtypes_H
//---------------------------------------------------------------------------
#include <vector>
#include "point2d.h"
#include "pose2d.h"
#include "size2d.h"
//---------------------------------------------------------------------------
namespace all{
namespace math{

typedef std::vector<point2d>	        point2d_vect;
typedef point2d_vect::iterator			point2d_vect_it;
typedef point2d_vect::const_iterator	point2d_vect_cit;

typedef std::vector<pose2d>				pose2d_vect;
typedef	pose2d_vect::iterator			pose2d_vect_it;
typedef	pose2d_vect::const_iterator		pose2d_vect_cit;

typedef std::pair<angle,angle>			angle_pair;
typedef std::vector<angle_pair>			angle_pair_vect;
typedef angle_pair_vect::iterator		angle_pair_vect_it;	

typedef std::vector<size2d>				sizes2d;
typedef sizes2d::iterator				sizes2d_it;

}//namespace math
}//namespace all
//---------------------------------------------------------------------------
#endif //geom2dtypes_H