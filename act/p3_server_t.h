#ifndef p3_server_t_H_INCLUDED
#define p3_server_t_H_INCLUDED
//-------------------------------------------------------------------
#include <alcor/core/server_base_t.hpp>
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
  p3_server_t();  
  ///

public:
  	void send_p3_data(client_connection_ptr_t, net_packet_ptr_t);

private:
  p3_gateway_sptr   p3_;
  p3_server_data_t  p3_data_;
};
//-------------------------------------------------------------------
}}
//-------------------------------------------------------------------
#endif //p3_server_t_H_INCLUDED