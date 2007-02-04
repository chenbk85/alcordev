#ifndef image_info_t_H_INCLUDED
#define image_info_t_H_INCLUDED
//----------------------------------------------------------------++
#include <algorithm>
//----------------------------------------------------------------++
namespace all { namespace core {
//----------------------------------------------------------------++
struct image_info_t
	{
	public:
    ///
	image_info_t();
	///
	image_info_t(const image_info_t&);
	///
	int height;
	///
	int width;
	///
	int channels;
  ///
  double focal;
  ///what for?
  std::size_t memory_size;
	///	
	image_info_t& operator=( const image_info_t& );

	private:
	///
	void swap_( image_info_t& other ) throw();
	};
///////////////////////////////////////////////////////////////////////////++
inline image_info_t::image_info_t():
height(0),width(0),channels(0),memory_size(0)
{
}
///////////////////////////////////////////////////////////////////////////++
inline image_info_t::image_info_t(const image_info_t & other)
{
	height = other.height;

	width = other.width;

	channels=other.channels;

	memory_size=other.memory_size;
}
///////////////////////////////////////////////////////////////////////////++
inline image_info_t& image_info_t::operator=( const image_info_t& other )
{
	image_info_t temp( other ); // do all the work off to the side
	swap_( temp );    // then "commit" the work using
	return *this;    //  nonthrowing operations only
}
///////////////////////////////////////////////////////////////////////////++
inline void image_info_t::swap_(image_info_t& other)
{
	std::swap(height, other.height);
	std::swap(width, other.width);
	std::swap(channels,	other.channels );
	std::swap(memory_size, other.memory_size);
}
///////////////////////////////////////////////////////////////////////////++
inline std::ostream& operator<<(std::ostream& os, const image_info_t& temp)
{	
	//boost::format fmrpy("\nRoll:  %3.2f\nPitch: %3.2f\nYaw:   %3.2f\nElapsed:%d\n");
	//		os << fmrpy		% temp.roll 
	//						% temp.pitch 
	//						% temp.yaw 
	//						% temp.elapsed;
	return os;
}
//----------------------------------------------------------------++
	}}//all::core
//----------------------------------------------------------------++
#endif //image_info_t_H_INCLUDED
///////////////////////////////////////////////////////////////////////////++