#ifndef ptu_server_data_t_HPP_INCLUDED
#define ptu_server_data_t_HPP_INCLUDED


#include "alcor/core/net_packet_t.hpp"
#include "alcor/core/i_serializable.h"

namespace all {
	namespace act {
///ptu_server_data_t
class ptu_server_data_t : public core::i_serializable
{
public:
  void import(void*){};
  void pack(void* )  {}; 
};

  }}//all::act

#endif //ptu_server_data_t_HPP_INCLUDED