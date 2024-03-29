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
//---------------------------------------------------------
  //CTOR
//---------------------------------------------------------
  ///ctor
  template <typename T>
  quaternion_t<T>::quaternion_t()
    :quat_(ublas::zero_vector<T>(QSIZE))
    ,axis_(quat_, ublas::range(1,QSIZE))
  {

  };
//---------------------------------------------------------
  ///ctor 
  template <typename T>
  quaternion_t<T>::quaternion_t( const ublas::bounded_vector<T,QSIZE>& src )
    :quat_(src)
    ,axis_(quat_, ublas::range(1,QSIZE))
  {
    normalize();
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
  quaternion_t<T>::quaternion_t( const quaternion_t<T>& src  )
    :axis_(quat_, ublas::range(1,QSIZE))
  {    
    (*this)= src;
    normalize();
  };
//---------------------------------------------------------
  ///ctor
  template <typename T>
  quaternion_t<T>::quaternion_t( const std::vector<T>& src  )
    :axis_(quat_, ublas::range(1,QSIZE))
  {    
    assign(src(0),src(1),src(2), src(3));
    normalize();
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
  quaternion_t<T>::quaternion_t(const axis_type& ax, T aa)
    :axis_(quat_, ublas::range(1,QSIZE))
  {
    from_axis_and_angle(ax, aa);
  }
//---------------------------------------------------------
  template <typename T>
  void quaternion_t<T>::make_identity()
  {
    assign(1.0, 0,0,0);
    normalize();
  }
//---------------------------------------------------------
  template <typename T>
  void quaternion_t<T>::assign(T vw, T vx, T vy, T vz )
  {
    FILL_UVECT4(quat_, vw, vx, vy, vz);
    normalize();
  }
//---------------------------------------------------------
  template <typename T>
  quaternion_t<T>& quaternion_t<T>::operator =(const quaternion_t<T>& src )
  {
    assign(src.w(),src.x(), src.y(),src.z());
    return (*this);
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
  quaternion_t<T>& quaternion_t<T>::operator *= (const T &rhs)
  {
    quat_*= rhs;
    return *this;
  }
//---------------------------------------------------------
  template <typename T>
  quaternion_t<T>& quaternion_t<T>::operator *= (const quaternion_t<T>& rhs)
  {
    T W  = (rhs.w() * w() ) - (rhs.x() * x()) - (rhs.y() * y()) - (rhs.z() * z());
    T X  = (rhs.w() * x() ) + (rhs.x() * w()) + (rhs.y() * z()) - (rhs.z() * y());
    T Y  = (rhs.w() * y() ) + (rhs.y() * w()) + (rhs.z() * x()) - (rhs.x() * z());
    T Z  = (rhs.w() * z() ) + (rhs.z() * w()) + (rhs.x() * y()) - (rhs.y() * x());

    assign(W,X,Y,Z);
    return (*this);
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
  template <typename T>
  quaternion_t<T> quaternion_t<T>::operator*(const quaternion_t<T>& rhs) const
  {
    quaternion_t<T> tmp;

    T W  = (rhs.w() * w() ) - (rhs.x() * x()) - (rhs.y() * y()) - (rhs.z() * z());
    T X  = (rhs.w() * x() ) + (rhs.x() * w()) + (rhs.y() * z()) - (rhs.z() * y());
    T Y  = (rhs.w() * y() ) + (rhs.y() * w()) + (rhs.z() * x()) - (rhs.x() * z());
    T Z  = (rhs.w() * z() ) + (rhs.z() * w()) + (rhs.x() * y()) - (rhs.y() * x());

    tmp.assign(W,X,Y,Z);
    return tmp;
  }
//---------------------------------------------------------
  //template <typename T>
  //ublas::bounded_vector<T, quaternion_t<T>::AXSIZE>  
	 // quaternion_t<T>::operator*(const ublas::bounded_vector<T, quaternion_t<T>::AXSIZE>& v) const
  //{
  ////  // nVidia SDK implementation

  //  ublas::bounded_vector<T, quaternion_t<T>::AXSIZE > uv;
  //  ublas::bounded_vector<T, quaternion_t<T>::AXSIZE > uuv;

  //  uv  = cross_product<T>(axis_,  v);
  //  uuv = cross_product<T>(axis_, uv);
  //  uv *= (2.0 * quat_(eW));
  //  uuv *= 2.0;

  //  return v + uv + uuv;
  //}
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
  template <typename T>
      // Creates a matrix from this quaternion
  void quaternion_t<T>::to_rotation_matrix( rotation_matrix_type &dest ) const
  {
    //
    T q00 = quat_(eW)*quat_(eW); 
    T q0x = quat_(eW)*quat_(eX); 
    T q0y = quat_(eW)*quat_(eY); 
    T q0z = quat_(eW)*quat_(eZ);
    T qxx = quat_(eX)*quat_(eX);
    T qxy = quat_(eX)*quat_(eY);
    T qxz = quat_(eX)*quat_(eZ);
    T qyy = quat_(eY)*quat_(eY); 
    T qyz = quat_(eY)*quat_(eZ);
    T qzz = quat_(eZ)*quat_(eZ);

    //fill in. there should be better ways!
    // first row
    dest(0,0) = q00 + qxx - qyy - qzz;
    dest(0,1) = 2*(qxy - q0z);
    dest(0,2) = 2*(qxz + q0y);
    // second row
    dest(1,0) = 2*(qxy + q0z);
    dest(1,1) = q00 - qxx + qyy - qzz;
    dest(1,2) = 2*(qyz - q0x);
    //third row
    dest(2,0) = 2*(qxz - q0y);
    dest(2,1) = 2*(qyz + q0x);
    dest(2,2) = q00 - qxx - qyy + qzz;
    
//
//R = [ q00 + qxx - qyy - qzz       2*(qxy - q0z)	          2*(qxz + q0y)   
//          2*(qxy + q0z)	      q00 - qxx + qyy - qzz       2*(qyz - q0x)
//          2*(qxz - q0y)	          2*(qyz + q0x)	      q00 - qxx - qyy + qzz ];
  }
//---------------------------------------------------------
  template <typename T>
  void quaternion_t<T>::rotate(vect3_type& vect) const
  {    
    T v1 = vect(0);
    T v2 = vect(1);
    T v3 = vect(2);
    //
    vect3_type temp(vect);
    //
    T t2 =   quat_(eW)*quat_(eX);
    T t3 =   quat_(eW)*quat_(eY);
    T t4 =   quat_(eW)*quat_(eZ);
    T t5 =  -quat_(eX)*quat_(eY);
    T t6 =   quat_(eX)*quat_(eY);
    T t7 =   quat_(eX)*quat_(eZ);
    T t8 =  -quat_(eY)*quat_(eY);
    T t9 =   quat_(eY)*quat_(eZ);
    T t10 = -quat_(eZ)*quat_(eZ);
    //
    vect(0) = 2*( (t8 + t10)* v1 + (t6 -  t4)*v2 + (t3 + t7)*v3 ) + v1;
    vect(1) = 2*( (t4 +  t6)* v1 + (t5 + t10)*v2 + (t9 - t2)*v3 ) + v2;
    vect(2) = 2*( (t7 -  t3)* v1 + (t2 +  t9)*v2 + (t5 + t8)*v3 ) + v3;
  }
//---------------------------------------------------------
  template <typename T>
  void quaternion_t<T>::rotate(T vect[]) const
  {
    T v1 = vect[0];
    T v2 = vect[1];
    T v3 = vect[2];
    //
    T t2 =   quat_(eW)*quat_(eX);
    T t3 =   quat_(eW)*quat_(eY);
    T t4 =   quat_(eW)*quat_(eZ);
    T t5 =  -quat_(eX)*quat_(eY);
    T t6 =   quat_(eX)*quat_(eY);
    T t7 =   quat_(eX)*quat_(eZ);
    T t8 =  -quat_(eY)*quat_(eY);
    T t9 =   quat_(eY)*quat_(eZ);
    T t10 = -quat_(eZ)*quat_(eZ);
    //
    vect(0) = 2*( (t8 + t10)* v1 + (t6 -  t4)*v2 + (t3 + t7)*v3 ) + v1;
    vect(1) = 2*( (t4 +  t6)* v1 + (t5 + t10)*v2 + (t9 - t2)*v3 ) + v2;
    vect(2) = 2*( (t7 -  t3)* v1 + (t2 +  t9)*v2 + (t5 + t8)*v3 ) + v3;
  }
//---------------------------------------------------------
  template <typename T>
  void quaternion_t<T>::to_euler(vect3_type& euler) const
  {
	  T sqw = w()*w();
	  T sqx = x()*x();
	  T sqy = y()*y();
	  T sqz = z()*z();

	  //// heading = rotation about z-axis
    euler(2) = (T) (std::atan2(2.0 * (x()*y() +z()*w()),(sqx - sqy - sqz + sqw)));

	  //// bank = rotation about x-axis
	  //euler.X = (f32) (atan2(2.0 * (Y*Z +X*W),(-sqx - sqy + sqz + sqw)));

	  //// attitude = rotation about y-axis
	  //euler.Y = (f32) (asin(-2.0 * (X*Z - Y*W)));
  }
  //---------------------------------------------------------
  template <typename T>
  void quaternion_t<T>::from_axis_and_angle(const axis_type& axis, T ang) 
  {
	  T sinAngleOver2 = std::sin(ang/2.0);
    quat_(eW) = std::cos(ang/2);
	  quat_(eX) = axis(0)*sinAngleOver2;
	  quat_(eY) = axis(1)*sinAngleOver2;
	  quat_(eZ) = axis(2)*sinAngleOver2;
    normalize();
  }
////---------------------------------------------------------
  template <typename T>
  void quaternion_t<T>::from_euler(const vect3_type& euler) 
  {
	//f64 angle;

	//angle = x * 0.5;
	//f64 sr = (f32)sin(angle);
	//f64 cr = (f32)cos(angle);

	//angle = y * 0.5;
	//f64 sp = (f32)sin(angle);
	//f64 cp = (f32)cos(angle);

	//angle = z * 0.5;
	//f64 sy = (f32)sin(angle);
	//f64 cy = (f32)cos(angle);

	//f64 cpcy = cp * cy;
	//f64 spcy = sp * cy;
	//f64 cpsy = cp * sy;
	//f64 spsy = sp * sy;

	//X = (f32)(sr * cpcy - cr * spsy);
	//Y = (f32)(cr * spcy + sr * cpsy);
	//Z = (f32)(cr * cpsy - sr * spcy);
	//W = (f32)(cr * cpcy + sr * spsy);

  }
//---------------------------------------------------------
}}//all::math
//---------------------------------------------------------

