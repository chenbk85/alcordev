#ifndef opencv_utils_HPP_INCLUDED
#define opencv_utils_HPP_INCLUDED
//-------------------------------------------------------------------
#include "alcor/core/core.h"
#include "alcor/core/image_utils.h"
//-------------------------------------------------------------------
#include "cxcore.h"
#include "cv.h"
//-------------------------------------------------------------------
namespace all { namespace core {
//-------------------------------------------------------------------
  ///this from:
  ///http://www.site.uottawa.ca/~laganier/tutorial/opencv+directshow/cvision.htm
  ///IplImageIterator<unsigned char> it(image);
//    while (!it) {
// 
//     pixel= &it;
//     if (pixel[1]>pixel[0] && pixel[1]>pixel[2]) {
// 
//         pixel[0]= 0xFF;   // 255
//         pixel[1]= 0xFF;   // 255
//         pixel[2]= 0xFF;   // 255
//     }
//     it+= 3;
//  }

  template <class PEL>
class IplImageIterator {
  int i, i0,j;
  PEL* data;
  PEL* pix;
  int step;
  int nl, nc;
  int nch;
 
 public:
 
  /* constructor */
  IplImageIterator(IplImage* image,
     int x=0, int y=0, int dx= 0, int dy=0) :
       i(x), j(y), i0(0) {
    
    data= reinterpret_cast<PEL*>(image->imageData);
    step= image->widthStep / sizeof(PEL);
 
    nl= image->height;
    if ((y+dy)>0 && (y+dy)<nl) nl= y+dy;
    if (y<0) j=0;
    data+= step*j;

 
    nc= image->width ;
    if ((x+dx)>0 && (x+dx)<nc) nc= x+dx;
    nc*= image->nChannels;
    if (x>0) i0= x*image->nChannels;
    i= i0;
 
    nch= image->nChannels;
    pix= new PEL[nch];}

 
  /* has next ? */
  bool operator!() const { return j < nl; }
 
  /* next pixel */
  IplImageIterator& operator++() {i++;
    if (i >= nc) { i=i0; j++; data+= step; }
    return *this;}
  IplImageIterator& operator+=(int s) {i+=s;
    if (i >= nc) { i=i0; j++; data+= step; }
    return *this;}
  /* pixel access */
  PEL& operator*() { return data[i]; }
  const PEL operator*() const { return data[i]; }
  const PEL neighbor(int dx, int dy) const
    { return *(data+dy*step+i+dx); }
  PEL* operator&() const { return data+i; }
  /* current pixel coordinates */
  int column() const { return i/nch; }
  int line() const { return j; }
};

//=============================================================================
//=============================================================================
static size_t size_from_ipldepth(int depth)
{
  //
  size_t size_;

  //assign correct type id
  switch (depth)
  {
  case IPL_DEPTH_8U:
    size_ = core::traits<core::uint8_t>::size;
    break;

  case IPL_DEPTH_32F:
    size_ = core::traits<core::single_t>::size;
    break;

  case IPL_DEPTH_16S:
    size_ = core::traits<core::int16_t>::size;
    break;

  case IPL_DEPTH_16U:
    //uhm
    size_ = core::traits<core::int16_t>::size;
    break;

  case IPL_DEPTH_64F:
    size_ = core::traits<core::double_t>::size;
    break;

  default:
    size_ = core::traits<uint8_t>::size;
    break;
  }
  return size_;
}
//=============================================================================
//=============================================================================
  ///some handy routines
  struct ocv
  {

//=============================================================================
    static void release_iplimage(IplImage* p_image)
    {
      if (p_image!=0) cvReleaseImage(&p_image);
    }
//=============================================================================
    //
    typedef boost::shared_ptr<IplImage> iplimage_ptr;
//=============================================================================
    static all::core::uint8_sarr iplimage_to_planar(IplImage* p_image)
    {
      //-----------------------------------------------------------------------
      all::core::uint8_sarr p_buffer(new all::core::uint8_t[p_image->imageSize]);
      //-----------------------------------------------------------------------
      memcpy(p_buffer.get(), p_image->imageData, p_image->imageSize);
      //
      change_ordering::to_planar(p_buffer, p_image->height, p_image->width, p_image->nChannels);
      //
      return p_buffer;
    }
//=============================================================================
  };
//-------------------------------------------------------------------
}}
//-------------------------------------------------------------------
#endif //opencv_utils_HPP_INCLUDED