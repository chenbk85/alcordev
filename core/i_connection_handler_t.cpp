//---------------------------------------------------------------------------
#include "i_connection_handler_t.h"
#include "alcor/core/config_parser_t.hpp"
//---------------------------------------------------------------------------
namespace all{
	namespace core{
//---------------------------------------------------------------------------
    i_connection_handler_t::i_connection_handler_t(core::i_service_handler* _parent 
                                                , const std::string& _inifile)

		:m_state( netLinkDown::Instance() )
    ,  running(true)
    ,  parent(_parent)

{
  //Aria::init();

	printf("Opening ini config.\n");

  config_parser_t config;
  config.load(core::tags::ini,_inifile);

	printf("Gathering address and port.\n");

  m_addr.hostname = config.get<std::string>("server:hostname","localhost");
  m_addr.port	    = config.get<int>("server:port",11111);	

  printf("Connection Handler\n");
  printf("Host: %s:",m_addr.hostname.c_str() );
	printf("Port: %d.\n",m_addr.port );

  connection_thr.reset(
    new boost::thread 
    (
    boost::bind(
    &i_connection_handler_t::run_thread
    , this) 
    )
    );

}
//---------------------------------------------------------------------------
void i_connection_handler_t::change_state(i_connection_state_t* s)
{
    m_state = s;
}
//---------------------------------------------------------------------------
net_state_t i_connection_handler_t::this_state() const
{
	return (m_state->m_id);
}
//---------------------------------------------------------------------------
void i_connection_handler_t::run_thread()
{
	printf("Client Thread Created!\n");;
	while( running )
	{
	m_state->handle( dynamic_cast<i_connection_handler_t*>(this) );
		Sleep(100);
	}
}

//---------------------------------------------------------------------------
//###########################################################################
//---------------------------------------------------------------------------
i_connection_state_t::i_connection_state_t(net_state_t _sid):
	m_id(_sid)
{
}
//---------------------------------------------------------------------------
void i_connection_state_t::change_state(i_connection_handler_t* cxt, i_connection_state_t* is)
{
	cxt->change_state(is);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
netEstablished* netEstablished::m_instance = 0;
//---------------------------------------------------------------------------
netEstablished::netEstablished():
			i_connection_state_t(eEstablished)
{
}
//---------------------------------------------------------------------------
netEstablished* netEstablished::Instance()
{
	if(m_instance == 0){
		m_instance = new netEstablished;
	}
	return m_instance;
}
//---------------------------------------------------------------------------
void netEstablished::handle(i_connection_handler_t* cxt)
{
  cxt->parent->register_to();

	cxt->m_client.logDataList();

	cxt->m_client.runAsync();

	printf("Established ----> ActiveOpen\n");

	change_state(cxt,netActiveOpen::Instance());
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
netActiveOpen* netActiveOpen::m_instance = 0;
//---------------------------------------------------------------------------
netActiveOpen::netActiveOpen():
			i_connection_state_t(eActiveOpen)
{
}
//---------------------------------------------------------------------------
netActiveOpen* netActiveOpen::Instance()
{
	if(m_instance == 0){
		m_instance = new netActiveOpen;
	}
	return m_instance;
}
//---------------------------------------------------------------------------
void netActiveOpen::handle(i_connection_handler_t* cxt)
{

	if( !(cxt->m_client.isConnected())  )
  {
		printf("ActiveOpen ----->LinkDown\n");
    cxt->parent->lost_connection(); 
		change_state(cxt, netLinkDown::Instance());
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
netLinkDown* netLinkDown::m_instance = 0;
//---------------------------------------------------------------------------
netLinkDown::netLinkDown():
		i_connection_state_t(eLinkDown)
{
}
//---------------------------------------------------------------------------
netLinkDown* netLinkDown::Instance()
{
	if(m_instance == 0){
		m_instance = new netLinkDown;
	}
	return m_instance;
}
//---------------------------------------------------------------------------
void netLinkDown::handle(i_connection_handler_t* cxt)
{
	printf("netLinkDown\n");
  if(cxt->m_client.blockingConnect(cxt->m_addr.hostname.c_str(), cxt->m_addr.port) )
	{
		printf("LinkDown ----> Established\n");
    printf("On : %s:%d",cxt->m_addr.hostname.c_str(), cxt->m_addr.port );
		change_state(cxt,netEstablished::Instance());
	}
	else
    printf("Server offline on address: %s : port %d\n", 
    cxt->m_addr.hostname.c_str(),cxt->m_addr.port); 
}
//---------------------------------------------------------------------------
}
}
//---------------------------------------------------------------------------


