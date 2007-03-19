#include "splam_data.h"
#include "detail/value_iteration.h"
//-----------------------------------------------------------------------------------------------
using namespace all::util;
//-----------------------------------------------------------------------------------------------
namespace all{
namespace splam{

void	splam_data::metric_goal_finding()
{
	//// STEP 1: STRUCTURE INITIALIZATION

	// variables definitions and initializations
	value_iteration vi(og_row_, og_col_, static_cast<int>(og_resolution_*100.0));
	size2d curr_coord = get_current_coord();
	size2d sizee = vi.get_size();
	size_t i,j;
	int temp;

	// primo passaggio, serve per porre a zero le zone unknown
	for(i=0;i<sizee.row_;++i)
	{
		for(j=0;j<sizee.col_;++j)
		{
			temp = og_cells_[i*sizee.col_+j];
			if(temp == 0)
				vi.set_row_col(i,j,0,0);
			else
				vi.set_row_col(i,j,100000000,(temp+128));
		}
	}

	// secondo passaggio, serve per porre a infinito i muri e le zone troppo vicine ai muri
	for(i=0;i<sizee.row_;++i)
		for(j=0;j<sizee.col_;++j)
			if(og_cells_[i*sizee.col_+j]>0)
				vi.set_untouch_area(i,j,static_cast<int>(0.30/og_resolution_));

	// ultima condizione: il robot non può trovarsi in aree inaccessibili
	for(i=curr_coord.row_-1;i<=curr_coord.row_+1;++i)
		for(j=curr_coord.col_-1;j<=curr_coord.col_+1;++j)
			if(in_map(i,j))
				vi.set_row_col(i,j,100000000,(static_cast<int>(og_cells_[i*sizee.col_+j])+128));

	//// STEP 2: LOOP

	// loop
	value_iteration_info info=vi.loop();

	//// STEP 3: CALCULATE UNEXPLORED PATH

	// structures setting
	value_iteration_path path;
	goal_.reset();

	if(!vi.get_path(get_current_coord(),path))
	{
		if(!path_.empty())
			goal_.goal_far_ = path_.back();
		goal_.path_.push_back(goal_.goal_far_.getP());
	}else{
		for(value_iteration_path_it it=path.begin(); it!=path.end();++it)
			goal_.path_.push_back(get_position_of(*it));
		if(!goal_.path_.empty())
		{
			goal_.goal_far_ = pose2d(goal_.path_.back(),angle(0.0,rad_tag));
		}else{
			goal_.goal_far_ = get_current_position();
			goal_.path_.push_back(goal_.goal_far_.getP());
		}
	}

	size_t sizzo = static_cast<size_t>(1.0/og_resolution_);
	if(goal_.path_.size()<sizzo)
		goal_.goal_near_ = goal_.goal_far_;
	else
		goal_.goal_near_ = pose2d(goal_.path_.at(sizzo-1),angle(0.0,deg_tag));

	goal_.is_valid_=true;
	goal_.recognition_ = false;
	goal_.head_direction_ = (goal_.goal_near_.getP()-get_current_position().getP()).orientation();
	goal_.relative_head_direction_ = goal_.head_direction_ - get_current_position().getTh();
}


}//namespace splam
}//namespace all
