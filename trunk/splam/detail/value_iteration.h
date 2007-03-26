#ifndef ValueIterationH
#define ValueIterationH
//--------------------------------------------------------------------------------
#include <vector>
#include "alcor/math/size2d.h"
#include "alcor/math/geom2dtypes.h"
//--------------------------------------------------------------------------------
using namespace all::math;
//---------------------------------------------------------------------------
namespace all{
namespace util{

struct	value_iteration_info
{
	int		nStep;			///< step before convergence
	long	mSecElapsed;	///< time elapsed
	bool	noProblems;		///< if data_ is ok
};

struct	value_iteration_node
{
	int	value;
	int	cost;
};

class	value_iteration
{
public:	
	value_iteration();
	value_iteration(size_t,size_t,int);
	~value_iteration();

public:	
	bool						create(size_t,size_t,int);
	bool						create(size2d,int);
	bool						kill();

public:	
	inline size2d				get_size() const {return size_;}
	void						set_untouch_area(size_t,size_t,int,bool=false);
	void						set_searchable(size_t, size_t, int);

	inline bool					in_map(size_t,size_t) const;	///< true if (int,int) is inside the data
	inline bool					in_map(size2d temp)	const {return in_map(temp.row_,temp.col_);}
	value_iteration_node		get_row_col(size_t,size_t) const;	///< fast data access function
	value_iteration_node		get_row_col(size2d) const;
	inline void					set_row_col(size_t,size_t,int,int=0);	///< fast data access function
	bool						get_path(size2d, sizes2d&);

public:	
	value_iteration_info		loop();				///< value iteration loop... do all the things!

protected:
	bool						update(size_t,size_t);	///< single cell update

protected:
	value_iteration_node*		data_;
	int							dist_;
	size2d						size_;
	bool						loop_done_;
};

}//namespace util
}//namespace all
//--------------------------------------------------------------------------------
#endif

