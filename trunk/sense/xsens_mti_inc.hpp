#ifndef xsens_mti_inc_HPP_INCLUDED
#define xsens_mti_inc_HPP_INCLUDED

namespace all { namespace sense {

//namespace tags
//{
  struct heading_reset_t{};
  struct global_reset_t{};
  struct object_reset_t{};
  struct align_reset_t{};

  static heading_reset_t heading_reset = heading_reset_t(); 
  static global_reset_t global_reset = global_reset_t();
  static object_reset_t object_reset = object_reset_t();
  static align_reset_t align_reset   = align_reset_t();
//}

}}//all::sense

#endif//xsens_mti_inc_HPP_INCLUDED