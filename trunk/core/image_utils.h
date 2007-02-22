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
//#pragma omp parallel   
//  {

  for(int row = height; row; --row)
  {
    //#pragma omp for 
    for(int col = width; col; --col)
    {
      //printf("R: %d C: %d\n",row-1,col-1);
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

  //}//parallel
  arr=(temp);
    }
//-------------------------------------------------------------------
}}//namespace all::core
//-------------------------------------------------------------------
#endif //image_utils_H_INCLUDED