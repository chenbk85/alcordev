#ifndef fast_mti_integral_t_HPP_INCLUDED
#define fast_mti_integral_t_HPP_INCLUDED
//---------------------------------------------------------
#include <boost/numeric/ublas/vector.hpp>
//---------------------------------------------------------
using namespace boost::numeric;
//---------------------------------------------------------
#include "alcor/math/moving_average_t.hpp"
//---------------------------------------------------------
//---------------------------------------------------------
//#define DOFILELOG
#define _MTILOGONSCREEN_
//
#ifdef DOFILELOG
#include <fstream>
using std::cout;
using std::endl;
#endif


//---------------------------------------------------------
namespace all { namespace math {
//---------------------------------------------------------

//---------------------------------------------------------
//source:http://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation
//given a quaternion z = a + bi + cj + dk (with |z| = 1) 
//and a vector v with elements v1, v2, and v3, 
//the following code performs a rotation.
//t2 =   a*b
//t3 =   a*c
//t4 =   a*d
//t5 =  -b*b
//t6 =   b*c
//t7 =   b*d
//t8 =  -c*c
//t9 =   c*d
//t10 = -d*d
//v1new = 2*( (t8 + t10)*v1 + (t6 -  t4)*v2 + (t3 + t7)*v3 ) + v1
//v2new = 2*( (t4 +  t6)*v1 + (t5 + t10)*v2 + (t9 - t2)*v3 ) + v2
//v3new = 2*( (t7 -  t3)*v1 + (t2 +  t9)*v2 + (t5 + t8)*v3 ) + v3

//---------------------------------------------------------
  ///
class fast_mti_integral_t
{
public:
  ///
fast_mti_integral_t();
~fast_mti_integral_t();
///
void set_initial_location(float loc_[]);
//void set_initial_orientation (float orient_[]);
void set_initial_velocity(float vel_[]);

///just weird
void update(float data_[], double elapsed_secs_);

///
void reset_state();

private:
float _acc[3];
float _loc[3];
float _vel[3];

float _prev_loc[3];
float _prev_vel[3];
float _prev_acc[3];

///seconds
float prev_secs_;
float delta_T_;
float delta_T2_2_;

///constant  gravity acceleration
const float G_acc;

///
all::math::uniform_moving_average_t<float> mav_accx_;
///
all::math::uniform_moving_average_t<float> mav_accy_;
///
all::math::uniform_moving_average_t<float> mav_accz_;


#ifdef DOFILELOG
std::fstream flog;
#endif
};
//---------------------------------------------------------
}} //all::math
//---------------------------------------------------------
#endif //fast_mti_integral_t_HPP_INCLUDED