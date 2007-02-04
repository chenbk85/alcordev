#include "alcor/act/p3dx_gateway.h"
//---------------------------------------------------------------------------
#include <string>
#include <sstream>
//---------------------------------------------------------------------------
#include "alcor/core/config_parser_t.h"
#include "alcor/core/core.h"
//---------------------------------------------------------------------------
namespace all
	{
	namespace act{
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
p3dx_gateway::p3dx_gateway():
			m_serial_port(5)
  		,myConnectedCB(this, &p3dx_gateway::connected)
			,m_error_disconnect_cb(this, &p3dx_gateway::on_error_cb)
			//remote services
			,m_get_odometry_cb (this, &p3dx_gateway::get_odometry_imp)
			,m_set_localized_cb(this, &p3dx_gateway::set_localized_imp)

			,m_set_vel_cb(this, &p3dx_gateway::set_vel_imp)
			,m_set_rot_vel_cb(this, &p3dx_gateway::set_rot_vel_imp)
			,m_stop_cb(this, &p3dx_gateway::stop_imp)
      ,m_set_heading_cb(this, &p3dx_gateway::set_heading_imp)
      ,m_set_delta_heading_cb(this, &p3dx_gateway::set_delta_heading_imp)
      ,m_set_wandermode_cb(this, &p3dx_gateway::set_wandermode_imp)
      ,m_set_localdir_cb(this, &p3dx_gateway::set_localdir_imp)
{
	//Mandatory init ..
	Aria::init();
	//
	m_robot = new ArRobot;
	
  //// let the global aria stuff know about it
  Aria::setKeyHandler(&keyHandler);
  //// toss it on the robot
  m_robot->attachKeyHandler(&keyHandler);
  //printf("You may press escape to exit\n");

	//Connect  sonars al robot
	m_robot->addRangeDevice(&m_sonar);
	
	 // add the callbacks to the robot
	m_robot->addConnectCB(&myConnectedCB, ArListPos::FIRST);
	m_robot->addDisconnectOnErrorCB(&m_error_disconnect_cb,ArListPos::FIRST);

	//server is always localhost
	m_addr.hostname = "localhost";
  core::config_parser_t config;
  config.load(core::ini,"config/p3dx_conf.ini");

	m_addr.port		  = config.as_int("server.port",22222);
	m_goto_dir_dist = config.as_int("params.m_goto_dir_dist",1000);

	//serial port (if any)
	m_serial_port = config.as_int("serial.port",6);

	//
	init_wander_action();
	//	
	init_stop_action();
	//	
	init_goto_dir();

	//Starts with actions deactivated ...
	m_robot->deactivateActions();
}
//---------------------------------------------------------------------------
p3dx_gateway::~p3dx_gateway()
	{
	m_server.stopRunning();

		if(m_robot)  
			m_robot->stopRunning();
	
		Aria::shutdown();
	}
//---------------------------------------------------------------------------
	///Serial Connection
bool p3dx_gateway::serial_connect()
	{
    std::string comPort = "COM";
    comPort += core::make_string(m_serial_port);
		printf("Attempting Pioneer connection on port: %s\n",comPort.c_str());
		// connection to the robot		
    m_serialConn.setPort(comPort.c_str());
    
    //SPOSTIAMOLO
		//m_robot->setDeviceConnection(&m_serialConn);
		if(!m_serialConn.openSimple() )
		{
			printf( "Doro SERIAL Connection failed on port %s !!\n",  
				comPort.c_str());
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
bool p3dx_gateway::tcp_connect(char* addr, int port)
	{
		if (m_tcpConn.open() != 0)
    	{
      	printf( "Doro TCP Connection failed !!" );
      	return false;
      }
		m_robot->setDeviceConnection(&m_tcpConn);
		return true;
	}
//---------------------------------------------------------------------------
bool p3dx_gateway::blocking_connect()
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
void p3dx_gateway::robot_run()
{
  // run the robot in its own thread, so it gets and processes packets and such
  m_robot->runAsync(false);
  m_robot->enableMotors();
  start_server();
  m_robot->waitForRunExit();
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//								SERVER SIDE
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
void p3dx_gateway::start_server()
	{
	// first open the server up
	if (!m_server.open(m_addr.port))
		{
			printf("Could not open server port: %d\n", m_addr.port );
			exit(1);
		}

	//[Assign Callbacks]
	m_server.addData("getOdometry", "Get non localized odometry from Pioneer OS", 
						&m_get_odometry_cb, "none", "none");
		//[Assign Callbacks]
	m_server.addData("setLocalized", "Set Localized 2D Odometry", 
						&m_set_localized_cb, "none", "none");
		//[Assign Callbacks]
	m_server.addData("setvel", "SetVel", 
						&m_set_vel_cb, "none", "none");
		//[Assign Callbacks]
	m_server.addData("setheading", "Set Heading", 
    &m_set_heading_cb, "none", "none");

		//[Assign Callbacks]
	m_server.addData("setdeltaheading", "Set Delta Heading", 
    &m_set_delta_heading_cb, "none", "none");

			//[Assign Callbacks]
	m_server.addData("setrotvel", "SetRotVel", 
						&m_set_rot_vel_cb, "none", "none");

	m_server.addData("stop", "Stop", 
						&m_stop_cb, "none", "none");
  //ACTIONS
	m_server.addData("wander", "Set Wander Action", 
						&m_set_wandermode_cb, "none", "none");

	m_server.addData("setlocaldir", "Set Local Dir Action", 
    &m_set_localdir_cb, "none", "Delta angle in degrees");

	m_server.runAsync();

	}
//---------------------------------------------------------------------------
void p3dx_gateway::get_odometry_imp(ArServerClient* cl, ArNetPacket* msg)
	{
		ArNetPacket pkt;
    //TODO: check This!!
    //quale è il default?
    //millimetri+gradi??
    //m_odometry.setX( math::metric(m_robot->getPose().getX(),math::metric::Millimeters() ) );
    //m_odometry.setY( math::metric(m_robot->getPose().getY(),math::metric::Millimeters() ));
    //m_odometry.setTh(math::angle(m_robot->getPose().getTh(),math::deg_t));

    m_odometry.set_x1(m_robot->getPose().getX()/1000.0);
    m_odometry.set_x2(m_robot->getPose().getY()/1000.0);
    m_odometry.set_th(m_robot->getPose().getTh(), math::deg_tag);

		m_odometry.pack(&pkt);
		cl->sendPacketTcp(&pkt);
	}
//---------------------------------------------------------------------------
void p3dx_gateway::set_localized_imp(ArServerClient* cl, ArNetPacket* msg)
	{
		//printf("Localized Position received\n");
		//m_localized.import(msg);
		//moveTo(m_localized);
	}
//---------------------------------------------------------------------------
void p3dx_gateway::set_vel_imp(ArServerClient* cl, ArNetPacket* msg)
	{
	double setvel = msg->bufToDouble();
	m_robot->setVel(setvel);
	printf("SetVel %f\n", setvel);
	}
//---------------------------------------------------------------------------
void p3dx_gateway::set_rot_vel_imp(ArServerClient* cl, ArNetPacket* msg)
	{
	double setrotvel = msg->bufToDouble();
	m_robot->setRotVel(setrotvel);
	printf("SetRotVel: %f\n", setrotvel);

	}
//---------------------------------------------------------------------------
void p3dx_gateway::stop_imp(ArServerClient* cl, ArNetPacket* msg)
	{
		printf("STOP\n");
		stop();
	}
//---------------------------------------------------------------------------
void p3dx_gateway::set_heading_imp(ArServerClient* cl, ArNetPacket* msg)
	{
	double heading = msg->bufToDouble();
  set_heading(heading);
	printf("Heading: %f\n", heading);
	}
//---------------------------------------------------------------------------
void p3dx_gateway::set_delta_heading_imp(ArServerClient* cl, ArNetPacket* msg)
	{
	double dheading = msg->bufToDouble();
  set_delta_heading(dheading);
	printf("Delta Heading: %f\n", dheading);
	}
//---------------------------------------------------------------------------
void p3dx_gateway::set_wandermode_imp (ArServerClient *, ArNetPacket *)
{
  printf("WANDER\n");
  wander_mode();
}
//---------------------------------------------------------------------------
void p3dx_gateway::set_localdir_imp (ArServerClient *, ArNetPacket* msg)
{
  printf("Set Local Dir\n");
  //DEGREES
  double delta_deg = msg->bufToDouble();
  set_local_dir(delta_deg);
}
//---------------------------------------------------------------------------
p3dx_position_t const& p3dx_gateway::get_localized() const
	{
		return m_localized;
	}
//---------------------------------------------------------------------------
ArPose  p3dx_gateway::get_odometry() const
	{
		return m_robot->getPose();
	}
//---------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void p3dx_gateway::init_wander_action()
{
  //TODO enable external ini files...
  m_wander.reset(new ArActionGroup(m_robot));

  // if we're stalled we want to back up and recover
  m_wander->addAction(new ArActionStallRecover, 100);

  // avoid things closer to us
  m_wander->addAction(new ArActionAvoidFront("Avoid Front Near", 225, 0), 50);

  // avoid things further away
  m_wander->addAction(new ArActionAvoidFront, 45);

  // keep moving
  m_wander->addAction(new ArActionConstantVelocity("Constant Velocity", 100), 25);
}
//---------------------------------------------------------------------------
void p3dx_gateway::init_goto_dir()
{
  ////TODO enable external ini files...
  m_goto_dir.reset(new ArActionGroup(m_robot));

//		name = "stall recover",
//		double 	  obstacleDistance = 225,
//		int 	  cyclesToMove = 50,
//		double 	  speed = 150,
//		double 	  degreesToTurn = 45
//	)  

  //// if we're stalled we want to back up and recover
  m_goto_dir->addAction(new ArActionStallRecover(	"Stallo",
													225,
													50,
													50,
													30), 100);

  // avoid things closer to us
  m_goto_dir->addAction(new ArActionAvoidFront("Avoid Front Near", 
												230, // near distance
												0,  // avoid velocity
												15),  //Turn amount
												50);

  // avoid things further away
  // 450,200,15
  m_goto_dir->addAction(new ArActionAvoidFront("Avoid Far",
												300, //Far distance
												50,  //turn velocity
												12  // turn amount
												),
												40);

  m_goto_dir->addAction(new ArActionAvoidSide("Avoid Side Far",  250, 5)    , 25);
  m_goto_dir->addAction(new ArActionAvoidSide("Avoid Side Close",100,10)    , 40);

  ///
  m_ac_goto_dir.reset( new ArActionGoto(
								"goto_dir", //name
								ArPose(0,0,0), //goal
								1, //closeDist
								100,  //speed
								35,  //speedToTurnAt
								10)); //turnAmount


  //Goto Forcing?
  m_goto_dir->addAction(m_ac_goto_dir.get(), 40);

  //// keep moving
  m_goto_dir->addAction(new ArActionConstantVelocity("Constant Velocity", 120), 10);
}
//---------------------------------------------------------------------------
void p3dx_gateway::init_stop_action()
{
  //Stop Action ...
  m_stop.reset(new 	ArActionGroupStop(m_robot));
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
bool p3dx_gateway::is_running()
	{
		if (m_robot)
			return m_robot->isRunning();
		else
			return false;
	}
//---------------------------------------------------------------------------
void p3dx_gateway::stop_running()
	{
	if (m_robot)
		{
			if (m_robot->isRunning())
				{
					m_robot->stopRunning();
				}
		}
		//Aria::shutdown();
	}
//---------------------------------------------------------------------------
void p3dx_gateway::wander_mode()
{
	m_robot->clearDirectMotion();
	m_wander->activateExclusive();
	printf("The robot will now just wander around avoiding things.\n");
}
//---------------------------------------------------------------------------
void p3dx_gateway::stop_mode()
{
	m_robot->clearDirectMotion();
	m_stop->activateExclusive();
	printf("The robot will now just stop.\n");
}
//---------------------------------------------------------------------------
void p3dx_gateway::goto_dir_mode()
	{
	m_robot->clearDirectMotion();
	m_goto_dir->activateExclusive();
	printf("goto_dir_mode.\n");
	}
//---------------------------------------------------------------------------
void p3dx_gateway::set_local_dir(double delta_deg)
	{
	m_goto_dir->activateExclusive();
	ArPose current = m_robot->getPose();

	///weird! set a far 'goto goal' in the desired direction.
	double new_theta = ArMath::addAngle(current.getTh(), delta_deg);

	double cos_theta = ArMath::cos(new_theta);
	double sin_theta = ArMath::sin(new_theta);

	////millimeters???
	//double dist = 1000;
	double new_x = current.getX() + 2000*cos_theta;
	double new_y = current.getY() + 2000*sin_theta;

	m_ac_goto_dir->setGoal(ArPose(new_x,new_y,new_theta));
	}
//---------------------------------------------------------------------------
void p3dx_gateway::set_local_goto_dir(ArPose newpose)
	{
	m_ac_goto_dir->setGoal(newpose);
	}
//---------------------------------------------------------------------------
void p3dx_gateway::deactivate_actions()
{
	//
	m_robot->deactivateActions();
	//
	stop();
}

//---------------------------------------------------------------------------
// turn on motors, and off sonar, and off amigobot sounds, when connected
void p3dx_gateway::connected(void)
{
  printf("Doro is Connected\n");
  //m_robot->comInt(ArCommands::SONAR, 1);
  //m_robot->comInt(ArCommands::ENABLE, 1);
  //m_robot->comInt(ArCommands::SOUNDTOG, 0);
}
//---------------------------------------------------------------------------
void p3dx_gateway::on_error_cb()
	{
	printf("DISCONNECT ON ERROR!!\n");
	ArUtil::sleep(2000);
	serial_connect();
	}
//---------------------------------------------------------------------------
///Clear Direct Motion Commands
void p3dx_gateway::clear_direct_motion()
	{
		m_robot->lock();
		m_robot->clearDirectMotion();
		m_robot->unlock();
	}
//---------------------------------------------------------------------------
///
void p3dx_gateway::move(double dist)
	{
		m_robot->lock();
		m_robot->move(dist);
		m_robot->unlock();
	}
//---------------------------------------------------------------------------
void p3dx_gateway::moveTo(ArPose _pose)
	{
		m_robot->lock();
			m_robot->moveTo(_pose);
		m_robot->unlock();	
	}
//---------------------------------------------------------------------------
///
void p3dx_gateway::set_heading(double heading)
	{
		m_robot->lock();
		m_robot->setHeading(heading);
		m_robot->unlock();		
	}
//---------------------------------------------------------------------------
///
void p3dx_gateway::set_delta_heading(double dheading)
	{
		m_robot->lock();
		m_robot->setDeltaHeading(dheading);
		m_robot->unlock();		
	}	
//---------------------------------------------------------------------------
///
void p3dx_gateway::set_rot_vel(double rot_vel)
	{
		m_robot->lock();
		m_robot->setRotVel(rot_vel);
		m_robot->unlock();
	}
//---------------------------------------------------------------------------
///
void p3dx_gateway::set_vel(double vel)
	{
		m_robot->lock();
		m_robot->setVel(vel);
		m_robot->unlock();
	}
//---------------------------------------------------------------------------
///
void p3dx_gateway::set_vel2(double left_vel, double right_vel)
	{
		m_robot->lock();
		m_robot->setVel2(left_vel,right_vel);
		m_robot->unlock();
	}
//---------------------------------------------------------------------------
///
void p3dx_gateway::stop()
	{
    //printf("Stop command\n");
		m_robot->lock();
		m_robot->stop();
		m_robot->unlock();		
	}
//---------------------------------------------------------------------------
void p3dx_gateway::setConnectionTimeOut(int msec)
	{
		m_robot->setConnectionTimeoutTime(msec);
	}
//---------------------------------------------------------------------------
void p3dx_gateway::log_tasks()
	{
	m_robot->logAllTasks();
	}
//---------------------------------------------------------------------------
ArPose p3dx_gateway::get_pose()
	{
		return m_robot->getPose();
	}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
}//alcor
}//device
//---------------------------------------------------------------------------