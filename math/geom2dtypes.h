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

typedef std::vector<point2d>		points2d;
typedef points2d::iterator			points2d_it;
typedef points2d::const_iterator	points2d_cit;

typedef std::vector<pose2d>			poses2d;
typedef	poses2d::iterator			poses2d_it;
typedef	poses2d::const_iterator		poses2d_cit;

typedef std::pair<angle,angle>		angle_pair;
typedef std::vector<angle_pair>		angle_pair_vect;
typedef angle_pair_vect::iterator	angle_pair_vect_it;	

typedef std::vector<size2d>			sizes2d;
typedef sizes2d::iterator			sizes2d_it;

}//namespace math
}//namespace all
//---------------------------------------------------------------------------
#endif //geom2dtypes_H