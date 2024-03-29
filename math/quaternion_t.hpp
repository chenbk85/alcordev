#ifndef quaternion_t_HPP_INCLUDED
#define quaternion_t_HPP_INCLUDED
//---------------------------------------------------------
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/config.hpp>
//---------------------------------------------------------
#include <vector>
//---------------------------------------------------------
#include <boost/numeric/ublas/io.hpp>
using namespace boost::numeric;
//---------------------------------------------------------
namespace all { namespace math {
//---------------------------------------------------------
  ///
  namespace 
  {
    ///
    typedef double QUATPRECISION;

    ///cross product
    template<typename T > 
    inline ublas::bounded_vector<T,3> 
      cross_product(const ublas::bounded_vector<T,3>& lhs
                  , const ublas::bounded_vector<T,3>& rhs)
    {
      ublas::bounded_vector<T,3> result;

      result( 0 ) = lhs ( 1 ) * rhs ( 2 ) - rhs( 1 ) * lhs ( 2 );
      result( 1 ) = rhs ( 0 ) * lhs ( 2 ) - lhs( 0 ) * rhs ( 2 );
      result( 2 ) = lhs ( 0 ) * rhs ( 1 ) - rhs( 0 ) * lhs ( 1 );

      return result;
    };

    #define FILL_UVECT3(__VECT_, __V1_, __V2_, __V3_)\
      __VECT_(0) = __V1_; \
      __VECT_(1) = __V2_; \
      __VECT_(2) = __V3_;

    #define FILL_UVECT4(__VECT_, __V1_, __V2_, __V3_, __V4_)\
      __VECT_(0) = __V1_; \
      __VECT_(1) = __V2_; \
      __VECT_(2) = __V3_; \
      __VECT_(3) = __V4_; 

    ///
    template < typename T >
    struct op 
    {
    ///degree to radians
    static inline T d2r(T val)   
    {
    return static_cast<T>(val * 3.1415926535897932384626433832795/180.0);
    };
    ///radians to degree
    static inline T r2d(T val)  {
    return static_cast<T>(val * 180.0/3.1415926535897932384626433832795);
	//return static_cast<T>(val * 57.2958 );
    };

    };
    //
    typedef op<double> op64f;
  }
//---------------------------------------------------------
  ///
  template <typename T = QUATPRECISION>
  class quaternion_t 
  {
    BOOST_STATIC_CONSTANT(size_t, AXSIZE = 3);
    BOOST_STATIC_CONSTANT(size_t, QSIZE = 4);

  public: //TRAITS
    ///
    typedef T value_type;
    ///
    typedef ublas::bounded_vector<T,QSIZE> quat_type;
    ///
    typedef ublas::bounded_vector<T,AXSIZE> vect3_type;
    ///
    typedef ublas::bounded_vector<T,AXSIZE> axis_type;
    ///
    typedef ublas::bounded_matrix<T,AXSIZE,AXSIZE> rotation_matrix_type;
    ///
    static const quaternion_t<T> ZERO;
    ///
    static const quaternion_t<T> IDENTITY;

    ///indices
    enum qcomp_index_t
    { 
      //this way
      eW = 0,
      eX = 1,
      eY = 2,
      eZ = 3,
    } ;

  public://CTOR
    ///ctor default
    quaternion_t();
    ///ctor 4 tap ublas::vector (w x y z)
    quaternion_t( const quat_type& src );
    ///ctor 4 tap std::vector (w x y z)
    quaternion_t( const std::vector<T>& src  );
    ///ctor 4 tap simple c array (w x y z)
    quaternion_t(  T src[QSIZE] );
    ///copy ctor 
    quaternion_t( const quaternion_t& src  );
    ///ctor
    quaternion_t( T vw, T vx, T vy, T vz );
    ///ctor axis and angle
    quaternion_t(const axis_type& ax, T aa);

  public://assign + math operators
    ///
    void make_identity();
    ///
    void assign(T vw, T vx, T vy, T vz );
    ///copy operator UHM
    quaternion_t<T>& operator =(const quaternion_t<T>& src );
    ///
    quaternion_t<T> operator / (const T &rhs) const;
    ///
    quaternion_t<T> operator * (const T &rhs) const;
    ///
    quaternion_t<T>& operator *= (const T &rhs);
    ///
    quaternion_t<T>& operator *= (const quaternion_t<T>& rhs);

    ///dot product
    T dot(const quaternion_t<T>& rhs) const;
    ///operator
    quaternion_t<T> operator*(const quaternion_t<T>& rhs) const ;
    /////
    //ublas::bounded_vector<T, quaternion_t<T>::AXSIZE >  
    //  operator* (const ublas::bounded_vector<T, quaternion_t<T>::AXSIZE>& v) const;

  public: //ACCESSORS
    ///W
    T w()     const {return quat_(eW);};
    ///X
    T x()     const {return quat_(eX);};
    ///Y
    T y()     const {return quat_(eY);};
    ///Z
    T z()     const {return quat_(eZ);};
    ///W
    T W()     const {return quat_(eW);};
    ///X
    T X()     const {return quat_(eX);};
    ///Y
    T Y()     const {return quat_(eY);};
    ///Z
    T Z()     const {return quat_(eZ);};

    ///same as above
    T scalar() const{return quat_(eW);};
    ///get current axis
    axis_type 
      axis() const{return axis_;};
    ///get whole quaternion
    quat_type 
      quat() const {return quat_;};

  //op
  public:
    ///normalize
    void normalize();
    ///
    void scale(T newlen);
    ///
    T norm() const;
    ///squared norm
    T norm2() const;
    ///
    void conjugate();

  public://INVERSE
    ///
    void unit_inverse();	//	assumes we have a unit quaternion    
    ///
    void inverse();    
    ///
    quaternion_t<T> get_conjugate();
    ///
    quaternion_t<T> get_unit_inverse();	//	assumes we have a unit quaternion
    ///
    quaternion_t<T> get_inverse();

  public://ROTATION
    // Creates a matrix from this quaternion
    void to_rotation_matrix( rotation_matrix_type &dest ) const;
    ///rotate vector
    void rotate(vect3_type& vect) const;
    ///rotate vector overload
    void rotate(T vect[3])  const;

    ///EULERO
    void to_euler(vect3_type& euler)  const;
    ///
    void from_axis_and_angle(const axis_type& axis, T vv);
    ///
    void from_euler(const vect3_type& euler);

  private:    
    ///the whole quaternion
    ublas::bounded_vector<T, QSIZE> quat_;
    ///proxy representation of the axis
    ublas::vector_range<ublas::bounded_vector<T, QSIZE> > axis_;
  };

//---------------------------------------------------------
  //
  typedef all::math::quaternion_t<double> quat64_t;
  typedef all::math::quaternion_t<float>  quat32_t;
//---------------------------------------------------------

//---------------------------------------------------------
}}//all::math
//---------------------------------------------------------
#include "alcor/math/quaternion_t_inl.hpp"
//---------------------------------------------------------
#endif //quaternion_t_HPP_INCLUDED