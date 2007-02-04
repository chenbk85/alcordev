#ifndef bumblebee_t_H_INCLUDED
#define bumblebee_t_H_INCLUDED
//-----------------------------------------------------------------------++
#include "alcor2/sense/i_device_driver.h"
#include "alcor2/sense/detail/bumblebee_tags.hpp"
//-----------------------------------------------------------------------++
#include "alcor2/core/core.h"
//-----------------------------------------------------------------------++
#include <memory>
//-----------------------------------------------------------------------++
namespace all
	{ 
	namespace sense 
		{
		///bumblebee_impl
		class bumblebee_impl;
		///A Point Grey Bumblebee digital camera.
		class bumblebee_t;
		}
	}
//-----------------------------------------------------------------------++
///
class all::sense::bumblebee_t : public i_device_driver
	{
public:
	///Constructor.
	bumblebee_t();

	///Inherited
	bool open(const std::string&);
	///Inherited
	bool close();

	///initialize
	bool initialize(sense::detail::stereo_mode_t);
	///initialize
	bool initialize(sense::detail::color_mode_t);

	///Grabs Stereo Color Image.
	void grab_color( );

	///grab
	void grab();

	///grab
	void grab(sense::detail::right_cam_t);
	///grab
	void grab(sense::detail::left_cam_t);

public:
	///nrows
	size_t nrows() const;
	///ncols
	size_t ncols() const

public:
	///get_color_buffer
	bool  get_color_buffer(core::byte_t*, sense::detail::right_cam_t);
	///get_color_buffer
	bool  get_color_buffer(core::byte_t*, sense::detail::left_cam_t);

	///get_depth_buffer
	bool  get_depth_buffer(float*);

protected:
	///PIMPL Idiom
	std::auto_ptr<bumblebee_impl> pimpl;
	};
//-----------------------------------------------------------------------++
#endif //bumblebee_t_H_INCLUDED