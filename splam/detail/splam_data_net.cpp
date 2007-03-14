#include <iostream>
#include "splam_data_net.h"
#include "typedefs.hpp"
//---------------------------------------------------------------------------
using namespace std;
using namespace all::util;
//---------------------------------------------------------------------------
namespace all{
namespace splam{

splam_data_net::splam_data_net(size_t mapDim)
{
	runLenght_ = new TGeneric<CRLE>;
	target_ = new BYTE [mapDim+2];
	header_ = broken_data;
}

splam_data_net::~splam_data_net()
{
	delete runLenght_;
	delete [] target_;
}

void	splam_data_net::pack_others(ArNetPacket* pack)
{
	pose2d_vect_it	it;
	int_vect_it		itt;
	size_t		size;
	int total_size = 0;

	//header
	pack->byteToBuf(header_);
	if(header_ == broken_data)
		return;
	total_size +=1;

	// Percorso del Robot
	pack->byte4ToBuf(data_->path_.size());
	for(it = data_->path_.begin(); it != data_->path_.end(); ++it)
	{
		pack->doubleToBuf(it->pos.x);
		pack->doubleToBuf(it->pos.y);
		pack->doubleToBuf(it->rot);
	}
	total_size += 4 + 24*data_->path_.size();

	// Ultimo Scan del Laser
	//Laser Ranges
	pack->doubleToBuf(data_->scan_.start_angle);
	pack->doubleToBuf(data_->scan_.angle_step);
	pack->byte4ToBuf(data_->scan_.ranges.size());
	for(itt = data_->scan_.ranges.begin(); itt != data_->scan_.ranges.end(); ++itt)
		pack->byte4ToBuf(*itt);
	//Posa Odometrica
	pack->doubleToBuf(data_->scan_.pos_odometrica.pos.x);
	pack->doubleToBuf(data_->scan_.pos_odometrica.pos.y);
	pack->doubleToBuf(data_->scan_.pos_odometrica.rot);
	//Tempo di acquisizione
	pack->byte4ToBuf(data_->scan_.tempo);
	total_size += 48 + 4*data_->scan_.ranges.size();

	// Goal
	//goal reale
	pack->doubleToBuf(data_->goal_.pos.x);
	pack->doubleToBuf(data_->goal_.pos.y);
	pack->doubleToBuf(data_->goal_.rot);
	//goal "vicino"
	pack->doubleToBuf(data_->goalTemp_.pos.x);
	pack->doubleToBuf(data_->goalTemp_.pos.y);
	pack->doubleToBuf(data_->goalTemp_.rot);
	//goal Path
	pack->byte4ToBuf(data_->goalPath_.size());
	for(it = data_->goalPath_.begin(); it != data_->goalPath_.end(); ++it)
	{
		pack->doubleToBuf(it->pos.x);
		pack->doubleToBuf(it->pos.y);
		pack->doubleToBuf(it->rot);
	}
	total_size += 52 + 24*data_->goalPath_.size();

	cout << "Others Sent...\n";
	cout << "TOTAL PACKET DIMENSION: " << total_size<<endl;
}

void	splam_data_net::import_others(ArNetPacket* pack)
{
	PoseVectIt	it;
	IntVectIt	itt;
	size_t size;

	//header
	header_ = pack->bufToByte();
	if(header_ == broken_data)
	{
		cout << "Broken Data!!!"<<endl;
		return;
	}
	

	// Percorso del Robot
	size = pack->bufToByte4();
	data_->path_.resize(size);
	for(it = data_->path_.begin(); it != data_->path_.end(); ++it)
	{
		it->pos.x = pack->bufToDouble();
		it->pos.y = pack->bufToDouble();
		it->rot = pack->bufToDouble();
	}
	

	// Ultimo Scan del Laser
	//Laser Ranges
	data_->scan_.start_angle = pack->bufToDouble();
	data_->scan_.angle_step = pack->bufToDouble();
	size = pack->bufToByte4();
	data_->scan_.ranges.resize(size);
	for(itt = data_->scan_.ranges.begin(); itt != data_->scan_.ranges.end(); ++itt)
		*itt = pack->bufToByte4();
	//Posa Odometrica
	data_->scan_.pos_odometrica.pos.x = pack->bufToDouble();
	data_->scan_.pos_odometrica.pos.y = pack->bufToDouble();
	data_->scan_.pos_odometrica.rot = pack->bufToDouble();
	//Tempo di acquisizione
	data_->scan_.tempo = pack->bufToByte4();
	

	// Goal
	//goal reale
	data_->goal_.pos.x = pack->bufToDouble();
	data_->goal_.pos.y = pack->bufToDouble();
	data_->goal_.rot = pack->bufToDouble();
	////goal "vicino"
	data_->goalTemp_.pos.x = pack->bufToDouble();
	data_->goalTemp_.pos.y = pack->bufToDouble();
	data_->goalTemp_.rot = pack->bufToDouble();
	//goal Path
	size = pack->bufToByte4();
	data_->goalPath_.resize(size);
	for(it = data_->goalPath_.begin(); it != data_->goalPath_.end(); ++it)
	{
		it->pos.x = pack->bufToDouble();
		it->pos.y = pack->bufToDouble();
		it->rot = pack->bufToDouble();
	}
	
	cout << "Pacchetto Others ricevuto!!!"<<endl;
}

void	splam_data_net::pack_og_map(ArNetPacket* pack)
{
	size_t		size;
	int total_size = 0;

	//mappa
	//dimensioni e risoluzione
	pack->byte4ToBuf(data_->mapXdimension_);
	pack->byte4ToBuf(data_->mapYdimension_);
	pack->doubleToBuf(data_->mapResolution_);
	//Data... COMPRESSED!!!!
	runLenght_->Encode(target_, target_lenght, (BYTE*)(&(*data_->cells_.begin())), data_->cells_.size());
	pack->byte4ToBuf(target_lenght);
	for(size = 0; size<target_lenght; ++size)
		pack->byteToBuf(target_[size]);
	//total_size += 20 + target_lenght;

	cout << "true map dimension: "<< data_->cells_.size()<< " - coded map dimension: "<< target_lenght<<endl;

}

void	splam_data_net::import_og_map(ArNetPacket* pack)
{
	size_t		size;

	// Mappa
	data_->mapXdimension_ = pack->bufToByte4();
	data_->mapYdimension_ = pack->bufToByte4();
	data_->mapResolution_ = pack->bufToDouble();
	//Data... COMPRESSED!!!!
	long source_lenght;
	target_lenght = pack->bufToByte4();
	//cout<<"target lenght: "<< target_lenght <<endl;
	for(size = 0; size<target_lenght; ++size)
		target_[size] = pack->bufToByte();
	
	source_lenght  =  runLenght_->GetMaxDecoded(target_);
	data_->cells_.resize(source_lenght);
	runLenght_->Decode( (BYTE*)(&(*data_->cells_.begin())), source_lenght, target_, target_lenght  );
	

	cout << "coded map dimension: "<< target_lenght<< " - true map dimension: "<< data_->cells_.size()<<endl;

}

}//namespace splam
}//namespace all
//-----------------------------------------------------------------------------------------------
