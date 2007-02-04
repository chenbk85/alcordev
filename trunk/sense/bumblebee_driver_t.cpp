#include "alcor/sense/bumblebee_driver_t.h"
#include "alcor/sense/detail/bumblebee_driver_impl.cpp"
//------------------------------------------------------------------+
#include "alcor/core/config_parser_t.h"
//------------------------------------------------------------------+
namespace all { namespace sense {
//------------------------------------------------------------------+
  bumblebee_driver_t::bumblebee_driver_t()
  {
    impl.reset( new detail::bumblebee_driver_impl() );
  }
//------------------------------------------------------------------+
///Inherited
  bool bumblebee_driver_t::open(const std::string &confname)
{
  bool bIsOk = true;

  params.load(confname);

  bIsOk   = bIsOk && impl->init_digiclops_context_(params._unit_number, params._digiclopsini);
  bIsOk   = bIsOk && impl->init_triclops_context_(params._triclopsini);
  bIsOk   = bIsOk && impl->init_grabbing_();
if (bIsOk)
  impl->allocate_buffers_();

  return bIsOk;
}
//------------------------------------------------------------------+
///Inherited
bool bumblebee_driver_t::close()
{
}
//------------------------------------------------------------------+
long bumblebee_driver_t::serialnum() const
{
  return params._unit_number;
}
//------------------------------------------------------------------+
///
size_t bumblebee_driver_t::nrows() const
{
  return impl->rows_;
}
//------------------------------------------------------------------+
///
size_t bumblebee_driver_t::ncols() const
{
  return impl->cols_;
}
//------------------------------------------------------------------+
float bumblebee_driver_t::focal() const
{
  return impl->focal_;
}
//------------------------------------------------------------------+
///
float  bumblebee_driver_t::center_row() const
{
  return impl->center_row_;
}
//------------------------------------------------------------------+
///
float  bumblebee_driver_t::center_col() const
{
  return impl->center_col_;
}
//------------------------------------------------------------------+
///
std::string bumblebee_driver_t::name() const
{
  return params._name;
}
//------------------------------------------------------------------+
bool  bumblebee_driver_t::grab()
{
  return impl->grab_();
}
//------------------------------------------------------------------+
///
all::core::uint8_sarr  bumblebee_driver_t::get_color_buffer(core::left_img_t , bool shared)
{
  all::core::uint8_sarr return_ptr = impl->get_rgb_left_();

  if(shared)
    return return_ptr;
  else
  {
    all::core::uint8_sarr a_copy_of( return_ptr.get() ); 
    return a_copy_of;
  }
}
//------------------------------------------------------------------+
///
all::core::uint8_sarr bumblebee_driver_t::get_color_buffer(core::right_img_t , bool shared )
{
  all::core::uint8_sarr return_ptr = impl->get_rgb_right_();

  if(shared)
    return return_ptr;
  else
  {
    all::core::uint8_sarr a_copy_of( return_ptr.get() ); 
    return a_copy_of;
  }
}
//------------------------------------------------------------------+
///
all::core::single_sarr bumblebee_driver_t::get_depth_buffer(bool shared)
{
  return impl->get_depthmap_();
}
//------------------------------------------------------------------+

//###################################################################
void bumblebee_parameters_t::load(const std::string& confname)
{
  core::config_parser_t config;
  config.load(core::ini, confname);
  _unit_number  = config.as_long("config.unitnumber", 6213002);

  _digiclopsini = config.as_string("config.digiclopsini", "digiclopsA.ini");

  _triclopsini  = config.as_string("config.triclopsini", "triclopsA.ini");

  _name         = config.as_string("config.name", "aristotele");

};
//###################################################################

//###################################################################

}}//namespaces