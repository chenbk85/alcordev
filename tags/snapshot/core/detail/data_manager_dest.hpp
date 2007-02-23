#ifdef __cplusplus
extern "C" {
#endif

#include "jpeglib.h"

#ifdef __cplusplus
}
#endif
//-.-----------------------------------------------------------------
#include "alcor/core/core.h"
//-.-----------------------------------------------------------------
namespace all  { namespace core { namespace detail {
  ///
static const int jpeg_COMPRESS_BUFFER_SIZE  = 4096;
//-.-----------------------------------------------------------------
//Expanded data destination object for stdio output 
///
typedef struct {
  struct jpeg_destination_mgr pub; /* public fields */
  
  JOCTET jpg_buf_[jpeg_COMPRESS_BUFFER_SIZE];		//buffer stream 
  boost::shared_array<JOCTET> jpeg_data_out_;
  int jpeg_data_count_;

  JOCTET*                     start_buffer_;		  //start of buffer 
} my_destination_mgr;
//-.-----------------------------------------------------------------
///
typedef my_destination_mgr* my_dest_ptr;
//-.-----------------------------------------------------------------

//-.-----------------------------------------------------------------
/*
 * Initialize destination --- called by jpeg_start_compress
 * before any data is actually written.
 */
//-.-----------------------------------------------------------------
inline METHODDEF(void)
init_destination (j_compress_ptr cinfo)
{
  my_dest_ptr dest = (my_dest_ptr) cinfo->dest;
  if(!dest->jpeg_data_out_.get())
  {
    //more than necessary...
    dest->jpeg_data_out_.reset(new JOCTET[cinfo->image_height*cinfo->image_width]);
  }
  dest->start_buffer_ = 
    (JOCTET*) dest->jpg_buf_; 

  dest->jpeg_data_count_ = 0;

  dest->pub.next_output_byte  = dest->start_buffer_;
  dest->pub.free_in_buffer    = jpeg_COMPRESS_BUFFER_SIZE;
}
//-.-----------------------------------------------------------------
inline METHODDEF(boolean)
empty_output_buffer (j_compress_ptr cinfo)
{
  my_dest_ptr dest = (my_dest_ptr) cinfo->dest;
  
  size_t datacount = jpeg_COMPRESS_BUFFER_SIZE 
                     - dest->pub.free_in_buffer;
  
  //
  memcpy(&dest->jpeg_data_out_[dest->jpeg_data_count_], 
         dest->start_buffer_, 
         datacount);

  dest->jpeg_data_count_ += datacount;

  dest->pub.next_output_byte = dest->start_buffer_;
  dest->pub.free_in_buffer = cinfo->image_height*cinfo->image_width;

  return TRUE;
}
//-.-----------------------------------------------------------------
/*
 * Terminate destination --- called by jpeg_finish_compress
 * after all data has been written.  Usually needs to flush buffer.
 *
 * NB: *not* called by jpeg_abort or jpeg_destroy; surrounding
 * application must deal with any cleanup that should happen even
 * for error exit.
 */

inline METHODDEF(void)
term_destination (j_compress_ptr cinfo)
{
  my_dest_ptr dest = (my_dest_ptr) cinfo->dest;
  size_t datacount = jpeg_COMPRESS_BUFFER_SIZE - dest->pub.free_in_buffer;

  dest->jpeg_data_count_ += datacount;
  
  ///SCrivi!!
  ///
  memcpy(&dest->jpeg_data_out_[dest->jpeg_data_count_], 
         dest->start_buffer_, 
         datacount);

  dest->jpeg_data_count_ += datacount;

  dest->pub.next_output_byte  = dest->start_buffer_;
  dest->pub.free_in_buffer    = jpeg_COMPRESS_BUFFER_SIZE;
}
//-.-----------------------------------------------------------------
}}}//namespaces