#ifndef matlab_tags_HPP_INCLUDED
#define matlab_tags_HPP_INCLUDED
//-----------------------------------------------------------------------++
#include "matrix.h"  
#include <boost/config.hpp>
#include "alcor/core/core.h"
//-----------------------------------------------------------------------++
//Creation Tags
namespace all { 
	namespace matlab {

//---------------------------------------------------------------------------+
template <class T>
class traits;
//-------------------------------------------------------------------------++
template< >
class traits<all::core::uint8_t>
{
public:
  typedef all::core::uint8_t value_type;
  typedef value_type* ptr;
  typedef const ptr const_ptr;
  BOOST_STATIC_CONSTANT(int, size=sizeof(value_type));
  BOOST_STATIC_CONSTANT(mxClassID, tag = mxUINT8_CLASS);
};
//-------------------------------------------------------------------------++
template< >
class traits<all::core::int8_t>
{
public:
  typedef all::core::int8_t value_type;
  typedef value_type* ptr;
  typedef const ptr const_ptr;
  BOOST_STATIC_CONSTANT(int, size=sizeof(value_type));
  BOOST_STATIC_CONSTANT(mxClassID, tag = mxINT8_CLASS);
};
//-------------------------------------------------------------------------++
template< >
class traits<all::core::single_t>
{
public:
  typedef all::core::single_t value_type;
  typedef all::core::traits<all::core::single_t>::ptr ptr;
  typedef const ptr const_ptr;
  BOOST_STATIC_CONSTANT(int, size=sizeof(value_type));
  BOOST_STATIC_CONSTANT(mxClassID, tag = mxSINGLE_CLASS);
};

////#########################################################################
//namespace detail
//		{
//
////#########################################################################
//		struct uint8_t
//            {
//            typedef all::core::uint8_t value_type;
//            typedef value_type* pointer;
//            typedef const pointer const_pointer;
//            BOOST_STATIC_CONSTANT(int, size=sizeof(value_type));
//			BOOST_STATIC_CONSTANT(mxClassID, tag = mxUINT8_CLASS);
//			};
//
//		struct int8_t{
//			typedef signed char value_type; 
//            typedef value_type* pointer;
//            typedef const pointer const_pointer;
//            BOOST_STATIC_CONSTANT(int, size=sizeof(value_type));
//			BOOST_STATIC_CONSTANT(mxClassID, tag = mxINT8_CLASS);
//			};
//
//		struct int16_t{
//			typedef short value_type;
//            typedef value_type* pointer;
//            typedef const pointer const_pointer;
//            BOOST_STATIC_CONSTANT(int, size=sizeof(value_type));
//			BOOST_STATIC_CONSTANT(mxClassID, tag = mxINT16_CLASS);
//			};
//
//		struct int32_t{
//			typedef int value_type; 
//            typedef value_type* pointer;
//            typedef const pointer const_pointer;
//            BOOST_STATIC_CONSTANT(int, size=sizeof(value_type));
//			BOOST_STATIC_CONSTANT(mxClassID, tag = mxINT32_CLASS);
//			};
//
//		struct int64_t{
//			typedef long value_type; 
//            typedef value_type* pointer;
//            typedef const pointer const_pointer;
//            BOOST_STATIC_CONSTANT(int, size=sizeof(value_type));
//			BOOST_STATIC_CONSTANT(mxClassID, tag = mxINT64_CLASS);
//			};
//
//		struct single_t{
//			typedef float value_type;
//            typedef value_type* pointer;
//            typedef const pointer const_pointer;
//            BOOST_STATIC_CONSTANT(int, size=sizeof(value_type));
//			BOOST_STATIC_CONSTANT(mxClassID, tag = mxSINGLE_CLASS);
//			};
//
//		struct double_t{
//			typedef double value_type; 
//            typedef value_type* pointer;
//            typedef const pointer const_pointer;
//            BOOST_STATIC_CONSTANT(int, size=sizeof(value_type));
//			BOOST_STATIC_CONSTANT(mxClassID, tag = mxDOUBLE_CLASS);
//			};
//
		struct row_major_t{};
		struct column_major_t{};
//		}//detail
//-----------------------------------------------------------------------++
//#define _GENERATE_MX_TAG_(_MXTYPE_, _MXTAG_)\
//static const _MXTYPE_ _MXTAG_ = _MXTYPE_();
////
//_GENERATE_MX_TAG_(matlab::detail::uint8_t, uint8_tag)
//_GENERATE_MX_TAG_(matlab::detail::int8_t,  int8_tag)
//_GENERATE_MX_TAG_(matlab::detail::int16_t, mxInt16)
//_GENERATE_MX_TAG_(matlab::detail::int32_t, mxInt32)
//_GENERATE_MX_TAG_(matlab::detail::single_t, mxSingle)
//_GENERATE_MX_TAG_(matlab::detail::double_t, mxDouble)
//-----------------------------------------------------------------------++
static const all::matlab::row_major_t row_major = 
		all::matlab::row_major_t();
static const all::matlab::column_major_t column_major= 
		all::matlab::column_major_t();
//-----------------------------------------------------------------------++
	}}//namespaces
//-----------------------------------------------------------------------++
//typedef enum {
//	mxUNKNOWN_CLASS = 0,
//	mxCELL_CLASS,
//	mxSTRUCT_CLASS,
//	mxLOGICAL_CLASS,
//	mxCHAR_CLASS,
//	mxVOID_CLASS,
//	mxDOUBLE_CLASS,
//	mxSINGLE_CLASS,
//	mxINT8_CLASS,
//	mxUINT8_CLASS,
//	mxINT16_CLASS,
//	mxUINT16_CLASS,
//	mxINT32_CLASS,
//	mxUINT32_CLASS,
//	mxINT64_CLASS,
//	mxUINT64_CLASS,
//	mxFUNCTION_CLASS,
//        mxOPAQUE_CLASS,
//	mxOBJECT_CLASS
//} mxClassID;
#endif //matlab_tags_HPP_INCLUDED