/*! @file mexSepFiltClean.cpp 
\verbatim mexSepFiltClean - clean convolution with separable filters.
 result = mexSepFiltClean(data, xFilter, yFilter)
    Convolves data with the separable filters xFilter and
    yFilter. For border elements, the filters are truncated.
    Hence, result has the same size as data.

 See also mexCleanConvolve.
\endverbatim */

// This file is part of the SaliencyToolbox - Copyright (C) 2006
// by Dirk Walther and the California Institute of Technology.
// The Saliency Toolbox is released under the GNU General Public 
// License. See the enclosed COPYRIGHT document for details. 
// For more information about this project see: 
// http://www.saliencytoolbox.net

#include "mexLog.h"
#include "Image.h"
#include "Filters.h"
#include "MexParams.h"

/* the main program */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  MexParams Params(0,1,nlhs,plhs,3,3,nrhs,prhs);
  const Image src(Params.getInput(0));
  
  Image result = xFilterClean(src,Params.getInput(1));
  result = yFilterClean(result,Params.getInput(2));

  Params.setOutput(0,result.getArray());
}
