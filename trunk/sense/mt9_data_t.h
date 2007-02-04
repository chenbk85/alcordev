#ifndef shared_mt9_data_t_H_INCLUDED
#define shared_mt9_data_t_H_INCLUDED
//-------------------------------------------------------------------------++
#include <string>
#include <algorithm>
#include <iostream>
#include <boost/format.hpp> 
//include for angles
#include <quan/angle.hpp>
//#include <quan/angle/out/angle.hpp>
//-------------------------------------------------------------------------++
using quan::angle;
//-------------------------------------------------------------------------++
namespace all { namespace sense {
//-------------------------------------------------------------------------++
///
class mt9_data_t
{
public:
	///
	mt9_data_t();
	///
	mt9_data_t(const mt9_data_t&);
	///
	float roll;
	angle roll_;
	///
	float pitch;
	///
	float yaw;
	///
	long  elapsed;//msec
	///future use
	long  tag;
	///
	long msec_since(const mt9_data_t&) const;
	///	
	mt9_data_t& operator=( const mt9_data_t& );

private:
	///
	void swap_( mt9_data_t& other ) throw();

};
///////////////////////////////////////////////////////////////////////////++
inline mt9_data_t::mt9_data_t():
roll(0.f)
,pitch(0.f)
,yaw(0.f)
,elapsed(0l)
,tag(0l)
{
	//roll_ = quan::angle::deg;
}
///////////////////////////////////////////////////////////////////////////++
inline mt9_data_t::mt9_data_t(const mt9_data_t & other)
{
	roll = other.roll;

	roll_ = other.roll_;

	pitch = other.pitch;

	yaw=other.yaw;

	elapsed=other.elapsed;
	
	tag = other.tag;
}
///////////////////////////////////////////////////////////////////////////++
inline long mt9_data_t::msec_since (const mt9_data_t& other) const
{
	return (elapsed - other.elapsed);
}
///////////////////////////////////////////////////////////////////////////++
inline mt9_data_t& mt9_data_t::operator=( const mt9_data_t& other )
{
	mt9_data_t temp( other ); // do all the work off to the side
	swap_( temp );    // then "commit" the work using
	return *this;    //  nonthrowing operations only
}
///////////////////////////////////////////////////////////////////////////++
inline void mt9_data_t::swap_(mt9_data_t& other)
{
	std::swap(roll_,other.roll_);
	std::swap(roll, other.roll);
	std::swap(pitch, other.pitch);
	std::swap(yaw,	other.yaw );
	std::swap(elapsed, other.elapsed);
	std::swap(tag, other.tag);
}
///////////////////////////////////////////////////////////////////////////++

std::ostream& operator<<(std::ostream& os, const mt9_data_t& temp)
{	
	////boost::format fmrpy("\nRoll:  %3.2f\nPitch: %3.2f\nYaw:   %3.2f\nElapsed:%d\n");
	////		os << fmrpy		% temp.roll_ 
	////						% temp.pitch 
	////						% temp.yaw 
	////						% temp.elapsed;
	//os << "Roll: " << quan::angle::deg(temp.roll_) << std::endl;

	return os;
}
//-------------------------------------------------------------------------++
	}}//namespaces
//-------------------------------------------------------------------------++
#endif //shared_mt9_data_t_H_INCLUDED