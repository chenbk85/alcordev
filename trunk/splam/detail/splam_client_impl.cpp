#include "Aria.h"
#include "ArNetworking.h"
#include "alcor/core/core.h"
#include "alcor/core/i_connection_handler_t.h"
#include "alcor/core/iniWrapper.h"
#include "splam_data_net.h"
//---------------------------------------------------------------------------
using namespace all::core;
//---------------------------------------------------------------------------
namespace all{
namespace splam{

class	splam_client_impl	:public i_service_handler
{
public:	//typedefs
	typedef ArFunctor1C<splam_client_impl, ArNetPacket*>	functor;

public:	//ctor, dtor, copy ctor, copy assign
	explicit splam_client_impl(const char* inifile = "config/splam.ini");
	~splam_client_impl();

public:	//services
	void	get_splam_data(splam_data&);

public:
	i_connection_handler_t	connection_handler_;

public:
	void	lock(){splam_data_mutex_.lock();}
	void	unlock(){splam_data_mutex_.unlock();}

private:	//callback su "avvenuta connessione" e "disconnessione" dal server
	virtual void register_to();
	virtual void lost_connection(){};

private:	//data
	//splam_data_ptr		splam_data_;
	splam_data_net_ptr	splam_data_net_;
	ArMutex				splam_data_mutex_;

private:	//callbacks per la ricezione dei dati
	functor			map_receive_callback;
	functor			sal_receive_callback;
	functor			others_callback;
	void			map_receive_cb(ArNetPacket*);
	void			sal_receive_cb(ArNetPacket*);
	void			others_receive_cb(ArNetPacket*);

private:
	iniWrapper		ini_;
};

splam_client_impl::splam_client_impl(const char* inifile)
	:connection_handler_(this, std::string(inifile))
	,map_receive_callback(this,&splam_client_impl::map_receive_cb)
	,sal_receive_callback(this,&splam_client_impl::sal_receive_cb)
	,others_callback(this,&splam_client_impl::others_receive_cb)
	,ini_(inifile)
{
	double temp = ini_.GetInt("mappa:larghezza",0) * ini_.GetInt("mappa:altezza",0);
	splam_data_net_.reset(new splam_data_net(static_cast<size_t>(temp)));
	splam_data_net_->data_.reset(new splam_data);
	int lenghtmask = ini_.GetInt("laser:num_step",0);
	splam_data_net_->data_->og_cells_.resize(lenghtmask,0);
}

void	splam_client_impl::register_to()
{
	connection_handler_.m_client.addHandler("Occupancy",&map_receive_callback);
	connection_handler_.m_client.addHandler("Saliency",&sal_receive_callback);
	connection_handler_.m_client.addHandler("Others",&others_callback);
	connection_handler_.m_client.request("Occupancy",200);
	connection_handler_.m_client.request("Saliency",200);
	connection_handler_.m_client.request("Others",200);
}

splam_client_impl::~splam_client_impl()
{
	connection_handler_.m_client.requestStop("Occupancy");
	connection_handler_.m_client.requestStop("Saliency");
	connection_handler_.m_client.requestStop("Others");
	connection_handler_.m_client.remHandler("Occupancy",&map_receive_callback);
	connection_handler_.m_client.remHandler("Saliency",&sal_receive_callback);
	connection_handler_.m_client.remHandler("Others",&others_callback);
	connection_handler_.m_client.stopRunning();
	connection_handler_.m_client.join();
	connection_handler_.running = false;
}

void	splam_client_impl::map_receive_cb(ArNetPacket* packet)
{
	lock();
	splam_data_net_->import_og_map(packet);
	unlock();
}

void	splam_client_impl::sal_receive_cb(ArNetPacket* packet)
{
	lock();
	splam_data_net_->import_sg_map(packet);
	unlock();
}

void	splam_client_impl::others_receive_cb(ArNetPacket* packet)
{
	lock();
	splam_data_net_->import_others(packet);
	unlock();
}

void	splam_client_impl::get_splam_data(splam_data& data)
{
	lock();
	data = *(splam_data_net_->data_.get());
	unlock();
}

}//namespace splam
}//namespace all
//---------------------------------------------------------------------------

