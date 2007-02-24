// ######################################################################
template <typename T>
matlab_image_t<T>::matlab_image_t()
  : initialized_(false)
{}

// ######################################################################
template <typename T>
matlab_image_t<T>::matlab_image_t(const mxArray *arr)
{
  initialize(arr);
}

// ######################################################################
template <typename T>
matlab_image_t<T>::matlab_image_t(const matlab_image_t& other)
{
  initialize(other.get_const_array());
}

// ######################################################################
template <typename T>
matlab_image_t<T>::matlab_image_t(const int h, const int w, const int d)
{
  initialize(w,h,d);
}
    
// ######################################################################
template <typename T>
matlab_image_t matlab_image_t<T>::operator=(const matlab_engine_t other)
{
  initialized_ = other.initialized_();
  if (is_initialized())
    initialize(other.get_const_array());
  return *this;
}

// ######################################################################
template <typename T>
void matlab_image_t<T>::initialize(const mxArray *arr)
{
  const int* dimensions = mxGetDimensions(arr);
  initialize(dimensions[0], dimensions[1], dimensions[2]);
  memcpy(beginw(),mxGetPr(arr), get_size() * T::size);
}

// ######################################################################
template <typename T>
void matlab_image_t<T>::initialize(const int h, const int w, const int d)
{
  //
  initialized_ = true;
  //
  int dims[] = {h, w, d};
  //
  int ndimensions = ( (d > 2 ) ? (3) : (2) );
  mxCreateNumericArray(ndimensions, dims, T::tag, mxREAL);
}
// ######################################################################
template <typename T>
int matlab_image_t<T>::get_width() const
{
  //ASSERT(isInitialized());
  return mxGetN(get_const_array());
}

// ######################################################################
template <typename T>
int matlab_image_t<T>::get_height() const
{
  //ASSERT(isInitialized());
  return mxGetM(get_const_array());
}
// ######################################################################
template <typename T>
int matlab_image_t<T>::get_size() const
{
  //ASSERT(isInitialized());
  return mxGetNumberOfElements(get_const_array());
}
// ######################################################################
template <typename T>
matlab_image_t<T>::const_iterator 
  matlab_image_t<T>::begin() const
{
  return mxGetPr(get_const_array());
}

// ######################################################################
template <typename T>
matlab_image_t<T>::const_iterator 
    matlab_image_t<T>::end() const
{
  return (begin() + get_size());
}

// ######################################################################
template <typename T>
matlab_image_t<T>::iterator matlab_image_t<T>::beginw()
{
  return mxGetPr(get_array());
}

// ######################################################################
template <typename T>
matlab_image_t<T>::iterator matlab_image_t<T>::endw()
{
  return (beginw() + get_size());
}

// ######################################################################
template <typename T>
bool matlab_image_t<T>::is_initialized() const
{
  return initialized_;
}
// ######################################################################
template <typename T>
mxArray* matlab_image_t<T>::get_array()
{
  return array_;
}
//#######################################################################
template <typename T>
const mxArray* matlab_image_t<T>::get_const_array() const
{
  return array_;
}
//#######################################################################
template <typename T>
matlab_image_t<T> matlab_image_t<T>::operator*=(const double factor)
{
  //ASSERT(isInitialized());
  for (matlab_image_t<T>::iterator ptr = 
              beginw(); 
              ptr != endw(); 
              ++ptr)

    *ptr *= factor;
  return *this;
}
//#######################################################################
////! Check if the coordinates are within the bounds of the Image.
//template <typename T>
//bool matlab_image_t<T>::coords_ok(int x, int y) const
//{
//}
//#######################################################################
//! Returns a pixel value by index.
template <typename T>
T::value_type matlab_image_t<T>::get_val(int index) const
{
    return *(begin()+index);
}
//#######################################################################
//! Returns a pixel value by coordinates.
template <typename T>
T::value_type matlab_image_t<T>::get_val(int r, int c, int d) const
{
  matlab_image_t<T>const_iterator ptr = 
            begin() + (r * get_height()) + c;
  return *ptr;
}
//#######################################################################
//! Sets a pixel value by index.
template <typename T>
void matlab_image_t<T>::set_val(int index, T::value_type val)
{
    *(beginw()+index) = val;
}
//#######################################################################
//! Sets a pixel value by coordinates.
template <typename T>
void matlab_image_t<T>::set_val(int x, int y, int d, T::value_type val)
{
  iterator ptr = beginw() + x * getHeight() + y;
  *ptr = val;
}
//#######################################################################