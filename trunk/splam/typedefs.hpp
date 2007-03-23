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

typedef	unsigned char				pixel_value;
typedef std::vector<pixel_value>	pixel_values;
typedef pixel_values::iterator		pixel_values_it;

typedef std::vector<int>			int_vect;
typedef int_vect::iterator			int_vect_it;

typedef std::vector<double>			double_vect;
typedef double_vect::iterator		double_vect_it;

typedef std::pair<double,double>	double_pair;
typedef std::vector<double_pair>	double_pair_vect;
typedef double_pair_vect::iterator	double_pair_vect_it;

typedef std::pair<int,int>			int_pair;
typedef std::vector<int_pair>		int_pair_vect;
typedef int_pair_vect::iterator		int_pair_vect_it;

}//namespace util
}//namespace all
//---------------------------------------------------------------------------
#endif