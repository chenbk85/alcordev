#ifndef image_utils_H_INCLUDED
#define image_utils_H_INCLUDED
//-------------------------------------------------------------------
#include "alcor/core/core.h"
//-------------------------------------------------------------------
namespace all { namespace core {
//-------------------------------------------------------------------
struct change_ordering
{
  static void  to_interleaved(all::core::uint8_sarr& arr, 
                        size_t height, 
                        size_t width, 
                        size_t depth);

  static void  to_interleaved(all::core::single_sarr& arr, 
                      size_t height, 
                      size_t width, 
                      size_t depth);

  static void  to_topleft(all::core::uint8_sarr& arr, 
                      size_t height, 
                      size_t width, 
                      size_t depth);

  static void  from_rgba_opengl_to_planar(
                      const all::core::uint8_sarr& arr, 
                      all::core::uint8_sarr& out,
                      size_t height, 
                      size_t width);

  static void  from_rgba_opengl_to_interleaved(
                    const all::core::uint8_sarr& arr, 
                    all::core::uint8_sarr& out,
                    size_t height, 
                    size_t width);

  static void to_planar(all::core::uint8_sarr& arr, 
                        size_t height, 
                        size_t width, 
                        size_t depth);
};
//-------------------------------------------------------------------
inline void  change_ordering::to_interleaved(all::core::uint8_sarr& arr, 
                        size_t height, 
                        size_t width, 
                        size_t depth)

  {
  size_t channel_stride  = height*width;
  size_t channel_stride2 = channel_stride*2;

  size_t row_stride     = width;

  core::uint8_ptr temp = new core::uint8_t[height*width*depth];

  size_t comp_index_pl    = 0;
  size_t comp_index_il    = 0;

  for(int row = height; row ; --row)
  {
    for(int col = width; col ; --col)
    {
    comp_index_pl = ((row-1)*row_stride) + (col-1);
    comp_index_il = comp_index_pl*3;

    //R
    temp[comp_index_il]      = arr[comp_index_pl];
    //G
    temp[comp_index_il + 1]  = arr[comp_index_pl + channel_stride];
    //B
    temp[comp_index_il + 2]  = arr[comp_index_pl + channel_stride2];
    }
  }

  arr.reset(temp);

  }
//-------------------------------------------------------------------
inline void  change_ordering::to_interleaved(all::core::single_sarr& arr, 
                        size_t height, 
                        size_t width, 
                        size_t depth)

  {
    size_t channel_stride  = height*width;//*sizeof(core::single_t);
    size_t channel_stride2 = channel_stride*2;

    size_t row_stride     = width;//*sizeof(core::single_t);

    core::single_ptr temp = new core::single_t[height*width*depth];

    size_t comp_index_pl    = 0;
    size_t comp_index_il    = 0;

    for(int row = height; row ; --row)
    {
      for(int col = width; col ; --col)
      {
      comp_index_pl = ((row-1)*row_stride) + ( (col-1) );
      comp_index_il = comp_index_pl*3;

      //R
      temp[comp_index_il]      = arr[comp_index_pl];
      //G
      temp[comp_index_il + 1]  = arr[comp_index_pl + channel_stride];
      //B
      temp[comp_index_il + 2]  = arr[comp_index_pl + channel_stride2];
      }
    }

  arr.reset(temp);

  }
//-------------------------------------------------------------------
///FROM BOTTOM LEFT TO TOPLEFT ORIGIN -- PLANAR
inline void  change_ordering::to_topleft(all::core::uint8_sarr& arr, 
                    size_t height, 
                    size_t width, 
                    size_t depth)
{
  size_t channel_stride  = height*width;
  size_t channel_stride2 = channel_stride*2;

  size_t row_stride     = width;

  core::uint8_ptr temp = new core::uint8_t[height*width*depth];

  //bottom left index
  size_t comp_index_bl    = 0;
  //topleft index
  size_t comp_index_tl    = 0;

  for(int row = height; row ; --row)
  {
    //copy (bottomleft) row to its right (topleft) position
    //origin pointer
    core::uint8_ptr from_rowptr =  arr.get()+ ((row-1)        * row_stride);
    core::uint8_ptr to_rowptr   =  temp     + ((height - row) * row_stride);

    //plane 0
    memcpy(to_rowptr, from_rowptr, row_stride);
    //plane 1
    memcpy(to_rowptr+channel_stride,  from_rowptr+channel_stride, row_stride);
    //plane 2
    memcpy(to_rowptr+channel_stride2, from_rowptr+channel_stride2, row_stride);
    
  }

  arr.reset(temp);
}
//-------------------------------------------------------------------
//origin is bottom left
inline void  change_ordering::from_rgba_opengl_to_planar(
                      const all::core::uint8_sarr& in,
                      all::core::uint8_sarr& out,
                      size_t height, 
                      size_t width)
  {

  size_t channel_stride  = height*width;
  size_t channel_stride2 = channel_stride*2;

	// get num pixels
	unsigned int total_size = width * height * 4;
	unsigned int this_pixel_start = 0;
  unsigned int out_pixel_start =0;

  	// loop through each pixel
	for( ; this_pixel_start != total_size  ; this_pixel_start += 4, ++out_pixel_start  ) 
  {

		//// get address of pixel data
    const unsigned char* pixel = in.get() + this_pixel_start;

    out[out_pixel_start] = pixel[0];
    out[out_pixel_start + channel_stride] = pixel[1];
    out[out_pixel_start + channel_stride2] = pixel[2];
	}

  }
//-------------------------------------------------------------------
inline void  change_ordering::from_rgba_opengl_to_interleaved(
                    const all::core::uint8_sarr& arr, 
                    all::core::uint8_sarr& out,
                    size_t height, 
                    size_t width)
{
	// get num pixels
	unsigned int total_size = width * height * 4;
	unsigned int this_pixel_start = 0;
  unsigned int out_pixel_start =0;

  	// loop through each pixel
	for( ; this_pixel_start != total_size  ; this_pixel_start += 4, out_pixel_start +=3  ) 
  {

		//// get address of pixel data
    const unsigned char* pixel = arr.get() + this_pixel_start;

    out[out_pixel_start] = pixel[0];
    out[out_pixel_start + 1] = pixel[1];
    out[out_pixel_start + 2] = pixel[2];
	}
}
//-------------------------------------------------------------------
inline  void change_ordering::to_planar(all::core::uint8_sarr& arr, 
                        size_t height, 
                        size_t width, 
                        size_t depth=3)
    {
  size_t channel_stride  = height*width;
  size_t channel_stride2 = channel_stride*2;

  size_t row_stride     = width;

  //core::uint8_ptr temp = new core::uint8_t[height*width*depth];
  core::uint8_sarr temp (new core::uint8_t[height*width*depth]);


  size_t comp_index_pln    = 0;
  size_t comp_index_ilv    = 0;

  for(int row = height; row; --row)
  {

    for(int col = width; col; --col)
    {
    //
    comp_index_pln = ((row-1)*row_stride) + (col-1);
    comp_index_ilv = comp_index_pln*3;

    //R
    temp[comp_index_pln]                    = arr[comp_index_ilv];
    //G
    temp[comp_index_pln + channel_stride]   = arr[comp_index_ilv + 1];
    //B
    temp[comp_index_pln + channel_stride2]  = arr[comp_index_ilv + 2];
    }
  }

  arr = temp;
    }
//-------------------------------------------------------------------
}}//namespace all::core
//-------------------------------------------------------------------
#endif //image_utils_H_INCLUDED