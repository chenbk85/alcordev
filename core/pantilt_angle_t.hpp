#pragma once
//---------------------------------------------------------------------------
#include "alcor\math\all_math.h"

#include <utility>
#include <algorithm>
//---------------------------------------------------------------------------
//using namespace all;
//---------------------------------------------------------------------------
namespace all { namespace core{
//---------------------------------------------------------------------------
///
//struct pantilt_angle_t
//{
//  pantilt_angle_t(double pan_ = 0, double tilt_=0):
//        pan(pan_)
//      , tilt(tilt_)
//    {};
//
//
//  pantilt_angle_t& operator=(const pantilt_angle_t& other)
//  {
//    pan = other.pan;
//    tilt = other.tilt;
//    return *this;
//  };
//
//  double pan;
//  double tilt;
//
//};



class pantilt_angle_t
{
public:
  ///ctor from double
  pantilt_angle_t(math::deg_t, double pan = 0, double tilt = 0);
  pantilt_angle_t(math::rad_t, double pan = 0, double tilt = 0);

  ///ctor from math::angle
  pantilt_angle_t(const math::angle&, const math::angle&);

  ///copy ctor
  pantilt_angle_t(const pantilt_angle_t&);
  ///assign
  pantilt_angle_t& operator=(const pantilt_angle_t&);

  ///
  typedef std::pair<all::math::angle, all::math::angle> value_type;

  ///Setters --------------------------------------------------
  void set_pan( math::deg_t,  double);
  ///
  void set_pan( math::rad_t,  double);
  ///
  void set_tilt( math::deg_t, double);
  ///
  void set_tilt( math::rad_t, double);

  ///Pan Getters --------------------------------------------------
  double get_pan(all::math::deg_t);
  ///
  double get_pan(all::math::rad_t);
  ///

  ///Tilt Getters---------------------------------------------------  
  ///
  double get_tilt(math::deg_t);
  ///
  double get_tilt(math::rad_t);

public: //binary operators
 // ///
	//pantilt_angle_t&	operator+=(const pantilt_angle_t& temp);
 // ///
 // pantilt_angle_t&	operator-=(const pantilt_angle_t& temp);

private:
  value_type pantilt_;
  //void swap_(pantilt_angle_t&);

};
//---------------------------------------------------------------------------
//inline void pantilt_angle_t::swap_(pantilt_angle_t& other)
//{
//  std::swap(pantilt_.first,  other.get_pan());
//  std::swap(pantilt_.second, other.get_tilt());
//}
//###################################################################
//###################################################################
//########           IMPLEMENTATION                  ################
//###################################################################
//Getters  inlined--------------------------------------------------
inline  double all::core::pantilt_angle_t::get_pan(all::math::deg_t)
    {return pantilt_.first.deg();};
///
inline double all::core::pantilt_angle_t::get_pan(all::math::rad_t)
    {return (this->pantilt_).first.rad();};
///
inline  double all::core::pantilt_angle_t::get_tilt(math::deg_t)
    {return this->pantilt_.second.deg();};
///
inline  double all::core::pantilt_angle_t::get_tilt(math::rad_t)
    {return this->pantilt_.second.rad();};


//---------------------------------------------------------------------------
inline pantilt_angle_t::pantilt_angle_t(math::deg_t, double pan, double tilt)
  {
    pantilt_ = std::make_pair(
      math::angle(pan, math::deg_tag),
      math::angle(tilt, math::deg_tag)
      );
  }
//---------------------------------------------------------------------------
inline pantilt_angle_t::pantilt_angle_t(math::rad_t, double pan, double tilt)
  {
    pantilt_ = std::make_pair(
      math::angle(pan, math::deg_tag),
      math::angle(tilt, math::deg_tag)
      );
  }
//---------------------------------------------------------------------------
inline pantilt_angle_t::pantilt_angle_t( const math::angle& pan, 
                                    const math::angle& tilt)
  {
    pantilt_.first = pan;
    pantilt_.second = tilt;
  }
//---------------------------------------------------------------------------
  ///copy ctor
inline pantilt_angle_t::pantilt_angle_t(const pantilt_angle_t& other)
  {

  }
//---------------------------------------------------------------------------
  ///assign
inline pantilt_angle_t& pantilt_angle_t::operator=(const pantilt_angle_t& other)
  {
  //TODO:
	return *this;    //  nonthrowing operations only
  }
//---------------------------------------------------------------------------
  ///Setters
inline void pantilt_angle_t::set_pan( math::deg_t,double arg)
  {
    pantilt_.first.set_deg(arg);
  }
//---------------------------------------------------------------------------
  ///
inline void pantilt_angle_t::set_pan( math::rad_t,double arg)
  {
    pantilt_.first.set_rad(arg);
  }
//---------------------------------------------------------------------------
  ///
inline void pantilt_angle_t::set_tilt( math::deg_t,double arg)
  {
    pantilt_.second.set_deg(arg);
  }
//---------------------------------------------------------------------------
  ///
inline void pantilt_angle_t::set_tilt( math::rad_t,double arg)
  {    
    pantilt_.second.set_rad(arg);
  }

////---------------------------------------------------------------------------
//inline pantilt_angle_t&	pantilt_angle_t::operator+=(const pantilt_angle_t& other)
//  {
//    //casino con gli overload
//    //TODO: creare temporanei?
//    //pantilt_.first+= (math::angle)other.get_pan();
//    //pantilt_.second+= (math::angle)other.get_tilt();
//    return *this;
//  }
////---------------------------------------------------------------------------
//inline pantilt_angle_t&	pantilt_angle_t::operator-=(const pantilt_angle_t& other)
//  {
//    //pantilt_.first-= other;
//    //pantilt_.second-= other;    
//    return *this;
//  }
//---------------------------------------------------------------------------
}}//namespaces
//---------------------------------------------------------------------------