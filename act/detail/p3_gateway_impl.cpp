//---------------------------------------------------------------------------
#include <Aria.h>
#include <ArNetworking.h>
//---------------------------------------------------------------------------
#include <memory>
using std::auto_ptr;
//---------------------------------------------------------------------------
#include "alcor/core/core.h"
#include "alcor/core/iniWrapper.h"
#include "alcor/math/point2d.h"
//---------------------------------------------------------------------------
namespace all { namespace act { namespace detail {
struct p3_gateway_impl
{
  ///weird... no better solution now...
  p3_gateway_impl(bool is_p3dx, iniWrapper&);

  ~p3_gateway_impl();

	///Serial Connection
	bool serial_connect(char*);
	///TCP Connection
  bool tcp_connect(char*, int){return true;};
	//
	bool blocking_connect();
  ///
  void robot_run();

  //[Limits]
  void init_robot_settings(iniWrapper&);

  //[ACTIONS]
  void init_follow_action(iniWrapper&);//follow
  void init_wander_action();//wander
  void init_stop_action();//stop
  void init_goto_action();//goto
  void set_goto_pose(const math::point2d& reltarget , double mmpersecs);//set_goto

  //FOLLOW ActionGroup ---------------------
  auto_ptr<ArActionGroup>         m_follow;
 

  //keep a pointer to set the current target
  auto_ptr<ArActionGotoStraight>  m_ac_follow;

  //WANDER ActionGroup ---------------------
  auto_ptr<ArActionGroup>         m_wander;

  //STOP ActionGroup -----------------------
	auto_ptr<ArActionGroup>         m_stop;

  //GOTO ACtionGroup -----------------------
  auto_ptr<ArActionGroup>     m_goto;
  auto_ptr<ArActionGoto>      m_action_goto;
  //----------------------------------------
  ///connections
	ArTcpConnection 	  m_tcpConn;
	ArSerialConnection 	m_serialConn;	
  ///Sonars
	ArSonarDevice		    m_sonar;
  ///
	ArRobot*          m_robot;	
};

//###########################################################################
//IMPL
//###########################################################################
inline p3_gateway_impl::p3_gateway_impl(bool is_p3dx, iniWrapper& ini)
{
  	//Mandatory init ..
	Aria::init();
	//
	m_robot = new ArRobot;


  if (is_p3dx)
    m_robot->loadParamFile("config/p3dx.p");

  //Connect  sonars al robot
  m_robot->addRangeDevice(&m_sonar);

  ///
   init_robot_settings(ini);

  //Init actions
  ///Stop
  init_stop_action();

  ///
  init_wander_action();

  ///Follow
  init_follow_action(ini);

  ///
  init_goto_action();

  //
	m_robot->logActions();

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
inline void p3_gateway_impl::init_robot_settings(iniWrapper& ini)
{
  m_robot->setRotVelMax(10);
}
//---------------------------------------------------------------------------
//ACTIONS!!
//---------------------------------------------------------------------------
inline void p3_gateway_impl::init_stop_action() 
{
   //Stop Action ...
  m_stop.reset(new 	ArActionGroupStop(m_robot));
}
//---------------------------------------------------------------------------
inline void p3_gateway_impl::init_wander_action()
{

	int velocity, turn, priority, distance;

  m_wander.reset(new 	ArActionGroupStop(m_robot));

  //TODO enable external ini files...
  m_wander.reset(new ArActionGroup(m_robot));

  // if we're stalled we want to back up and recover
  m_wander->addAction(new ArActionStallRecover, 100);

  // avoid things closer to us
  m_wander->addAction(new ArActionAvoidFront("Avoid Front Near", 225, 0), 50);

  // avoid things further away
  m_wander->addAction(new ArActionAvoidFront, 45);

  // keep moving
  m_wander->addAction(new ArActionConstantVelocity("Constant Velocity", 150), 25);
}
//---------------------------------------------------------------------------
inline void p3_gateway_impl::init_follow_action(iniWrapper& ini)
{
  
	int velocity, turn, priority, distance;
	//printf("init_follow_action\n");
	//TODO: enable external ini files...
  m_follow.reset(new ArActionGroup(m_robot));

  // if we're stalled we want to back up and recover
  m_follow->addAction(new ArActionStallRecover, 100);

  //m_follow->addAction( new ArActionLimiterForwards("limiter", 150, 0, 0, 1.3), 90);

				   //double stopDistance = 250,
				   //double slowDistance = 1000,
				   //double slowSpeed = 200,
				   //double widthRatio = 1);

  velocity = ini.GetInt("p3at_follow_straight:velocity", 0);
  priority = ini.GetInt("p3at_follow_straight:priority", 50);

  	//printf("init_follow_action4\n");
  m_ac_follow.reset(new ArActionGotoStraight("target", velocity) );
  m_ac_follow->setCloseDist(100);
  m_ac_follow->setSpeed(0);

  ////drive toward the target
  m_follow->addAction(m_ac_follow.get(), priority);

  //distance = ini.GetInt("p3at_follow_near:distance", 100);
  //velocity = ini.GetInt("p3at_follow_near:velocity", 0);
  //turn = ini.GetInt("p3at_follow_near:turn", 5);
  //priority = ini.GetInt("p3at_follow_near:priority", 60);
  
	//printf("init_follow_action6");
  // avoid things closer to us
  //m_follow->addAction(new ArActionAvoidFront("Avoid Front Near", distance, velocity, turn), priority);

  distance = ini.GetInt("p3at_follow_far:distance", 300);
  velocity = ini.GetInt("p3at_follow_far:velocity", 150);
  turn = ini.GetInt("p3at_follow_far:turn", 5);
  priority = ini.GetInt("p3at_follow_far:priority", 70);

  // avoid things further away
  m_follow->addAction(new ArActionAvoidFront("Avoid Front Far", distance, velocity, turn), priority);

  //distance = ini.GetInt("p3at_follow_far:distance", 300);
  //turn = ini.GetInt("p3at_follow_far:turn", 5);
  //priority = ini.GetInt("p3at_follow_far:priority", 60);

  //// avoid side
  //m_follow->addAction(new ArActionAvoidSide("Side Avoid", distance, turn), priority);

   //m_ac_follow->setGoalRel(0,0,false, false);
  //  // keep moving
}
//---------------------------------------------------------------------------
inline void p3_gateway_impl::init_goto_action()
{
	int velocity, turn, priority, distance;

  m_goto.reset(new ArActionGroup(m_robot));

  // if we're stalled we want to back up and recover
  //m_goto->addAction(new ArActionStallRecover, 100);

    // turn to avoid things closer to us
  m_goto->addAction(new ArActionAvoidFront("Avoid Front Near", 200, 0), 95);

  // turn avoid things further away
  //m_goto->addAction(new ArActionAvoidFront, 90);

  //goal, closeDist, speed, speedToTurn, turn
  m_action_goto.reset( new ArActionGoto("goto", ArPose(), 10, 200, 150,7) );
  ///  
  m_robot->addAction(m_action_goto.get(), 50);

}
//---------------------------------------------------------------------------
inline void p3_gateway_impl::set_goto_pose(const math::point2d& reltarget , double mmpersecs)
{
  printf("p3_gateway_impl::set_goto_pose\n");
  //pose is meters, degree (relative pose)
  //ArPose is mm, degrees
  m_robot->lock();  
  //correct with relative
  ArPose newgoal;
  ArPose current = m_robot->getPose();
  //
  double xoffset = reltarget.magnitude()*reltarget.orientation().cos();
  double yoffset = reltarget.magnitude()*reltarget.orientation().sin();
  //
  newgoal.setX(static_cast<int>(current.getX() + xoffset*1000.0));
  newgoal.setY(static_cast<int>(current.getY() + yoffset*1000.0));
  newgoal.setTh(ArMath::addAngle(reltarget.orientation().deg(), current.getTh() ));
  //
  m_action_goto->setGoal(newgoal);
  m_action_goto->setSpeed(mmpersecs);

  //
  printf("ArCurrentPose: X: %.2f Y %.2f\n", current.getX(), current.getY());
  printf("ArGoal: X: %.2f Y %.2f\n", newgoal.getX(), newgoal.getY());

  //
  m_robot->unlock();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
}}}//all::act::detail
//---------------------------------------------------------------------------