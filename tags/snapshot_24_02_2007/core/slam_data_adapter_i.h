#ifndef slam_data_adapter_i_H_INCLUDED
#define slam_data_adapter_i_H_INCLUDED
//---------------------------------------------------------------------------
#include "alcor/math/angle.h"
namespace all { namespace core {
//---------------------------------------------------------------------------
///slam_data_adapter_i.
class slam_data_adapter_i
{
public:
  ///Localized rotation angle in degrees.
  virtual double get_current_rot(math::deg_t) = 0;
  ///Localized rotation angle in radians.
  virtual double get_current_rot(math::rad_t) = 0;
  ///
  virtual double get_current_x(){return 0;};
  virtual double get_current_y(){return 0;};
  virtual double get_current_z(){return 0;};
};
//---------------------------------------------------------------------------
}}//namespaces
//---------------------------------------------------------------------------
#endif //slam_data_adapter_i_H_INCLUDED
