#include <iostream>
#include <algorithm>
#include "splam_data_net.h"
#include "alcor/splam/typedefs.hpp"
#include "alcor/core/iniwrapper.h"
//---------------------------------------------------------------------------
using namespace std;
using namespace all::util;
using namespace all::math;
//---------------------------------------------------------------------------
namespace all{
namespace splam{

int		pack_pose2d(ArNetPacket* pack, const pose2d& pose)
{
	pack->doubleToBuf(pose.get_x1());
	pack->doubleToBuf(pose.get_x2());
	pack->doubleToBuf(pose.get_th(deg_tag));
	return 3*sizeof(double);
}

void	import_pose2d(ArNetPacket* pack, pose2d& pose)
{
	pose.set_x1(pack->bufToDouble());
	pose.set_x2(pack->bufToDouble());
	pose.set_th(pack->bufToDouble(),deg_tag);
}

int		pack_point2d(ArNetPacket* pack, const point2d& point)
{
	pack->doubleToBuf(point.get_x1());
	pack->doubleToBuf(point.get_x2());
	return 2*sizeof(double);
}

void	import_point2d(ArNetPacket* pack, point2d& point)
{
	point.set_x1(pack->bufToDouble());
	point.set_x2(pack->bufToDouble());
}

splam_data_net::splam_data_net(const char* name)
{
	iniWrapper ini(name);
	size_t temp = ini.GetInt("mappa:larghezza",0) * ini.GetInt("mappa:altezza",0);
	runLenght_ = new TGeneric<CRLE>;
	target_ = new BYTE [temp+2];
	header_ = broken_data;
	intermediate_ = new BYTE [temp];
}

splam_data_net::~splam_data_net()
{
	delete runLenght_;
	delete [] target_;
	delete [] intermediate_;
}

char	reduce(char temp)
{
	if(temp==0)
		return 0;
	if(temp>124)
		return 125;
	if(temp>0)
		return (temp/5+1)*5;
	if(temp<-124)
		return -125;
	if(temp<0)
		return (temp/5-1)*5;
	return 0;
}

void	splam_data_net::pack_og_map(ArNetPacket* pack)
{
	// mappa
	// dimensioni e risoluzione
	pack->byte4ToBuf(static_cast<ArTypes::Byte4>(data_->og_row_));
	pack->byte4ToBuf(static_cast<ArTypes::Byte4>(data_->og_col_));
	pack->doubleToBuf(data_->og_resolution_);

	// data... transformed & compressed
	transform(data_->og_cells_.begin(), data_->og_cells_.end(), intermediate_, reduce);
	runLenght_->Encode(target_, target_lenght, intermediate_, static_cast<long>(data_->og_cells_.size()));

	// data... compressed!!!!
	//runLenght_->Encode(target_, target_lenght, (BYTE*)(&data_->og_cells_[0]), static_cast<long>(data_->og_cells_.size()));

	pack->byte4ToBuf(target_lenght);
	for(size_t size = 0; size< static_cast<size_t>(target_lenght); ++size)
		pack->byteToBuf(target_[size]);

	cout << "true map dimension: "<< data_->og_cells_.size()<< " - coded map dimension: "<< target_lenght<<endl;

}

void	splam_data_net::import_og_map(ArNetPacket* pack)
{
	// Mappa
	data_->og_row_ = pack->bufToByte4();
	data_->og_col_ = pack->bufToByte4();
	data_->og_resolution_ = pack->bufToDouble();
	//Data... COMPRESSED!!!!
	long source_lenght;
	target_lenght = pack->bufToByte4();
	//cout<<"target lenght: "<< target_lenght <<endl;
	for(size_t size = 0; size<static_cast<size_t>(target_lenght); ++size)
		target_[size] = pack->bufToByte();
	
	source_lenght  =  runLenght_->GetMaxDecoded(target_);
	data_->og_cells_.resize(source_lenght);
	runLenght_->Decode( (BYTE*)(&data_->og_cells_[0]), source_lenght, target_, target_lenght  );

	cout << "coded map dimension: "<< target_lenght<< " - true map dimension: "<< data_->og_cells_.size()<<endl;
}

void	splam_data_net::pack_sg_map(ArNetPacket* pack)
{
	// mappa
	// dimensioni e risoluzione
	pack->byte4ToBuf(static_cast<ArTypes::Byte4>(data_->sg_row_));
	pack->byte4ToBuf(static_cast<ArTypes::Byte4>(data_->sg_col_));
	pack->doubleToBuf(data_->sg_resolution_);
	// Data... COMPRESSED!!!!
	runLenght_->Encode(target_, target_lenght, (BYTE*)(&(*data_->sg_cells_.begin())), static_cast<long>(data_->sg_cells_.size()));
	pack->byte4ToBuf(target_lenght);
	for(size_t size = 0; size<static_cast<size_t>(target_lenght); ++size)
		pack->byteToBuf(target_[size]);

	cout << "true saliency map dimension: "<< data_->sg_cells_.size()<< " - coded saliency map dimension: "<< target_lenght<<endl;
}

void	splam_data_net::import_sg_map(ArNetPacket* pack)
{
	// Mappa
	data_->sg_row_ = pack->bufToByte4();
	data_->sg_col_ = pack->bufToByte4();
	data_->sg_resolution_ = pack->bufToDouble();
	//Data... COMPRESSED!!!!
	long source_lenght;
	target_lenght = pack->bufToByte4();
	//cout<<"target lenght: "<< target_lenght <<endl;
	for(size_t size = 0; size<static_cast<size_t>(target_lenght); ++size)
		target_[size] = pack->bufToByte();
	
	source_lenght  =  runLenght_->GetMaxDecoded(target_);
	data_->sg_cells_.resize(source_lenght);
	runLenght_->Decode( (BYTE*)(&(*data_->sg_cells_.begin())), source_lenght, target_, target_lenght  );

	cout << "coded saliency map dimension: "<< target_lenght<< " - true saliency map dimension: "<< data_->sg_cells_.size()<<endl;
}

void	splam_data_net::pack_others(ArNetPacket* pack)
{
	size_t total_size = 0;

	// header
	pack->byteToBuf(header_);
	if(header_ == broken_data)
		return;
	total_size += sizeof(header_);

	// paths
	// robot path
	pack->byte4ToBuf(static_cast<ArTypes::Byte4>(data_->path_.size()));
	total_size += 4;
	for(poses2d_it it = data_->path_.begin(); it != data_->path_.end(); ++it)
		total_size += pack_pose2d(pack,*it);

	// saliency path
	pack->byte4ToBuf(static_cast<ArTypes::Byte4>(data_->observations_.size()));
	total_size += 4;
	for(saliency_path_it_t itx = data_->observations_.begin(); itx != data_->observations_.end(); ++itx)
	{
		pack->byte4ToBuf(static_cast<ArTypes::Byte4>(itx->scan_count_));
		pack->doubleToBuf(itx->bearing_.get_pan(deg_tag));
		pack->doubleToBuf(itx->bearing_.get_tilt(deg_tag));
		pack->byteToBuf(itx->recognition_);
		pack->byte4ToBuf(static_cast<ArTypes::Byte4>(itx->observations_.size()));
		total_size += 9 + 2*sizeof(double);
		for(observation_data_vect_it_t ity = itx->observations_.begin(); ity != itx->observations_.end(); ++ity)
		{
			total_size += pack_point2d(pack,ity->relative_position_);
			pack->doubleToBuf(ity->radius_);
			pack->doubleToBuf(ity->weight_);
			total_size += 2*sizeof(double);
		}
	}

	// attention_fov
	pack->doubleToBuf(data_->attention_.horizontal_fov_.deg());
	pack->doubleToBuf(data_->attention_.near_);
	pack->doubleToBuf(data_->attention_.far_);
	total_size += 3*sizeof(double);

	// scan_data
	// laser
	pack->doubleToBuf(data_->last_scan_.start_angle_.deg());
	pack->doubleToBuf(data_->last_scan_.angle_step_.deg());
	pack->byte4ToBuf(static_cast<ArTypes::Byte4>(data_->last_scan_.ranges_.size()));
	for(scan_values_it itt = data_->last_scan_.ranges_.begin(); itt != data_->last_scan_.ranges_.end(); ++itt)
		pack->byte4ToBuf(*itt);
	total_size += 4 + 2*sizeof(double) + 4*data_->last_scan_.ranges_.size();

	// robot
	total_size += pack_pose2d(pack,data_->last_scan_.odo_pose_);
	pack->byte4ToBuf(static_cast<ArTypes::Byte4>(data_->last_scan_.scan_step_));
	total_size += 4;

	// time
	pack->byte4ToBuf(data_->last_scan_.time_stamp_);
	total_size += 4;

	// goals
	pack->byteToBuf(data_->goal_.is_valid_);
	pack->doubleToBuf(data_->goal_.head_direction_.deg());
	pack->doubleToBuf(data_->goal_.relative_head_direction_.deg());
	pack->byteToBuf(data_->goal_.recognition_);
	total_size += 2 + 2*sizeof(double);
	total_size += pack_pose2d(pack,data_->goal_.goal_near_);
	total_size += pack_pose2d(pack,data_->goal_.goal_far_);
	pack->byte4ToBuf(static_cast<ArTypes::Byte4>(data_->goal_.path_.size()));
	total_size += 4;
	for(points2d_it ite = data_->goal_.path_.begin(); ite != data_->goal_.path_.end(); ++ite)
		total_size += pack_point2d(pack, *ite);

	cout << "true others dimension: " << total_size<<endl;
}

void	splam_data_net::import_others(ArNetPacket* pack)
{
	//header
	header_ = pack->bufToByte();
	if(header_ == broken_data)
	{
		cout << "Broken Data!!!"<<endl;
		return;
	}
	
	// paths
	// robot path
	data_->path_.resize(pack->bufToByte4());
	for(poses2d_it it = data_->path_.begin(); it != data_->path_.end(); ++it)
		import_pose2d(pack, *it);
	
	// saliency path
	data_->observations_.resize(pack->bufToByte4());
	for(saliency_path_it_t itx = data_->observations_.begin(); itx != data_->observations_.end(); ++itx)
	{
		itx->scan_count_ = pack->bufToByte4();
		itx->bearing_.set_pan(deg_tag, pack->bufToDouble()); 
		itx->bearing_.set_tilt(deg_tag, pack->bufToDouble()); 
		itx->recognition_ = pack->bufToByte();
		itx->observations_.resize(pack->bufToByte4());
		for(observation_data_vect_it_t ity = itx->observations_.begin(); ity != itx->observations_.end(); ++ity)
		{
			import_point2d(pack,ity->relative_position_);
			ity->radius_ = pack->bufToDouble();
			ity->weight_ = pack->bufToDouble();
		}
	}

	// attention_fov
	data_->attention_.horizontal_fov_.set_deg(pack->bufToDouble());
	data_->attention_.near_ = pack->bufToDouble();
	data_->attention_.far_ = pack->bufToDouble();

	// scan_data
	// laser
	data_->last_scan_.start_angle_.set_deg(pack->bufToDouble());
	data_->last_scan_.angle_step_.set_deg(pack->bufToDouble());
	data_->last_scan_.ranges_.reserve(pack->bufToByte4());
	for(scan_values_it itt = data_->last_scan_.ranges_.begin(); itt != data_->last_scan_.ranges_.end(); ++itt)
		*itt = pack->bufToByte4();

	// robot
	import_pose2d(pack,data_->last_scan_.odo_pose_);
	data_->last_scan_.scan_step_ = pack->bufToByte4();

	// time
	data_->last_scan_.time_stamp_ = pack->bufToByte4();

	// goals
	data_->goal_.is_valid_ = pack->bufToByte();
	data_->goal_.head_direction_.set_deg(pack->bufToDouble());
	data_->goal_.relative_head_direction_.set_deg(pack->bufToDouble());
	data_->goal_.recognition_ = pack->bufToByte();
	import_pose2d(pack,data_->goal_.goal_near_);
	import_pose2d(pack,data_->goal_.goal_far_);
	data_->goal_.path_.resize(pack->bufToByte4());
	for(points2d_it ite = data_->goal_.path_.begin(); ite != data_->goal_.path_.end(); ++ite)
		import_point2d(pack, *ite);

	cout << "coded others received"<<endl;
}


}//namespace splam
}//namespace all
//-----------------------------------------------------------------------------------------------
