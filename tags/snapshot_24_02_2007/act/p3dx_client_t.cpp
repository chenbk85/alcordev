#include "p3dx_client_t.h"
//---------------------------------------------------------------------------
#include "alcor/core/i_connection_handler_t.h"
#include "alcor/act/p3dx_server_data_t.h"
#include <boost/thread.hpp>
#include "alcor/core/core.h"
//---------------------------------------------------------------------------
#include <iostream>
//---------------------------------------------------------------------------
namespace all{
	namespace act{

class p3dx_client_imp :
  public all::act::p3dx_client_t
  , public core::i_service_handler
	{
	public:
    p3dx_client_imp(const std::string& _inifile = "config/p3dx_conf.ini");

	public:
    ///
    void start();
    ///
		void stop();
    ///
    core::net_state_t net_state() const;

	//[Setters]
	public:
    ///
		void set_localized(double, double, double);
    ///
		void set_localized(const math::pose2d&);
    ///
		void set_vel(double);
    ///
		void set_rot_vel(double);
    //TODO: aggiungere al server i servizi!!
    ///
    void move(double distance);
    ///
    void move_diff(const math::point2d&);
    ///
    void set_heading(const math::angle&);  
    ///
    void set_delta_heading(const math::angle&); 
    ///
    void wander();
    ///
    void set_local_dir (double delta_degree);
    ///
		void stop_robot();

	//[Getter]
	public:
		math::pose2d const& get_odometry() const;
		math::pose2d const& get_localized() const;

	public:
		///Implement this with handlers
		void register_to();
		///Called when server goes down
		void lost_connection();


	private:
		void m_get_odometry_imp ( ArNetPacket* pkt);
		void m_set_localized_imp( ArNetPacket* pkt);

		//void set_vel_imp(ArNetPacket* pkt){};
		//void set_rot_vel_imp(ArNetPacket* pkt){};
		//void stop_imp(ArNetPacket* pkt){};

	protected:
		ArFunctor1C<p3dx_client_imp, ArNetPacket* > m_get_odometry_cb;
		ArFunctor1C<p3dx_client_imp, ArNetPacket* > m_set_localized_cb;

		//ArFunctor1C<p3dx_client_imp, ArNetPacket* > m_set_vel_cb;
		//ArFunctor1C<p3dx_client_imp, ArNetPacket* > m_set_rot_vel_cb;
		//ArFunctor1C<p3dx_client_imp, ArNetPacket* > m_stop_cb;

	private:
    ///
    all::core::i_connection_handler_t connection_handler;
    ///
    //boost::thread* conn_thr;
		///
    act::p3dx_position_t  m_p3dx_odometry;
		///
    act::p3dx_position_t  m_p3dx_localized;
	};

//###########################################################################
//---------------------------------------------------------------------------
p3dx_client_imp::p3dx_client_imp(const std::string& _inifile):
			connection_handler(this,_inifile),
			m_get_odometry_cb(this, &p3dx_client_imp::m_get_odometry_imp),
			m_set_localized_cb(this, &p3dx_client_imp::m_set_localized_imp)/*,

			m_set_vel_cb(this, &p3dx_client_imp::set_vel_imp),
			m_stop_cb(this, &p3dx_client_imp::stop_imp),
			m_set_rot_vel_cb(this, &p3dx_client_imp::set_rot_vel_imp)*/

	{
	}
//---------------------------------------------------------------------------
  void p3dx_client_imp::start()
  {  

    //parte il thread    
    printf("DEPRECATED p3dx_client_imp::Start Thread\n");

    //conn_thr=
    //    new boost::thread(
    //    boost::bind(&i_connection_handler_t::runThread, &connection_handler)
    //    );

  }
//---------------------------------------------------------------------------
void p3dx_client_imp::stop()
	{
    connection_handler.running = false;
	}
//---------------------------------------------------------------------------
core::net_state_t p3dx_client_imp::net_state() const
  {
    //return this_state();
    return connection_handler.this_state();
  }
//---------------------------------------------------------------------------
void p3dx_client_imp::set_localized(double _x, double _y, double _th)
	{
		ArNetPacket msg;
		//compose
		m_p3dx_localized.setX(_x);
		m_p3dx_localized.setY(_y);
    m_p3dx_localized.setTh(math::angle(_th,math::deg_tag));
		//pack
		m_p3dx_localized.pack(&msg);
    //
    msg.finalizePacket();
		//send
    connection_handler.m_client.requestOnce("setLocalized", &msg);
	}
//---------------------------------------------------------------------------
void p3dx_client_imp::set_localized(const math::pose2d& pose)
	{		
		ArNetPacket msg;
		//compose
    m_p3dx_localized = pose;
		//pack
		m_p3dx_localized.pack(&msg);
    //
    msg.finalizePacket();
		//send
		connection_handler.m_client.requestOnce("setLocalized", &msg);
	}
//---------------------------------------------------------------------------
//void p3dx_client_imp::print_localized()
//	{
//	m_p3dx_localized.log();
//	}
//---------------------------------------------------------------------------
math::pose2d const& p3dx_client_imp::get_odometry() const 
	{
	return m_p3dx_odometry;
	}
//---------------------------------------------------------------------------
math::pose2d const& p3dx_client_imp::get_localized() const
{
  return m_p3dx_localized;
}
//---------------------------------------------------------------------------
void p3dx_client_imp::set_vel(double _vel)
	{
	ArNetPacket cmd;
	cmd.doubleToBuf(_vel);
	cmd.finalizePacket();
	connection_handler.m_client.requestOnce("setvel", &cmd);
	}
//---------------------------------------------------------------------------
void p3dx_client_imp::set_rot_vel(double _rotvel)
	{
	ArNetPacket cmd;
	cmd.doubleToBuf(_rotvel);
	cmd.finalizePacket();
	connection_handler.m_client.requestOnce("setrotvel", &cmd);
	}
//---------------------------------------------------------------------------
///
void p3dx_client_imp::set_heading(const math::angle& head)
{
	ArNetPacket cmd;
  cmd.doubleToBuf(head.deg());
	cmd.finalizePacket();
	connection_handler.m_client.requestOnce("setheading", &cmd);
}
//---------------------------------------------------------------------------
///
void p3dx_client_imp::set_delta_heading(const math::angle& dhead)
{
	ArNetPacket cmd;
  cmd.doubleToBuf(dhead.deg());
	cmd.finalizePacket();
	connection_handler.m_client.requestOnce("setdeltaheading", &cmd);
}
//---------------------------------------------------------------------------
void p3dx_client_imp::wander()
{
connection_handler.m_client.requestOnce("wander", 0);
}
//---------------------------------------------------------------------------
void p3dx_client_imp::set_local_dir (double delta_degree)
{
	ArNetPacket cmd;
  cmd.doubleToBuf(delta_degree);
	cmd.finalizePacket();
	connection_handler.m_client.requestOnce("setlocaldir", &cmd);
}
//---------------------------------------------------------------------------
void p3dx_client_imp::move(double distance)
{
	ArNetPacket cmd;
  cmd.doubleToBuf(distance);
	cmd.finalizePacket();
	connection_handler.m_client.requestOnce("move", &cmd);
}
//---------------------------------------------------------------------------
void p3dx_client_imp::move_diff(const math::point2d&)
{

}
//---------------------------------------------------------------------------
void p3dx_client_imp::stop_robot()
	{
	connection_handler.m_client.requestOnce("stop", 0);
	}
//---------------------------------------------------------------------------
//void p3dx_client_imp::print_odometry()
//	{
//	m_p3dx_odometry.log();
//	}
//---------------------------------------------------------------------------
void p3dx_client_imp::register_to()
	{
		printf("Registering Handlers!\n");
		connection_handler.m_client.addHandler("getOdometry", &m_get_odometry_cb);
		connection_handler.m_client.addHandler("setLocalized",&m_set_localized_cb);

		//connection_handler.m_client.addHandler("setvel",&m_set_vel_cb);
		//connection_handler.m_client.addHandler("setrotvel",&m_set_rot_vel_cb);
		//connection_handler.m_client.addHandler("stop",&m_stop_cb);

	  connection_handler.m_client.request("getOdometry",50);
	}
//---------------------------------------------------------------------------
void p3dx_client_imp::lost_connection()
	{
		//Do something meaningful for the application.
	}
//---------------------------------------------------------------------------
void p3dx_client_imp::m_get_odometry_imp(ArNetPacket* msg)
	{
	m_p3dx_odometry.import(msg);
	}
////---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void p3dx_client_imp::m_set_localized_imp(ArNetPacket* msg)
	{
		//Nothing here ....
	}
//---------------------------------------------------------------------------
//###########################################################################
//boost::shared_ptr<p3dx_client_t> create_p3dx_client()
//  {
//    boost::shared_ptr<p3dx_client_t> p_client (
//    reinterpret_cast<p3dx_client_t*>
//    (new p3dx_client_imp())
//    );
//  return p_client;
//  }

boost::shared_ptr<p3dx_client_t> create_p3dx_client()
  {
    printf("create_p3dx_client .. creating ...\n");
    boost::shared_ptr<p3dx_client_t> p_client (
    reinterpret_cast<p3dx_client_t*>
    (new p3dx_client_imp())
    );
    printf("create_p3dx_client .. created\n");
  return p_client;
  }
//---------------------------------------------------------------------------

		}//alcor
	}//device
//---------------------------------------------------------------------------