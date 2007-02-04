#ifndef jpeg_memory_dest_T_H_INCLUDED
#define jpeg_memory_dest_T_H_INCLUDED

#include "alcor.extern/jpeg-c++/jpegStream.h"

class jpeg_memory_dest_t : public jpeg::OStream
{
public:
    /**
     * writes nb_to_write bytes.
     */
    void writeBytes(char* src_buf,int nb_to_write);
};

#endif //jpeg_memory_dest_T_H_INCLUDED