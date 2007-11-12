#include "alcor/math/fast_mti_integral_t.hpp"
//---------------------------------------------------------
namespace 
{

///
const int MVA_LENGHT = 50;
///
const float GACC = 9.80665f;
}
//---------------------------------------------------------
namespace all { namespace math {
//---------------------------------------------------------

fast_mti_integral_t::fast_mti_integral_t()
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
fast_mti_integral_t::~fast_mti_integral_t()
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
void fast_mti_integral_t::update(float data_[], double elapsed_secs_)

{ 
  //time matters
  delta_T_    = static_cast<float>(elapsed_secs_ - prev_secs_);
  prev_secs_  = static_cast<float>(elapsed_secs_);


#ifdef DOFILELOG
  flog << Macc_gl[0] << " " << Macc_gl[1] << " " << Macc_gl[2] << endl;
#endif

  //
  delta_T2_2_ = (delta_T_*delta_T_)/2.0f;
  //UPDATE
  _loc[0] =  _prev_acc[0]*delta_T2_2_ + (_prev_vel[0]*delta_T_) + _prev_loc[0];
  _loc[1] =  _prev_acc[1]*delta_T2_2_ + (_prev_vel[1]*delta_T_) + _prev_loc[1];
  _loc[2] =  _prev_acc[2]*delta_T2_2_ + (_prev_vel[2]*delta_T_) + _prev_loc[2];


  //prepare for the next cycle
  //Rotate acceleration to Global Reference R_gs and compensate
  //Rgs = [a d g; b e h; c f i]
  _acc[0] =   (data_[0] * data_[9]) + (data_[3]*data_[10]) + (data_[6]*data_[11]) ;
  _acc[1] =   (data_[1] * data_[9]) + (data_[4]*data_[10]) + (data_[7]*data_[11]) ;
  _acc[2] = ( (data_[2] * data_[9]) + (data_[5]*data_[10]) + (data_[8]*data_[11]) )- G_acc;
                   
  //ACC
  mav_accx_.push(_acc[0]);
  mav_accy_.push(_acc[1]);
  mav_accz_.push(_acc[2]);

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
    , _acc[0], _acc[1], _acc[2]);

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
}}//all::math
//---------------------------------------------------------