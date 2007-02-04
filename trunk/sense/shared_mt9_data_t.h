#ifndef shared_mt9_data_t_H_INCLUDED
#define shared_mt9_data_t_H_INCLUDED
//-------------------------------------------------------------------------++
#include <string>
#include <algorithm>
#include <iostream>
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
roll(0.f),pitch(0.f),yaw(0.f),elapsed(0l),tag(0l)
{
};
///////////////////////////////////////////////////////////////////////////++
inline mt9_data_t::mt9_data_t(const mt9_data_t & other)
{
	roll = other.roll;

	pitch = other.pitch;

	yaw=other.yaw;

	elapsed=other.elapsed;
	
	tag = other.tag;
};
///////////////////////////////////////////////////////////////////////////++
inline long mt9_data_t::msec_since (const mt9_data_t& other) const
{
	return (elapsed - other.elapsed);
};
///////////////////////////////////////////////////////////////////////////++
inline mt9_data_t& mt9_data_t::operator=( const mt9_data_t& other )
{
	mt9_data_t temp( other ); // do all the work off to the side
	swap_( temp );    // then "commit" the work using
	return *this;    //  nonthrowing operations only
};
///////////////////////////////////////////////////////////////////////////++
inline void mt9_data_t::swap_(mt9_data_t& other)
{
	std::swap(roll, other.roll);
	std::swap(pitch, other.pitch);
	std::swap(yaw,	other.yaw );
	std::swap(elapsed, other.elapsed);
	std::swap(tag, other.tag);
};

//-------------------------------------------------------------------------++
	}}//namespaces
///////////////////////////////////////////////////////////////////////////++
inline std::ostream& operator<<(std::ostream& os, const all::sense::mt9_data_t& temp)
{	
	os	<< "Roll:    " << temp.roll  << "\n"
		<< "Pitch:   " << temp.pitch << "\n"
		<< "Yaw:     " << temp.yaw   << "\n"
		<< "Elapsed: "<<temp.elapsed << "\n";

	return os;
};
///////////////////////////////////////////////////////////////////////////++
//inline std::ostream& operator<<(std::ostream& os, all::sense::mt9_data_t* temp)
//{	
//	os	<< "Roll:    " << *temp.roll  << "\n"
//		<< "Pitch:   " << *temp.pitch << "\n"
//		<< "Yaw:     " << *temp.yaw   << "\n"
//		<< "Elapsed: "<<*temp.elapsed << "\n";
//
//	return os;
//};
//-------------------------------------------------------------------------++
#endif //shared_mt9_data_t_H_INCLUDED