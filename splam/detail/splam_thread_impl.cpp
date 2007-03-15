#include "Aria.h"
#include "ArNetworking.h"
#include "alcor/core/core.h"
#include "alcor/sense/urg_laser_t.hpp"
//#include "alcor/act/p3dx_client_t.h"
#include "alcor/core/iniWrapper.h"
#include "alcor/splam/splam_data.h"
#include "alcor/splam/scan_data.h"
#include "splam_data_net.h"
#include "pmap_wrap.h"
//-----------------------------------------------------------------------------------------------
using namespace all::core;
//using namespace all::act;
using namespace all::sense;
//-----------------------------------------------------------------------------------------------
namespace all{
namespace splam{

class splam_thread_impl: public ArASyncTask 
{
public:		// typedefs
	typedef ArFunctor2C<splam_thread_impl,ArServerClient*, ArNetPacket*>	functor;

public:		// ctor, dtor, copy ctor, copy assign
	splam_thread_impl(const char* name = "./config/splam.ini");
	~splam_thread_impl();

public:		// thread side
	virtual void*		runThread(void*);
	void				stop(){this->stopRunning();}

private:	// hokuyo urg laser
	urg_laser_t			urg_laser_;
	urg_scan_data_ptr	urg_scan_data_ptr_;
	int_vect		    laser_mask_;
	void				set_laser_mask(std::pair<double,double>);
	void				filter();			//le precondizioni è che lo urg_scan_data_t sia già pieno
	scan_data			current_scan_;		//
	scan_data			previous_scan_;			//
	ArMutex				scan_data_mutex_;	//
	size_t				scan_count_;		//number of current scan
	size_t				urg_step_start_;	//
	size_t				urg_step_end_;		//
	size_t				urg_cc_;			//

public:		// Pioneer Robot (p3dx or p3at)
	//boost::shared_ptr<p3dx_client_t>		robot_;

private:	// data Acquisition
	void				acquire_all();
	void				acquire_laser_scan();	// internal
	void				acquire_odometry();		// internal
	void				estimate_odometry();

public:		// data Processing
	void				slam_process();
	void				fill_slam_data();

private:	// splam
	pmap_wrap			pmap_wrap_;
	boost::shared_ptr<splam_data>		splam_data_;
	boost::shared_ptr<splam_data_net>	splam_data_net_;
	ArMutex				splam_data_mutex_;
	ArMutex				pmap_mutex_;

public:		// data Broadcasting
	void				start_server();			// 
	void				stop_server();			// 
	void				broadcast_splam_data();
	void				update_pose_on_robot();
	void				maps(ArServerClient* ,ArNetPacket*);
	void				others(ArServerClient* ,ArNetPacket*);
private:
	ArServerBase		server_;
	ip_address_t		server_address_;
	bool				server_started_;
	functor				maps_callback;
	functor				others_callback;
	//bool				odoPresent_;

public:		// misc
	//std::ofstream		logfile_;
	iniWrapper			ini_;

};

splam_thread_impl::splam_thread_impl( const char* name)
	:ini_(name)
	,maps_callback(this, &splam_thread_impl::maps)
	,others_callback(this, &splam_thread_impl::others)
	,pmap_wrap_(name)
	,scan_count_(0)
	,server_started_(false)
	,urg_scan_data_ptr_(0)

{
		// ARIA initialization
	Aria::init();

		// splam_data initialization
	splam_data_.reset(new splam_data);
	splam_data_->last_scan_ = current_scan_;
	splam_data_net_.reset(new splam_data_net(pmap_wrap_.get_map_cells()));
	splam_data_net_->data_ = splam_data_;

		// laser initialization and connection
	int lenghtmask = ini_.GetInt("laser:num_step",0);
	laser_mask_.resize(lenghtmask,1);
	current_scan_.ranges_.resize(lenghtmask,0);
	fill(laser_mask_.begin(), laser_mask_.end(), 1);
	urg_step_start_	= ini_.GetInt("laser:start_step", urg_scan_data_t::default_start);
	urg_step_end_	= ini_.GetInt("laser:end_step", urg_scan_data_t::default_end);
	urg_cc_	= ini_.GetInt("laser:cc", 1);
	if(!urg_laser_.connect())
		throw std::runtime_error("cannot connect laser");
	
		// robot client
	//robot = all::act::create_p3dx_client();
	//odoPresent_ = (ini_.GetInt("robot:odo",1) == 1);
	//robot->Run();

	// server IP initialization... to start server clients must call "splam_thread::start_server()"
	server_address_.hostname = "127.0.0.1";
	server_address_.port = ini_.GetInt("server:port",12321);
}

splam_thread_impl::~splam_thread_impl()
{
	if(server_started_)
		stop_server();
	this->stopRunning();
	this->join();
}

void	splam_thread_impl::maps(ArServerClient* client, ArNetPacket* clientPack)
{
	//cout << "Mappa Richiesta :D"<<endl;
	// empty callback
}

void	splam_thread_impl::others(ArServerClient* client, ArNetPacket* clientPack)
{
	//cout << "Altri Dati Richiesti :D"<<endl;
	// empty callback
}

void	splam_thread_impl::start_server()
{
		// open the server
	if (!server_.open(server_address_.port))
		throw std::runtime_error("Error in SlamServer::start_server");

		// add the "splam_data" service
	server_.addData("Mappa", "Mappa from SLAM module", &maps_callback, "none", "none");
	server_.addData("Others", "Other data from SLAM module", &others_callback, "none", "none");

		// run the server thread
	server_.runAsync();

		// notification
	server_started_ = true;
}

void	splam_thread_impl::stop_server()
{
	server_.stopRunning();
	server_.join();
	server_.close();
	server_started_ = false;
}

void splam_thread_impl::acquire_all()
{
	// laser
	acquire_laser_scan();

	// odometry
	acquire_odometry();
}

void splam_thread_impl::acquire_laser_scan()
{
	// lock
	scan_data_mutex_.lock();

	// save last scan
	if(scan_count_ != 0)
		previous_scan_ = current_scan_;
	
	// scan acquisition from laser and filling of scan_data structure
	urg_scan_data_ptr_ = urg_laser_.do_scan(urg_step_start_, urg_step_end_, urg_cc_);

	current_scan_.ranges_  = urg_scan_data_ptr_->scan_points;
	for (scan_values_it it=current_scan_.ranges_.begin(); it!=current_scan_.ranges_.end();++it)
		if(*it < 20)
			*it = 0;
	current_scan_.start_angle_ = urg_laser_t::step2angle(urg_step_start_) ;
	current_scan_.scan_step_ = urg_laser_t::resolution(urg_cc_);
	current_scan_.scan_step_ = scan_count_;
	current_scan_.time_stamp_ = clock();

	scan_count_++;
	
	// unlock
	scan_data_mutex_.unlock();
}

void splam_thread_impl::acquire_odometry()
{
	// lock
	scan_data_mutex_.lock();

	//acquisizione odometria del Robot e riempimento del campo pos_odometrica della struttura scan_data
	if((odoPresent_) && (robot->net_state() == all::core::eActiveOpen))
	{
		current_scan_.pos_odometrica = pose2d_to_pose2_t(robot->get_odometry());
		current_scan_.pos_odometrica.pos.x/=1000;
		current_scan_.pos_odometrica.pos.y/=1000;
	}else{
		if(scan_count_ < 2)
		{
			current_scan_.pos_odometrica.pos.x = 0;
			current_scan_.pos_odometrica.pos.y = 0;
			current_scan_.pos_odometrica.rot   = 0;
		}else{
			estimate_odometry();
		}
	}

	// unlock
	scan_data_mutex_.unlock();
}

void*	splam_thread_impl::runThread(void* arg)
{
	while(this->ArASyncTask::getRunning())
	{
		acquire_all();
		slam_process();
		fill_slam_data();
		broadcast_slam_data();
		update_pose_on_robot();
		ArUtil::sleep(1);
		cout << "splam_thread_impl IS RUNNING....................................." << endl;
	}
	return NULL;
}

}//namespace splam
}//namespace all
//-----------------------------------------------------------------------------------------------
