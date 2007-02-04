#ifndef i_connection_handler_t_H_INCLUDED
#define i_connection_handler_t_H_INCLUDED
//###########################################################################
#include <Aria.h>
#include <ArNetworking.h>
//---------------------------------------------------------------------------
#include <boost\shared_ptr.hpp>
#include <boost\thread.hpp>
//---------------------------------------------------------------------------
#include <boost/function.hpp>
#include <boost/bind.hpp>
//---------------------------------------------------------------------------
#include "alcor/core/core.h"
//###########################################################################
//---------------------------------------------------------------------------
namespace all{
	namespace core{
//---------------------------------------------------------------------------
class i_connection_state_t;
////---------------------------------------------------------------------------+
//---------------------------------------------------------------------------+
class i_connection_handler_t
{
public:
  i_connection_handler_t(core::i_service_handler*, const std::string&);

public:
	net_state_t this_state() const;

private:
  void change_state(i_connection_state_t*);

public:
	/////callback handlers
 // boost::function< void (void) > register_to;

	/////Called when disconnected
 // boost::function< void (void) > lost_connection;


public:
	//alcor::types::tServiceAddr 
  all::core::ip_address_t m_addr;
  ///
	ArClientBase m_client;
  ///
  core::i_service_handler* parent;
  ///
  i_connection_state_t* m_state;    
  ///
	friend class i_connection_state_t;

public:

  bool running;
	//Non fa altro che chiamare il metodo handle dello stato corrente...
	void run_thread();

private:
  ///
  boost::shared_ptr<boost::thread> connection_thr;
};
//---------------------------------------------------------------------------
class i_connection_state_t
{
public:
	i_connection_state_t(net_state_t);

public:
	net_state_t m_id;

public:
    virtual void handle(i_connection_handler_t*) = 0;

protected:
    virtual  void change_state(i_connection_handler_t* ,i_connection_state_t*);
};
//---------------------------------------------------------------------------
class netEstablished : public i_connection_state_t
{
public:
    static netEstablished* Instance();

public:
    virtual void handle(i_connection_handler_t*);

protected:
    netEstablished();

private:
	static netEstablished* m_instance;  
};
//---------------------------------------------------------------------------
class netActiveOpen : public i_connection_state_t
{
public:
    static netActiveOpen* Instance();

public:
    virtual void handle(i_connection_handler_t*);

protected:
    netActiveOpen();

private:
	static netActiveOpen* m_instance;  
};
//---------------------------------------------------------------------------
class netLinkDown : public i_connection_state_t
{
public:
    static netLinkDown* Instance();

public:
    virtual void handle(i_connection_handler_t*);

protected:
    netLinkDown();
 
private:
	static netLinkDown* m_instance;  
};
//---------------------------------------------------------------------------
	}//nspace
}//nspace
//---------------------------------------------------------------------------
#endif //i_connection_state_t_H_INCLUDED

