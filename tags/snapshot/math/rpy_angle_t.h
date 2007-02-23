#ifndef  rpy_angle_t_H_INC
#define  rpy_angle_t_H_INC

#include "angle.h"

namespace all { namespace math {
///
class rpy_angle_t
{
public:
  ///
	rpy_angle_t();
	///
	rpy_angle_t(const rpy_angle_t&);
	///
	rpy_angle_t(const angle& r, const angle& p ,const angle& y);

  angle roll;
  angle pitch;
  angle yaw;
	///	
	rpy_angle_t& operator=( const rpy_angle_t& );
  ///
  void print ();

private:
	///
	void swap_( rpy_angle_t& other ) throw();

};

///////////////////////////////////////////////////////////////////////////++
inline rpy_angle_t::rpy_angle_t()
{
}
///////////////////////////////////////////////////////////////////////////++
inline rpy_angle_t::rpy_angle_t(const angle& r, const angle& p ,const angle& y)
{
  roll = r;
  pitch = p;
  yaw = y;
}
///////////////////////////////////////////////////////////////////////////++
inline rpy_angle_t::rpy_angle_t(const rpy_angle_t & other)
{
  roll  = other.roll;
  pitch = other.pitch;
  yaw   = other.yaw;
}
///////////////////////////////////////////////////////////////////////////++
inline rpy_angle_t& rpy_angle_t::operator=( const rpy_angle_t& other )
{
	rpy_angle_t temp( other ); // do all the work off to the side
	swap_( temp );    // then "commit" the work using
	return *this;    //  nonthrowing operations only
}
///////////////////////////////////////////////////////////////////////////++
inline void rpy_angle_t::swap_(rpy_angle_t& other)
{
  std::swap(roll, other.roll);
  std::swap(pitch, other.pitch);
  std::swap(yaw,	other.yaw );
}
///////////////////////////////////////////////////////////////////////////++
///
inline  void rpy_angle_t::print ()
{
  printf("Roll: %3.2f Pitch: %3.2f Yaw: %3.2f\n",roll.deg(), pitch.deg(), yaw.deg() );
}
///////////////////////////////////////////////////////////////////////////++
inline std::ostream& operator<<(std::ostream& os, const rpy_angle_t& temp)
{	
  os << "Roll: " << temp.roll.deg() << "Pitch: " << temp.pitch.deg() << "Yaw: " << temp.yaw.deg();
	return os;
}
///////////////////////////////////////////////////////////////////////////++
}}//namespace all::math
///////////////////////////////////////////////////////////////////////////++
#endif //rpy_angle_t_H_INC