/*!@file Filters.cpp various filter functions
 */

// This file is part of the SaliencyToolbox - Copyright (C) 2006
// by Dirk Walther and the California Institute of Technology.
// The Saliency Toolbox is released under the GNU General Public 
// License. See the enclosed COPYRIGHT document for details. 
// For more information about this project see: 
// http://www.saliencytoolbox.net

#include "mexLog.h"
#include "Image.h"
#include "Filters.h"

#include <algorithm>
#include <cmath>
#include <limits>


// ########################################################################
// ### Some of this code is modified from the iLab Neuromorphic Vision   ##
// ### C++ Toolkit: http://iLab.usc.edu/toolkit                          ##
// ########################################################################


// ######################################################################
// kernel: 1 5 10 10 5 1
Image lowPass6yDecY(const Image& src)
{
  const int w = src.getWidth(), hs = src.getHeight();
  int hr = hs / 2;
  if (hr == 0) hr = 1;
  
  Image result(w,hr);
  Image::iterator rptr = result.beginw();
  Image::const_iterator sptr = src.begin();
  
  if (hs <= 1)
    result = src;
  else if (hs == 2)
    for (int x = 0; x < w; ++x)
      {
        // use kernel [1 1]^T / 2
        *rptr++ = (sptr[0] + sptr[1]) / 2.0;
        sptr += 2;
      }
  else if (hs == 3)
    for (int x = 0; x < w; ++x)
      {
        // use kernel [1 2 1]^T / 4
        *rptr++ = (sptr[0] + sptr[1] * 2.0 + sptr[2]) / 4.0;
        sptr += 3;
      }
  else // general case with hs >= 4
    for (int x = 0; x < w; ++x)
      {
        // top most point - use kernel [10 10 5 1]^T / 26
        *rptr++ = ((sptr[0] + sptr[1]) * 10.0 + 
                    sptr[2] * 5.0 + sptr[3]) / 26.0;
        //++sptr;
        
        // general case
        int y;
        for (y = 0; y < (hs - 5); y += 2)
          {
            // use kernel [1 5 10 10 5 1]^T / 32
            *rptr++ = ((sptr[1] + sptr[4])  *  5.0 +
                       (sptr[2] + sptr[3])  * 10.0 +
                       (sptr[0] + sptr[5])) / 32.0;
            sptr += 2;
          }
        
        // find out how to treat the bottom most point
        if (y == (hs - 5))
          {
            // use kernel [1 5 10 10 5]^T / 31
             *rptr++ = ((sptr[1] + sptr[4])  *  5.0 +
                        (sptr[2] + sptr[3])  * 10.0 +
                         sptr[0])            / 31.0;
            sptr += 5;
          }
        else
          {
            // use kernel [1 5 10 10]^T / 26
            *rptr++ = ( sptr[0] + sptr[1]  *  5.0 +
                       (sptr[2] + sptr[3]) * 10.0) / 26.0;
            sptr += 4;
          }
        }
            
   return result;      
}

// ######################################################################
// kernel: 1 5 10 10 5 1
Image lowPass6xDecX(const Image& src)
{
  const int ws = src.getWidth(), h = src.getHeight();
  const int h2 = h * 2, h3 = h * 3, h4 = h * 4, h5 = h * 5;
  int wr = ws / 2;
  if (wr == 0) wr = 1;
  
  Image result(wr,h);
  Image::iterator rptr = result.beginw();
  Image::const_iterator sptr = src.begin();

  if (ws <= 1)
    result = src;
  else if (ws == 2)
    for (int y = 0; y < h; ++y)
      {
        // use kernel [1 1] / 2
        *rptr++ = (sptr[0] + sptr[h]) / 2.0;
        ++sptr;
      }
  else if (ws == 3)
    for (int y = 0; y < h; ++y)
      {
        // use kernel [1 2 1] / 4
        *rptr++ = (sptr[0] + sptr[h] * 2.0 + sptr[h2]) / 4.0;
        ++sptr;
      }
  else // general case for ws >= 4
    {
      // left most point - use kernel [10 10 5 1] / 26
      for (int y = 0; y < h; ++y)
        {
          *rptr++ = ((sptr[0] + sptr[h]) * 10.0 + 
                      sptr[h2] * 5.0 + sptr[h3]) / 26.0;
          ++sptr;
        }
      sptr -= h;
      
      // general case
      int x;
      for (x = 0; x < (ws - 5); x += 2)
        {
          for (int y = 0; y < h; ++y)
            {
              // use kernel [1 5 10 10 5 1] / 32
              *rptr++ = ((sptr[h]  + sptr[h4])  *  5.0 +
                         (sptr[h2] + sptr[h3])  * 10.0 +
                         (sptr[0]  + sptr[h5])) / 32.0;
              ++sptr;
            }
          sptr += h;
        }
        
      // find out how to treat the right most point
      if (x == (ws - 5))
        for (int y = 0; y < h; ++y)
          {
            // use kernel [1 5 10 10 5] / 31
            *rptr++ = ((sptr[h]  + sptr[h4])  *  5.0 +
                       (sptr[h2] + sptr[h3])  * 10.0 +
                        sptr[0]) / 31.0;
            ++sptr;
          }
      else
        for (int y = 0; y < h; ++y)
          {
            // use kernel [1 5 10 10] / 26
            *rptr++ = ( sptr[0]  + sptr[h]   * 5.0 + 
                       (sptr[h2] + sptr[h3]) * 10.0) / 26.0;
            ++sptr;
          }
    }
  return result;
}

// ######################################################################
// kernel: 1 3 3 1
Image lowPass4yDecY(const Image& src)
{
  const int w = src.getWidth(), hs = src.getHeight();
  int hr = hs / 2;
  if (hr == 0) hr = 1;
  
  Image result(w,hr);
  Image::iterator rptr = result.beginw();
  Image::const_iterator sptr = src.begin();
  
  if (hs <= 1)
    result = src;
  else if (hs == 2)
    for (int x = 0; x < w; ++x)
      {
        // use kernel [1 1]^T / 2
        *rptr++ = (sptr[0] + sptr[1]) / 2.0;
        sptr += 2;
      }
  else if (hs == 3)
    for (int x = 0; x < w; ++x)
      {
        // use kernel [1 2 1]^T / 4
        *rptr++ = (sptr[0] + sptr[1] * 2.0 + sptr[2]) / 4.0;
        sptr += 3;
      }
  else // general case with hs >= 4
    for (int x = 0; x < w; ++x)
      {
        // top most point - use kernel [3 3 1]^T / 7
        *rptr++ = ((sptr[0] + sptr[1]) * 3.0 + sptr[2]) / 7.0;
        ++sptr;
        
        // general case
        int y;
        for (y = 1; y < (hs - 3); y += 2)
          {
            // use kernel [1 3 3 1]^T / 8
            *rptr++ = ((sptr[1] + sptr[2])  * 3.0 +
                       (sptr[0] + sptr[3])) / 8.0;
            sptr += 2;
          }
        
        // find out how to treat the bottom most point
        if (y == (hs - 3))
          {
            // use kernel [1 3 3]^T / 7
            *rptr++ = (sptr[0] + (sptr[1] + sptr[2]) * 3.0) / 7.0;
            sptr += 3;
          }
        else sptr += 2;
      }
            
   return result;      
}

// ######################################################################
// kernel: 1 3 3 1
Image lowPass4xDecX(const Image& src)
{
  const int ws = src.getWidth(), h = src.getHeight();
  const int h2 = h * 2, h3 = h * 3;
  int wr = ws / 2;
  if (wr == 0) wr = 1;
  
  Image result(wr,h);
  Image::iterator rptr = result.beginw();
  Image::const_iterator sptr = src.begin();

  if (ws <= 1)
    result = src;
  else if (ws == 2)
    for (int y = 0; y < h; ++y)
      {
        // use kernel [1 1] / 2
        *rptr++ = (sptr[0] + sptr[h]) / 2.0;
        ++sptr;
      }
  else if (ws == 3)
    for (int y = 0; y < h; ++y)
      {
        // use kernel [1 2 1] / 4
        *rptr++ = (sptr[0] + sptr[h] * 2.0 + sptr[h2]) / 4.0;
        ++sptr;
      }
  else // general case for ws >= 4
    {
      // left most point - use kernel [3 3 1] / 7
      for (int y = 0; y < h; ++y)
        {
          *rptr++ = ((sptr[0] + sptr[h]) * 3.0 + sptr[h2]) / 7.0;
          ++sptr;
        }
      
      // general case
      int x;
      for (x = 1; x < (ws - 3); x += 2)
        {
          for (int y = 0; y < h; ++y)
            {
              // use kernel [1 3 3 1] / 8
              *rptr++ = ((sptr[h] + sptr[h2])  * 3.0 +
                         (sptr[0] + sptr[h3])) / 8.0;
              ++sptr;
            }
          sptr += h;
        }
        
      // find out how to treat the right most point
      if (x == (ws - 3))
        for (int y = 0; y < h; ++y)
          {
            // use kernel [1 3 3] / 7
            *rptr++ = (sptr[0] + (sptr[h] + sptr[h2]) * 3.0) / 7.0;
            ++sptr;
          }
    }
  return result;
}

// ######################################################################
Image yFilterClean(const Image& src, const mxArray *filter)
{
  double *hFilt = mxGetPr(filter);
  int hfs = mxGetNumberOfElements(filter);
  
  // deliberately mix up w and h to accomodate Matlab storage convention
  const int h = src.getWidth(), w = src.getHeight();

  if (hfs == 0) return src;  // no filter
  Image result(h, w);
  Image::const_iterator sptr = src.begin();
  Image::iterator dptr = result.beginw();

  ASSERT(hfs & 1);
  const int hfs2 = (hfs - 1) / 2;

  // flip the filter to accelerate convolution:
  double *hFilter = new double[hfs]; double sumh = 0.0;
  for (int x = 0; x < hfs; x ++)
    { sumh += hFilt[x]; hFilter[hfs-1-x] = hFilt[x]; }

  // *** horizontal pass ***
  if (w < hfs)  // special function for very small images
    for (int j = 0; j < h; j ++)
      for (int i = 0; i < w; i ++)
        {
          double val = 0.0; double sum = 0.0F;
          for (int k = 0; k < hfs; k ++)
            if (i + k - hfs2 >= 0 && i + k - hfs2 < w)
              {
                val += sptr[k - hfs2] * hFilter[k];
                sum += hFilter[k];
              }
          *dptr++ = val * sumh / sum; sptr ++;
        }
  else  // function for reasonably large images
    for (int jj = 0; jj < h; jj ++)
      {
        // leftmost points:
        for (int j = hfs2; j < hfs - 1; j ++)
          {
            double val = 0.0; double sum = 0.0; int fp = hfs - 1 - j;
            for (int k = 0; k <= j; k ++)
              {
                val += sptr[k] * hFilter[fp + k];
                sum += hFilter[fp + k];
              }
            *dptr++ = val * sumh / sum;
          }
        // bulk (far from the borders):
        for (int i = 0; i < w - hfs + 1; i ++)
          {
            double val = 0.0;
            for (int k = 0; k < hfs; k ++) val += sptr[k] * hFilter[k];
            *dptr++ = val; sptr++;
          }
        // rightmost points:
        for (int j = hfs - 2; j >= hfs2; j --)
          {
            double val = 0.0; double sum = 0.0;
            for (int k = 0; k <= j; k ++)
              {
                val += sptr[k] * hFilter[k];
                sum += hFilter[k];
              }
            *dptr++ = val * sumh / sum;
            sptr++;
          }
        sptr += hfs2;  // sptr back to same as dptr (start of next line)
      }
  delete [] hFilter;
  return result;
}

// ######################################################################
Image xFilterClean(const Image& src, const mxArray *filter)
{
  double *vFilt = mxGetPr(filter);
  int vfs = mxGetNumberOfElements(filter);

  // deliberately mix up w and h to accomodate Matlab storage convention
  const int h = src.getWidth(), w = src.getHeight();

  if (vfs == 0) return src;  // no filter
  Image result(h, w);
  Image::const_iterator sptr = src.begin();
  Image::iterator dptr = result.beginw();

  ASSERT(vfs & 1);
  const int vfs2 = (vfs - 1) / 2;

  // flip the filter to accelerate convolution:
  double *vFilter = new double[vfs]; double sumv = 0.0;
  for (int x = 0; x < vfs; x ++)
    { sumv += vFilt[x]; vFilter[vfs-1-x] = vFilt[x]; }

  int ww[vfs];
  for (int i = 0; i < vfs; i ++) ww[i] = w * i; // speedup precompute

  if (h < vfs)  // special function for very small images
    for (int j = 0; j < h; j ++)
      for (int i = 0; i < w; i ++)
        {
          double val = 0.0; double sum = 0.0;
          for (int k = 0; k < vfs; k ++)
            if (j + k - vfs2 >= 0 && j + k - vfs2 < h)
              {
                val += sptr[w * (k - vfs2)] * vFilter[k];
                sum += vFilter[k];
              }
          *dptr++ = val * sumv / sum; sptr ++;
        }
  else  // function for reasonably large images
    {
      // top points:
      for (int j = vfs2; j < vfs - 1; j ++)
        {
          int fp = vfs - 1 - j;
          for (int i = 0; i < w; i ++)  // scan all points of given horiz
            {
              double val = 0.0; double sum = 0.0;
              for (int k = 0; k <= j; k ++)
                {
                  val += sptr[ww[k]] * vFilter[fp + k];
                  sum += vFilter[fp + k];
                }
              *dptr++ = val * sumv / sum;
              sptr++;
            }
          sptr -= w;   // back to top-left corner
        }
      // bulk (far from edges):
      for (int j = 0; j < h - vfs + 1; j ++)
        for (int i = 0; i < w; i ++)
          {
            double val = 0.0;
            for (int k = 0; k < vfs; k ++) val += sptr[ww[k]] * vFilter[k];
            *dptr++ = val; sptr ++;
          }
      // bottommost points:
      for (int j = vfs - 2; j >= vfs2; j --)
        for (int i = 0; i < w; i ++)
          {
            double val = 0.0; double sum = 0.0;
            for (int k = 0; k <= j; k ++)
              {
                val += sptr[ww[k]] * vFilter[k];
                sum += vFilter[k];
              }
            *dptr++ = val * sumv / sum;
            sptr ++;
          }
    }
  delete [] vFilter;
  return result;
}

// ######################################################################
Image cleanConvolve(const Image& src, const Image& f)
{
  ASSERT(src.isInitialized());

  // width and height are deliberately swapped to conform
  // with Matlab's data storage convention
  const int src_h = src.getWidth();
  const int src_w = src.getHeight();

  Image::const_iterator filter = f.begin();
  const int fil_h = f.getWidth();
  const int fil_w = f.getHeight();

  ASSERT((fil_w & 1) && (fil_h & 1));

  Image result(src_h, src_w);
  Image::const_iterator sptr = src.begin();
  Image::iterator dptr = result.beginw();

  const int fil_end = fil_w * fil_h - 1;
  const int Nx2 = (fil_w - 1) / 2;
  const int Ny2 = (fil_h - 1) / 2;

  const int srow_skip = src_w-fil_w;

  for (int dst_y = 0; dst_y < src_h; ++dst_y)
    {
      // Determine if we're safely inside the image in the y-direction:
      const bool y_clean = dst_y >= Ny2 && dst_y <  (src_h - Nx2);

      for (int dst_x = 0; dst_x < src_w; ++dst_x, ++dptr)
        {
          // Determine if we're safely inside the image in the x-direction:
          const bool x_clean = dst_x >= Nx2 && dst_x <  (src_w - Nx2);

          // Here is where we pick whether we can use the optimized inner
          // loop (in cases where the filter and image patch overlap
          // completely) or whether we must use the inner loop that can
          // handle boundary conditions.

          if (x_clean && y_clean)
            {
              float dst_val = 0.0f;

              Image::const_iterator fptr = filter+fil_end;

              Image::const_iterator srow_ptr =
                sptr + src_w*(dst_y-Nx2) + dst_x - Nx2;

              for (int f_y = 0; f_y < fil_h; ++f_y)
                {
                  for (int f_x = 0; f_x < fil_w; ++f_x)
                    dst_val += (*srow_ptr++) * (*fptr--);

                  srow_ptr += srow_skip;
                }
              *dptr = dst_val;
              continue;
            }
          else
            {
              // OK, we're at an image boundary, so what do we do to make
              // up for the missing pixels? The approach here is to
              // compute the average value of the non-missing pixels, and
              // proceed as if the missing pixels had that value. This
              // minimizes the introduction of edge artifacts e.g. when
              // convolving with an oriented filter.

              float dst_val = 0.0f;
              float src_sum = 0.0f;
              int src_cnt = 0;
              float fil_sum_skipped = 0.0f;

              for (int f_y = 0; f_y < fil_h; ++f_y)
                {
                  const int src_y = f_y + dst_y - Ny2;
                  if (src_y >= 0 && src_y < src_h)
                    {
                      for (int f_x = 0; f_x < fil_w; ++f_x)
                        {
                          const float fil = filter[fil_end - f_x - fil_w*f_y];

                          const int src_x = f_x + dst_x - Nx2;
                          if (src_x >= 0 && src_x < src_w)
                            {
                              const float src_val = sptr[src_x + src_w * src_y];
                              dst_val += src_val * fil;
                              src_sum += src_val;
                              ++src_cnt;
                            }
                          else
                            {
                              fil_sum_skipped += fil;
                            }
                        }
                    }
                  else
                    {
                      for (int f_x = 0; f_x < fil_w; ++f_x)
                        fil_sum_skipped += filter[fil_end - f_x - fil_w*f_y];
                    }
                }
              const float src_avg = src_sum / src_cnt;
              *dptr = dst_val + (fil_sum_skipped * src_avg);
            }
        }
    }
  return result;
}

// ######################################################################
double max(double a, double b)
{ return (a > b) ? a : b; }

// ######################################################################
Image maxX(const Image& src, int kernelSize, int overlap)
{
  double ov(overlap), ker(kernelSize);
  int w = src.getWidth(), h = src.getHeight();
  int rw = (int)ceil(double(w) * ov / kernelSize);
  Image result(rw,h);
  Image::iterator rptr = result.beginw();
  
  for (int x = 0; x < rw; x++)
    {
      int xr1 = (int)floor(double(x)*ker/ov);
      int xr2 = xr1 + kernelSize;
      if (xr2 > w) xr2 = w;
      
      for (int y = 0; y < h; y++)
        {
          *rptr = src.getVal(xr1,y);
          for (int xr = xr1+1; xr < xr2; xr++)
            *rptr = max(*rptr,src.getVal(xr,y));
          
          rptr++;
        }
    }
  return result;
}

// ######################################################################
Image maxY(const Image& src, int kernelSize, int overlap)
{
  double ov(overlap), ker(kernelSize);
  int w = src.getWidth(), h = src.getHeight();
  int rh = (int)ceil(double(h) * ov / kernelSize);
  Image result(w,rh);
  Image::iterator rptr = result.beginw();
  
  for (int x = 0; x < w; x++)
    for (int y = 0; y < rh; y++)
      {
        int yr1 = (int)floor(double(y)*ker/ov);
        int yr2 = yr1 + kernelSize;
        if (yr2 > h) yr2 = h;
        
        *rptr = src.getVal(x,yr1);
      
        for (int yr = yr1+1; yr < yr2; yr++)
          *rptr = max(*rptr,src.getVal(x,yr));
        
        rptr++;
      }

  return result;
}
