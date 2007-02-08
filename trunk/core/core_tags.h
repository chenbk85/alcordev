#ifndef CORE_TAGS_H_INCLUDED
#define CORE_TAGS_H_INCLUDED

namespace all { namespace tags {

///random tags
struct interleaved_t {};
struct planar_t {};
struct rgb_t {};
struct gray_t {gray_t(){};};
//-----------------------------------------------------------------------++
static const interleaved_t interleaved_tag = interleaved_t();
static const planar_t planar_tag = planar_t();
//-----------------------------------------------------------------------++
static const rgb_t rgb_tag = rgb_t();
static const gray_t gray_tag = gray_t();
//-----------------------------------------------------------------------++
	struct left_img_t {};
	struct right_img_t{};
	struct depth_img_t{};

  static const left_img_t   left_img    = left_img_t ();
  static const right_img_t  right_img   = right_img_t ();
  static const depth_img_t  depth_img   = depth_img_t ();

}}
#endif