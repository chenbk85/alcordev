#ifndef moving_average_t_HPP_INCLUDED
#define moving_average_t_HPP_INCLUDED
//---------------------------------------------------------
#include <boost/circular_buffer.hpp>
#include <boost/foreach.hpp>
#include <vector>
//---------------------------------------------------------
#define foreach BOOST_FOREACH
#include <numeric>
//---------------------------------------------------------
namespace all { namespace math{
//---------------------------------------------------------
template <typename T>
class moving_average_base_t
{
public:
  ///
  moving_average_base_t(size_t);

  ///actual size (num of elements inside)
  size_t size() const;

  ///filter lenght
  size_t lenght() const;

  ///flushes old
  void reset_lenght(size_t);
  ///
  void set_weights(T weights[]);
  ///
  void set_weights(const std::vector<T>& weights);
  ///
  virtual void push(const T& sample);
  ///
  virtual T mav() = 0;

  ///
  void print_values();
  ///
  void print_weights();

  ///empty the queue
  void flush();

protected:
  ///
  boost::circular_buffer<T> cb_;
  ///
  std::vector<T> weights_;
  ///
  size_t buffer_len_;
};
//---------------------------------------------------------
///
template <typename T>
class simple_moving_average_t : public moving_average_base_t<T>
{
public:
  ///
  simple_moving_average_t(size_t);
  ///
  T mav();
};
//---------------------------------------------------------
///
template <typename T>
class weighted_moving_average_t : public moving_average_base_t<T>
{
public:
  ///
  weighted_moving_average_t(size_t);
  ///
  T mav();
  ///

};
//---------------------------------------------------------
///
template <typename T>
class exp_weighted_moving_average_t : public moving_average_base_t<T>
{
public:
  ///
  exp_weighted_moving_average_t(size_t);
  ///
  T mav();
  ///
  void push(const T& sample);
  ///
  void set_smoothing_factor(double alpha){alpha_=alpha;};

private:
  double alpha_;
  T mav_;
  T prev_mav_;
};
//---------------------------------------------------------
//---------------------------------------------------------
// Implementation
//---------------------------------------------------------
template <typename T>
inline moving_average_base_t<T>::moving_average_base_t(size_t lenght)
  : buffer_len_(lenght)
  , cb_(lenght)
  //, weights_(lenght)
{
  //
  double val = 1.0;///double(lenght);
  //
  T w = static_cast<T>(val);

  //fill with uniform weights
  for(int i = 0; i < lenght; i++)
  {
    weights_.push_back(w);
  }
}
//---------------------------------------------------------
template <typename T>
inline size_t moving_average_base_t<T>::size() const
{
  return cb_.size();
}
//---------------------------------------------------------
template <typename T>
inline size_t moving_average_base_t<T>::lenght() const
{
  return static_cast<size_t> (cb_.capacity());
}
//---------------------------------------------------------
template <typename T>
inline void moving_average_base_t<T>::reset_lenght(size_t lenght)
{
  //cb_.clear();
  //typedef boost::circular_buffer<T>::capacity_type capacity_t;
  //capacity_t newcapacity = static_cast<capacity_t> (lenght);
  //cb_.set_capacity(newcapacity);
  //buffer_len_ = lenght;
}
//---------------------------------------------------------
template <typename T>
inline void moving_average_base_t<T>::set_weights(T weights[])
{
  foreach( T val, weights)
  {
    weights_.push_back(val);
  }
}
//---------------------------------------------------------
template <typename T>
inline void moving_average_base_t<T>::set_weights(const std::vector<T>& weights)
{
  foreach(T val, weights)
  {
    weights_.push_back(val);
  }
}
//---------------------------------------------------------
template <typename T>
inline void moving_average_base_t<T>::push(const T& sample)
{
  cb_.push_back(sample);
}
//---------------------------------------------------------
template <typename T>
inline void moving_average_base_t<T>::print_values()
{
  printf("VALUES: ");
  foreach(T val, cb_)
  {
    printf("% 6.2f", val);
  }
  printf("\n");

}
//---------------------------------------------------------
template <typename T>
inline void moving_average_base_t<T>::print_weights()
{
  printf("WEIGHTS: ");
  foreach(T w, weights_)
  {
    printf("% 6.2f", w);
  }
  printf("\n");
}
//---------------------------------------------------------
template <typename T>
inline void moving_average_base_t<T>::flush()
{
}
////---------------------------------------------------------
////---------------------------------------------------------
// UNIFORM MOVING AVERAGE
//---------------------------------------------------------
template <typename T>
inline simple_moving_average_t<T>::simple_moving_average_t(size_t lenght):moving_average_base_t(lenght)
{
}
////---------------------------------------------------------
template <typename T>
inline T simple_moving_average_t<T>::mav()
{
  //
  T result;
  ////
  //printf("Calculating MVA\n");
  //printf("CB::size(): %d ", cb_.size());

  ////
  for( size_t ind = 0; ind < cb_.size(); ind++ )
  {
    result += cb_[ind];
  }

  //printf("acc: %6.2f ", result );
  //
  result /= cb_.size();
  return result;
}
////-------------------------------------------------------
// EXPONENTIAL  MOVING AVERAGE
//---------------------------------------------------------
  ///
template <typename T>
inline exp_weighted_moving_average_t<T>::exp_weighted_moving_average_t(size_t lenght)
        :moving_average_base_t(lenght)
        ,prev_mav_(0)
{
  alpha_ = (2.0/static_cast<double>(lenght+1));
}
//---------------------------------------------------------
template <typename T>
inline void exp_weighted_moving_average_t<T>::push(const T& val)
{
  cb_.push_back(val);
  mav_ = prev_mav_ + (alpha_*(val-prev_mav_));
  prev_mav_ = mav_;
}
//---------------------------------------------------------
template <typename T>
inline T exp_weighted_moving_average_t<T>::mav()
{
return mav_;
}
//---------------------------------------------------------
}}//all::math
//---------------------------------------------------------
#endif //moving_average_t_HPP_INCLUDED