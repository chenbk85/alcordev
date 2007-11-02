#ifndef fast_mti_integral_t_HPP_INCLUDED
#define fast_mti_integral_t_HPP_INCLUDED
//---------------------------------------------------------
//#include <boost/config.hpp>
//---------------------------------------------------------
namespace all { namespace math {
//---------------------------------------------------------
  ///
class fast_mti_integral_t
{
public:
  ///
fast_mti_integral_t();
///
void set_initial_location(float loc_[]);
void set_initial_angle (float orient_[]);
void set_initial_velocity(float vel_[]);

///just weird
void update(float data_[], double elapsed_secs_);

///
void reset_state();

private:

float _loc[3];
float _vel[3];
float _orient[3];

///constant  gravity acceleration
const float G_acc;

};
//---------------------------------------------------------
inline fast_mti_integral_t::fast_mti_integral_t()
:G_acc(9.80665f)
{

}
//---------------------------------------------------------
inline void fast_mti_integral_t::update(float data_[], double elapsed_)

{
//given accelerations, compensate for G

}
//---------------------------------------------------------
}} //all::math
//---------------------------------------------------------
#endif //fast_mti_integral_t_HPP_INCLUDED