#ifndef saliency_server_t_H_INCLUDED
#define saliency_server_t_H_INCLUDED
//-------------------------------------------------------------------
#include <alcor/core/server_base_t.hpp>
#include <boost/shared_ptr.hpp>
//-------------------------------------------------------------------
#include "alcor/core/attention_data_net_t.hpp"
//-------------------------------------------------------------------
namespace all { namespace core {
//-------------------------------------------------------------------
  class saliency_server_t : public server_base_t
  {
  public:
  ///
    saliency_server_t(const char* ini_file); 

  private:
    //
    core::attention_data_net_t data;
  };

//-------------------------------------------------------------------
typedef boost::shared_ptr<saliency_server_t>  saliency_server_ptr_t;
//-------------------------------------------------------------------
}}//all::core
//-------------------------------------------------------------------
#endif //saliency_server_t_H_INCLUDED