#ifndef quaternion_t_HPP_INCLUDED
#define quaternion_t_HPP_INCLUDED
//---------------------------------------------------------
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
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
    //
    const int QSIZE  = 4;
    const int AXSIZE = 3;
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

  }
//---------------------------------------------------------
  ///
  template <typename T = QUATPRECISION>
  class quaternion_t 
  {
  public://ctor
    ///ctor default
    quaternion_t();
    ///ctor 4 tap ublas::vector (w x y z)
    quaternion_t( const ublas::vector<T>& src );
    ///ctor 4 tap std::vector (w x y z)
    quaternion_t( const std::vector<T>& src  );
    ///ctor 4 tap simple c array (w x y z)
    quaternion_t(  T src[QSIZE] );
    ///copy ctor 
    quaternion_t( const quaternion_t& src  );
    ///ctor
    quaternion_t( T vw, T vx, T vy, T vz );
    ///scalar and vector comp   quat4d( double s, const vector3d& v );
    quaternion_t(T scalar, const ublas::vector<T>& vv);

  public://assign + math operators
    ///
    void assign(T vw, T vx, T vy, T vz );
    ///copy operator
    quaternion_t<T>& operator =(const quaternion_t<T>& src );
    ///
    quaternion_t<T> operator / (const T &rhs) const;
    ///
    quaternion_t<T> operator * (const T &rhs) const;
    ///
    quaternion_t<T>& operator *= (const T &rhs);
    ///dot product
    T dot(const quaternion_t<T>& rhs) const;
    ///multiplication operator uhm ... vedere!
    //quaternion_t<T> operator*(const quaternion_t<T>& rhs) const;
    ///
    ublas::vector<T>  operator* (const ublas::vector<T>& v) const;


  public:
    ///
    typedef T value_type;
    ///
    static const quaternion_t<T> ZERO;
    ///
    static const quaternion_t<T> IDENTITY;
    ///
    static const quaternion_t<T> UNIT_X;
        ///
    static const quaternion_t<T> UNIT_Y;
        ///
    static const quaternion_t<T> UNIT_Z;

    ///indices
    enum qcomp_index_t
    { 
      //this way
      eW = 0,
      eX = 1,
      eY = 2,
      eZ = 3,
    } ;

  public: //ACCESSORS
    ///generic comp accessor
    T operator() (qcomp_index_t ii) const {return quat_(ii);};

    ///W
    T w()     const {return quat_(eW);};
    ///X
    T x()     const {return quat_(eX);};
    ///Y
    T y()     const {return quat_(eY);};
    ///Z
    T z()     const {return quat_(eZ);};

    ///same as above
    T scalar() const{return quat_(eW);};
    ///get current axis
    ublas::vector<T> 
      axis() const{return ublas::vector<T>(axis_);};

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
    //! Creates a matrix from this quaternion
    void rotation_matrix( ublas::matrix<T> &dest ) const;

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
}}//all::math
//---------------------------------------------------------
#include "alcor/math/quaternion_t_inl.hpp"
//---------------------------------------------------------
#endif //quaternion_t_HPP_INCLUDED