#ifndef slam2d_adapter_H_INCLUDED
#define slam2d_adapter_H_INCLUDED
//---------------------------------------------------------------------------
#include "alcor/core/slam_data_adapter_i.h"
#include "alcor.apps/splam/splam_client_t.h"
//---------------------------------------------------------------------------
namespace all { namespace sense {
//---------------------------------------------------------------------------
  class slam2d_adapter : public all::core::slam_data_adapter_i
{
public:
  slam2d_adapter(boost::shared_ptr<splam_client_t> islam):slam_(islam){};

public:
  ///Localized rotation angle in degrees.
  double get_current_rot(math::deg_t);
  ///Localized rotation angle in radians.
  double get_current_rot(math::rad_t);

private:
  ///does not own this pointer.
  boost::shared_ptr<splam_client_t>  slam_;
  SlamData slam_data;
};
//---------------------------------------------------------------------------
//###########################################################################
///Localized rotation angle in degrees.
  inline double slam2d_adapter::get_current_rot(math::deg_t)
{  
  if(slam_.get())
  {
    slam_->get_slam_data(slam_data);
    if (!slam_data.path_.empty())
      return core::dconstants::rad_to_deg(slam_data.path_.back().rot);
  else
    return 0;
  }
  else
    return 0;
}
//---------------------------------------------------------------------------
///Localized rotation angle in radians.
  inline double slam2d_adapter::get_current_rot(math::rad_t)
{
  if(slam_.get())
  {
    slam_->get_slam_data(slam_data);
    if (!slam_data.path_.empty())
      return slam_data.path_.back().rot;
  else
    return 0 ;
  }
  else 
    return 0;
}
//###########################################################################
}}//namespaces
//---------------------------------------------------------------------------
#endif //slam2d_adapter_H_INCLUDED