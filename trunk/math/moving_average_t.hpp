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
template <typename DATATYPE, typename RESULTTYPE>
class moving_average_base_t
{
public:
  ///
  moving_average_base_t(size_t);

  ///
  typedef DATATYPE   value_type;
  typedef RESULTTYPE result_type;

  ///actual size (num of elements inside)
  size_t size() const;

  ///filter lenght
  size_t lenght() const;

  ///flushes old
  void reset_lenght(size_t);
  ///
  void set_weights(DATATYPE weights[]);
  ///
  void set_weights(const std::vector<DATATYPE>& weights);
  ///
  virtual void push(const DATATYPE& sample);
  ///
  virtual RESULTTYPE mav() = 0;

  ///
  void print_values();
  ///
  void print_weights();

  ///empty the queue
  void flush();

protected:
  ///
  boost::circular_buffer<DATATYPE> cb_;
  ///
  std::vector<DATATYPE> weights_;
  ///
  size_t buffer_len_;
  ///
  RESULTTYPE mav_;
  ///
  RESULTTYPE prev_mav_;

};
//---------------------------------------------------------
///
template <typename DATATYPE, typename RESULTTYPE>
class simple_moving_average_t : public moving_average_base_t<DATATYPE, RESULTTYPE>
{
public:
  ///
  simple_moving_average_t(size_t);
  ///
  virtual void push(const DATATYPE& sample);
  ///
  RESULTTYPE mav();
};
//---------------------------------------------------------
///
template <typename DATATYPE, typename RESULTTYPE>
class weighted_moving_average_t : public moving_average_base_t<DATATYPE, RESULTTYPE>
{
public:
  ///
  weighted_moving_average_t(size_t);
  ///
  virtual void push(const DATATYPE& sample);
  ///
  RESULTTYPE mav();
};
//---------------------------------------------------------
///
template <typename DATATYPE, typename RESULTTYPE>
class exp_weighted_moving_average_t : public moving_average_base_t<DATATYPE, RESULTTYPE>
{
public:
  ///
  exp_weighted_moving_average_t(size_t);
  ///
  RESULTTYPE mav();
  ///
  virtual void push(const DATATYPE& sample);
  ///
  void set_smoothing_factor(double alpha){alpha_=alpha;};

private:
  double alpha_;
};
//---------------------------------------------------------
//---------------------------------------------------------
// Implementation
//---------------------------------------------------------
template <typename DATATYPE, typename RESULTTYPE>
inline moving_average_base_t<DATATYPE, RESULTTYPE>::moving_average_base_t(size_t lenght)
  : buffer_len_(lenght)
  , cb_(lenght)
  , mav_(0)
  , prev_mav_(0)
{

}
//---------------------------------------------------------
template <typename DATATYPE, typename RESULTTYPE>
inline size_t moving_average_base_t<DATATYPE, RESULTTYPE>::size() const
{
  return cb_.size();
}
//---------------------------------------------------------
template <typename DATATYPE, typename RESULTTYPE>
inline size_t moving_average_base_t<DATATYPE, RESULTTYPE>::lenght() const
{
  return static_cast<size_t> (cb_.capacity());
}
//---------------------------------------------------------
template <typename DATATYPE, typename RESULTTYPE>
inline void moving_average_base_t<DATATYPE, RESULTTYPE>::reset_lenght(size_t lenght)
{

}
//---------------------------------------------------------
template <typename DATATYPE, typename RESULTTYPE>
inline void moving_average_base_t<DATATYPE, RESULTTYPE>::set_weights(DATATYPE weights[])
{
  foreach( DATATYPE val, weights)
  {
    weights_.push_back(val);
  }
}
//---------------------------------------------------------
template <typename DATATYPE, typename RESULTTYPE>
inline void moving_average_base_t<DATATYPE, RESULTTYPE>::set_weights(const std::vector<DATATYPE>& weights)
{
  foreach(DATATYPE val, weights)
  {
    weights_.push_back(val);
  }
}
//---------------------------------------------------------
template <typename DATATYPE, typename RESULTTYPE>
inline void moving_average_base_t<DATATYPE, RESULTTYPE>::push(const DATATYPE& sample)
{
  cb_.push_back(sample);
}
//---------------------------------------------------------
template <typename DATATYPE, typename RESULTTYPE>
inline void moving_average_base_t<DATATYPE, RESULTTYPE>::print_values()
{
  printf("VALUES: ");
  //TODO: use iostream instead
  foreach(DATATYPE val, cb_)
  {
    printf("% 6.2f", val);
  }
  printf("\n");

}
//---------------------------------------------------------
template <typename DATATYPE, typename RESULTTYPE>
inline void moving_average_base_t<DATATYPE, RESULTTYPE>::print_weights()
{
  printf("WEIGHTS: ");
  foreach(DATATYPE w, weights_)
  {
    printf("% 6.2f", w);
  }
  printf("\n");
}
//---------------------------------------------------------
template <typename DATATYPE, typename RESULTTYPE>
inline void moving_average_base_t<DATATYPE, RESULTTYPE>::flush()
{
}
////---------------------------------------------------------
////---------------------------------------------------------
// UNIFORM MOVING AVERAGE
//---------------------------------------------------------
template <typename DATATYPE, typename RESULTTYPE>
inline simple_moving_average_t<DATATYPE, RESULTTYPE>::simple_moving_average_t(size_t lenght):moving_average_base_t(lenght)
{
}
////---------------------------------------------------------
template <typename DATATYPE, typename RESULTTYPE>
inline void simple_moving_average_t<DATATYPE, RESULTTYPE>::push(const DATATYPE& val)
{
  //
  DATATYPE accum = DATATYPE();
  //
  moving_average_base_t::push(val);
  //cb_.push_back(val);
  //
  for( size_t ind = 0; ind < cb_.size(); ind++ )
  {
    accum +=  cb_[ind] ;
  }

  //
  mav_ = static_cast<RESULTTYPE> ( accum/cb_.size() );
  //printf("acc: %4.3f -> mav(): %4.3f\n",accum, mav_);
}
////---------------------------------------------------------
template <typename DATATYPE, typename RESULTTYPE>
inline RESULTTYPE simple_moving_average_t<DATATYPE, RESULTTYPE>::mav()
{
  return mav_;
}
////-------------------------------------------------------
// EXPONENTIAL  MOVING AVERAGE
//---------------------------------------------------------
  ///
template <typename DATATYPE, typename RESULTTYPE>
inline exp_weighted_moving_average_t<DATATYPE, RESULTTYPE>::exp_weighted_moving_average_t(size_t lenght)
        :moving_average_base_t(lenght)
{
  alpha_ = (2.0/static_cast<double>(lenght+1));
}
//---------------------------------------------------------
template <typename DATATYPE, typename RESULTTYPE>
inline void exp_weighted_moving_average_t<DATATYPE, RESULTTYPE>::push(const DATATYPE& val)
{
  cb_.push_back(val);
  mav_ = prev_mav_ + (alpha_*(val - prev_mav_));
  prev_mav_ = mav_;
}
//---------------------------------------------------------
template <typename DATATYPE, typename RESULTTYPE>
inline RESULTTYPE exp_weighted_moving_average_t<DATATYPE, RESULTTYPE>::mav()
{
return mav_;
}
//---------------------------------------------------------
}}//all::math
//---------------------------------------------------------
#endif //moving_average_t_HPP_INCLUDED