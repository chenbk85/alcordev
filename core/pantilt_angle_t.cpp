#include "pantilt_angle_t.h"
//using namespace boost::tuples;
//---------------------------------------------------------------------------
namespace all { namespace core {
//---------------------------------------------------------------------------
  pantilt_angle_t::pantilt_angle_t(math::deg_t, double pan, double tilt)
  {
    pantilt_ = std::make_pair(
      math::angle(pan, math::degT),
      math::angle(tilt, math::degT)
      );
  }
//---------------------------------------------------------------------------
  pantilt_angle_t::pantilt_angle_t(math::rad_t, double pan, double tilt)
  {
    pantilt_ = std::make_pair(
      math::angle(pan, math::radT),
      math::angle(tilt, math::radT)
      );
  }
//---------------------------------------------------------------------------
  pantilt_angle_t::pantilt_angle_t( const math::angle& pan, 
                                    const math::angle& tilt)
  {
    pantilt_.first = pan;
    pantilt_.second = tilt;
  }
//---------------------------------------------------------------------------
  ///copy ctor
  pantilt_angle_t::pantilt_angle_t(const pantilt_angle_t& other)
  {
  ////TODO: Implementare .... 
    //double temp = other.get_pan(math::degT);
  //double temp = 99;

  //set_pan(math::degT, temp);

    //set_pan(math::degT, other.get_pan(math::degT));
    //pantilt_.first.setDeg(other.get_pan(math::deg_t()));
  //  pantilt_.second.setDeg(other.get_tilt(math::degT));
  }
//---------------------------------------------------------------------------
  ///assign
  pantilt_angle_t& pantilt_angle_t::operator=(const pantilt_angle_t& other)
  {
	//pantilt_angle_t temp( other ); // do all the work off to the side
	//swap_( temp );    // then "commit" the work using
	return *this;    //  nonthrowing operations only
  }
//---------------------------------------------------------------------------
  ///Setters
  void pantilt_angle_t::set_pan( math::deg_t,double arg)
  {
    pantilt_.first.setDeg(arg);
  }
//---------------------------------------------------------------------------
  ///
  void pantilt_angle_t::set_pan( math::rad_t,double arg)
  {
    pantilt_.first.setRad(arg);
  }
//---------------------------------------------------------------------------
  ///
  void pantilt_angle_t::set_tilt( math::deg_t,double arg)
  {
    pantilt_.second.setDeg(arg);
  }
//---------------------------------------------------------------------------
  ///
  void pantilt_angle_t::set_tilt( math::rad_t,double arg)
  {    
    pantilt_.second.setRad(arg);
  }

//---------------------------------------------------------------------------
  pantilt_angle_t&	pantilt_angle_t::operator+=(const pantilt_angle_t& other)
  {
    //casino con gli overload
    //TODO: creare temporanei?
    //pantilt_.first+= (math::angle)other.get_pan();
    //pantilt_.second+= (math::angle)other.get_tilt();
    return *this;
  }
//---------------------------------------------------------------------------
  pantilt_angle_t&	pantilt_angle_t::operator-=(const pantilt_angle_t& other)
  {
    //pantilt_.first-= other;
    //pantilt_.second-= other;    
    return *this;
  }
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
}}//namespaces
//---------------------------------------------------------------------------