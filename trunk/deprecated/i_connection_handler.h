#ifndef i_connection_handler_H_INCLUDED_DEPRECATE
#define i_connection_handler_H_INCLUDED_DEPRECATE
//---------------------------------------------------------------------------
#include "alcor/deprecated/CommonDef.h"
#include "alcor/core/iniWrapper.h"
//---------------------------------------------------------------------------
using namespace alcor::types;
//---------------------------------------------------------------------------
namespace alcor{
	namespace interfaces{
//---------------------------------------------------------------------------
class i_connection_state;
//---------------------------------------------------------------------------+
//---------------------------------------------------------------------------+
class i_connection_handler : public ArASyncTask
{
public:
	i_connection_handler( char* inifile);

public:
	virtual tStateId thisState();

private:

    void changeState(i_connection_state*);
	iniWrapper m_config_ini;

public:
	///register handlers
	virtual void register_to() = 0;
	///Called when disconnected
	virtual void lost_connection()=0;

public:
	alcor::types::tServiceAddr m_addr;

public:	
	ArClientBase m_client;
    i_connection_state* m_state;    
	friend class i_connection_state;

public:
	//Non fa altro che chiamare il metodo handle dello stato corrente...
	void* runThread(void*);
};
//---------------------------------------------------------------------------
class i_connection_state
{
public:
	i_connection_state(tStateId);

public:
	tStateId m_id;

public:
    virtual void handle(i_connection_handler*) = 0;

protected:
    virtual  void changeState(i_connection_handler* ,i_connection_state*);
};
//---------------------------------------------------------------------------
class netEstablished : public i_connection_state
{
public:
    static netEstablished* Instance();

public:
    virtual void handle(i_connection_handler*);

protected:
    netEstablished();

private:
	static netEstablished* m_instance;  
};
//---------------------------------------------------------------------------
class netActiveOpen : public i_connection_state
{
public:
    static netActiveOpen* Instance();

public:
    virtual void handle(i_connection_handler*);

protected:
    netActiveOpen();

private:
	static netActiveOpen* m_instance;  
};
//---------------------------------------------------------------------------
class netLinkDown : public i_connection_state
{
public:
    static netLinkDown* Instance();

public:
    virtual void handle(i_connection_handler*);

protected:
    netLinkDown();
 
private:
	static netLinkDown* m_instance;  
};
//---------------------------------------------------------------------------
	}//nspace
}//nspace
//---------------------------------------------------------------------------
#endif //i_connection_handler_H_INCLUDED

