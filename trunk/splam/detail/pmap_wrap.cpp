#include "pmap_wrap.h"
#include "conversion.hpp"
#include "alcor/core/iniWrapper.h"
//-----------------------------------------------------------------------------------------------
using namespace all::math;
using namespace all::util;
//-----------------------------------------------------------------------------------------------
namespace all{
namespace splam{

pmap_wrap::pmap_wrap(const char* name)
{
	iniWrapper w(name);

	/** parametri contenuti nel file ini per allocare le mappe */

	laser_scan_number_= w.GetInt("laser:num_step",0);
	double r_max= w.GetDouble("laser:laser_scan_data_o_max",0.0);
	double r_res = w.GetDouble("mappa:dim_cella",0.0);
	int n_camp= w.GetInt("mappa:num_camp",0);
	double r_start = w.GetDouble("laser:start_angle",0.0);
	double r_step = w.GetDouble("laser:angle_step", 0.0);
	double larg = w.GetDouble("mappa:larghezza",0.0);
	double alt = w.GetDouble("mappa:altezza",0.0);

	/** Alloca la memoria per omap_, pmap_, lodo_*/

	pmap_ = pmap_alloc(laser_scan_number_, r_max, r_start, r_step, n_camp, larg*r_res, alt*r_res, r_res);
	lodo_ = lodo_alloc(laser_scan_number_, r_max, r_res, r_start, r_step);
	omap_ = omap_alloc(laser_scan_number_, r_max, r_start, r_step, larg*r_res, alt*r_res, r_res);
	laser_scan_data_ = new double[laser_scan_number_];

	offset_laser_pose_.set_x1(w.GetDouble("laser:offset_x",0.0));
	offset_laser_pose_.set_x2(w.GetDouble("laser:offset_y",0.0));
	offset_laser_pose_.set_th(w.GetDouble("laser:offset_th",0.0),deg_tag);
}

/*
Il distruttore della classe pensa a deallocare la memoria di lodo_, pmap_ e omap_*/

pmap_wrap::~pmap_wrap()
{
	lodo_free(lodo_);
	pmap_free(pmap_);
	omap_free(omap_);
	delete[] laser_scan_data_;
}

/*
La funzione GetPosition()restituisce la posizione del campione che in quel momento risulta migliore,
per fare ciò indicizza il puntatore alla struttura dei campioni di pmap_ sul campione miglire e dopo 
copia la posizione corrispondente nella variabile privata di pmap_wrap corrpose.
*/
pose2d pmap_wrap::get_current_position() const
{
	pmap_sample_t* campione=PMAP_GET_SAMPLE(pmap_,pmap_->best_sample);
	return pose2_t_to_pose2d(campione->pose);
}

/**
 *  Function fill_slam_data fills the splam_data object passed ad a shared pointer.
 *  Not all splam_data object can be filled by pmap_wrap... what is filled here is:
 *  1) 
 */
void pmap_wrap::fill_slam_data(splam_data_ptr data)
{
	pmap_sample_t*	best_sample;
	pmap_scan_t*	scann;
	pose2_t			pose_slap;

	//Best sample
	best_sample = PMAP_GET_SAMPLE(pmap_, pmap_->best_sample);
	
	//Realizzo una occupancy grid con i dati della posizione e della scansione del
	//campione migliore
	omap_clear(omap_);
	data->path_.clear();
	data->og_cells_.resize(omap_->grid_sx * omap_->grid_sy, 0);

	data->path_.reserve(pmap_->step_count);
	for (int j = 0; j < pmap_->step_count; ++j){
		scann = pmap_->scans + j;
		pose_slap = best_sample->poses[j];
		omap_add(omap_, pose_slap, pmap_->num_ranges, scann->ranges);
		data->path_.push_back(pose2_t_to_pose2d(pose_slap));
	}

	for (int i=0; i< omap_->grid_size; ++i){
		data->og_cells_.at(i)= omap_->grid[i];
	}
	data->og_resolution_ = omap_->grid_res;
	data->og_col_ = omap_->grid_sx;
	data->og_row_ = omap_->grid_sy;

#if 1
	static int contazzo=0;
	char mappazza[10];

	contazzo++;

	if(!(contazzo%10))
	{
		sprintf(mappazza,"mappa%d.png",contazzo);
		save_map_as_file(mappazza);
	}
#endif

}

void pmap_wrap::process(scan_data_ptr scan)
{
	pose2_t current_lodoPose;

	std::copy(scan->ranges_.begin(), scan->ranges_.end(), laser_scan_data_);

	current_lodoPose = lodo_add_scan(lodo_,pose2d_to_pose2_t(scan->odo_pose_),laser_scan_number_,laser_scan_data_);
	pmap_update(pmap_, pose2_add(pose2d_to_pose2_t(offset_laser_pose_), current_lodoPose), laser_scan_number_, laser_scan_data_);	
	
}

void pmap_wrap::save_map_as_file(const char* filename)
{
	omap_save_pgm(omap_, filename);
}

size2d	pmap_wrap::get_current_coord() const
{
	pmap_sample_t* campione=PMAP_GET_SAMPLE(pmap_,pmap_->best_sample);
	pose2_t temp = campione->pose;
	return size2d((omap_->grid_sy/2 + temp.pos.y/omap_->grid_res) , (omap_->grid_sx/2 + temp.pos.x/omap_->grid_res));
}

}//namespace splam
}//namespace all
