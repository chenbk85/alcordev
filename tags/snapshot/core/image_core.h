#ifndef image_core_H_INCLUDED
#define image_core_H_INCLUDED

#include "alcor/core/core.h"

typedef struct rgb_image_buffer_t
{
   // The number of rows in the image.
   int 		     nrows;
   // The number of columns in the image.
   int 		     ncols;
   // The row increment of the image.
   int		     rowinc;
   // The pixel data for red band of the image.
   unsigned char*    red;
   // The pixel data for green band of the image.
   unsigned char*    green;
   // The pixel data for blue band of the image.
   unsigned char*    blue;

} color_image_t;

#endif //image_core_H_INCLUDED