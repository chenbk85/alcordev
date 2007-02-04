#ifndef ipc_serializable_image_HPP_INCLUDED
#define ipc_serializable_image_HPP_INCLUDED
//------------------------------------------------------------++
#include <boost/config.hpp>
//-----------------------------------------------------------------------++
#include <boost/call_traits.hpp>
//------------------------------------------------------------++
#include <vector>
//------------------------------------------------------------++
#include "alcor/core/core.h"
//------------------------------------------------------------++
namespace all { namespace core {
//------------------------------------------------------------++
///
template  < typename T,int _height, int _width, int _channels>
class ipc_serializable_image_t
    {
	public:

	///
	typedef typename boost::call_traits<T>::value_type  value_type;
	///
	typedef typename boost::call_traits<T>::param_type  param_type;
	///
	typedef typename boost::call_traits<T>::reference	reference;
	///
	typedef typename boost::call_traits<T>::const_reference  const_reference;

	///
    typedef ipc_serializable_image_t<value_type,_height, _width, _channels>  
        self_type;

    ///
    BOOST_STATIC_CONSTANT( int, pixel_size = sizeof(T));
    ///
    BOOST_STATIC_CONSTANT( int, planar_inc = _width*_height);
    ///
    BOOST_STATIC_CONSTANT( int, byte_lenght = _width*_height*_channels*sizeof(T));

    ///
    enum {  height = _height,
            width = _width, 
            channels = _channels};
    ///
    T data[_height*_width*_channels];
    };
//------------------------------------------------------------++
/////
typedef  ipc_serializable_image_t<all::core::single_t, 480, 640, 3> float_image_640_t;
/////
typedef ipc_serializable_image_t<all::core::single_t, 240, 320, 3 > float_image_320_t;
/////
typedef  ipc_serializable_image_t<all::core::single_t,240, 320, 1> float_image_320_mono_t;
//
typedef  ipc_serializable_image_t<all::core::uint8_t, 240, 320, 3> uint8_image_320_t;
//
typedef  ipc_serializable_image_t<all::core::uint8_t, 240, 320, 3> uint8_image_320_t;
//
typedef  ipc_serializable_image_t<all::core::uint8_t, 240, 320, 3> byte_image_320_t;
///
typedef  ipc_serializable_image_t<all::core::uint8_t,480, 640, 3> uint8_image_640_t;
//------------------------------------------------------------++
//------------------------------------------------------------++
    }}///namespaces
//------------------------------------------------------------++
#endif //ipc_serializable_image_HPP_INCLUDED

