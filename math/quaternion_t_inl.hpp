#include "quaternion_t.hpp"
#include <cmath>
//---------------------------------------------------------
namespace all { namespace math {
//---------------------------------------------------------
  // static member
  template<typename T>
  const quaternion_t<T> quaternion_t<T>::ZERO     = quaternion_t<T>( 0, 0, 0, 0 );
  // static member
  template<typename T>
  const quaternion_t<T> quaternion_t<T>::IDENTITY = quaternion_t<T>( 1, 0, 0, 0 );
    // static member
  template<typename T>
  const quaternion_t<T> quaternion_t<T>::UNIT_X     = quaternion_t<T>( 0, 1, 0, 0 );
    // static member
  template<typename T>
  const quaternion_t<T> quaternion_t<T>::UNIT_Y     = quaternion_t<T>( 0, 0, 1, 0 );
    // static member
  template<typename T>
  const quaternion_t<T> quaternion_t<T>::UNIT_Z     = quaternion_t<T>( 0, 0, 0, 1 );
//---------------------------------------------------------
  //CTOR
//---------------------------------------------------------
  ///ctor
  template <typename T>
  quaternion_t<T>::quaternion_t()
    :axis_(quat_, ublas::range(1,QSIZE))
  {
    //
    quat_.assign(ublas::zero_vector<T>(QSIZE));
  };
//---------------------------------------------------------
  ///ctor 
  template <typename T>
  quaternion_t<T>::quaternion_t( const ublas::vector<T>& src )
    :axis_(quat_, ublas::range(1,QSIZE))
  {
    quat_.assign(src);
  };
//---------------------------------------------------------
  ///ctor
  template <typename T>
  quaternion_t<T>::quaternion_t(  T src[QSIZE] )
    :axis_(quat_, ublas::range(1,QSIZE))
  {    
    assign( src[ 0 ], src[ 1 ], src[ 2 ], src[ 3 ] );
  };
//---------------------------------------------------------
  ///ctor
  template <typename T>
  quaternion_t<T>::quaternion_t( const quaternion_t& src  )
    :axis_(quat_, ublas::range(1,QSIZE))
  {    
    quat_= src;
  };
//---------------------------------------------------------
  ///ctor
  template <typename T>
  quaternion_t<T>::quaternion_t( const std::vector<T>& src  )
    :axis_(quat_, ublas::range(1,QSIZE))
  {    
    assign(src(0),src(1),src(2), src(3));
  };
//---------------------------------------------------------
  ///ctor
  template <typename T>
  quaternion_t<T>::quaternion_t( T vw, T vx, T vy, T vz )
    :axis_(quat_, ublas::range(1,QSIZE))
  {    
    assign(vw, vx, vy, vz );
  };
//--------------------------------------------------------- 
  ///scalar and vector comp   quaternion_t( double s, const vector3d& v );
  template <typename T>
  quaternion_t<T>::quaternion_t(T scalar, const ublas::vector<T>& vv)
    :axis_(quat_, ublas::range(1,QSIZE))
  {
    assign(scalar, vv(0),vv(1),vv(2));
  }
//---------------------------------------------------------
//---------------------------------------------------------
  template <typename T>
  void quaternion_t<T>::assign(T vw, T vx, T vy, T vz )
  {
    quat_(eW) = vw;
    quat_(eX) = vx;
    quat_(eY) = vy;
    quat_(eZ) = vz;
  }
//---------------------------------------------------------
  template <typename T>
  quaternion_t<T>& quaternion_t<T>::operator =(const quaternion_t<T>& src )
  {
    assign(src.w(),src.x(), src.y(),src.z());
    return *this;
  }
//---------------------------------------------------------
  template <typename T>
  quaternion_t<T> quaternion_t<T>::operator / (const T &rhs) const
  {
    return quaternion_t<T>(quat_(eW)/rhs, quat_(eX)/rhs, quat_(eY)/rhs, quat_(eZ)/rhs);
  }
//---------------------------------------------------------
  template <typename T>
  quaternion_t<T> quaternion_t<T>::operator * (const T &rhs) const
  {
    return quaternion_t<T>(quat_(eW)*rhs, quat_(eX)*rhs, quat_(eY)*rhs, quat_(eZ)*rhs);
  }
//---------------------------------------------------------
  template <typename T>
    ///
  quaternion_t<T>& quaternion_t<T>::operator *= (const T &rhs)
  {
    quat_*= rhs;
    return *this;
  }
//---------------------------------------------------------
    ///dot product
  template <typename T>
  T quaternion_t<T>::dot(const quaternion_t<T>& rhs) const
  {
    return  (quat_(eW) * rhs.w()) 
          + (quat_(eX) * rhs.x()) 
          + (quat_(eY) * rhs.y()) 
          + (quat_(eZ) * rhs.z());
  }
//---------------------------------------------------------
  //template <typename T>
  //quaternion_t<T> quaternion_t<T>::operator*(const quaternion_t<T>& other) const
  //{
  //  quaternion_t<T> tmp;

  //  T W  = (other.w() * w() ) - (other.x() * x()) - (other.y() * y()) - (other.z() * z());
  //  T X  = (other.w() * x() ) + (other.x() * w()) + (other.y() * z()) - (other.z() * y());
  //  T Y  = (other.w() * y() ) + (other.y() * w()) + (other.z() * x()) - (other.x() * z());
  //  T Z  = (other.w() * z() ) + (other.z() * w()) + (other.x() * y()) - (other.y() * x());

  //  tmp.assign(W,X,Y,Z);
  //  return tmp;
  //}
//---------------------------------------------------------
  template <typename T>
  ublas::vector<T>  quaternion_t<T>::operator * (const ublas::vector<T>& v) const
  {
    // nVidia SDK implementation

    ublas::bounded_vector<T,3> uv;
    ublas::bounded_vector<T,3> uuv;

    uv  = cross_product<T>(axis_,  v);
    uuv = cross_product<T>(axis_, uv);
    uv *= (2.0 * quat_(eW));
    uuv *= 2.0;

    return v + uv + uuv;
  }
//---------------------------------------------------------
  template <typename T>
  T quaternion_t<T>::norm() const
  {
   return ublas::norm_2 (quat_);
  }
//---------------------------------------------------------
  template <typename T>
  T quaternion_t<T>::norm2() const
  {
    return quat_(eW)*quat_(eW)
          +quat_(eX)*quat_(eX)
          +quat_(eY)*quat_(eY)
          +quat_(eZ)*quat_(eZ);
  }
  //---------------------------------------------------------
  template <typename T>
  void quaternion_t<T>::scale(T nlen)
  {
    quat_*= (nlen/norm());
  }
//---------------------------------------------------------
  template <typename T>
  void quaternion_t<T>::normalize()
  {
    scale(1.0);
  }
//---------------------------------------------------------
  template <typename T>
  void quaternion_t<T>::conjugate()
  {
    assign(quat_(eW), -quat_(eX), -quat_(eY), -quat_(eZ));
  }
//---------------------------------------------------------
  template <typename T>
  quaternion_t<T> quaternion_t<T>::get_conjugate()
  {
    return quaternion_t<T>(quat_(eW), -quat_(eX), -quat_(eY), -quat_(eZ));
  }
//---------------------------------------------------------
  template <typename T>
  void quaternion_t<T>::unit_inverse()	//	assumes we have a unit quaternion
  {
    conjugate();
  }
//---------------------------------------------------------
  template <typename T>
  quaternion_t<T> quaternion_t<T>::get_unit_inverse()	//	assumes we have a unit quaternion
  {
    get_conjugate();
  }
//---------------------------------------------------------
  template <typename T>
  void quaternion_t<T>::inverse()
  {
    normalize();
    conjugate();
  }
//---------------------------------------------------------
  template <typename T>
  quaternion_t<T> quaternion_t<T>::get_inverse()
  {
    normalize();
    return get_conjugate();
  }
//---------------------------------------------------------
}}//all::math
//---------------------------------------------------------