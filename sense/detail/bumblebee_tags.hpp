#ifndef bumblebee_tags_H_INCLUDED
#define bumblebee_tags_H_INCLUDED
//-----------------------------------------------------------------------++
//Creation Tags
namespace all { 
	namespace sense {
//-----------------------------------------------------------------------++
namespace detail
		{
		struct left_cam_t{};
		struct right_cam_t{};

		struct stereo_mode_t{};
		struct color_mode_t{};
		struct std_mode_t{};
		}//detail
//-----------------------------------------------------------------------++
	static const sense::detail::left_cam_t left_cam 
		= sense::detail::left_cam_t();
	static const sense::detail::right_cam_t right_cam 
		= sense::detail::right_cam_t();
//-----------------------------------------------------------------------++
	static const sense::detail::stereo_mode_t stereo_mode 
		= sense::detail::stereo_mode_t();
	static const sense::detail::color_mode_t color_mode 
		= sense::detail::color_mode_t();
	static const sense::detail::std_mode_t std_mode 
		= sense::detail::std_mode_t();
//-----------------------------------------------------------------------++
//-----------------------------------------------------------------------++
	}}
//-----------------------------------------------------------------------++
#endif //bumblebee_tags_H_INCLUDED