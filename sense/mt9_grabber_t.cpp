#include "mt9_grabber_t.h"
#include "shared_mt9_data_t.h"
#include "alcor/sense/detail/PiGateway.h"
//---------------------------------------------------------------------------
#include "alcor/core/config_parser_t.h"
#include "alcor/core/ipc_serializable_t.h"
//---------------------------------------------------------------------------
#include <boost/thread/thread.hpp>
//---------------------------------------------------------------------------
using  namespace boost::interprocess;
//---------------------------------------------------------------------------
using namespace all::sense;
//---------------------------------------------------------------------------
mt9_grabber_t::mt9_grabber_t():
_running(true)

	{
	core::config_parser_t conf;
	conf.load(core::ini,"xmlconf/mt9.ini");
	int port = conf.as_int("mt9.port",5);

#ifdef MT9_DEBUG_INFO
	myfile.open ("mt9log.txt");
	myfile << "Opening MT9 on port: " << port << std::endl;
#endif

	mt9_.reset( new PiGateway(port));

	if(mt9_->Init())
		{	
			if(mt9_->Start())
				{
#ifdef MT9_DEBUG_INFO
				myfile	<< "MT9 OK .. Inited and Started" << std::endl;
#endif
				mt9_->Reset();
				}
			else
				{
#ifdef MT9_DEBUG_INFO
				myfile	<< "MT9::Error in Start()" << std::endl;
#endif
				}
		}
	else
		{
#ifdef MT9_DEBUG_INFO
		myfile	<< "MT9::Error in Init()" << std::endl;
#endif
		}

	}
//---------------------------------------------------------------------------
mt9_grabber_t::~mt9_grabber_t()
	{
	mt9_->Stop();
	//delete mt9_;
#ifdef MT9_DEBUG_INFO
	myfile.close();
#endif
	}
//---------------------------------------------------------------------------
float mt9_grabber_t::get_yaw() const
	{ 
	return mt9_->GetYaw();
	};
//---------------------------------------------------------------------------
float mt9_grabber_t::get_pitch() const
	{ 
	return mt9_->GetPitch();
	}; 
//---------------------------------------------------------------------------
float mt9_grabber_t::get_roll() const
	{ 
	return mt9_->GetRoll();
	}
//---------------------------------------------------------------------------
void mt9_grabber_t::reset()
{
    mt9_->Reset();
}
//---------------------------------------------------------------------------
//void mt9_grabber_t::get_euler(float *rpy) const
//{			
//	rpy[0] = mt9_->GetRoll();
//	rpy[1] = mt9_->GetPitch();
//	rpy[2] = mt9_->GetYaw();
//}
//---------------------------------------------------------------------------
//#include <boost/format.hpp> 
//---------------------------------------------------------------------------
void mt9_grabber_t::run_thread()
	{
	std::string  mem_obj_tag  = "_mt9_B_euler_data_";

	//////////////////////////////////////////////////////////////
	core::ipc_serializable_t<sense::mt9_data_t> 
		ipc_mt9(core::open_write, mem_obj_tag);
	//////////////////////////////////////////////////////////////

	//boost::format fmrpy("RPY: %3.2f %3.2f %3.2f %d");
	
	//boost::interprocess::named_mutex n_mutex(open_or_create, mem_mutex_tag.c_str());

	std::cout << std::endl << std::endl;
	m_timer.restart();
	while(_running)
		{
			//ENTER_CRITICAL(ipc_mt9)

			ipc_mt9.get_reference().roll = mt9_->GetRoll(); //fmrpy % mt9_data->roll;
			ipc_mt9.get_reference().pitch= mt9_->GetPitch();//fmrpy % mt9_data->pitch;
			ipc_mt9.get_reference().yaw	 = mt9_->GetYaw();  //fmrpy % mt9_data->yaw;
			ipc_mt9.get_reference().elapsed	= static_cast<long>( (m_timer.elapsed()*1000) );
			ipc_mt9.get_reference().tag = 0;

			//LEAVE_CRITICAL
				//std::cout << ipc_mt9.get_reference() << std::endl;
		////std::cout.flush();
		//	std::cout << fmrpy	% ipc_mt9.get_const().roll 
		//					% ipc_mt9.get_const().pitch 
		//					% ipc_mt9.get_const().yaw 
		//					% ipc_mt9.get_const().elapsed;

		//std::cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
		//std::cout.flush();
		boost::thread::yield();
		Sleep(25);
		}
	}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void mt9_grabber_t::cancel()
	{
		_running = false;
	}
//---------------------------------------------------------------------------
