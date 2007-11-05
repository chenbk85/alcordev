#ifndef xsens_mti_inc_HPP_INCLUDED
#define xsens_mti_inc_HPP_INCLUDED

namespace all { namespace sense {

namespace tags{
  struct heading_reset_t{};
  struct global_reset_t{};
  struct object_reset_t{};
  struct align_reset_t{};
}//all::sense::tags

  static tags::heading_reset_t heading_reset = tags::heading_reset_t(); 
  static tags::global_reset_t global_reset = tags::global_reset_t();
  static tags::object_reset_t object_reset = tags::object_reset_t();
  static tags::align_reset_t align_reset   = tags::align_reset_t();

}}//all::sense

#endif//xsens_mti_inc_HPP_INCLUDED