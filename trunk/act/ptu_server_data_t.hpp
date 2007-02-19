#ifndef ptu_server_data_t_HPP_INCLUDED
#define ptu_server_data_t_HPP_INCLUDED


#include "alcor/core/net_packet_t.hpp"
 

namespace all {
	namespace act {
///ptu_server_data_t
class ptu_server_data_t //: public core::i_serializable
{
public:
  ptu_server_data_t():pan(0),tilt(0){}
public:
	void import(core::net_packet_ptr_t);
	void pack(core::net_packet_ptr_t);
  double pan;
  double tilt;
};


inline void ptu_server_data_t::import(core::net_packet_ptr_t packet)
{
  pan = packet->buf_to_double();
  tilt= packet->buf_to_double();
}

inline void ptu_server_data_t::pack(core::net_packet_ptr_t packet)
{
  packet->double_to_buf(pan);
  packet->double_to_buf(tilt);
	//packet->finalize_packet();
}


  }}//all::act

#endif //ptu_server_data_t_HPP_INCLUDED