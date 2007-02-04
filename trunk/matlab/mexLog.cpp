/*!@file mexLog.cpp a few helpful functions for logging in mex files.
 */

// This file is part of the SaliencyToolbox - Copyright (C) 2006
// by Dirk Walther and the California Institute of Technology.
// The Saliency Toolbox is released under the GNU General Public 
// License. See the enclosed COPYRIGHT document for details. 
// For more information about this project see: 
// http://www.saliencytoolbox.net

#include "mexLog.h"
#include <cstdarg>
#include <cstdio>

#define BUFMAXSIZE 1000

// ######################################################################
void mexInfo(const char *fmt,...)
{
  char buf[BUFMAXSIZE];
  va_list ap;
  va_start(ap,fmt);
  vsnprintf(buf,BUFMAXSIZE,fmt,ap);
  va_end(ap);
  mexPrintf("%s: %s\n",mexFunctionName(),buf);
}

// ######################################################################
void mexError(const char *fmt,...)
{
  char buf[BUFMAXSIZE];
  va_list ap;
  va_start(ap,fmt);
  vsnprintf(buf,BUFMAXSIZE,fmt,ap);
  va_end(ap);
  mexPrintf("Error in %s: %s\n",mexFunctionName(),buf);
}

// ######################################################################
void mexFatal(const char *fmt,...)
{
  char buf[BUFMAXSIZE];
  va_list ap;
  va_start(ap,fmt);
  vsnprintf(buf,BUFMAXSIZE,fmt,ap);
  va_end(ap);
  mexErrMsgTxt(buf);
}

// ######################################################################
void mexDebug(const char *fmt,...)
{
  char buf[BUFMAXSIZE];
  va_list ap;
  va_start(ap,fmt);
  vsnprintf(buf,BUFMAXSIZE,fmt,ap);
  va_end(ap);
  mexPrintf("%s-debug: %s\n",mexFunctionName(),buf);
}
