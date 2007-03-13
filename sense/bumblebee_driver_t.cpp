#include "alcor/sense/bumblebee_driver_t.h"
#include "alcor/sense/detail/bumblebee_driver_impl.cpp"
//------------------------------------------------------------------+
#include "alcor/core/iniWrapper.h"
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

  bIsOk   = bIsOk && impl->init_digiclops_context_(   params._unit_number, params._digiclopsini
                                                    , params._framerate);
  bIsOk   = bIsOk && impl->init_triclops_context_(params._triclopsini);
  bIsOk   = bIsOk && impl->init_grabbing_();

  if (bIsOk)
    impl->allocate_buffers_();

  color_buffer_size_ = impl->rows_*impl->cols_*3;
  depth_buffer_size_ = impl->rows_*impl->cols_*sizeof(core::single_t);

  return bIsOk;
}
//------------------------------------------------------------------+
///Inherited
bool bumblebee_driver_t::close()
{
return true;
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
size_t bumblebee_driver_t::color_buffer_size() const
{
  return color_buffer_size_;
}
//------------------------------------------------------------------+
///
size_t bumblebee_driver_t::depth_buffer_size() const
{
  return depth_buffer_size_;
}
//------------------------------------------------------------------+
///
size_t bumblebee_driver_t::valid_3d_points() const
{
  return impl->valid_count_;
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
    size_t a_size = impl->pixelnum;

    all::core::uint8_sarr a_copy_of(new all::core::uint8_t[a_size]);
  
    memcpy(a_copy_of.get(), return_ptr.get(), a_size);

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
    size_t a_size = impl->pixelnum;

    all::core::uint8_sarr a_copy_of(new all::core::uint8_t[a_size]);
  
    memcpy(a_copy_of.get(), return_ptr.get(), a_size);

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
///  
all::core::single_sarr bumblebee_driver_t::get_depth_buffer(core::interleaved_t, bool shared)
{
  return impl->get_depthmap_interleaved_();
}
//------------------------------------------------------------------+

//###################################################################
bool bumblebee_parameters_t::load(const std::string& confname)
{
  iniWrapper config;
  printf("\n");
  if( config.Load(confname) )
    //printf("Ini File loaded!\n");
    printf("Loading parameters from: %s\n", confname.c_str());
  else
  {
    printf("Impossible to read from %s\n", confname.c_str());
    return false;
  }
  char* temp;

  int tmp_unit_number = config.GetInt("config:unitnumber", 6213002);
  _unit_number = static_cast<int>(tmp_unit_number);
  printf ("config:unitnumber = %d\n", _unit_number );

  _framerate = config.GetInt("config:framerate", 50);
  printf ("config:framerate = %d\n", _framerate);

  temp = config.GetStringAsChar("config:digiclopsini", "digiclopsA.ini");
  _digiclopsini.assign(temp);
  printf ("config:digiclopsini = %s\n", _digiclopsini.c_str());

  temp  = config.GetStringAsChar("config:triclopsini", "triclopsA.ini");
  _triclopsini.assign(temp);
  printf ("config:triclopsini = %s\n", _triclopsini.c_str());

  temp  = config.GetStringAsChar("config:name", "aristotele");
  _name.assign(temp);
  printf ("config:name = %s\n", _name.c_str());

  printf("Loaded parameters from: %s\n", confname.c_str());

  printf("\n");
  return true;
};
//###################################################################

//###################################################################

}}//namespaces