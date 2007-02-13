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

  angle roll;
  angle pitch;
  angle yaw;
	///	
	rpy_angle_t& operator=( const rpy_angle_t& );

private:
	///
	void swap_( rpy_angle_t& other ) throw();

};

///////////////////////////////////////////////////////////////////////////++
inline rpy_angle_t::rpy_angle_t()//:
//roll(0),pitch(0),yaw(0) 
{
}
///////////////////////////////////////////////////////////////////////////++
inline rpy_angle_t::rpy_angle_t(const rpy_angle_t & other)
{
  roll = other.roll;
  pitch = other.pitch;
  yaw=other.yaw;
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
inline std::ostream& operator<<(std::ostream& os, const rpy_angle_t& temp)
{	
	//boost::format fmrpy("\nRoll:  %3.2f\nPitch: %3.2f\nYaw:   %3.2f\nElapsed:%d\n");
	//		os << fmrpy		% temp.roll 
	//						% temp.pitch 
	//						% temp.yaw 
	//						% temp.elapsed;
  os << "Roll: " << temp.roll << "Pitch: " << temp.pitch << "Yaw: " << temp.yaw;
	return os;
}
///////////////////////////////////////////////////////////////////////////++
}}//namespace all::math
///////////////////////////////////////////////////////////////////////////++
#endif //rpy_angle_t_H_INC