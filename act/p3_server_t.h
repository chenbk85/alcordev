#ifndef p3_server_t_H_INCLUDED
#define p3_server_t_H_INCLUDED
//-------------------------------------------------------------------
#include <alcor/core/server_base_t.hpp>
#include <boost/shared_ptr.hpp>
//-------------------------------------------------------------------
#include "alcor/act/p3_gateway.h"
#include "alcor/act/p3_server_data_t.hpp"
//-------------------------------------------------------------------
namespace all { namespace act{
///
class p3_server_t : public server_base_t
{
public:
  ///
  p3_server_t(const char* ini_file); 
  ///
  void set_gateway_ptr(p3_gateway_ptr_t);


public:
  ///
  void send_p3_data(client_connection_ptr_t, net_packet_ptr_t);

private:
    ///
  void register_();
  ///
  p3_gateway_ptr_t   p3_;
  ///
  p3_server_data_t  p3_data_;
};
//-------------------------------------------------------------------
typedef boost::shared_ptr<p3_server_t>  p3_server_ptr_t;
//-------------------------------------------------------------------
}}
//-------------------------------------------------------------------
#endif //p3_server_t_H_INCLUDED