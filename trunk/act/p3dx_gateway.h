#ifndef p3dx_gateway_H_INCLUDED
#define p3dx_gateway_H_INCLUDED
//---------------------------------------------------------------------------
#include <Aria.h>
#include <ArNetworking.h>
//---------------------------------------------------------------------------
#include "alcor/core/core.h"
#include "alcor/act/p3dx_server_data_t.h"
#include "alcor/math/all_math.h"
//---------------------------------------------------------------------------
#include <memory>
using std::auto_ptr;
//---------------------------------------------------------------------------
namespace all
	{
	namespace act{
//---------------------------------------------------------------------------
class p3dx_gateway
{
public:
	///public ctor
	p3dx_gateway();
	///public dtor
	~p3dx_gateway();
	
public:
	ArRobot*      m_robot;	
	ArServerBase  m_server;
  all::core::ip_address_t m_addr;
  ArKeyHandler keyHandler;
public:	
	void robot_run();
	bool is_running();
	void stop_running();
	
public:	
	///Enable Wandering Action
	void wander_mode();
	///Enable Stop Action
	void stop_mode();
  ///
  void follow_mode();
  ///only when in follow_mode.
  ///distance (mt) e offset angolare dal target (gradi)
  void set_target (double distance, double offset,double speed = 100);

	///Enable Goto DIR Action ///////////////////////////////////////////////////////
	void goto_dir_mode();
	///Set the goto pose corresponding to a local direction (relative to robot frame).
	/// \param dir local direction expressed as angles.
	void set_local_dir(double dir);

	void set_local_goto_dir(ArPose);
	/////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////

public:
	///
	void deactivate_actions();	
	
public:
	///Serial Connection
	bool serial_connect();
	///TCP Connection
	bool tcp_connect(char*,int);
	//
	bool blocking_connect();


public:
	///Clear Direct Motion Commands
	void clear_direct_motion();

	///Move forward.
	void move(double);
	///
	void set_heading(double);
	///
	void set_delta_heading(double);
	///
	void set_rot_vel(double);
	///
	void set_vel(double);
	///
	void set_vel2(double, double);
	///
	void stop();	
	///
	void setConnectionTimeOut(int msec);
  ///
	void log_tasks();
	///
	void moveTo(ArPose);

public:
	///Range Reading .. todo
	void get_sonar();
	///get Odometry
	ArPose get_pose(); 

private:
	///
	void init_wander_action();
	///
  	void init_stop_action();
	///ops changed naming convention ;)
	void init_goto_dir();	
  ///
  void init_follow_action();

private:	
  auto_ptr<ArActionGroup> m_wander;
	auto_ptr<ArActionGroup> m_goto;
	auto_ptr<ArActionGroup> m_stop;
	auto_ptr<ArActionGroup> m_goto_dir;
	auto_ptr<ArActionGroup> m_goto_pos;
  auto_ptr<ArActionGroup> m_follow;
	///The private goto action handled by set_local_dir.
	auto_ptr<ArActionGoto>  m_ac_goto_dir;
  auto_ptr<ArActionGotoStraight> m_ac_follow;

private:
	ArTcpConnection 	m_tcpConn;
	int					m_serial_port;

  //ArSimpleConnector connector;

	ArSerialConnection 	m_serialConn;	
	ArSonarDevice		    m_sonar;
	
private:
	// to be called if the connection was made
	void connected(void);	
	void on_error_cb(void);
	// the functor callbacks
	ArFunctorC<p3dx_gateway> myConnectedCB;
	ArFunctorC<p3dx_gateway> m_error_disconnect_cb;


private:
	///Some parameters
	double m_goto_dir_dist;

/////////////////////////////////////////////////////////////////////////////
//					Server Side
/////////////////////////////////////////////////////////////////////////////
public:
///Start server
	void start_server();

public:
	///Get SLAM localized p3dx position.
	p3dx_position_t const& get_localized()const;

	///Get local p3dx dead reckoning position.
	ArPose get_odometry() const;

  //[Server Callback]
private:
	ArFunctor2C<p3dx_gateway, ArServerClient *, ArNetPacket *> m_get_odometry_cb;
	ArFunctor2C<p3dx_gateway, ArServerClient *, ArNetPacket *> m_set_localized_cb;

private:
	ArFunctor2C<p3dx_gateway, ArServerClient *, ArNetPacket *> m_set_vel_cb;
	ArFunctor2C<p3dx_gateway, ArServerClient *, ArNetPacket *> m_set_rot_vel_cb;
	ArFunctor2C<p3dx_gateway, ArServerClient *, ArNetPacket *> m_stop_cb;
	ArFunctor2C<p3dx_gateway, ArServerClient *, ArNetPacket *> m_set_heading_cb;
	ArFunctor2C<p3dx_gateway, ArServerClient *, ArNetPacket *> m_set_delta_heading_cb;
	ArFunctor2C<p3dx_gateway, ArServerClient *, ArNetPacket *> m_set_wandermode_cb;
	ArFunctor2C<p3dx_gateway, ArServerClient *, ArNetPacket *> m_set_localdir_cb;
	ArFunctor2C<p3dx_gateway, ArServerClient *, ArNetPacket *> m_move_cb;

//[the remote callbacks]
private:
	void get_odometry_imp (ArServerClient *, ArNetPacket *);
	void set_localized_imp(ArServerClient *, ArNetPacket *);
	void set_vel_imp (ArServerClient *, ArNetPacket *);
	void set_rot_vel_imp (ArServerClient *, ArNetPacket *);
	void stop_imp (ArServerClient *, ArNetPacket *);
	void set_heading_imp (ArServerClient *, ArNetPacket *);
	void set_delta_heading_imp (ArServerClient *, ArNetPacket *);
	void set_wandermode_imp (ArServerClient *, ArNetPacket *);
	void set_localdir_imp (ArServerClient *, ArNetPacket *);
  void move_imp(ArServerClient *, ArNetPacket *);

private:
	///Localized
  all::act::p3dx_position_t m_localized;
	///From onboard odometry
	all::act::p3dx_position_t m_odometry;
};
//---------------------------------------------------------------------------
typedef boost::shared_ptr<p3dx_gateway> p3dx_gateway_sptr;
//---------------------------------------------------------------------------
}//alcor
}//device
//---------------------------------------------------------------------------
#endif //p3dx_gateway_H_INCLUDED
