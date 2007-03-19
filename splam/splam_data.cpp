#include "splam_data.h"
#include "detail/value_iteration.h"
//-----------------------------------------------------------------------------------------------
using namespace all::util;
//-----------------------------------------------------------------------------------------------
namespace all{
namespace splam{

void	splam_data::metric_goal_finding(goal_t& goal_temp)
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
	goal_temp.goal_near_ = 0;
	goal_temp.goal_far_ = 0;
	if(!vi.get_path(get_current_coord(),path))
	{
	
	}
	
	todo...
}



void	splam_thread_impl::GoalFinding()
{


	//STEP 3: CALCULATE UNEXPLORED PATH
	if(vi.get_path(worker_.GetCurrentCoord(), path) == false)
	{
		if(splam_data_.path_.empty())
		{
			splam_data_.goal_.pos.x = 0;
			splam_data_.goal_.pos.y = 0;
			splam_data_.goal_.rot = 0;
		}else{
			splam_data_.goal_ = splam_data_.path_.back();
		}
		splam_data_.goalPath_.push_back(splam_data_.goal_);
	}else{
		ValueIterationPathIt it;
		for(it=path.begin(); it!=path.end(); ++it)
		{
			//tempazzo << it->first << " "<< it->second<< endl;
			splam_data_.goalPath_.push_back(worker_.GetPositionOf(*it));
		}
		//tempazzo<< endl << endl;

		if(!splam_data_.goalPath_.empty()){
			splam_data_.goal_ = splam_data_.goalPath_.back();
		}else{
			if(splam_data_.path_.empty())
			{
				splam_data_.goal_.pos.x = 0;
				splam_data_.goal_.pos.y = 0;
				splam_data_.goal_.rot = 0;
			}else{
				splam_data_.goal_ = splam_data_.path_.back();
			}
			splam_data_.goalPath_.push_back(splam_data_.goal_);
		}
	}
	size_t sizzo = (1.0/splam_data_.mapResolution_);
	if(splam_data_.goalPath_.size()<sizzo)
	{
		splam_data_.goalTemp_ = splam_data_.goal_;
	}else{
		splam_data_.goalTemp_ = splam_data_.goalPath_.at(sizzo-1);
	}
	//cout << "GOAL PATH CALCOLATO!!!!!!!!!!!!!!!"<<endl;
	//cout << "SIZE: "<< path.size()<<endl;
	//size2d curr_coord = worker_.GetCurrentCoord();
	//cout << "CURRENT COORD: X: "<< curr_coord.first << " Y: "<< curr_coord.second<<endl;
	//cout << "CURRENT POSITIONS: X: "<<worker_.GetPosition().pos.x << " Y: "<<worker_.GetPosition().pos.y<<endl;
	//cout << "CURRENT POSITION RECALCULATED: X: "<< worker_.GetPositionOf(curr_coord).pos.x<< " Y: "<<worker_.GetPositionOf(curr_coord).pos.y<<endl;

}



}//namespace splam
}//namespace all
