#ifndef TYPEDEFS_H
#define TYPEDEFS_H
//---------------------------------------------------------------------------
#include <vector>
//---------------------------------------------------------------------------
namespace all{
namespace util{

typedef short int					scan_value;
typedef	std::vector<scan_value>		scan_values;
typedef	scan_values::iterator		scan_values_it;

typedef	signed char					map_value;
typedef std::vector<map_value>		map_values;
typedef map_values::iterator		map_values_it;

typedef std::vector<int>			int_vect;
typedef int_vect::iterator			int_vect_it;

typedef std::vector<double>			double_vect;
typedef double_vect::iterator		double_vect_it;

typedef std::pair<double,double>	double_pair;

}//namespace util
}//namespace all
//---------------------------------------------------------------------------
#endif