#ifndef CORE_TAGS_H_INCLUDED
#define CORE_TAGS_H_INCLUDED

namespace all { namespace tags {

//TYPES 
typedef struct interleaved_t {};
typedef struct planar_t {};
typedef struct rgb_t {};
typedef struct gray_t {};
//-----------------------------------------------------------------------++  
typedef struct row_major_t{};
typedef struct column_major_t{};    
//-----------------------------------------------------------------------++
typedef struct left_img_t {};
typedef struct right_img_t{};
typedef struct depth_img_t{};
//-----------------------------------------------------------------------++    
//TAGS 
//-----------------------------------------------------------------------++
static const rgb_t  rgb     = rgb_t();
static const gray_t gray    = gray_t();    
//-----------------------------------------------------------------------++
static const interleaved_t  interleaved = interleaved_t();
static const planar_t       planar      = planar_t();  
//-----------------------------------------------------------------------++    
static const row_major_t    row_major   = row_major_t();
static const olumn_major_t  column_major= column_major_t();
//------------------------------------------------------------
static const left_img_t   left_img    = left_img_t ();
static const right_img_t  right_img   = right_img_t ();
static const depth_img_t  depth_img   = depth_img_t ();

}}
#endif