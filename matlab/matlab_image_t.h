#ifndef matlab_image_t_INCLUDED
#define matlab_image_t_INCLUDED

#include <mex.h>
//--------------------------------------------------------------------------+
namespace all{ namespace matlab {
//--------------------------------------------------------------------------+
template <typename T>
class matlab_image_t
{
public:
  // ######################################################################
  /*! @name Constructors and initialization routines
      An Image can be constructed from an existing two-dimensional Matlab
      array, or created empty, or constructed uninitialzed. In this case,
      the Image must be initialized before any operation with it is 
      attempted. The copy constructor and assigment make true memory
      copies of the other image */
  //@{

  //! Contructor for an uninitialized Image.
  matlab_image_t();
  
  //! Constructor from a two-dimensional Matlab array.
  matlab_image_t(const mxArray *arr);
  
  //! Copy constructor.
  matlab_image_t(const matlab_image_t& other);
  
  //! Constructs a new empty Image with width w and height h.
  matlab_image_t(const int h, const int w, const int d);
  
  //! Copy assigment operator.
  matlab_image_t operator=(const matlab_image_t other);
   
  //! Initializes from a two-dimensional Matlab array.
  void initialize(const mxArray *arr);
  
  //! Initializes a new empty image with width w and height h.
  void initialize(const int h, const int w, const int d);
  //@}
  // ######################################################################
  /*! @name Access functions. */
  //@{
  
  //! Returns the width of the Image.
  int get_width() const;
  
  //! Returns the height of the Image.
  int get_height() const;

  //!Returns the depth of the Image
  int get_depth() const;
  
  //! Returns the number of pixels in the Image.
  int get_size() const;
  
  //! Returns whether the Image is initialized.
  bool is_initialized() const;
  
  //! Returns a pointer to the underlying Matlab array.
  /*! This version allows manipulation of the array from the outside.*/
  mxArray* get_array();
  
  //! Returns a constant pointer to the underlying Matlab array.
  /*! This version prevents manipulation of the array from the outside.*/
  const mxArray* get_const_array() const;
  
  ////! Check if the coordinates are within the bounds of the Image.
  //bool coords_ok(int x, int y) const;

  //! Returns a pixel value by index.
  T::value_type get_val(int index) const;
  
  //! Returns a pixel value by coordinates.
  T::value_type get_val(int r, int c, int d) const;
  
  //! Sets a pixel value by index.
  void set_val(int index, T::value_type val);
  
  //! Sets a pixel value by coordinates.
  void set_val(int r, int c, int d, T::value_type val);

  //@}
  // ######################################################################
  // ######################################################################
  /*! @name Computation functions. */
  //@{
  
  ////! Clamp the image between bottom and top.
  ///*! All values less than bottom are set to bottom; all values greater
  //    than top are set to top.*/
  //void clamp(const double bottom, const double top);
  //
  ////! Clamp the image only at the bottom.
  ///*! All values less than bottom are set to bottom.*/
  //void clamp_bottom(const double bottom);
  //
  ////! Clamp the image only at the top.
  ///*! All values greater than top are set to top.*/
  //void clamp_top(const double top);
  
  ////! Count and sum all local maxima above a threshold.
  ///*!@param thresh the threshold for a local maximum.
  //   @param lm_num returns the number of local maxima.
  //   @param lm_sum returns the sum of the local maxima. */
  //void get_local_maxima(const T::value_type thresh, int *lm_num, double *lm_sum);
  
  //! Multiply all values by a scalar factor.
  matlab_image_t operator*=(const double factor);
  
  //@}
  // #####################################################################
  /*! @name Iterators
      There are const and non-const versions of iterators, which are
      returned by begin()/end() and beginw()/endw(), respectively. The "w"
      in beginw()/endw() is a mnemonic for "write" or "writeable".
   */
  //@{
  
  typedef T element_type;

  //! Read/write iterator.
  typedef T::pointer iterator;
  
  //! Read-only iterator.
  typedef const T::const_pointer const_iterator;
  
  //! Returns a read-only iterator to the beginning of the Image data
  const_iterator begin() const;
  
  //! Returns a read-only iterator to the one-past-the-end of the Image data
  const_iterator end() const;
  
  //! Returns a read/write iterator to the beginning of the Image data
  iterator beginw();
  
  //! Returns a read/write iterator to the one-past-the-end of the Image data
  iterator endw();
  
  //@}
  
protected:
  bool      initialized_;
  mxArray*  array_; 
};

#include "detail/matlab_image_inline.hpp"
//--------------------------------------------------------------------------+
}}
//--------------------------------------------------------------------------+

#endif