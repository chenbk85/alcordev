//---------------------------------------------------------------------------
#include "i_connection_handler.h"
//---------------------------------------------------------------------------
namespace alcor{
	namespace interfaces{
//---------------------------------------------------------------------------
i_connection_handler::i_connection_handler(char* _inifile):
		m_state( netLinkDown::Instance() )
{
	printf("Opening ini config.\n");

  if (m_config_ini.Load(_inifile))
		{
		printf("Gathering address and port.\n");

		//m_addr.hostname = "localhost";
		m_addr.hostname = m_config_ini.GetStringAsChar("server:hostname","localhost");
		m_addr.port	= m_config_ini.GetInt("server:port",99999);	

		printf("Host: %s:",m_addr.hostname );
		printf("Port: %d.\n",m_addr.port );

		}
	else
	{
		//file is non existent ...
		m_addr.hostname	= "localhost";
		m_addr.port		= 99999;
	}

}
//---------------------------------------------------------------------------
void i_connection_handler::changeState(i_connection_state* s)
{
    m_state = s;
}
//---------------------------------------------------------------------------
tStateId i_connection_handler::thisState()
{
	return (m_state->m_id);
}
//---------------------------------------------------------------------------
void* i_connection_handler::runThread(void*)
{
	cout << "Client Thread Created!" << endl;
	while( this->ArASyncTask::getRunning() )
	{
	m_state->handle( dynamic_cast<i_connection_handler*>(this) );
		ArUtil::sleep(50);
	}
	return NULL;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
i_connection_state::i_connection_state(tStateId _sid):
	m_id(_sid)
{
}
//---------------------------------------------------------------------------
void i_connection_state::changeState
				(i_connection_handler* cxt, i_connection_state* is)
{
	cxt->changeState(is);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
netEstablished* netEstablished::m_instance = 0;
//---------------------------------------------------------------------------
netEstablished::netEstablished():
			i_connection_state(alcor::def::eEstablished)
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
void netEstablished::handle(i_connection_handler* cxt)
{
	cxt->register_to();

	cxt->m_client.logDataList();

	cxt->m_client.runAsync();

	cout << "Established ----> ActiveOpen" << endl;

	changeState(cxt,netActiveOpen::Instance());
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
netActiveOpen* netActiveOpen::m_instance = 0;
//---------------------------------------------------------------------------
netActiveOpen::netActiveOpen():
			i_connection_state(alcor::def::eActiveOpen)
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
void netActiveOpen::handle(i_connection_handler* cxt)
{
	if( !(cxt->m_client.isConnected())  ){
		cout << "ActiveOpen ----->LinkDown" << endl;
		cxt->lost_connection();
		changeState(cxt, netLinkDown::Instance());
	}

}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
netLinkDown* netLinkDown::m_instance = 0;
//---------------------------------------------------------------------------
netLinkDown::netLinkDown():
		i_connection_state(alcor::def::eLinkDown)
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
void netLinkDown::handle(i_connection_handler* cxt)
{
	cout << "netLinkDown" << endl;
	if(cxt->m_client.blockingConnect(cxt->m_addr.hostname, cxt->m_addr.port) )
	{
		cout << "LinkDown ----> Established" << endl;
		changeState(cxt,netEstablished::Instance());
	}
	else
		cout << "Server offline on address: " 
		<< cxt->m_addr.hostname << ":"
			<< cxt->m_addr.port 
		<< endl;
}
//---------------------------------------------------------------------------
}
}
//---------------------------------------------------------------------------


