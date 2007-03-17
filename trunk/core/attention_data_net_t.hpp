#ifndef attention_data_net_t_HPP_INCLUDED
#define attention_data_net_t_HPP_INCLUDED
//-------------------------------------------------------------
#include "alcor/core/attention_data_t.hpp"
#include <alcor/core/net_packet_t.hpp>
//-------------------------------------------------------------
#include <boost/foreach.hpp>
//-------------------------------------------------------------
namespace all { namespace core {
//-------------------------------------------------------------
///
struct attention_data_net_t
{
  ///
  void import(core::net_packet_ptr_t);
  ///
  void pack(core::net_packet_ptr_t);
  ///
  core::attention_data_t attention_data_;
};
//-------------------------------------------------------------
//IMPORT
inline void attention_data_net_t::import(core::net_packet_ptr_t pkt)
{
  //scan_count_
  attention_data_.scan_count_ = static_cast<size_t>(pkt->buf_to_int());

  //bearing_
  attention_data_.bearing_.set_pan  (math::deg_tag,pkt->buf_to_double());
  attention_data_.bearing_.set_tilt (math::deg_tag,pkt->buf_to_double());

  //
  int numobservations = static_cast<size_t>( pkt->buf_to_int() );

  //prepare storage
  attention_data_.observations_.resize(numobservations);

  //
  for (int i=0; i < numobservations; i++)
  {
    //
    core::observation_data_t obsv;
    //relative_position_
    obsv.relative_position_.set_x1(pkt->buf_to_double());
    obsv.relative_position_.set_x2(pkt->buf_to_double());
    obsv.relative_position_.set_th( pkt->buf_to_double(), math::deg_tag);

    //radius_
    obsv.radius_  = pkt->buf_to_double();

    //weight_
    obsv.weight_  = pkt->buf_to_double();
    //
    attention_data_.observations_.push_back(obsv);
  }

}
//-------------------------------------------------------------
//PACK
inline void attention_data_net_t::pack( core::net_packet_ptr_t pkt)
{
  //size_t scan_count_
  pkt->int_to_buf(static_cast<int>(attention_data_.scan_count_ ) );

  //pantilt_angle_t  bearing_
  pkt->double_to_buf(attention_data_.bearing_.get_pan(math::deg_tag) );
  pkt->double_to_buf(attention_data_.bearing_.get_tilt(math::deg_tag) );

  //how many observations in vector
  int numobs = 
    static_cast<int>(attention_data_.observations_.size() );

  //push how many observations in vector
  pkt->int_to_buf(numobs);

  //push each observation in
  BOOST_FOREACH( all::core::observation_data_t& obs,  attention_data_.observations_)
  { 
    //pose2d relative_position_ 
    pkt->double_to_buf(obs.relative_position_.get_x1());
    pkt->double_to_buf(obs.relative_position_.get_x2());
    pkt->double_to_buf(obs.relative_position_.getTh().deg());

    //double radius_
    pkt->double_to_buf(obs.radius_);
    //double weight_
    pkt->double_to_buf(obs.weight_);
  } 

}
//-------------------------------------------------------------
}}//all::core
//-------------------------------------------------------------
#endif //attention_data_net_HPP_INCLUDED