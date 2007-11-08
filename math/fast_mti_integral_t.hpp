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
namespace 
{
  ///
template <typename T>
void truncate_2(T& val)
{
  int temp = static_cast<int>(val*100);
  val = static_cast<T>(temp)/static_cast<T>(100.0);
}
//---------------------------------------------------------
///
template <typename T>
void truncate_1(T& val)
{
  int temp = static_cast<int>(val*10);
  val = static_cast<T>(temp)/static_cast<T>(10.0);
}
///
const int MVA_LENGHT = 50;
///
const float GACC = 9.80665f;
}
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
all::math::simple_moving_average_t<float> mav_accx_;
///
all::math::simple_moving_average_t<float> mav_accy_;
///
all::math::simple_moving_average_t<float> mav_accz_;


#ifdef DOFILELOG
std::fstream flog;
#endif
};
//---------------------------------------------------------
inline fast_mti_integral_t::fast_mti_integral_t()
:   G_acc(GACC)
  , prev_secs_(0.0f)
  , delta_T_(0.0f)
  , delta_T2_2_(0.0f)
  , mav_accx_(MVA_LENGHT)
  , mav_accy_(MVA_LENGHT)
  , mav_accz_(MVA_LENGHT)

{
  _prev_acc[0] = 0;  _prev_acc[1] = 0;  _prev_acc[2] = 0;
  _prev_vel[0] = 0;  _prev_vel[1] = 0;  _prev_vel[2] = 0;
  _prev_loc[0] = 0;  _prev_loc[1] = 0;  _prev_loc[2] = 0;

  #ifdef DOFILELOG
  flog.open("mtilog.dat",std::ios::out);
  #endif
}
//---------------------------------------------------------
inline fast_mti_integral_t::~fast_mti_integral_t()
{
  #ifdef DOFILELOG
  flog.close();
  #endif
}
//---------------------------------------------------------
//[rot(9) acc(3)]
//[0-8 9-11]
//rot is [a b c d e f g h i]
//        0 1 2 3 4 5 6 7 8
//acc is [accx accy accz]
//elasped_ is in seconds
inline void fast_mti_integral_t::update(float data_[], double elapsed_secs_)

{ 
  //time matters
  delta_T_ = static_cast<float>(elapsed_secs_ - prev_secs_);
  prev_secs_    = static_cast<float>(elapsed_secs_);

  //Rotate acceleration to Global Reference R_gs and compensate
  //Rgs = [a d g; b e h; c f i]
  float Macc_gl[]= {  data_[0] * data_[9] + data_[3]*data_[10] + data_[6]*data_[11]
                    , data_[1] * data_[9] + data_[4]*data_[10] + data_[7]*data_[11]
                    ,(data_[2] * data_[9] + data_[5]*data_[10] + data_[8]*data_[11])
                      - G_acc
                   };

#ifdef DOFILELOG
  flog << Macc_gl[0] << " " << Macc_gl[1] << " " << Macc_gl[2] << endl;
#endif

  //
  delta_T2_2_ = (delta_T_*delta_T_)/2.0f;
  ////UPDATE
  _loc[0] =  _prev_acc[0]*delta_T2_2_ + (_prev_vel[0]*delta_T_) + _prev_loc[0];
  _loc[1] =  _prev_acc[1]*delta_T2_2_ + (_prev_vel[1]*delta_T_) + _prev_loc[1];
  _loc[2] =  _prev_acc[2]*delta_T2_2_ + (_prev_vel[2]*delta_T_) + _prev_loc[2];


  //prepare for the next cycle
  //ACC
  mav_accx_.push(Macc_gl[0]);
  mav_accy_.push(Macc_gl[1]);
  mav_accz_.push(Macc_gl[2]);

  _prev_acc[0] = mav_accx_.mav();
  _prev_acc[1] = mav_accy_.mav();
  _prev_acc[2] = mav_accz_.mav();

  //VEL
  _vel[0] = (mav_accx_.mav()*delta_T_) + _prev_vel[0];
  _vel[1] = (mav_accy_.mav()*delta_T_) + _prev_vel[1];
  _vel[2] = (mav_accz_.mav()*delta_T_) + _prev_vel[2];
  //  
  _prev_vel[0] = _vel[0];
  _prev_vel[1] = _vel[1];
  _prev_vel[2] = _vel[2];

  //LOC
  _prev_loc[0] = _loc[0];
  _prev_loc[1] = _loc[1];
  _prev_loc[2] = _loc[2];


#ifdef _MTILOGONSCREEN_
  ////
  printf("\nNOT compensated: %4.3f %4.3f %4.3f\n"
    , data_[9],data_[10],data_[11]);

  printf("Compensated: %4.3f %4.3f %4.3f\n"
    , Macc_gl[0],Macc_gl[1],Macc_gl[2]);

  printf("Smoothed: %4.3f %4.3f %4.3f\n"
    , mav_accx_.mav(), mav_accy_.mav(), mav_accz_.mav());

  printf("VEL: %4.3f %4.3f %4.3f\n"
    , _vel[0], _vel[1], _vel[2]);
  //
  printf("Location: %4.2f %4.2f %4.2f\n"
  , _loc[0],_loc[1],_loc[2]);
#endif
}
//---------------------------------------------------------
}} //all::math
//---------------------------------------------------------
#endif //fast_mti_integral_t_HPP_INCLUDED