#include <fstream>
#include <exception>
#include <stdexcept>
#include "Aria.h"
#include "ArNetworking.h"
#include "alcor/core/core.h"
#include "alcor/sense/urg_laser_t.hpp"
#include "alcor/act/p3_client_t.h"
#include "alcor/core/iniWrapper.h"
#include "alcor/splam/splam_data.h"
#include "alcor/splam/scan_data.h"
#include "splam_data_net.h"
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
public:		// typedefs
	typedef ArFunctor2C<splam_thread_impl,ArServerClient*, ArNetPacket*>	functor;

public:		// ctor, dtor, copy ctor, copy assign
	splam_thread_impl(const char* name = "config/splam.ini");
	~splam_thread_impl();

public:		// thread side
	virtual void*		runThread(void*);
	void				stop(){this->stopRunning();}

private:	// hokuyo urg laser
	urg_laser_t			urg_laser_;
	urg_scan_data_ptr	urg_scan_data_ptr_;
	int_vect		    laser_mask_;								
	//bool				set_laser_mask(double_pair);		// angles are GRADS
	//bool				set_laser_mask(angle_pair);			// done
	//bool				set_laser_mask(size2d);				// done
	//bool				set_laser_mask(int,int);			// done
	//bool				set_laser_mask(int_pair_vect);		// done
	//bool				set_laser_mask(double_pair_vect);	// to do...
	//bool				set_laser_mask(angle_pair_vect);	// to do...
	//bool				set_laser_mask(sizes2d);			// to do...
	//void				filter();			// to do...
	scan_data			current_scan_;		//
	ArMutex				scan_data_mutex_;	//
	size_t				scan_count_;		//number of current scan
	int					urg_step_start_;	//
	int					urg_step_end_;		//
	int					urg_cc_;			//
	bool				laser_present_;

public:		// Pioneer Robot (p3dx or p3at)
	p3_client_ptr_t		robot_;
	void				acquire_laser_scan();	// internal
	void				emulate_laser_scan();
	void				fill_scan_data();
	pose2d				emulate_odometry();

private:	// splam
	pmap_wrap			pmap_wrap_;
	splam_data_ptr		splam_data_;
	splam_data_net_ptr	splam_data_net_;
	ArMutex				splam_data_mutex_;
	ArMutex				pmap_mutex_;

public:		// data Broadcasting
	void				start_server();			// 
	void				stop_server();			// 
	void				broadcast_splam_data();
	void				map_cb(ArServerClient* ,ArNetPacket*);
	void				sal_cb(ArServerClient* ,ArNetPacket*);
	void				others_cb(ArServerClient* ,ArNetPacket*);
private:
	ArServerBase		server_;
	ip_address_t		server_address_;
	bool				server_started_;
	functor				map_callback;
	functor				sal_callback;
	functor				others_callback;
	//bool				odoPresent_;

public:		// misc
	iniWrapper			ini_;
};

splam_thread_impl::splam_thread_impl( const char* name)
	:ini_(name)
	,map_callback(this, &splam_thread_impl::map_cb)
	,sal_callback(this, &splam_thread_impl::sal_cb)
	,others_callback(this, &splam_thread_impl::others_cb)
	,pmap_wrap_(name)
	,scan_count_(0)
	,server_started_(false)
	,laser_present_(true)
{
		// ARIA initialization
	Aria::init();

		// splam_data initialization
	splam_data_.reset(new splam_data);
	splam_data_net_.reset(new splam_data_net(pmap_wrap_.get_map_cells()));
	splam_data_net_->data_ = splam_data_;

		// laser initialization and connection
	int lenghtmask = ini_.GetInt("laser:num_step",0);
	//laser_mask_.resize(lenghtmask,1);
	//fill(laser_mask_.begin(), laser_mask_.end(), 1);
	current_scan_.ranges_.resize(lenghtmask,0);
	urg_step_start_	= ini_.GetInt("laser:start_step", urg_scan_data_t::default_start);
	urg_step_end_	= ini_.GetInt("laser:end_step", urg_scan_data_t::default_end);
	urg_cc_	= ini_.GetInt("laser:cc", 1);
	if(!urg_laser_.connect())
		laser_present_ = false;

		// robot client
	robot_.reset(new p3_client_t("config/p3_conf.ini"));
	robot_->run_async();

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

void	splam_thread_impl::map_cb(ArServerClient* client, ArNetPacket* clientPack)
{
	//cout << "Mappa Richiesta :D"<<endl;
	// empty callback
}

void	splam_thread_impl::sal_cb(ArServerClient* client, ArNetPacket* clientPack)
{
	//cout << "Mappa Richiesta :D"<<endl;
	// empty callback
}

void	splam_thread_impl::others_cb(ArServerClient* client, ArNetPacket* clientPack)
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
	server_.addData("Occupancy", "OG from SLAM module", &map_callback, "none", "none");
	server_.addData("Saliency", "SG from SLAM module", &sal_callback, "none", "none");
	server_.addData("Others", "Other data from SLAM module", &others_callback, "none", "none");

		// run the server thread
	server_.runAsync();

		// notification
	server_started_ = true;

	std::cout << "SPLAM Server Started at - hostname: " << server_address_.hostname<< " - port: " << server_address_.port<< std::endl;
}

void	splam_thread_impl::stop_server()
{
	server_.stopRunning();
	server_.join();
	server_.close();
	server_started_ = false;
}

void	splam_thread_impl::fill_scan_data()
{
	current_scan_.start_angle_ = urg_laser_t::step2angle(urg_step_start_) ;
	current_scan_.angle_step_ = urg_laser_t::resolution(urg_cc_);
	current_scan_.scan_step_ = scan_count_;
	current_scan_.time_stamp_ = clock();
	scan_count_++;
}

void	splam_thread_impl::emulate_laser_scan()
{
	int temp=0,value=rand()%5000;
	srand((unsigned)time(NULL));
	for(scan_values_it iter = current_scan_.ranges_.begin(); iter != current_scan_.ranges_.end(); ++iter)
	{
		temp++;
		if(temp%10 == 0)
			value = rand()%5000;
		*iter = value;
		//*iter = rand()%5000;
		//std::cout << " " << *iter;
	}
	std::cout<< "LASER NOT PRESENT... RUNNING UNDER SIMULATED DATA"<<std::endl;
}

void	splam_thread_impl::acquire_laser_scan()
{
	urg_scan_data_ptr_ = urg_laser_.do_scan(urg_step_start_, urg_step_end_, urg_cc_);
	current_scan_.ranges_  = urg_scan_data_ptr_->scan_points;
	for (scan_values_it it=current_scan_.ranges_.begin(); it!=current_scan_.ranges_.end();++it)
		if(*it < 20)
			*it = 0;
}

pose2d	splam_thread_impl::emulate_odometry()
{
	pose2d returnvalue;
	//returnvalue = splam_data_->get_current_position().move(static_cast<double>(rand()%100)/1000.0,angle(rand()%180-90,deg_tag)).rotate(angle(rand()%180-90,deg_tag));
	returnvalue = splam_data_->goal_.goal_near_;
	std::cout << "GOAL NEAR: " << splam_data_->goal_.goal_near_ << std::endl;
	returnvalue.set_th(rand(),deg_tag);
	if(returnvalue.magnitude() >= 14.0)
		returnvalue.setP(returnvalue.getP().resize(15.0));
	std::cout<< "ROBOT NOT PRESENT... RUNNING UNDER SIMULATED DATA"<<std::endl;

	return returnvalue;
}

void	splam_thread_impl::broadcast_splam_data()
{
	ArNetPacket og_map_packet;
	ArNetPacket sg_map_packet;
	ArNetPacket others_packet;
	splam_data_net_->header_ = splam_data_net::full_data;

	splam_data_net_->pack_og_map(&og_map_packet);
	splam_data_net_->pack_sg_map(&sg_map_packet);
	splam_data_net_->pack_others(&others_packet);
	server_.broadcastPacketTcp(&og_map_packet,"Occupancy");
	ArUtil::sleep(100);
	server_.broadcastPacketTcp(&sg_map_packet,"Saliency");
	ArUtil::sleep(100);
	server_.broadcastPacketTcp(&others_packet,"Others");
}

//void splam_thread_impl::filter()
//{
//	scan_data_mutex_.lock();
//	for (size_t i=0; i<laser_mask_.size();++i)
//		current_scan_.ranges_[i]= laser_mask_.at(i)* current_scan_.ranges_[i];
//	scan_data_mutex_.unlock();
//}
//
//bool splam_thread_impl::set_laser_mask(double_pair temp)
//{
//	return set_laser_mask(std::make_pair<angle,angle>(angle(temp.first,deg_tag), angle(temp.second,deg_tag)));
//}
//
//bool splam_thread_impl::set_laser_mask(angle_pair temp)
//{
//	return set_laser_mask(urg_laser_t::angle2step(temp.first), urg_laser_t::angle2step(temp.second));
//}
//
//bool splam_thread_impl::set_laser_mask(size2d temp)
//{
//	return set_laser_mask(static_cast<int>(temp.row_), static_cast<int>(temp.col_));
//}
//
//bool splam_thread_impl::set_laser_mask(int first, int last)
//{
//	if (first<urg_step_start_ || last>urg_step_end_)
//		return false;
//	std::fill(laser_mask_.begin()+first-urg_step_start_, laser_mask_.begin()+last+1-urg_step_start_, 0);
//	return true;
//}
//
//bool splam_thread_impl::set_laser_mask(int_pair_vect temp)
//{
//	bool returnvalue = true;
//	for(int_pair_vect_it it=temp.begin(); it!=temp.end();++it)
//		returnvalue = returnvalue && set_laser_mask(it->first, it->second);
//	return returnvalue;
//}

void*	splam_thread_impl::runThread(void* arg)
{
	while(this->ArASyncTask::getRunning())
	{
		static int step=0;
		// laser scan acquisition
		std::cout << "runThread......... STEP "<<step<< " - FASE 1"<<std::endl;
		if(laser_present_)
			acquire_laser_scan();
		else
			emulate_laser_scan();
		fill_scan_data();

		// odometry acquisition
		std::cout << "runThread......... STEP "<<step<< " - FASE 2"<<std::endl;
		if(robot_->is_connected())
			current_scan_.odo_pose_ = robot_->get_odometry();
		else
			current_scan_.odo_pose_ = emulate_odometry();

		// slam processin
		std::cout << "runThread......... STEP "<<step<< " - FASE 3"<<std::endl;
		pmap_wrap_.process(current_scan_);

		// filling slam_data
		std::cout << "runThread......... STEP "<<step<< " - FASE 4"<<std::endl;
		splam_data_->last_scan_ = current_scan_;
		pmap_wrap_.fill_slam_data(splam_data_);

		// saliency building
		std::cout << "runThread......... STEP "<<step<< " - FASE 5"<<std::endl;
		splam_data_->build_saliency_map();

		// goal finding
		std::cout << "runThread......... STEP "<<step<< " - FASE 6"<<std::endl;
		//splam_data_->saliency_goal_finding(&splam_data_->goal_)
		splam_data_->metric_goal_finding();

		// splam data broadcasting
		std::cout << "runThread......... STEP "<<step<< " - FASE 7"<<std::endl;
		if(server_started_)
			broadcast_splam_data();
		else
			std::cout << "NOT BROADCASTING DATA.... SERVER NOT STARTED!!" << std::endl;

		// p3_server updating with localized coord
		std::cout << "runThread......... STEP "<<step<< " - FASE 8"<<std::endl;
		robot_->set_slam_localized(pmap_wrap_.get_current_position());

		// processor yielding and heartbeat
		ArUtil::sleep(100);
		cout << "splam_thread_impl IS RUNNING....................................." << endl;

		step++;
	}
	return NULL;
}

}//namespace splam
}//namespace all
//-----------------------------------------------------------------------------------------------
