#ifndef iplimage_serializable_t_HPP
#define iplimage_serializable_t_HPP

#include "alcor/core/i_binary_serializable_t.hpp"
#include <cv.h>
#include "alcor/core/opencv_utils.hpp"
///
namespace all { namespace core {
///
  class iplimage_serializable_t : public all::core::i_binary_serializable<IplImage>
  {
    
  public:
      ///
      iplimage_serializable_t(){};

      ///
      void set_dims(size_t width, size_t height, size_t channels)
      {
      width_ = width; 
      height_=height; 
      channels_=channels;
      pixel_num_ = width_ * height_ * channels_;
      }

  public:
      ///
      virtual void serialize(const IplImage* image, std::fstream& bstream)
      {
        bstream.write((char*)image->imageData, pixel_num_ * core::size_from_ipldepth(image->depth));
      };

  private:
       size_t width_, height_, channels_;
       size_t pixel_num_;
       size_t pixel_size_;
  };

}}

#endif //iplimage_serializable_t_HPP