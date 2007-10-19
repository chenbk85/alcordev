#ifndef i_binary_serializable_HPP_INCLUDED
#define i_binary_serializable_HPP_INCLUDED

#include <iostream>
#include <fstream>

namespace all { namespace core {

template <typename T>
class i_binary_serializable
{
public:
  typedef T value_type;

  //template <typename T>
  virtual void serialize(const T&, std::fstream&){};

  //template <typename T>
  virtual void serialize(const T*, std::fstream&){};

  //template < typename T>
  //virtual T deserialize() = 0;

  //template < typename T>
  virtual void deserialize(T&, std::fstream&){};

  //template < typename T>
  virtual void deserialize(T*, std::fstream&){};

};

}}

#endif //i_binary_serializable_HPP_INCLUDED