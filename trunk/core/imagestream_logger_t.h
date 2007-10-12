#ifndef image_stream_logger_t_H_INCLUDED
#define image_stream_logger_t_H_INCLUDED
//-------------------------------------------------------------------------++
#include <iostream>
#include <fstream>
#include <boost/timer.hpp>
//-------------------------------------------------------------------------++
#include <cv.h>
//-------------------------------------------------------------------------++
#include "alcor/core/cameralog_inc.h"
//-------------------------------------------------------------------------++
namespace all {
  namespace core {
//-------------------------------------------------------------------------++
///image_stream_logger_t
template <typename T>
class image_stream_logger_t
{
public:
  ///ctor
  image_stream_logger_t(const std::string& fname = "cameralog.bin"):
         logname_(fname)
        ,nsamples_(0)
        ,img_chunk_size_(0)
        ,height_(0)
        ,width_(0)
        ,depth_(0)
        ,header_size_(0)
        ,elapsed_(0)
         {
          //
          gazelog_.open(logname_.c_str(),std::ios::out|std::ios::binary);
         }
//-------------------------------------------------------------------------++
  ///dtor
  ~image_stream_logger_t()
  {
    printf("writing down the header\n");
    write_header_();
    printf("closing the binary log\n");
    gazelog_.close(); 
  }
//-------------------------------------------------------------------------++
public:
  ///
  typedef T value_type;
//-------------------------------------------------------------------------++
  ///
  bool open(log_type logtype, size_t height, size_t width, size_t depth)
  {
    //
    img_chunk_size_     = 
      height*width*depth*sizeof(image_stream_logger_t::value_type);

    //
    height_ = height;
    width_  = width;
    depth_  = depth;
    //
    logtype_ = logtype;
    //
    header_size_ =  + sizeof(nsamples_)
                    + sizeof(logtype_)
                    + sizeof(height_)
                    + sizeof(width_)
                    + sizeof(depth_)
                    + sizeof(img_chunk_size_)
                    + sizeof(elapsed_);
    //
    gazelog_.seekp(header_size_);

    return true;
  }
//-------------------------------------------------------------------------++
  size_t nsamples() const {return nsamples_;}
//-------------------------------------------------------------------------++
  ///
  void add(boost::shared_array<T> data, double timestamp)
  {
    gazelog_.write((char*)data.get(), img_chunk_size_ );
    gazelog_.write((char*)&timestamp , sizeof(timestamp) );
    nsamples_++;
  }
//-------------------------------------------------------------------------++
  ///
  void add_iplimage(IplImage* iplimage, double timestamp)
  {
    gazelog_.write(iplimage->imageData, img_chunk_size_ );
    gazelog_.write((char*)&timestamp , sizeof(timestamp) );
    nsamples_++;
  }
//-------------------------------------------------------------------------++
private:
  ///
  void write_header_()
  {
    //go to the beginning
    gazelog_.seekp(std::ios::beg);
    //write down the header
    gazelog_.write( (char*)&nsamples_,   sizeof(nsamples_)); 
    gazelog_.write( (char*)&logtype_,   sizeof(logtype_));
    gazelog_.write( (char*)&img_chunk_size_, sizeof(img_chunk_size_));
    gazelog_.write( (char*)&height_,     sizeof(height_));
    gazelog_.write( (char*)&width_,      sizeof(width_));
    gazelog_.write( (char*)&depth_,      sizeof(depth_));
    gazelog_.write( (char*)&elapsed_,    sizeof(elapsed_));

    //
    printf("-> WRITE HEADER\n");
    printf("-> nsamples: %d\n", nsamples_);
    printf("-> logtype: %d\n", logtype_);
    printf("-> chunk_size: %d\n", img_chunk_size_);
    printf("-> Image Dims: %d:%d:%d\n", height_, width_, depth_);
    printf("-> Elapsed %f seconds.\n", elapsed_);
  }
//-------------------------------------------------------------------------++
  ///Binary Data Stream
  std::fstream gazelog_;
  ///
  std::string logname_;
  ///
  size_t nsamples_;
  ///
  size_t img_chunk_size_;
  ///
  size_t header_size_;
  ///
  size_t height_;
  ///
  size_t width_;
  ///
  size_t depth_;
  ///
  boost::timer timer_;
  ///
  double elapsed_;
  ///
  log_type logtype_;
};
//-------------------------------------------------------------------------++
  }} //all::core
//-------------------------------------------------------------------------++
#endif //image_stream_logger_t_H_INCLUDED
//--------------------------------------------------------------------------++