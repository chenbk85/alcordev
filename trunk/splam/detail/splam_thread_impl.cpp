#include "Aria.h"
#include "ArNetworking.h"
#include "alcor/core/core.h"
#include "alcor/act/p3dx_client_t.h"
#include "alcor/sense/urg_laser_t.hpp"
#include "alcor/core/iniWrapper.h"
#include "pmap_wrap.h"
//-----------------------------------------------------------------------------------------------
using namespace all::core;
using namespace all::act;
using namespace all::sense;
//-----------------------------------------------------------------------------------------------
namespace all{
namespace splam{

class splam_thread_impl: public ArASyncTask 
{
public:		//typedefs
	typedef ArFunctor2C<splam_thread_impl,ArServerClient*, ArNetPacket*>	functor;

public:		//ctor, dtor, copy ctor, copy assign
	splam_thread_impl(char* name = "./config/splam.ini");
	~splam_thread_impl();

public:		//thread side
	virtual void*		runThread(void*);
	void				stop(){this->stopRunning();}

private:		//hokuyo urg laser
	std::vector<int>    laser_mask;
	void				set_laser_mask(std::pair<double,double>);
	void				filter();			//le precondizioni è che lo urg_scan_data_t sia già pieno
	urg_scan_data_t		current_scan_;		//
	urg_scan_data_t		last_scan_;			//
	ArMutex				scan_data_mutex_;	//
	size_t				scan_count_;		//number of current scan
	int					laser_com_port_;	//
	size_t				urg_step_start_;	//
	size_t				urg_step_end_;		//
	size_t				urg_cc_;			//

public:		//Pioneer Robot (p3dx or p3at)
	boost::shared_ptr<p3dx_client_t>		robot_;

private:	//
	ArMutex				splam_data_mutex_;
	ArMutex				pmap_mutex_;

private:		//Data Acquisition
	void				acquire_all();
	void				acquire_laser_scan();	//internal
	void				acquire_odometry();	//internal
	void				estimate_odometry();

public:	//Data Processing
	void				process_();
	void				fill_map_data();

private:
	pmap_wrap			pmap_wrap_;
	splam_data			splam_data_;
	value_iteration		value_iteration_;

public:	//Data Broadcasting
	void				start_server();
	void				stop_server();
	void				broadcast_splam_data();
	void				update_pose_on_robot();
	void				maps(ArServerClient* ,ArNetPacket*);
	void				others(ArServerClient* ,ArNetPacket*);

private:
	functor				maps_callback;
	functor				others_callback;
	ArServerBase		server_;
	ip_address_t		server_address_;
	boost::shared_ptr<splam_data_net>	splam_data_net_;
	bool				odoPresent_;

public:
	std::ofstream		logfile_;
	iniWrapper			ini_;

};

splam_thread_impl::splam_thread_impl( char* name)
	:ini_(name)
	,maps_callback(this, &splam_thread_impl::maps)
	,others_callback(this, &splam_thread_impl::others)
	,pmap_wrap_(name)
	,scan_count_(0)
{
	Aria::init();

	// creo spazio per i vector di mask e di scan
	int lenghtmask = ini_.GetInt("laser:num_step",0);
	mask.resize(lenghtmask,1);
	current_scan_.ranges.resize(lenghtmask,0);
	splam_data_.scan_ = current_scan_;
	fill(mask.begin(),mask.end(),1);

	//inizializzazione dei parametri del robot
	odoPresent_ = (ini_.GetInt("robot:odo",1) == 1);

	//inizializzazione della struttura di splam_data_net
	slamDataNet_ = new splam_data_net(worker_.GetSize());
	slamDataNet_->data_ = &splam_data_;

	//inizializzazione dei parametri di connessione del laser
	urg_step_start_	= ini_.GetInt("laser:start_step", UrgDriver::default_start);
	urg_step_end_	= ini_.GetInt("laser:end_step", UrgDriver::default_end);
	urg_cc_	= ini_.GetInt("laser:cc", 2);
	laser_com_port_ = ini_.GetInt("laser:comport",0);
	if(laser_com_port_)
	{
		UrgDriver::Instance()->Connect(laser_com_port_);
	}else{
		//logFile_.open(ini_.GetStringAsChar("laser:logfile","laserlog.txt"),std::ios::out);
	}

	//inizializzazione della struttura di value_iteration per il goal metrico
	value_iteration_.Create(ini_.GetInt("mappa:larghezza",0), ini_.GetInt("mappa:altezza",0),ini_.GetDouble("mappa:dim_cella",0.0)*100.0);

	// leggo dal file INI la porta del server
	port_ = ini_.GetInt("server:port",12321);
	robot = all::act::create_p3dx_client();
	//robot->Run();
}

splam_thread_impl::~splam_thread_impl()
{
	this->stopRunning();
	this->join();
	delete slamDataNet_;
}


}//namespace splam
}//namespace all
//-----------------------------------------------------------------------------------------------
