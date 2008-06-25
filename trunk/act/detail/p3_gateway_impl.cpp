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

  typedef enum
  {
    P3DX,
    P3AT
  }p3_t;

	///Serial Connection
	bool serial_connect(char*);
	///TCP Connection
    bool tcp_connect(char*, int);
	//
	bool blocking_connect();
  ///
  void robot_run();

  //[Limits]
  void init_robot_settings(iniWrapper&);

  //[ACTIONS]
  void init_follow_action(iniWrapper&);//follow
  void set_target(const math::point2d& reltarget , double mmpersecs);//set_follow

  void init_wander_action(iniWrapper&);//wander
  void init_stop_action();//stop
  void init_goto_action(iniWrapper&);//goto
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
  ///
  p3_t              m_type;

  // Ini

  iniWrapper* iniFile;
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
	iniFile = &ini;

  if (is_p3dx)
  {
    m_robot->loadParamFile("config/p3dx.p");
    m_type = P3DX;
  }
  else
  {
    m_type = P3AT;
  }

  //Connect  sonars al robot
  m_robot->addRangeDevice(&m_sonar);

  ///
 init_robot_settings(ini);

  //Init actions
  ///Stop
  init_stop_action();

  ///
  init_wander_action(ini);

  ///Follow
  init_follow_action(ini);

  ///
  init_goto_action(ini);

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
	///TCP Connection
inline bool p3_gateway_impl::tcp_connect(char* hostName, int port)
	{
		printf("Attempting %s tcp connection on port: %d\n",m_robot->getName(), port);
		// connection to the robot	
		
		m_tcpConn.setPort();
		if(!m_tcpConn.openSimple() )
		{
			printf( "%s TCP Connection failed on port %d on host %s!!\n", m_robot->getName(), port, m_tcpConn.getHost());
			return false;    
		}

		
		m_robot->setDeviceConnection(&m_tcpConn);

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
  m_robot->setRotVelMax(5);
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
inline void p3_gateway_impl::init_wander_action(iniWrapper& ini)
{
  		
  int distance = ini.GetInt("p3at_follow_far:distance", 300);
  int velocity = ini.GetInt("p3at_follow_far:velocity", 150);
  int turn = ini.GetInt("p3at_follow_far:turn", 5);
  int priority = ini.GetInt("p3at_follow_far:priority", 70);

  m_wander.reset( new ArActionGroupWander(m_robot, 150, 80, 0, 5));
/*

  m_wander.reset(new 	ArActionGroupStop(m_robot));

  //TODO enable external ini files...
  m_wander.reset(new ArActionGroup(m_robot));

  // if we're stalled we want to back up and recover
  m_wander->addAction(new ArActionStallRecover, 100);

  // avoid things closer to us
  //m_wander->addAction(new ArActionAvoidFront("Avoid Front Near", 225, 0), 50);
  m_wander->addAction(new ArActionAvoidFront("Avoid Front Near", 225, 0, 3), 50);

  // avoid things further away
  //m_wander->addAction(new ArActionAvoidFront, 45);
  m_wander->addAction(new ArActionAvoidFront("Avoid Front Near", 450, 200, 3), 45);

  // keep moving
  m_wander->addAction(new ArActionConstantVelocity("Constant Velocity", 150), 25);
*/
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

  ////drive toward the target
  m_follow->addAction(new ArActionGotoStraight("myfollowtarget", 0) ,priority);


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
inline void p3_gateway_impl::init_goto_action(iniWrapper& ini)
{
	int velocity, turn, priority, distance;

  m_goto.reset(new ArActionGroup(m_robot));

  // if we're stalled we want to back up and recover
  m_goto->addAction(new ArActionStallRecover, 100);

  ///////////////////////////////////////////////////////////////////
  if(m_type==P3DX)
  {
  distance  = ini.GetInt("p3dx/goto:frontnear_distance", 100);
  velocity  = ini.GetInt("p3dx/goto:frontnear_velocity", 0);
  turn      = ini.GetInt("p3dx/goto:frontnear_turn", 5);
  priority  = ini.GetInt("p3dx/goto:frontnear_priority",95);
  }
  else
  {
  distance  = ini.GetInt("p3at/goto:frontnear_distance", 100);
  velocity  = ini.GetInt("p3at/goto:frontnear_velocity", 0);
  turn      = ini.GetInt("p3at/goto:frontnear_turn", 5);
  priority  = ini.GetInt("p3at/goto:frontnear_priority",95);
  }
  // turn avoid things close to us
  //frontnear_distance, vel, turn
  m_goto->addAction
    (new ArActionAvoidFront("avoid_front_near", distance, velocity, turn), priority);
  ///////////////////////////////////////////////////////////////////
  // turn avoid things further away
  if(m_type==P3DX)
  {
  distance  = ini.GetInt("p3dx/goto:frontfar_distance", 350);
  velocity  = ini.GetInt("p3dx/goto:frontfar_velocity", 100);
  turn      = ini.GetInt("p3dx/goto:frontfar_turn", 5);
  priority  = ini.GetInt("p3dx/goto:frontfar_priority",90);
  }
  else
  {
  distance  = ini.GetInt("p3at/goto:frontfar_distance", 350);
  velocity  = ini.GetInt("p3at/goto:frontfar_velocity", 100);
  turn      = ini.GetInt("p3at/goto:frontfar_turn", 5);
  priority  = ini.GetInt("p3at/goto:frontfar_priority",90);
  }
  m_goto->addAction(new ArActionAvoidFront
        ("avoid_front_far",distance, velocity, turn), priority);
  ///////////////////////////////////////////////////////////////////
  //// avoid side
  if(m_type==P3DX)
  {
  distance  = ini.GetInt("p3dx/side:side_distance", 250);
  turn      = ini.GetInt("p3dx/side:side_turn", 5);
  priority  = ini.GetInt("p3dx/side:side_priority",80);
  }
  else
  {
    distance = 150;
    velocity = 100;
    priority = 90;
  //distance  = ini.GetInt("p3at:goto:frontfar_distance", 350);
  //velocity  = ini.GetInt("p3at:goto:frontfar_velocity", 100);
  //turn      = ini.GetInt("p3at:goto:frontfar_turn", 5);
  //priority  = ini.GetInt("p3at:goto:frontfar_priority",90);
  }
  printf("goto:side:  distance:%d turn: %d prior: %d\n", distance, turn, priority);
  m_goto->addAction(new ArActionAvoidSide("side_avoid", distance, turn), priority);

  ///////////////////////////////////////////////////////////////////
  int speed_to_turn_at = 0;
  if(m_type==P3DX)
  {
  distance  = ini.GetInt("p3dx/goto:mygoto_closedist", 100);
  velocity  = ini.GetInt("p3dx/goto:mygoto_speed", 200);
  speed_to_turn_at = 
    ini.GetInt("p3dx/goto:mygoto_speed_to_turn_at", 100);
  turn      = ini.GetInt("p3dx/goto:mygoto_turn", 5);
  priority  = ini.GetInt("p3dx/:goto:mygoto_priority",90);
  }
  else
  {
  distance  = ini.GetInt("p3at/goto:mygoto_closedist", 100);
  velocity  = ini.GetInt("p3at/goto:mygoto_speed", 200);
  speed_to_turn_at = 
    ini.GetInt("p3at/goto:mygoto_speed_to_turn_at", 100);
  turn      = ini.GetInt("p3at/goto:mygoto_turn", 5);
  priority  = ini.GetInt("p3at/goto:mygoto_priority",90);
  }
  //
  m_goto->addAction(new ArActionGoto
    ("mygoto", ArPose(), distance, velocity, speed_to_turn_at, turn), priority );
}
//---------------------------------------------------------------------------
inline void p3_gateway_impl::set_target(const math::point2d& reltarget , double mmpersecs)
{
  m_robot->lock(); 
  ArActionGotoStraight* mygoto 
    = (ArActionGotoStraight*) m_robot->findAction("myfollowtarget");

  if(mygoto)
  {

  printf(
      "Target dist: %f theta: %f\n", 
      reltarget.magnitude()*1000.0
    , reltarget.orientation().deg());

  //
  printf("Speed: %f\n", mmpersecs);
  //
  mygoto->setSpeed(mmpersecs);

  mygoto->setGoalRel(  
                    reltarget.magnitude()*1000.0
                  , reltarget.orientation().deg()
                  , false, false
                  );
  }

  m_robot->unlock();
}
//---------------------------------------------------------------------------
inline void p3_gateway_impl::set_goto_pose(const math::point2d& reltarget , double mmpersecs)
{   
  m_robot->lock(); 
  ArActionGoto* mygoto = (ArActionGoto*) m_robot->findAction("mygoto");

  if(mygoto)
  {
  //printf("p3_gateway_impl::set_goto_pose\n");

  //pose is meters, degree (relative pose)
  //ArPose is mm, degrees
 
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

  //update rel goal ...
  mygoto->setGoal(newgoal);
  //update speed ...
  mygoto->setSpeed(mmpersecs);

  //
  printf("---------------------------\n", reltarget.magnitude()*1000.0, reltarget.orientation().deg());
  printf("Goal dist: %f theta: %f\n", reltarget.magnitude()*1000.0, reltarget.orientation().deg());
  printf("Speed (mmpersecs): %f\n", mmpersecs);
  printf("ArCurrentPose: X: %.2f Y %.2f Th:%.2f\n", current.getX(), current.getY(),current.getTh());
  printf("ArGoal: X: %.2f Y %.2f Th:%.2f\n\n", newgoal.getX(), newgoal.getY(), newgoal.getTh());

  //mygoto->log();
  //
  }  
  m_robot->unlock();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
}}}//all::act::detail
//---------------------------------------------------------------------------