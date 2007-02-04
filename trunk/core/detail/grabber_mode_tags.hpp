#ifndef grabber_mode_tags_H_INCLUDED
#define grabber_mode_tags_H_INCLUDED

//Creation Tags
namespace all { namespace core {
//-------------------------------------------------------------------------++
namespace detail
		{
struct video_mode_t {};
struct camera_mode_t {};
		}//detail
//-------------------------------------------------------------------------++
static const core::detail::video_mode_t   open_video  = 
										core::detail::video_mode_t();
static const core::detail::camera_mode_t  open_camera = 
										core::detail::camera_mode_t();
}}
//-------------------------------------------------------------------------++

#endif //grabber_mode_tags_H_INCLUDED