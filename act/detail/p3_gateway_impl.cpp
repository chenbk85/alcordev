//---------------------------------------------------------------------------
#include <Aria.h>
#include <ArNetworking.h>
//---------------------------------------------------------------------------
#include <memory>
using std::auto_ptr;
//---------------------------------------------------------------------------
#include "alcor/core/core.h"
//---------------------------------------------------------------------------
namespace all { namespace act { namespace detail {
struct p3_gateway_impl
{
  ///
  p3_gateway_impl();

  ~p3_gateway_impl();

	///Serial Connection
	bool serial_connect(char*);
	///TCP Connection
  bool tcp_connect(char*, int){return true;};
	//
	bool blocking_connect();
  ///
  void robot_run();

  //[ACTIONS]
  void init_follow_action();
  void init_stop_action();

  //FOLLOW ActionGroup ---------------------
  auto_ptr<ArActionGroup>         m_follow;
  //keep a pointer to set the current target
  auto_ptr<ArActionGotoStraight>  m_ac_follow;

  //STOP ActionGroup -----------------------
	auto_ptr<ArActionGroup> m_stop;

  ///connections
	ArTcpConnection 	  m_tcpConn;
	ArSerialConnection 	m_serialConn;	
  ///Sonars
	ArSonarDevice		    m_sonar;
  /////unused
  //all::core::ip_address_t m_addr;
	ArRobot*      m_robot;	
};

//###########################################################################
//IMPL
//###########################################################################
inline p3_gateway_impl::p3_gateway_impl()
{
  	//Mandatory init ..
	Aria::init();
	//
	m_robot = new ArRobot;

  m_robot->loadParamFile("config/p3dx.p");

  //Connect  sonars al robot
  m_robot->addRangeDevice(&m_sonar);

  //Init actions
  ///Stop
  init_stop_action();
  ///Follow
  init_follow_action();

	//Starts with actions deactivated ...
	m_robot->deactivateActions();
}  
//---------------------------------------------------------------------------
inline p3_gateway_impl::~p3_gateway_impl()
{
	if(m_robot)  
			m_robot->stopRunning();

  Aria::shutdown();
}
//---------------------------------------------------------------------------
	///Serial Connection
inline bool p3_gateway_impl::serial_connect(char* comPort)
	{
    //std::string comPort = "COM";
    //comPort += core::make_string(m_serial_port);
		printf("Attempting %s connection on port: %s\n",m_robot->getName(), comPort);
		// connection to the robot		
    m_serialConn.setPort(comPort);
    
		if(!m_serialConn.openSimple() )
		{
			printf( "%s SERIAL Connection failed on port %s !!\n", m_robot->getName(),  
				comPort);
			return false;    
		}

    //
    m_robot->setDeviceConnection(&m_serialConn);

		// try to connect, if we fail, the connection handler should bail
		if (!blocking_connect())
		{
			return false;
		}
	
  robot_run();

		return true;
	}
//---------------------------------------------------------------------------
inline void p3_gateway_impl::robot_run()
{    
  // run the robot in its own thread, so it gets and processes packets and such
  m_robot->loadParamFile("config/p3dx.p");
  m_robot->runAsync(false);
  m_robot->enableMotors();
}
//---------------------------------------------------------------------------
inline bool p3_gateway_impl::blocking_connect()
	{	
	  // try to connect, if we fail, the connection handler should bail
  if (!m_robot->blockingConnect())
  {
    // this should have been taken care of by the connection handler
    // but just in case
    printf(
    "asyncConnect failed because robot is not running in its own thread.\n");
    Aria::shutdown();
    return false;
  }
  return true;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//ACTIONS!!
//---------------------------------------------------------------------------
inline void p3_gateway_impl::init_stop_action() 
{
   //Stop Action ...
  m_stop.reset(new 	ArActionGroupStop(m_robot));
}
//---------------------------------------------------------------------------
inline void p3_gateway_impl::init_follow_action()
{
  //TODO: enable external ini files...
  m_follow.reset(new ArActionGroup(m_robot));

  // if we're stalled we want to back up and recover
  m_follow->addAction(new ArActionStallRecover, 100);

  //create action
  m_ac_follow.reset(new ArActionGotoStraight("target",250) );

  //
  m_follow->addAction(m_ac_follow.get(), 49);

  // avoid things closer to us
  m_follow->addAction(new ArActionAvoidFront("Avoid Front Near", 200, 0), 50);

  // avoid things further away
  m_follow->addAction(new ArActionAvoidFront, 45);

  // keep moving
  //m_follow->addAction(new ArActionConstantVelocity("Constant Velocity", 100), 25);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
}}}//all::act::detail
//---------------------------------------------------------------------------