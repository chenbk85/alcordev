#ifndef saliency_client_t_H_INCLUDED
#define saliency_client_t_H_INCLUDED
//-------------------------------------------------------------------
#include <alcor/core/client_base_t.hpp>
#include <boost/shared_ptr.hpp>
//-------------------------------------------------------------------
#include "alcor/core/attention_data_net_t.hpp"
//-------------------------------------------------------------------
namespace all { namespace core {
//-------------------------------------------------------------------
  class saliency_client_t : public core::client_base_t 
{
public:
    ///
    saliency_client_t(const char* ini);

private:
    ///
	  void connected_callback();
    ///
    core::attention_data_net_t data;
};
//-------------------------------------------------------------------
typedef boost::shared_ptr<saliency_client_t>  saliency_client_ptr_t;
//-------------------------------------------------------------------
}}//all::core
//-------------------------------------------------------------------
#endif //saliency_client_t_H_INCLUDED