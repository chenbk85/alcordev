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
	void			get_splam_data(splam_data&);

private:	//callbacks per la ricezione dei dati
	functor			map_receive_callback;
	functor			sal_receive_callback;
	functor			others_callback;
	void			map_receive_cb(ArNetPacket*);
	void			sal_receive_cb(ArNetPacket*);
	void			others_receive_cb(ArNetPacket*);

public:
	i_connection_handler_t	connection_handler_;

private:	//callback su "avvenuta connessione" e "disconnessione" dal server
	virtual void register_to();
	virtual void lost_connection(){};

private:	//data
	splam_data_ptr		splam_data_;
	splam_data_net_ptr	splam_data_net_;
	ArMutex				splam_data_mutex_;

private:
	iniWrapper		ini_;
};

}//namespace splam
}//namespace all
//---------------------------------------------------------------------------

#if 0
//---------------------------------------------------------------------------
splam_client_impl::splam_client_impl(char* inifile)
	:alcor::interfaces::i_connection_handler(inifile)
	,MapDataReceiveCB(this,&splam_client_impl::MapDataReceive)
	,OthersDataReceiveCB(this,&splam_client_impl::OthersDataReceive)
	,VictimDataReceiveCB(this,&splam_client_impl::VictimDataReceive)
	,ini_(inifile)
	,victimCount_(0)
	,victims_(0,0)
{
	//inizializzazione della struttura SlamData
	int lenghtmask = ini_.GetInt("laser:num_step",0);
	slamData_.cells_.resize(lenghtmask,0);
	slamData_.scan_ = slamData_.scan_;

	//inizializzazione di SlamDataNet: associazione della SlamData
	double temp = ini_.GetInt("mappa:larghezza",0) * ini_.GetInt("mappa:altezza",0);
	slamDataNet_ = new SlamDataNet(static_cast<size_t>(temp));
	slamDataNet_->data_= &slamData_;

	ArTypes::Byte tempo=0;
	victimPack.byteToBuf(tempo);
}

splam_client_impl::~splam_client_impl()
{
	m_client.requestStop("Mappa");
	m_client.requestStop("Others");
	m_client.requestStop("Victim");
	m_client.remHandler("Mappa",&MapDataReceiveCB);
	m_client.remHandler("Others",&OthersDataReceiveCB);
	m_client.remHandler("Victim",&VictimDataReceiveCB);
	m_client.stopRunning();
	m_client.join();
	stopRunning();
	join();
	delete slamDataNet_;
}

//---------------------------------------------------------------------------
void	splam_client_impl::register_to()
{
	m_client.addHandler("Mappa",&MapDataReceiveCB);
	m_client.addHandler("Others",&OthersDataReceiveCB);
	m_client.addHandler("Victim",&VictimDataReceiveCB);
	m_client.request("Mappa",200);
	m_client.request("Others",200);
	m_client.request("Victim",200,&victimPack);
}

//---------------------------------------------------------------------------
void	splam_client_impl::MapDataReceive(ArNetPacket* packet)
{
	//Lo slamData_ non deve essere letto mentre viene acquisito.
	slamDataMutex_.lock();

	//Acquisizione dei dati
	slamDataNet_->importMap(packet);

	//cout << "posizione localizzata: "<< slamData_.path_.back().pos.x<< "  "<< slamData_.path_.back().pos.y<< "  "<<slamData_.path_.back().rot<< endl;
	//cout << "goal temporaneo: "<< slamData_.goalTemp_.pos.x<< "  "<< slamData_.goalTemp_.pos.y<< "  "<<slamData_.goalTemp_.rot<< endl;

	//Rilascio del lock
	slamDataMutex_.unlock();
}

//---------------------------------------------------------------------------
void	splam_client_impl::OthersDataReceive(ArNetPacket* packet)
{
	//Lo slamData_ non deve essere letto mentre viene acquisito.
	slamDataMutex_.lock();

	//Acquisizione dei dati
	slamDataNet_->importOthers(packet);

	//cout << "posizione localizzata: "<< slamData_.path_.back().pos.x<< "  "<< slamData_.path_.back().pos.y<< "  "<<slamData_.path_.back().rot<< endl;
	//cout << "goal temporaneo: "<< slamData_.goalTemp_.pos.x<< "  "<< slamData_.goalTemp_.pos.y<< "  "<<slamData_.goalTemp_.rot<< endl;

	//Rilascio del lock
	slamDataMutex_.unlock();
}

//---------------------------------------------------------------------------
void	splam_client_impl::VictimDataReceive(ArNetPacket* packet)
{
	victims_.first = packet->bufToDouble();
	victims_.second = packet->bufToDouble();
	victimCount_ = packet->bufToByte();
	cout<<"E' ARRIVATA UNA VITTIMAAAAAAAA!!!!   "<<victims_.first<<"   "<<victims_.second<<endl;
	cout<<"Ci sono ora "<<(int)victimCount_<<" vittime"<<endl;
}

//---------------------------------------------------------------------------
void	splam_client_impl::FillSlamData(SlamData &target)
{
	//Lo SlamData non deve essere modificato mentre viene letto
	slamDataMutex_.lock();

	//Essendo la struttura SlamData composta da "value"
	target = slamData_;

	//Rilascio del lock
	slamDataMutex_.unlock();
}

bool	splam_client_impl::IsValid()
{
	bool returnvalue;
	//Lo SlamData non deve essere modificato mentre testiamo la validità dei dati correnti
	slamDataMutex_.lock();

	returnvalue = slamDataNet_->header_ == SlamDataNet::valid_data;

	//Rilascio del lock
	slamDataMutex_.unlock();

	return returnvalue;
}

void		splam_client_impl::SetVictimPosition(DoublePair pos)
{
	ArTypes::Byte header=1;//1=set, 0=default heartbeat
	ArNetPacket pack;
	pack.byteToBuf(header);
	pack.doubleToBuf(pos.first);
	pack.doubleToBuf(pos.second);
	m_client.requestOnce("Victim",&pack);
	cout<<"Ho appena settato una nuova vittima: "<<pos.first<<"  "<<pos.second<<endl;
}

DoublePair	splam_client_impl::GetVictimPosition()
{
	cout<<"Ho appena chiesto la posizione della nuova vittima: "<<victims_.first<<"  "<<victims_.second<<endl;
	return victims_;
}
#endif