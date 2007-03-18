#ifndef saliency_client_t_H_INCLUDED
#define saliency_client_t_H_INCLUDED
//-------------------------------------------------------------------
//#pragma comment (lib, "saliency_service.lib")
//-------------------------------------------------------------------
#include <alcor/core/client_base_t.hpp>
#include <boost/shared_ptr.hpp>
//-------------------------------------------------------------------
#include "alcor/core/saliency_data_net_t.hpp"
//-------------------------------------------------------------------
namespace all { namespace core {
//-------------------------------------------------------------------
  class saliency_client_t : public core::client_base_t 
{
public:
    ///
    saliency_client_t(const char* ini);
    ///
    void send_saliency_data();
    ///
    void reset_data();
    ///
    void add_observation(core::observation_data_t&);
    
private:
    ///
	  void connected_callback();
    ///
    core::saliency_data_net_t saliency_data;
};
//-------------------------------------------------------------------
typedef boost::shared_ptr<saliency_client_t>  saliency_client_ptr_t;
//-------------------------------------------------------------------
}}//all::core
//-------------------------------------------------------------------
#endif //saliency_client_t_H_INCLUDED