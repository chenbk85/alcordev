#ifndef p3_client_t_H_INCLUDED
#define p3_client_t_H_INCLUDED
//-------------------------------------------------------------------
#include <alcor/core/client_base_t.hpp>
#include <boost/shared_ptr.hpp>
//-------------------------------------------------------------------
#include "alcor/act/p3_server_data_t.hpp"
//-------------------------------------------------------------------
namespace all { namespace act{
//-------------------------------------------------------------------
///
  class p3_client_t : public core::client_base_t 
  {
  public:
    ///
    p3_client_t(char* ini);
    ///
    const math::pose2d& 
      get_odometry() const;

  private:
    ///
	  void update_p3_data(net_packet_ptr_t);
    ///
	  void connected_callback();
    ///
    p3_server_data_t p3_data_;
  };

//-------------------------------------------------------------------
typedef boost::shared_ptr<p3_client_t>  p3_client_ptr_t;
//-------------------------------------------------------------------
}}
//-------------------------------------------------------------------
#endif//p3_client_t_H_INCLUDED