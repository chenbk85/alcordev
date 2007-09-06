#ifndef imagestream_reader_t_H_INCLUDED
#define imagestream_reader_t_H_INCLUDED
//-------------------------------------------------------------------
#include <iostream>
#include <fstream>
//-------------------------------------------------------------------
namespace all { namespace core {
//-------------------------------------------------------------------
  ///
  template <typename T>
  class imagestream_reader_t
  {
  public:
    ///
    imagestream_reader_t():
         current_sample_(0)
        ,nsamples_(0)
        ,img_chunk_size_(0)
        ,height_(0)
        ,width_(0)
        ,depth_(0)
        ,header_size_(0)
    {

    }

    ///
    ~imagestream_reader_t()
    {
      gazelog_.close();
    }

    ///
    size_t nsamples() const {return nsamples_;}

    ///
    size_t height() const {return height_;}

    ///
    size_t width() const {return width_;}

    ///
    size_t depth() const {return depth_;}

    ///
    size_t current_sample() const {return current_sample_;}

    ///
    bool open(const std::string& fname = "cameralog.bin")
    {
      //
      gazelog_.open(fname.c_str(), std::ios::in|std::ios::binary);
      //
      gazelog_.read((char*)&nsamples_, sizeof(nsamples_));
        printf("-> nsamples: %d\n", nsamples_);

      //
      gazelog_.read((char*)&img_chunk_size_, sizeof(img_chunk_size_));
        printf("-> img_chunk_size_: %d\n", img_chunk_size_);

      //H
      gazelog_.read((char*)&height_, sizeof(height_));
      //W
      gazelog_.read((char*)&width_, sizeof(width_));
      //D
      gazelog_.read((char*)&depth_, sizeof(depth_));
        //
        printf("-> Image Dims: %d:%d:%d\n", height_, width_, depth_);

      //
      gazelog_.read((char*)&elapsed_, sizeof(elapsed_));
        //
        printf("-> Seconds: %f\n", elapsed_);

      //HEADER SIZE
      header_size_ =  + sizeof(nsamples_)
                      + sizeof(height_)
                      + sizeof(width_)
                      + sizeof(depth_)
                      + sizeof(img_chunk_size_)
                      + sizeof(elapsed_);

      return true;
    }

    ///
    bool sample(boost::shared_array<T> data, double& timestamp)
    {
      if(!eof())
      {
        //
        current_sample_++;

        //img chunk
        gazelog_.read((char*)&data[0],  img_chunk_size_);
        //elapsed time chunk
        gazelog_.read((char*)&timestamp,  sizeof(timestamp));
      }
      else
        return false;

      return true;
    }

    ///
    bool eof()
    {
      return current_sample_ == nsamples_;
    }

    ///
    void reset()
    {
      gazelog_.seekg(header_offset_);
      current_sample_ = 0;
    }

    ///
    typedef T value_type;
//-------------------------------------------------------------------------++
  private:
    ///Binary Data Stream
    std::fstream gazelog_;
    ///
    std::string logname_;
    ///
    size_t nsamples_;
    ///
    size_t current_sample_;
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
    double elapsed_;
  };
//-------------------------------------------------------------------
}}//all::core
//-------------------------------------------------------------------
#endif //imagestream_reader_t_H_INCLUDED