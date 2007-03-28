#include <fstream>
#include "splam_data.h"
#include "detail/value_iteration.h"
#include "alcor/core/iniwrapper.h"
//-----------------------------------------------------------------------------------------------
using namespace all::util;
//-----------------------------------------------------------------------------------------------
namespace all{
namespace splam{

splam_data::splam_data(const char* name)
{
	iniWrapper ini(name);
	og_row_ = ini.GetInt("mappa:larghezza",0);
	og_col_ = ini.GetInt("mappa:altezza",0);
	og_resolution_ = ini.GetDouble("mappa:dim_cella",0.0);
}


void	splam_data::save_og_pgm(const char* filename) const
{
	std::ofstream file(filename,std::ios::binary);
	//FILE *file;
	//file = fopen(filename, "w+");

	//fprintf(file, "P5 %d %d 255\n", og_row_, og_col_);
	file << "P5 "<<og_row_<<" "<<og_col_<<" 255\n";

	for (int i = 0; i < og_col_*og_row_; i++)
	{
		unsigned char d = static_cast<unsigned char>( 127 - og_cells_.at(i));
		//fwrite(&d, 1, 1,  file);
		file<<d;
	}

	//fclose(file);
}

void	splam_data::save_sg_pgm(const char* filename) const
{
	std::ofstream file(filename,std::ios::binary);
	file << "P5 "<<og_row_<<" "<<og_col_<<" 255\n";
	for (int i = 0; i < og_col_*og_row_; i++)
	{
		unsigned char d = static_cast<unsigned char>( 127 - og_cells_.at(i));
		file<<d;
	}
}

void	splam_data::metric_goal_finding()
{
	//// STEP 1: STRUCTURE INITIALIZATION

	//std::cout<< "splam_data::metric_goal_finding() .......  FASE 1"<<std::endl;
	// variables definitions and initializations
	static value_iteration vi(og_row_, og_col_, static_cast<int>(og_resolution_*100.0));
	size2d curr_coord = get_current_coord();
	size2d sizee = vi.get_size();
	size_t i,j;
	int temp;

	//std::cout<< "splam_data::metric_goal_finding() .......  FASE 2"<<std::endl;
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

	//std::cout<< "splam_data::metric_goal_finding() .......  FASE 3"<<std::endl;
	// secondo passaggio, serve per porre a infinito i muri e le zone troppo vicine ai muri
	for(i=0;i<sizee.row_;++i)
		for(j=0;j<sizee.col_;++j)
			if(og_cells_[i*sizee.col_+j]>0)
				vi.set_untouch_area(i,j,static_cast<int>(0.30/og_resolution_));

	//std::cout<< "splam_data::metric_goal_finding() .......  FASE 4"<<std::endl;
	// ultima condizione: il robot non può trovarsi in aree inaccessibili
	for(i=curr_coord.row_-1;i<=curr_coord.row_+1;++i)
		for(j=curr_coord.col_-1;j<=curr_coord.col_+1;++j)
			if(in_map(i,j))
				vi.set_searchable(i,j,(static_cast<int>(og_cells_[i*sizee.col_+j])+128));

	//// STEP 2: LOOP

	//std::cout<< "splam_data::metric_goal_finding() .......  FASE 5"<<std::endl;
	// loop
	value_iteration_info info=vi.loop();

	//// STEP 3: CALCULATE UNEXPLORED PATH

	//std::cout<< "splam_data::metric_goal_finding() .......  FASE 6"<<std::endl;
	// structures setting
	sizes2d path;
	goal_.reset();

	if(!vi.get_path(curr_coord,path))
	{
		if(!path_.empty())
			goal_.goal_far_ = path_.back();
		goal_.path_.push_back(goal_.goal_far_.getP());
	}else{
		for(sizes2d_it it=path.begin(); it!=path.end();++it)
			goal_.path_.push_back(get_position_of(*it));
		if(!goal_.path_.empty())
		{
			goal_.goal_far_ = pose2d(goal_.path_.back(),angle(0.0,rad_tag));
		}else{
			goal_.goal_far_ = get_current_position();
			goal_.path_.push_back(goal_.goal_far_.getP());
		}
	}

	//std::cout<< "splam_data::metric_goal_finding() .......  FASE 7"<<std::endl;
	size_t sizzo = static_cast<size_t>(1.0/og_resolution_);
	if(goal_.path_.size()<sizzo)
		goal_.goal_near_ = goal_.goal_far_;
	else
		goal_.goal_near_ = pose2d(goal_.path_.at(sizzo-1),angle(0.0,deg_tag));

	goal_.is_valid_=true;
	goal_.recognition_ = false;
	goal_.head_direction_ = (goal_.goal_near_.getP()-get_current_position().getP()).orientation();
	goal_.relative_head_direction_ = goal_.head_direction_ - get_current_position().getTh();

	//std::cout<< "splam_data::metric_goal_finding() .......  FASE 8"<<std::endl;
	//// STEP 4: LOG DATA ON FILE
	std::ostringstream namefile;
	namefile << "metric_goal_find" << path_.size() << ".txt";
	std::ofstream filelog(namefile.str().c_str(), std::ios::out);
	filelog << "goal near: "<< goal_.goal_near_ << std::endl;
	filelog << "goal near coord: "<< get_coord_of(goal_.goal_near_.getP()) << std::endl;
	filelog << "goal far: "<< goal_.goal_far_ << std::endl;
	filelog << "goal far coord: "<< get_coord_of(goal_.goal_far_.getP()) << std::endl<< std::endl;
	filelog << "current position: "<< get_current_position() << std::endl;
	filelog << "current coord: "<< get_current_coord() << " - Value: "<< vi.get_row_col(get_current_coord()).value<< std::endl<< std::endl;

	filelog << "vi path size: "<< path.size() << std::endl;
	filelog << "vi path: " << std::endl;
	for(i=0;i<path.size();i++)
		filelog << i << " - Coord: "<<path[i]<<" - Position: "<< get_position_of(path[i]) << " - Value: "<< vi.get_row_col(path[i]).value<< std::endl;

	filelog << std::endl <<"path size: "<< path_.size() << std::endl;
	filelog << "path: "<< std::endl;
	i=0;
	for(poses2d_it it =path_.begin(); it!= path_.end(); ++it)
		filelog << i++ << " - Coord: "<< *it << std::endl;

	//// log Value data
	//for(i=0;i<sizee.row_;++i)
	//{
	//	for(j=0;j<sizee.col_;++j)
	//		filelog << vi.get_row_col(i,j).value << " ";
	//	filelog << std::endl;
	//}
	//std::cout<< "splam_data::metric_goal_finding() .......  FASE 9"<<std::endl;
}


}//namespace splam
}//namespace all
