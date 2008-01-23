#include <algorithm>
#include <fstream>
#include "Aria.h"
#include "ArNetworking.h"
#include "alcor/core/core.h"
#include "alcor/core/i_connection_handler_t.h"
#include "alcor/core/iniWrapper.h"
#include "splam_data_net.h"
#include "alcor.extern/CImg/CImg.h"
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
	void			get_splam_data(splam_data&);
	uint8_sarr		get_splam_image();

public:
	i_connection_handler_t	connection_handler_;

public:
	void	lock(){splam_data_mutex_.lock();}
	void	unlock(){splam_data_mutex_.unlock();}

private:	//callback su "avvenuta connessione" e "disconnessione" dal server
	virtual void register_to();
	virtual void lost_connection(){};

public:
	splam_data_net_ptr	splam_data_net_;

private:	//data
	ArMutex				splam_data_mutex_;

private:	//callbacks per la ricezione dei dati
	functor			map_receive_callback;
	functor			sal_receive_callback;
	functor			others_callback;
	void			map_receive_cb(ArNetPacket*);
	void			sal_receive_cb(ArNetPacket*);
	void			others_receive_cb(ArNetPacket*);

public:
	iniWrapper		ini_;
	bool			display_;
	void			show_display();
	void			init_display();
	void			exit_display();
	cimg_library::CImgDisplay*		og_displ;
	cimg_library::CImg<map_value>	og_disp;
	uint8_sarr		splam_image_;

};

splam_client_impl::splam_client_impl(const char* inifile)
	:connection_handler_(this, inifile)
	,map_receive_callback(this,&splam_client_impl::map_receive_cb)
	,sal_receive_callback(this,&splam_client_impl::sal_receive_cb)
	,others_callback(this,&splam_client_impl::others_receive_cb)
	,ini_(inifile)
	,display_(false)
{
	Aria::init();
	size_t temp = ini_.GetInt("mappa:larghezza",0) * ini_.GetInt("mappa:altezza",0);
	splam_data_net_.reset(new splam_data_net(inifile));
	splam_data_net_->data_.reset(new splam_data(inifile));
	int lenghtmask = ini_.GetInt("laser:num_step",0);
	splam_data_net_->data_->og_cells_.resize(lenghtmask,0);
	splam_image_.reset(new uint8_t[temp]);
}

void	splam_client_impl::register_to()
{
	connection_handler_.m_client.addHandler("Occupancy",&map_receive_callback);
	connection_handler_.m_client.addHandler("Saliency",&sal_receive_callback);
	connection_handler_.m_client.addHandler("Others",&others_callback);
	connection_handler_.m_client.request("Occupancy",10000);
	connection_handler_.m_client.request("Saliency",10000);
	connection_handler_.m_client.request("Others",10000);
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
	if(display_)
		show_display();
}

void	splam_client_impl::sal_receive_cb(ArNetPacket* packet)
{
	lock();
	splam_data_net_->import_sg_map(packet);
	unlock();
	if(display_)
		show_display();
}

void	splam_client_impl::others_receive_cb(ArNetPacket* packet)
{
	lock();
	splam_data_net_->import_others(packet);
	unlock();
	if(display_)
		show_display();
}

void	splam_client_impl::get_splam_data(splam_data& data)
{
	lock();
	data = *(splam_data_net_->data_.get());
	std::cout<< "SPLAM_DATA COPIED!"<<std::endl;
	unlock();
}

uint8_sarr	splam_client_impl::get_splam_image()
{
	for(int i=0; i<splam_data_net_->data_->og_row_*splam_data_net_->data_->og_col_; i++)
		splam_image_[i] = static_cast<uint8_t>( static_cast<int>(127) - static_cast<int>(splam_data_net_->data_->og_cells_[i]));
	return splam_image_;
}


void	splam_client_impl::init_display()
{
	og_displ = new cimg_library::CImgDisplay(splam_data_net_->data_->og_col_*2, splam_data_net_->data_->og_row_*2, "Occupancy");
}
void	splam_client_impl::exit_display()
{
	delete og_displ;
}

all::util::pixel_value	trasf(const all::util::map_value& temp)
{
	return static_cast<pixel_value>(127 - static_cast<int>(temp));
}

all::util::pixel_value	trasf2(const all::util::map_value& temp)
{
	return static_cast<pixel_value>(127 + static_cast<int>(temp));
}

void	splam_client_impl::show_display()
{
	lock();
	all::util::pixel_values	temp;
	std::transform(
		splam_data_net_->data_->og_cells_.begin(), 
		splam_data_net_->data_->og_cells_.end(), 
		std::back_inserter<all::util::pixel_values>(temp),
#if 1
		trasf
#else
		trasf2
#endif
		);
	og_disp.assign(&temp[0], splam_data_net_->data_->og_col_, splam_data_net_->data_->og_row_, 1,1);
	og_disp.display(*og_displ);
	//std::ofstream rotolo("scorreggione.txt", std::ios::out);
	//for(int i=0;i<temp.size();i++)
	//	rotolo << (int)temp[i]<< std::endl;
	unlock();
}

}//namespace splam
}//namespace all
//---------------------------------------------------------------------------