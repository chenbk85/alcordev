#ifndef	geom2dtypes_H
#define	geom2dtypes_H

/**
 *	file geom2dtypes.h
 *	this file contains the definitions of some common types
 */

#include <vector>
#include "point2d.h"
#include "pose2d.h"

namespace all
{
namespace math
{

typedef std::vector<point2d>	point2dVect;
typedef std::vector<pose2d>		pose2dVect;

typedef point2dVect::iterator			point2dVectIt;
typedef point2dVect::const_iterator		point2dVectCit;

typedef	pose2dVect::iterator			pose2dVectIt;
typedef	pose2dVect::const_iterator		pose2dVectCit;

}//namespace math
}//namespace all

#endif //geom2dtypes_H