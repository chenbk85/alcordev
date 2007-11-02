#ifndef fast_mti_integral_t_HPP_INCLUDED
#define fast_mti_integral_t_HPP_INCLUDED
//---------------------------------------------------------
#include <boost/numeric/ublas/vector.hpp>
//---------------------------------------------------------
using namespace boost::numeric;
//---------------------------------------------------------
namespace all { namespace math {
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
///
void set_initial_location(float loc_[]);
void set_initial_orintation (float orient_[]);
void set_initial_velocity(float vel_[]);

///just weird
void update(float data_[], double elapsed_secs_);

///
void reset_state();

private:

float _loc[3];
float _vel[3];
float _orient[3];

///seconds
float prev_secs_;

///constant  gravity acceleration
const float G_acc;

};
//---------------------------------------------------------
inline fast_mti_integral_t::fast_mti_integral_t()
:   G_acc(9.80665f)
  , prev_secs_(0.0f)
{

}
//---------------------------------------------------------
//[rot(9) acc(3)]
//[0-8 9-11]
//rot is [a b c d e f g h i]
//acc is [accx accy accz]
//elasped_ is in seconds
inline void fast_mti_integral_t::update(float data_[], double elapsed_secs_)

{ 
  //time matters
  float delta_T = static_cast<float>(elapsed_secs_ - prev_secs_);
  prev_secs_    = static_cast<float>(elapsed_secs_);

  //=> given accelerations, compensate for G
  //rotate G_acc=[0 0 G] into the MTi reference system (S)
  //G_mti = rotSG * G_acc = (c*G, f*G, i*G)' 
  //NOTE: first two components of G_acc are null
  float G_MTi[] = {data_[2]*G_acc, data_[5]*G_acc, data_[8]*G_acc}; 
  //ublas::vector<float> uG_MTi(3)  = {data_[2]*G_acc, data_[5]*G_acc, data_[8]*G_acc};
  ////Compensated accelerations
  float M_acc[]  = {data_[9] - G_MTi[0]
                  , data_[10]- G_MTi[1]
                  , data_[11]- G_MTi[2]};
  ////
                    
  printf("\nNOT compensated: %4.2f %4.2f %4.2f\n"
    , data_[9],data_[10],data_[11]);

  printf("Compensated: %4.2f %4.2f %4.2f\n"
    , M_acc[0],M_acc[1],M_acc[2]);

}
//---------------------------------------------------------
}} //all::math
//---------------------------------------------------------
#endif //fast_mti_integral_t_HPP_INCLUDED