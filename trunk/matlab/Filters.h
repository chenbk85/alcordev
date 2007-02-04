/*!@file Filters.h various filter functions
 */

// This file is part of the SaliencyToolbox - Copyright (C) 2006
// by Dirk Walther and the California Institute of Technology.
// The Saliency Toolbox is released under the GNU General Public 
// License. See the enclosed COPYRIGHT document for details. 
// For more information about this project see: 
// http://www.saliencytoolbox.net

#ifndef FILTERS_H_DEFINED
#define FILTERS_H_DEFINED

#include <mex.h>


// ########################################################################
// ### Some of this code is modified from the iLab Neuromorphic Vision   ##
// ### C++ Toolkit: http://iLab.usc.edu/toolkit                          ##
// ########################################################################


class Image;

//! Low-pass with a 6x6 separable kernel in Y, and Y decimated by 2
Image lowPass6yDecY(const Image& src);

//! Low-pass with a 6x6 separable kernel in X, and X decimated by 2
Image lowPass6xDecX(const Image& src);

//! Low-pass with a 4x4 separable kernel in Y, and Y decimated by 2
Image lowPass4yDecY(const Image& src);

//! Low-pass with a 4x4 separable kernel in X, and X decimated by 2
Image lowPass4xDecX(const Image& src);

//! Separable filter convolution in Y with clean boundary
Image yFilterClean(const Image& src, const mxArray *filter);

//! Separable filter convolution in X with clean boundary
Image xFilterClean(const Image& src, const mxArray *filter);

//! 2D convolution of src with f as a filter, with clean boundary
Image cleanConvolve(const Image& src, const Image& f);

//! Applies a max kernel along the x axis
Image maxX(const Image& src, int kernelSize, int overlap);

//! Applies a max kernel along the y axis
Image maxY(const Image& src, int kernelSize, int overlap);

//! Returns the max of a and b
double max(double a, double b);

#endif
